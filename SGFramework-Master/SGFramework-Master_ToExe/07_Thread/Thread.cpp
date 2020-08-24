/*----------------------------------------------------------------------------------
汎用更新スレッドを管理するThread class
------------------------------------------------------------------------------------*/
#include "Thread.hpp"
#include "../05_Graphics/Graphics.hpp"
#include "../06_Audio/Audio.hpp"

// Framework namespace
namespace SGFramework
{
	//このクラスが管理している汎用更新専用スレッド数
	GetOnlyProperty<uint> Thread::numUpdateThreads(m_numUseUpdateThreads);
	//このクラスが管理している更新ライン数
	GetOnlyProperty<uint> Thread::numFunctionLines(m_numFunctionLines);
	//実行しているPCのハードウェアスレッド数
	GetOnlyProperty<uint> Thread::numHardwareThreads(m_numHardwareThreads);

	//汎用更新スレッド
	std::vector<std::thread> Thread::m_updateThreads;							
	//初期更新リスト
	std::vector<std::vector<void(*)()>> Thread::m_beginFunctions;
	//ライン割当リスト
	std::vector<std::vector<int>> Thread::m_lineAllocations;
	
	//汎用スレッド更新開始条件変数たち
	std::vector<ConditionVariable::AutoReference<bool>> Thread::m_startUpdateConditions;
	//汎用スレッド更新終了条件変数たち
	std::vector<ConditionVariable::AutoReference<bool>> Thread::m_endUpdateConditions;
	//オーディオグラフィック情報同期開始条件変数
	UniquePointer<ConditionVariable::AutoReference<bool>> Thread::m_startAudioGraphicsCondition;
	//オーディオグラフィック情報同期終了条件変数
	UniquePointer<ConditionVariable::AutoReference<bool>> Thread::m_endAudioGraphicsCondition;

	std::thread Thread::m_audioGraphicsThread;			//AudioGraphicsUpdate用スレッド
	UniquePointer<bool[]> Thread::m_conditionFlags;		//条件変数用フラグたち
	std::atomic<uint> Thread::m_runningCounter = 0;	//使用スレッド数計測
	std::atomic<uint> Thread::m_beginCounter = 0;		//BeginFunctionカウンタ
	uint Thread::m_numMaxUsingThreads = 0;					//最大使用スレッド数Clamp
	uint Thread::m_numHardwareThreads = 0;					//PCのハードウェアスレッド数
	uint Thread::m_numFunctionLines = 0;						//本クラスが持つ更新ライン数
	uint Thread::m_numUseUpdateThreads = 0;				//本クラスが持つ汎用更新スレッド数
	uint Thread::m_updateType = 0;									//何を更新するか
	uint Thread::m_callbackType = 0;								//何のComponent->Callbackを実行するか
	uint Thread::m_transformCount = 0;							//transformUpdateスレッド数確認用
	
	//----------------------------------------------------------------------------------
	//[StartUp]
	//スレッドを起動する
	//throw: ハードウェアスレッド数が4未満の場合 (アプリケーションを実行できない)
	//引数1: 関数キューの数
	//引数2: 最大使用スレッド数
	void Thread::StartUp(const ReadElement::Pack& numLines, const ReadElement::Pack& numMaxUsingThreads)
	{
		//スレッド数取得
		m_numHardwareThreads = std::thread::hardware_concurrency();
		//4スレッド未満な場合実行できないのでthrow
		if (m_numHardwareThreads < 4)
			throw InvalidArgument(L"Error!! Thread->StartUp", L"This application requires a cpu with 4 or more threads");

		//最大使用スレッド数取得
		m_numMaxUsingThreads = static_cast<uint>(numMaxUsingThreads[0].valueInt);

		//使用スレッドを関数ライン数と取得できる最大スレッド数を比較し小さい方に設定
		m_numUseUpdateThreads = MathAF::Min(m_numHardwareThreads - m_cNumExclusiveUseThreads,
			static_cast<uint>(numLines[0].valueInt));
		//最大使用スレッド数までクランプ
		m_numUseUpdateThreads = MathAF::Min(m_numUseUpdateThreads, m_numMaxUsingThreads);

		//関数ライン数
		m_numFunctionLines = static_cast<uint>(numLines[0].valueInt);
		//条件変数用フラグ
		m_conditionFlags = Pointer::MakeUniqueArray::Auto<bool>((m_numUseUpdateThreads * 2) + 2);

		//ライン割当
		for (uint i = 0; i < m_numFunctionLines; ++i)
		{
			if (i < m_numUseUpdateThreads)
				m_lineAllocations.emplace_back(std::vector<int>());

			m_lineAllocations[i % m_numUseUpdateThreads].emplace_back(static_cast<int>(i));
		}

		//Begin Functions作成
		m_beginFunctions.resize(m_numUseUpdateThreads);
		for (uint i = 0, threads = 0, isAddCounterFunction = 1; i < m_numUseUpdateThreads || i < m_cNumBeginFunctions; ++i, ++threads)
		{
			if (threads >= m_numUseUpdateThreads)
			{
				threads = 0;
				isAddCounterFunction = 0;
			}

			if (i < m_cNumBeginFunctions)
			{
				m_beginFunctions[threads].emplace_back(m_cBeginFunctions[i]);
				if (isAddCounterFunction > 0)
					m_beginFunctions[threads].emplace_back(IncrementBeginCounter);
			}
			else if (isAddCounterFunction > 0)
			{
				m_beginFunctions[threads].emplace_back(WaitBeginCounter);
			}


			//定数しか入らないのでゴリゴリプログラムを組みます
			switch (threads)
			{
			case 0:
				m_beginFunctions[threads].emplace_back([]() { Transform::SyncAndUpdateForMultiThread(m_lineAllocations[0], Component::CallbackFunctions::Update); });
				break;
			case 1:
				m_beginFunctions[threads].emplace_back([]() { Transform::SyncAndUpdateForMultiThread(m_lineAllocations[1], Component::CallbackFunctions::Update); });
				break;
			case 2:
				m_beginFunctions[threads].emplace_back([]() { Transform::SyncAndUpdateForMultiThread(m_lineAllocations[2], Component::CallbackFunctions::Update); });
				break;
			case 3:
				m_beginFunctions[threads].emplace_back([]() { Transform::SyncAndUpdateForMultiThread(m_lineAllocations[3], Component::CallbackFunctions::Update); });
				break;
			case 4:
				m_beginFunctions[threads].emplace_back([]() { Transform::SyncAndUpdateForMultiThread(m_lineAllocations[4], Component::CallbackFunctions::Update); });
				break;
			case 5:
				m_beginFunctions[threads].emplace_back([]() { Transform::SyncAndUpdateForMultiThread(m_lineAllocations[5], Component::CallbackFunctions::Update); });
				break;
			case 6:
				m_beginFunctions[threads].emplace_back([]() { Transform::SyncAndUpdateForMultiThread(m_lineAllocations[6], Component::CallbackFunctions::Update); });
				break;
			case 7:
				m_beginFunctions[threads].emplace_back([]() { Transform::SyncAndUpdateForMultiThread(m_lineAllocations[7], Component::CallbackFunctions::Update); });
				break;
			case 8:
				m_beginFunctions[threads].emplace_back([]() { Transform::SyncAndUpdateForMultiThread(m_lineAllocations[8], Component::CallbackFunctions::Update); });
				break;
			case 9:
				m_beginFunctions[threads].emplace_back([]() { Transform::SyncAndUpdateForMultiThread(m_lineAllocations[9], Component::CallbackFunctions::Update); });
				break;
			case 10:
				m_beginFunctions[threads].emplace_back([]() { Transform::SyncAndUpdateForMultiThread(m_lineAllocations[10], Component::CallbackFunctions::Update); });
				break;
			case 11:
				m_beginFunctions[threads].emplace_back([]() { Transform::SyncAndUpdateForMultiThread(m_lineAllocations[11], Component::CallbackFunctions::Update); });
				break;
			case 12:
				m_beginFunctions[threads].emplace_back([]() { Transform::SyncAndUpdateForMultiThread(m_lineAllocations[12], Component::CallbackFunctions::Update); });
				break;
			case 13:
				m_beginFunctions[threads].emplace_back([]() { Transform::SyncAndUpdateForMultiThread(m_lineAllocations[13], Component::CallbackFunctions::Update); });
				break;
			case 14:
				m_beginFunctions[threads].emplace_back([]() { Transform::SyncAndUpdateForMultiThread(m_lineAllocations[14], Component::CallbackFunctions::Update); });
				break;
			case 15:
				m_beginFunctions[threads].emplace_back([]() { Transform::SyncAndUpdateForMultiThread(m_lineAllocations[15], Component::CallbackFunctions::Update); });
				break;
			case 16:
				m_beginFunctions[threads].emplace_back([]() { Transform::SyncAndUpdateForMultiThread(m_lineAllocations[16], Component::CallbackFunctions::Update); });
				break;
			case 17:
				m_beginFunctions[threads].emplace_back([]() { Transform::SyncAndUpdateForMultiThread(m_lineAllocations[17], Component::CallbackFunctions::Update); });
				break;
			case 18:
				m_beginFunctions[threads].emplace_back([]() { Transform::SyncAndUpdateForMultiThread(m_lineAllocations[18], Component::CallbackFunctions::Update); });
				break;
			case 19:
				m_beginFunctions[threads].emplace_back([]() { Transform::SyncAndUpdateForMultiThread(m_lineAllocations[19], Component::CallbackFunctions::Update); });
				break;
			case 20:
				m_beginFunctions[threads].emplace_back([]() { Transform::SyncAndUpdateForMultiThread(m_lineAllocations[20], Component::CallbackFunctions::Update); });
				break;
			case 21:
				m_beginFunctions[threads].emplace_back([]() { Transform::SyncAndUpdateForMultiThread(m_lineAllocations[21], Component::CallbackFunctions::Update); });
				break;
			case 22:
				m_beginFunctions[threads].emplace_back([]() { Transform::SyncAndUpdateForMultiThread(m_lineAllocations[22], Component::CallbackFunctions::Update); });
				break;
			case 23:
				m_beginFunctions[threads].emplace_back([]() { Transform::SyncAndUpdateForMultiThread(m_lineAllocations[23], Component::CallbackFunctions::Update); });
				break;
			case 24:
				m_beginFunctions[threads].emplace_back([]() { Transform::SyncAndUpdateForMultiThread(m_lineAllocations[24], Component::CallbackFunctions::Update); });
				break;
			case 25:
				m_beginFunctions[threads].emplace_back([]() { Transform::SyncAndUpdateForMultiThread(m_lineAllocations[25], Component::CallbackFunctions::Update); });
				break;
			case 26:
				m_beginFunctions[threads].emplace_back([]() { Transform::SyncAndUpdateForMultiThread(m_lineAllocations[26], Component::CallbackFunctions::Update); });
				break;
			case 27:
				m_beginFunctions[threads].emplace_back([]() { Transform::SyncAndUpdateForMultiThread(m_lineAllocations[27], Component::CallbackFunctions::Update); });
				break;
			case 28:
				m_beginFunctions[threads].emplace_back([]() { Transform::SyncAndUpdateForMultiThread(m_lineAllocations[28], Component::CallbackFunctions::Update); });
				break;
			case 29:
				m_beginFunctions[threads].emplace_back([]() { Transform::SyncAndUpdateForMultiThread(m_lineAllocations[29], Component::CallbackFunctions::Update); });
				break;
			case 30:
				m_beginFunctions[threads].emplace_back([]() { Transform::SyncAndUpdateForMultiThread(m_lineAllocations[30], Component::CallbackFunctions::Update); });
				break;
			case 31:
				m_beginFunctions[threads].emplace_back([]() { Transform::SyncAndUpdateForMultiThread(m_lineAllocations[31], Component::CallbackFunctions::Update); });
				break;
			case 32:
				m_beginFunctions[threads].emplace_back([]() { Transform::SyncAndUpdateForMultiThread(m_lineAllocations[32], Component::CallbackFunctions::Update); });
				break;
			case 33:
				m_beginFunctions[threads].emplace_back([]() { Transform::SyncAndUpdateForMultiThread(m_lineAllocations[33], Component::CallbackFunctions::Update); });
				break;
			case 34:
				m_beginFunctions[threads].emplace_back([]() { Transform::SyncAndUpdateForMultiThread(m_lineAllocations[34], Component::CallbackFunctions::Update); });
				break;
			case 35:
				m_beginFunctions[threads].emplace_back([]() { Transform::SyncAndUpdateForMultiThread(m_lineAllocations[35], Component::CallbackFunctions::Update); });
			default:
				break;
			}
		}

		//条件変数作成
		m_startAudioGraphicsCondition = Pointer::MakeUnique::Auto<ConditionVariable::AutoReference<bool>>(m_conditionFlags[m_conditionFlags.getArraySize() - 2]);
		m_endAudioGraphicsCondition = Pointer::MakeUnique::Auto<ConditionVariable::AutoReference<bool>>(m_conditionFlags[m_conditionFlags.getArraySize() - 1]);
		//複数条件変数作成
		for (uint i = 0; i < m_numUseUpdateThreads; i++)
		{
			m_startUpdateConditions.emplace_back(m_conditionFlags[i]);
			m_endUpdateConditions.emplace_back(m_conditionFlags[m_numUseUpdateThreads + i]);
		}

		//スレッドを必要数分作成, スタンバイ状態にさせる
		m_updateThreads.reserve(m_numUseUpdateThreads);
		for (uint i = 0, shift = 0; i < m_numUseUpdateThreads; i++)
		{
			m_updateThreads.emplace_back(UpdateThread, i);

			do
			{
				++shift;
				if (shift >= m_numHardwareThreads) shift = 0;
			} 
			while (IS_TRUE(getIsEqualExclusiveThreadFlags(1U << shift)));

			SetThreadAffinityMask(m_updateThreads.back().native_handle(), 1Ui64 << shift);
		}


		//スレッド作成
		m_audioGraphicsThread = std::thread(AudioGraphicsUpdate);
		setAffinityGraphicsUpdate(m_audioGraphicsThread.native_handle());
		
		//グラフィックスのコマンドリストを初期化
		Graphics::InitCommandList(m_updateThreads);
		//ランダム登録
		Random::InitThreads(m_updateThreads);
		Random::InitThread(m_audioGraphicsThread);
		//GUI登録
		{
			sgstring string1 = L"Update[";
			sgstring string2 = L"]";
			for (uint i = 0, max = static_cast<uint>(m_updateThreads.size()); i < max; ++i)
				GUI::RegisterDebugStream(string1 + i + string2, m_updateThreads[i].get_id());
			GUI::RegisterDebugStream(L"AudioGraphics", m_audioGraphicsThread.get_id());
		}

		//notify
		m_endAudioGraphicsCondition->NotifyOne();
	}
	//----------------------------------------------------------------------------------
	//[ShutDown]
	//スレッドを終了する
	void Thread::ShutDown()
	{
		//全スレッド終了フラグ設定
		m_updateType = m_UpdateQuit;
		//更新開始を許可(抜けるため)
		if (IS_TRUE(m_startAudioGraphicsCondition.getIsOwned()))
			m_startAudioGraphicsCondition->NotifyOne();
		
		for (auto& e : m_startUpdateConditions)
			e.NotifyOne();

		//join
		for (uint i = 0; i < m_numUseUpdateThreads; i++)
			if (IS_TRUE(m_updateThreads[i].joinable())) m_updateThreads[i].join();
		
		if (IS_TRUE(m_audioGraphicsThread.joinable()))
			m_audioGraphicsThread.join();
	}
}