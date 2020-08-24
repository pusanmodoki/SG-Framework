/*----------------------------------------------------------------------------------
Assetの管理, 生成を行うAssetManager class
------------------------------------------------------------------------------------*/
#include "AssetManager.hpp"
#include "../07_Thread/Thread.hpp"

// Framework namespace
namespace SGFramework
{
	//フレームワークの管理をするAdministrator namespace
	namespace Administrator
	{
		//監視間隔
		const std::chrono::seconds AssetManager::m_cMonitoringInterval(3);
		//アセット検索用リスト
		std::unordered_map<Asset::AssetType, std::unordered_map<
			std::wstring, WeakPointer<BaseClass::BaseAsset>>> AssetManager::m_findAssets;
		//アセット管理リスト
		std::unordered_map<uint, WeakPointer<BaseClass::BaseAsset>> AssetManager::m_assets;
		std::thread AssetManager::m_monitoringThread;					//アセット監視スレッド										
		std::atomic_ushort AssetManager::m_lock = false;				//lock
		std::atomic_bool AssetManager::m_isEndUpdate = false;		//アップデート終了フラグ
		std::atomic_bool AssetManager::m_isNotStopUpdate = true;//Update停止フラグ
	

		//----------------------------------------------------------------------------------
		//[StartMonitoring]
		//アセットの監視を別スレッドで行う (デバッグモード限定)
		void AssetManager::StartMonitoring()
		{
			//デバッグモードでなければ終了
			if (IS_FALSE(IsDebug::isValue())) return;

			//終了フラグ初期化
			AtomicOperation::Init(m_isEndUpdate, false);
			//アップデートを別スレッドで起動
			m_monitoringThread = std::thread(MonitoringThread);
			//アフィニティ設定, スレッド登録
			Thread::setAffinityAssetMonitoring(m_monitoringThread.native_handle());
			Thread::RegisterRunningFrameworkThread();
		}
		//----------------------------------------------------------------------------------
		//[EndMonitoring]
		//別スレッドで行っていたアセットの監視を終了する (デバッグモード限定)
		void AssetManager::EndMonitoring()
		{
			//デバッグモードでなければ終了
			if (IS_FALSE(IsDebug::isValue())) return;
			
			//終了フラグ設定
			AtomicOperation::Init(m_isEndUpdate, true);
			//スレッド終了, スレッド登録解除
			if (m_monitoringThread.joinable())
			{
				m_monitoringThread.join();		
				Thread::UnRegisterRunningFrameworkThread();
			}
		}

		//----------------------------------------------------------------------------------
		//[CleanUp]
		//監視しているアセットにリンク切れがないか確認する (通常はありえない)
		void AssetManager::CleanUp()
		{
			//リストをいじる前にロック
			LockGuardAsset guard(m_lock, true);

			//確認->削除ループ
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
		//アセットを監視する (別スレッド動作)
		void AssetManager::MonitoringThread()
		{
			//ループ
			while (true)
			{
				//終了フラグtrueで終了
				if (IS_TRUE(m_isEndUpdate.load())) return;

				//Reloadリスト
				std::vector<uint> reloads;
				//次回スレッド起動時間
				auto nextWakeTime = std::chrono::system_clock::now() + m_cMonitoringInterval;

				//LockGuardのために囲みます
				{
					//リストをいじる前にロック
					LockGuardAsset guard(m_lock, false);
					//監視ループ
					for (auto& e : m_assets)
					{
						//リンク切れてなければ更新チェック
						if (e.second.getIsValid() & e.second.LockShared())
						{
							//更新チェック、更新があればリロード追加
							if (e.second->CheckForUpdates())
								reloads.emplace_back(e.first);
							e.second.UnlockShared();
						}
					}
				}

				//リロードループ
				if (IS_FALSE(reloads.empty()))
				{
					//更新ストップ
					m_isNotStopUpdate = false;
					//一時停止し全てのスレッドの停止を待つ
					std::this_thread::sleep_for(std::chrono::seconds(1));

					//更新
					for (auto& e : reloads)
						m_assets.at(e)->Reload();

					//終了
					m_isNotStopUpdate = true;
				}

				//指定秒数経過するまでロック
				std::this_thread::sleep_until(nextWakeTime);
			}
		}
	}
}