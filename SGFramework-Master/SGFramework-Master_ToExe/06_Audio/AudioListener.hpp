/*----------------------------------------------------------------------------------
�I�[�f�B�I���Ǘ�����Audio class
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_AUDIO_LISTENER_HPP_
#define SGFRAMEWORK_HEADER_AUDIO_LISTENER_HPP_
#include <xaudio2.h>
#include <x3daudio.h>
#include "../05_Graphics/TransformUsing.hpp"

// Framework namespace
namespace SGFramework
{
	class AudioListener final : public Component
	{
	public:
		friend class Audio;

		//Listener�p�T�E���h�R�[��, �v�Z�̏ڍׂ͉��L���Q��
		//https://docs.microsoft.com/ja-jp/windows/win32/xaudio2/sound-cones
		//https://docs.microsoft.com/ja-jp/windows/win32/api/x3daudio/ns-x3daudio-x3daudio_cone
		struct ListenerCone
		{
			//�����̉~���p�x, 0.03f ~ MathAF::cPix2�̒l�ɐ��������
			//default = MathAF::cPix2
			float innerConeAngle = MathAF::cPix2;
			//�����̉~���p�x, innerConeAngle  ~ MathAF::cPix2�̒l�ɐ��������
			//default = MathAF::cPix2
			float outerAngle = MathAF::cPix2;

			//�����̉~����, �O���̉~���Ƃ̊ԂɓK�p�����{�����[���X�P�[��, 0.0f ~ 2.0f�̒l�ɐ��������
			//default = 1.0f
			float innerVolumeScaler = 1.0f;
			//�O���̉~����(�����̉~���̊O), �~���O�ɓK�p�����{�����[���X�P�[��, 0.0f ~ 2.0f�̒l�ɐ��������
			//default = 1.0f
			float outerVolumeScaler = 1.0f;
			//�����̉~����, �O���̉~���Ƃ̊ԂɓK�p����郍�[�p�X�t�B���^�[�W��, 0.0f ~ 1.0f�̒l�ɐ��������
			//default = 1.0f
			float innerLowPassScaler = 1.0f;
			//�O���̉~����(�����̉~���̊O), �~���O�ɓK�p����郍�[�p�X�t�B���^�[�W��, 0.0f ~ 1.0f�̒l�ɐ��������
			//default = 1.0f
			float outerLowPassScaler = 1.0f;
			//�����̉~����, �O���̉~���Ƃ̊ԂɓK�p����郊�o�[�u���M���x���W��, 0.0f ~ 2.0f�̒l�ɐ��������
			//default = 1.0f
			float innerReverbLevelScale = 1.0f;
			//�O���̉~����(�����̉~���̊O), �~���O�ɓK�p����郊�o�[�u���M���x���W��, 0.0f ~ 2.0f�̒l�ɐ��������
			//default = 1.0f
			float outerReverbLevelScaler = 1.0f;
		};

	private:
		//----------------------------------------------------------------------------------
		//[EnabledCallbacks]
		//return: ���g��Call�����Callback Flags, Component::CallbackFunctions�Q��
		inline uint EnableCallbacks() override { return CallbackFunctions::Awake 
			| CallbackFunctions::Update | CallbackFunctions::OnDestroy; }
		//----------------------------------------------------------------------------------
		//[Awake]
		//�C���X�^���X�������u�ԂɌĂ΂��Callback�֐�
		inline void Awake() override;
		//----------------------------------------------------------------------------------
		//[Update]
		//���t���[��Update�u���b�N��Callback�����֐�
		inline void Update() override;
		//----------------------------------------------------------------------------------
		//[OnDestroy]
		//�C���X�^���X���폜�����u�Ԃ�Callback�����֐�
		inline void OnDestroy() override;
		//----------------------------------------------------------------------------------
		//[LateUpdate]
		//Update, Sync��ɌĂ΂��X�V�u���b�N (LateUpdate) ��Callback�����֐�
		inline void LateUpdate() override;
	
		//listener(singleton instance)<static> (read only property)
		SGF_FUNCTION_PROPERTY static void getListener(X3DAUDIO_LISTENER& apply, float deltaTime);
		//listener(defualt)<static> (read only property)
		SGF_FUNCTION_PROPERTY static void getDefaultListener(X3DAUDIO_LISTENER& apply);
		
		//hidden callbacks
		inline void OnEnable() override {}
		inline void Start() override {}
		inline void Sync() override {}
		inline void FixedUpdate() override {}
		inline void MakeDrawCommand() override {}
		inline void OnDisable() override {}
		inline void OnCollisionEnter(const ContactInfo& contactInfo) override {}
		inline void OnCollisionStay(const ContactInfo& contactInfo) override {}
		inline void OnCollisionExit(const ContactInfo& contactInfo) override {}
		inline void OnTriggerEnter(const ContactInfo& contactInfo) override {}
		inline void OnTriggerStay(const ContactInfo& contactInfo) override {}
		inline void OnTriggerExit(const ContactInfo& contactInfo) override {}
		inline void ChangeUpperHierarchy(WeakPointer<Transform> newParent, WeakPointer<Transform> oldParent) override {}

		//static instance
		static WeakPointer<AudioListener> m_instance;
		//instance lock
		static std::atomic_bool m_lock;
		
		Vector3 m_forward;					//listener forward direction
		Vector3 m_up;							//listener up direction
		Vector3 m_position;					//listener position
		Vector3 m_velocity;					//listener velocity		
		ListenerCone* m_conePointer;	//listener cone pointer
		
		ListenerCone m_cone;				//cone
		Vector3 m_oldPosition;				//old frame position, using velocity
		bool m_isThisInstance;				//this static instance?
	};

	//----------------------------------------------------------------------------------
	//[Awake]
	//�C���X�^���X�������u�ԂɌĂ΂��Callback�֐�
	inline void AudioListener::Awake()
	{
		AtomicOperation::LockGuard guard(m_lock);

		//�C���X�^���X�o�^
		if (IS_FALSE(m_instance.getIsValid()))
		{
			m_instance = m_thisPointer.DynamicCast<AudioListener>();
			m_isThisInstance = true;
		}
		//�o�^�ς݂Ȃ�throw
		else
		{
			m_isThisInstance = false;
			throw InvalidArgument(L"Error!! AudioListener->Awake", L"Instance registered. Instance is a singleton!!");
		}
	}
	//----------------------------------------------------------------------------------
	//[Update]
	//���t���[��Update�u���b�N��Callback�����֐�
	inline void AudioListener::Update()
	{
		if (IS_FALSE(m_isThisInstance)) return;

		//�X�V
		m_forward = transform->getForward();
		m_up = transform->getUp();
		m_position = transform->getWorldPosition();
		m_velocity = m_position - m_oldPosition;
	}
	//----------------------------------------------------------------------------------
	//[OnDestroy]
	//�C���X�^���X���폜�����u�Ԃ�Callback�����֐�
	inline void AudioListener::OnDestroy()
	{
		//���g���C���X�^���X�Ȃ�J��
		if (IS_TRUE(m_isThisInstance))
			m_instance.Reset();
	}
	//----------------------------------------------------------------------------------
	//[LateUpdate]
	//Update, Sync��ɌĂ΂��X�V�u���b�N (LateUpdate) ��Callback�����֐�
	inline void AudioListener::LateUpdate()
	{
		if (IS_FALSE(m_isThisInstance)) return;

		//�X�V
		m_oldPosition = transform->getWorldPosition();
	}

	//listener(singleton instance)<static> (read only property)
	SGF_FUNCTION_PROPERTY void AudioListener::getListener(X3DAUDIO_LISTENER & apply, float deltaTime)
	{
		AtomicOperation::LockGuard guard(m_lock);

		//instance�o�^�ς݂Ȃ�instance��copy, �����łȂ��Ȃ�default listener��copy
		if (IS_TRUE(m_instance.LockShared()))
		{
			memcpy_s(&apply, sizeof(X3DAUDIO_LISTENER), &m_instance->m_forward, sizeof(X3DAUDIO_LISTENER));
			
			apply.Velocity.x *= (1.0f / deltaTime);
			apply.Velocity.y *= (1.0f / deltaTime);
			apply.Velocity.z *= (1.0f / deltaTime);

			m_instance.UnlockShared();
		}
		else
			getDefaultListener(apply);
	}
	//listener(defualt)<static> (read only property)
	SGF_FUNCTION_PROPERTY void AudioListener::getDefaultListener(X3DAUDIO_LISTENER & apply)
	{
		static X3DAUDIO_LISTENER s_listener = {};

		memcpy_s(&apply, sizeof(X3DAUDIO_LISTENER), &s_listener, sizeof(X3DAUDIO_LISTENER));
	}
}
#endif //!SGFRAMEWORK_HEADER_AUDIO_LISTENER_HPP_