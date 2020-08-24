#include "RootHeader.hlsli"

float4 main(VSOutputStandard input) : SV_Target
{	
    float4 color;
    
    color.rgb = g_textureSlot0.Sample(g_samplerSlot0, input.textureUV).rgb;
	color.a = input.color.a;// * cMaterialBuffer2D.color.a;
    
    return color;
}