#include "RootHeader.hlsli"

VSOutputStandard main(float4 position : POSITION0,
	float4 normal : NORMAL0,
	float2 textureUV : TEXCOORD0)
{
	VSOutputStandard output = (VSOutputStandard)0;

	output.drawPosition = position;
	output.drawPosition = mul(position, cObjectBuffer.worldMatrix);
	output.worldPosition = output.drawPosition;
	output.drawPosition = mul(output.drawPosition, cSceneBuffer.viewMatrix);
	output.drawPosition = mul(output.drawPosition, cSceneBuffer.projectionMatrix);
	output.textureUV = textureUV;

	//input.normal.w = 0.0f;					// 法線はベクトルなのでＷの値を０にする。

	output.normal = normal;
	output.normal.w = 0.0f;

	output.normal = mul(output.normal, cObjectBuffer.worldMatrix);
	output.normal = normalize(output.normal);


	return output;
}