/*----------------------------------------------------------------------------------
コンスタントバッファクラスを集めたDirectX11ConstantBuffer.hpp
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_GRAPHICS_X11_CONSTANT_BUFFER_HPP_
#define SGFRAMEWORK_HEADER_GRAPHICS_X11_CONSTANT_BUFFER_HPP_
#include "../../../02_CustomVariable/CustomVariable.hpp"
#include "../Light/LightX11.hpp"

// Framework namespace
namespace SGFramework
{
	class Application;
	class Thread;
	//フレームワークの管理をするAdministrator namespace
	namespace Administrator
	{
		//GraphicsDetail namespace
		namespace GraphicsDetail
		{
			//DirectX11
			namespace X11
			{
				//コンスタントバッファ->Scene, 変更注意!
				struct ConstantBufferScene
				{
					Vector4x4 viewMatrix;
					Vector4x4 projectionMatrix;
				};
				//コンスタントバッファ->Light, 変更注意!
				struct ConstantBufferLight
				{
					struct Directional
					{
						Vector4 directionalDirections[DirectionalLightX11::cDirectionalLightMax];
						Color directionalColors[DirectionalLightX11::cDirectionalLightMax];
						float directionalPowers[DirectionalLightX11::cDirectionalLightMax];
						float directionalActives[DirectionalLightX11::cDirectionalLightMax];
					};
					struct Point
					{
						Vector4 pointPositions[PointLightX11::cPointLightMax];
						Color pointColors[PointLightX11::cPointLightMax];
						float pointPowers[PointLightX11::cPointLightMax];
						float pointActives[PointLightX11::cPointLightMax];
					};
					struct HemiSphere
					{
						Vector4 hemiSphereDirection[HemiSphereLightX11::cHemiSphereLightMax];
						Color hemiSphereSkyColor[HemiSphereLightX11::cHemiSphereLightMax];
						Color hemiSphereGroundColor[HemiSphereLightX11::cHemiSphereLightMax];
						float hemiSpherePowerSky[HemiSphereLightX11::cHemiSphereLightMax];
						float hemiSpherePowerGround[HemiSphereLightX11::cHemiSphereLightMax];
						float hemiSphereActives[HemiSphereLightX11::cHemiSphereLightMax];
					};

					Vector4 eyePosition = Const::Vector4::zero;
					Vector4 eyeDirection = Const::Vector4::zero;
					Directional directional = {};
					Point point = {};
					HemiSphere hemiSphere = {};
				};
				//コンスタントバッファ->Object, 変更注意!
				struct ConstantBufferObject
				{
					Vector4x4 worldMatrix;
					Vector4x4 worldInverseMatrix;
				};
				//コンスタントバッファ->Material3D, 変更注意!
				struct ConstantBufferMaterial3D
				{
					Color ambient = Const::Color::white;		//Ambient
					Color diffuse = Const::Color::white;		//Diffuse
					Color emission = Const::Color::white;		//Emission
					Color specular = Const::Color::white;		//Specular
					Vector3 bump = Const::Vector3::zero;	//Bump
					float transparent = 1.0f;							//Transparent
					float power = 1.0f;									//Power
				};
				//コンスタントバッファ->Material2D, 変更注意!
				struct ConstantBufferMaterial2D
				{
					Color color = Const::Color::white;			//Polygon Color
				};
				//コンスタントバッファ->Model, 変更注意!
				struct ConstantBufferModel
				{
					Vector4x4 bones[1000];
				};
			}
		}
	}
}
#endif //!SGFRAMEWORK_HEADER_GRAPHICS_X11_CONSTANT_BUFFER_HPP_