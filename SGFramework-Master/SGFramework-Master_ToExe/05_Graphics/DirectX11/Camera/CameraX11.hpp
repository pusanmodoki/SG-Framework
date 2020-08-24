/*----------------------------------------------------------------------------------
DirectX11 Cameraを扱うCameraX11 class
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
	//フレームワークの管理をするAdministrator namespace
	namespace Administrator
	{
		//GraphicsDetail namespace
		namespace GraphicsDetail
		{
			//DirectX11 Cameraを扱うCameraX11 class
			class CameraX11 : public ComponentX11
			{
			public:
				//friend
				friend class SGFramework::Administrator::GraphicsX11;
				friend class SGFramework::Administrator::GraphicsDetail::ComponentX11;

				//ビューポートを設定するViewPort
				struct ViewPort
				{
					//----------------------------------------------------------------------------------
					//[コンストラクタ]
					//代入初期化する
					//引数1: 左上座標, 画面サイズに対する0.0f ~ 1.0fまでの倍率
					//引数2: 描画サイズ, 画面サイズに対する0.0f ~ 1.0fまでの倍率
					//引数3: 被写界深度最小値
					//引数4: 被写界深度最大値
					inline ViewPort(const Vector2& topLeftPosition = Const::Vector2::zero,
						const Vector2& drawSizePerScreen = Const::Vector2::one,
						float depthMin = 0.0f, float depthMax = 1.0f)
						: topLeftPosition(topLeftPosition), drawSizePerScreen(drawSizePerScreen), depthMin(depthMin), depthMax(depthMax) {}
					//----------------------------------------------------------------------------------
					//[コンストラクタ]
					//代入初期化する
					//引数1: コピー
					inline ViewPort(const ViewPort& copy) { memcpy_s(this, sizeof(ViewPort), &copy, sizeof(ViewPort)); }
			
					//----------------------------------------------------------------------------------
					//[ConvertDrawViewPort]	
					//サイズ倍率を実際の値へ変換し描画用のViewPortへと変換する
					//自身の値を変更するので注意！Cameraはバックグラウンドで自動補正をかけています。
					//Camera継承クラスでのbufferに対しての使用は厳禁！
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

					Vector2 topLeftPosition;			//左上座標, 画面サイズに対する0.0f ~ 1.0fまでの倍率
					Vector2 drawSizePerScreen;	//描画サイズ, 画面サイズに対する0.0f ~ 1.0fまでの倍率
					float depthMin;						//被写界深度最小値
					float depthMax;						//被写界深度最大値
				};

				//----------------------------------------------------------------------------------
				//[StartUp]
				//カメラの初期設定を行う, これを実行しない場合正常に動作しない
				//引数1: 描画レイヤーマスク, defualt = All layers mask
				//引数2: nearClip, default = 1.0f
				//引数3: farClip, default = 1000.0f
				//引数4: fov, default = MathAF::cPiDiv2
				//引数5: ビューポート, default = ViewPort()
				inline void StartUp(LayerMask layerMask = LayerMask::getAllLayersMask(),
					float nearClip = 1.0f, float farClip = 1000.0f, 
					float fov = MathAF::cPiDiv2, const ViewPort& viewPort = ViewPort());

				//graphics set camera matrix(graphics thread only property)
				SGF_FUNCTION_PROPERTY void setCameraMatrix();

				//look at(毎フレーム初期化されます, 設定しなかった場合はforwardを設定します) (get only property)
				SGF_FUNCTION_PROPERTY Vector3 getLookAt() {  return m_lookAt; }
				//look at(毎フレーム初期化されます, 設定しなかった場合はforwardを設定します) (set only property)
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

			//protected用型宣言
			private:
				//描画用バッファパック
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
				//[コンストラクタ]
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

				//Draw用設定 (LookAtは別途関数から実行)
				DrawBuffers m_drawBuffers;

			private:
				//Sync用オブジェクト
				struct SyncObject : GraphicsObjectX11
				{
					//----------------------------------------------------------------------------------
					//[Constructor]
					inline SyncObject(CameraX11& reference) : reference(reference) {}

					//----------------------------------------------------------------------------------
					//[Sync]
					//情報を同期する, コールバック関数
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
				//カメラのセット情報を初期化する
				inline static void InitCameraSetting() { m_setCamera = nullptr; m_setLayerMask = 0; }
	
				static CameraX11* m_setCamera;			//最後に設定したカメラ
				static LayerMask m_setLayerMask;			//最後に設定したレイヤーマスク

				DrawBuffers m_buffersUseGraphics;		//Buffer (Use graphics thread)
				SyncObject m_syncObject;						//Sync用
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
			//カメラの初期設定を行う, これを実行しない場合正常に動作しない
			//引数1: 描画レイヤーマスク, defualt = All layers mask
			//引数2: LookAtが行われなかったフレームに実行されるLookAt = forward * thisの値, default = 10.0f
			//引数3: nearClip, default = 1.0f
			//引数4: farClip, default = 1000.0f
			//引数5: fov, default = MathAF::cPiDiv2
			//引数6: ビューポート, default = ViewPort()
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
				
				//すでに設定済みならば終了
				if (m_setCamera == this) return;

				//設定
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
				//Vector4なので直接取得
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