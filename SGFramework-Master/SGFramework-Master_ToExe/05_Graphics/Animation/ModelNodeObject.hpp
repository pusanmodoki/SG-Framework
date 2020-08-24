#ifndef SGFRAMEWORK_HEADER_MODEL_NODE_OBJECT_HPP_
#define SGFRAMEWORK_HEADER_MODEL_NODE_OBJECT_HPP_
#include "../TransformUsing.hpp"
#include "../../11_GameObject/GameObject.hpp"
#include "../Vertex/Vertex.hpp"
#include "../MeshUsing.hpp"
#include "../MaterialUsing.hpp"
#include "assimp/matrix4x4.h"

// Framework namespace
namespace SGFramework
{
	//あ
	class ModelNodeObject : public GameObject
	{
	public:
		friend Asset::ModelAsset;
		friend class Animator;
		friend class Animation;

		inline ModelNodeObject() : m_sync(*this) {}

		SGF_FUNCTION_PROPERTY Vector4x4 getGraphicsBoneMatrix() { return m_graphicsMatrix; }
		SGF_FUNCTION_PROPERTY int getNodeID() { return m_nodeID; }
		SGF_FUNCTION_PROPERTY bool getIsRegisterAnimationBone() { return m_isSetAnimationMatrix; }

	private:
		struct SyncGraphicsObject : public  Administrator::GraphicsDetail::GraphicsObjectX11
		{
		public:
			inline SyncGraphicsObject(ModelNodeObject& reference) : reference(reference) {}

			void Sync() override
			{
				memcpy_s(&reference.m_graphicsMatrix, sizeof(Vector4x4), &reference.m_boneWorldMatrix, sizeof(Vector4x4));
			}
			ModelNodeObject& reference;
		};


		inline bool RegisterAnimationMatrix(const aiMatrix4x4& offsetMatrix)
		{
			if (m_isSetAnimationMatrix)
			{
				Vector4x4 test;
				memcpy_s(&test, sizeof(Vector4x4), &offsetMatrix, sizeof(aiMatrix4x4));
				test.setTranspose();

				return false;
			}
			memcpy_s(&m_animationOffsetMatrix, sizeof(aiMatrix4x4), &offsetMatrix, sizeof(aiMatrix4x4));
			
			m_isSetAnimationMatrix = true;
			return true;
		}

		WeakPointer<MeshRenderer<Vertex::Model, Material*>> AddMeshRenderer(SharedPointer<Mesh<Vertex::Model, Material*>> meshPointer, uint priority)
		{
			auto component = AddComponent<MeshRenderer<Vertex::Model, Material*>>();

			component->InitDrawInfos(meshPointer, priority, false);
			return std::move(component);
		}


		//hidden
		inline uint EnableCallbacks() override { return 0; }
		inline void Awake() override {}
		inline void OnEnable() override {}
		inline void Start() override {}
		inline void Update() override {}
		inline void Sync() override {}
		inline void LateUpdate() override {}
		inline void FixedUpdate() override {}
		inline void MakeDrawCommand() override {}
		inline void OnDisable() override {}
		inline void OnDestroy() override {}
		inline void OnCollisionEnter(const ContactInfo& contactInfo) override {}
		inline void OnCollisionStay(const ContactInfo& contactInfo) override {}
		inline void OnCollisionExit(const ContactInfo& contactInfo) override {}
		inline void OnTriggerEnter(const ContactInfo& contactInfo) override {}
		inline void OnTriggerStay(const ContactInfo& contactInfo) override {}
		inline void OnTriggerExit(const ContactInfo& contactInfo) override {}
		inline void ChangeUpperHierarchy(WeakPointer<Transform> newParent, WeakPointer<Transform> oldParent) override {}

		SyncGraphicsObject m_sync;
		aiMatrix4x4 m_animationOffsetMatrix = aiMatrix4x4();
		aiMatrix4x4 m_animationBoneMatrix = aiMatrix4x4();
		aiMatrix4x4 m_boneWorldMatrix = aiMatrix4x4();
		Vector4x4 m_graphicsMatrix = Const::Vector4x4::identity;
		int m_nodeID = 0;
		bool m_isSetAnimationMatrix = false;
	};
}

#endif //!SGFRAMEWORK_HEADER_MODEL_NODE_OBJECT_HPP_