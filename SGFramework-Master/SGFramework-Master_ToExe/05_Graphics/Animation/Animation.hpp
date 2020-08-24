#ifndef SGFRAMEWORK_HEADER_ANIMATION_HPP_
#define SGFRAMEWORK_HEADER_ANIMATION_HPP_
#include "../../02_CustomVariable/CustomVariable.hpp"
#include "../TransformUsing.hpp"
#include "assimp/anim.h"

// Framework namespace
namespace SGFramework
{
	class Animation
	{
	public:
		friend class Animator;
		//あに
		struct AnimationCallback
		{
			std::function<void()> callbackFunction;
			sgstring name;
			float m_invokeSeconds;
		};

		Animation(const sgstring& name, aiAnimation* animation, WeakPointer<Transform> animatorTransform,
			std::unordered_map<std::string, SharedPointer<ModelNodeObject>>* animationObjects)
		 : m_animation(animation), m_name(name) 	
		{
			m_animationSeconds = static_cast<float>(animation->mDuration / animation->mTicksPerSecond);

			//Vector3::Lerp3D
			m_animatorTransform = std::move(animatorTransform);
			m_findObjects = animationObjects;
		}

		void Play()
		{
			m_playSecondsCounter = 0.0f;
			m_isPlay = true;
			m_isLoop = true;


			std::vector<float> distances;

			for (uint i = 0; i < m_animation->mNumChannels; ++i)
			{
				aiNodeAnim* node = m_animation->mChannels[i];
				float d = 0.0f;
				for (uint k = 0; k < node->mNumPositionKeys - 1; ++k)
				{
					d += Vector3::ExactDistance(node->mPositionKeys[k].mValue,
						node->mPositionKeys[k + 1].mValue);
				}
				distances.emplace_back(d);
			}

			int a = 0;
			a = a;
		}

		void Update()
		{
			if (!m_isPlay) return;


			m_playSecondsCounter += m_animatorTransform->timeLayer->deltaTime;
			if (m_playSecondsCounter > m_animationSeconds)
			{
				m_playSecondsCounter -= m_animationSeconds;
			
				if (!m_isLoop)
				{
					m_isPlay = false;
					return;
				}
			}

			int isSet = false;
			
			if (m_playSecondsCounter > 6.0f)
			{
				int a = 0; 
				a = a;
			}
			
			aiMatrix4x4 matrix;

			for (uint i = 0; i < m_animation->mNumChannels; ++i)
			{
				aiNodeAnim* node = m_animation->mChannels[i];
				auto find = m_findObjects->find(node->mNodeName.C_Str());

				if (find != m_findObjects->end())
				{

					matrix = aiMatrix4x4();

					Vector3 scale(InterpolateScale(node));
					aiQuaternion rotation(InterpolateRotation(node));
					Vector3 position(InterpolatePosition(node));

					find->second->m_animationBoneMatrix = aiMatrix4x4(scale.ToAssimp(), rotation, position.ToAssimp());
				}
			}
		}

	private:
		static constexpr byte m_cBlendModeBlend = 0x1;
		static constexpr byte m_cBlendModeFade = 0x2;

		std::vector<Vector3> posList;
		std::vector<float> xList;
		std::vector<float> facList;
		std::vector<uint> indexList;
		float facBuf;
		uint indexBuf;
		uint indexBuf2;
		uint numIndex;
		uint numIndex2;
		std::vector<float> yList;
	
		std::unordered_map<aiNodeAnim*, Vector3 > mmmmm;

		Vector3 InterpolatePosition(aiNodeAnim* node)
		{
			if (node->mNumPositionKeys == 1)
				return node->mPositionKeys[0].mValue;

			uint index = 0, nextIndex = 1;

			for (uint i = 0; i < node->mNumPositionKeys - 1; ++i)
				if (m_playSecondsCounter < static_cast<float>(node->mPositionKeys[i + 1].mTime / m_animation->mTicksPerSecond))
				{
					indexBuf = index = i; nextIndex = index + 1;
					break;
				}
			numIndex = node->mNumPositionKeys;
			
			float absoluteTime = static_cast<float>(node->mPositionKeys[nextIndex].mTime - node->mPositionKeys[index].mTime);
			
			float t = static_cast<float>(node->mPositionKeys[index].mTime);
			float factor = (m_playSecondsCounter * static_cast<float>(m_animation->mTicksPerSecond) - t) / absoluteTime;
			facBuf = factor;
	
			return Vector3::Lerp3D(node->mPositionKeys[index].mValue, node->mPositionKeys[nextIndex].mValue, factor);
		}

		Vector3 InterpolateScale(aiNodeAnim* node)
		{
			if (node->mNumScalingKeys == 1)
				return node->mScalingKeys[0].mValue;

			uint index = 0, nextIndex = 1;

			for (uint i = 0; i < node->mNumScalingKeys - 1; ++i)
				if (m_playSecondsCounter < static_cast<float>(node->mScalingKeys[i + 1].mTime / m_animation->mTicksPerSecond))
				{
					index = i; nextIndex = index + 1;
					break;
				}

			float absoluteTime = static_cast<float>(node->mScalingKeys[nextIndex].mTime - node->mScalingKeys[index].mTime);

			float t = static_cast<float>(node->mScalingKeys[index].mTime);
			float factor = (m_playSecondsCounter * static_cast<float>(m_animation->mTicksPerSecond) - t) / absoluteTime;

			return Vector3::Lerp3D(node->mScalingKeys[index].mValue, node->mScalingKeys[nextIndex].mValue, factor);
		}

		aiQuaternion InterpolateRotation(aiNodeAnim* node)
		{
			if (node->mNumRotationKeys == 1)
				return node->mRotationKeys[0].mValue;

			uint index = 0, nextIndex = 1;

			for (uint i = 0; i < node->mNumRotationKeys - 1; ++i)
				if (m_playSecondsCounter < static_cast<float>(node->mRotationKeys[i + 1].mTime / m_animation->mTicksPerSecond))
				{
					index = i; nextIndex = index + 1;
					break;
				}
			numIndex = node->mNumRotationKeys;

			float absoluteTime = static_cast<float>(node->mRotationKeys[nextIndex].mTime - node->mRotationKeys[index].mTime);

			float t = static_cast<float>(node->mRotationKeys[index].mTime);
			float factor = (m_playSecondsCounter * static_cast<float>(m_animation->mTicksPerSecond) - t) / absoluteTime;
	
			aiQuaternion result;
			aiQuaternion::Interpolate(result,
				node->mRotationKeys[index].mValue, 
				node->mRotationKeys[nextIndex].mValue, factor);
			return result;
		}

		std::unordered_map<std::wstring, AnimationCallback> m_callbacks;
		std::unordered_map<std::string, SharedPointer<ModelNodeObject>>* m_findObjects;
		WeakPointer<Transform> m_animatorTransform;
		aiAnimation* m_animation = nullptr;
		aiAnimation* m_blendAnimation = nullptr;
		sgstring m_name = L"";

		float m_playSecondsCounter = 0.0f;
		float m_blendLengthSeconds = 0.0f;
		float m_speed = 0.0f;
		float m_animationSeconds = 0.0f;
		byte m_blendMode = 0;
		bool m_isLoop  = false;
		bool m_isPlay = false;
	};
}

#endif //!SGFRAMEWORK_HEADER_ANIMATION_HPP_