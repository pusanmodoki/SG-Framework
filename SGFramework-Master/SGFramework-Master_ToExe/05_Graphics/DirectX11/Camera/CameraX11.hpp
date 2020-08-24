/*----------------------------------------------------------------------------------
DirectX11 Camera������CameraX11 class
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_CAMERA_X11_HPP_
#define SGFRAMEWORK_HEADER_CAMERA_X11_HPP_
#include <unordered_map>
#include <vector>
#include <unordered_set>
#include <mutex>
#include "../../../02_CustomVariable/CustomVariable.hpp"
#include "../Transform/TransformX11.hpp"
#include "../GraphicsX11/GraphicsX11.hpp"
#include "../GraphicsObject/GraphicsObjectX11.hpp"

// Framework namespace
namespace SGFramework
{
	//�t���[�����[�N�̊Ǘ�������Administrator namespace
	namespace Administrator
	{
		//GraphicsDetail namespace
		namespace GraphicsDetail
		{
			//DirectX11 Camera������CameraX11 class
			class CameraX11 : public ComponentX11
			{
			public:
				//friend
				friend class SGFramework::Administrator::GraphicsX11;
				friend class SGFramework::Administrator::GraphicsDetail::ComponentX11;

				//�r���[�|�[�g��ݒ肷��ViewPort
				struct ViewPort
				{
					//----------------------------------------------------------------------------------
					//[�R���X�g���N�^]
					//�������������
					//����1: ������W, ��ʃT�C�Y�ɑ΂���0.0f ~ 1.0f�܂ł̔{��
					//����2: �`��T�C�Y, ��ʃT�C�Y�ɑ΂���0.0f ~ 1.0f�܂ł̔{��
					//����3: ��ʊE�[�x�ŏ��l
					//����4: ��ʊE�[�x�ő�l
					inline ViewPort(const Vector2& topLeftPosition = Const::Vector2::zero,
						const Vector2& drawSizePerScreen = Const::Vector2::one,
						float depthMin = 0.0f, float depthMax = 1.0f)
						: topLeftPosition(topLeftPosition), drawSizePerScreen(drawSizePerScreen), depthMin(depthMin), depthMax(depthMax) {}
					//----------------------------------------------------------------------------------
					//[�R���X�g���N�^]
					//�������������
					//����1: �R�s�[
					inline ViewPort(const ViewPort& copy) { memcpy_s(this, sizeof(ViewPort), &copy, sizeof(ViewPort)); }
			
					//----------------------------------------------------------------------------------
					//[ConvertDrawViewPort]	
					//�T�C�Y�{�������ۂ̒l�֕ϊ����`��p��ViewPort�ւƕϊ�����
					//���g�̒l��ύX����̂Œ��ӁICamera�̓o�b�N�O���E���h�Ŏ����␳�������Ă��܂��B
					//Camera�p���N���X�ł�buffer�ɑ΂��Ă̎g�p�͌��ցI
					inline void ConvertDrawViewPort()
					{	
						topLeftPosition.x *= SGFramework::Administrator::GraphicsX11::screen.screenSizeFloat->x;
						topLeftPosition.y *= SGFramework::Administrator::GraphicsX11::screen.screenSizeFloat->y;
						drawSizePerScreen.x *= SGFramework::Administrator::GraphicsX11::screen.screenSizeFloat->x;
						drawSizePerScreen.y *= SGFramework::Administrator::GraphicsX11::screen.screenSizeFloat->y;
					}

					//copy operator
					inline const ViewPort& operator = (const ViewPort* viewPort) { memcpy_s(this, sizeof(ViewPort), viewPort, sizeof(ViewPort)); return *this; }
					//copy operator
					inline const ViewPort& operator = (const ViewPort& viewPort) { memcpy_s(this, sizeof(ViewPort), &viewPort, sizeof(ViewPort)); return *this; }

					Vector2 topLeftPosition;			//������W, ��ʃT�C�Y�ɑ΂���0.0f ~ 1.0f�܂ł̔{��
					Vector2 drawSizePerScreen;	//�`��T�C�Y, ��ʃT�C�Y�ɑ΂���0.0f ~ 1.0f�܂ł̔{��
					float depthMin;						//��ʊE�[�x�ŏ��l
					float depthMax;						//��ʊE�[�x�ő�l
				};

				//----------------------------------------------------------------------------------
				//[StartUp]
				//�J�����̏����ݒ���s��, ��������s���Ȃ��ꍇ����ɓ��삵�Ȃ�
				//����1: �`�惌�C���[�}�X�N, defualt = All layers mask
				//����2: nearClip, default = 1.0f
				//����3: farClip, default = 1000.0f
				//����4: fov, default = MathAF::cPiDiv2
				//����5: �r���[�|�[�g, default = ViewPort()
				inline void StartUp(LayerMask layerMask = LayerMask::getAllLayersMask(),
					float nearClip = 1.0f, float farClip = 1000.0f, 
					float fov = MathAF::cPiDiv2, const ViewPort& viewPort = ViewPort());

				//graphics set camera matrix(graphics thread only property)
				SGF_FUNCTION_PROPERTY void setCameraMatrix();

				//look at(���t���[������������܂�, �ݒ肵�Ȃ������ꍇ��forward��ݒ肵�܂�) (get only property)
				SGF_FUNCTION_PROPERTY Vector3 getLookAt() {  return m_lookAt; }
				//look at(���t���[������������܂�, �ݒ肵�Ȃ������ꍇ��forward��ݒ肵�܂�) (set only property)
				//argument 1: set value
				SGF_FUNCTION_PROPERTY Vector3 setLookAt(const Vector3& set) { m_isLookAt = true; return m_lookAt = set; }

				//view port (get only property)
				SGF_FUNCTION_PROPERTY ViewPort getViewPort() { return m_drawBuffers.viewPort; }
				//view port (set only property)
				//argument 1: set value
				SGF_FUNCTION_PROPERTY ViewPort setViewPort(const ViewPort& set){ return m_drawBuffers.viewPort = set; }

				//view objects layer mask (get only property)
				SGF_FUNCTION_PROPERTY LayerMask getDrawLayerMask() { return m_drawBuffers.layerMask; }
				//view objects layer mask (set only property)
				//argument 1: set value
				SGF_FUNCTION_PROPERTY LayerMask setDrawLayerMask(const LayerMask& set) { return m_drawBuffers.layerMask = set; }

				//near clip (get only property)
				SGF_FUNCTION_PROPERTY float getNearClip() { return m_drawBuffers.nearClip; }
				//near clip (set only property)
				//argument 1: set value
				SGF_FUNCTION_PROPERTY float setNearClip(float set) { return m_drawBuffers.nearClip = set; }

				//far clip (get only property)
				SGF_FUNCTION_PROPERTY float getFarClip() { return m_drawBuffers.farClip; }
				//far clip (set only property)
				//argument 1: set value
				SGF_FUNCTION_PROPERTY float setFarClip(float set) { return m_drawBuffers.farClip = set; }

				//fov (get only property)
				SGF_FUNCTION_PROPERTY float getFov() { return m_drawBuffers.fov; }
				//fov (set only property)
				//argument 1: set value
				SGF_FUNCTION_PROPERTY float setFov(float set) { return m_drawBuffers.fov = set; }

			//protected�p�^�錾
			private:
				//�`��p�o�b�t�@�p�b�N
				struct DrawBuffers
				{
					ViewPort viewPort = ViewPort();		//ViewPort
					LayerMask layerMask = 0;				//Draw objects layer mask
					float nearClip = 0.0f;						//Near Clip
					float farClip = 0.0f;							//Far Clip
					float fov = 0.0f;								//Fov
				};

			//protected
			protected:
				//----------------------------------------------------------------------------------
				//[�R���X�g���N�^]
				CameraX11() : m_drawBuffers(), m_buffersUseGraphics(), m_syncObject(*this),
					m_projectionMatrix(), m_viewMatrix(), m_lookAt(), m_lookAtUseGrapics(),
					m_isCreateProjection(true), m_isCreateView(true), m_isLookAt(false), m_isLookAtUseGrapics(false) {}

				//hidden callbacks
				inline virtual uint EnableCallbacks() { return 0; }
				inline virtual void Awake() {}
				inline virtual void OnEnable() {}
				inline virtual void Start() {}
				inline virtual void Update() {}
				inline virtual void Sync() {}
				inline virtual void LateUpdate() {}
				inline virtual void FixedUpdate() {}
				inline virtual void MakeDrawCommand() {}
				inline virtual void OnDisable() {}
				inline virtual void OnDestroy() {}
				inline virtual void OnCollisionEnter(const ContactInfo& contactInfo) {}
				inline virtual void OnCollisionStay(const ContactInfo& contactInfo) {}
				inline virtual void OnCollisionExit(const ContactInfo& contactInfo) {}
				inline virtual void OnTriggerEnter(const ContactInfo& contactInfo) {}
				inline virtual void OnTriggerStay(const ContactInfo& contactInfo) {}
				inline virtual void OnTriggerExit(const ContactInfo& contactInfo) {}
				inline virtual void ChangeUpperHierarchy(WeakPointer<TransformX11> newParent, WeakPointer<TransformX11> oldParent) {}

				//Draw�p�ݒ� (LookAt�͕ʓr�֐�������s)
				DrawBuffers m_drawBuffers;

			private:
				//Sync�p�I�u�W�F�N�g
				struct SyncObject : GraphicsObjectX11
				{
					//----------------------------------------------------------------------------------
					//[Constructor]
					inline SyncObject(CameraX11& reference) : reference(reference) {}

					//----------------------------------------------------------------------------------
					//[Sync]
					//���𓯊�����, �R�[���o�b�N�֐�
					inline void Sync()
					{
						memcpy_s(&reference.m_buffersUseGraphics, sizeof(DrawBuffers),
							&reference.m_drawBuffers, sizeof(DrawBuffers));
						reference.m_lookAtUseGrapics = reference.m_lookAt;
						reference.m_isLookAtUseGrapics = reference.m_isLookAt;

						reference.m_buffersUseGraphics.viewPort.ConvertDrawViewPort();

						reference.m_lookAt = Const::Vector3::zero;
						reference.m_isLookAt = false;
						AtomicOperation::Init(reference.m_isCreateProjection, false);
						AtomicOperation::Init(reference.m_isCreateView, false);
					}

					CameraX11& reference;	//reference this camera
				};

				//----------------------------------------------------------------------------------
				//[InitCameraSetting]
				//�J�����̃Z�b�g��������������
				inline static void InitCameraSetting() { m_setCamera = nullptr; m_setLayerMask = 0; }
	
				static CameraX11* m_setCamera;			//�Ō�ɐݒ肵���J����
				static LayerMask m_setLayerMask;			//�Ō�ɐݒ肵�����C���[�}�X�N

				DrawBuffers m_buffersUseGraphics;		//Buffer (Use graphics thread)
				SyncObject m_syncObject;						//Sync�p
				Vector4x4 m_projectionMatrix;				//Projection
				Vector4x4 m_viewMatrix;						//View
				Vector3 m_lookAt;									//LookAt
				Vector3 m_lookAtUseGrapics;					//LookAt (Use graphics)
				std::atomic_bool m_isCreateProjection;	//Create Projection?
				std::atomic_bool m_isCreateView;			//Create View?
				bool m_isLookAt;									//LookAt?
				bool m_isLookAtUseGrapics;					//LookAt? (Use graphics)
			};

			//----------------------------------------------------------------------------------
			//[StartUp]
			//�J�����̏����ݒ���s��, ��������s���Ȃ��ꍇ����ɓ��삵�Ȃ�
			//����1: �`�惌�C���[�}�X�N, defualt = All layers mask
			//����2: LookAt���s���Ȃ������t���[���Ɏ��s�����LookAt = forward * this�̒l, default = 10.0f
			//����3: nearClip, default = 1.0f
			//����4: farClip, default = 1000.0f
			//����5: fov, default = MathAF::cPiDiv2
			//����6: �r���[�|�[�g, default = ViewPort()
			inline void CameraX11::StartUp(LayerMask layerMask, float nearClip, 
				float farClip, float fov, const ViewPort & viewPort)
			{
				m_drawBuffers.viewPort = viewPort;
				m_drawBuffers.layerMask = layerMask;
				m_drawBuffers.nearClip = nearClip;
				m_drawBuffers.farClip = farClip;
				m_drawBuffers.fov = fov;


				AtomicOperation::Init(m_isCreateProjection, false);
				AtomicOperation::Init(m_isCreateView, false);
			}

			//graphics set camera matrix(graphics thread only property)
			SGF_FUNCTION_PROPERTY void CameraX11::setCameraMatrix()
			{
				using GraphicsX11 = SGFramework::Administrator::GraphicsX11;
				
				//���łɐݒ�ς݂Ȃ�ΏI��
				if (m_setCamera == this) return;

				//�ݒ�
				m_setCamera = this;
				m_setLayerMask = m_buffersUseGraphics.layerMask;

				//Create Projection
				if (IS_FALSE(m_isCreateProjection.load()))
				{
					m_projectionMatrix = Matrix::ProjectionMatrix(m_buffersUseGraphics.fov,
						m_buffersUseGraphics.viewPort.drawSizePerScreen.x / m_buffersUseGraphics.viewPort.drawSizePerScreen.y,
						m_buffersUseGraphics.nearClip, m_buffersUseGraphics.farClip);

					AtomicOperation::Init(m_isCreateProjection, true);
				}

				//Create View
				if (IS_FALSE(m_isCreateView.load()))
				{
					//LookAt
					if (IS_TRUE(m_isLookAtUseGrapics))
						m_viewMatrix = Matrix::ViewMatrix(transform->m_drawMatrix.getTranslate(), 
							m_lookAt, transform->m_drawMatrix.getUp());
					//Auto Focus
					else
						m_viewMatrix = Matrix::ViewMatrixForDirection(transform->m_drawMatrix.getTranslate(),
							transform->m_drawMatrix.getForward(), 
							transform->m_drawMatrix.getUp());

					AtomicOperation::Init(m_isCreateView, true);
				}


				//Set projection matrix
				GraphicsX11::m_setConstantBufferScene.projectionMatrix = m_projectionMatrix.getTranspose();
				//Set view matrix
				GraphicsX11::m_setConstantBufferScene.viewMatrix = m_viewMatrix.getTranspose();

				//Set Light params
				//Vector4�Ȃ̂Œ��ڎ擾
				GraphicsX11::m_setConstantBufferLight.eyePosition = transform->m_drawMatrix.getVector4(3);
				GraphicsX11::m_setConstantBufferLight.eyeDirection = transform->m_drawMatrix.getVector4(2);	


				//Update constant buffer
				SGFramework::Administrator::GraphicsX11::context->UpdateSubresource(GraphicsX11::m_constantBufferScene,
					0, nullptr, &GraphicsX11::m_setConstantBufferScene, 0, 0);
				SGFramework::Administrator::GraphicsX11::context->UpdateSubresource(GraphicsX11::m_constantBufferLight,
					0, nullptr, &GraphicsX11::m_setConstantBufferLight, 0, 0);


				//Set constant buffer
				GraphicsX11::context->VSSetConstantBuffers(GraphicsX11::m_cConstantBufferSceneSlot,
					1, &GraphicsX11::m_constantBufferScene);
				GraphicsX11::context->PSSetConstantBuffers(GraphicsX11::m_cConstantBufferSceneSlot,
					1, &GraphicsX11::m_constantBufferScene);
				//Set constant buffer
				GraphicsX11::context->PSSetConstantBuffers(GraphicsX11::m_cConstantBufferLightSlot,
					1, &GraphicsX11::m_constantBufferLight);
				//Set view port
				GraphicsX11::context->RSSetViewports(1, reinterpret_cast<D3D11_VIEWPORT*>(&m_buffersUseGraphics.viewPort));
			}
		}
	}
}
#endif //!SGFRAMEWORK_HEADER_CAMERA_X11_HPP_