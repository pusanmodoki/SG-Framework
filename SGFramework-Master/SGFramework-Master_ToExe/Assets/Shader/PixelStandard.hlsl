#include "RootHeader.hlsli"

float4 main(VSOutputStandard input) : SV_Target
{	
	//�J��������
	float4 lightDirection = cLightBuffer.directionalDirections[0];
	//�@��
	float4 normal = input.normal;

	lightDirection.w = 0.0f;								// ���̕����̓x�N�g���Ȃ̂łv�̒l���O�ɂ���B

    lightDirection = lightDirection;

	normal.w = 0.0f;
	normal = normalize(normal);
	
	// �g�U���ˌ��̌v�Z
    float4 diffuse = cMaterialBuffer3D.diffuse * (cLightBuffer.directionalColors[0] * cLightBuffer.directionalPowers[0]) *dot(normal, lightDirection); //�����o�[�g�]�����ƃ}�e���A���l�Ɗ|���Z
    //diffuse.rgb = clamp(diffuse.rgb * cLightBuffer.directionalColors[0].rgb * cLightBuffer.directionalPowers[0], 0, 1);


	// ���ʔ��ˌ��̌v�Z
    float4 specularDetail = normalize(
    lightDirection + normalize(mul(cObjectBuffer.worldInverseMatrix, mul(cObjectBuffer.worldInverseMatrix, cLightBuffer.eyeDirection))));
    float4 specular = pow(clamp(dot(normal, specularDetail) * cMaterialBuffer3D.specular, 0.0, 1.0), 50);
   // specular.rgb = clamp(specular.rgb * cLightBuffer.directionalColors[0].rgb * cLightBuffer.directionalPowers[0], 0, 1);

	////�F
	float4 resultColor;
    resultColor.rgb =  g_textureSlot0.Sample(g_samplerSlot0, input.textureUV).rgb;
    resultColor.rgb = resultColor.rgb + (g_textureSlot1.Sample(g_samplerSlot1, input.textureUV).rgb - resultColor.rgb) * (g_textureSlot1.Sample(g_samplerSlot1, input.textureUV).rgb / 255);
    resultColor.rgb = specular.rgb + diffuse.rgb * g_textureSlot0.Sample(g_samplerSlot0, input.textureUV).rgb;
    
    resultColor.rgb = resultColor.rgb + (cLightBuffer.directionalColors[0].rgb - resultColor.rgb) * (cLightBuffer.directionalPowers[0] / 255);

	resultColor.a = 1.0f;
	return resultColor;
}