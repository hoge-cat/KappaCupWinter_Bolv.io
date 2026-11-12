// Soft, unlit smoke using the existing sphere mesh. No texture asset required.
cbuffer ModelCb : register(b0)
{
    float4x4 mWorld;
    float4x4 mView;
    float4x4 mProj;
};
cbuffer SmokeCb : register(b1)
{
    float4 smokeColor;
};
struct VSInput
{
    float4 position : POSITION;
    float3 normal : NORMAL;
};
struct PSInput
{
    float4 position : SV_POSITION;
    float3 viewNormal : TEXCOORD0;
};
PSInput VSMain(VSInput input)
{
    PSInput output;
    float4 worldPosition = mul(mWorld, input.position);
    output.position = mul(mProj, mul(mView, worldPosition));
    float3 worldNormal = normalize(mul((float3x3)mWorld, input.normal));
    output.viewNormal = mul((float3x3)mView, worldNormal);
    return output;
}
float4 PSMain(PSInput input) : SV_TARGET
{
    float facing = saturate(abs(normalize(input.viewNormal).z));
    float alpha = smokeColor.a * smoothstep(0.0f, 0.75f, facing) * facing;
    return float4(smokeColor.rgb, alpha);
}