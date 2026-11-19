#include "stdafx.h"
#include "ShapeBox.h"
#include "BulletImpact.h"
#include "Player.h"

#include <cmath>
#include <random>


namespace
{
    struct ShapeParameters
    {
        int hp;
        int experience;
        int bodyDamage;
        float radius;
        float scale;
        float height;
        Vector4 color;
    };
    // 通常図形の基礎値。スコア表示は既存どおり経験値の10倍。
    const ShapeParameters SHAPES[] =
    {
        { 10, 10, 8, 48.0f, 1.6f, 48.0f, Vector4(0.98f, 0.81f, 0.30f, 1) },
        { 30, 25, 8, 40.0f, 1.0f, 60.0f, Vector4(0.96f, 0.31f, 0.31f, 1) },
        { 100, 130, 12, 52.0f, 1.0f, 60.0f, Vector4(0.32f, 0.51f, 0.94f, 1) }
    };
    static_assert(sizeof(SHAPES) / sizeof(SHAPES[0]) == static_cast<int>(ShapeBox::Type::Count),
        "Shape parameters must match Type");
    const ShapeParameters& Parameters(ShapeBox::Type type)
    {
        return SHAPES[static_cast<int>(type)];
    }

    // ============================================
    // ShapeBoxの当たり判定範囲
    // ============================================

    // X方向。
    // 左右が少しめり込んでいたので
    // 少しだけ広げる。
    const float COLLISION_HALF_X =
        32.0f;


    // 高さ。
    const float COLLISION_HEIGHT =
        48.0f;


    // ============================================
    // Z方向
    // ============================================

    // 初期カメラ側(-Z側)。
    const float COLLISION_MIN_Z =
        -14.0f;


    // 反対側(+Z側)。
    // 56.0fで見た目と合ったので固定。
    const float COLLISION_MAX_Z =
        56.0f;
}


// ============================================================
// Start
// ============================================================


void ShapeBox::SetType(Type type)
{
    if (static_cast<int>(type) < 0 || type >= Type::Count) { return; }
    m_type = type;
    m_hp = GetMaxHP();
    m_respawnTimer = 0.0f;
    m_pushVelocity = Vector3::Zero;
}

int ShapeBox::GetMaxHP() const { return Parameters(m_type).hp; }
int ShapeBox::GetExperienceReward() const { return Parameters(m_type).experience; }
int ShapeBox::GetBodyDamageValue() const { return Parameters(m_type).bodyDamage; }
float ShapeBox::GetCollisionRadius() const { return Parameters(m_type).radius; }
float ShapeBox::GetRenderScale() const { return Parameters(m_type).scale; }
float ShapeBox::GetHeight() const { return Parameters(m_type).height; }
Vector4 ShapeBox::GetColor() const { return Parameters(m_type).color; }

// 新しいモデルは原点を中心とする正多角柱。逆回転して各辺の内側を判定する。
bool ShapeBox::IsPolygonHit(const Vector3& start, const Vector3& end, float radius) const
{
    const Vector3 origin = GetCenter();
    const Vector3 a = ToCollisionSpace(start) - origin;
    const Vector3 b = ToCollisionSpace(end) - origin;
    const Vector3 movement = b - a;
    float enter = 0.0f, exit = 1.0f;
    auto clipPlane = [&](float distance, float speed)
    {
        if (std::fabs(speed) < 0.000001f) { return distance <= 0.0f; }
        const float t = -distance / speed;
        if (speed < 0.0f) { if (t > enter) { enter = t; } }
        else if (t < exit) { exit = t; }
        return enter <= exit;
    };
    if (!clipPlane(a.y - GetHeight() * 0.5f - radius, movement.y) ||
        !clipPlane(-a.y - GetHeight() * 0.5f - radius, -movement.y)) { return false; }
    const int sides = m_type == Type::Triangle ? 3 : 5;
    const float r = GetCollisionRadius();
    for (int i = 0; i < sides; ++i)
    {
        const float angle = -1.57079633f + 6.2831853f * i / sides;
        const float next = -1.57079633f + 6.2831853f * (i + 1) / sides;
        const float x = std::cos(angle) * r, z = std::sin(angle) * r;
        const float dx = std::cos(next) * r - x, dz = std::sin(next) * r - z;
        const float length = std::sqrt(dx * dx + dz * dz);
        const float nx = dz / length, nz = -dx / length;
        if (!clipPlane((a.x - x) * nx + (a.z - z) * nz - radius,
            movement.x * nx + movement.z * nz)) { return false; }
    }
    return true;
}

bool ShapeBox::IsPolygonTouch(const Vector3& center, float radius) const
{
    const Vector3 p = ToCollisionSpace(center) - GetCenter();
    const int sides = m_type == Type::Triangle ? 3 : 5;
    const float r = GetCollisionRadius();
    bool inside = true;
    float nearestSquared = 1.0e30f;
    for (int i = 0; i < sides; ++i)
    {
        const float angle = -1.57079633f + 6.2831853f * i / sides;
        const float next = -1.57079633f + 6.2831853f * (i + 1) / sides;
        const float x = std::cos(angle) * r, z = std::sin(angle) * r;
        const float dx = std::cos(next) * r - x, dz = std::sin(next) * r - z;
        if ((p.x - x) * dz - (p.z - z) * dx > 0.0f) { inside = false; }
        float t = ((p.x - x) * dx + (p.z - z) * dz) / (dx * dx + dz * dz);
        if (t < 0.0f) { t = 0.0f; }
        if (t > 1.0f) { t = 1.0f; }
        const float ex = p.x - x - dx * t, ez = p.z - z - dz * t;
        const float d = ex * ex + ez * ez;
        if (d < nearestSquared) { nearestSquared = d; }
    }
    float heightDistance = std::fabs(p.y) - GetHeight() * 0.5f;
    if (heightDistance < 0.0f) { heightDistance = 0.0f; }
    return (inside ? 0.0f : nearestSquared) + heightDistance * heightDistance <= radius * radius;
}

bool ShapeBox::Start()
{
    // 図形ごとに方向・速さ・回転を変え、全体が同じ動きにならないようにする。
    static std::mt19937 random(std::random_device{}());
    std::uniform_real_distribution<float> angle(0.0f, 6.2831853f);
    std::uniform_real_distribution<float> speed(6.0f, 12.0f);
    std::uniform_real_distribution<float> spin(0.08f, 0.16f);
    std::uniform_int_distribution<int> sign(0, 1);
    m_driftAngle = angle(random);
    m_rotationAngle = angle(random);
    m_driftSpeed = speed(random);
    m_spinSpeed = spin(random) * (sign(random) ? 1.0f : -1.0f);
    m_turnSpeed = 0.04f * (sign(random) ? 1.0f : -1.0f);
    return true;
}

Quaternion ShapeBox::GetRotation() const
{
    Quaternion rotation = Quaternion::Identity;
    rotation.SetRotationDeg(Vector3::AxisY, m_rotationAngle * 57.2957795f);
    return rotation;
}

Vector3 ShapeBox::GetRenderPosition() const
{
    if (m_type != Type::Square) { return GetCenter(); }
    // モデル中心がZ方向に21ずれているため、中心を軸に回すよう補正する。
    return GetCenter() + Vector3(-21.0f * std::sin(m_rotationAngle), 0,
        -21.0f * std::cos(m_rotationAngle));
}

Vector3 ShapeBox::ToCollisionSpace(const Vector3& point) const
{
    const Vector3 center = GetCenter();
    const float x = point.x - center.x;
    const float z = point.z - center.z;
    const float c = std::cos(m_rotationAngle);
    const float s = std::sin(m_rotationAngle);
    // 点を逆回転し、既存の箱の判定をそのまま使う。
    return Vector3(center.x + c * x - s * z, point.y,
        center.z + s * x + c * z);
}

void ShapeBox::AddImpulse(const Vector3& direction, float strength)
{
    if (!IsAlive() || strength <= 0.0f) { return; }
    const float length = std::sqrt(direction.x * direction.x + direction.z * direction.z);
    if (length < 0.0001f) { return; }
    m_pushVelocity += Vector3(direction.x / length, 0, direction.z / length) * strength;
    // 連続命中しても図形が高速で飛び続けないようにする。
    const float speed = std::sqrt(m_pushVelocity.x * m_pushVelocity.x + m_pushVelocity.z * m_pushVelocity.z);
    if (speed > 90.0f) { m_pushVelocity *= 90.0f / speed; }
}

void ShapeBox::UpdateMotion(float deltaTime)
{
    if (deltaTime <= 0.0f) { return; }
    m_driftAngle = std::fmod(m_driftAngle + m_turnSpeed * deltaTime, 6.2831853f);
    m_rotationAngle = std::fmod(m_rotationAngle + m_spinSpeed * deltaTime, 6.2831853f);
    Vector3 velocity(std::cos(m_driftAngle) * m_driftSpeed, 0,
        std::sin(m_driftAngle) * m_driftSpeed);
    // 図形同士が近づいたときは緩やかに押し合い、重なり続けるのを防ぐ。
    QueryGOs<ShapeBox>("shapeBox", [&](ShapeBox* other)
    {
        if (!other || other == this || !other->IsAlive()) { return true; }
        Vector3 away = GetCenter() - other->GetCenter();
        const float distance = std::sqrt(away.x * away.x + away.z * away.z);
        const float separation = GetCollisionRadius() + other->GetCollisionRadius();
        if (distance < separation)
        {
            if (distance < 0.001f)
            {
                // 完全に重なった場合も、個別の漂流方向から離れる向きを決める。
                away = Vector3(std::cos(m_driftAngle), 0, std::sin(m_driftAngle));
            }
            AddImpulse(away, (separation - distance) * 2.0f * deltaTime);
        }
        return true;
    });
    // 押された速度は指数減衰。積分も行い、フレームレートによる差を抑える。
    const float decay = std::exp(-3.0f * deltaTime);
    m_position += velocity * deltaTime + m_pushVelocity * ((1.0f - decay) / 3.0f);
    m_pushVelocity *= decay;

    // 回転時の角まで含めて、外周壁の内側に収める。
    const Vector3 center = GetCenter();
    const float LIMIT = 1500.0f - GetCollisionRadius() - 2.0f;
    if (center.x < -LIMIT || center.x > LIMIT)
    {
        const bool right = center.x > LIMIT;
        m_position.x = right ? LIMIT : -LIMIT;
        if ((right && velocity.x > 0) || (!right && velocity.x < 0)) { m_driftAngle = 3.14159265f - m_driftAngle; }
        if ((right && m_pushVelocity.x > 0) || (!right && m_pushVelocity.x < 0)) { m_pushVelocity.x *= -0.3f; }
    }
    if (center.z < -LIMIT || center.z > LIMIT)
    {
        const bool positiveZ = center.z > LIMIT;
        m_position.z = (positiveZ ? LIMIT : -LIMIT) - 21.0f;
        if ((positiveZ && velocity.z > 0) || (!positiveZ && velocity.z < 0)) { m_driftAngle = -m_driftAngle; }
        if ((positiveZ && m_pushVelocity.z > 0) || (!positiveZ && m_pushVelocity.z < 0)) { m_pushVelocity.z *= -0.3f; }
    }
}


// ============================================================
// Render
// ============================================================

void ShapeBox::Render(
    RenderContext& rc)
{}


// ============================================================
// 弾との衝突
// ============================================================

bool ShapeBox::IsHit(
    const Vector3& start,
    const Vector3& end,
    float bulletRadius
) const
{
    if (m_hp <= 0)
    {
        return false;
    }


    if (m_type != Type::Square) { return IsPolygonHit(start, end, bulletRadius); }

    // ============================================
    // 箱の判定範囲
    // ============================================

    const float minBounds[3] =
    {
        m_position.x -
            COLLISION_HALF_X -
            bulletRadius,

        m_position.y -
            bulletRadius,

        m_position.z +
            COLLISION_MIN_Z -
            bulletRadius
    };


    const float maxBounds[3] =
    {
        m_position.x +
            COLLISION_HALF_X +
            bulletRadius,

        m_position.y +
            COLLISION_HEIGHT +
            bulletRadius,

        m_position.z +
            COLLISION_MAX_Z +
            bulletRadius
    };


    const Vector3 localStart = ToCollisionSpace(start);
    const Vector3 localEnd = ToCollisionSpace(end);
    const float startValues[3] =
    {
        localStart.x,
        localStart.y,
        localStart.z
    };


    const float movement[3] =
    {
        localEnd.x - localStart.x,
        localEnd.y - localStart.y,
        localEnd.z - localStart.z
    };


    float enterTime =
        0.0f;

    float exitTime =
        1.0f;


    // ============================================
    // 線分とAABBの判定
    // ============================================

    for (int axis = 0;
        axis < 3;
        ++axis)
    {
        if (std::fabs(
            movement[axis]) <
            0.0001f)
        {
            if (
                startValues[axis] <
                minBounds[axis] ||
                startValues[axis] >
                maxBounds[axis])
            {
                return false;
            }


            continue;
        }


        float nearTime =
            (
                minBounds[axis] -
                startValues[axis]
                ) /
            movement[axis];


        float farTime =
            (
                maxBounds[axis] -
                startValues[axis]
                ) /
            movement[axis];


        if (nearTime >
            farTime)
        {
            const float temporary =
                nearTime;

            nearTime =
                farTime;

            farTime =
                temporary;
        }


        if (nearTime >
            enterTime)
        {
            enterTime =
                nearTime;
        }


        if (farTime <
            exitTime)
        {
            exitTime =
                farTime;
        }


        if (enterTime >
            exitTime)
        {
            return false;
        }
    }


    return true;
}


// ============================================================
// ダメージ
// ============================================================

void ShapeBox::TakeDamage(
    int damage,
    bool rewardPlayer)
{
    if (m_hp <= 0 ||
        damage <= 0)
    {
        return;
    }


    m_hp -=
        damage;


    if (m_hp <= 0)
    {
        BulletImpact::EmitDestruction(GetCenter(), GetCollisionRadius() / 48.0f, GetColor());
        m_pushVelocity = Vector3::Zero;
        // 5秒後に復活。
        m_respawnTimer =
            5.0f;


        // ========================================
        // 報酬
        // ========================================

        Player* player =
            FindGO<Player>(
                "player"
            );


        if (player != nullptr &&
            rewardPlayer)
        {
            player->AddExperience(
                GetExperienceReward()
            );


            player->AddScore(
                GetExperienceReward() * 10
            );
        }
    }
}


// ============================================================
// Update
// ============================================================

void ShapeBox::Update()
{
    if (m_hp > 0)
    {
        UpdateMotion(g_gameTime->GetFrameDeltaTime());
        return;
    }


    m_respawnTimer -=
        g_gameTime
        ->GetFrameDeltaTime();


    if (m_respawnTimer <=
        0.0f)
    {
        m_hp =
            GetMaxHP();


        m_respawnTimer =
            0.0f;
    }
}


// ============================================================
// 自機などとの接触
// ============================================================

bool ShapeBox::IsTouch(
    const Vector3& center,
    float radius
) const
{
    if (m_hp <= 0)
    {
        return false;
    }


    if (m_type != Type::Square) { return IsPolygonTouch(center, radius); }

    // ============================================
    // 箱の判定範囲
    //
    // X方向は左右に少し広げる。
    // Z方向はモデルの見た目に合わせて非対称。
    // ============================================

    const float minBounds[3] =
    {
        m_position.x -
            COLLISION_HALF_X,

        m_position.y,

        m_position.z +
            COLLISION_MIN_Z
    };


    const float maxBounds[3] =
    {
        m_position.x +
            COLLISION_HALF_X,

        m_position.y +
            COLLISION_HEIGHT,

        m_position.z +
            COLLISION_MAX_Z
    };


    const Vector3 localCenter = ToCollisionSpace(center);
    const float values[3] =
    {
        localCenter.x,
        localCenter.y,
        localCenter.z
    };


    float distanceSquared =
        0.0f;


    // ============================================
    // 球の中心から箱の最も近い点を求める
    // ============================================

    for (int axis = 0;
        axis < 3;
        ++axis)
    {
        float closest =
            values[axis];


        if (closest <
            minBounds[axis])
        {
            closest =
                minBounds[axis];
        }
        else if (
            closest >
            maxBounds[axis])
        {
            closest =
                maxBounds[axis];
        }


        const float difference =
            values[axis] -
            closest;


        distanceSquared +=
            difference *
            difference;
    }


    // ============================================
    // 球と箱が接触しているか
    // ============================================

    return
        distanceSquared <=
        radius *
        radius;
}