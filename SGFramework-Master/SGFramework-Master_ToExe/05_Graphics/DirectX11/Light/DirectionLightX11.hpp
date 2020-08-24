/*----------------------------------------------------------------------------------
DirectX11 DirectionalLight������DirectionalLightX11 class
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_DIRECTION_LIGHT_X11_HPP_
#define SGFRAMEWORK_HEADER_DIRECTION_LIGHT_X11_HPP_
#include "../../../02_CustomVariable/CustomVariable.hpp"
#include "BaseLightX11.hpp"

// Framework namespace
namespace SGFramework
{
	//�t���[�����[�N�̊Ǘ�������Administrator namespace
	namespace Administrator
	{
		//friend
		class GraphicsX11;

		//GraphicsDetail namespace
		namespace GraphicsDetail
		{
			//DirectX11 DirectionalLight������DirectionalLightX11 class
			//mutex�ɂ��ی�͍s��Ȃ��̂Œ���
			class DirectionalLightX11 final : public BaseClass::BaseLightX11
			{
			public:
				//friend
				friend class SGFramework::Administrator::GraphicsX11;
				
				DELETE_ALL_DEFAULT_CLASS_FUNCTIONS(DirectionalLightX11);

				//�Z�b�g����v�f������Element class
				struct Element
				{
					//default
					Element() = default;
					//----------------------------------------------------------------------------------
					//[�R���X�g���N�^]
					//�������������
					//����1: ���C�g�̕���
					//����2: ���C�g�̐F
					//����3: ���C�g�̖��邳�{��
					//����4: ���C�g���L�����ۂ�
					Element(const Vector3& direction, const Color& color, float power, bool isEnabled)
						: direction(direction), color(color), power(power), isEnabled(isEnabled) {}

					Vector3 direction = Const::Vector3::zero;	//���C�g�̕���
					Color color = Const::Color::white;				//���C�g�̐F
					float power = 1.0f;										//���C�g�̖��邳�{��
					bool isEnabled = true;								//���C�g���L�����ۂ�
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
				//argument 2: ���C�g�̕���
				//argument 3: ���C�g�̐F
				//argument 4: ���C�g�̖��邳�{��
				//argument 5: ���C�g���L�����ۂ�, default = true
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

				//DirectionalLight�ő吔
				static constexpr uint cDirectionalLightMax = 4;

			private:
				//�n���₷���悤��
				struct Pack
				{
					//���C�g�̕���
					Vector4 directions[cDirectionalLightMax];
					//���C�g�̐F
					Color colors[cDirectionalLightMax];
					//���C�g�̖��邳�{��
					float powers[cDirectionalLightMax];
					//���C�g���L�����ۂ�
					float isEnableds[cDirectionalLightMax];
				};

				//value pack
				static Pack m_pack;
			};
		}
	}
}
#endif //!SGFRAMEWORK_HEADER_DIRECTION_LIGHT_X11_HPP_