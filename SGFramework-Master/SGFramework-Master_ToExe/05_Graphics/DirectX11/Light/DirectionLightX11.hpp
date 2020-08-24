/*----------------------------------------------------------------------------------
DirectX11 DirectionalLightを扱うDirectionalLightX11 class
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_DIRECTION_LIGHT_X11_HPP_
#define SGFRAMEWORK_HEADER_DIRECTION_LIGHT_X11_HPP_
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
			//DirectX11 DirectionalLightを扱うDirectionalLightX11 class
			//mutexによる保護は行わないので注意
			class DirectionalLightX11 final : public BaseClass::BaseLightX11
			{
			public:
				//friend
				friend class SGFramework::Administrator::GraphicsX11;
				
				DELETE_ALL_DEFAULT_CLASS_FUNCTIONS(DirectionalLightX11);

				//セットする要素を内包するElement class
				struct Element
				{
					//default
					Element() = default;
					//----------------------------------------------------------------------------------
					//[コンストラクタ]
					//代入初期化する
					//引数1: ライトの方向
					//引数2: ライトの色
					//引数3: ライトの明るさ倍率
					//引数4: ライトが有効か否か
					Element(const Vector3& direction, const Color& color, float power, bool isEnabled)
						: direction(direction), color(color), power(power), isEnabled(isEnabled) {}

					Vector3 direction = Const::Vector3::zero;	//ライトの方向
					Color color = Const::Color::white;				//ライトの色
					float power = 1.0f;										//ライトの明るさ倍率
					bool isEnabled = true;								//ライトが有効か否か
				};


				//now parameters <static>(get function property)
				//argument 1: index (index < cDirectionalLightMax)
				SGF_FUNCTION_PROPERTY static Element getParameter(uint index)
				{
					return index < cDirectionalLightMax ?
						Element(m_pack.directions[index], m_pack.colors[index],
							m_pack.powers[index], m_pack.isEnableds[index] > 0.0f ? true : false)
						: Element(Const::Vector3::zero , Const::Color::black, 0.0f, false);
				}
				//set parameters[index] <static>(set function property)
				//return: set complete = true, failed (index < cDirectionalLightMax) = false
				//argument 1: index (index < cDirectionalLightMax)
				//argument 2: ライトの方向
				//argument 3: ライトの色
				//argument 4: ライトの明るさ倍率
				//argument 5: ライトが有効か否か, default = true
				SGF_FUNCTION_PROPERTY static bool setParameter(uint index, const Vector3& direction, const Color& color, float power, bool isEnabled = true)
				{
					if (index < cDirectionalLightMax)
					{
						m_pack.directions[index].setVector3((direction * Vector3(1.0f, -1.0f, 1.0f)).Normalized(), 0.0f);
						m_pack.colors[index] = color;
						m_pack.powers[index] = power;
						m_pack.isEnableds[index] = IS_TRUE(isEnabled) ? 1.0f : 0.0f;
						m_isChangeds[m_cIsChangedDirection] = true;
						return true;
					}
					else
						return false;
				}
				//set parameters[index] <static>(set function property)
				//return: set complete = true, failed (index < cDirectionalLightMax) = false
				//argument 1: index (index < cDirectionalLightMax)
				//argument 2: Element
				SGF_FUNCTION_PROPERTY static bool setParameter(uint index, const Element& element)
				{
					if (index < cDirectionalLightMax)
					{
						m_pack.directions[index].setVector3((element.direction * Vector3(1.0f, -1.0f, 1.0f)).Normalized(), 0.0f);
						m_pack.colors[index] = element.color;
						m_pack.powers[index] = element.power;
						m_pack.isEnableds[index] = IS_TRUE(element.isEnabled) ? 1.0f : 0.0f;
						m_isChangeds[m_cIsChangedDirection] = true;
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
						if (i < cDirectionalLightMax && i < size)
						{
							const Element& element = e;
							m_pack.directions[i].setVector3((element.direction * Vector3(1.0f, -1.0f, 1.0f)).Normalized(), 0.0f);
							m_pack.colors[i] = element.color;
							m_pack.powers[i] = element.power;
							m_pack.isEnableds[i] = IS_TRUE(element.isEnabled) ? 1.0f : 0.0f;
							m_isChangeds[m_cIsChangedDirection] = true;
							++i;
						}
						else
							break;
					}
				}

				//light is enabled? <static>(get function property)
				//argument 1: index (index < cDirectionalLightMax)
				SGF_FUNCTION_PROPERTY static bool getIsEnabled(uint index)
				{
					return index < cDirectionalLightMax && m_pack.isEnableds[index] > 0.0f ? true : false;
				}
				//set light is enabled? <static>(set function property)
				//argument 1: index (index < cDirectionalLightMax)
				//argument 2: set value
				SGF_FUNCTION_PROPERTY static void setIsEnabled(uint index, bool isEnabled)
				{
					m_isChangeds[m_cIsChangedDirection] = true;
					m_pack.isEnableds[index] = IS_TRUE(isEnabled) ? 1.0f : 0.0f;
				}

				//DirectionalLight最大数
				static constexpr uint cDirectionalLightMax = 4;

			private:
				//渡しやすいように
				struct Pack
				{
					//ライトの方向
					Vector4 directions[cDirectionalLightMax];
					//ライトの色
					Color colors[cDirectionalLightMax];
					//ライトの明るさ倍率
					float powers[cDirectionalLightMax];
					//ライトが有効か否か
					float isEnableds[cDirectionalLightMax];
				};

				//value pack
				static Pack m_pack;
			};
		}
	}
}
#endif //!SGFRAMEWORK_HEADER_DIRECTION_LIGHT_X11_HPP_