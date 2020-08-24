/*----------------------------------------------------------------------------------
�R���X�^���g�o�b�t�@�N���X���W�߂�DirectX11ConstantBuffer.hpp
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
	//�t���[�����[�N�̊Ǘ�������Administrator namespace
	namespace Administrator
	{
		//GraphicsDetail namespace
		namespace GraphicsDetail
		{
			//DirectX11
			namespace X11
			{
				//�R���X�^���g�o�b�t�@->Scene, �ύX����!
				struct ConstantBufferScene
				{
					Vector4x4 viewMatrix;
					Vector4x4 projectionMatrix;
				};
				//�R���X�^���g�o�b�t�@->Light, �ύX����!
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
				//�R���X�^���g�o�b�t�@->Object, �ύX����!
				struct ConstantBufferObject
				{
					Vector4x4 worldMatrix;
					Vector4x4 worldInverseMatrix;
				};
				//�R���X�^���g�o�b�t�@->Material3D, �ύX����!
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
				//�R���X�^���g�o�b�t�@->Material2D, �ύX����!
				struct ConstantBufferMaterial2D
				{
					Color color = Const::Color::white;			//Polygon Color
				};
				//�R���X�^���g�o�b�t�@->Model, �ύX����!
				struct ConstantBufferModel
				{
					Vector4x4 bones[1000];
				};
			}
		}
	}
}
#endif //!SGFRAMEWORK_HEADER_GRAPHICS_X11_CONSTANT_BUFFER_HPP_