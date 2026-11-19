///////////////////////////////////////
// 構造体。
///////////////////////////////////////

// ピクセルシェーダーへの入力
struct SPSIn
{
    float4 pos : SV_POSITION;       //座標。
    float3 normal : NORMAL;         //法線。
    float3 tangent  : TANGENT;      //接ベクトル。
    float3 biNormal : BINORMAL;     //従ベクトル。
    float2 uv : TEXCOORD0;
    float3 worldPosition : TEXCOORD1;          //UV座標。
};

// ピクセルシェーダーからの出力
struct SPSOut
{
    float4 albedo : SV_Target0;         // アルベド
    float4 normal : SV_Target1;         // 法線
    float4 metaricShadowSmooth : SV_Target2;  // メタリック、影パラメータ、スムース。rにメタリック、gに影パラメータ、aにスムース。
};

///////////////////////////////////////
// 頂点シェーダーの共通処理をインクルードする。
///////////////////////////////////////

#include "ModelVSCommon.h"


///////////////////////////////////////
// シェーダーリソース
///////////////////////////////////////
Texture2D<float4> g_albedo : register(t0);      //アルベドマップ
Texture2D<float4> g_normal : register(t1);      //法線マップ
Texture2D<float4> g_spacular : register(t2);    //スペキュラマップ

///////////////////////////////////////
// サンプラーステート
///////////////////////////////////////
sampler g_sampler : register(s0);

///////////////////////////////////////
// 関数
///////////////////////////////////////

// 法線マップから法線を取得。
float3 GetNormalFromNormalMap(float3 normal, float3 tangent, float3 biNormal, float2 uv)
{
    float3 binSpaceNormal = g_normal.SampleLevel (g_sampler, uv, 0.0f).xyz;
    binSpaceNormal = (binSpaceNormal * 2.0f) - 1.0f;

    float3 newNormal = tangent * binSpaceNormal.x + biNormal * binSpaceNormal.y + normal * binSpaceNormal.z;
    
    return newNormal;
}



// モデル用の頂点シェーダーのエントリーポイント
SPSIn VSMainCore(SVSIn vsIn, float4x4 mWorldLocal, uniform bool isUsePreComputedVertexBuffer)
{
    SPSIn psIn;
    psIn.pos = CalcVertexPositionInWorldSpace(vsIn.pos, mWorldLocal, isUsePreComputedVertexBuffer);
    psIn.worldPosition = psIn.pos.xyz;
    psIn.pos = mul(mView, psIn.pos); // ワールド座標系からカメラ座標系に変換
    psIn.pos = mul(mProj, psIn.pos); // カメラ座標系からスクリーン座標系に変換

    // ワールド空間の法線、接ベクトル、従ベクトルを計算する。
    CalcVertexNormalTangentBiNormalInWorldSpace(
		psIn.normal,
		psIn.tangent,
		psIn.biNormal,
		mWorldLocal,
		vsIn.normal,
		vsIn.tangent,
		vsIn.biNormal,
		isUsePreComputedVertexBuffer
	);

    psIn.uv = vsIn.uv;
    
    return psIn;
}
SPSOut PSMainCore( SPSIn psIn, int isShadowReciever)
{
    // G-Bufferに出力
    SPSOut psOut;
    // アルベドカラーと深度値を出力
    // 床と壁の配色をテクスチャから独立させる。
    float3 surfaceNormal = normalize(psIn.normal);
    float3 color = float3(0.48f, 0.51f, 0.54f);
    if (abs(surfaceNormal.y) > 0.9f && abs(psIn.worldPosition.y) < 2.0f)
    {
        // ワールド座標基準なので、UVや解像度によらず同じ間隔になる。
        float2 p = psIn.worldPosition.xz;
        float2 footprint = max(fwidth(p), float2(0.001f, 0.001f));
        float2 distanceToLine = abs(frac(p / 100.0f + 0.5f) - 0.5f) * 100.0f;
        float2 coverage = 1.0f - smoothstep(float2(0.0f, 0.0f), footprint + 0.8f, distanceToLine);
        // 遠方の線をフェードさせ、斜め視点でのちらつきを抑える。
        float grid = max(coverage.x, coverage.y) * (1.0f - smoothstep(12.0f, 40.0f, max(footprint.x, footprint.y)));
        color = lerp(float3(0.78f, 0.79f, 0.80f), float3(0.65f, 0.67f, 0.69f), grid * 0.65f);
        float edge = max(abs(p.x), abs(p.y));
        float boundary = smoothstep(1492.0f - max(footprint.x, footprint.y), 1495.0f, edge);
        color = lerp(color, float3(0.40f, 0.44f, 0.48f), boundary);
    }
    else
    {
        // 外周壁の上面を明るくして、立体感と境界の見やすさを保つ。
        color = lerp(color, float3(0.62f, 0.65f, 0.68f), saturate(surfaceNormal.y));
    }
    psOut.albedo = float4(color, 1.0f);
    
    clip(psOut.albedo.a - 0.2f);    // ピクセルキル

    psOut.albedo.w = psIn.pos.z;
    // 法線を出力
    psOut.normal.xyz = surfaceNormal;
    psOut.normal.w = 1.0f;
    // メタリックスムースを出力。
    psOut.metaricShadowSmooth = float4(0.0f, 0.0f, 0.0f, 0.0f);
    // 影パラメータ。
    psOut.metaricShadowSmooth.g = 255.0f * isShadowReciever;
    
    return psOut;
}
// モデル用のピクセルシェーダーのエントリーポイント
SPSOut PSMain(SPSIn psIn)
{
    return PSMainCore(psIn, 0);
}
SPSOut PSMainShadowReciever(SPSIn psIn)
{
    return PSMainCore(psIn, 1);
}
