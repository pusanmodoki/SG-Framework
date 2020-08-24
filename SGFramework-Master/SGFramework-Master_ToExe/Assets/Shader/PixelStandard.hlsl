#include "RootHeader.hlsli"

float4 main(VSOutputStandard input) : SV_Target
{	
	//カメラ方向
	float4 lightDirection = cLightBuffer.directionalDirections[0];
	//法線
	float4 normal = input.normal;

	lightDirection.w = 0.0f;								// 光の方向はベクトルなのでＷの値を０にする。

    lightDirection = lightDirection;

	normal.w = 0.0f;
	normal = normalize(normal);
	
	// 拡散反射光の計算
    float4 diffuse = cMaterialBuffer3D.diffuse * (cLightBuffer.directionalColors[0] * cLightBuffer.directionalPowers[0]) *dot(normal, lightDirection); //ランバート余弦則とマテリアル値と掛け算
    //diffuse.rgb = clamp(diffuse.rgb * cLightBuffer.directionalColors[0].rgb * cLightBuffer.directionalPowers[0], 0, 1);


	// 鏡面反射光の計算
    float4 specularDetail = normalize(
    lightDirection + normalize(mul(cObjectBuffer.worldInverseMatrix, mul(cObjectBuffer.worldInverseMatrix, cLightBuffer.eyeDirection))));
    float4 specular = pow(clamp(dot(normal, specularDetail) * cMaterialBuffer3D.specular, 0.0, 1.0), 50);
   // specular.rgb = clamp(specular.rgb * cLightBuffer.directionalColors[0].rgb * cLightBuffer.directionalPowers[0], 0, 1);

	////色
	float4 resultColor;
    resultColor.rgb =  g_textureSlot0.Sample(g_samplerSlot0, input.textureUV).rgb;
    resultColor.rgb = resultColor.rgb + (g_textureSlot1.Sample(g_samplerSlot1, input.textureUV).rgb - resultColor.rgb) * (g_textureSlot1.Sample(g_samplerSlot1, input.textureUV).rgb / 255);
    resultColor.rgb = specular.rgb + diffuse.rgb * g_textureSlot0.Sample(g_samplerSlot0, input.textureUV).rgb;
    
    resultColor.rgb = resultColor.rgb + (cLightBuffer.directionalColors[0].rgb - resultColor.rgb) * (cLightBuffer.directionalPowers[0] / 255);

	resultColor.a = 1.0f;
	return resultColor;
}