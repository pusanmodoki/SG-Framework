/*----------------------------------------------------------------------------------
3DSE������AudioAssetEffect3D class
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

	//Asset�֘A���i�[����Asset namespace
	namespace Asset
	{
		//3DSE������AudioAssetEffect3D class
		class AudioAssetEffect3D final : public BaseClass::BaseAudioSEAsset
		{
		public:
			//friend
			friend class Audio;
			friend class AssetManager;
			template <typename TAsset>
			friend SharedPointer<TAsset> Asset::InstantiateAsset(const sgstring& filePath, bool isRegisterSharedAsset);

			//----------------------------------------------------------------------------------
			//[�f�X�g���N�^]
			//�A�Z�b�g�̊J�����s��
			inline ~AudioAssetEffect3D() { ReleaseVoice(); CloseBaseAsset(getType(), true); }

			//----------------------------------------------------------------------------------
			//[Open]
			//�A�Z�b�g�̃��[�h���s��
			//����1: ��������������SubmixLayer, default = monoral root layer
			//����2: �\�[�X�̐� (�ő哯���Đ���)
			//����3: �Đ��Ԋu
			//����4: �X���[�Y�ɕ�������͈�
			//����5: ����5�̊p�x
			//����6: ���ʃJ�[�u�����̃X�P�[���[ 
			//����7: �h�b�v���[���ʂ̃X�P�[���[
			//����8: �Đ��J�n�b��, default = cDefault (Begin)
			//����9: ���[�v���J�n�b��, default = cDefault (Begin)
			//����10: �Đ��I���b��, default = cDefault (End)
			void Open(WeakPointer<Audio::SubmixLayer> submixLayer = Audio::submixMonoralRootLayer(),
				uint amountOfSource = 1, float playbackInterval = 0.0f, float innerRadius = 0.0f, 
				float innerRadiusAngle = 0.0f, float curveDistanceScaler = 50.0f, float dopplerScaler = 50.0f,
				float setPlayBeginSeconds = cDefault, float setLoopBeginSeconds = cDefault, float setPlayEndSeconds = cDefault);

			//----------------------------------------------------------------------------------
			//[Reload]
			//�A�Z�b�g�̃����[�h���s��
			void Reload() override;
			//----------------------------------------------------------------------------------
			//[Close]
			//�t�@�C�������
			void Close()  override;



			//----------------------------------------------------------------------------------
			//[Play]
			//�Đ����s��, ���ۂɍĐ������̂�1�t���[����ƂȂ�
			//����1: �����̍��W
			//����2: �����̈ړ��� (�b��)
			//����3: �����̉�]
			//����4: ���[�v��, default = 0
			inline void Play(const Vector3 & position, const Vector3 & velocityPerSec,
					const Quaternion & rotation, uint loopCount = 0)
			{
				using namespace Detail::Audio;
				XmVector quaternion = rotation;

				//Message�쐬
				AudioMessage message(MessageType::Effect3DFlag | MessageType::PlayEffect3D);
				message.attachment.playEffect3D.position = position;
				message.attachment.playEffect3D.velocity = velocityPerSec;
				message.attachment.playEffect3D.forward = Vector3::Rotate(Const::Vector3::foward, quaternion);
				message.attachment.playEffect3D.up = Vector3::Rotate(Const::Vector3::up, quaternion);
				message.attachment.playEffect3D.loopCount = loopCount;
				message.attachment.playEffect3D.instanceID = instanceID();
				//Message���M
				Audio::SendAudioMessage(message);
			}
			//----------------------------------------------------------------------------------
			//[Play]
			//�Đ����s��, ���ۂɍĐ������̂�1�t���[����ƂȂ�
			//����1: �����̍��W
			//����2: �����̈ړ��� (�b��)
			//����3: �����̑O�����x�N�g��
			//����4: �����̏�����x�N�g��
			//����5: ���[�v��, default = 0
			inline void Play(const Vector3 & position, const Vector3 & velocityPerSec,
				const Vector3 & forward, const Vector3 & up, uint loopCount = 0)
			{
				using namespace Detail::Audio;

				//Message�쐬
				AudioMessage message(MessageType::Effect3DFlag | MessageType::PlayEffect3D);
				message.attachment.playEffect3D.position = position;
				message.attachment.playEffect3D.velocity = velocityPerSec;
				message.attachment.playEffect3D.forward = forward;
				message.attachment.playEffect3D.up = up;
				message.attachment.playEffect3D.loopCount = loopCount;
				message.attachment.playEffect3D.instanceID = instanceID();
				//Message���M
				Audio::SendAudioMessage(message);
			}
			//----------------------------------------------------------------------------------
			//[Stop]
			//�Đ���S�Ē�~����, ���ۂɒ�~�����̂�1�t���[����ƂȂ�
			inline void Stop()
			{
				using namespace Detail::Audio;

				//Message�쐬
				AudioMessage message(MessageType::Effect3DFlag | MessageType::StopEffect3D);
				message.attachment.onlyInstanceID = instanceID();
				//Message���M
				Audio::SendAudioMessage(message);
			}


			//----------------------------------------------------------------------------------
			//[Reset3DInformation]
			//����1: �X���[�Y�ɕ�������͈�, default = 0
			//����2: ����5�̊p�x, default = 0
			//����3: ���ʃJ�[�u�����̃X�P�[���[, default = 50 ����
			//����4: �h�b�v���[���ʂ̃X�P�[���[, default = 50
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
			//�����̃J�[�u���蓮�Őݒ肷�� (default = nullptr, �K��ݒ�)
			//����1: �{�����[���J�[�u, default = nullptr
			//����2: LFE�J�[�u, default = nullptr
			//����3: LPF�J�[�u, default = nullptr
			//����4: LPF���o�[�u�J�[�u, default = nullptr
			//����5: ���o�[�u�J�[�u, default = nullptr
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
			//Open���Ɏw�肵���l���Đݒ肷�� (default = 0 & nullptr, �K��ݒ�)
			//����1: �����`�����l�����̉�����u������, default = 0
			//����2: Channel Azimuths, default = nullptr
			inline void ResetChannelInfomation(float channelRadius = 0.0f, float* channelAzimuths = nullptr)
			{
				AtomicOperation::LockAtomic(m_lock);
				m_emitterBuffer.ChannelRadius = channelRadius;
				m_emitterBuffer.pChannelAzimuths = channelAzimuths;
				AtomicOperation::UnlockAtomic(m_lock);
			}
			//----------------------------------------------------------------------------------
			//[ResetCone]
			//Open���Ɏw�肵���l���Đݒ肷�� (default = nullptr, �K��ݒ�)
			//����1: �I�[�f�B�I�R�[��
			inline void ResetCone(X3DAUDIO_CONE * cone = nullptr)
			{
				AtomicOperation::LockAtomic(m_lock);
				m_emitterBuffer.pCone = cone;
				AtomicOperation::UnlockAtomic(m_lock);
			}
			//----------------------------------------------------------------------------------
			//[ResetCalculateFlag]
			//Open���Ɏw�肵���l���Đݒ肷�� (default = X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_DOPPLER
			//		| X3DAUDIO_CALCULATE_LPF_DIRECT | X3DAUDIO_CALCULATE_REVERB | X3DAUDIO_CALCULATE_EMITTER_ANGLE)
			//����1: �v�Z���Ɏg�p����t���O
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
			//[�R���X�g���N�^]
			//���������s��
			inline AudioAssetEffect3D() : BaseAudioSEAsset() {}\

			//�f�t�H���g�v�Z�t���O
			static constexpr uint m_cDefaultFlags = X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_DOPPLER
				| X3DAUDIO_CALCULATE_LPF_DIRECT | X3DAUDIO_CALCULATE_REVERB | X3DAUDIO_CALCULATE_EMITTER_ANGLE;
			
			X3DAUDIO_EMITTER m_emitter = {};				//�G�~�b�^�[
			X3DAUDIO_EMITTER m_emitterBuffer = {};	//�ύX�G�~�b�^�[
			uint m_flagsBuffer = m_cDefaultFlags;			//X3DAudio�v�Z�t���O�ύX�p
			uint m_flags = m_cDefaultFlags;						//X3DAudio�v�Z�t���O
			
			//----------------------------------------------------------------------------------
			//[PlayExecution]
			//���ۂɍĐ����s��
			void PlayExecution(const Vector3 & position, const Vector3 & velocityPerSec,
				const Vector3 & forward, const Vector3 & up, uint loopCount);
			//----------------------------------------------------------------------------------
			//[StopExecution]
			//���ۂɒ�~���s��
			void StopExecution();
			//----------------------------------------------------------------------------------
			//[ReleaseVoice]
			//�������J������
			void ReleaseVoice();
		};		
	}
}
#endif //!SGFRAMEWORK_HEADER_AUDIO_ASSET_EFFECT_3D_HPP_
