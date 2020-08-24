static const int g_cDirectionalLightMax = 4;
static const int g_cPointLightMax = 4;
static const int g_cHemiSphereLightMax = 1;
static const int g_cNumMaxOneModelBones = 1000;
static const int g_cIdentityBoneIndex = 999;
static const int g_cNumMaxVertexBones = 8;

Texture2D g_textureSlot0: register(t0);             // テクスチャ
Texture2D g_textureSlot1 : register(t1);            // テクスチャ
Texture2D g_textureNormalMap : register(t2);   // テクスチャ
Texture2D g_textureHeightMap : register(t3);    // テクスチャ

SamplerState g_samplerSlot0 : register(s0);     // サンプラー
SamplerState g_samplerSlot1 : register(s1);     // サンプラー
SamplerState g_samplerNormalMap : register(s2); // サンプラー
SamplerState g_samplerHeightMap : register(s3); // サンプラー

struct LightBuffer
{
	float4 eyePosition;
    float4 eyeDirection;

	float4 directionalDirections[g_cDirectionalLightMax];
	float4 directionalColors[g_cDirectionalLightMax];
	float directionalPowers[g_cDirectionalLightMax];
	float directionalActives[g_cDirectionalLightMax];

	float4 pointPositions[g_cPointLightMax];
	float4 pointColors[g_cPointLightMax];
	float pointPowers[g_cPointLightMax];
	float pointActives[g_cPointLightMax];

	float4 hemiSphereDirection[g_cHemiSphereLightMax];
	float4 hemiSphereSkyColor[g_cHemiSphereLightMax];
	float4 hemiSphereGroundColor[g_cHemiSphereLightMax];
	float hemiSpherePowerSky[g_cHemiSphereLightMax];
	float hemiSpherePowerGround[g_cHemiSphereLightMax];
	float hemiSphereActives[g_cHemiSphereLightMax];
};
struct SceneBuffer
{
	matrix viewMatrix;
	matrix projectionMatrix;
};
struct ObjectBuffer
{
	matrix worldMatrix;
    matrix worldInverseMatrix;
};
struct Material3DBuffer
{
    float4 ambient;     //Ambient
    float4 diffuse;       //Diffuse
    float4 emission;    //Emission
    float4 specular;    //Specular
    float3 bump;        //Bump
    float transparent; //Transparent
    float power;        //Power
};
struct Material2DBuffer
{
    float4 color;       //Color
};
struct ModelBuffer
{
    matrix bones[g_cNumMaxOneModelBones];
};

cbuffer ConstantBufferScene: register(b0)
{
	SceneBuffer cSceneBuffer;
};

cbuffer ConstantBufferLight: register(b1)
{
	LightBuffer cLightBuffer;
};

cbuffer ConstantBufferObject: register(b2)
{
	ObjectBuffer cObjectBuffer;
};

cbuffer ConstantBufferMaterial3D: register(b3)
{
    Material3DBuffer cMaterialBuffer3D;
};
cbuffer ConstantBufferMaterial2D: register(b4)
{
    Material2DBuffer cMaterialBuffer2D;
};

cbuffer ConstantBufferModel : register(b5)
{
    ModelBuffer cModelBuffer;
};

//撤去予定
struct VSInputStandard
{
	float4 position : POSITION0;
	float4 normal : NORMAL0;
	float2 textureUV : TEXCOORD0;
};

//撤去予定
struct VSOutputStandard
{

	//float4 Pos : SV_POSITION;
	//float4 Color : COLOR0;
	//float2 Tex : TEXCOORD;
	//float4 WPos : TEXCOORD1;
	//float4 Normal : TEXCOORD2;

	float4 drawPosition : SV_POSITION;
	float4 color : COLOR0;
	float2 textureUV : TEXCOORD0;
	float4 worldPosition : TEXCOORD1;
	float4 normal : TEXCOORD2;

	//float4 position : POSITION0;
	//float4 normal : NORMAL0;
	//float2 textureUV : TEXCOORD0;
};

struct VertexInputModel
{
    float4 position : POSITION0;
    float4 normal : NORMAL0;
    float2 textureUV0 : TEXCOORD0;
    float2 textureUV1 : TEXCOORD1;
    float4 weight0 : BLENDWEIGHT0;
    float4 weight1 : BLENDWEIGHT1;
    int4 boneIndex0 : BLENDINDICES0;
    int4 boneIndex1 : BLENDINDICES1;
};
struct VertexOutputModel
{
	float4 drawPosition : SV_POSITION;
    float4 worldPosition : VPOS;
    float4 normal : NORMAL0;
    float2 textureUV0 : TEXCOORD0;
    float2 textureUV1 : TEXCOORD1;
};