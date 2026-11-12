#include "stdafx.h"
#include "MuzzleSmoke.h"
#include <algorithm>
#include <cmath>

void MuzzleSmoke::Init()
{
    for (auto& particle : m_particles)
    {
        ModelInitData data;
        data.m_tkmFilePath = "Assets/modelData/tank/Bullet.tkm";
        data.m_fxFilePath = "Assets/shader/MuzzleSmoke.fx";
        data.m_modelUpAxis = enModelUpAxisY;
        data.m_alphaBlendMode = AlphaBlendMode_Trans;
        data.m_isDepthWrite = false;
        data.m_isDepthTest = true;
        data.m_colorBufferFormat[0] = DXGI_FORMAT_R16G16B16A16_FLOAT;
        data.m_expandConstantBuffer = &particle.color;
        data.m_expandConstantBufferSize = sizeof(particle.color);
        particle.model.Init(data);
    }
    m_initialized = true;
}

void MuzzleSmoke::Emit(const Vector3& position, const Vector3& inputDirection, float scale)
{
    if (!m_initialized || scale <= 0.0f)
    {
        return;
    }
    Vector3 direction(inputDirection.x, 0.0f, inputDirection.z);
    if (direction.x * direction.x + direction.z * direction.z < 0.0001f)
    {
        return;
    }
    direction.Normalize();
    const Vector3 right(direction.z, 0.0f, -direction.x);
    int emitted = 0;
    for (auto& particle : m_particles)
    {
        if (particle.remaining > 0.0f)
        {
            continue;
        }
        // A private deterministic sequence leaves gameplay randomness untouched.
        const float spread = std::sin(static_cast<float>(m_sequence++ % 4096) * 2.4f);
        particle.lifetime = 0.30f + 0.06f * emitted;
        particle.remaining = particle.lifetime;
        particle.scale = scale;
        particle.position = position + direction * (2.0f * scale) +
            right * (spread * 1.5f * scale);
        particle.velocity = direction * ((18.0f + emitted * 8.0f) * scale) +
            right * (spread * 7.0f * scale);
        particle.velocity.y = (10.0f + emitted * 4.0f) * scale;
        particle.color.w = 0.28f;
        const float size = 0.55f * scale;
        particle.model.UpdateWorldMatrix(particle.position, Quaternion::Identity,
            Vector3(size, size, size));
        if (++emitted == 2)
        {
            break;
        }
    }
}

void MuzzleSmoke::Update(float deltaTime)
{
    for (auto& particle : m_particles)
    {
        if (particle.remaining <= 0.0f)
        {
            continue;
        }
        particle.remaining -= deltaTime;
        if (particle.remaining <= 0.0f)
        {
            particle.remaining = 0.0f;
            continue;
        }
        particle.position += particle.velocity * deltaTime;
        particle.velocity *= std::exp(-3.0f * deltaTime);
        const float remaining = particle.remaining / particle.lifetime;
        particle.color.w = 0.28f * remaining * remaining;
        const float size = (0.55f + (1.0f - remaining) * 1.15f) * particle.scale;
        particle.model.UpdateWorldMatrix(particle.position, Quaternion::Identity,
            Vector3(size, size, size));
    }
}

void MuzzleSmoke::Draw()
{
    for (const auto& particle : m_particles)
    {
        if (particle.remaining > 0.0f)
        {
            g_renderingEngine->AddRenderObject(this);
            return;
        }
    }
}

void MuzzleSmoke::OnTlanslucentRender(RenderContext& rc)
{
    std::array<Particle*, MAX_PARTICLES> visible;
    int count = 0;
    for (auto& particle : m_particles)
    {
        if (particle.remaining > 0.0f)
        {
            visible[count++] = &particle;
        }
    }
    const Vector3 eye = g_camera3D->GetPosition();
    const auto distanceSquared = [&](const Particle* particle)
    {
        const Vector3 offset = particle->position - eye;
        return offset.x * offset.x + offset.y * offset.y + offset.z * offset.z;
    };
    std::sort(visible.begin(), visible.begin() + count,
        [&](const Particle* a, const Particle* b)
        {
            return distanceSquared(a) > distanceSquared(b);
        });
    for (int i = 0; i < count; ++i)
    {
        visible[i]->model.Draw(rc);
    }
}