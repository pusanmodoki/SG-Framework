#include "RootHeader.hlsli"

VSOutputStandard main(float4 position : POSITION0,
	float4 color : COLOR0,
	float2 textureUV : TEXCOORD0)
{
    VSOutputStandard output = (VSOutputStandard) 0;

    output.drawPosition = position;
    output.drawPosition = mul(position, cObjectBuffer.worldMatrix);
    output.worldPosition = output.drawPosition;
    output.drawPosition = mul(output.drawPosition, cSceneBuffer.viewMatrix);
    output.drawPosition = mul(output.drawPosition, cSceneBuffer.projectionMatrix);
    output.textureUV = textureUV;
    
    output.color = color;

    return output;
}