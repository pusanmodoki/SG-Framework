/*----------------------------------------------------------------------------------
DirectX11 PointLight������PointLightX11 class
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_POINT_LIGHT_X11_HPP_
#define SGFRAMEWORK_HEADER_POINT_LIGHT_X11_HPP_
#include "../../../02_CustomVariable/CustomVariable.hpp"
#include "BaseLightX11.hpp"

// Framework namespace
namespace SGFramework
{
	//�t���[�����[�N�̊Ǘ�������Administrator namespace
	namespace Administrator
	{
		//GraphicsDetail namespace
		namespace GraphicsDetail
		{
			//friend
			class GraphicsX11;

			//DirectX11 PointLight������PointLightX11 class
			class PointLightX11 final : public SGFramework::BaseClass::BaseLightX11
			{
			public:
				//friend
				friend class SGFramework::Administrator::GraphicsX11;

				DELETE_ALL_DEFAULT_CLASS_FUNCTIONS(PointLightX11);

				//�Z�b�g����v�f������Element class
				//mutex�ɂ��ی�͍s��Ȃ��̂Œ���
				struct Element
				{
					//default
					Element() = default;
					//----------------------------------------------------------------------------------
					//[�R���X�g���N�^]
					//�������������
					//����1: ���C�g�̈ʒu
					//����2: ���C�g�̐F
					//����3: ���C�g�̖��邳�{��
					//����4: ���C�g���L�����ۂ�
					Element(const Vector3& position, const Color& color, float power, bool isEnabled)
						: position(position), color(color), power(power), isEnabled(isEnabled) {}

					Vector3 position = Const::Vector3::zero;
					Color color = Const::Color::white;				//���C�g�̐F
					float power = 1.0f;										//���C�g�̖��邳�{��
					bool isEnabled = true;								//���C�g���L�����ۂ�
				};


				//now parameters <static>(get function property)
				//argument 1: index (index < cPointLightMax)
				SGF_FUNCTION_PROPERTY static Element getParameter(uint index)
				{
					return Element(m_pack.positions[index], m_pack.colors[index],
						m_pack.powers[index], m_pack.isEnableds[index] > 0.0f ? true : false);
				}
				//set parameters[index] <static>(set function property)
				//return: set complete = true, failed (index < cPointLightMax) = false
				//argument 1: index (index < cPointLightMax)
				//argument 2: ���C�g�̕���
				//argument 3: ���C�g�̐F
				//argument 4: ���C�g�̖��邳�{��
				//argument 5: ���C�g���L�����ۂ�, default = true
				SGF_FUNCTION_PROPERTY static bool setParameter(uint index, const Vector3& position, const Color& color, float power, bool isEnabled = true)
				{
					if (index < cPointLightMax)
					{
						m_pack.positions[index].setVector3(position, 0.0f);
						m_pack.colors[index] = color;
						m_pack.powers[index] = power;
						m_pack.isEnableds[index] = IS_TRUE(isEnabled) ? 1.0f : 0.0f;
						m_isChangeds[m_cIsChangedPoint] = true;
						return true;
					}
					else
						return false;
				}
				//set parameters[index] <static>(set function property)
				//return: set complete = true, failed (index < cPointLightMax) = false
				//argument 1: index (index < cPointLightMax)
				//argument 2: Element
				SGF_FUNCTION_PROPERTY static bool setParameter(uint index, const Element& element)
				{
					if (index < cPointLightMax)
					{
						m_pack.positions[index].setVector3(element.position, 0.0f);
						m_pack.colors[index] = element.color;
						m_pack.powers[index] = element.power;
						m_pack.isEnableds[index] = IS_TRUE(element.isEnabled) ? 1.0f : 0.0f;
						m_isChangeds[m_cIsChangedPoint] = true;
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
						if (i < cPointLightMax && i < size)
						{
							const Element& element = e;
							m_pack.positions[i].setVector3(element.position, 0.0f);
							m_pack.colors[i] = element.color;
							m_pack.powers[i] = element.power;
							m_pack.isEnableds[i] = IS_TRUE(element.isEnabled) ? 1.0f : 0.0f;
							m_isChangeds[m_cIsChangedPoint] = true;
							++i;
						}
						else
							break;
					}
				}

				//light is enabled? <static>(get function property)
				//argument 1: index (index < cPointLightMax)
				SGF_FUNCTION_PROPERTY static bool getIsEnabled(uint index)
				{
					return m_pack.isEnableds[index] > 0.0f ? true : false;
				}
				//set light is enabled? <static>(set function property)
				//argument 1: index (index < cPointLightMax)
				//argument 2: set value
				SGF_FUNCTION_PROPERTY static bool setIsEnabled(uint index, bool isEnabled)
				{
					m_isChangeds[m_cIsChangedPoint] = true;
					return m_pack.isEnableds[index] = isEnabled ? 1.0f : 0.0f;
				}

				//PointLight�ő吔
				static constexpr uint cPointLightMax = 4;

			private:
				//�n���₷���悤��
				struct Pack
				{
					//���C�g�̈ʒu
					Vector4 positions[cPointLightMax];
					//���C�g�̐F
					Color colors[cPointLightMax];
					//���C�g�̖��邳�{��
					float powers[cPointLightMax];
					//���C�g���L�����ۂ�
					float isEnableds[cPointLightMax];
				};

				//value pack
				static Pack m_pack;
			};
		}
	}
}
#endif //!SGFRAMEWORK_HEADER_POINT_LIGHT_X11_HPP_