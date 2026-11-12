#include "stdafx.h"
#include "PlayerGrowthEffect.h"
#include <algorithm>
#include <cmath>

void PlayerGrowthEffect::Init(int level, int tankClass)
{
    m_lastLevel = level;
    m_lastClass = tankClass;
    for (auto& part : m_parts)
    {
        ModelInitData data;
        data.m_tkmFilePath = "Assets/modelData/tank/Bullet.tkm";
        data.m_fxFilePath = "Assets/shader/PlayerGrowthEffect.fx";
        data.m_modelUpAxis = enModelUpAxisY;
        data.m_alphaBlendMode = AlphaBlendMode_Trans;
        data.m_isDepthWrite = false;
        data.m_isDepthTest = true;
        data.m_colorBufferFormat[0] = DXGI_FORMAT_R16G16B16A16_FLOAT;
        data.m_expandConstantBuffer = &part.parameters;
        data.m_expandConstantBufferSize = sizeof(part.parameters);
        part.model.Init(data);
    }
    m_initialized = true;
}

void PlayerGrowthEffect::Trigger(bool evolution)
{
    // Keep the stronger evolution cue when both changes happen together.
    if (!evolution && m_active && m_evolution)
    {
        return;
    }
    m_active = true;
    m_evolution = evolution;
    m_age = 0.0f;
    m_duration = evolution ? 0.95f : 0.65f;
}

void PlayerGrowthEffect::Observe(int level, int tankClass)
{
    if (!m_initialized)
    {
        return;
    }
    if (tankClass != m_lastClass)
    {
        Trigger(true);
    }
    else if (level > m_lastLevel)
    {
        Trigger(false);
    }
    m_lastLevel = level;
    m_lastClass = tankClass;
}

void PlayerGrowthEffect::Update(float deltaTime, bool alive)
{
    if (!alive)
    {
        m_active = false;
    }
    if (m_active)
    {
        m_age += deltaTime;
        if (m_age >= m_duration)
        {
            m_active = false;
        }
    }
}

void PlayerGrowthEffect::Draw(const Vector3& position, float levelScale)
{
    for (auto& part : m_parts)
    {
        part.visible = false;
    }
    if (!m_initialized || !m_active)
    {
        return;
    }
    const Vector3 tint = m_evolution ? Vector3(1.0f, 0.72f, 0.16f) :
        Vector3(0.15f, 0.82f, 1.0f);
    const float progress = m_age / m_duration;
    const float fade = (1.0f - progress) * (1.0f - progress);
    for (int i = 0; i < PART_COUNT; ++i)
    {
        auto& part = m_parts[i];
        Vector3 size;
        float opacity;
        if (i < 2)
        {
            if (i == 1 && (!m_evolution || m_age < 0.12f))
            {
                continue;
            }
            const float ringProgress = (m_age - i * 0.12f) / m_duration;
            const float radius = (22.0f + ringProgress * (m_evolution ? 58.0f : 40.0f)) * levelScale;
            part.position = position + Vector3(0.0f, (2.0f + i) * levelScale, 0.0f);
            size = Vector3(radius / 6.0f, 0.08f * levelScale, radius / 6.0f);
            part.parameters.mode.x = 1.0f;
            opacity = 0.65f * fade;
        }
        else
        {
            const float angle = (i - 2) * 6.2831853f / 12.0f + m_age * 0.8f;
            const float radius = (23.0f + progress * (m_evolution ? 30.0f : 20.0f)) * levelScale;
            const float height = (22.0f + std::sin(progress * 3.14159265f) * 24.0f + (i % 3) * 4.0f) * levelScale;
            part.position = position + Vector3(std::cos(angle) * radius, height, std::sin(angle) * radius);
            const float sparkleSize = (0.18f + (i % 2) * 0.05f) * levelScale;
            size = Vector3(sparkleSize, sparkleSize, sparkleSize);
            part.parameters.mode.x = 0.0f;
            opacity = 0.8f * fade;
        }
        part.parameters.color = Vector4(tint.x, tint.y, tint.z, opacity);
        part.model.UpdateWorldMatrix(part.position, Quaternion::Identity, size);
        part.visible = true;
    }
    g_renderingEngine->AddRenderObject(this);
}

void PlayerGrowthEffect::OnTlanslucentRender(RenderContext& rc)
{
    std::array<Part*, PART_COUNT> visible;
    int count = 0;
    for (auto& part : m_parts)
    {
        if (part.visible)
        {
            visible[count++] = &part;
        }
    }
    const Vector3 eye = g_camera3D->GetPosition();
    const auto distanceSquared = [&](const Part* part)
    {
        const Vector3 offset = part->position - eye;
        return offset.x * offset.x + offset.y * offset.y + offset.z * offset.z;
    };
    std::sort(visible.begin(), visible.begin() + count,
        [&](const Part* a, const Part* b)
        {
            return distanceSquared(a) > distanceSquared(b);
        });
    for (int i = 0; i < count; ++i)
    {
        visible[i]->model.Draw(rc);
    }
}