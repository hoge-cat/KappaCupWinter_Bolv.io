#pragma once
#include <array>

// Scene-owned, bounded cosmetic pool. Collision and damage remain in the bullets.
class BulletImpact : public IGameObject, public IRenderer
{
public:
    bool Start();
    void Update();
    void Render(RenderContext& rc);
    void OnTlanslucentRender(RenderContext& rc) override;
    static void Emit(const Vector3& position, float scale, bool enemyShot);
    static void EmitDestruction(const Vector3& position, float scale, const Vector4& color);

    template<class HitTest>
    static void AtHit(const Vector3& start, const Vector3& end, float scale, bool enemyShot, HitTest hit)
    {
        // Locate the first contact using the existing, read-only collision predicate.
        float low = 0, high = 1;
        for (int i = 0; i < 6; ++i)
        {
            const float middle = (low + high) * 0.5f;
            if (hit(start + (end - start) * middle)) { high = middle; }
            else { low = middle; }
        }
        Emit(start + (end - start) * high, scale, enemyShot);
    }

private:
    static constexpr int BURST_COUNT = 24;
    static constexpr int PARTS = 12;
    struct Part
    {
        Model model;
        Vector4 color = Vector4(1, 1, 1, 0);
        Vector3 position = Vector3::Zero;
        Vector3 velocity = Vector3::Zero;
    };
    struct Burst
    {
        std::array<Part, PARTS> parts;
        Vector3 origin = Vector3::Zero;
        float age = 1;
        float scale = 1;
        bool enemyShot = false;
        bool destruction = false;
        float duration = 0.28f;
        int count = 6;
        Vector4 tint = Vector4(1, 1, 1, 1);
    };
    std::array<Burst, BURST_COUNT> m_bursts;
    unsigned int m_sequence = 0;
    bool m_initialized = false;
    void Spawn(const Vector3& position, float scale, bool enemyShot, bool destruction = false, const Vector4& tint = Vector4(1, 1, 1, 1));
    void Place(Burst& burst);
};
