/*----------------------------------------------------------------------------------
DirectX11 HemiSphereLightを扱うHemiSphereX11 class
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_HEMI_SPHERE_LIGHT_X11_HPP_
#define SGFRAMEWORK_HEADER_HEMI_SPHERE_LIGHT_X11_HPP_
#include "../../../02_CustomVariable/CustomVariable.hpp"
#include "BaseLightX11.hpp"

// Framework namespace
namespace SGFramework
{
		//フレームワークの管理をするAdministrator namespace
	namespace Administrator
	{
		//friend
		class GraphicsX11;
	
		//GraphicsDetail namespace
		namespace GraphicsDetail
		{
			//DirectX11 HemiSphereLightを扱うHemiSphereX11 class
			//mutexによる保護は行わないので注意
			class HemiSphereLightX11 final : public SGFramework::BaseClass::BaseLightX11
			{
			public:
				//friend
				friend class SGFramework::Administrator::GraphicsX11;

				DELETE_ALL_DEFAULT_CLASS_FUNCTIONS(HemiSphereLightX11);

				//セットする要素を内包するElement class
				struct Element
				{
					//default
					Element() = default;
					//----------------------------------------------------------------------------------
					//[コンストラクタ]
					//代入初期化する
					//引数1: ライトの位置
					//引数2: ライトの色 (空側)
					//引数3: ライトの色 (地面側)
					//引数4: ライトの明るさ倍率 (空側)
					//引数5: ライトの明るさ倍率 (地面側)
					//引数6: ライトが有効か否か
					Element(const Vector3& direction, const Color& colorSky, const Color& colorGround,
						float powerSky, float powerGround, bool isEnabled)
						: direction(direction), colorSky(colorSky), colorGround(colorGround),
						powerSky(powerSky), powerGround(powerGround), isEnabled(isEnabled) {}

					Vector3 direction = Const::Vector3::zero;
					Color colorSky = Const::Color::white;			//ライトの色 (Sky)
					Color colorGround = Const::Color::white;	//ライトの色 (Ground)
					float powerSky = 1.0f;									//ライトの明るさ倍率 (Sky)
					float powerGround = 1.0f;							//ライトの明るさ倍率 (Ground)
					bool isEnabled = true;								//ライトが有効か否か
				};


				//now parameters <static>(get function property)
				//argument 1: index (index < cHemiSphereLightMax)
				SGF_FUNCTION_PROPERTY static Element getParameter(uint index)
				{
					return Element(m_pack.directions[index], m_pack.colorSkys[index], m_pack.colorGrounds[index],
						m_pack.powerSkys[index], m_pack.powerGrounds[index], m_pack.isEnableds[index] > 0.0f ? true : false);
				}
				//set parameters[index] <static>(set function property)
				//return: set complete = true, failed (index < cHemiSphereLightMax) = false
				//argument 1: index (index < cHemiSphereLightMax)
				//引数2: ライトの位置
				//引数3: ライトの色 (空側)
				//引数4: ライトの色 (地面側)
				//引数5: ライトの明るさ倍率 (空側)
				//引数6: ライトの明るさ倍率 (地面側)
				//引数7: ライトが有効か否か, default = true
				SGF_FUNCTION_PROPERTY static bool setParameter(uint index, const Vector3& direction, const Color& colorSky,
					const Color& colorGround, float powerSky, float powerGround, bool isEnabled = true)
				{
					if (index < cHemiSphereLightMax)
					{
						m_pack.directions[index].setVector3(direction, 0.0f);
						m_pack.colorSkys[index] = colorSky;
						m_pack.colorGrounds[index] = colorGround;
						m_pack.powerSkys[index] = powerSky;
						m_pack.powerGrounds[index] = powerGround;
						m_pack.isEnableds[index] = IS_TRUE(isEnabled) ? 1.0f : 0.0f;
						m_isChangeds[m_cIsChangedHemiSphere] = true;
						return true;
					}
					else
						return false;
				}
				//set parameters[index] <static>(set function property)
				//return: set complete = true, failed (index < cHemiSphereLightMax) = false
				//argument 1: index (index < cHemiSphereLightMax)
				//argument 2: Element
				SGF_FUNCTION_PROPERTY static bool setParameter(uint index, const Element& element)
				{
					if (index < cHemiSphereLightMax)
					{
						m_pack.directions[index].setVector3(element.direction, 0.0f);
						m_pack.colorSkys[index] = element.colorSky;
						m_pack.colorGrounds[index] = element.colorGround;
						m_pack.powerSkys[index] = element.powerSky;
						m_pack.powerGrounds[index] = element.powerGround;
						m_pack.isEnableds[index] = IS_TRUE(element.isEnabled) ? 1.0f : 0.0f;
						m_isChangeds[m_cIsChangedHemiSphere] = true;
						return true;
					}
					else
						return false;
				}
				//set parameters loop <static>(set function property)
				//argument 1: elements variable length arguments, setting start -> index 0
				template<class ...Elements>
				SGF_FUNCTION_PROPERTY static void setParameters(const Elements&... elements)
				{
					uint i = 0, size = static_cast<uint>(sizeof...(elements));
					for (auto& e : { elements })
					{
						if (i < cHemiSphereLightMax && i < size)
						{
							const Element& element = e;
							m_pack.directions[i].setVector3(element.direction, 0.0f);
							m_pack.colorSkys[i] = element.colorSky;
							m_pack.colorGrounds[i] = element.colorGround;
							m_pack.powerSkys[i] = element.powerSky;
							m_pack.powerGrounds[i] = element.powerGround;
							m_pack.isEnableds[i] = IS_TRUE(element.isEnabled) ? 1.0f : 0.0f;
							m_isChangeds[m_cIsChangedHemiSphere] = true;
							++i;
						}
						else
							break;
					}
				}

				//light is enabled? <static>(get function property)
				//argument 1: index (index < cHemiSphereLightMax)
				SGF_FUNCTION_PROPERTY static bool getIsEnabled(uint index)
				{
					return m_pack.isEnableds[index] > 0.0f ? true : false;
				}
				//set light is enabled? <static>(set function property)
				//argument 1: index (index < cHemiSphereLightMax)
				//argument 2: set value
				SGF_FUNCTION_PROPERTY static bool setIsEnabled(uint index, bool isEnabled)
				{
					m_isChangeds[m_cIsChangedHemiSphere] = true;
					return m_pack.isEnableds[index] = IS_TRUE(isEnabled) ? 1.0f : 0.0f;
				}

				//PointLight最大数
				static constexpr uint cHemiSphereLightMax = 1;

			private:
				//渡しやすいように
				struct Pack
				{
					//ライトの位置
					Vector4 directions[cHemiSphereLightMax];
					//ライトの色 (Sky)
					Color colorSkys[cHemiSphereLightMax];
					//ライトの色 (Ground)
					Color colorGrounds[cHemiSphereLightMax];
					//ライトの明るさ倍率 (Sky)
					float powerSkys[cHemiSphereLightMax];
					//ライトの明るさ倍率 (Ground)
					float powerGrounds[cHemiSphereLightMax];
					//ライトが有効か否か
					float isEnableds[cHemiSphereLightMax];
				};

				//value pack
				static Pack m_pack;
			};
		}
	}
}
#endif //!SGFRAMEWORK_HEADER_HEMI_SPHERE_LIGHT_X11_HPP_