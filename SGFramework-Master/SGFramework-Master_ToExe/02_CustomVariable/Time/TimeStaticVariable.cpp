/*----------------------------------------------------------------------------------
���Ԃ���������Time Class
	��static�ϐ����L�q����TimeStaticVariable.cpp
------------------------------------------------------------------------------------*/
#include "Time.hpp"

//Framework namespace
namespace SGFramework
{
	//public �Q�b�^�[
	//double
	GetOnlyProperty<double> Time::timeWithClock(m_frequencyTime);								//Clock�x�[�X�̎���
	GetOnlyProperty<double> Time::fixedInterval(m_fixedInterval);									//�ݒ�ς݂�Fixed�X�V�Ԋu(�����l�v���Z�b�g)
	//float
	GetOnlyProperty<float> Time::fps(m_fps);																	//fps
	GetOnlyProperty<float> Time::targetFrameRate(m_refreshRate);									//�^�[�Q�b�g�t���[�����[�g
	GetOnlyProperty<float> Time::worldTimeScale(m_worldTimeScale);								//�edeltaTime�ɂ�����{��
	//float NotFixed
	GetOnlyProperty<float> Time::time(m_time);																//Start���s������̌o�ߕb��
	GetOnlyProperty<float> Time::worldDeltaTime(m_worldDeltaTime);								//�t���[�����Ƃ̎���
	GetOnlyProperty<float> Time::unScaledWorldDeltaTime(m_worldUnScaledDeltaTime);	//�t���[�����Ƃ̎���(Scale�������Ȃ�)
	//float Fixed		
	GetOnlyProperty<float> Time::fixedTime(m_fixedTime);												//Start���s������̌o�ߕb��(fixedFreame�ɍX�V)
	GetOnlyProperty<float> Time::fixedDeltaTime(m_fixedDeltaTime);								//�t���[�����Ƃ̎���(fixed)
	GetOnlyProperty<float> Time::unScaledFixedDeltaTime(m_unScaledFixedDeltaTime);	//�t���[�����Ƃ̎���(Scale�������Ȃ�, fixed)		
	//TimeLayer[0]
	GetOnlyProperty<SharedPointer<Time::TimeLayer>> Time::rootTimeLayer(m_rootLayer);
	//bool
	GetOnlyProperty<bool> Time::isUpdateFreame(m_isUpdateFreame);							//Update���s�t���[����
	GetOnlyProperty<bool> Time::isFixedUpdateFreame(m_isFixedUpdateFreame);				//Fixed�X�V�t���[����

	//private

	//TimeGetter
	TimeProcessing::TimeGetterDouble Time::m_timeGetter;
	//FPS����
	TimeProcessing::FPSLimiter Time::m_fpsLimiter;
	//FPS�v�����Ԑ���
	TimeProcessing::TimeLimiter Time::m_fpsInstrumentationLimiter;
	//rootLayer
	SharedPointer<Time::TimeLayer> Time::m_rootLayer;
	//LayerMap
	std::unordered_map<uint, SharedPointer<Time::TimeLayer>> Time::m_layerMap;

	//double
	double Time::m_frequencyTime = 0;					//���ݎ���
	double Time::m_startUpTime = 0;						//Start���s����
	double Time::m_updateStartTime = 0;				//delta�p�O��t���[������̃^�C��
	double Time::m_fixedInterval = 0;						//Fixed�X�V�Ԋu(�����l�v���Z�b�g)
	double Time::m_fixedStartTime = 0;					//Fixed�Ď��J�n����

	//float
	float Time::m_fps = 0.0f;									//����FPS
	float Time::m_worldTimeScale = 1.0f;					//�edeltaTime�ɂ�����{��
	float Time::m_refreshRate = 0.0f;						//��ʍX�V�Ԋu, �������p�ɏ���
	//float NotFixed
	float Time::m_time = 0.0f;									//Start���s������̌o�ߕb��
	float Time::m_worldDeltaTime = 0.0f;					//�t���[�����Ƃ̎���
	float Time::m_worldUnScaledDeltaTime = 0.0f;	//�t���[�����Ƃ̎���(Scale�������Ȃ�)
	//float Fixed
	float Time::m_fixedTime = 0.0f;							//Start���s������̌o�ߕb��(fixedFreame�ɍX�V)
	float Time::m_fixedDeltaTime = 0.0f;					//�t���[�����Ƃ̎���(fixed)
	float Time::m_unScaledFixedDeltaTime = 0.0f;	//�t���[�����Ƃ̎���(Scale�������Ȃ�, fixed)
	//uint
	uint Time::m_frameCount = 0;							//�t���[�����J�E���g
	uint Time::m_fixedCount = 0;								//Fixed�X�V��
	uint Time::m_layerCount = 0;								//���C���[�J�E���g
	//bool
	bool Time::m_isUpdateFreame = false;				//Update�t���[����
	bool Time::m_isFixedUpdateFreame = false;		//FixedUpdate�t���[����
	bool Time::m_isAllowUpdate = true;					//Update�������邩
}