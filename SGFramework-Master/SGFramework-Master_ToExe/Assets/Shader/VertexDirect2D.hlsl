#include "RootHeader.hlsli"

VSOutputStandard main(float4 position : POSITION0,
	float4 color : COLOR0,
	float2 textureUV : TEXCOORD0,
	float2 normalUV : TEXCOORD1)
{
    VSOutputStandard output = (VSOutputStandard) 0;

    output.drawPosition = position;
    output.drawPosition = mul(position, cObjectBuffer.worldMatrix);
    output.worldPosition = output.drawPosition;
    output.textureUV = textureUV;
    
    output.color = color;

    return output;
}