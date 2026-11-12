cbuffer ModelCb : register(b0)
{
    float4x4 mWorld;
    float4x4 mView;
    float4x4 mProj;
};
cbuffer GrowthCb : register(b1)
{
    float4 effectColor;
    float4 effectMode;
};
struct VSInput
{
    float4 position : POSITION;
    float3 normal : NORMAL;
};
struct PSInput
{
    float4 position : SV_POSITION;
    float3 localNormal : TEXCOORD0;
    float3 viewNormal : TEXCOORD1;
};
PSInput VSMain(VSInput input)
{
    PSInput output;
    output.position = mul(mProj, mul(mView, mul(mWorld, input.position)));
    output.localNormal = input.normal;
    output.viewNormal = mul((float3x3)mView,
        normalize(mul((float3x3)mWorld, input.normal)));
    return output;
}
float4 PSMain(PSInput input) : SV_TARGET
{
    float alpha;
    if (effectMode.x > 0.5f)
    {
        // Keep only the equatorial band of the flattened sphere: a soft ring.
        float latitude = abs(normalize(input.localNormal).y);
        alpha = smoothstep(0.02f, 0.15f, latitude) *
            (1.0f - smoothstep(0.35f, 0.55f, latitude));
    }
    else
    {
        alpha = pow(saturate(abs(normalize(input.viewNormal).z)), 1.5f);
    }
    return float4(effectColor.rgb * 1.2f, effectColor.a * alpha);
}