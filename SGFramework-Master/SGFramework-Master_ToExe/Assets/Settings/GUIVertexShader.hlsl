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

struct VS_INPUT
{
    float2 position : POSITION0;
    float4 color : COLOR0;
    float2 uv : TEXCOORD0;
};
struct PS_INPUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR0;
    float2 uv : TEXCOORD0;
};

PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output;

    output.position = mul(cSceneBuffer.projectionMatrix, float4(input.position.xy, 0.f, 1.f));

    output.color = input.color;

    output.uv = input.uv;

    return output;
}