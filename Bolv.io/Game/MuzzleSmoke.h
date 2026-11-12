#pragma once
#include <array>

// A bounded, visual-only particle pool owned by the player.
class MuzzleSmoke : public IRenderer
{
public:
    void Init();
    void Emit(const Vector3& position, const Vector3& direction, float scale);
    void Update(float deltaTime);
    void Draw();
    void OnTlanslucentRender(RenderContext& rc) override;

private:
    static constexpr int MAX_PARTICLES = 48;
    struct Particle
    {
        Model model;
        Vector4 color = Vector4(0.68f, 0.70f, 0.72f, 0.0f);
        Vector3 position = Vector3::Zero;
        Vector3 velocity = Vector3::Zero;
        float remaining = 0.0f;
        float lifetime = 0.0f;
        float scale = 1.0f;
    };
    std::array<Particle, MAX_PARTICLES> m_particles;
    unsigned int m_sequence = 0;
    bool m_initialized = false;
};