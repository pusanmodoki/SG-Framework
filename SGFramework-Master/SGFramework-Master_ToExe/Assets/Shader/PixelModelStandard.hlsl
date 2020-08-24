#include "RootHeader.hlsli"

float4 main(VertexOutputModel input) : SV_Target
{	
	//カメラ方向
	float4 lightDirection = cLightBuffer.directionalDirections[0];
	//法線
	float4 normal = input.normal;

	//lightDirection.w = 0.0f;								// 光の方向はベクトルなのでＷの値を０にする。
	//逆行列をかける
  // lightDirection = normalize(mul(cObjectBuffer.worldInverseMatrix, lightDirection));

//	normal.w = 0.0f;
	//normal = normalize(normal);
	
	// 拡散反射光の計算
    float4 diffuse = cMaterialBuffer3D.diffuse * (cLightBuffer.directionalColors[0] * cLightBuffer.directionalPowers[0]) * dot(normal, lightDirection); //ランバート余弦則とマテリアル値と掛け算
   // diffuse.rgb = diffuse.rgb * cLightBuffer.directionalColors[0].rgb * cLightBuffer.directionalPowers[0];


	//// 鏡面反射光の計算
    float4 specularDetail = normalize(lightDirection + normalize(cLightBuffer.eyePosition - input.worldPosition));
    float4 specular = pow(max(0, dot(normal, specularDetail)), 50) * cMaterialBuffer3D.specular;
   // specular.rgb = specular.rgb * cLightBuffer.directionalColors[0].rgb * cLightBuffer.directionalPowers[0];

	////色
	float4 resultColor;
    float4 textureColor;
    textureColor.rgb = g_textureSlot0.Sample(g_samplerSlot0, input.textureUV0).rgb;
    textureColor.rgb = textureColor.rgb + (g_textureSlot1.Sample(g_samplerSlot1, input.textureUV1).rgb - textureColor.rgb) * (g_textureSlot1.Sample(g_samplerSlot1, input.textureUV1).rgb / 255);
    resultColor.rgb = cMaterialBuffer3D.diffuse.rgb * textureColor.rgb * 5.5f;
    resultColor.rgb = resultColor.rgb + (cLightBuffer.directionalColors[0].rgb - resultColor.rgb) * (cLightBuffer.directionalPowers[0] / 255);

	resultColor.a = 1.0f * cMaterialBuffer3D.transparent;
	return resultColor;
}