//RootHeader
SamplerState g_samplerSlot0 : register(s0); // サンプラー
//RootHeader
Texture2D g_textureSlot0 : register(t0);        // テクスチャ
 
//RootHeader
struct SceneBuffer
{
    matrix viewMatrix;
    matrix projectionMatrix;
};
//RootHeader
cbuffer ConstantBufferScene : register(b0)
{
    SceneBuffer cSceneBuffer;
};
  
struct PS_INPUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR0;
    float2 uv : TEXCOORD0;
};


float4 main(PS_INPUT input) : SV_Target
{
    float4 result = input.color * g_textureSlot0.Sample(g_samplerSlot0, input.uv);

    return result;
}