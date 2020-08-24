/*----------------------------------------------------------------------------------
Meshを描画するMeshRenderer class (Component)
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_MODEL_MESH_RENDERER_HPP_
#define SGFRAMEWORK_HEADER_MODEL_MESH_RENDERER_HPP_
#include "../Transform/TransformX11.hpp"
#include "MeshX11.hpp"
#include "../GraphicsObject/GraphicsObjectX11.hpp"
#include "../GraphicsX11/GraphicsX11.hpp"

// Framework namespace
namespace SGFramework
{
	//フレームワークの管理をするAdministrator namespace
	namespace Administrator
	{
		//GraphicsDetail namespace
		namespace GraphicsDetail
		{
			//Meshを描画するMeshRenderer class (Component)
			//template 1: Mesh->VertexType
			//template 2: Mesh->MaterialPointerType
			template<class MeshVertexType, class MeshMaterialPointerType>
			class MeshRendererX11 : public ComponentX11
			{
			public:
				inline MeshRendererX11() : ComponentX11(), m_pointerSync(*this) {}

				//----------------------------------------------------------------------------------
				//[InitDrawInfos]
				//描画に必要な情報をすべて初期化する
				//引数1: mesh
				//引数2: draw priority, default = 0
				//引数3: is drawing component callback, default = true
				inline void InitDrawInfos(SharedPointer
					<MeshX11<MeshVertexType, MeshMaterialPointerType>> mesh, uint priority = 0, bool isAutoDraw = true)
				{
					m_mesh = std::move(mesh);
					AtomicOperation::Init(m_priority, priority);
					
					m_isDrawCall = isAutoDraw;
					ChangeCallbackFlags(CallbackFunctions::MakeDrawCommand, m_isDrawCall);	
				}

				//----------------------------------------------------------------------------------
				//[DrawManual]
				//手動でDarwを行う, Make Draw Command->SendDrawFunction用
				inline void DrawManual()
				{
					m_meshUsingGraphics->Draw();
				}

				//draw mesh data (read function property)
				SGF_FUNCTION_PROPERTY SharedPointer
					<MeshX11<MeshVertexType, MeshMaterialPointerType>> getMeshData() const
				{
					return m_mesh; 
				}
				//draw mesh data (set function property)
				//argument 1: set value
				SGF_FUNCTION_PROPERTY SharedPointer
					<MeshX11<MeshVertexType, MeshMaterialPointerType>> setMeshData(SharedPointer<MeshX11<int, int>> set)
				{
					return m_mesh = std::move(set); 
				}

				//draw priority (read function property)
				SGF_FUNCTION_PROPERTY uint getDrawPriority() const { return m_priority.load(); }
				//draw priority (set function property)
				//argument 1: set value
				SGF_FUNCTION_PROPERTY uint setDrawPriority(uint set) { return AtomicOperation::Init(m_priority, set); }

				//is drawing component callback (read function property)
				SGF_FUNCTION_PROPERTY bool getIsAutoDrawCall() const { return m_isDrawCall; }
				//is drawing component callback (set function property)
				//argument 1: set value
				SGF_FUNCTION_PROPERTY bool setIsAutoDrawCall(bool set) { ChangeCallbackFlags(CallbackFunctions::MakeDrawCommand, set); return m_isDrawCall = set; }

			private:
				//ポインタ同期に使用するPointerSyncObject class
				class PointerSyncObject : public GraphicsObjectX11
				{
				public:
					inline PointerSyncObject(MeshRendererX11& reference) : m_reference(reference) {}

				private:
					//----------------------------------------------------------------------------------
					//[Sync]
					//情報を同期する, コールバック関数
					void Sync() override 
					{
						if (m_reference.m_mesh.getIsValid())
							m_reference.m_meshUsingGraphics = std::move(m_reference.m_mesh);
					}

					MeshRendererX11& m_reference;
				};

				//----------------------------------------------------------------------------------
				//[EnabledCallbacks]
				//return: 自身がCallされるCallback Flags, Component::CallbackFunctions参照
				inline uint EnableCallbacks() override { return CallbackFunctions::MakeDrawCommand; }
				//----------------------------------------------------------------------------------
				//[MakeDrawCommand]
				//Update, Sync, LateUpdate ブロック後にCallbackされる,
				//		DrawCommandを作成するための関数
				inline void MakeDrawCommand() override
				{
					if (m_meshUsingGraphics.getIsValid() & getIsActiveAndEnabled())
					{
						Administrator::GraphicsX11::SendDrawFunction(
							[this]() { transform->setDrawMatrix(); m_meshUsingGraphics->Draw(); }, 
							m_priority.load());
					}
				}

				//hidden
				inline void Awake() override {}
				inline void OnEnable() override {}
				inline void Start() override {}
				inline void Update() override {}
				inline void Sync() override {}
				inline void LateUpdate() override {}
				inline void FixedUpdate() override {}
				inline void OnDisable() override {}
				inline void OnDestroy() override {}
				inline void OnCollisionEnter(const ContactInfo& contactInfo) override {}
				inline void OnCollisionStay(const ContactInfo& contactInfo) override {}
				inline void OnCollisionExit(const ContactInfo& contactInfo) override {}
				inline void OnTriggerEnter(const ContactInfo& contactInfo) override {}
				inline void OnTriggerStay(const ContactInfo& contactInfo) override {}
				inline void OnTriggerExit(const ContactInfo& contactInfo) override {}
				inline void ChangeUpperHierarchy(WeakPointer<TransformX11> newParent, WeakPointer<TransformX11> oldParent) override {}


				//mesh (using graphics thread)
				SharedPointer<MeshX11<MeshVertexType, MeshMaterialPointerType>> m_meshUsingGraphics;
				//mesh
				SharedPointer<MeshX11<MeshVertexType, MeshMaterialPointerType>> m_mesh;
				//ポインタ同期に使用
				PointerSyncObject m_pointerSync;
				//draw priority
				std::atomic<uint> m_priority = 0;
				//this draw call?
				bool m_isDrawCall = true;
			};
		}
	}
}

#endif //!SGFRAMEWORK_HEADER_MODEL_MESH_RENDERER_HPP_