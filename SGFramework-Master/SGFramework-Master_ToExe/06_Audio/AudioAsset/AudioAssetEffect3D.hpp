/*----------------------------------------------------------------------------------
3DSEを扱うAudioAssetEffect3D class
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_AUDIO_ASSET_EFFECT_3D_HPP_
#define SGFRAMEWORK_HEADER_AUDIO_ASSET_EFFECT_3D_HPP_
#include <xaudio2.h>
#include <x3daudio.h>
#include "../../02_CustomVariable/CustomVariable.hpp"
#include "../../03_MathAF/MathAF.hpp"
#include "../../04_Asset/Asset/BaseAsset.hpp"
#include "AudioAsset.hpp"
#include "../Audio.hpp"

#pragma comment(lib, "xaudio2.lib")

// Framework namespace
namespace SGFramework
{
	class Audio;
	class Audio::SubmixLayer;
	class AssetManager;

	//Asset関連を格納したAsset namespace
	namespace Asset
	{
		//3DSEを扱うAudioAssetEffect3D class
		class AudioAssetEffect3D final : public BaseClass::BaseAudioSEAsset
		{
		public:
			//friend
			friend class Audio;
			friend class AssetManager;
			template <typename TAsset>
			friend SharedPointer<TAsset> Asset::InstantiateAsset(const sgstring& filePath, bool isRegisterSharedAsset);

			//----------------------------------------------------------------------------------
			//[デストラクタ]
			//アセットの開放を行う
			inline ~AudioAssetEffect3D() { ReleaseVoice(); CloseBaseAsset(getType(), true); }

			//----------------------------------------------------------------------------------
			//[Open]
			//アセットのロードを行う
			//引数1: 音声が所属するSubmixLayer, default = monoral root layer
			//引数2: ソースの数 (最大同時再生数)
			//引数3: 再生間隔
			//引数4: スムーズに聞こえる範囲
			//引数5: 引数5の角度
			//引数6: 音量カーブ距離のスケーラー 
			//引数7: ドップラー効果のスケーラー
			//引数8: 再生開始秒数, default = cDefault (Begin)
			//引数9: ループ時開始秒数, default = cDefault (Begin)
			//引数10: 再生終了秒数, default = cDefault (End)
			void Open(WeakPointer<Audio::SubmixLayer> submixLayer = Audio::submixMonoralRootLayer(),
				uint amountOfSource = 1, float playbackInterval = 0.0f, float innerRadius = 0.0f, 
				float innerRadiusAngle = 0.0f, float curveDistanceScaler = 50.0f, float dopplerScaler = 50.0f,
				float setPlayBeginSeconds = cDefault, float setLoopBeginSeconds = cDefault, float setPlayEndSeconds = cDefault);

			//----------------------------------------------------------------------------------
			//[Reload]
			//アセットのリロードを行う
			void Reload() override;
			//----------------------------------------------------------------------------------
			//[Close]
			//ファイルを閉じる
			void Close()  override;



			//----------------------------------------------------------------------------------
			//[Play]
			//再生を行う, 実際に再生されるのは1フレーム後となる
			//引数1: 音源の座標
			//引数2: 音源の移動量 (秒速)
			//引数3: 音源の回転
			//引数4: ループ回数, default = 0
			inline void Play(const Vector3 & position, const Vector3 & velocityPerSec,
					const Quaternion & rotation, uint loopCount = 0)
			{
				using namespace Detail::Audio;
				XmVector quaternion = rotation;

				//Message作成
				AudioMessage message(MessageType::Effect3DFlag | MessageType::PlayEffect3D);
				message.attachment.playEffect3D.position = position;
				message.attachment.playEffect3D.velocity = velocityPerSec;
				message.attachment.playEffect3D.forward = Vector3::Rotate(Const::Vector3::foward, quaternion);
				message.attachment.playEffect3D.up = Vector3::Rotate(Const::Vector3::up, quaternion);
				message.attachment.playEffect3D.loopCount = loopCount;
				message.attachment.playEffect3D.instanceID = instanceID();
				//Message送信
				Audio::SendAudioMessage(message);
			}
			//----------------------------------------------------------------------------------
			//[Play]
			//再生を行う, 実際に再生されるのは1フレーム後となる
			//引数1: 音源の座標
			//引数2: 音源の移動量 (秒速)
			//引数3: 音源の前方向ベクトル
			//引数4: 音源の上方向ベクトル
			//引数5: ループ回数, default = 0
			inline void Play(const Vector3 & position, const Vector3 & velocityPerSec,
				const Vector3 & forward, const Vector3 & up, uint loopCount = 0)
			{
				using namespace Detail::Audio;

				//Message作成
				AudioMessage message(MessageType::Effect3DFlag | MessageType::PlayEffect3D);
				message.attachment.playEffect3D.position = position;
				message.attachment.playEffect3D.velocity = velocityPerSec;
				message.attachment.playEffect3D.forward = forward;
				message.attachment.playEffect3D.up = up;
				message.attachment.playEffect3D.loopCount = loopCount;
				message.attachment.playEffect3D.instanceID = instanceID();
				//Message送信
				Audio::SendAudioMessage(message);
			}
			//----------------------------------------------------------------------------------
			//[Stop]
			//再生を全て停止する, 実際に停止されるのは1フレーム後となる
			inline void Stop()
			{
				using namespace Detail::Audio;

				//Message作成
				AudioMessage message(MessageType::Effect3DFlag | MessageType::StopEffect3D);
				message.attachment.onlyInstanceID = instanceID();
				//Message送信
				Audio::SendAudioMessage(message);
			}


			//----------------------------------------------------------------------------------
			//[Reset3DInformation]
			//引数1: スムーズに聞こえる範囲, default = 0
			//引数2: 引数5の角度, default = 0
			//引数3: 音量カーブ距離のスケーラー, default = 50 引数
			//引数4: ドップラー効果のスケーラー, default = 50
			inline void Reset3DInformation(float innerRadius = 0.0f, float innerRadiusAngle = 0.0f,
				float curveDistanceScaler = 50.0f, float dopplerScaler = 50.0f)
			{
				AtomicOperation::LockAtomic(m_lock);
				m_emitterBuffer.InnerRadius = innerRadius;
				m_emitterBuffer.InnerRadiusAngle = innerRadiusAngle;
				m_emitterBuffer.CurveDistanceScaler = curveDistanceScaler;
				m_emitterBuffer.DopplerScaler = dopplerScaler;
				AtomicOperation::UnlockAtomic(m_lock);
			}
			//----------------------------------------------------------------------------------
			//[ManualSetCurve]
			//音源のカーブを手動で設定する (default = nullptr, 規定設定)
			//引数1: ボリュームカーブ, default = nullptr
			//引数2: LFEカーブ, default = nullptr
			//引数3: LPFカーブ, default = nullptr
			//引数4: LPFリバーブカーブ, default = nullptr
			//引数5: リバーブカーブ, default = nullptr
			inline void ManualSetCurve(X3DAUDIO_DISTANCE_CURVE* volumeCurve = nullptr, X3DAUDIO_DISTANCE_CURVE* lfeCurve = nullptr,
				X3DAUDIO_DISTANCE_CURVE* lpfDirectCurve = nullptr, X3DAUDIO_DISTANCE_CURVE* lpfRevertCurve = nullptr,
				X3DAUDIO_DISTANCE_CURVE* reverbCurve = nullptr)
			{
				AtomicOperation::LockAtomic(m_lock);
				m_emitterBuffer.pVolumeCurve = volumeCurve;
				m_emitterBuffer.pLFECurve = lfeCurve;
				m_emitterBuffer.pLPFDirectCurve = lpfDirectCurve;
				m_emitterBuffer.pLPFReverbCurve = lpfRevertCurve;
				AtomicOperation::UnlockAtomic(m_lock);
			}
			//----------------------------------------------------------------------------------
			//[ResetChannelInfomation]
			//Open時に指定した値を再設定する (default = 0 & nullptr, 規定設定)
			//引数1: 複数チャンネル時の音源を置く距離, default = 0
			//引数2: Channel Azimuths, default = nullptr
			inline void ResetChannelInfomation(float channelRadius = 0.0f, float* channelAzimuths = nullptr)
			{
				AtomicOperation::LockAtomic(m_lock);
				m_emitterBuffer.ChannelRadius = channelRadius;
				m_emitterBuffer.pChannelAzimuths = channelAzimuths;
				AtomicOperation::UnlockAtomic(m_lock);
			}
			//----------------------------------------------------------------------------------
			//[ResetCone]
			//Open時に指定した値を再設定する (default = nullptr, 規定設定)
			//引数1: オーディオコーン
			inline void ResetCone(X3DAUDIO_CONE * cone = nullptr)
			{
				AtomicOperation::LockAtomic(m_lock);
				m_emitterBuffer.pCone = cone;
				AtomicOperation::UnlockAtomic(m_lock);
			}
			//----------------------------------------------------------------------------------
			//[ResetCalculateFlag]
			//Open時に指定した値を再設定する (default = X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_DOPPLER
			//		| X3DAUDIO_CALCULATE_LPF_DIRECT | X3DAUDIO_CALCULATE_REVERB | X3DAUDIO_CALCULATE_EMITTER_ANGLE)
			//引数1: 計算時に使用するフラグ
			inline void ResetCalculateFlag(uint flags)
			{
				AtomicOperation::LockAtomic(m_lock);
				m_flagsBuffer = flags;
				AtomicOperation::UnlockAtomic(m_lock);
			}

			//get asset type (read function property)
			SGF_FUNCTION_PROPERTY Asset::AssetType getType() override { return Asset::AssetType::AudioEffect3D; }

			//3D audio emitter (get function property)
			SGF_FUNCTION_PROPERTY const X3DAUDIO_EMITTER& getEmitter() 
			{ 
				AtomicOperation::LockGuard guard(m_lock);
				return m_emitterBuffer; 
			}


		private:

			//----------------------------------------------------------------------------------
			//[コンストラクタ]
			//初期化を行う
			inline AudioAssetEffect3D() : BaseAudioSEAsset() {}\

			//デフォルト計算フラグ
			static constexpr uint m_cDefaultFlags = X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_DOPPLER
				| X3DAUDIO_CALCULATE_LPF_DIRECT | X3DAUDIO_CALCULATE_REVERB | X3DAUDIO_CALCULATE_EMITTER_ANGLE;
			
			X3DAUDIO_EMITTER m_emitter = {};				//エミッター
			X3DAUDIO_EMITTER m_emitterBuffer = {};	//変更エミッター
			uint m_flagsBuffer = m_cDefaultFlags;			//X3DAudio計算フラグ変更用
			uint m_flags = m_cDefaultFlags;						//X3DAudio計算フラグ
			
			//----------------------------------------------------------------------------------
			//[PlayExecution]
			//実際に再生を行う
			void PlayExecution(const Vector3 & position, const Vector3 & velocityPerSec,
				const Vector3 & forward, const Vector3 & up, uint loopCount);
			//----------------------------------------------------------------------------------
			//[StopExecution]
			//実際に停止を行う
			void StopExecution();
			//----------------------------------------------------------------------------------
			//[ReleaseVoice]
			//音声を開放する
			void ReleaseVoice();
		};		
	}
}
#endif //!SGFRAMEWORK_HEADER_AUDIO_ASSET_EFFECT_3D_HPP_
