#include "RootHeader.hlsli"

float4 main(VSOutputStandard input) : SV_Target
{	
    float4 color;
    
    color.rgb = g_textureSlot0.Sample(g_samplerSlot0, input.textureUV).rgb;
	//グレースケール化
	color.rgb = color.r * 0.3f + color.g * 0.6f + color.b * 0.1f;
	color.a = input.color.a;// * cMaterialBuffer2D.color.a;
    
    return color;
}