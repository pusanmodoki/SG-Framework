#ifndef SGFRAMEWORK_HEADER_SCENE_MANAGER_HPP_
#define SGFRAMEWORK_HEADER_SCENE_MANAGER_HPP_
#include <stack>
#include <vector>
#include "../02_CustomVariable/CustomVariable.hpp"
#include "Scene.hpp"

//Framework namespace
namespace SGFramework
{
	class GUI;

	//Scene
	namespace Scene
	{
		//Scene���Ǘ�����SceneManager class
		//���̃N���X�̊֐��͑S�Ĕ�X���b�h�Z�[�t�ł�
		class SceneManager final
		{
		public:
			//friend
			friend class Application;
			friend class GUI;

			//----------------------------------------------------------------------------------
			//[�R���X�g���N�^]
			//�V�[���}�l�[�W���[�̓o�^���s��
			//����1: �ŏ��ɓǂݍ��ރV�[��, InstantiateFirstScene�Ŏw�肷�� 
			//����2: �f�o�b�O���[�h
			inline SceneManager(UniquePointer<BaseScene>&& FirstScene, bool isDebugMode);
			//----------------------------------------------------------------------------------
			//[�f�X�g���N�^]
			//�V�[���J�����s��
			inline ~SceneManager();
			//----------------------------------------------------------------------------------
			//[InstantiateFirstScene]
			//�ŏ��̃V�[���C���X�^���X���쐬����
			template<class SceneClass>
			inline static UniquePointer<BaseScene> InstantiateFirstScene();

			//----------------------------------------------------------------------------------
			//[RegisterUnloadSharedPointer]
			//UnloadPointer��o�^����, �o�^����Pointer�̓V�[���폜����J������Ȃ�
			//template: shared pointer type
			//return: ����������ture, ���s(���ł�key������)�����ꍇfalse
			//����1: pointer
			//����2: key
			template<class PointerType>
			inline static bool RegisterUnloadSharedPointer(SharedPointer<PointerType> pointer, const sgstring& key);

			//----------------------------------------------------------------------------------
			//[FindUnloadSharedPointer]
			//template: �L���X�g����SharedPointer, dynamic_cast�ȂǂŊm�F���s��Ȃ��̂Œ���
			//return: �o�^����UnloadSharedPointer, key�������ȏꍇnull pointer
			//����1: key
			template<class PointerType>
			inline static SharedPointer<PointerType> FindUnloadSharedPointer(const sgstring& key);
			
			//----------------------------------------------------------------------------------
			//[UnregisterUnloadSharedPointer]
			//UnloadSharedPointer��o�^��������
			//return: ���������ꍇtrue, ���s�����ꍇfalse
			//����1: key
			inline static bool UnregisterUnloadSharedPointer(const sgstring& key);

			//now scene (read function property)
			SGF_FUNCTION_PROPERTY static const UniquePointer<BaseScene>& getNowScene()
			{
				return m_scenes.top();
			}
			//unload pointers (read function property)
			SGF_FUNCTION_PROPERTY static uint getNumUnloadPointers()
			{
				return m_unloadCounter.load();
			}

		private:
			//----------------------------------------------------------------------------------
			//[GameStart]
			//�ŏ��̃V�[����Start�������s��
			inline static void GameStart();
			//----------------------------------------------------------------------------------
			//[Update]
			//�V�[���̍X�V���s��
			inline static void Update();
			//----------------------------------------------------------------------------------
			//[Sync]
			//�V�[���̓����X�V���s��
			//return: Scene�̑��s��
			inline static bool Sync();
			//----------------------------------------------------------------------------------
			//[LateUpdate]
			//�V�[���̓����X�V���s��
			inline static void LateUpdate();
			//----------------------------------------------------------------------------------
			//[FixedUpdate]
			//�V�[����Fixed����X�V���s��
			inline static void FixedUpdate();
			//----------------------------------------------------------------------------------
			//[MakeDrawCommand]
			//�V�[���̕`����s��
			inline static void MakeDrawCommand();
			//----------------------------------------------------------------------------------
			//[Release]
			//�V�[���̑S�J�����s��
			inline static void Release();
			//----------------------------------------------------------------------------------
			//[LoadScene]
			//���V�[���̓ǂݍ��݂��s��
			inline static bool LoadScene();

			//Scenes
			static std::stack<UniquePointer<BaseScene>, std::vector<UniquePointer<BaseScene>>> m_scenes;
			//unload pointers
			static std::unordered_map<std::wstring, Detail::Pointer::BaseCountPointer*> m_unloadSharedPointers;
			static std::mutex m_mutex;		//mutex
			//Unload pointer �J�E���g
			static std::atomic<ushort> m_unloadCounter;	
			static bool m_isEndUpdate;			//�X�V�I���t���O
			static bool m_isInstantiate;			//�C���X�^���X���邩�ȃt���O
		};


		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//�V�[���}�l�[�W���[�̓o�^���s��
		//����1: �ŏ��ɓǂݍ��ރV�[��, InstantiateFirstScene�Ŏw�肷�� 
		//����2: �f�o�b�O���[�h
		inline SceneManager::SceneManager(UniquePointer<BaseScene>&& FirstScene, bool isDebugMode)
		{
			//�C���X�^���X�쐬���Ă��Ȃ� && �|�C���^���L��
			if (IS_FALSE(m_isInstantiate) & FirstScene.getIsOwned())
			{
				m_isInstantiate = true;						//���
				IsDebug::m_isValue = isDebugMode;	//�f�o�b�O���[�h

				//�ǉ�
				m_scenes.emplace(std::move(FirstScene));
			}
		}
		//----------------------------------------------------------------------------------
		//[�f�X�g���N�^]
		//�V�[���J�����s��
		inline SceneManager::~SceneManager()
		{
			Release();	//�J��
		}
		//----------------------------------------------------------------------------------
		//[InstantiateFirstScene]
		//�ŏ��̃V�[���C���X�^���X���쐬����
		//throw: ���ɃC���X�^���X������ꍇ
		//return: �쐬�����V�[��
		template<class SceneClass>
		inline UniquePointer<BaseScene> SceneManager::InstantiateFirstScene()
		{
			//BaseScene�����N���X�łȂ��Ȃ�Error
			static_assert(std::is_base_of_v<BaseScene, SceneClass>, L"InstantiateFirstScene template should be the Base of BaseScene class");
			//�C���X�^���X����������
			if (IS_TRUE(m_isInstantiate))
			{
				//ErrorMessage
				MessageBox(Window::HwndHolder::main, L"Not Allow Instantiate", L"Error!! SceneManager->InstantiateFirstScene", MB_OK);
				//throw
				throw InvalidArgument(L"Error!! SceneManager->InstantiateFirstScene", L"Not Allow Instantiate");
			}

			//MakeUnique
			auto result = UniquePointer<SceneClass>(new SceneClass());
			//return
			return std::move(result.MovingDownCast<BaseScene>());
		}

		//----------------------------------------------------------------------------------
		//[RegisterUnloadSharedPointer]
		//UnloadPointer��o�^����, �o�^����Pointer�̓V�[���폜����J������Ȃ�
		//return: ����������ture, ���s(���ł�key������)�����ꍇfalse
		//����1: pointer
		//����2: key
		template<class PointerType>
		inline bool SceneManager::RegisterUnloadSharedPointer(SharedPointer<PointerType> pointer, const sgstring & key)
		{
			std::lock_guard<std::mutex> guard(m_mutex);
			if (key != L"" && m_unloadSharedPointers.find(key) == m_unloadSharedPointers.end())
			{	
				AtomicOperation::Add(m_unloadCounter, 1U);
				Detail::Pointer::BaseCountPointer* newPointer = new SharedPointer<PointerType>(pointer);
				m_unloadSharedPointers.try_emplace(key, newPointer);
				return true;
			}
			else
				return false;
		}

		//----------------------------------------------------------------------------------
		//[FindUnloadSharedPointer]
		//template: �L���X�g����SharedPointer, dynamic_cast�ȂǂŊm�F���s��Ȃ��̂Œ���
		//return: �o�^����UnloadSharedPointer, key�������ȏꍇnull pointer
		//����1: key
		template<class PointerType>
		inline SharedPointer<PointerType> SceneManager::FindUnloadSharedPointer(const sgstring & key)
		{
			std::lock_guard<std::mutex> guard(m_mutex);
			if (m_unloadSharedPointers.find(key) != m_unloadSharedPointers.end())
				return *reinterpret_cast<SharedPointer<PointerType>*>(m_unloadSharedPointers.at(key));
			else
				return SharedPointer<PointerType>::getNull();
		}

		//----------------------------------------------------------------------------------
		//[UnregisterUnloadSharedPointer]
		//UnloadSharedPointer��o�^��������
		//return: ���������ꍇtrue, ���s�����ꍇfalse
		//����1: key
		inline bool SceneManager::UnregisterUnloadSharedPointer(const sgstring & key)
		{
			std::lock_guard<std::mutex> guard(m_mutex);
			if (m_unloadSharedPointers.find(key) != m_unloadSharedPointers.end())
			{
				AtomicOperation::Subtract(m_unloadCounter, 1U);
				delete m_unloadSharedPointers.at(key);
				m_unloadSharedPointers.erase(key);
				return true;
			}
			else
				return false;
		}

		//----------------------------------------------------------------------------------
		//[GameStart]
		//�ŏ��̃V�[����Start�������s��
		inline void SceneManager::GameStart()
		{
			SceneBaton batton;
			m_scenes.top()->StartScene(batton);
		}
		//----------------------------------------------------------------------------------
		//[Update]
		//�V�[���̕���X�V���s��
		inline void SceneManager::Update()
		{
			//if ((m_scenes.empty() ^ true) ^ m_isEndUpdate)
			{
				m_scenes.top()->Update();
				m_scenes.top()->UpdateSubScene();
			}
		}
		//----------------------------------------------------------------------------------
		//[Sync]
		//�V�[���̓����X�V���s��
		inline bool SceneManager::Sync()
		{
			//if ((m_scenes.empty() ^ true) ^ m_isEndUpdate)
			{
				bool result = m_scenes.top()->Sync();
				m_scenes.top()->SyncSubScene();
				if (IS_TRUE(result)) return true;
				else return LoadScene();
			}
			//return false;
		}
		//----------------------------------------------------------------------------------
		//[LateUpdate]
		//�V�[���̓����X�V���s��
		inline void SceneManager::LateUpdate()
		{
			//if ((m_scenes.empty() ^ true) ^ m_isEndUpdate)
			{
				m_scenes.top()->LateUpdate();
				m_scenes.top()->LateUpdateSubScene();
			}
		}
		//----------------------------------------------------------------------------------
		//[FixedUpdate]
		//�V�[����Fixed����X�V���s��
		inline void SceneManager::FixedUpdate()
		{
			//if ((m_scenes.empty() ^ true) ^ m_isEndUpdate)
			{
				m_scenes.top()->FixedUpdate();
				m_scenes.top()->FixedUpdateSubScene();
			}
		}
		//----------------------------------------------------------------------------------
		//[MakeDrawCommand]
		//�V�[���̕`����s��
		inline void SceneManager::MakeDrawCommand()
		{
			//if ((m_scenes.empty() ^ true) ^ m_isEndUpdate)
			{
				m_scenes.top()->MakeDrawCommand();
				m_scenes.top()->MakeDrawCommandSubScene();
			}
		}
		//----------------------------------------------------------------------------------
		//[Release]
		//�V�[���̑S�J�����s��
		inline void SceneManager::Release()
		{
			std::lock_guard<std::mutex> guard(m_mutex);
			
			SceneBaton null;
			uint size = static_cast<uint>(m_scenes.size());
			bool bNull1, bNull2;
			for (uint i = 0; i < size; i++)
			{
				m_scenes.top()->EndScene(null, bNull1, bNull2);
				m_scenes.top().Release();
				m_scenes.pop();
			}

			for (auto& e : m_unloadSharedPointers)
				delete e.second;
			m_unloadSharedPointers.clear();
		}

		//----------------------------------------------------------------------------------
		//[LoadScene]
		//���V�[���̓ǂݍ��݂��s��
		inline bool SceneManager::LoadScene()
		{
			std::lock_guard<std::mutex> guard(m_mutex);
	
			//End�Ŋi�[�������
			SceneBaton baton = {};
			bool isClear = false, isPop = false;
			//End, ���V�[���|�C���^�Q�b�g
			auto ptr = m_scenes.top()->EndScene(baton, isClear, isPop);

			//Pop
			if (isPop)
			{
				m_scenes.top().Release();
				m_scenes.pop();
			}
			//Clear
			if (IS_TRUE(isClear) & IS_FALSE(m_scenes.empty()))
			{
				Release();
			}

			//�|�C���^�L��
			if (IS_TRUE(ptr.getIsOwned()))
			{
				//�V�[���ǉ�
				m_scenes.emplace(std::move(ptr));
				//Start
				ptr->StartScene(baton);
				return true;
			}
			//���V�[���|�C���^����&�܂��V�[��������Pop����->��߂�
			else if (IS_FALSE(m_scenes.empty()) & IS_TRUE(isPop))
			{
				//Rewind
				m_scenes.top()->RewindScene(baton);
				return true;
			}
			//����ȊO�͏I�����݂Ȃ�
			else
			{
				m_isEndUpdate = true;
				return false;
			}
		}
	}
}
#endif // !SGFRAMEWORK_HEADER_SCENE_MANAGER_HPP_