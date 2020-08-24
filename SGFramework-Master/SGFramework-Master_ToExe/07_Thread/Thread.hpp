/*----------------------------------------------------------------------------------
汎用更新スレッドを管理するThread class
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_THREAD_HPP_
#define SGFRAMEWORK_HEADER_THREAD_HPP_
#include <thread>
#include <vector>
#include <atomic>
#include <functional>
#include <exception>
#include <stdexcept>
#include <Windows.h>
#include "../02_CustomVariable/CustomVariable.hpp"
#include "../03_MathAF/MathAF.hpp"
#include "../05_Graphics/Graphics.hpp"
#include "../06_Audio/Audio.hpp"
#include "../08_Input/Input.hpp"

// Framework namespace
namespace SGFramework
{
	//friend用
	class Physics;
	namespace Administrator
	{
		class AssetManager;
		class GraphicsX11;
		class GraphicsGL;
	}
	namespace Asset
	{
		class AudioAssetMusic;
	}
	
	//汎用更新スレッドを管理するThread class
	class Thread final
	{
	public:
		//friend
		friend class Application;
		friend class Audio;
		friend class Physics;
		friend class Administrator::GraphicsX11;
		friend class Administrator::GraphicsGL;
		friend class Administrator::AssetManager;
		friend class Asset::AudioAssetMusic;
		friend int WINAPI::WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow);

		DELETE_ALL_DEFAULT_CLASS_FUNCTIONS(Thread);

		//----------------------------------------------------------------------------------
		//[AutoThreadAllocation]
		//BGMストリーミングなどの随時割り振り型別スレッド関数を適切なスレッドに割り振る
		inline static uint64 AutoThreadAllocation(const HANDLE& threadHandle)
		{
			static std::atomic<uint> allocation = 0;	//シフト用

			//nullなら終了
			if (threadHandle == nullptr) return 0;

			//マスク取得
			uint64 result = 1Ui64 << AtomicOperation::Add(allocation, 1) % 
				MathAF::Min(std::thread::hardware_concurrency(), m_numMaxUsingThreads);

			//アフィニティ設定
			SetThreadAffinityMask(threadHandle, result);

			return result;
		}
		//----------------------------------------------------------------------------------
		//[ThreadAllocation]
		//BGMストリーミングなどの随時割り振り型別スレッド関数をマニュアルで適切なスレッドに割り振る
		inline static void ThreadAllocation(const HANDLE& threadHandle, uint64 useThread)
		{
			//nullなら終了
			if (threadHandle == nullptr || useThread == 0) return;

			//アフィニティ設定
			SetThreadAffinityMask(threadHandle, useThread);
		}

		//all framework running threads (max -> hardware + 3 and music)
		SGF_FUNCTION_PROPERTY static uint getNumRunningFrameworkThreads()
		{
			return m_numUseUpdateThreads
				+ (IsDebug::isValue() ? 1 : 0) //Asset monitoring
				+ m_runningCounter	//Asset etc..
				+ 1		//WinMain
				+ 1		//ApplicationMain
				+ 1		//PhysX
				+ 1		//XAudio2
				+ 1		//Audio
				+ 1;		//Graphics
		}

		//all framework using hardware threads (max -> hardware)
		SGF_FUNCTION_PROPERTY static uint getNumFrameworkUsingHardwareThreads()
		{
			uint result = m_numUseUpdateThreads + m_cNumExclusiveUseThreads;
			if (result > m_numHardwareThreads) result = m_numHardwareThreads;
			if (result > m_numMaxUsingThreads) result = m_numMaxUsingThreads;

			return result;
		}


		//このクラスが管理している汎用更新専用スレッド数
		static GetOnlyProperty<uint> numUpdateThreads;
		//このクラスが管理している更新ライン数
		static GetOnlyProperty<uint> numFunctionLines;
		//実行しているPCのハードウェアスレッド数
		static GetOnlyProperty<uint> numHardwareThreads;

	private:
		//----------------------------------------------------------------------------------
		//[StartUp]
		//スレッドを起動する
		//throw: ハードウェアスレッド数が4未満の場合 (アプリケーションを実行できない)
		//引数1: 関数キューの数
		//引数2: 最大使用スレッド数
		static void StartUp(const ReadElement::Pack& linesCount, const ReadElement::Pack& numMaxUsingThreads);
		//----------------------------------------------------------------------------------
		//[ShutDown]
		//スレッドを終了する
		static void ShutDown();

		//----------------------------------------------------------------------------------
		//[MainThreadBeginBeforeSync]
		//メインスレッド用関数
		//Update(before sync)->Begin
		static void MainThreadBeginBeforeSync();
		//----------------------------------------------------------------------------------
		//[MainBeginFixed]
		//メインスレッド用関数
		//Update(before sync)->End
		static void MainThreadEndBeforeSync();
		//----------------------------------------------------------------------------------
		//[MainThreadBeginBeforeSync]
		//メインスレッド用関数
		//Update(after sync)->Begin
		static void MainThreadBeginAfterSync();
		//----------------------------------------------------------------------------------
		//[MainBeginFixed]
		//メインスレッド用関数
		//Update(after sync)->End
		static void MainThreadEndAfterSync();
		//----------------------------------------------------------------------------------
		//[MainThreadBeginFixed]
		//メインスレッド用関数
		//FixedUpdate->Begin
		static void MainThreadBeginFixed();
		//----------------------------------------------------------------------------------
		//[MainThreadEndFixed]
		//メインスレッド用関数
		//FixedUpdate->End
		static void MainThreadEndFixed();
		//----------------------------------------------------------------------------------
		//[UpdateThread]
		//汎用更新スレッド用関数
		//関数キューの消化と実行待機をループする
		//引数1: 実行スレッドの要素番号
		static void UpdateThread(uint myThreadNumber);



		//----------------------------------------------------------------------------------
		//[MainThreadUpdate]
		//メインスレッド用関数
		//汎用更新スレッドの待機状態を解除し、関数キューの消化を並列で行う
		//引数1: callbackType
		static void MainThreadSingleUpdate(uint callbackType);
		//----------------------------------------------------------------------------------
		//[MainThreadUpdate]
		//メインスレッド用関数
		//汎用更新スレッドの待機状態を解除し、関数キューの消化を並列で行う
		static void MainThreadUpdate();
		//----------------------------------------------------------------------------------
		//[MainThreadUpdate]
		//メインスレッド用関数
		//汎用更新スレッドの待機状態を解除し、関数キューの消化を並列で行う
		//引数1: callbackType
		static void MainThreadUpdate(uint callbackType);
		//----------------------------------------------------------------------------------
		//[MainThreadInvokeCallbacks]
		//メインスレッド用関数
		//汎用更新スレッドの待機状態を解除し、コールバック呼び出しを並列で行う
		//引数1: callbackType
		static void MainThreadInvokeCallbacks(uint callbackType);
		//----------------------------------------------------------------------------------
		//[MainThreadSingleInvokeCallbacks]
		//メインスレッド用関数
		//コールバック呼び出しをシングルスレッドで行う
		//引数1: callbackType
		static void MainThreadSingleInvokeCallbacks(uint callbackType);
		//----------------------------------------------------------------------------------
		//[MainThreadBeginUpdate]
		//メインスレッド用関数
		//汎用更新スレッドの待機状態を解除し、初期更新を並列実行する
		static void MainThreadBeginUpdate();
		//----------------------------------------------------------------------------------
		//[MainThreadGraphicsUpdate]
		//メインスレッド用関数
		//汎用更新スレッドの待機状態を解除し、描画リストの作成＆ソートを並列で行う
		static void MainThreadGraphicsUpdate();
		//----------------------------------------------------------------------------------
		//[ThreadUpdate1]
		//汎用更新スレッド用関数
		//関数キューの消化と実行待機をループする
		//引数1: 実行スレッドの要素番号
		//引数2: 例外ポインタ
		static void ThreadUpdate1(uint myThreadNumber);
		//----------------------------------------------------------------------------------
		//[StartAudioGraphicsUpdate]
		//オーディオグラフィックアップデートを開始する
		static void StartAudioGraphicsUpdate();
		//----------------------------------------------------------------------------------
		//[WaitAudioGraphicsUpdate]
		//オーディオグラフィックアップデートの終了まで待機する
		static void WaitAudioGraphicsUpdate();
		//----------------------------------------------------------------------------------
		//[AudioGraphicsUpdate]
		//オーディオグラフィック優先処理スレッド用関数
		//1フレーム前の情報を基に計算, 描画や再生を行う
		//引数1: 例外ポインタ
		static void AudioGraphicsUpdate();
		//----------------------------------------------------------------------------------
		//[AudioUpdate]
		//オーディオ処理スレッド用関数
		//1フレーム前の情報を基に計算, 再生を行う
		static void AudioUpdate();

		//----------------------------------------------------------------------------------
		//[RegisterRunningFrameworkThread]
		//AudioAssetMusicなどでRunning登録を行う
		inline static void RegisterRunningFrameworkThread() { AtomicOperation::Add(m_runningCounter, 1); }
		//----------------------------------------------------------------------------------
		//[UnregisterRunningFrameworkThread]
		//AudioAssetMusicなどでRunning登録解除を行う
		inline static void UnRegisterRunningFrameworkThread() { AtomicOperation::Subtract(m_runningCounter, 1); }

		//----------------------------------------------------------------------------------
		//IncrementBeginCounter]
		inline static void IncrementBeginCounter() { AtomicOperation::Add(m_beginCounter, 1); }
		//----------------------------------------------------------------------------------
		//WaitBeginCounter]
		inline static void WaitBeginCounter()
		{
			uint oldCounter = m_cNumBeginFunctions, newCounter = m_cNumBeginFunctions;

			 while (1 ^ m_beginCounter.compare_exchange_weak(oldCounter, newCounter)) {}
		}
		
		//GraphicsAndAudioUpdate, ApplicationMain, PhysX
		SGF_FUNCTION_PROPERTY static uint getExclusiveThreadFlags(uint index);
		//getExclusiveThreadFlags == flag
		SGF_FUNCTION_PROPERTY static bool getIsEqualExclusiveThreadFlags(uint flag);

		//get ApplicationMain thread affinity property (get only)
		SGF_FUNCTION_PROPERTY static uint getAffinityApplicationMain();
		//get WinMain thread affinity property (get only)
		SGF_FUNCTION_PROPERTY static uint getAffinityWinMain();
		//get Graphics thread affinity property (get only)
		SGF_FUNCTION_PROPERTY static uint getAffinityGraphics();
		//get Audio thread affinity property (get only)
		SGF_FUNCTION_PROPERTY static uint getAffinityAudio();
		//get PhysX thread affinity property (get only)
		SGF_FUNCTION_PROPERTY static uint getAffinityPhysX();
		
		//setting ApplicationMain thread affinity property (set only)
		SGF_FUNCTION_PROPERTY static void setAffinityApplicationMain(const HANDLE& threadHandle);
		//setting WinMain thread affinity property (set only)
		SGF_FUNCTION_PROPERTY static void setAffinityWinMain();
		//setting AssetMonitoring thread affinity property (set only)
		SGF_FUNCTION_PROPERTY static void setAffinityAssetMonitoring(const HANDLE& threadHandle);
		//setting GraphicsUpdate thread affinity property (set only)
		SGF_FUNCTION_PROPERTY static void setAffinityGraphicsUpdate(const HANDLE& threadHandle);
		//setting AudioUpdate thread affinity property (set only)
		SGF_FUNCTION_PROPERTY static void setAffinityAudioUpdate(const HANDLE& threadHandle);
		//setting PhysicsUpdate thread affinity property (set only)
		SGF_FUNCTION_PROPERTY static void setAffinityPhysicsUpdate(const HANDLE& threadHandle);

		//BeginFunctions
		static constexpr uint m_cNumBeginFunctions = 2;
		static constexpr void(*m_cBeginFunctions[m_cNumBeginFunctions])() = 
		{ Input::Update, Graphics::SyncGraphics };

		//ApplicationMain, GraphicsUpdate, AudioUpdate, PhysX
		static constexpr uint m_cNumExclusiveUseThreads = 4;

		//Update->Default Before Sync 
		static constexpr uint m_cUpdateModeBeforeSync = 0x01;
		//Update->Default After Sync 
		static constexpr uint m_cUpdateModeAfterSync = 0x02;
		//Update->Fixed
		static constexpr uint m_cUpdateModeFixed = 0x04;
		//Update->Quit
		static constexpr uint m_cUpdateModeQuit = 0x08;

		//Update->Default
		static constexpr uint m_UpdateDefault = 0x01;
		//Update->SortGraphics
		static constexpr uint m_UpdateSortGraphics = 0x02;
		//Update->BeginFunction
		static constexpr uint m_UpdateBeginFunction = 0x04;
		//Update->Quit
		static constexpr uint m_UpdateQuit = 0x08;
		//Update->Invoke Callbacks
		static constexpr uint m_cInvokeCallbacks = 0x10;
		//Update->NotCallback
		static constexpr uint m_UpdateNotCallback = 0x20;

		//汎用更新スレッド
		static std::vector<std::thread> m_updateThreads;								
		//初期更新リスト
		static std::vector<std::vector<void(*)()>> m_beginFunctions;
		//ライン割当リスト
		static std::vector<std::vector<int>> m_lineAllocations;
		//汎用スレッド更新開始条件変数たち
		static std::vector<ConditionVariable::AutoReference<bool>> m_startUpdateConditions;
		//汎用スレッド更新終了条件変数たち
		static std::vector<ConditionVariable::AutoReference<bool>> m_endUpdateConditions;
		//オーディオグラフィック情報同期開始条件変数
		static UniquePointer<ConditionVariable::AutoReference<bool>> m_startAudioGraphicsCondition;
		//オーディオグラフィック情報同期終了条件変数
		static UniquePointer<ConditionVariable::AutoReference<bool>> m_endAudioGraphicsCondition;

		static std::thread m_audioGraphicsThread;			//AudioGraphicsUpdate用スレッド
		static UniquePointer<bool[]> m_conditionFlags;		//条件変数用フラグたち
		static std::atomic<uint> m_runningCounter;			//使用スレッド数計測
		static std::atomic<uint> m_beginCounter;			//BeginFunctionカウンタ
		static uint m_numMaxUsingThreads;						//最大使用スレッド数Clamp
		static uint m_numHardwareThreads;						//PCのハードウェアスレッド数
		static uint m_numUseUpdateThreads;						//本クラスが持つ汎用更新スレッド数
		static uint m_numFunctionLines;								//本クラスが持つ更新ライン数
		static uint m_updateType;										//何を更新するか
		static uint m_callbackType;										//何のComponent->Callbackを実行するか
		static uint m_transformCount;									//transformUpdateスレッド数確認用
	};

	//ApplicationMain, GraphicsUpdate, AudioUpdate, PhysX
	SGF_FUNCTION_PROPERTY uint Thread::getExclusiveThreadFlags(uint index)
	{
		switch (index)
		{
		case 0:
			return getAffinityApplicationMain();
		case 1:
			return getAffinityGraphics();
		case 2:
			return getAffinityAudio();
		case 3:
			return getAffinityPhysX();
		default:
			return 0U;
		}
	}
	//getExclusiveThreadFlags == flag
	SGF_FUNCTION_PROPERTY bool Thread::getIsEqualExclusiveThreadFlags(uint flag)
	{
		for (uint i = 0; i < m_cNumExclusiveUseThreads; ++i)
			if (getExclusiveThreadFlags(i) == flag) return true;
		return false;
	}

	//get ApplicationMain thread affinity property (get only)
	SGF_FUNCTION_PROPERTY uint Thread::getAffinityApplicationMain()
	{
		return 1U << 0U;
	}
	//get ApplicationMain thread affinity property (get only)
	SGF_FUNCTION_PROPERTY uint Thread::getAffinityWinMain()
	{
		return 1U << 1U;
	}
	//get Graphics thread affinity property (get only)
	SGF_FUNCTION_PROPERTY uint Thread::getAffinityGraphics()
	{
		return 2U < m_numMaxUsingThreads ?
			(1U << 2U) : (1U << (m_numMaxUsingThreads - 1));
	}
	//get Audio thread affinity property (get only)
	SGF_FUNCTION_PROPERTY uint Thread::getAffinityAudio()
	{
		return 4U < m_numMaxUsingThreads ?
			(1U << 4U) : (1U << (m_numMaxUsingThreads - 1));
	}
	//get PhysX thread affinity property (get only)
	SGF_FUNCTION_PROPERTY uint Thread::getAffinityPhysX()
	{
		return 6U < m_numMaxUsingThreads ?
			(1U << 6U) : (1U << (m_numMaxUsingThreads - 1));
	}

	//setting ApplicationMain thread affinity property (set only)
	SGF_FUNCTION_PROPERTY void Thread::setAffinityApplicationMain(const HANDLE& threadHandle)
	{
		SetThreadAffinityMask(threadHandle, static_cast<uint64>(getAffinityApplicationMain()));
	}
	//setting WinMain thread affinity property (set only)
	SGF_FUNCTION_PROPERTY void Thread::setAffinityWinMain()
	{
		SetThreadAffinityMask(GetCurrentThread(), static_cast<uint64>(getAffinityWinMain()));
	}
	//setting AssetMonitoring thread affinity property (set only)
	SGF_FUNCTION_PROPERTY void Thread::setAffinityAssetMonitoring(const HANDLE & threadHandle)
	{
		//WinMainと同居させます (3秒ごと更新だしそこまで負荷は高くない。はず。)
		SetThreadAffinityMask(threadHandle, static_cast<uint64>(getAffinityWinMain()));
	}
	//setting GraphicsUpdate thread affinity property (set only)
	SGF_FUNCTION_PROPERTY void Thread::setAffinityGraphicsUpdate(const HANDLE& threadHandle)
	{
		SetThreadAffinityMask(threadHandle, static_cast<uint64>(getAffinityGraphics()));
	}
	//setting AudioUpdate thread affinity property (set only)
	SGF_FUNCTION_PROPERTY void Thread::setAffinityAudioUpdate(const HANDLE& threadHandle)
	{
		SetThreadAffinityMask(threadHandle, static_cast<uint64>(getAffinityAudio()));
	}
	//setting PhysicsUpdate thread affinity property (set only)
	SGF_FUNCTION_PROPERTY void Thread::setAffinityPhysicsUpdate(const HANDLE & threadHandle)
	{
		SetThreadAffinityMask(threadHandle, static_cast<uint64>(getAffinityPhysX()));
	}
}
#endif // !SGFRAMEWORK_HEADER_THREAD_HPP_