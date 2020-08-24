/*----------------------------------------------------------------------------------
Time処理をするクラスを集めたTimeProcessing.hpp
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_TIME_PROCESSING_HPP_
#define SGFRAMEWORK_HEADER_TIME_PROCESSING_HPP_
#include "../Property/Property.hpp"

//Framework namespace
namespace SGFramework
{
	//Time関連クラス
	namespace TimeProcessing
	{
		//現在時刻(クロックベース)を取得するTimeGetter class
		class TimeGetter
		{
		public:
			//----------------------------------------------------------------------------------
			//[() operator]
			//return: 現在時刻(クロックベース)
			inline float operator() ()
			{
				LARGE_INTEGER frequency;						//現在周波数保存
				LARGE_INTEGER timeCounter;					//現在カウンター保存			
				//周波数取得
				QueryPerformanceFrequency(&frequency);
				//カウンター取得
				QueryPerformanceCounter(&timeCounter);
				//現在時間を返す
				return static_cast<float>(timeCounter.QuadPart) / static_cast<float>(frequency.QuadPart);
			}
		};

		//現在時刻(クロックベース)を取得するTimeGetterDouble class(double)
		class TimeGetterDouble
		{
		public:
			//----------------------------------------------------------------------------------
			//[() operator]
			//return: 現在時刻(クロックベース)
			inline double operator() ()
			{
				LARGE_INTEGER frequency;						//現在周波数保存
				LARGE_INTEGER timeCounter;					//現在カウンター保存			
				//周波数取得
				QueryPerformanceFrequency(&frequency);
				//カウンター取得
				QueryPerformanceCounter(&timeCounter);
				//現在時間を返す
				return static_cast<double>(timeCounter.QuadPart) / static_cast<double>(frequency.QuadPart);
			}
		};

		//Start()からStop()までの時間を測るStopWatch class
		class StopWatch
		{
		public:
			//----------------------------------------------------------------------------------
			//[Start]
			//ストップウォッチを開始する
			inline void Start()
			{
				//初期化
				m_elapasedTime = 0.0f;
				m_startTime = m_timeGetter();
			}
			//----------------------------------------------------------------------------------
			//[Resume]
			//Stop中のストップウォッチを再開する
			inline void Resume()
			{
				//初期化
				m_startTime = m_timeGetter();
			}
			//----------------------------------------------------------------------------------
			//[Stop]
			//ストップウォッチを停止する, ここで初めてelapasedMilliSecond に値が入る
			inline void Stop()
			{
				//現在時刻を取得して減算しミリ秒にするために*1000
				double stopTime = m_timeGetter();
				m_elapasedTime = (stopTime - m_startTime) * 1000.0f;
			}

			//StartからStopまでの秒数(ミリ秒)
			GetOnlyProperty<double> elapasedMilliSecond = GetOnlyProperty<double>(m_elapasedTime);
			//StartTime
			GetOnlyProperty<double> startTime = GetOnlyProperty<double>(m_startTime);

		private:
			TimeGetterDouble m_timeGetter;		//Time取得用
			double m_startTime = 0.0f;				//開始時刻
			double m_elapasedTime = 0.0f;			//StartからStopまでの時間
		};

		//FPS制限をかけるFPSLimiter
		class FPSLimiter
		{
		public:
			//----------------------------------------------------------------------------------
			//[Start]
			//制限を開始する
			//引数1: 制限FPS
			inline void Start(double fps)
			{
				//初期化
				TimeGetter timeGetter;
				m_startTime = timeGetter();
				m_fps = fps;
				m_interval = 1 / m_fps;
			}
			//----------------------------------------------------------------------------------
			//[ChangeFPS]
			//制限FPSを変更する
			//引数1: 制限FPS
			inline void ChangeFPS(double fps)
			{
				m_fps = fps;
				m_interval = 1 / m_fps;
			}
			//----------------------------------------------------------------------------------
			//[operator ()]
			//return: 現在がStartで指定した制限フレームに該当する場合はtrue
			inline bool operator() ()
			{
				TimeGetter timeGetter;
				return this->operator()(timeGetter());
			}
			//----------------------------------------------------------------------------------
			//[operator ()]
			//return: 現在がStartで指定した制限フレームに該当する場合はtrue
			//引数1: 現在時刻(クロックベース)
			inline bool operator() (double nowTime)
			{
				//Timeが設定fps超過
				if ((nowTime - m_startTime) >= m_interval)
				{
					//Time初期化
					m_startTime = nowTime;
					return true;		//true返却
				}
				//違う場合false返す
				else
					return false;
			}
		
			//StartTime
			GetOnlyProperty<double> startTime = m_startTime;
			//FPS
			GetOnlyProperty<double> fps = m_fps;
			//Interval
			GetOnlyProperty<double> interval = m_interval;
		private:
			double m_fps = 0;					//制限FPS
			double m_startTime = 0;		//開始時刻
			double m_interval = 0;			//true返却間隔
		};

		//時間制限をかけるTImeLimiter
		class TimeLimiter
		{
		public:
			//----------------------------------------------------------------------------------
			//[Start]
			//制限を開始する
			//引数1: 制限時間
			inline void Start(double limit) 
			{
				//初期化
				TimeGetter timeGetter;
				m_startTime = timeGetter();
				m_limit = limit;
				m_previousStartTime = 0;
			}
			//----------------------------------------------------------------------------------
			//[ChangeLimit]
			//制限時刻を変更する
			//引数1: 制限時間
			inline void ChangeLimit(double limit)
			{
				m_limit = limit;
			}
			//----------------------------------------------------------------------------------
			//[operator ()]
			//return: 現在がStartで指定した制限時間経過フレームに該当する場合はtrue
			inline bool operator() ()
			{
				TimeGetter timeGetter;
				return this->operator()(timeGetter());
			}
			//----------------------------------------------------------------------------------
			//[operator ()]
			//return: 現在がStartで指定した制限時間経過フレームに該当する場合はtrue
			//引数1: 現在時刻(クロックベース)
			inline bool operator() (double nowTime)
			{
				m_deltaTime = (nowTime - m_startTime);
				
				//Timeが設定fps超過
				if (m_deltaTime >= m_limit)
				{
					//Time初期化
					m_previousStartTime = m_startTime;
					m_startTime = nowTime;
					return true;		//true返却
				}
				//違う場合false返す
				else
					return false;
			}
		
			//OldStartTime
			GetOnlyProperty <double> previousStartTime = GetOnlyProperty<double>(m_previousStartTime);

		private:
			double m_limit = 0.0f;						//制限時間
			double m_startTime = 0.0f;				//開始時刻
			double m_previousStartTime = 0.0f;	//前開始時刻
			double m_deltaTime = 0.0f;				//delta
		};

		//時間を計測するTimeCounter
		class TimeCounter
		{
		public:
			//----------------------------------------------------------------------------------
			//[Start]
			//計測を開始する
			inline void Start()
			{
				//初期化
				TimeGetter timeGetter;
				m_startTime = timeGetter();
			}
			//----------------------------------------------------------------------------------
			//[operator ()]
			//Startからの経過時間を返却
			inline float operator () ()
			{
				TimeGetter timeGetter;
				return this->operator()(timeGetter());
			}
			//----------------------------------------------------------------------------------
			//[operator ()]
			//Startからの経過時間を返却
			//引数1: 現在時刻(クロックベース)
			inline float operator() (float nowTime)
			{
				//return
				return nowTime - m_startTime;
			}
		
			//StartTime
			GetOnlyProperty<float> startTime = GetOnlyProperty<float>(m_startTime);

		private:
			float m_startTime = 0.0f;			//開始時刻
		};
	}

}
#endif // !SGFRAMEWORK_HEADER_TIME_PROCESSING_HPP_