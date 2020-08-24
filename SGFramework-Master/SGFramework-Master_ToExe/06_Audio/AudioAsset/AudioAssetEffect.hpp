/*----------------------------------------------------------------------------------
�ʏ�SE������AudioAssetEffect class
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_AUDIO_ASSET_EFFECT_HPP_
#define SGFRAMEWORK_HEADER_AUDIO_ASSET_EFFECT_HPP_
#include <xaudio2.h>
#include "../../02_CustomVariable/CustomVariable.hpp"
#include "../../03_MathAF/MathAF.hpp"
#include "../../04_Asset/Asset/BaseAsset.hpp"
#include "AudioAsset.hpp"
#include "../Audio.hpp"

// Framework namespace
namespace SGFramework
{
	//prototype
	class Audio;
	class Audio::SubmixLayer;
	class AssetManager;

	//Asset�֘A���i�[����Asset namespace
	namespace Asset
	{
		//�ʏ�SE������AudioAssetEffect class
		class AudioAssetEffect final : public BaseClass::BaseAudioSEAsset
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
			inline ~AudioAssetEffect() { ReleaseVoice(); CloseBaseAsset(getType(), true); }

			//----------------------------------------------------------------------------------
			//[Open]
			//�A�Z�b�g�̃��[�h���s��
			//����1: ��������������SubmixLayer, default = not monoral root layer
			//����2: �\�[�X�̐� (�ő哯���Đ���)
			//����3: �Đ��Ԋu
			//����4: �Đ��J�n�b��, default = cDefault (Begin)
			//����5: ���[�v���J�n�b��, default = cDefault (Begin)
			//����6: �Đ��I���b��, default = cDefault (End)
			void Open(WeakPointer<Audio::SubmixLayer> submixLayer = Audio::submixRootLayer(),
				uint amountOfSource = 1, float playbackInterval = 0.0f, float setPlayBeginSeconds = cDefault, 
				float setLoopBeginSeconds = cDefault, float setPlayEndSeconds = cDefault);

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
			//����1: ���[�v��, default = 0
			inline void Play(uint loopCount = 0)
			{
				using namespace Detail::Audio;

				//Message�쐬
				AudioMessage message(MessageType::EffectFlag | MessageType::PlayEffect);
				message.attachment.playEffect.instanceID = instanceID();
				message.attachment.playEffect.loopCount = loopCount;
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
				AudioMessage message(MessageType::EffectFlag | MessageType::StopEffect);
				message.attachment.onlyInstanceID = instanceID();
				//Message���M
				Audio::SendAudioMessage(message);
			}

			//get asset type (read function property)
			SGF_FUNCTION_PROPERTY Asset::AssetType getType() override { return Asset::AssetType::AudioEffect; }

		private:	
			//----------------------------------------------------------------------------------
			//[�R���X�g���N�^]
			//���������s��
			inline AudioAssetEffect() : BaseAudioSEAsset() {}

			//----------------------------------------------------------------------------------
			//[PlayExecution]
			//���ۂɍĐ����s��
			void PlayExecution(uint loopCount);
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
#endif //!SGFRAMEWORK_HEADER_AUDIO_ASSET_EFFECT_HPP_ 