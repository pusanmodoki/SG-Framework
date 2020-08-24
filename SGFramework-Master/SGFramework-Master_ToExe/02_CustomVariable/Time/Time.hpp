/*----------------------------------------------------------------------------------
���Ԃ���������Time Class
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
	//���Ԃ���������Time Class
	class Time final
	{
	public:
		//TimeScale��Layer class
		class TimeLayer;
		//friend
		friend TimeLayer;
		friend class Application;

		DELETE_ALL_DEFAULT_CLASS_FUNCTIONS(Time);

	private:

		//----------------------------------------------------------------------------------
		//[Start]
		//�^�C�������̏��������s��
		inline static void StartTimeProcessing(const ReadElement::Pack& isNoRestrictionsFPS,
			const ReadElement::Pack& fixedInterval, const ReadElement::Groop& layers, bool isRemote);
		//----------------------------------------------------------------------------------
		//[FixedUpdate]
		//Fixed�^�C�������̍X�V���s��
		//return: Fixed�X�V�t���[�����ۂ�, n��true���J��Ԃ�
		inline static bool FixedUpdate();
		//----------------------------------------------------------------------------------
		//[FixedLastUpdate]
		//Fixed�^�C�������̍ŏI�X�V���s��
		//�␳���[�h�̊m�F���s�����߃t���[�����s�����̒��ɓ���邱��
		inline static void FixedLastUpdate();
		//----------------------------------------------------------------------------------
		//[Update]
		//�^�C�������̍X�V���s��
		//return: �X�V�t���[�����ۂ�
		inline static bool Update();

	public:

		//----------------------------------------------------------------------------------
		//[FindTimeLayer]
		//TimeLayer���擾����
		//return: layerName�ɊY�����郌�C���[, ������Ȃ��ꍇ�͋�|�C���^
		//����1: ���C���[��(getTimeLayer���d��)
		inline static SharedPointer<TimeLayer> FindTimeLayer(const sgstring& layerName);

		//----------------------------------------------------------------------------------
		//[ResetWorldTimeScale]
		//RootLayer�̃^�C���X�P�[����ݒ肷��
		//0�ɂ���ƃ^�C���n�ϐ������S�ɃX�g�b�v����
		//����1: �ݒ肷��l(0~)
		inline static void ResetWorldTimeScale(float value);
		//----------------------------------------------------------------------------------
		//[CurrentTimeToString]
		//return: "[��:��:�b]"
		inline static sgstring CurrentTimeToString();
		//----------------------------------------------------------------------------------
		//[CurrentTimeToStdString]
		//return: "[��:��:�b]"
		inline static std::string CurrentTimeToStdString();
		//----------------------------------------------------------------------------------
		//[CurrentTime]
		//sec, min, hour�e������NULL�łȂ���Ό��ݎ�����������
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

		//�Q�b�^�[
		//double
		static GetOnlyProperty<double> timeWithClock;					//Clock�x�[�X�̎���
		static GetOnlyProperty<double> fixedInterval;						//�ݒ�ς݂�Fixed�X�V�Ԋu(�����l�v���Z�b�g)
		//float
		static GetOnlyProperty<float> fps;										//fps
		static GetOnlyProperty<float> targetFrameRate;					//�^�[�Q�b�g�t���[�����[�g
		static GetOnlyProperty<float> worldTimeScale;					//�edeltaTime�ɂ�����{��
		//float NotFixed
		static GetOnlyProperty<float> time;									//Start���s������̌o�ߕb��
		static GetOnlyProperty<float> worldDeltaTime;					//�t���[�����Ƃ̎���
		static GetOnlyProperty<float> unScaledWorldDeltaTime;		//�t���[�����Ƃ̎���(Scale�������Ȃ�)
		//float Fixed		
		static GetOnlyProperty<float> fixedTime;								//Start���s������̌o�ߕb��(fixedFreame�ɍX�V)
		static GetOnlyProperty<float> fixedDeltaTime;						//�t���[�����Ƃ̎���(fixed)
		static GetOnlyProperty<float> unScaledFixedDeltaTime;		//�t���[�����Ƃ̎���(Scale�������Ȃ�, fixed)		
		//TimeLayer[0]
		static GetOnlyProperty<SharedPointer<TimeLayer>> rootTimeLayer;
		//bool
		static GetOnlyProperty<bool> isUpdateFreame;					//Update���s�t���[����
		static GetOnlyProperty<bool> isFixedUpdateFreame;			//Fixed�X�V�t���[����

		//TimeScale��Layer class
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
			//argument 1: �ݒ�X�P�[�� (0~)
			SGF_FUNCTION_PROPERTY void setScale(float timeScale) { m_timeScale = timeScale; if (m_timeScale < 0.0f) m_timeScale = 0.0f; }
			//set childrens time scale property(set only)
			//argument 1: �ݒ�X�P�[�� (0~)
			SGF_FUNCTION_PROPERTY void setScaleToChildrens(float timeScale) { for (auto& it : m_childrens) it->setScale(timeScale); }

			static constexpr wchar cRootName[] = L"root";
			static constexpr uint cInvaridID = VariableLimit::cUintMax;

			GetOnlyProperty<float> timeScale;						//���݂̃^�C���X�P�[��, Root���玩�g�܂ł̂��ׂĂ�settingScale���������l�ɂȂ�
			GetOnlyProperty<float> settingScale;					//�^�C���X�P�[���ݒ�l
			GetOnlyProperty<float> deltaTime;						//���̃��C���[��deltaTime
			GetOnlyProperty<float> fixedDeltaTime;				//���̃��C���[��fixedDeltaTime
			GetOnlyProperty<float> unscaledDeltaTime;		//unscaledDeltaTime * this time scale
			GetOnlyProperty<float> unscaledFixedDeltaTime;	//unscaledDeltaTime * this time scale
			GetOnlyProperty<sgstring> layerName;				//���C���[��
			GetOnlyProperty<uint> layerID;							//���C���[ID

		private:
			//----------------------------------------------------------------------------------
			//[�R���X�g���N�^]
			inline TimeLayer();

			//----------------------------------------------------------------------------------
			//[InitRoot]
			//Root���C���[�p, �����ݒ���s��
			inline void InitRoot();
			//----------------------------------------------------------------------------------
			//[Init]
			//Root�ȊO�̃��C���[�p, �����ݒ���s��
			//����1: this Pointer
			//����2: �e���C���[
			//����3: ���C���[��
			//����4: �ݒ�X�P�[��
			//����5: ���C���[ID
			inline void Init(SharedPointer<TimeLayer>& thisPointer,
				SharedPointer<TimeLayer>& parent, const sgstring& myLayerName, float settingScale, uint layerID);


			//----------------------------------------------------------------------------------
			//[Update]
			//deltaTime�̍X�V���s��
			inline void Update();

			//�q���C���[
			std::vector<WeakPointer<TimeLayer>> m_childrens;
			sgstring m_layerName;							//���C���[��
			WeakPointer<TimeLayer> m_parent;		//�e���C���[
			float m_timeScale;									//���݃X�P�[��
			float m_settingScale;								//�ݒ�X�P�[��
			float m_deltaTime;									//deltaTime
			float m_fixedDeltaTime;							//fixedDeltaTime
			float m_unscaledDeltaTime;					//unscaledDeltaTime * this time scale
			float m_unscaledFixedDeltaTime;				//unscaledDeltaTime * this time scale
			uint m_layerID;										//���C���[ID
		};

	private:

		//force allow update = true<static> (set only property)
		SGF_FUNCTION_PROPERTY static void setFourceUpdate() { m_isAllowUpdate = true; }

		//TimeGetter
		static TimeProcessing::TimeGetterDouble m_timeGetter;
		//FPS����
		static TimeProcessing::FPSLimiter m_fpsLimiter;
		//FPS�v�����Ԑ���
		static TimeProcessing::TimeLimiter m_fpsInstrumentationLimiter;
		//rootLayer
		static SharedPointer<TimeLayer> m_rootLayer;
		//LayerMap
		static std::unordered_map<uint, SharedPointer<TimeLayer>> m_layerMap;


		//double
		static double m_frequencyTime;						//���ݎ���
		static double m_startUpTime;							//Start���s����
		static double m_updateStartTime;					//delta�p�O��t���[������̃^�C��
		static double m_fixedInterval;							//Fixed�X�V�Ԋu(�����l�v���Z�b�g)
		static double m_fixedStartTime;						//Fixed�Ď��J�n����

		//float
		static float m_fps;											//����FPS
		static float m_worldTimeScale;						//�edeltaTime�ɂ�����{��
		static float m_refreshRate;								//��ʍX�V�Ԋu, �������p�ɏ���
		//float NotFixed
		static float m_time;										//Start���s������̌o�ߕb��
		static float m_worldDeltaTime;						//�t���[�����Ƃ̎���
		static float m_worldUnScaledDeltaTime;			//�t���[�����Ƃ̎���(Scale�������Ȃ�)
		//float Fixed
		static float m_fixedTime;									//Start���s������̌o�ߕb��(fixedFreame�ɍX�V)
		static float m_fixedDeltaTime;							//�t���[�����Ƃ̎���(fixed)
		static float m_unScaledFixedDeltaTime;			//�t���[�����Ƃ̎���(Scale�������Ȃ�, fixed)
		//uint
		static uint m_frameCount;								//�t���[�����J�E���g
		static uint m_fixedCount;								//Fixed�X�V��
		static uint m_layerCount;								//���C���[�J�E���g
		//bool
		static bool m_isUpdateFreame;						//Update�t���[����
		static bool m_isFixedUpdateFreame;				//FixedUpdate�t���[����
		static bool m_isAllowUpdate;							//Update�������邩
	};

	//----------------------------------------------------------------------------------
	//[Start]
	//�^�C�������̏��������s��
	inline void Time::StartTimeProcessing(const ReadElement::Pack& isNoRestrictionsFPS,
		const ReadElement::Pack& fixedInterval, const ReadElement::Groop& layers, bool isRemote)
	{
		//���t���b�V�����[�g�擾
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
		//Interval���
		m_fixedInterval = fixedInterval[0].valueFloat;

		//RootLayer�ݒ�
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

		m_fpsLimiter.Start(m_refreshRate);		//FPS�����J�n
		m_fpsInstrumentationLimiter.Start(1.0f);	//FPS�v�������J�n

		m_startUpTime = m_timeGetter();			//Start���s����
		//���
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
	//Fixed�^�C�������̍X�V���s��
	//return: Fixed�X�V�t���[�����ۂ�, n��true���J��Ԃ�
	inline bool Time::FixedUpdate()
	{
		static double delta = 0;	//Intervl�m�F���[�v�p

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
			m_fpsLimiter.Start(m_refreshRate);		//FPS�����J�n
			m_fpsInstrumentationLimiter.Start(1.0f);	//FPS�v�������J�n
			//fixed�X�V����
			m_fixedStartTime = m_frequencyTime - m_fixedInterval;
			m_fixedCount = 0;
		}

		//FixedCount���܂��c���Ă���
		if (m_fixedCount > 1)
		{
			--m_fixedCount;		//�J�E���^�[���Z
			//Fixed�X�V�񐔂��܂�����݌v�X�V���Ԃ�FixedInterval�𒴉߂����ꍇ�X�V����߂�
			if (m_fixedCount > 1 && m_timeGetter() - m_fixedStartTime > m_fixedInterval)
				m_fixedCount = 0;
			//true�ԋp
			return m_isFixedUpdateFreame = true;
		}

		//���݂�Fixed�������Ԏ擾
		delta = m_frequencyTime - m_fixedStartTime;
		//�J�E���^�[������
		m_fixedCount = 0;

		//Interval�m�F���[�v
		while (delta > m_fixedInterval)
		{
			++m_fixedCount;				//�J�E���^�[���Z
			delta -= m_fixedInterval;	//delta���Z
		}
		//�J�E���^�[0���� -> Fixed Update Frame
		if (m_fixedCount > 0)
		{
			//Get FixedApplicationElapasedTime
			m_fixedTime = static_cast<float>(m_frequencyTime - m_startUpTime);
			//Get FixedDeltaTime
			m_unScaledFixedDeltaTime = static_cast<float>(m_frequencyTime - m_fixedStartTime);
			//delta�ݒ�
			m_fixedDeltaTime = m_unScaledFixedDeltaTime * m_worldTimeScale;

			m_time = m_fixedTime;							//Time���
			m_fixedStartTime += m_fixedInterval;	//StartTime�X�V
			return m_isFixedUpdateFreame = true;	//true�ԋp
		}

		//Update�͋���
		m_isAllowUpdate = true;
		//false�ԋp
		return m_isFixedUpdateFreame = false;
	}
	//----------------------------------------------------------------------------------
	//[FixedLastUpdate]
	//Fixed�^�C�������̍ŏI�X�V���s��
	//�␳���[�h�̊m�F���s�����߃t���[�����s�����̒��ɓ���邱��
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

		//FixedUpdate���s�b����Interval����->Update�X�V�������Ȃ�
		//���߂��Ă��Ȃ����Update�X�V��������
		if (m_timeGetter() - m_fixedStartTime < m_fixedInterval)
			m_isAllowUpdate = true;
		else
			m_isAllowUpdate = false;
	}

	//----------------------------------------------------------------------------------
	//[Update]
	//�^�C�������̍X�V���s��
	//return: �X�V�t���[�����ۂ�
	inline bool Time::Update()
	{
		//Update�X�V��������Ă��Ȃ��ꍇ��false�ԋp
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
	//TimeLayer���擾����
	//return: layerName�ɊY�����郌�C���[, ������Ȃ��ꍇ�͋�|�C���^
	//����1: ���C���[��(getTimeLayer���d��)
	inline SharedPointer<Time::TimeLayer> Time::FindTimeLayer(const sgstring& layerName)
	{
		for (auto& it : m_layerMap)
			if (it.second->layerName() == layerName) return it.second;
		return SharedPointer<Time::TimeLayer>::getNull();
	}

	//----------------------------------------------------------------------------------
	//[ResetWorldTimeScale]
	//RootLayer�̃^�C���X�P�[����ݒ肷��
	//0�ɂ���ƃ^�C���n�ϐ������S�ɃX�g�b�v����
	//����1: �ݒ肷��l(0~)
	inline void Time::ResetWorldTimeScale(float value)
	{
		m_worldTimeScale = value;		//�ݒ�
		if (m_worldTimeScale < 0.0f)	m_worldTimeScale = 0.0f;

		//Delta Update
		m_worldDeltaTime = m_worldUnScaledDeltaTime * m_worldTimeScale;
		//Root�ݒ�
		m_rootLayer->Update();
		//Update
		for (auto& it : m_layerMap)
			it.second->Update();

		//Fixed�ݒ�
		m_fixedDeltaTime = static_cast<float>(m_fixedInterval) * m_worldTimeScale;
	}
	//----------------------------------------------------------------------------------
	//[CurrentTimeToString]
	//return: "[��:��:�b]"
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
	//return: "[��:��:�b]"
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
	//sec, min, hour�e������NULL�łȂ���Ό��ݎ�����������
	inline void Time::CurrentTime(int * sec, int * min, int * hour)
	{
		tm t;
		mktime(&t);
		if (sec != NULL) *sec = t.tm_sec;
		if (min != NULL) *min = t.tm_min;
		if (hour != NULL) *hour = t.tm_hour;
	}


	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	inline Time::TimeLayer::TimeLayer()
		: timeScale(m_timeScale), settingScale(m_settingScale), deltaTime(m_deltaTime),
		fixedDeltaTime(m_fixedDeltaTime), unscaledDeltaTime(m_unscaledDeltaTime),
		unscaledFixedDeltaTime(m_unscaledFixedDeltaTime), layerName(m_layerName), layerID(m_layerID) {}
	//----------------------------------------------------------------------------------
	//[InitRoot]
	//Root���C���[�p, �����ݒ���s��
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
	//Root�ȊO�̃��C���[�p, �����ݒ���s��
	//����1: this Pointer
	//����2: �e���C���[
	//����3: ���C���[��
	//����4: �ݒ�X�P�[��
	//����5: ���C���[ID
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
	//deltaTime�̍X�V���s��
	inline void Time::TimeLayer::Update()
	{
		//Root���C���[
		if (m_layerID == 0)
		{
			//world�l�����̂܂ܐݒ�
			m_deltaTime = Time::worldDeltaTime * m_timeScale;
			m_fixedDeltaTime = Time::fixedDeltaTime * m_timeScale;
			m_unscaledDeltaTime = Time::m_worldUnScaledDeltaTime * m_timeScale;
			m_unscaledFixedDeltaTime = Time::m_unScaledFixedDeltaTime* m_timeScale;
		}
		//����ȊO
		else
		{
			//���g�̂��̂��ЂƂ܂��ݒ�
			m_timeScale = m_settingScale;
			//�e���擾(Root������̂ŃG���[�͋N���Ȃ�)
			auto layer = m_parent.TryGetInstance();
			//�|���Z���[�v, Root�ɂ��ǂ蒅���܂ŏI���Ȃ�
			do {
				m_timeScale = m_timeScale * layer->settingScale;
				layer = layer->m_parent.TryGetInstance();
			} while (layer != nullptr);

			//deltaTime�ݒ�
			m_deltaTime = Time::worldDeltaTime * m_timeScale;
			m_fixedDeltaTime = Time::fixedDeltaTime * m_timeScale;
			m_unscaledDeltaTime = Time::m_worldUnScaledDeltaTime * m_timeScale;
			m_unscaledFixedDeltaTime = Time::m_unScaledFixedDeltaTime* m_timeScale;
		}
	}
}
#endif // !SGFRAMEWORK_HEADER_TIME_HPP_