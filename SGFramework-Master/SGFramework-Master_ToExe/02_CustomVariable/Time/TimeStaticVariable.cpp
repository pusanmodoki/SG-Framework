/*----------------------------------------------------------------------------------
時間を処理するTime Class
	→static変数を記述するTimeStaticVariable.cpp
------------------------------------------------------------------------------------*/
#include "Time.hpp"

//Framework namespace
namespace SGFramework
{
	//public ゲッター
	//double
	GetOnlyProperty<double> Time::timeWithClock(m_frequencyTime);								//Clockベースの時間
	GetOnlyProperty<double> Time::fixedInterval(m_fixedInterval);									//設定済みのFixed更新間隔(初期値プリセット)
	//float
	GetOnlyProperty<float> Time::fps(m_fps);																	//fps
	GetOnlyProperty<float> Time::targetFrameRate(m_refreshRate);									//ターゲットフレームレート
	GetOnlyProperty<float> Time::worldTimeScale(m_worldTimeScale);								//各deltaTimeにかける倍率
	//float NotFixed
	GetOnlyProperty<float> Time::time(m_time);																//Start実行時からの経過秒数
	GetOnlyProperty<float> Time::worldDeltaTime(m_worldDeltaTime);								//フレームごとの時間
	GetOnlyProperty<float> Time::unScaledWorldDeltaTime(m_worldUnScaledDeltaTime);	//フレームごとの時間(Scaleをかけない)
	//float Fixed		
	GetOnlyProperty<float> Time::fixedTime(m_fixedTime);												//Start実行時からの経過秒数(fixedFreameに更新)
	GetOnlyProperty<float> Time::fixedDeltaTime(m_fixedDeltaTime);								//フレームごとの時間(fixed)
	GetOnlyProperty<float> Time::unScaledFixedDeltaTime(m_unScaledFixedDeltaTime);	//フレームごとの時間(Scaleをかけない, fixed)		
	//TimeLayer[0]
	GetOnlyProperty<SharedPointer<Time::TimeLayer>> Time::rootTimeLayer(m_rootLayer);
	//bool
	GetOnlyProperty<bool> Time::isUpdateFreame(m_isUpdateFreame);							//Update実行フレームか
	GetOnlyProperty<bool> Time::isFixedUpdateFreame(m_isFixedUpdateFreame);				//Fixed更新フレームか

	//private

	//TimeGetter
	TimeProcessing::TimeGetterDouble Time::m_timeGetter;
	//FPS制限
	TimeProcessing::FPSLimiter Time::m_fpsLimiter;
	//FPS計測時間制限
	TimeProcessing::TimeLimiter Time::m_fpsInstrumentationLimiter;
	//rootLayer
	SharedPointer<Time::TimeLayer> Time::m_rootLayer;
	//LayerMap
	std::unordered_map<uint, SharedPointer<Time::TimeLayer>> Time::m_layerMap;

	//double
	double Time::m_frequencyTime = 0;					//現在時間
	double Time::m_startUpTime = 0;						//Start実行時刻
	double Time::m_updateStartTime = 0;				//delta用前回フレームからのタイム
	double Time::m_fixedInterval = 0;						//Fixed更新間隔(初期値プリセット)
	double Time::m_fixedStartTime = 0;					//Fixed監視開始時刻

	//float
	float Time::m_fps = 0.0f;									//現在FPS
	float Time::m_worldTimeScale = 1.0f;					//各deltaTimeにかける倍率
	float Time::m_refreshRate = 0.0f;						//画面更新間隔, 初期化用に所持
	//float NotFixed
	float Time::m_time = 0.0f;									//Start実行時からの経過秒数
	float Time::m_worldDeltaTime = 0.0f;					//フレームごとの時間
	float Time::m_worldUnScaledDeltaTime = 0.0f;	//フレームごとの時間(Scaleをかけない)
	//float Fixed
	float Time::m_fixedTime = 0.0f;							//Start実行時からの経過秒数(fixedFreameに更新)
	float Time::m_fixedDeltaTime = 0.0f;					//フレームごとの時間(fixed)
	float Time::m_unScaledFixedDeltaTime = 0.0f;	//フレームごとの時間(Scaleをかけない, fixed)
	//uint
	uint Time::m_frameCount = 0;							//フレーム数カウント
	uint Time::m_fixedCount = 0;								//Fixed更新回数
	uint Time::m_layerCount = 0;								//レイヤーカウント
	//bool
	bool Time::m_isUpdateFreame = false;				//Updateフレームか
	bool Time::m_isFixedUpdateFreame = false;		//FixedUpdateフレームか
	bool Time::m_isAllowUpdate = true;					//Updateを許可するか
}