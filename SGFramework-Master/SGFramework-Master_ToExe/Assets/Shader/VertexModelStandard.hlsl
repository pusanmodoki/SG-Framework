#include "RootHeader.hlsli"

VertexOutputModel main(VertexInputModel input)
{
    VertexOutputModel output = (VertexOutputModel)0;

    float4x4 animationMatrix = (float4x4) 0;
    float allWeights = 0.0f;
    
    animationMatrix += cModelBuffer.bones[input.boneIndex0[0]] * input.weight0[0];
    animationMatrix += cModelBuffer.bones[input.boneIndex0[1]] * input.weight0[1];
    animationMatrix += cModelBuffer.bones[input.boneIndex0[2]] * input.weight0[2];
    animationMatrix += cModelBuffer.bones[input.boneIndex0[3]] * input.weight0[3];
    animationMatrix += cModelBuffer.bones[input.boneIndex1[0]] * input.weight1[0];
    animationMatrix += cModelBuffer.bones[input.boneIndex1[1]] * input.weight1[1];
    animationMatrix += cModelBuffer.bones[input.boneIndex1[2]] * input.weight1[2];
    animationMatrix += cModelBuffer.bones[input.boneIndex1[3]] * input.weight1[3];
    
    allWeights = input.weight0[0] + input.weight0[1] + input.weight0[2] + input.weight0[3] +
    input.weight1[0] + input.weight1[1] + input.weight1[2] + input.weight1[3];
    
    animationMatrix += cModelBuffer.bones[g_cIdentityBoneIndex] * (1.0f - allWeights);
    
   // animationMatrix = normalize(animationMatrix);
    //output.drawPosition = mul(position, cObjectBuffer.worldMatrix);
   
    output.drawPosition = input.position;
    output.drawPosition = mul(output.drawPosition, animationMatrix);
    output.drawPosition = mul(output.drawPosition, cObjectBuffer.worldMatrix);
    
    //output.drawPosition = mul(position, cObjectBuffer.worldMatrix);
    
    output.worldPosition = output.drawPosition;
    output.drawPosition = mul(output.drawPosition, cSceneBuffer.viewMatrix);
    output.drawPosition = mul(output.drawPosition, cSceneBuffer.projectionMatrix);
    output.textureUV0 = input.textureUV0;
    output.textureUV1 = input.textureUV1;
    
    float4 setNormal = float4(input.normal.x, input.normal.y, input.normal.z, 0.0f);
    setNormal = mul(input.normal, animationMatrix);
    setNormal = mul(setNormal, cObjectBuffer.worldMatrix);
    output.normal = normalize(setNormal);
    
    return output;
}