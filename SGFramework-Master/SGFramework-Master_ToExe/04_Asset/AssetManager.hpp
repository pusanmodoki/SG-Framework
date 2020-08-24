/*----------------------------------------------------------------------------------
Asset�̊Ǘ�, �������s��AssetManager class
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_ASSET_MANAGER_HPP_
#define SGFRAMEWORK_HEADER_ASSET_MANAGER_HPP_
#include <Windows.h>
#include <memory>
#include <atomic>
#include <thread>
#include "../02_CustomVariable/CustomVariable.hpp"
#include "Asset/TextAsset.hpp"
#include "Asset/BinaryAsset.hpp"
#include "FileRW/FileRW.hpp"
#include "FileStream/FileStream.hpp"

// Framework namespace
namespace SGFramework
{
	//prototype
	class GUI;
	namespace Administrator 
	{
		namespace GraphicsDetail
		{
			class VertexShaderAssetX11;
			class PixelShaderAssetX11;
			class TextureAssetX11;
		}
	}

	//Asset�֘A���i�[����Asset namespace
	namespace Asset
	{
		//prototpye
		class AudioAssetMusic;
		class AudioAssetEffect;
		class AudioAssetEffect3D;
		using VertexShaderAsset = Administrator::GraphicsDetail::VertexShaderAssetX11;
		using PixelShaderAsset = Administrator::GraphicsDetail::PixelShaderAssetX11;
		using TextureAsset = Administrator::GraphicsDetail::TextureAssetX11;
		
		//----------------------------------------------------------------------------------
		//[FindAssetType]
		//template: asset class
		//return: asset type
		template <typename TAsset>
		inline AssetType FindAssetType()
		{
			static_assert(std::is_base_of_v<SGFramework::BaseClass::BaseAsset, TAsset>,
				"FindAssetType template -> derived class only!!");

			if (std::is_same_v<TAsset, BinaryAsset>) return AssetType::Binary;
			else if (std::is_same_v<TAsset, TextAsset>) return AssetType::Text;
			else if (std::is_same_v<TAsset, VertexShaderAsset>) return AssetType::VertexShader;
			else if (std::is_same_v<TAsset, PixelShaderAsset>) return AssetType::PixelShader;
			else if (std::is_same_v<TAsset, TextureAsset>) return AssetType::Texture;
			else if (std::is_same_v<TAsset, AudioAssetEffect>) return AssetType::AudioEffect;
			else if (std::is_same_v<TAsset, AudioAssetEffect3D>) return AssetType::AudioEffect3D;
			else if (std::is_same_v<TAsset, AudioAssetMusic>) return AssetType::AudioMusic;
			else return AssetType::Null;
		}

		//----------------------------------------------------------------------------------
		//[InstantiateAsset]
		//�A�Z�b�g�N���X�𐶐�����
		//template: asset class
		//return: make_shared pointer
		//����1: file path
		//����2: asset is register shared? 
		//			(����Shared�ݒ�̓������^�|�C���^���������ꍇ�����ԋp����), default = true
		template <typename TAsset>
		inline SharedPointer<TAsset> InstantiateAsset(const sgstring& filePath, bool isRegisterSharedAsset);
	}

	//Prototype
	class Application;

	//�t���[�����[�N�̊Ǘ�������Administrator namespace
	namespace Administrator
	{
		//Asset�̊Ǘ�, �������s��AssetManager class
		class AssetManager final
		{
		public:
			template <typename TAsset>
			friend SharedPointer<TAsset> Asset::InstantiateAsset(const sgstring& filePath, bool isRegisterSharedAsset);
			friend class BaseClass::BaseAsset;
			friend class Application;
			friend class GUI;
			
			DELETE_ALL_DEFAULT_CLASS_FUNCTIONS(AssetManager);

			//----------------------------------------------------------------------------------
			//[StartMonitoring]
			//�A�Z�b�g�̊Ď���ʃX���b�h�ōs�� (�f�o�b�O���[�h����)
			static void StartMonitoring();
			//----------------------------------------------------------------------------------
			//[EndMonitoring]
			//�ʃX���b�h�ōs���Ă����A�Z�b�g�̊Ď����I������ (�f�o�b�O���[�h����)
			static void EndMonitoring();

			//----------------------------------------------------------------------------------
			//[CleanUp]
			//�Ď����Ă���A�Z�b�g�Ƀ����N�؂ꂪ�Ȃ����m�F���� (�ʏ�͂��肦�Ȃ�)
			static void CleanUp();

		private:
			//LockGuard
			class LockGuardAsset
			{
			public:
				//----------------------------------------------------------------------------------
				//[�R���X�g���N�^]
				inline LockGuardAsset(std::atomic_ushort& atomic, bool isWrite)
					: m_atomic(atomic), m_isLockWrite(isWrite)
				{
					ushort oldFlag = 0, newFlag = 0;

					oldFlag = 0;
					newFlag = m_cWriteLockFlag;

					do
					{
						//ReadLock�̏ꍇRead�J�E���^�����Z�����l�ɂ���
						if (IS_FALSE(m_isLockWrite))
						{
							oldFlag = m_atomic.load() - (m_atomic.load() & m_cWriteLockFlag);
							newFlag = oldFlag + 1;
						}
					}
					while (1 ^ m_atomic.compare_exchange_weak(oldFlag, newFlag));
				}

				//----------------------------------------------------------------------------------
				//[�f�X�g���N�^]
				inline ~LockGuardAsset()
				{
					ushort oldFlag = 0, newFlag = 0;

					oldFlag = m_cWriteLockFlag;
					newFlag = 0;

					do
					{
						//ReadLock�̏ꍇRead�J�E���^�����Z�����l�ɂ���
						if (IS_FALSE(m_isLockWrite))
						{
							oldFlag = m_atomic.load();
							newFlag = oldFlag - 1;
						}
					} while (1 ^ m_atomic.compare_exchange_weak(oldFlag, newFlag));
				}

			private:
				static constexpr ushort m_cWriteLockFlag = 0x1000;
				std::atomic_ushort& m_atomic;
				bool m_isLockWrite;
			};

			//----------------------------------------------------------------------------------
			//[FindAsset]
			//return: hit -> pointer, else -> null
			//����1: file path
			//����2: asset type
			inline static SharedPointer<BaseClass::BaseAsset> FindAsset(const sgstring& filePath, Asset::AssetType type)
			{
				LockGuardAsset guard(m_lock, false);

				if (type != Asset::AssetType::Null &&
					m_findAssets[type].find(filePath) != m_findAssets[type].end())
					return m_findAssets[type].at(filePath).InstantiateShared();
				else
					return SharedPointer<BaseClass::BaseAsset>::getNull();
			}
			//----------------------------------------------------------------------------------
			//[RegisterAsset]
			//����1: object pointer
			//����2: asset type
			//����3: register shared
			inline static void RegisterAsset(WeakPointer<BaseClass::BaseAsset> pointer,
				Asset::AssetType type, bool isRegisterShared)
			{
				LockGuardAsset guard(m_lock, true);

				//�Ď����X�g�ɓo�^
				m_assets.try_emplace(pointer->instanceID(), pointer);

				//���L���X�g�ɓo�^
				if (IS_TRUE(isRegisterShared))
				{
					pointer->m_isSharedAsset = true;
					m_findAssets[type].try_emplace(pointer->name(), std::move(pointer));
				}
			}
			//----------------------------------------------------------------------------------
			//[UnregisterSharedAsset]
			//����1: asset name
			//����2: asset type
			//����3: asset instanceID
			inline static void UnregisterSharedAsset(const sgstring& name, Asset::AssetType type, uint instanceID)
			{
				LockGuardAsset guard(m_lock, true);

				//�Ď����X�g�폜
				m_assets.erase(instanceID);

				//���L���X�g�폜
				if (m_findAssets[type].find(name) != m_findAssets[type].end())
					m_findAssets.at(type).erase(name);
			}

			//----------------------------------------------------------------------------------
			//[MonitoringThread]
			//�A�Z�b�g���Ď����� (�ʃX���b�h����)
			static void MonitoringThread();

			//�Ď��Ԋu
			static const std::chrono::seconds m_cMonitoringInterval;
			//�A�Z�b�g�Ǘ����X�g
			static std::unordered_map<uint, WeakPointer<BaseClass::BaseAsset>> m_assets;
			//�A�Z�b�g�����p���X�g
			static std::unordered_map<Asset::AssetType, std::unordered_map<
				std::wstring, WeakPointer<BaseClass::BaseAsset>>> m_findAssets;	
			static std::thread m_monitoringThread;				//�A�Z�b�g�Ď��X���b�h
			static std::atomic_ushort m_lock;						//lock
			static std::atomic_bool m_isEndUpdate;				//�A�b�v�f�[�g�I���t���O
			static std::atomic_bool m_isNotStopUpdate;			//Update��~�t���O
		};
	}
}

#endif // !SGFRAMEWORK_HEADER_ASSET_MANAGER_HPP_