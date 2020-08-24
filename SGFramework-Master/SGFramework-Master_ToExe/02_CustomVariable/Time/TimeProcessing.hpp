/*----------------------------------------------------------------------------------
Time����������N���X���W�߂�TimeProcessing.hpp
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_TIME_PROCESSING_HPP_
#define SGFRAMEWORK_HEADER_TIME_PROCESSING_HPP_
#include "../Property/Property.hpp"

//Framework namespace
namespace SGFramework
{
	//Time�֘A�N���X
	namespace TimeProcessing
	{
		//���ݎ���(�N���b�N�x�[�X)���擾����TimeGetter class
		class TimeGetter
		{
		public:
			//----------------------------------------------------------------------------------
			//[() operator]
			//return: ���ݎ���(�N���b�N�x�[�X)
			inline float operator() ()
			{
				LARGE_INTEGER frequency;						//���ݎ��g���ۑ�
				LARGE_INTEGER timeCounter;					//���݃J�E���^�[�ۑ�			
				//���g���擾
				QueryPerformanceFrequency(&frequency);
				//�J�E���^�[�擾
				QueryPerformanceCounter(&timeCounter);
				//���ݎ��Ԃ�Ԃ�
				return static_cast<float>(timeCounter.QuadPart) / static_cast<float>(frequency.QuadPart);
			}
		};

		//���ݎ���(�N���b�N�x�[�X)���擾����TimeGetterDouble class(double)
		class TimeGetterDouble
		{
		public:
			//----------------------------------------------------------------------------------
			//[() operator]
			//return: ���ݎ���(�N���b�N�x�[�X)
			inline double operator() ()
			{
				LARGE_INTEGER frequency;						//���ݎ��g���ۑ�
				LARGE_INTEGER timeCounter;					//���݃J�E���^�[�ۑ�			
				//���g���擾
				QueryPerformanceFrequency(&frequency);
				//�J�E���^�[�擾
				QueryPerformanceCounter(&timeCounter);
				//���ݎ��Ԃ�Ԃ�
				return static_cast<double>(timeCounter.QuadPart) / static_cast<double>(frequency.QuadPart);
			}
		};

		//Start()����Stop()�܂ł̎��Ԃ𑪂�StopWatch class
		class StopWatch
		{
		public:
			//----------------------------------------------------------------------------------
			//[Start]
			//�X�g�b�v�E�H�b�`���J�n����
			inline void Start()
			{
				//������
				m_elapasedTime = 0.0f;
				m_startTime = m_timeGetter();
			}
			//----------------------------------------------------------------------------------
			//[Resume]
			//Stop���̃X�g�b�v�E�H�b�`���ĊJ����
			inline void Resume()
			{
				//������
				m_startTime = m_timeGetter();
			}
			//----------------------------------------------------------------------------------
			//[Stop]
			//�X�g�b�v�E�H�b�`���~����, �����ŏ��߂�elapasedMilliSecond �ɒl������
			inline void Stop()
			{
				//���ݎ������擾���Č��Z���~���b�ɂ��邽�߂�*1000
				double stopTime = m_timeGetter();
				m_elapasedTime = (stopTime - m_startTime) * 1000.0f;
			}

			//Start����Stop�܂ł̕b��(�~���b)
			GetOnlyProperty<double> elapasedMilliSecond = GetOnlyProperty<double>(m_elapasedTime);
			//StartTime
			GetOnlyProperty<double> startTime = GetOnlyProperty<double>(m_startTime);

		private:
			TimeGetterDouble m_timeGetter;		//Time�擾�p
			double m_startTime = 0.0f;				//�J�n����
			double m_elapasedTime = 0.0f;			//Start����Stop�܂ł̎���
		};

		//FPS������������FPSLimiter
		class FPSLimiter
		{
		public:
			//----------------------------------------------------------------------------------
			//[Start]
			//�������J�n����
			//����1: ����FPS
			inline void Start(double fps)
			{
				//������
				TimeGetter timeGetter;
				m_startTime = timeGetter();
				m_fps = fps;
				m_interval = 1 / m_fps;
			}
			//----------------------------------------------------------------------------------
			//[ChangeFPS]
			//����FPS��ύX����
			//����1: ����FPS
			inline void ChangeFPS(double fps)
			{
				m_fps = fps;
				m_interval = 1 / m_fps;
			}
			//----------------------------------------------------------------------------------
			//[operator ()]
			//return: ���݂�Start�Ŏw�肵�������t���[���ɊY������ꍇ��true
			inline bool operator() ()
			{
				TimeGetter timeGetter;
				return this->operator()(timeGetter());
			}
			//----------------------------------------------------------------------------------
			//[operator ()]
			//return: ���݂�Start�Ŏw�肵�������t���[���ɊY������ꍇ��true
			//����1: ���ݎ���(�N���b�N�x�[�X)
			inline bool operator() (double nowTime)
			{
				//Time���ݒ�fps����
				if ((nowTime - m_startTime) >= m_interval)
				{
					//Time������
					m_startTime = nowTime;
					return true;		//true�ԋp
				}
				//�Ⴄ�ꍇfalse�Ԃ�
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
			double m_fps = 0;					//����FPS
			double m_startTime = 0;		//�J�n����
			double m_interval = 0;			//true�ԋp�Ԋu
		};

		//���Ԑ�����������TImeLimiter
		class TimeLimiter
		{
		public:
			//----------------------------------------------------------------------------------
			//[Start]
			//�������J�n����
			//����1: ��������
			inline void Start(double limit) 
			{
				//������
				TimeGetter timeGetter;
				m_startTime = timeGetter();
				m_limit = limit;
				m_previousStartTime = 0;
			}
			//----------------------------------------------------------------------------------
			//[ChangeLimit]
			//����������ύX����
			//����1: ��������
			inline void ChangeLimit(double limit)
			{
				m_limit = limit;
			}
			//----------------------------------------------------------------------------------
			//[operator ()]
			//return: ���݂�Start�Ŏw�肵���������Ԍo�߃t���[���ɊY������ꍇ��true
			inline bool operator() ()
			{
				TimeGetter timeGetter;
				return this->operator()(timeGetter());
			}
			//----------------------------------------------------------------------------------
			//[operator ()]
			//return: ���݂�Start�Ŏw�肵���������Ԍo�߃t���[���ɊY������ꍇ��true
			//����1: ���ݎ���(�N���b�N�x�[�X)
			inline bool operator() (double nowTime)
			{
				m_deltaTime = (nowTime - m_startTime);
				
				//Time���ݒ�fps����
				if (m_deltaTime >= m_limit)
				{
					//Time������
					m_previousStartTime = m_startTime;
					m_startTime = nowTime;
					return true;		//true�ԋp
				}
				//�Ⴄ�ꍇfalse�Ԃ�
				else
					return false;
			}
		
			//OldStartTime
			GetOnlyProperty <double> previousStartTime = GetOnlyProperty<double>(m_previousStartTime);

		private:
			double m_limit = 0.0f;						//��������
			double m_startTime = 0.0f;				//�J�n����
			double m_previousStartTime = 0.0f;	//�O�J�n����
			double m_deltaTime = 0.0f;				//delta
		};

		//���Ԃ��v������TimeCounter
		class TimeCounter
		{
		public:
			//----------------------------------------------------------------------------------
			//[Start]
			//�v�����J�n����
			inline void Start()
			{
				//������
				TimeGetter timeGetter;
				m_startTime = timeGetter();
			}
			//----------------------------------------------------------------------------------
			//[operator ()]
			//Start����̌o�ߎ��Ԃ�ԋp
			inline float operator () ()
			{
				TimeGetter timeGetter;
				return this->operator()(timeGetter());
			}
			//----------------------------------------------------------------------------------
			//[operator ()]
			//Start����̌o�ߎ��Ԃ�ԋp
			//����1: ���ݎ���(�N���b�N�x�[�X)
			inline float operator() (float nowTime)
			{
				//return
				return nowTime - m_startTime;
			}
		
			//StartTime
			GetOnlyProperty<float> startTime = GetOnlyProperty<float>(m_startTime);

		private:
			float m_startTime = 0.0f;			//�J�n����
		};
	}

}
#endif // !SGFRAMEWORK_HEADER_TIME_PROCESSING_HPP_