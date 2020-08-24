/*----------------------------------------------------------------------------------
Asset�̊Ǘ�, �������s��AssetManager class
------------------------------------------------------------------------------------*/
#include "AssetManager.hpp"
#include "../07_Thread/Thread.hpp"

// Framework namespace
namespace SGFramework
{
	//�t���[�����[�N�̊Ǘ�������Administrator namespace
	namespace Administrator
	{
		//�Ď��Ԋu
		const std::chrono::seconds AssetManager::m_cMonitoringInterval(3);
		//�A�Z�b�g�����p���X�g
		std::unordered_map<Asset::AssetType, std::unordered_map<
			std::wstring, WeakPointer<BaseClass::BaseAsset>>> AssetManager::m_findAssets;
		//�A�Z�b�g�Ǘ����X�g
		std::unordered_map<uint, WeakPointer<BaseClass::BaseAsset>> AssetManager::m_assets;
		std::thread AssetManager::m_monitoringThread;					//�A�Z�b�g�Ď��X���b�h										
		std::atomic_ushort AssetManager::m_lock = false;				//lock
		std::atomic_bool AssetManager::m_isEndUpdate = false;		//�A�b�v�f�[�g�I���t���O
		std::atomic_bool AssetManager::m_isNotStopUpdate = true;//Update��~�t���O
	

		//----------------------------------------------------------------------------------
		//[StartMonitoring]
		//�A�Z�b�g�̊Ď���ʃX���b�h�ōs�� (�f�o�b�O���[�h����)
		void AssetManager::StartMonitoring()
		{
			//�f�o�b�O���[�h�łȂ���ΏI��
			if (IS_FALSE(IsDebug::isValue())) return;

			//�I���t���O������
			AtomicOperation::Init(m_isEndUpdate, false);
			//�A�b�v�f�[�g��ʃX���b�h�ŋN��
			m_monitoringThread = std::thread(MonitoringThread);
			//�A�t�B�j�e�B�ݒ�, �X���b�h�o�^
			Thread::setAffinityAssetMonitoring(m_monitoringThread.native_handle());
			Thread::RegisterRunningFrameworkThread();
		}
		//----------------------------------------------------------------------------------
		//[EndMonitoring]
		//�ʃX���b�h�ōs���Ă����A�Z�b�g�̊Ď����I������ (�f�o�b�O���[�h����)
		void AssetManager::EndMonitoring()
		{
			//�f�o�b�O���[�h�łȂ���ΏI��
			if (IS_FALSE(IsDebug::isValue())) return;
			
			//�I���t���O�ݒ�
			AtomicOperation::Init(m_isEndUpdate, true);
			//�X���b�h�I��, �X���b�h�o�^����
			if (m_monitoringThread.joinable())
			{
				m_monitoringThread.join();		
				Thread::UnRegisterRunningFrameworkThread();
			}
		}

		//----------------------------------------------------------------------------------
		//[CleanUp]
		//�Ď����Ă���A�Z�b�g�Ƀ����N�؂ꂪ�Ȃ����m�F���� (�ʏ�͂��肦�Ȃ�)
		void AssetManager::CleanUp()
		{
			//���X�g��������O�Ƀ��b�N
			LockGuardAsset guard(m_lock, true);

			//�m�F->�폜���[�v
			for (auto it = m_assets.begin(); it != m_assets.end();)
			{
				if (!it->second.getIsValid())  it = m_assets.erase(it);
				else it++;
			}
			for (auto it0 = m_findAssets.begin(); it0 != m_findAssets.end(); ++it0)
			{
				for (auto it1 = it0->second.begin(); it1 != it0->second.end();)
				{
					if (!it1->second.getIsValid())  it1 = it0->second.erase(it1);
					else it1++;
				}
			}
		}
		//----------------------------------------------------------------------------------
		//[MonitoringThread]
		//�A�Z�b�g���Ď����� (�ʃX���b�h����)
		void AssetManager::MonitoringThread()
		{
			//���[�v
			while (true)
			{
				//�I���t���Otrue�ŏI��
				if (IS_TRUE(m_isEndUpdate.load())) return;

				//Reload���X�g
				std::vector<uint> reloads;
				//����X���b�h�N������
				auto nextWakeTime = std::chrono::system_clock::now() + m_cMonitoringInterval;

				//LockGuard�̂��߂Ɉ݂͂܂�
				{
					//���X�g��������O�Ƀ��b�N
					LockGuardAsset guard(m_lock, false);
					//�Ď����[�v
					for (auto& e : m_assets)
					{
						//�����N�؂�ĂȂ���΍X�V�`�F�b�N
						if (e.second.getIsValid() & e.second.LockShared())
						{
							//�X�V�`�F�b�N�A�X�V������΃����[�h�ǉ�
							if (e.second->CheckForUpdates())
								reloads.emplace_back(e.first);
							e.second.UnlockShared();
						}
					}
				}

				//�����[�h���[�v
				if (IS_FALSE(reloads.empty()))
				{
					//�X�V�X�g�b�v
					m_isNotStopUpdate = false;
					//�ꎞ��~���S�ẴX���b�h�̒�~��҂�
					std::this_thread::sleep_for(std::chrono::seconds(1));

					//�X�V
					for (auto& e : reloads)
						m_assets.at(e)->Reload();

					//�I��
					m_isNotStopUpdate = true;
				}

				//�w��b���o�߂���܂Ń��b�N
				std::this_thread::sleep_until(nextWakeTime);
			}
		}
	}
}