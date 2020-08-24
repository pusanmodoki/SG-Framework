/*----------------------------------------------------------------------------------
汎用更新スレッドを管理するThread class
------------------------------------------------------------------------------------*/
#include "Thread.hpp"
#include "../05_Graphics/Graphics.hpp"
#include "../06_Audio/Audio.hpp"
#include "../97_Application/Application.hpp"

// Framework namespace
namespace SGFramework
{
	//----------------------------------------------------------------------------------
	//[MainThreadUpdate]
	//メインスレッド用関数
	//汎用更新スレッドの待機状態を解除し、関数キューの消化を並列で行う
	//引数1: callbackType
	void Thread::MainThreadSingleUpdate(uint callbackType)
	{
		//Callback更新です
		m_updateType = m_UpdateDefault;
		//コールバックを指定
		m_callbackType = callbackType;
		//更新
		Transform::UpdateForSingleThread(callbackType);
		//終了処理
		Transform::EndUpdateForMultiThread();
	}
	//----------------------------------------------------------------------------------
	//[MainThreadUpdate]
	//メインスレッド用関数
	//汎用更新スレッドの待機状態を解除し、関数キューの消化を並列で行う
	void Thread::MainThreadUpdate()
	{
		//通常更新です
		m_updateType = m_UpdateNotCallback;
		//コールバックを指定
		m_callbackType = 0;
		//更新開始を許可する
		for (auto& e : m_startUpdateConditions)
			e.NotifyOne();

		//更新
		Transform::UpdateForMultiThread1();

		//全てのスレッドで更新が終わるまで待機
		for (auto& e : m_endUpdateConditions)
			e.Wait();

		//終了処理
		Transform::EndUpdateForMultiThread();
	}
	//----------------------------------------------------------------------------------
	//[MainThreadUpdate]
	//メインスレッド用関数
	//汎用更新スレッドの待機状態を解除し、関数キューの消化を並列で行う
	//引数1: callbackType
	void Thread::MainThreadUpdate(uint callbackType)
	{
		//通常更新です
		m_updateType = m_UpdateDefault;
		//コールバックを指定
		m_callbackType = callbackType;
		//更新開始を許可する
		for (auto& e : m_startUpdateConditions)
			e.NotifyOne();

		//更新
		Transform::UpdateForMultiThread1(callbackType);

		//全てのスレッドで更新が終わるまで待機
		for (auto& e : m_endUpdateConditions)
			e.Wait();
		
		//終了処理
		Transform::EndUpdateForMultiThread();
	}
	//----------------------------------------------------------------------------------
	//[MainThreadInvokeCallbacks]
	//メインスレッド用関数
	//汎用更新スレッドの待機状態を解除し、コールバック呼び出しを並列で行う
	//引数1: callbackType
	void Thread::MainThreadInvokeCallbacks(uint callbackType)
	{
		//Callback更新です
		m_updateType = m_cInvokeCallbacks;
		//コールバックを指定
		m_callbackType = callbackType;
		//更新開始を許可する
		for (auto& e : m_startUpdateConditions)
			e.NotifyOne();

		//更新
		Transform::InvokeCallbackForMultiThread1(callbackType);

		//全てのスレッドで更新が終わるまで待機
		for (auto& e : m_endUpdateConditions)
			e.Wait();

		//終了処理
		Transform::EndUpdateForMultiThread();
	}
	//----------------------------------------------------------------------------------
	//[MainThreadSingleInvokeCallbacks]
	//メインスレッド用関数
	//コールバック呼び出しをシングルスレッドで行う
	//引数1: callbackType
	void Thread::MainThreadSingleInvokeCallbacks(uint callbackType)
	{
		//Callback更新です
		m_updateType = m_cInvokeCallbacks;
		//コールバックを指定
		m_callbackType = callbackType;
		//更新
		Transform::InvokeCallbackForSingleThread(callbackType);
		//終了処理
		Transform::EndUpdateForMultiThread();
	}
	//----------------------------------------------------------------------------------
	//[MainThreadBeginUpdate]
	//メインスレッド用関数
	//汎用更新スレッドの待機状態を解除し、初期更新を並列実行する
	void Thread::MainThreadBeginUpdate()
	{		
		//初期更新です
		m_updateType = m_UpdateBeginFunction;
		//更新開始を許可する
		for (auto& e : m_startUpdateConditions)
			e.NotifyOne();

		//更新
		Transform::SyncForMultiThread1(Component::CallbackFunctions::Update);


		//全てのスレッドで更新が終わるまで待機
		for (auto& e : m_endUpdateConditions)
			e.Wait();

		AtomicOperation::Init(m_beginCounter, 0);

		//終了処理
		Transform::EndUpdateForMultiThread();
	}
	//----------------------------------------------------------------------------------
	//[MainThreadGraphicsUpdate]
	//メインスレッド用関数
	//汎用更新スレッドの待機状態を解除し、描画リストの作成＆ソートを並列で行う
	void Thread::MainThreadGraphicsUpdate()
	{
		//描画リスト作成
		MainThreadInvokeCallbacks(Component::CallbackFunctions::MakeDrawCommand);

		//ソート処理です
		m_updateType = m_UpdateSortGraphics;
		//更新開始を許可する
		for (auto& e : m_startUpdateConditions)
			e.NotifyOne();

		//ソート
		Graphics::SortList();

		//全てのスレッドで更新が終わるまで待機
		for (auto& e : m_endUpdateConditions)
			e.Wait();
	}

	void Thread::MainThreadBeginBeforeSync()
	{
		//初期更新です
		m_updateType = m_cUpdateModeBeforeSync;
		//更新開始を許可する
		for (auto& e : m_startUpdateConditions)
			e.NotifyOne();
	}
	
	void Thread::MainThreadEndBeforeSync()
	{
		//全てのスレッドで更新が終わるまで待機
		for (auto& e : m_endUpdateConditions)
			e.Wait();

		AtomicOperation::Init(m_beginCounter, 0);
	}

	void Thread::MainThreadBeginAfterSync()
	{
		//Sync後更新です
		m_updateType = m_cUpdateModeAfterSync;
		//更新開始を許可する
		for (auto& e : m_startUpdateConditions)
			e.NotifyOne();
	}

	void Thread::MainThreadEndAfterSync()
	{
		//ソート
		Graphics::SortList();

		//全てのスレッドで更新が終わるまで待機
		for (auto& e : m_endUpdateConditions)
			e.Wait();
	}

	void Thread::MainThreadBeginFixed()
	{
		//Fixed更新です
		m_updateType = m_cUpdateModeFixed;
		//更新開始を許可する
		for (auto& e : m_startUpdateConditions)
			e.NotifyOne();
	}

	void Thread::MainThreadEndFixed()
	{
		//全てのスレッドで更新が終わるまで待機
		for (auto& e : m_endUpdateConditions)
			e.Wait();
	}

	//----------------------------------------------------------------------------------
	//[UpdateThread]
	//汎用更新スレッド用関数
	//関数キューの消化と実行待機をループする
	//引数1: 実行スレッドの要素番号
	void Thread::UpdateThread(uint myThreadNumber)
	{
		//更新ループ
		while (true)
		{
			//Wait
			m_startUpdateConditions[myThreadNumber].Wait();

			//Update->Default Before Sync 
			if (m_updateType & m_cUpdateModeBeforeSync)
			{
				if (m_beginFunctions[myThreadNumber].size() > 2)
				{
					uint i = 0;
					for (auto& function : m_beginFunctions[myThreadNumber])
					{
						if (i == 0) 
							function();
						else if (IS_TRUE(Application::m_isUpdateReady.load()))
							function();

						++i;
					}
				}
				else if (IS_TRUE(Application::m_isUpdateReady.load()))
				{
					for (auto& function : m_beginFunctions[myThreadNumber])
						function();
				}
			}
			//Update->Default After Sync 
			else if (m_updateType & m_cUpdateModeAfterSync)
			{
				if (IS_TRUE(Application::m_isUpdateReady.load()))
				{
					Transform::InvokeCallbackForMultiThread(m_lineAllocations[myThreadNumber],
						Component::CallbackFunctions::LateUpdate);
				}

				Transform::UpdateAndMakeMatrixForMultiThread(m_lineAllocations[myThreadNumber],
					Component::CallbackFunctions::MakeDrawCommand);
				Graphics::SortList();
			}
			//Update->Fixed
			else if (m_updateType & m_cUpdateModeFixed)
			{
				Transform::InvokeCallbackForMultiThread(m_lineAllocations[myThreadNumber],
					Component::CallbackFunctions::FixedUpdate);
			}
			//Update->Quit
			else if (m_updateType & m_cUpdateModeQuit)
				break;

			//更新終了
			m_endUpdateConditions[myThreadNumber].NotifyOne();
		}

		//更新終了
		m_endUpdateConditions[myThreadNumber].NotifyOne();
	}

	//----------------------------------------------------------------------------------
	//[ThreadUpdate1]
	//汎用更新スレッド用関数
	//関数キューの消化と実行待機をループする
	//引数1: 実行スレッドの要素番号
	//引数2: 例外ポインタ
	void Thread::ThreadUpdate1(uint myThreadNumber)
	{
		//更新ループ
		while (true)
		{
			//Wait
			m_startUpdateConditions[myThreadNumber].Wait();
			
			//Default Update
			if (m_updateType & m_UpdateDefault)
			{
				Transform::UpdateForMultiThread1(m_callbackType);
			}
			//Sort Update
			else if (m_updateType & m_UpdateSortGraphics)
			{
				Graphics::SortList();
			}
			//Begin Update
			else if (m_updateType & m_UpdateBeginFunction)
			{
				for(auto& e : m_beginFunctions[myThreadNumber])
					e();
			}
			//Invoke Callback
			else if (m_updateType & m_cInvokeCallbacks)
			{
				Transform::InvokeCallbackForMultiThread1(m_callbackType);
			}
			//NotCallback
			else if (m_updateType & m_UpdateNotCallback)
			{
				Transform::UpdateForMultiThread1();
			}
			//Quit
			else if(m_updateType & m_UpdateQuit)
				return;

			//更新終了
			m_endUpdateConditions[myThreadNumber].NotifyOne();
		}
	}
	//----------------------------------------------------------------------------------
	//[StartAudioGraphicsUpdate]
	//オーディオグラフィックアップデートを開始する
	void Thread::StartAudioGraphicsUpdate()
	{
		//Update開始
		m_startAudioGraphicsCondition->NotifyOne();
	}		
	//----------------------------------------------------------------------------------
	//[WaitAudioGraphicsUpdate]
	//オーディオグラフィックアップデートの終了まで待機する
	void Thread::WaitAudioGraphicsUpdate()
	{
		//Update終了待機
		m_endAudioGraphicsCondition->Wait();
	}

	//----------------------------------------------------------------------------------
	//[AudioGraphicsUpdate]
	//オーディオグラフィック優先処理スレッド用関数
	//1フレーム前の情報を基に計算, 描画や再生を行う
	//引数1: 例外ポインタ
	void Thread::AudioGraphicsUpdate()
	{
		//更新ループ
		while (true)
		{
			//Update開始待機
			m_startAudioGraphicsCondition->Wait();
			
			//Quit->Return
			if (m_updateType & m_UpdateQuit) return;

			//Update
			Graphics::UpdateGraphics();

			//Update終了 
			m_endAudioGraphicsCondition->NotifyOne();
		}
	}
}