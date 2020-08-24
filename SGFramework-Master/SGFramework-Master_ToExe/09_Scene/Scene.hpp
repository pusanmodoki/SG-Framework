#ifndef SGFRAMEWORK_HEADER_SCENE_HPP_
#define SGFRAMEWORK_HEADER_SCENE_HPP_
#include "../02_CustomVariable/CustomVariable.hpp"

//Framework namespace
namespace SGFramework
{
	class GUI;

	//Scene
	namespace Scene
	{
		//�V�[�������p�������i�[����SceneBaton
		struct SceneBaton
		{
			std::vector<int> valueInts;
			std::vector<float> valueFloats;
			std::vector<sgstring> valueStrings;
			std::vector<Vector3> valueVectors;
			std::vector<SharedPointer<Object>> valueObjects;
		};

		class BaseScene;	//�t���[�����[�N�ŊǗ�����V�[���̃x�[�X�ƂȂ�BaseScene class
		class SubScene;		//�t���[�����[�N�ŊǗ�����T�u�V�[���̃x�[�X�ƂȂ�SubBaseScene class
		
		//�t���[�����[�N�ŊǗ�����V�[���̃x�[�X�ƂȂ�BaseScene class
		class BaseScene
		{
		public:
			//friend
			friend class SceneManager;
			friend class GUI;

			//----------------------------------------------------------------------------------
			//[�f�X�g���N�^]
			//���ɉ������Ȃ�
			inline virtual ~BaseScene(){}

			//----------------------------------------------------------------------------------
			//[StartScene]
			//�V�[���쐬�������ŌĂяo�����R�[���o�b�N�֐�
			//����1: �O�V�[������̈����p�����
			virtual void StartScene(SceneBaton& nextBaton) = 0;

			//----------------------------------------------------------------------------------
			//[EndScene]
			//�V�[���I���������ŌĂяo�����R�[���o�b�N�֐�, �I���������s��
			//�Ԃ�l��null�|�C���^�������ꍇ�A�A�v���P�[�V�������I������
			//����1: ���V�[���ֈ����p�����, Start�ŌĂяo�����
			//����2: true��������Ǝ��V�[���ȊO�̃X�^�b�N��{�V�[���܂߂ăX�^�b�N����N���A����
			//����2: true��������Ɩ{�V�[�����X�^�b�N���炷��
			virtual UniquePointer<BaseScene> EndScene(SceneBaton& nextBaton, bool& isStackClear, bool& isStackPop) = 0;

			//----------------------------------------------------------------------------------
			//[RewindScene]
			//�V�[�����I�������V�[���Ȃ� & isPop�Ŗ{�V�[���ɖ߂��Ă����ꍇ��
			//�����ŌĂяo�����R�[���o�b�N�֐�
			//����1: ���V�[������̈����p�����
			inline virtual void RewindScene(SceneBaton& nextBaton) {}
			
			//----------------------------------------------------------------------------------
			//[Update]
			//����X�V�������ŌĂяo�����R�[���o�b�N�֐�
			//���̃R�[���o�b�N�����ׂČĂяo������Ăяo������A�I�u�W�F�N�g�̍X�V�����s��
			inline virtual void Update() {}
			//----------------------------------------------------------------------------------
			//[Sync]
			//�����X�V�������ŌĂяo�����R�[���o�b�N�֐�
			//Update��ɃV���O���X���b�h�ŌĂяo����A�ϐ��̋��L�⓯�����s��
			//return: �V�[�����s -> true
			virtual bool Sync() = 0;

			//----------------------------------------------------------------------------------
			//[LateUpdate]
			//Update & Sync�I����̕���X�V�������ŌĂяo�����R�[���o�b�N�֐�
			//���̃R�[���o�b�N�Ɠ����ɕ���Ăяo������A�I�u�W�F�N�g�̍X�V�����s��
			inline virtual void LateUpdate() {}

			//----------------------------------------------------------------------------------
			//[FixedUpdate]
			//Fixed�t���[���ł̕���X�V�������ŌĂяo�����R�[���o�b�N�֐�
			//���̃R�[���o�b�N�Ɠ����ɕ���Ăяo������A�I�u�W�F�N�g�̍X�V�����s��
			inline virtual void FixedUpdate() {}			

			//----------------------------------------------------------------------------------
			//[MakeDrawCommand]
			//�`��X�V�������ŌĂяo�����R�[���o�b�N�֐�
			//�`��o�^�����s��
			inline virtual void MakeDrawCommand() {}

			//----------------------------------------------------------------------------------
			//[FindSubScene]
			//SubScene����������
			//return: �������subScene��Weak�|�C���^, ������񂩂�����null
			//����1: SubScene ID(�V�[���ԍ� << 16 | �T�u�V�[���ԍ�)
			inline WeakPointer<SubScene> FindSubScene(uint subSceneID);
			//----------------------------------------------------------------------------------
			//[FindSubScene]
			//SubScene����������
			//return: �������subScene��Weak�|�C���^, ������񂩂�����null
			//����1: SubScene Name
			inline WeakPointer<SubScene> FindSubScene(const sgstring& sceneName);
			//----------------------------------------------------------------------------------
			//[SubSceneBegin]
			//return: SubScene->Begin(��X���b�h�Z�[�t)
			inline auto SubSceneBegin();
			//----------------------------------------------------------------------------------
			//[SubSceneEnd]
			//return: SubScene->End(��X���b�h�Z�[�t)
			inline auto SubSceneEnd();

			//scene name (read function property)
			SGF_FUNCTION_PROPERTY virtual sgstring getSceneName() const = 0;
			
			GetOnlyProperty<uint> sceneID;
			GetOnlyProperty<ushort> numSubScenes;

		protected:
			//----------------------------------------------------------------------------------
			//[�R���X�g���N�^]
			//�J�E���^�[������������
			BaseScene() : m_subIDCounter(0), sceneID(m_sceneID), numSubScenes(m_subIDCounter) {}
			
			//----------------------------------------------------------------------------------
			//[OpenSubScene]
			//Sub�V�[�����J��
			//return: �쐬�����T�u�V�[���̃V�F�A�[�h�|�C���^(�V�[���O�Ɏ����o������)
			//����1: Sub�V�[���ւ̃o�g���|�C���^, nullptr����\
			//����2: �쐬�����Sub�V�[����ID����������, nullptr����\
			template<class SceneClass>
			inline SharedPointer<SubScene> OpenSubScene(SceneBaton* baton, uint* id);
			//----------------------------------------------------------------------------------
			//[CloseSubScene]
			//Sub�V�[�������
			//����1: Sub�V�[����ID
			inline void CloseSubScene(uint id);
			//----------------------------------------------------------------------------------
			//[CloseSubSceneAll]
			//Sub�V�[����S�ĕ���
			inline void CloseSubSceneAll();

	private:
			//----------------------------------------------------------------------------------
			//[UpdateSubScene]
			//SubScene�̕���X�V���s��
			inline void UpdateSubScene();
			//----------------------------------------------------------------------------------
			//[SyncSubScene]
			//SubScene�̓����X�V���s��
			inline void SyncSubScene();
			//----------------------------------------------------------------------------------
			//[LateUpdateSubScene]
			//SubScene�̕���X�V���s��
			inline void LateUpdateSubScene();
			//----------------------------------------------------------------------------------
			//[SubSceneFixedParallelUpdate]
			//SubScene��Fixed����X�V���s��
			inline void FixedUpdateSubScene();
			//----------------------------------------------------------------------------------
			//[MakeDrawCommand]
			//SubScene�̕���`��o�^���s��
			inline void MakeDrawCommandSubScene();
			
			//SubScene����
			std::unordered_map<uint, SharedPointer<SubScene>> m_subScenes;
			//mutex
			std::mutex m_mutex;
			//Sub�V�[����ID�J�E���^�[
			ushort m_subIDCounter;
			//�V�[���ԍ�, �T�u�V�[���ԍ��̓V�[���ԍ� << 16 | �T�u�V�[���ԍ�
			uint m_sceneID;
		};

		//�t���[�����[�N�ŊǗ�����T�u�V�[���̃x�[�X�ƂȂ�SubBaseScene class
		class SubScene
		{
		public:
			//----------------------------------------------------------------------------------
			//[�f�X�g���N�^]
			//���ɉ������Ȃ�
			inline virtual ~SubScene() {}
			//----------------------------------------------------------------------------------
			//[Start]
			//�V�[���쐬�������ŌĂяo�����R�[���o�b�N�֐�
			//����1: �e�V�[������̈����p�����(nullptr�ȉ\������)
			virtual void StartScene(SceneBaton* parentBaton) = 0;
			//----------------------------------------------------------------------------------
			//[EndScene]
			//�V�[���I���������ŌĂяo�����R�[���o�b�N�֐�, �I���������s��
			virtual void EndScene() {};
			//----------------------------------------------------------------------------------
			//[Update]
			//����X�V�������ŌĂяo�����R�[���o�b�N�֐�
			//���̃R�[���o�b�N�����ׂČĂяo������Ăяo������A�I�u�W�F�N�g�̍X�V�����s��
			inline virtual void Update() {};
			//----------------------------------------------------------------------------------
			//[Sync]
			//�����X�V�������ŌĂяo�����R�[���o�b�N�֐�
			//Update��ɌĂяo����A�ϐ��̋��L�⓯�����s��
			//return: �V�[�����s -> true
			virtual bool Sync() = 0;
			//----------------------------------------------------------------------------------
			//[LateUpdate]
			//Update & Sync�I����̕���X�V�������ŌĂяo�����R�[���o�b�N�֐�
			//���̃R�[���o�b�N�Ɠ����ɕ���Ăяo������A�I�u�W�F�N�g�̍X�V�����s��
			inline virtual void LateUpdate() {}
			//----------------------------------------------------------------------------------
			//[FixedUpdate]
			//Fixed�t���[���ł̕���X�V�������ŌĂяo�����R�[���o�b�N�֐�
			//�I�u�W�F�N�g�̍X�V�o�^, �������o�^�����s��
			inline virtual void FixedUpdate() {}
			//----------------------------------------------------------------------------------
			//[MakeDrawCommand]
			//�`��X�V�������ŌĂяo�����R�[���o�b�N�֐�
			//�`��o�^�����s��
			virtual void MakeDrawCommand() {}

			//set scene isActive property (set only)
			//argument 1: setting value
			inline bool setActive(bool set) { return m_isActive = set; }

			//scene name (read function property)
			SGF_FUNCTION_PROPERTY virtual sgstring getSceneName() const = 0;
			
			GetOnlyProperty<ushort> subID;		//�T�u�V�[��ID
			GetOnlyProperty<uint> sceneID;		//�V�[��ID
			GetOnlyProperty<bool> isActive;		//Active

		protected:
			//----------------------------------------------------------------------------------
			//[�R���X�g���N�^]
			//���������
			//����1: �T�u�V�[��ID
			SubScene(ushort id, uint parentID) : sceneID(m_sceneID), subID(m_subID), isActive(m_isActive), m_sceneID((parentID << 16) | id), m_subID(id), m_isActive(true) {}
		
		private:
			uint m_sceneID;		//SceneID
			ushort m_subID;		//SubID
			bool m_isActive;		//Active
		};

		//----------------------------------------------------------------------------------
		//[FindSubScene]
		//SubScene����������
		//return: �������subScene��Weak�|�C���^, ������񂩂�����null
		//����1: SubScene ID (�V�[���ԍ� << 16 | �T�u�V�[���ԍ�)
		inline WeakPointer<SubScene> BaseScene::FindSubScene(uint subSceneID)
		{
			std::lock_guard<std::mutex> guard(m_mutex);

			auto find = m_subScenes.find(subSceneID);

			if (find != m_subScenes.end())
				return find->second;
			else
				return WeakPointer<SubScene>::getNull();
		}
		//----------------------------------------------------------------------------------
		//[FindSubScene]
		//SubScene����������
		//return: �������subScene��Weak�|�C���^, ������񂩂�����null
		//����1: SubScene Name
		inline WeakPointer<SubScene> BaseScene::FindSubScene(const sgstring & sceneName)
		{
			std::lock_guard<std::mutex> guard(m_mutex);

			for (auto& e : m_subScenes)
			{
				if (e.second->getSceneName() == sceneName)
					return e.second;
			}

			return WeakPointer<SubScene>::getNull();
		}
		//----------------------------------------------------------------------------------
		//[SubSceneBegin]
		//return: SubScene->Begin(��X���b�h�Z�[�t)
		inline auto BaseScene::SubSceneBegin()
		{
			return m_subScenes.begin();
		}
		//----------------------------------------------------------------------------------
		//[SubSceneEnd]
		//return: SubScene->End(��X���b�h�Z�[�t)
		inline auto BaseScene::SubSceneEnd()
		{
			return m_subScenes.end();
		}

		//----------------------------------------------------------------------------------
		//[OpenSubScene]
		//Sub�V�[�����J��
		//return: �쐬�����T�u�V�[���̃V�F�A�[�h�|�C���^(�V�[���O�Ɏ����o������)
		//����1: Sub�V�[���ւ̃o�g���|�C���^, nullptr����\
		//����2: �쐬�����Sub�V�[����ID����������, nullptr����\
		template<class SceneClass>
		inline SharedPointer<SubScene> BaseScene::OpenSubScene(SceneBaton* baton, uint * id)
		{
			static_assert(std::is_base_of<SubScene, SceneClass>, L"InstantiateNextScene template should be the base of SubScene class");

			std::lock_guard<std::mutex> guard(m_mutex);

			auto ptr = Pointer::MakeShared<SceneClass>(m_subIDCounter);
			m_subScenes.try_emplace(m_subIDCounter, ptr);

			if (id != nullptr) *id = m_subIDCounter++;

			ptr->StartScene(baton);
			
			return ptr;
		}
		//----------------------------------------------------------------------------------
		//[CloseSubScene]
		//Sub�V�[�������
		//����1: Sub�V�[����ID
		inline void BaseScene::CloseSubScene(uint id)
		{
			std::lock_guard<std::mutex> guard(m_mutex);
			
			//�V�[��ID
			auto find = m_subScenes.find(id);
			
			//�폜���ďI��
			if (find != m_subScenes.end())
				m_subScenes.erase(id);
		}
		//----------------------------------------------------------------------------------
		//[CloseSubSceneAll]
		//Sub�V�[����S�ĕ���
		inline void BaseScene::CloseSubSceneAll()
		{
			std::lock_guard<std::mutex> guard(m_mutex);
	
			m_subScenes.clear();
		}

		//----------------------------------------------------------------------------------
		//[UpdateSubScene]
		//SubScene�̕���X�V���s��
		inline void BaseScene::UpdateSubScene()
		{
			for (auto& e : m_subScenes)
				e.second->Update();
		}
		//----------------------------------------------------------------------------------
		//[SyncSubScene]
		//SubScene�̓����X�V���s��
		inline void BaseScene::SyncSubScene()
		{
			for (auto it = m_subScenes.begin(); it != m_subScenes.end();)
			{
				bool result = it->second->Sync();
				if (IS_FALSE(result))
				{
					it->second->EndScene();
					m_subScenes.erase(it);
				}
				else
					++it;
			}
		}
		//----------------------------------------------------------------------------------
		//[LateUpdateSubScene]
		//SubScene�̕���X�V���s��
		inline void BaseScene::LateUpdateSubScene()
		{
			for (auto& e : m_subScenes)
				e.second->LateUpdate();
		}
		//----------------------------------------------------------------------------------
		//[FixedUpdateSubScene]
		//SubScene��Fixed����X�V���s��
		inline void BaseScene::FixedUpdateSubScene()
		{
			for (auto& e : m_subScenes)
				e.second->FixedUpdate();
		}
		//----------------------------------------------------------------------------------
		//[MakeDrawCommandSubScene]
		//SubScene�̕���`��X�V���s��
		inline void BaseScene::MakeDrawCommandSubScene()
		{
			for (auto& e : m_subScenes)
				e.second->MakeDrawCommand();
		}
	}
}

#endif // !SGFRAMEWORK_HEADER_SCENE_HPP_
