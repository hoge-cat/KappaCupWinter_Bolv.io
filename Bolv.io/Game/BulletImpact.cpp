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

void BulletImpact::Spawn(const Vector3& position, float scale, bool enemyShot)
{
    if (!m_initialized || !std::isfinite(scale) || scale <= 0) { return; }
    scale = (std::min)(scale, 2.0f);
    for (const auto& burst : m_bursts)
    {
        const Vector3 offset = burst.origin - position;
        const float distance = offset.x * offset.x + offset.y * offset.y + offset.z * offset.z;
        if (burst.age < 0.075f && distance < 100 * scale * scale) { return; }
    }
    for (auto& burst : m_bursts)
    {
        if (burst.age < DURATION) { continue; }
        burst.origin = position;
        burst.age = 0;
        burst.scale = scale;
        burst.enemyShot = enemyShot;
        const float phase = static_cast<float>(m_sequence++ % 1024) * 2.4f;
        for (int i = 0; i < PARTS; ++i)
        {
            auto& part = burst.parts[i];
            part.position = position;
            const float angle = phase + i * 1.256637f;
            part.velocity = i == 0 ? Vector3::Zero :
                Vector3(std::cos(angle) * (40 + i * 8), 24 + i * 7.0f, std::sin(angle) * (40 + i * 8)) * scale;
        }
        Place(burst);
        return;
    }
    // Saturation drops only visual feedback, never the underlying hit.
}

void BulletImpact::Place(Burst& burst)
{
    const float fade = 1.0f - burst.age / DURATION;
    for (int i = 0; i < PARTS; ++i)
    {
        auto& part = burst.parts[i];
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
        if (burst.age >= DURATION) { continue; }
        burst.age += deltaTime;
        if (burst.age >= DURATION) { continue; }
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
        if (burst.age < DURATION) { g_renderingEngine->AddRenderObject(this); return; }
    }
}

void BulletImpact::OnTlanslucentRender(RenderContext& rc)
{
    std::array<Part*, BURST_COUNT * PARTS> visible;
    int count = 0;
    for (auto& burst : m_bursts)
    {
        if (burst.age >= DURATION) { continue; }
        for (auto& part : burst.parts) { visible[count++] = &part; }
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
