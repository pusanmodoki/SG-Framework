/*----------------------------------------------------------------------------------
オーディオを管理するAudio class
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

		//Listener用サウンドコーン, 計算の詳細は下記を参照
		//https://docs.microsoft.com/ja-jp/windows/win32/xaudio2/sound-cones
		//https://docs.microsoft.com/ja-jp/windows/win32/api/x3daudio/ns-x3daudio-x3daudio_cone
		struct ListenerCone
		{
			//内側の円錐角度, 0.03f ~ MathAF::cPix2の値に制限される
			//default = MathAF::cPix2
			float innerConeAngle = MathAF::cPix2;
			//内側の円錐角度, innerConeAngle  ~ MathAF::cPix2の値に制限される
			//default = MathAF::cPix2
			float outerAngle = MathAF::cPix2;

			//内側の円錐内, 外側の円錐との間に適用されるボリュームスケール, 0.0f ~ 2.0fの値に制限される
			//default = 1.0f
			float innerVolumeScaler = 1.0f;
			//外側の円錐内(内側の円錐の外), 円錐外に適用されるボリュームスケール, 0.0f ~ 2.0fの値に制限される
			//default = 1.0f
			float outerVolumeScaler = 1.0f;
			//内側の円錐内, 外側の円錐との間に適用されるローパスフィルター係数, 0.0f ~ 1.0fの値に制限される
			//default = 1.0f
			float innerLowPassScaler = 1.0f;
			//外側の円錐内(内側の円錐の外), 円錐外に適用されるローパスフィルター係数, 0.0f ~ 1.0fの値に制限される
			//default = 1.0f
			float outerLowPassScaler = 1.0f;
			//内側の円錐内, 外側の円錐との間に適用されるリバーブ送信レベル係数, 0.0f ~ 2.0fの値に制限される
			//default = 1.0f
			float innerReverbLevelScale = 1.0f;
			//外側の円錐内(内側の円錐の外), 円錐外に適用されるリバーブ送信レベル係数, 0.0f ~ 2.0fの値に制限される
			//default = 1.0f
			float outerReverbLevelScaler = 1.0f;
		};

	private:
		//----------------------------------------------------------------------------------
		//[EnabledCallbacks]
		//return: 自身がCallされるCallback Flags, Component::CallbackFunctions参照
		inline uint EnableCallbacks() override { return CallbackFunctions::Awake 
			| CallbackFunctions::Update | CallbackFunctions::OnDestroy; }
		//----------------------------------------------------------------------------------
		//[Awake]
		//インスタンス化した瞬間に呼ばれるCallback関数
		inline void Awake() override;
		//----------------------------------------------------------------------------------
		//[Update]
		//毎フレームUpdateブロックでCallbackされる関数
		inline void Update() override;
		//----------------------------------------------------------------------------------
		//[OnDestroy]
		//インスタンスが削除される瞬間にCallbackされる関数
		inline void OnDestroy() override;
		//----------------------------------------------------------------------------------
		//[LateUpdate]
		//Update, Sync後に呼ばれる更新ブロック (LateUpdate) でCallbackされる関数
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
	//インスタンス化した瞬間に呼ばれるCallback関数
	inline void AudioListener::Awake()
	{
		AtomicOperation::LockGuard guard(m_lock);

		//インスタンス登録
		if (IS_FALSE(m_instance.getIsValid()))
		{
			m_instance = m_thisPointer.DynamicCast<AudioListener>();
			m_isThisInstance = true;
		}
		//登録済みならthrow
		else
		{
			m_isThisInstance = false;
			throw InvalidArgument(L"Error!! AudioListener->Awake", L"Instance registered. Instance is a singleton!!");
		}
	}
	//----------------------------------------------------------------------------------
	//[Update]
	//毎フレームUpdateブロックでCallbackされる関数
	inline void AudioListener::Update()
	{
		if (IS_FALSE(m_isThisInstance)) return;

		//更新
		m_forward = transform->getForward();
		m_up = transform->getUp();
		m_position = transform->getWorldPosition();
		m_velocity = m_position - m_oldPosition;
	}
	//----------------------------------------------------------------------------------
	//[OnDestroy]
	//インスタンスが削除される瞬間にCallbackされる関数
	inline void AudioListener::OnDestroy()
	{
		//自身がインスタンスなら開放
		if (IS_TRUE(m_isThisInstance))
			m_instance.Reset();
	}
	//----------------------------------------------------------------------------------
	//[LateUpdate]
	//Update, Sync後に呼ばれる更新ブロック (LateUpdate) でCallbackされる関数
	inline void AudioListener::LateUpdate()
	{
		if (IS_FALSE(m_isThisInstance)) return;

		//更新
		m_oldPosition = transform->getWorldPosition();
	}

	//listener(singleton instance)<static> (read only property)
	SGF_FUNCTION_PROPERTY void AudioListener::getListener(X3DAUDIO_LISTENER & apply, float deltaTime)
	{
		AtomicOperation::LockGuard guard(m_lock);

		//instance登録済みならinstanceをcopy, そうでないならdefault listenerをcopy
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