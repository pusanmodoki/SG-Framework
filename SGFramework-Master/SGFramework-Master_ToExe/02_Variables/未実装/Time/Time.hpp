/*----------------------------------------------------------------------------------
時間を処理するTime Class
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_TIME_HPP_
#define SGFRAMEWORK_HEADER_TIME_HPP_
#include <Windows.h>
#include <time.h>
#include <vector>
#include <unordered_map>
#include <memory>
#include "../ConstAndUsing/ConstAndUsing.hpp"
#include "../Property/Property.hpp"
#include "../String/String.hpp"
#include "../Window/HwndHolder/HwndHolder.hpp"
#include "../ReadElement/ReadElement.hpp"
#include "TimeProcessing.hpp"
#include "../InvalidArgument/InvalidArgument.hpp"
#include "../Pointer/Pointer.hpp"

//Framework namespace
namespace SGFramework
{
	//時間を処理するTime Class
	class Time final
	{
	public:
		//TimeScaleのLayer class
		class TimeLayer;
		//friend
		friend TimeLayer;
		friend class Application;

		DELETE_ALL_DEFAULT_CLASS_FUNCTIONS(Time);

	private:

		//----------------------------------------------------------------------------------
		//[Start]
		//タイム処理の初期化を行う
		inline static void StartTimeProcessing(const ReadElement::Pack& isNoRestrictionsFPS,
			const ReadElement::Pack& fixedInterval, const ReadElement::Groop& layers, bool isRemote);
		//----------------------------------------------------------------------------------
		//[FixedUpdate]
		//Fixedタイム処理の更新を行う
		//return: Fixed更新フレームか否か, n回trueを繰り返す
		inline static bool FixedUpdate();
		//----------------------------------------------------------------------------------
		//[FixedLastUpdate]
		//Fixedタイム処理の最終更新を行う
		//補正モードの確認を行うためフレーム実行処理の中に入れること
		inline static void FixedLastUpdate();
		//----------------------------------------------------------------------------------
		//[Update]
		//タイム処理の更新を行う
		//return: 更新フレームか否か
		inline static bool Update();

	public:

		//----------------------------------------------------------------------------------
		//[FindTimeLayer]
		//TimeLayerを取得する
		//return: layerNameに該当するレイヤー, 見つからない場合は空ポインタ
		//引数1: レイヤー名(getTimeLayerより重い)
		inline static SharedPointer<TimeLayer> FindTimeLayer(const sgstring& layerName);

		//----------------------------------------------------------------------------------
		//[ResetWorldTimeScale]
		//RootLayerのタイムスケールを設定する
		//0にするとタイム系変数が完全にストップする
		//引数1: 設定する値(0~)
		inline static void ResetWorldTimeScale(float value);
		//----------------------------------------------------------------------------------
		//[CurrentTimeToString]
		//return: "[時:分:秒]"
		inline static sgstring CurrentTimeToString();
		//----------------------------------------------------------------------------------
		//[CurrentTimeToStdString]
		//return: "[時:分:秒]"
		inline static std::string CurrentTimeToStdString();
		//----------------------------------------------------------------------------------
		//[CurrentTime]
		//sec, min, hour各引数がNULLでなければ現在時刻を代入する
		inline static void CurrentTime(int* sec, int* min, int* hour);

		//time layer[id], <static>(get function property)
		//return: layer[id] pointer,  not found->null
		//argument 1: layer id
		SGF_FUNCTION_PROPERTY static SharedPointer<TimeLayer> getTimeLayer(uint id)
		{
			if (m_layerCount > id) return m_layerMap[id];
			else SharedPointer<TimeLayer>::getNull();
		}
		//time layer->id, <static>(get function property)
		//return: layer->id,  not found->TimeLayer::cInvalidID
		//argument 1: layer name
		SGF_FUNCTION_PROPERTY static uint getTimeLayerID(const sgstring& layerName)
		{
			for (auto& it : m_layerMap)
				if (it.second->layerName() == layerName) return it.second->layerID;
			return TimeLayer::cInvaridID;
		}

		//ゲッター
		//double
		static GetOnlyProperty<double> timeWithClock;					//Clockベースの時間
		static GetOnlyProperty<double> fixedInterval;						//設定済みのFixed更新間隔(初期値プリセット)
		//float
		static GetOnlyProperty<float> fps;										//fps
		static GetOnlyProperty<float> targetFrameRate;					//ターゲットフレームレート
		static GetOnlyProperty<float> worldTimeScale;					//各deltaTimeにかける倍率
		//float NotFixed
		static GetOnlyProperty<float> time;									//Start実行時からの経過秒数
		static GetOnlyProperty<float> worldDeltaTime;					//フレームごとの時間
		static GetOnlyProperty<float> unScaledWorldDeltaTime;		//フレームごとの時間(Scaleをかけない)
		//float Fixed		
		static GetOnlyProperty<float> fixedTime;								//Start実行時からの経過秒数(fixedFreameに更新)
		static GetOnlyProperty<float> fixedDeltaTime;						//フレームごとの時間(fixed)
		static GetOnlyProperty<float> unScaledFixedDeltaTime;		//フレームごとの時間(Scaleをかけない, fixed)		
		//TimeLayer[0]
		static GetOnlyProperty<SharedPointer<TimeLayer>> rootTimeLayer;
		//bool
		static GetOnlyProperty<bool> isUpdateFreame;					//Update実行フレームか
		static GetOnlyProperty<bool> isFixedUpdateFreame;			//Fixed更新フレームか

		//TimeScaleのLayer class
		class TimeLayer final
		{
		public:
			friend class Time;

			//get parent layer property property(read only), if null = not parent
			SGF_FUNCTION_PROPERTY WeakPointer<TimeLayer> getParent() const { return m_parent; }
			//get childrens iterator cbegin property(read only)
			SGF_FUNCTION_PROPERTY auto getChildrensIteratorBegin() const { return m_childrens.cbegin(); }
			//get childrens iterator cend property(read only)
			SGF_FUNCTION_PROPERTY auto getChildrensIteratorEnd() const { return m_childrens.cend(); }
			//get num childrens property(read only)
			SGF_FUNCTION_PROPERTY uint getNumChildrens() const { return static_cast<uint>(m_childrens.size()); }
			//set this time scale property(set only)
			//argument 1: 設定スケール (0~)
			SGF_FUNCTION_PROPERTY void setScale(float timeScale) { m_timeScale = timeScale; if (m_timeScale < 0.0f) m_timeScale = 0.0f; }
			//set childrens time scale property(set only)
			//argument 1: 設定スケール (0~)
			SGF_FUNCTION_PROPERTY void setScaleToChildrens(float timeScale) { for (auto& it : m_childrens) it->setScale(timeScale); }

			static constexpr wchar cRootName[] = L"root";
			static constexpr uint cInvaridID = VariableLimit::cUintMax;

			GetOnlyProperty<float> timeScale;						//現在のタイムスケール, Rootから自身までのすべてのsettingScaleをかけた値になる
			GetOnlyProperty<float> settingScale;					//タイムスケール設定値
			GetOnlyProperty<float> deltaTime;						//このレイヤーのdeltaTime
			GetOnlyProperty<float> fixedDeltaTime;				//このレイヤーのfixedDeltaTime
			GetOnlyProperty<float> unscaledDeltaTime;		//unscaledDeltaTime * this time scale
			GetOnlyProperty<float> unscaledFixedDeltaTime;	//unscaledDeltaTime * this time scale
			GetOnlyProperty<sgstring> layerName;				//レイヤー名
			GetOnlyProperty<uint> layerID;							//レイヤーID

		private:
			//----------------------------------------------------------------------------------
			//[コンストラクタ]
			inline TimeLayer();

			//----------------------------------------------------------------------------------
			//[InitRoot]
			//Rootレイヤー用, 初期設定を行う
			inline void InitRoot();
			//----------------------------------------------------------------------------------
			//[Init]
			//Root以外のレイヤー用, 初期設定を行う
			//引数1: this Pointer
			//引数2: 親レイヤー
			//引数3: レイヤー名
			//引数4: 設定スケール
			//引数5: レイヤーID
			inline void Init(SharedPointer<TimeLayer>& thisPointer,
				SharedPointer<TimeLayer>& parent, const sgstring& myLayerName, float settingScale, uint layerID);


			//----------------------------------------------------------------------------------
			//[Update]
			//deltaTimeの更新を行う
			inline void Update();

			//子レイヤー
			std::vector<WeakPointer<TimeLayer>> m_childrens;
			sgstring m_layerName;							//レイヤー名
			WeakPointer<TimeLayer> m_parent;		//親レイヤー
			float m_timeScale;									//現在スケール
			float m_settingScale;								//設定スケール
			float m_deltaTime;									//deltaTime
			float m_fixedDeltaTime;							//fixedDeltaTime
			float m_unscaledDeltaTime;					//unscaledDeltaTime * this time scale
			float m_unscaledFixedDeltaTime;				//unscaledDeltaTime * this time scale
			uint m_layerID;										//レイヤーID
		};

	private:

		//force allow update = true<static> (set only property)
		SGF_FUNCTION_PROPERTY static void setFourceUpdate() { m_isAllowUpdate = true; }

		//TimeGetter
		static TimeProcessing::TimeGetterDouble m_timeGetter;
		//FPS制限
		static TimeProcessing::FPSLimiter m_fpsLimiter;
		//FPS計測時間制限
		static TimeProcessing::TimeLimiter m_fpsInstrumentationLimiter;
		//rootLayer
		static SharedPointer<TimeLayer> m_rootLayer;
		//LayerMap
		static std::unordered_map<uint, SharedPointer<TimeLayer>> m_layerMap;


		//double
		static double m_frequencyTime;						//現在時間
		static double m_startUpTime;							//Start実行時刻
		static double m_updateStartTime;					//delta用前回フレームからのタイム
		static double m_fixedInterval;							//Fixed更新間隔(初期値プリセット)
		static double m_fixedStartTime;						//Fixed監視開始時刻

		//float
		static float m_fps;											//現在FPS
		static float m_worldTimeScale;						//各deltaTimeにかける倍率
		static float m_refreshRate;								//画面更新間隔, 初期化用に所持
		//float NotFixed
		static float m_time;										//Start実行時からの経過秒数
		static float m_worldDeltaTime;						//フレームごとの時間
		static float m_worldUnScaledDeltaTime;			//フレームごとの時間(Scaleをかけない)
		//float Fixed
		static float m_fixedTime;									//Start実行時からの経過秒数(fixedFreameに更新)
		static float m_fixedDeltaTime;							//フレームごとの時間(fixed)
		static float m_unScaledFixedDeltaTime;			//フレームごとの時間(Scaleをかけない, fixed)
		//uint
		static uint m_frameCount;								//フレーム数カウント
		static uint m_fixedCount;								//Fixed更新回数
		static uint m_layerCount;								//レイヤーカウント
		//bool
		static bool m_isUpdateFreame;						//Updateフレームか
		static bool m_isFixedUpdateFreame;				//FixedUpdateフレームか
		static bool m_isAllowUpdate;							//Updateを許可するか
	};

	//----------------------------------------------------------------------------------
	//[Start]
	//タイム処理の初期化を行う
	inline void Time::StartTimeProcessing(const ReadElement::Pack& isNoRestrictionsFPS,
		const ReadElement::Pack& fixedInterval, const ReadElement::Groop& layers, bool isRemote)
	{
		//リフレッシュレート取得
		if (IS_FALSE(isRemote))
		{
			m_refreshRate = Administrator::GetRefreshRate(Window::HwndHolder::main);
		}
		else
		{
			m_refreshRate = 30.0f;
		}

		if (isNoRestrictionsFPS[0].valueInt)
			m_refreshRate = VariableLimit::cFloatMax;
		//Interval代入
		m_fixedInterval = fixedInterval[0].valueFloat;

		//RootLayer設定
		m_rootLayer = SharedPointer<TimeLayer>(new TimeLayer());
		m_rootLayer->InitRoot();
		m_layerMap.try_emplace(0, m_rootLayer);
		m_layerCount = 1;

		for (auto& e : layers.packs)
		{
			auto pointer = SharedPointer<TimeLayer>(new TimeLayer());
			auto parent = std::move(e.elements.size() >= 2
				? FindTimeLayer(e[1].valueString) : m_rootLayer);

			pointer->Init(pointer, parent, e.header, e[0].valueFloat, m_layerCount);
			m_layerMap.try_emplace(m_layerCount++, pointer);
		}

		m_fpsLimiter.Start(m_refreshRate);		//FPS制限開始
		m_fpsInstrumentationLimiter.Start(1.0f);	//FPS計測制限開始

		m_startUpTime = m_timeGetter();			//Start実行時刻
		//代入
		m_frequencyTime = m_startUpTime;
		m_updateStartTime = m_startUpTime;
		m_fixedStartTime = m_startUpTime;

		while (m_time < 1.0f)
		{
			if (IS_TRUE(Time::FixedUpdate()))
				Time::FixedLastUpdate();
			Time::Update();
		}
	}

	//----------------------------------------------------------------------------------
	//[FixedUpdate]
	//Fixedタイム処理の更新を行う
	//return: Fixed更新フレームか否か, n回trueを繰り返す
	inline bool Time::FixedUpdate()
	{
		static double delta = 0;	//Intervl確認ループ用

		//Get Time
		m_frequencyTime = m_timeGetter();

		//FPS Update
		if (m_fpsInstrumentationLimiter(m_frequencyTime))
		{
			//Set FPS
			m_fps = static_cast<float>(static_cast<double>(m_frameCount) / (m_frequencyTime - m_fpsInstrumentationLimiter.previousStartTime));
			//Reset Counter
			m_frameCount = 0;
			if (m_fps < m_refreshRate)
				m_fpsLimiter.ChangeFPS(m_refreshRate * 1.01f);
			else if (m_fps > m_refreshRate + 1.0f)
				m_fpsLimiter.ChangeFPS(m_refreshRate* 0.99f);
		}

		if (m_frequencyTime - m_fixedStartTime > m_fixedInterval * 3)
		{
			m_fpsLimiter.Start(m_refreshRate);		//FPS制限開始
			m_fpsInstrumentationLimiter.Start(1.0f);	//FPS計測制限開始
			//fixed更新強制
			m_fixedStartTime = m_frequencyTime - m_fixedInterval;
			m_fixedCount = 0;
		}

		//FixedCountがまだ残っている
		if (m_fixedCount > 1)
		{
			--m_fixedCount;		//カウンター減算
			//Fixed更新回数がまだあり累計更新時間がFixedIntervalを超過した場合更新を諦める
			if (m_fixedCount > 1 && m_timeGetter() - m_fixedStartTime > m_fixedInterval)
				m_fixedCount = 0;
			//true返却
			return m_isFixedUpdateFreame = true;
		}

		//現在のFixed差分時間取得
		delta = m_frequencyTime - m_fixedStartTime;
		//カウンター初期化
		m_fixedCount = 0;

		//Interval確認ループ
		while (delta > m_fixedInterval)
		{
			++m_fixedCount;				//カウンター加算
			delta -= m_fixedInterval;	//delta減算
		}
		//カウンター0超過 -> Fixed Update Frame
		if (m_fixedCount > 0)
		{
			//Get FixedApplicationElapasedTime
			m_fixedTime = static_cast<float>(m_frequencyTime - m_startUpTime);
			//Get FixedDeltaTime
			m_unScaledFixedDeltaTime = static_cast<float>(m_frequencyTime - m_fixedStartTime);
			//delta設定
			m_fixedDeltaTime = m_unScaledFixedDeltaTime * m_worldTimeScale;

			m_time = m_fixedTime;							//Time代入
			m_fixedStartTime += m_fixedInterval;	//StartTime更新
			return m_isFixedUpdateFreame = true;	//true返却
		}

		//Updateは許す
		m_isAllowUpdate = true;
		//false返却
		return m_isFixedUpdateFreame = false;
	}
	//----------------------------------------------------------------------------------
	//[FixedLastUpdate]
	//Fixedタイム処理の最終更新を行う
	//補正モードの確認を行うためフレーム実行処理の中に入れること
	inline void Time::FixedLastUpdate()
	{
		//FPS Update
		if (m_fpsInstrumentationLimiter(m_frequencyTime))
		{
			//Set FPS
			m_fps = static_cast<float>(static_cast<double>(m_frameCount) / (m_frequencyTime - m_fpsInstrumentationLimiter.previousStartTime));
			//Reset Counter
			m_frameCount = 0;
			if (m_fps < m_refreshRate)
				m_fpsLimiter.ChangeFPS(m_refreshRate * 1.01f);
			else if (m_fps > m_refreshRate + 1.0f)
				m_fpsLimiter.ChangeFPS(m_refreshRate* 0.99f);
		}

		//FixedUpdate実行秒数がInterval超過->Update更新を許可しない
		//超過していなければUpdate更新を許可する
		if (m_timeGetter() - m_fixedStartTime < m_fixedInterval)
			m_isAllowUpdate = true;
		else
			m_isAllowUpdate = false;
	}

	//----------------------------------------------------------------------------------
	//[Update]
	//タイム処理の更新を行う
	//return: 更新フレームか否か
	inline bool Time::Update()
	{
		//Update更新が許可されていない場合はfalse返却
		if (m_isAllowUpdate ^ true) return m_isUpdateFreame = false;

		//Get ApplicationElapasedTime
		m_time = static_cast<float>(m_frequencyTime - m_startUpTime);

		//UpdateFrame
		if (m_fpsLimiter(m_frequencyTime))
		{
			//Get
			m_worldUnScaledDeltaTime = static_cast<float>(m_frequencyTime - m_updateStartTime);
			if (m_worldUnScaledDeltaTime > 1.0f)
				m_worldUnScaledDeltaTime = static_cast<float>(m_fpsLimiter.interval());

			m_worldDeltaTime = m_worldUnScaledDeltaTime * m_worldTimeScale;
			//TimeLayer Update
			for (auto& it : m_layerMap) it.second->Update();

			//Save Time
			m_updateStartTime = m_frequencyTime;
			//FPS Counter++
			m_frameCount++;

			//return true
			return m_isUpdateFreame = true;
		}
		//NotUpdateFrame
		else
		{
			//return false
			return m_isUpdateFreame = false;
		}
	}

	//----------------------------------------------------------------------------------
	//[FindTimeLayer]
	//TimeLayerを取得する
	//return: layerNameに該当するレイヤー, 見つからない場合は空ポインタ
	//引数1: レイヤー名(getTimeLayerより重い)
	inline SharedPointer<Time::TimeLayer> Time::FindTimeLayer(const sgstring& layerName)
	{
		for (auto& it : m_layerMap)
			if (it.second->layerName() == layerName) return it.second;
		return SharedPointer<Time::TimeLayer>::getNull();
	}

	//----------------------------------------------------------------------------------
	//[ResetWorldTimeScale]
	//RootLayerのタイムスケールを設定する
	//0にするとタイム系変数が完全にストップする
	//引数1: 設定する値(0~)
	inline void Time::ResetWorldTimeScale(float value)
	{
		m_worldTimeScale = value;		//設定
		if (m_worldTimeScale < 0.0f)	m_worldTimeScale = 0.0f;

		//Delta Update
		m_worldDeltaTime = m_worldUnScaledDeltaTime * m_worldTimeScale;
		//Root設定
		m_rootLayer->Update();
		//Update
		for (auto& it : m_layerMap)
			it.second->Update();

		//Fixed設定
		m_fixedDeltaTime = static_cast<float>(m_fixedInterval) * m_worldTimeScale;
	}
	//----------------------------------------------------------------------------------
	//[CurrentTimeToString]
	//return: "[時:分:秒]"
	inline sgstring Time::CurrentTimeToString()
	{
		tm tm;
		time_t getTime;

		::time(&getTime);
		localtime_s(&tm, &getTime);

		return sgstring(L"[" + std::to_wstring(tm.tm_hour) + L":" +
			std::to_wstring(tm.tm_min) + L":" + std::to_wstring(tm.tm_sec) + L"]");
	}
	//----------------------------------------------------------------------------------
	//[CurrentTimeToStdString]
	//return: "[時:分:秒]"
	inline std::string Time::CurrentTimeToStdString()
	{
		tm tm;
		time_t getTime;

		::time(&getTime);
		localtime_s(&tm, &getTime);

		return std::string("[" + std::to_string(tm.tm_hour) + ":" +
			std::to_string(tm.tm_min) + ":" + std::to_string(tm.tm_sec) + "]");
	}
	//----------------------------------------------------------------------------------
	//[CurrentTime]
	//sec, min, hour各引数がNULLでなければ現在時刻を代入する
	inline void Time::CurrentTime(int * sec, int * min, int * hour)
	{
		tm t;
		mktime(&t);
		if (sec != NULL) *sec = t.tm_sec;
		if (min != NULL) *min = t.tm_min;
		if (hour != NULL) *hour = t.tm_hour;
	}


	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	inline Time::TimeLayer::TimeLayer()
		: timeScale(m_timeScale), settingScale(m_settingScale), deltaTime(m_deltaTime),
		fixedDeltaTime(m_fixedDeltaTime), unscaledDeltaTime(m_unscaledDeltaTime),
		unscaledFixedDeltaTime(m_unscaledFixedDeltaTime), layerName(m_layerName), layerID(m_layerID) {}
	//----------------------------------------------------------------------------------
	//[InitRoot]
	//Rootレイヤー用, 初期設定を行う
	inline void Time::TimeLayer::InitRoot()
	{
		m_layerName = cRootName;
		m_parent.setNull();
		m_settingScale = 1.0f;
		m_layerID = 0;
		m_timeScale = m_settingScale;
		m_deltaTime = Time::m_worldDeltaTime * m_timeScale;
		m_fixedDeltaTime = Time::m_fixedDeltaTime * m_timeScale;
		m_unscaledDeltaTime = Time::m_worldUnScaledDeltaTime * m_timeScale;
		m_unscaledFixedDeltaTime = Time::m_unScaledFixedDeltaTime* m_timeScale;
		Update();
	}
	//----------------------------------------------------------------------------------
	//[Init]
	//Root以外のレイヤー用, 初期設定を行う
	//引数1: this Pointer
	//引数2: 親レイヤー
	//引数3: レイヤー名
	//引数4: 設定スケール
	//引数5: レイヤーID
	inline void Time::TimeLayer::Init(SharedPointer<TimeLayer>& thisPointer,
		SharedPointer<TimeLayer>& parent, const sgstring & myLayerName, float settingScale, uint layerID)
	{
		if (parent.getIsValid())
			parent->m_childrens.emplace_back(thisPointer);
		else
		{
			m_rootLayer->m_childrens.emplace_back(thisPointer);
			m_parent = m_rootLayer;
		}

		m_parent = parent;
		m_layerName = myLayerName;
		m_settingScale = settingScale;
		m_layerID = layerID;
		m_timeScale = m_settingScale;
		m_deltaTime = Time::m_worldDeltaTime * m_timeScale;
		m_fixedDeltaTime = Time::m_fixedDeltaTime * m_timeScale;
		m_unscaledDeltaTime = Time::m_worldUnScaledDeltaTime * m_timeScale;
		m_unscaledFixedDeltaTime = Time::m_unScaledFixedDeltaTime* m_timeScale;
		Update();
	}

	//----------------------------------------------------------------------------------
	//[Update]
	//deltaTimeの更新を行う
	inline void Time::TimeLayer::Update()
	{
		//Rootレイヤー
		if (m_layerID == 0)
		{
			//world値をそのまま設定
			m_deltaTime = Time::worldDeltaTime * m_timeScale;
			m_fixedDeltaTime = Time::fixedDeltaTime * m_timeScale;
			m_unscaledDeltaTime = Time::m_worldUnScaledDeltaTime * m_timeScale;
			m_unscaledFixedDeltaTime = Time::m_unScaledFixedDeltaTime* m_timeScale;
		}
		//それ以外
		else
		{
			//自身のものをひとまず設定
			m_timeScale = m_settingScale;
			//親を取得(Rootがあるのでエラーは起きない)
			auto layer = m_parent.TryGetInstance();
			//掛け算ループ, Rootにたどり着くまで終わらない
			do {
				m_timeScale = m_timeScale * layer->settingScale;
				layer = layer->m_parent.TryGetInstance();
			} while (layer != nullptr);

			//deltaTime設定
			m_deltaTime = Time::worldDeltaTime * m_timeScale;
			m_fixedDeltaTime = Time::fixedDeltaTime * m_timeScale;
			m_unscaledDeltaTime = Time::m_worldUnScaledDeltaTime * m_timeScale;
			m_unscaledFixedDeltaTime = Time::m_unScaledFixedDeltaTime* m_timeScale;
		}
	}
}
#endif // !SGFRAMEWORK_HEADER_TIME_HPP_