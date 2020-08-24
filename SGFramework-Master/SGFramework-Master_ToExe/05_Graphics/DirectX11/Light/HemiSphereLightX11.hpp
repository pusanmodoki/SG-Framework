/*----------------------------------------------------------------------------------
DirectX11 HemiSphereLight������HemiSphereX11 class
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_HEMI_SPHERE_LIGHT_X11_HPP_
#define SGFRAMEWORK_HEADER_HEMI_SPHERE_LIGHT_X11_HPP_
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
			//DirectX11 HemiSphereLight������HemiSphereX11 class
			//mutex�ɂ��ی�͍s��Ȃ��̂Œ���
			class HemiSphereLightX11 final : public SGFramework::BaseClass::BaseLightX11
			{
			public:
				//friend
				friend class SGFramework::Administrator::GraphicsX11;

				DELETE_ALL_DEFAULT_CLASS_FUNCTIONS(HemiSphereLightX11);

				//�Z�b�g����v�f������Element class
				struct Element
				{
					//default
					Element() = default;
					//----------------------------------------------------------------------------------
					//[�R���X�g���N�^]
					//�������������
					//����1: ���C�g�̈ʒu
					//����2: ���C�g�̐F (��)
					//����3: ���C�g�̐F (�n�ʑ�)
					//����4: ���C�g�̖��邳�{�� (��)
					//����5: ���C�g�̖��邳�{�� (�n�ʑ�)
					//����6: ���C�g���L�����ۂ�
					Element(const Vector3& direction, const Color& colorSky, const Color& colorGround,
						float powerSky, float powerGround, bool isEnabled)
						: direction(direction), colorSky(colorSky), colorGround(colorGround),
						powerSky(powerSky), powerGround(powerGround), isEnabled(isEnabled) {}

					Vector3 direction = Const::Vector3::zero;
					Color colorSky = Const::Color::white;			//���C�g�̐F (Sky)
					Color colorGround = Const::Color::white;	//���C�g�̐F (Ground)
					float powerSky = 1.0f;									//���C�g�̖��邳�{�� (Sky)
					float powerGround = 1.0f;							//���C�g�̖��邳�{�� (Ground)
					bool isEnabled = true;								//���C�g���L�����ۂ�
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
				//����2: ���C�g�̈ʒu
				//����3: ���C�g�̐F (��)
				//����4: ���C�g�̐F (�n�ʑ�)
				//����5: ���C�g�̖��邳�{�� (��)
				//����6: ���C�g�̖��邳�{�� (�n�ʑ�)
				//����7: ���C�g���L�����ۂ�, default = true
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

				//PointLight�ő吔
				static constexpr uint cHemiSphereLightMax = 1;

			private:
				//�n���₷���悤��
				struct Pack
				{
					//���C�g�̈ʒu
					Vector4 directions[cHemiSphereLightMax];
					//���C�g�̐F (Sky)
					Color colorSkys[cHemiSphereLightMax];
					//���C�g�̐F (Ground)
					Color colorGrounds[cHemiSphereLightMax];
					//���C�g�̖��邳�{�� (Sky)
					float powerSkys[cHemiSphereLightMax];
					//���C�g�̖��邳�{�� (Ground)
					float powerGrounds[cHemiSphereLightMax];
					//���C�g���L�����ۂ�
					float isEnableds[cHemiSphereLightMax];
				};

				//value pack
				static Pack m_pack;
			};
		}
	}
}
#endif //!SGFRAMEWORK_HEADER_HEMI_SPHERE_LIGHT_X11_HPP_