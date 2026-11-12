#include "stdafx.h"
#include "BulletImpact.h"
#include <algorithm>
#include <cmath>

namespace { constexpr float DURATION = 0.28f; }

bool BulletImpact::Start()
{
    for (auto& burst : m_bursts)
    {
        for (auto& part : burst.parts)
        {
            ModelInitData data;
            data.m_tkmFilePath = "Assets/modelData/tank/Bullet.tkm";
            data.m_fxFilePath = "Assets/shader/MuzzleSmoke.fx";
            data.m_modelUpAxis = enModelUpAxisY;
            data.m_alphaBlendMode = AlphaBlendMode_Trans;
            data.m_isDepthWrite = false;
            data.m_isDepthTest = true;
            data.m_colorBufferFormat[0] = DXGI_FORMAT_R16G16B16A16_FLOAT;
            data.m_expandConstantBuffer = &part.color;
            data.m_expandConstantBufferSize = sizeof(part.color);
            part.model.Init(data);
        }
    }
    m_initialized = true;
    return true;
}

void BulletImpact::Emit(const Vector3& position, float scale, bool enemyShot)
{
    if (auto* effect = FindGO<BulletImpact>("bulletImpact"))
    {
        effect->Spawn(position, scale, enemyShot);
    }
}

void BulletImpact::EmitDestruction(const Vector3& position, float scale, const Vector4& color)
{
    if (auto* effect = FindGO<BulletImpact>("bulletImpact"))
    {
        effect->Spawn(position, scale, false, true, color);
    }
}

void BulletImpact::Spawn(const Vector3& position, float scale, bool enemyShot, bool destruction, const Vector4& tint)
{
    if (!m_initialized || !std::isfinite(scale) || scale <= 0) { return; }
    scale = (std::min)(scale, 2.0f);
    if (!destruction)
    {
        for (const auto& burst : m_bursts)
        {
            const Vector3 offset = burst.origin - position;
            const float distance = offset.x * offset.x + offset.y * offset.y + offset.z * offset.z;
            if (!burst.destruction && burst.age < 0.075f && distance < 100 * scale * scale) { return; }
        }
    }
    Burst* available = nullptr;
    for (auto& burst : m_bursts)
    {
        if (burst.age >= burst.duration) { available = &burst; break; }
    }
    // Give destruction priority over an old impact when the visual pool is full.
    if (!available && destruction)
    {
        for (auto& burst : m_bursts)
        {
            if (!burst.destruction && (!available || burst.age > available->age)) { available = &burst; }
        }
    }
    if (!available) { return; }
    auto& burst = *available;
    burst.origin = position;
    burst.age = 0;
    burst.scale = scale;
    burst.enemyShot = enemyShot;
    burst.destruction = destruction;
    burst.duration = destruction ? 0.65f : DURATION;
    burst.count = destruction ? PARTS : 6;
    burst.tint = tint;
    const float phase = static_cast<float>(m_sequence++ % 1024) * 2.4f;
    for (int i = 0; i < burst.count; ++i)
    {
        auto& part = burst.parts[i];
        part.position = position;
        const float angle = phase + i * (destruction ? 0.785398f : 1.256637f);
        const float speed = destruction ? 65.0f + i % 4 * 18 : 40.0f + i * 8;
        part.velocity = i == 0 ? Vector3::Zero :
            Vector3(std::cos(angle) * speed, 24 + i * 7.0f, std::sin(angle) * speed) * scale;
        if (destruction && i >= 9) { part.velocity *= 0.25f; }
    }
    Place(burst);
}
void BulletImpact::Place(Burst& burst)
{
    const float fade = 1.0f - burst.age / burst.duration;
    for (int i = 0; i < burst.count; ++i)
    {
        auto& part = burst.parts[i];
        if (burst.destruction)
        {
            const float progress = 1.0f - fade;
            float size;
            if (i == 0)
            {
                part.color = Vector4(1, 0.95f, 0.78f, 0.65f * fade * fade * fade);
                size = (2.0f + progress * 12.0f) * burst.scale;
            }
            else if (i >= 9)
            {
                part.color = Vector4(0.72f, 0.73f, 0.75f, 0.18f * fade * fade);
                size = (3.0f + progress * 7.0f) * burst.scale;
            }
            else
            {
                part.color = Vector4(burst.tint.x, burst.tint.y, burst.tint.z, fade * 0.95f);
                size = (1.5f + i % 3 * 0.4f) * (0.4f + fade * 0.6f) * burst.scale;
            }
            const float height = i > 0 && i < 9 ? 0.65f : 1.0f;
            part.model.UpdateWorldMatrix(part.position, Quaternion::Identity, Vector3(size, size * height, size));
            continue;
        }
        if (i == 0)
        {
            part.color = Vector4(1.0f, 0.96f, 0.75f, fade * fade * 0.85f);
        }
        else
        {
            part.color = burst.enemyShot ? Vector4(1, 0.48f, 0.32f, fade * 0.9f) :
                Vector4(0.42f, 0.87f, 1, fade * 0.9f);
        }
        const float size = (i == 0 ? 1.7f * fade + 0.4f : 0.45f * fade + 0.08f) * burst.scale;
        part.model.UpdateWorldMatrix(part.position, Quaternion::Identity, Vector3(size, size, size));
    }
}

void BulletImpact::Update()
{
    const float deltaTime = (std::max)(0.0f, g_gameTime->GetFrameDeltaTime());
    for (auto& burst : m_bursts)
    {
        if (burst.age >= burst.duration) { continue; }
        burst.age += deltaTime;
        if (burst.age >= burst.duration) { continue; }
        for (auto& part : burst.parts)
        {
            part.position += part.velocity * deltaTime;
            part.velocity.y -= 160.0f * burst.scale * deltaTime;
        }
        Place(burst);
    }
}

void BulletImpact::Render(RenderContext& rc)
{
    (void)rc;
    for (const auto& burst : m_bursts)
    {
        if (burst.age < burst.duration) { g_renderingEngine->AddRenderObject(this); return; }
    }
}

void BulletImpact::OnTlanslucentRender(RenderContext& rc)
{
    std::array<Part*, BURST_COUNT * PARTS> visible;
    int count = 0;
    for (auto& burst : m_bursts)
    {
        if (burst.age >= burst.duration) { continue; }
        for (int i = 0; i < burst.count; ++i) { visible[count++] = &burst.parts[i]; }
    }
    const Vector3 eye = g_camera3D->GetPosition();
    const auto distance = [&](const Part* part)
    {
        const Vector3 offset = part->position - eye;
        return offset.x * offset.x + offset.y * offset.y + offset.z * offset.z;
    };
    std::sort(visible.begin(), visible.begin() + count,
        [&](const Part* a, const Part* b) { return distance(a) > distance(b); });
    for (int i = 0; i < count; ++i) { visible[i]->model.Draw(rc); }
}
