#include "RootHeader.hlsli"

float4 main(VSOutputStandard input) : SV_Target
{	
	return input.color;// * cMaterialBuffer2D.color;
}