/*----------------------------------------------------------------------------------
�ėp�X�V�X���b�h���Ǘ�����Thread class
------------------------------------------------------------------------------------*/
#include "Thread.hpp"
#include "../05_Graphics/Graphics.hpp"
#include "../06_Audio/Audio.hpp"
#include "../97_Application/Application.hpp"

// Framework namespace
namespace SGFramework
{
	//----------------------------------------------------------------------------------
	//[MainThreadUpdate]
	//���C���X���b�h�p�֐�
	//�ėp�X�V�X���b�h�̑ҋ@��Ԃ��������A�֐��L���[�̏��������ōs��
	//����1: callbackType
	void Thread::MainThreadSingleUpdate(uint callbackType)
	{
		//Callback�X�V�ł�
		m_updateType = m_UpdateDefault;
		//�R�[���o�b�N���w��
		m_callbackType = callbackType;
		//�X�V
		Transform::UpdateForSingleThread(callbackType);
		//�I������
		Transform::EndUpdateForMultiThread();
	}
	//----------------------------------------------------------------------------------
	//[MainThreadUpdate]
	//���C���X���b�h�p�֐�
	//�ėp�X�V�X���b�h�̑ҋ@��Ԃ��������A�֐��L���[�̏��������ōs��
	void Thread::MainThreadUpdate()
	{
		//�ʏ�X�V�ł�
		m_updateType = m_UpdateNotCallback;
		//�R�[���o�b�N���w��
		m_callbackType = 0;
		//�X�V�J�n��������
		for (auto& e : m_startUpdateConditions)
			e.NotifyOne();

		//�X�V
		Transform::UpdateForMultiThread1();

		//�S�ẴX���b�h�ōX�V���I���܂őҋ@
		for (auto& e : m_endUpdateConditions)
			e.Wait();

		//�I������
		Transform::EndUpdateForMultiThread();
	}
	//----------------------------------------------------------------------------------
	//[MainThreadUpdate]
	//���C���X���b�h�p�֐�
	//�ėp�X�V�X���b�h�̑ҋ@��Ԃ��������A�֐��L���[�̏��������ōs��
	//����1: callbackType
	void Thread::MainThreadUpdate(uint callbackType)
	{
		//�ʏ�X�V�ł�
		m_updateType = m_UpdateDefault;
		//�R�[���o�b�N���w��
		m_callbackType = callbackType;
		//�X�V�J�n��������
		for (auto& e : m_startUpdateConditions)
			e.NotifyOne();

		//�X�V
		Transform::UpdateForMultiThread1(callbackType);

		//�S�ẴX���b�h�ōX�V���I���܂őҋ@
		for (auto& e : m_endUpdateConditions)
			e.Wait();
		
		//�I������
		Transform::EndUpdateForMultiThread();
	}
	//----------------------------------------------------------------------------------
	//[MainThreadInvokeCallbacks]
	//���C���X���b�h�p�֐�
	//�ėp�X�V�X���b�h�̑ҋ@��Ԃ��������A�R�[���o�b�N�Ăяo�������ōs��
	//����1: callbackType
	void Thread::MainThreadInvokeCallbacks(uint callbackType)
	{
		//Callback�X�V�ł�
		m_updateType = m_cInvokeCallbacks;
		//�R�[���o�b�N���w��
		m_callbackType = callbackType;
		//�X�V�J�n��������
		for (auto& e : m_startUpdateConditions)
			e.NotifyOne();

		//�X�V
		Transform::InvokeCallbackForMultiThread1(callbackType);

		//�S�ẴX���b�h�ōX�V���I���܂őҋ@
		for (auto& e : m_endUpdateConditions)
			e.Wait();

		//�I������
		Transform::EndUpdateForMultiThread();
	}
	//----------------------------------------------------------------------------------
	//[MainThreadSingleInvokeCallbacks]
	//���C���X���b�h�p�֐�
	//�R�[���o�b�N�Ăяo�����V���O���X���b�h�ōs��
	//����1: callbackType
	void Thread::MainThreadSingleInvokeCallbacks(uint callbackType)
	{
		//Callback�X�V�ł�
		m_updateType = m_cInvokeCallbacks;
		//�R�[���o�b�N���w��
		m_callbackType = callbackType;
		//�X�V
		Transform::InvokeCallbackForSingleThread(callbackType);
		//�I������
		Transform::EndUpdateForMultiThread();
	}
	//----------------------------------------------------------------------------------
	//[MainThreadBeginUpdate]
	//���C���X���b�h�p�֐�
	//�ėp�X�V�X���b�h�̑ҋ@��Ԃ��������A�����X�V�������s����
	void Thread::MainThreadBeginUpdate()
	{		
		//�����X�V�ł�
		m_updateType = m_UpdateBeginFunction;
		//�X�V�J�n��������
		for (auto& e : m_startUpdateConditions)
			e.NotifyOne();

		//�X�V
		Transform::SyncForMultiThread1(Component::CallbackFunctions::Update);


		//�S�ẴX���b�h�ōX�V���I���܂őҋ@
		for (auto& e : m_endUpdateConditions)
			e.Wait();

		AtomicOperation::Init(m_beginCounter, 0);

		//�I������
		Transform::EndUpdateForMultiThread();
	}
	//----------------------------------------------------------------------------------
	//[MainThreadGraphicsUpdate]
	//���C���X���b�h�p�֐�
	//�ėp�X�V�X���b�h�̑ҋ@��Ԃ��������A�`�惊�X�g�̍쐬���\�[�g�����ōs��
	void Thread::MainThreadGraphicsUpdate()
	{
		//�`�惊�X�g�쐬
		MainThreadInvokeCallbacks(Component::CallbackFunctions::MakeDrawCommand);

		//�\�[�g�����ł�
		m_updateType = m_UpdateSortGraphics;
		//�X�V�J�n��������
		for (auto& e : m_startUpdateConditions)
			e.NotifyOne();

		//�\�[�g
		Graphics::SortList();

		//�S�ẴX���b�h�ōX�V���I���܂őҋ@
		for (auto& e : m_endUpdateConditions)
			e.Wait();
	}

	void Thread::MainThreadBeginBeforeSync()
	{
		//�����X�V�ł�
		m_updateType = m_cUpdateModeBeforeSync;
		//�X�V�J�n��������
		for (auto& e : m_startUpdateConditions)
			e.NotifyOne();
	}
	
	void Thread::MainThreadEndBeforeSync()
	{
		//�S�ẴX���b�h�ōX�V���I���܂őҋ@
		for (auto& e : m_endUpdateConditions)
			e.Wait();

		AtomicOperation::Init(m_beginCounter, 0);
	}

	void Thread::MainThreadBeginAfterSync()
	{
		//Sync��X�V�ł�
		m_updateType = m_cUpdateModeAfterSync;
		//�X�V�J�n��������
		for (auto& e : m_startUpdateConditions)
			e.NotifyOne();
	}

	void Thread::MainThreadEndAfterSync()
	{
		//�\�[�g
		Graphics::SortList();

		//�S�ẴX���b�h�ōX�V���I���܂őҋ@
		for (auto& e : m_endUpdateConditions)
			e.Wait();
	}

	void Thread::MainThreadBeginFixed()
	{
		//Fixed�X�V�ł�
		m_updateType = m_cUpdateModeFixed;
		//�X�V�J�n��������
		for (auto& e : m_startUpdateConditions)
			e.NotifyOne();
	}

	void Thread::MainThreadEndFixed()
	{
		//�S�ẴX���b�h�ōX�V���I���܂őҋ@
		for (auto& e : m_endUpdateConditions)
			e.Wait();
	}

	//----------------------------------------------------------------------------------
	//[UpdateThread]
	//�ėp�X�V�X���b�h�p�֐�
	//�֐��L���[�̏����Ǝ��s�ҋ@�����[�v����
	//����1: ���s�X���b�h�̗v�f�ԍ�
	void Thread::UpdateThread(uint myThreadNumber)
	{
		//�X�V���[�v
		while (true)
		{
			//Wait
			m_startUpdateConditions[myThreadNumber].Wait();

			//Update->Default Before Sync 
			if (m_updateType & m_cUpdateModeBeforeSync)
			{
				if (m_beginFunctions[myThreadNumber].size() > 2)
				{
					uint i = 0;
					for (auto& function : m_beginFunctions[myThreadNumber])
					{
						if (i == 0) 
							function();
						else if (IS_TRUE(Application::m_isUpdateReady.load()))
							function();

						++i;
					}
				}
				else if (IS_TRUE(Application::m_isUpdateReady.load()))
				{
					for (auto& function : m_beginFunctions[myThreadNumber])
						function();
				}
			}
			//Update->Default After Sync 
			else if (m_updateType & m_cUpdateModeAfterSync)
			{
				if (IS_TRUE(Application::m_isUpdateReady.load()))
				{
					Transform::InvokeCallbackForMultiThread(m_lineAllocations[myThreadNumber],
						Component::CallbackFunctions::LateUpdate);
				}

				Transform::UpdateAndMakeMatrixForMultiThread(m_lineAllocations[myThreadNumber],
					Component::CallbackFunctions::MakeDrawCommand);
				Graphics::SortList();
			}
			//Update->Fixed
			else if (m_updateType & m_cUpdateModeFixed)
			{
				Transform::InvokeCallbackForMultiThread(m_lineAllocations[myThreadNumber],
					Component::CallbackFunctions::FixedUpdate);
			}
			//Update->Quit
			else if (m_updateType & m_cUpdateModeQuit)
				break;

			//�X�V�I��
			m_endUpdateConditions[myThreadNumber].NotifyOne();
		}

		//�X�V�I��
		m_endUpdateConditions[myThreadNumber].NotifyOne();
	}

	//----------------------------------------------------------------------------------
	//[ThreadUpdate1]
	//�ėp�X�V�X���b�h�p�֐�
	//�֐��L���[�̏����Ǝ��s�ҋ@�����[�v����
	//����1: ���s�X���b�h�̗v�f�ԍ�
	//����2: ��O�|�C���^
	void Thread::ThreadUpdate1(uint myThreadNumber)
	{
		//�X�V���[�v
		while (true)
		{
			//Wait
			m_startUpdateConditions[myThreadNumber].Wait();
			
			//Default Update
			if (m_updateType & m_UpdateDefault)
			{
				Transform::UpdateForMultiThread1(m_callbackType);
			}
			//Sort Update
			else if (m_updateType & m_UpdateSortGraphics)
			{
				Graphics::SortList();
			}
			//Begin Update
			else if (m_updateType & m_UpdateBeginFunction)
			{
				for(auto& e : m_beginFunctions[myThreadNumber])
					e();
			}
			//Invoke Callback
			else if (m_updateType & m_cInvokeCallbacks)
			{
				Transform::InvokeCallbackForMultiThread1(m_callbackType);
			}
			//NotCallback
			else if (m_updateType & m_UpdateNotCallback)
			{
				Transform::UpdateForMultiThread1();
			}
			//Quit
			else if(m_updateType & m_UpdateQuit)
				return;

			//�X�V�I��
			m_endUpdateConditions[myThreadNumber].NotifyOne();
		}
	}
	//----------------------------------------------------------------------------------
	//[StartAudioGraphicsUpdate]
	//�I�[�f�B�I�O���t�B�b�N�A�b�v�f�[�g���J�n����
	void Thread::StartAudioGraphicsUpdate()
	{
		//Update�J�n
		m_startAudioGraphicsCondition->NotifyOne();
	}		
	//----------------------------------------------------------------------------------
	//[WaitAudioGraphicsUpdate]
	//�I�[�f�B�I�O���t�B�b�N�A�b�v�f�[�g�̏I���܂őҋ@����
	void Thread::WaitAudioGraphicsUpdate()
	{
		//Update�I���ҋ@
		m_endAudioGraphicsCondition->Wait();
	}

	//----------------------------------------------------------------------------------
	//[AudioGraphicsUpdate]
	//�I�[�f�B�I�O���t�B�b�N�D�揈���X���b�h�p�֐�
	//1�t���[���O�̏�����Ɍv�Z, �`���Đ����s��
	//����1: ��O�|�C���^
	void Thread::AudioGraphicsUpdate()
	{
		//�X�V���[�v
		while (true)
		{
			//Update�J�n�ҋ@
			m_startAudioGraphicsCondition->Wait();
			
			//Quit->Return
			if (m_updateType & m_UpdateQuit) return;

			//Update
			Graphics::UpdateGraphics();

			//Update�I�� 
			m_endAudioGraphicsCondition->NotifyOne();
		}
	}
}