/*----------------------------------------------------------------------------------
�ėp�X�V�X���b�h���Ǘ�����Thread class
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
	//friend�p
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
	
	//�ėp�X�V�X���b�h���Ǘ�����Thread class
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
		//BGM�X�g���[�~���O�Ȃǂ̐�������U��^�ʃX���b�h�֐���K�؂ȃX���b�h�Ɋ���U��
		inline static uint64 AutoThreadAllocation(const HANDLE& threadHandle)
		{
			static std::atomic<uint> allocation = 0;	//�V�t�g�p

			//null�Ȃ�I��
			if (threadHandle == nullptr) return 0;

			//�}�X�N�擾
			uint64 result = 1Ui64 << AtomicOperation::Add(allocation, 1) % 
				MathAF::Min(std::thread::hardware_concurrency(), m_numMaxUsingThreads);

			//�A�t�B�j�e�B�ݒ�
			SetThreadAffinityMask(threadHandle, result);

			return result;
		}
		//----------------------------------------------------------------------------------
		//[ThreadAllocation]
		//BGM�X�g���[�~���O�Ȃǂ̐�������U��^�ʃX���b�h�֐����}�j���A���œK�؂ȃX���b�h�Ɋ���U��
		inline static void ThreadAllocation(const HANDLE& threadHandle, uint64 useThread)
		{
			//null�Ȃ�I��
			if (threadHandle == nullptr || useThread == 0) return;

			//�A�t�B�j�e�B�ݒ�
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


		//���̃N���X���Ǘ����Ă���ėp�X�V��p�X���b�h��
		static GetOnlyProperty<uint> numUpdateThreads;
		//���̃N���X���Ǘ����Ă���X�V���C����
		static GetOnlyProperty<uint> numFunctionLines;
		//���s���Ă���PC�̃n�[�h�E�F�A�X���b�h��
		static GetOnlyProperty<uint> numHardwareThreads;

	private:
		//----------------------------------------------------------------------------------
		//[StartUp]
		//�X���b�h���N������
		//throw: �n�[�h�E�F�A�X���b�h����4�����̏ꍇ (�A�v���P�[�V���������s�ł��Ȃ�)
		//����1: �֐��L���[�̐�
		//����2: �ő�g�p�X���b�h��
		static void StartUp(const ReadElement::Pack& linesCount, const ReadElement::Pack& numMaxUsingThreads);
		//----------------------------------------------------------------------------------
		//[ShutDown]
		//�X���b�h���I������
		static void ShutDown();

		//----------------------------------------------------------------------------------
		//[MainThreadBeginBeforeSync]
		//���C���X���b�h�p�֐�
		//Update(before sync)->Begin
		static void MainThreadBeginBeforeSync();
		//----------------------------------------------------------------------------------
		//[MainBeginFixed]
		//���C���X���b�h�p�֐�
		//Update(before sync)->End
		static void MainThreadEndBeforeSync();
		//----------------------------------------------------------------------------------
		//[MainThreadBeginBeforeSync]
		//���C���X���b�h�p�֐�
		//Update(after sync)->Begin
		static void MainThreadBeginAfterSync();
		//----------------------------------------------------------------------------------
		//[MainBeginFixed]
		//���C���X���b�h�p�֐�
		//Update(after sync)->End
		static void MainThreadEndAfterSync();
		//----------------------------------------------------------------------------------
		//[MainThreadBeginFixed]
		//���C���X���b�h�p�֐�
		//FixedUpdate->Begin
		static void MainThreadBeginFixed();
		//----------------------------------------------------------------------------------
		//[MainThreadEndFixed]
		//���C���X���b�h�p�֐�
		//FixedUpdate->End
		static void MainThreadEndFixed();
		//----------------------------------------------------------------------------------
		//[UpdateThread]
		//�ėp�X�V�X���b�h�p�֐�
		//�֐��L���[�̏����Ǝ��s�ҋ@�����[�v����
		//����1: ���s�X���b�h�̗v�f�ԍ�
		static void UpdateThread(uint myThreadNumber);



		//----------------------------------------------------------------------------------
		//[MainThreadUpdate]
		//���C���X���b�h�p�֐�
		//�ėp�X�V�X���b�h�̑ҋ@��Ԃ��������A�֐��L���[�̏��������ōs��
		//����1: callbackType
		static void MainThreadSingleUpdate(uint callbackType);
		//----------------------------------------------------------------------------------
		//[MainThreadUpdate]
		//���C���X���b�h�p�֐�
		//�ėp�X�V�X���b�h�̑ҋ@��Ԃ��������A�֐��L���[�̏��������ōs��
		static void MainThreadUpdate();
		//----------------------------------------------------------------------------------
		//[MainThreadUpdate]
		//���C���X���b�h�p�֐�
		//�ėp�X�V�X���b�h�̑ҋ@��Ԃ��������A�֐��L���[�̏��������ōs��
		//����1: callbackType
		static void MainThreadUpdate(uint callbackType);
		//----------------------------------------------------------------------------------
		//[MainThreadInvokeCallbacks]
		//���C���X���b�h�p�֐�
		//�ėp�X�V�X���b�h�̑ҋ@��Ԃ��������A�R�[���o�b�N�Ăяo�������ōs��
		//����1: callbackType
		static void MainThreadInvokeCallbacks(uint callbackType);
		//----------------------------------------------------------------------------------
		//[MainThreadSingleInvokeCallbacks]
		//���C���X���b�h�p�֐�
		//�R�[���o�b�N�Ăяo�����V���O���X���b�h�ōs��
		//����1: callbackType
		static void MainThreadSingleInvokeCallbacks(uint callbackType);
		//----------------------------------------------------------------------------------
		//[MainThreadBeginUpdate]
		//���C���X���b�h�p�֐�
		//�ėp�X�V�X���b�h�̑ҋ@��Ԃ��������A�����X�V�������s����
		static void MainThreadBeginUpdate();
		//----------------------------------------------------------------------------------
		//[MainThreadGraphicsUpdate]
		//���C���X���b�h�p�֐�
		//�ėp�X�V�X���b�h�̑ҋ@��Ԃ��������A�`�惊�X�g�̍쐬���\�[�g�����ōs��
		static void MainThreadGraphicsUpdate();
		//----------------------------------------------------------------------------------
		//[ThreadUpdate1]
		//�ėp�X�V�X���b�h�p�֐�
		//�֐��L���[�̏����Ǝ��s�ҋ@�����[�v����
		//����1: ���s�X���b�h�̗v�f�ԍ�
		//����2: ��O�|�C���^
		static void ThreadUpdate1(uint myThreadNumber);
		//----------------------------------------------------------------------------------
		//[StartAudioGraphicsUpdate]
		//�I�[�f�B�I�O���t�B�b�N�A�b�v�f�[�g���J�n����
		static void StartAudioGraphicsUpdate();
		//----------------------------------------------------------------------------------
		//[WaitAudioGraphicsUpdate]
		//�I�[�f�B�I�O���t�B�b�N�A�b�v�f�[�g�̏I���܂őҋ@����
		static void WaitAudioGraphicsUpdate();
		//----------------------------------------------------------------------------------
		//[AudioGraphicsUpdate]
		//�I�[�f�B�I�O���t�B�b�N�D�揈���X���b�h�p�֐�
		//1�t���[���O�̏�����Ɍv�Z, �`���Đ����s��
		//����1: ��O�|�C���^
		static void AudioGraphicsUpdate();
		//----------------------------------------------------------------------------------
		//[AudioUpdate]
		//�I�[�f�B�I�����X���b�h�p�֐�
		//1�t���[���O�̏�����Ɍv�Z, �Đ����s��
		static void AudioUpdate();

		//----------------------------------------------------------------------------------
		//[RegisterRunningFrameworkThread]
		//AudioAssetMusic�Ȃǂ�Running�o�^���s��
		inline static void RegisterRunningFrameworkThread() { AtomicOperation::Add(m_runningCounter, 1); }
		//----------------------------------------------------------------------------------
		//[UnregisterRunningFrameworkThread]
		//AudioAssetMusic�Ȃǂ�Running�o�^�������s��
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

		//�ėp�X�V�X���b�h
		static std::vector<std::thread> m_updateThreads;								
		//�����X�V���X�g
		static std::vector<std::vector<void(*)()>> m_beginFunctions;
		//���C���������X�g
		static std::vector<std::vector<int>> m_lineAllocations;
		//�ėp�X���b�h�X�V�J�n�����ϐ�����
		static std::vector<ConditionVariable::AutoReference<bool>> m_startUpdateConditions;
		//�ėp�X���b�h�X�V�I�������ϐ�����
		static std::vector<ConditionVariable::AutoReference<bool>> m_endUpdateConditions;
		//�I�[�f�B�I�O���t�B�b�N��񓯊��J�n�����ϐ�
		static UniquePointer<ConditionVariable::AutoReference<bool>> m_startAudioGraphicsCondition;
		//�I�[�f�B�I�O���t�B�b�N��񓯊��I�������ϐ�
		static UniquePointer<ConditionVariable::AutoReference<bool>> m_endAudioGraphicsCondition;

		static std::thread m_audioGraphicsThread;			//AudioGraphicsUpdate�p�X���b�h
		static UniquePointer<bool[]> m_conditionFlags;		//�����ϐ��p�t���O����
		static std::atomic<uint> m_runningCounter;			//�g�p�X���b�h���v��
		static std::atomic<uint> m_beginCounter;			//BeginFunction�J�E���^
		static uint m_numMaxUsingThreads;						//�ő�g�p�X���b�h��Clamp
		static uint m_numHardwareThreads;						//PC�̃n�[�h�E�F�A�X���b�h��
		static uint m_numUseUpdateThreads;						//�{�N���X�����ėp�X�V�X���b�h��
		static uint m_numFunctionLines;								//�{�N���X�����X�V���C����
		static uint m_updateType;										//�����X�V���邩
		static uint m_callbackType;										//����Component->Callback�����s���邩
		static uint m_transformCount;									//transformUpdate�X���b�h���m�F�p
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
		//WinMain�Ɠ��������܂� (3�b���ƍX�V���������܂ŕ��ׂ͍����Ȃ��B�͂��B)
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