#pragma once
#include <array>

// Cosmetic feedback only: observes committed progression without changing it.
class PlayerGrowthEffect : public IRenderer
{
public:
    void Init(int level, int tankClass);
    void Observe(int level, int tankClass);
    void Update(float deltaTime, bool alive);
    void Draw(const Vector3& position, float levelScale);
    void OnTlanslucentRender(RenderContext& rc) override;

private:
    void Trigger(bool evolution);
    struct Part
    {
        Model model;
        struct Parameters
        {
            Vector4 color = Vector4(1.0f, 1.0f, 1.0f, 0.0f);
            Vector4 mode = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
        } parameters;
        Vector3 position = Vector3::Zero;
        bool visible = false;
    };
    static constexpr int PART_COUNT = 14; // Two rings and twelve light motes.
    std::array<Part, PART_COUNT> m_parts;
    int m_lastLevel = 1;
    int m_lastClass = 0;
    float m_age = 0.0f;
    float m_duration = 0.65f;
    bool m_active = false;
    bool m_evolution = false;
    bool m_initialized = false;
};