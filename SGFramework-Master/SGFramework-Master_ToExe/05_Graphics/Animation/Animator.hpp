#ifndef SGFRAMEWORK_HEADER_ANIMATOR_HPP_
#define SGFRAMEWORK_HEADER_ANIMATOR_HPP_
#include "../GraphicsUsing.hpp"
#include "../TransformUsing.hpp"
#include "../ModelAssetUsing.hpp"
#include "ModelNodeObject.hpp"
#include "Animation.hpp"

// Framework namespace
namespace SGFramework
{
	class Animator : public Component
	{
	public:
		friend Asset::ModelAsset;

		//----------------------------------------------------------------------------------
		//[EnabledCallbacks]
		//return: 自身がCallされるCallback Flags, Component::CallbackFunctions参照
		inline virtual uint EnableCallbacks() { return CallbackFunctions::Update | CallbackFunctions::MakeDrawCommand; }
		//----------------------------------------------------------------------------------
		//[Update]
		//毎フレームUpdateブロックでCallbackされる関数
		inline virtual void Update() 
		{
			if (m_player)
			{
				m_player->Update();
				
				aiMatrix4x4 matrix = aiMatrix4x4();

				UpdateMatrix(m_scene->mRootNode, matrix);
			}
		}
		void UpdateMatrix(aiNode* node, aiMatrix4x4& worldMatrix)
		{
			auto& bone = m_animationObjects.find(node->mName.C_Str())->second;
			aiMatrix4x4 myWorldMatrix = worldMatrix * bone->m_animationBoneMatrix;
	
			bone->m_boneWorldMatrix = myWorldMatrix * bone->m_animationOffsetMatrix;
			
			for (uint i = 0; i < node->mNumChildren; ++i)
				UpdateMatrix(node->mChildren[i], myWorldMatrix);
		}

		void MakeDrawCommand() override
		{
			Graphics::SendDrawFunction([this]()
			{
				if (m_player)
				{
					for (auto& e : m_animationObjects)
					{
						if (IS_TRUE(e.second->getIsRegisterAnimationBone()))
							memcpy_s(&Graphics::m_setConstantBufferModel.bones[e.second->getNodeID()], sizeof(Vector4x4),
								&e.second->getGraphicsBoneMatrix(), sizeof(Vector4x4));
					}

					Graphics::context->UpdateSubresource(Graphics::m_constantBufferModel,
						0, nullptr, &Graphics::m_setConstantBufferModel, 0, 0);

					Graphics::context->VSSetConstantBuffers(Graphics::m_cConstantBufferModelSlot,
						1, &Graphics::m_constantBufferModel);

					Graphics::context->PSSetConstantBuffers(Graphics::m_cConstantBufferModelSlot,
						1, &Graphics::m_constantBufferModel);

					transform->setDrawMatrix();

					for (auto& e : m_meshRenderers)
						e->DrawManual();
				}
				else
				{
					for (auto& e : m_animationObjects)
					{
						if (IS_TRUE(e.second->getIsRegisterAnimationBone()))
							memcpy_s(&Graphics::m_setConstantBufferModel.bones[e.second->getNodeID()], sizeof(Vector4x4),
								&e.second->getGraphicsBoneMatrix(), sizeof(Vector4x4));
					}

					Graphics::context->UpdateSubresource(Graphics::m_constantBufferModel,
						0, nullptr, &Graphics::m_setConstantBufferModel, 0, 0);

					Graphics::context->VSSetConstantBuffers(Graphics::m_cConstantBufferModelSlot,
						1, &Graphics::m_constantBufferModel);

					Graphics::context->PSSetConstantBuffers(Graphics::m_cConstantBufferModelSlot,
						1, &Graphics::m_constantBufferModel);

					transform->setDrawMatrix();
					
					for (auto& e : m_meshRenderers)
						e->DrawManual();
				}
			}, m_priority);
		}

		void PlayAnimation(const sgstring& name)
		{
			auto find = m_animations.find(name);
			if (find != m_animations.end())
			{
				find->second.Play();
				m_player = &find->second;
			}
		}

		void PlayAnimation(uint index)
		{
			uint i = 0;

			for (auto& e : m_animations)
			{
				if (i == index)
				{
					e.second.Play();
					m_player = &e.second;
					break;
				}
				++i;
			}
		}

		void StopAnimation()
		{
			if (m_player)
			{
				m_player->m_playSecondsCounter = 0.0f;
				m_player = nullptr;
			}
		}

		void AddAnimation(const sgstring& name, aiAnimation* animation)
		{
			AtomicOperation::LockGuard guard(m_lock);
			m_animations.try_emplace(name, std::move(Animation(name, animation, transform, &m_animationObjects)));
		}

		void RemoveAnimation(const sgstring& name)
		{
			AtomicOperation::LockGuard guard(m_lock);
			m_animations.erase(name);
		}


	private:
		WeakPointer<ModelNodeObject> AddAnimationObject(SharedPointer<Transform> parent,
			const aiMatrix4x4& transfomation,  std::string& name, bool isCreateCopyName)
		{
			AtomicOperation::LockGuard guard(m_lock);
			
			sgstring sgName(name);
			if (IS_TRUE(isCreateCopyName))
			{
				sgName = CreateCopyName(sgName);
				name = sgName.to_std_string();
			}

			auto newObject = GameObject::Instantiate<ModelNodeObject>(
				IS_TRUE(parent.getIsValid()) ? parent : this->transform.InstantiateShared(), true, sgName);

			aiVector3D position, scale;
			aiQuaternion rotation;
			transfomation.Decompose(scale, rotation, position);

			newObject->transform->setLocalPosition(position);
			newObject->transform->setLocalScale(scale);
			newObject->transform->setLocalRotation(rotation);
		
			m_animationObjects.try_emplace(name, newObject);

			return newObject;
		}
		
		std::unordered_map<std::string, SharedPointer<ModelNodeObject>> m_animationObjects;
		std::vector<WeakPointer<MeshRenderer<Vertex::Model, Material*>>> m_meshRenderers;
		std::unordered_map<std::wstring, Animation> m_animations;
		Animation* m_player = nullptr;
		const aiScene* m_scene = nullptr;
		uint m_priority = 0;
		std::atomic_bool m_lock;
	};
}

#endif //!SGFRAMEWORK_HEADER_ANIMATOR_HPP_