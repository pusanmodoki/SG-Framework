/*----------------------------------------------------------------------------------
�ėp�X�V�X���b�h���Ǘ�����Thread class
------------------------------------------------------------------------------------*/
#include "Thread.hpp"
#include "../05_Graphics/Graphics.hpp"
#include "../06_Audio/Audio.hpp"

// Framework namespace
namespace SGFramework
{
	//���̃N���X���Ǘ����Ă���ėp�X�V��p�X���b�h��
	GetOnlyProperty<uint> Thread::numUpdateThreads(m_numUseUpdateThreads);
	//���̃N���X���Ǘ����Ă���X�V���C����
	GetOnlyProperty<uint> Thread::numFunctionLines(m_numFunctionLines);
	//���s���Ă���PC�̃n�[�h�E�F�A�X���b�h��
	GetOnlyProperty<uint> Thread::numHardwareThreads(m_numHardwareThreads);

	//�ėp�X�V�X���b�h
	std::vector<std::thread> Thread::m_updateThreads;							
	//�����X�V���X�g
	std::vector<std::vector<void(*)()>> Thread::m_beginFunctions;
	//���C���������X�g
	std::vector<std::vector<int>> Thread::m_lineAllocations;
	
	//�ėp�X���b�h�X�V�J�n�����ϐ�����
	std::vector<ConditionVariable::AutoReference<bool>> Thread::m_startUpdateConditions;
	//�ėp�X���b�h�X�V�I�������ϐ�����
	std::vector<ConditionVariable::AutoReference<bool>> Thread::m_endUpdateConditions;
	//�I�[�f�B�I�O���t�B�b�N��񓯊��J�n�����ϐ�
	UniquePointer<ConditionVariable::AutoReference<bool>> Thread::m_startAudioGraphicsCondition;
	//�I�[�f�B�I�O���t�B�b�N��񓯊��I�������ϐ�
	UniquePointer<ConditionVariable::AutoReference<bool>> Thread::m_endAudioGraphicsCondition;

	std::thread Thread::m_audioGraphicsThread;			//AudioGraphicsUpdate�p�X���b�h
	UniquePointer<bool[]> Thread::m_conditionFlags;		//�����ϐ��p�t���O����
	std::atomic<uint> Thread::m_runningCounter = 0;	//�g�p�X���b�h���v��
	std::atomic<uint> Thread::m_beginCounter = 0;		//BeginFunction�J�E���^
	uint Thread::m_numMaxUsingThreads = 0;					//�ő�g�p�X���b�h��Clamp
	uint Thread::m_numHardwareThreads = 0;					//PC�̃n�[�h�E�F�A�X���b�h��
	uint Thread::m_numFunctionLines = 0;						//�{�N���X�����X�V���C����
	uint Thread::m_numUseUpdateThreads = 0;				//�{�N���X�����ėp�X�V�X���b�h��
	uint Thread::m_updateType = 0;									//�����X�V���邩
	uint Thread::m_callbackType = 0;								//����Component->Callback�����s���邩
	uint Thread::m_transformCount = 0;							//transformUpdate�X���b�h���m�F�p
	
	//----------------------------------------------------------------------------------
	//[StartUp]
	//�X���b�h���N������
	//throw: �n�[�h�E�F�A�X���b�h����4�����̏ꍇ (�A�v���P�[�V���������s�ł��Ȃ�)
	//����1: �֐��L���[�̐�
	//����2: �ő�g�p�X���b�h��
	void Thread::StartUp(const ReadElement::Pack& numLines, const ReadElement::Pack& numMaxUsingThreads)
	{
		//�X���b�h���擾
		m_numHardwareThreads = std::thread::hardware_concurrency();
		//4�X���b�h�����ȏꍇ���s�ł��Ȃ��̂�throw
		if (m_numHardwareThreads < 4)
			throw InvalidArgument(L"Error!! Thread->StartUp", L"This application requires a cpu with 4 or more threads");

		//�ő�g�p�X���b�h���擾
		m_numMaxUsingThreads = static_cast<uint>(numMaxUsingThreads[0].valueInt);

		//�g�p�X���b�h���֐����C�����Ǝ擾�ł���ő�X���b�h�����r�����������ɐݒ�
		m_numUseUpdateThreads = MathAF::Min(m_numHardwareThreads - m_cNumExclusiveUseThreads,
			static_cast<uint>(numLines[0].valueInt));
		//�ő�g�p�X���b�h���܂ŃN�����v
		m_numUseUpdateThreads = MathAF::Min(m_numUseUpdateThreads, m_numMaxUsingThreads);

		//�֐����C����
		m_numFunctionLines = static_cast<uint>(numLines[0].valueInt);
		//�����ϐ��p�t���O
		m_conditionFlags = Pointer::MakeUniqueArray::Auto<bool>((m_numUseUpdateThreads * 2) + 2);

		//���C������
		for (uint i = 0; i < m_numFunctionLines; ++i)
		{
			if (i < m_numUseUpdateThreads)
				m_lineAllocations.emplace_back(std::vector<int>());

			m_lineAllocations[i % m_numUseUpdateThreads].emplace_back(static_cast<int>(i));
		}

		//Begin Functions�쐬
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


			//�萔��������Ȃ��̂ŃS���S���v���O������g�݂܂�
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

		//�����ϐ��쐬
		m_startAudioGraphicsCondition = Pointer::MakeUnique::Auto<ConditionVariable::AutoReference<bool>>(m_conditionFlags[m_conditionFlags.getArraySize() - 2]);
		m_endAudioGraphicsCondition = Pointer::MakeUnique::Auto<ConditionVariable::AutoReference<bool>>(m_conditionFlags[m_conditionFlags.getArraySize() - 1]);
		//���������ϐ��쐬
		for (uint i = 0; i < m_numUseUpdateThreads; i++)
		{
			m_startUpdateConditions.emplace_back(m_conditionFlags[i]);
			m_endUpdateConditions.emplace_back(m_conditionFlags[m_numUseUpdateThreads + i]);
		}

		//�X���b�h��K�v�����쐬, �X�^���o�C��Ԃɂ�����
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


		//�X���b�h�쐬
		m_audioGraphicsThread = std::thread(AudioGraphicsUpdate);
		setAffinityGraphicsUpdate(m_audioGraphicsThread.native_handle());
		
		//�O���t�B�b�N�X�̃R�}���h���X�g��������
		Graphics::InitCommandList(m_updateThreads);
		//�����_���o�^
		Random::InitThreads(m_updateThreads);
		Random::InitThread(m_audioGraphicsThread);
		//GUI�o�^
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
	//�X���b�h���I������
	void Thread::ShutDown()
	{
		//�S�X���b�h�I���t���O�ݒ�
		m_updateType = m_UpdateQuit;
		//�X�V�J�n������(�����邽��)
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