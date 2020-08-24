/*----------------------------------------------------------------------------------
ConditionVariable�n�N���X�̃x�[�X�ƂȂ�BaseConditionVariable class
----------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_COM_WEAR_HPP_
#define SGFRAMEWORK_HEADER_COM_WEAR_HPP_
#include "../ConstAndUsing/ConstAndUsing.hpp"
#include "../Atomic/Atomic.hpp"

//Framework namespace
namespace SGFramework
{
	//Base Classes
	namespace BaseClass
	{
		//Pointer��COM��������BaseComWear class
		//template 1: COM��������|�C���^�[�̌^
		template<class WearClass>
		class BaseComWear
		{
		public:
			//----------------------------------------------------------------------------------
			//[AddRef]
			//�J�E���^�����Z������
			//return: ���s��̃J�E���^��, �|�C���^�������������ꍇnull
			inline uint AddRef() { if (m_pointer == nullptr) return 0; return AtomicOperation::Add(m_counter, 1); }

			//<property> com pointer
			SGF_PROPERTY WearClass* _getPointer() const { return m_pointer; }
			//<property> now refecence count
			SGF_PROPERTY WearClass* _getNumReference() const { return m_counter != nullptr ? m_counter->load() : 0; }
			//<property> pointer is valid(�L��)?
			SGF_PROPERTY bool _getIsValid() const { return m_counter != nullptr && *m_counter > 0; }
			
			//pointer operator
			inline operator WearClass* () const { return m_pointer; }
			//pointer operator
			inline WearClass* operator -> () const { return m_pointer; }
			//pointer operator
			inline WearClass* operator () () const { return m_pointer; }
			//pointer operator
			inline WearClass& operator * () const { return *m_pointer; }

		protected:
			//----------------------------------------------------------------------------------
			//[�R���X�g���N�^]
			BaseComWear() : m_pointer(nullptr), m_counter(nullptr) {}

			//----------------------------------------------------------------------------------
			//[InitBaseMembers]
			//�|�C���^, �J�E���^������
			//����1: �|�C���^
			inline void InitBaseMembers(WearClass* pointer)
			{
				m_pointer = pointer;
				m_counter = pointer != nullptr ? new std::atomic<uint>(1) : nullptr;
			}
			//----------------------------------------------------------------------------------
			//[UnInitBaseMembers]
			//�J�E���^�폜
			inline void UnInitBaseMembers() { DELETE_POINTER(m_counter); }

			WearClass* m_pointer;					//�|�C���^
			std::atomic<uint>* m_counter;		//�J�E���^
		};
	}

	//Pointer��COM�������ARelease����delete����ComWearDelete class
	//template 1: COM��������|�C���^�[�̌^
	template<class WearClass>
	class ComWearDelete : public BaseClass::BaseComWear<WearClass>
	{
	public:
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//����1: �|�C���^
		inline ComWearDelete(WearClass* pointer) { this->InitBaseMembers(pointer); }

		//----------------------------------------------------------------------------------
		//[Init]
		//Release���s�㏉�������s��, �����ナ�[�N���N����\������, ���ӂ��ꂽ��
		//����1: �|�C���^
		//����2: now pointer is array?, default = false
		inline void Init(WearClass* pointer, bool isArray = false)
		{
			Release(isArray, true);
			this->InitBaseMembers(pointer);
		}

		//----------------------------------------------------------------------------------
		//[Release]
		//�J�E���^�����Z�����A0�ɂȂ����ꍇdelete�ŊJ�����s��
		//return: ���s��̃J�E���^��, �|�C���^�������������ꍇnull
		//����1: array pointer?, default = false
		//����2: �J�E���^��0�ɂȂ�Ȃ������ꍇ�������I��nullptr�������邩, default = false
		inline uint Release(bool isArray = false, bool isForceInitNull = false)
		{ 
			//null?
			if (this->m_pointer == nullptr) return 0;

			//�J�E���^���Z, 0�Ȃ�delete
			uint result = AtomicOperation::Subtract(*this->m_counter, 1);
			if (result == 0)
			{
				this->UnInitBaseMembers();
				if (IS_FALSE(isArray)) delete this->m_pointer;
				else delete[] this->m_pointer;
				this->m_pointer = nullptr;
			}
			else if (IS_TRUE(isForceInitNull))
			{
				this->m_pointer = nullptr;
				this->m_counter = nullptr;
			}

			return result;
		}
	};

	//Pointer��COM�������ARelease����ReleaseFunction�����s����ComWearFunction class
	//template 1: COM��������|�C���^�[�̌^
	template<class WearClass>
	class ComWearFunction : public BaseClass::BaseComWear<WearClass>
	{
		template<class WearClass>
		struct BaseFunctionCaller;
		template<class WearClass, typename ReleaseFunction>
		struct FunctionCaller;
	public:
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		inline ComWearFunction() : m_function(nullptr) {}
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//template 1: �|�C���^�[���J�������郁���o�֐�
		//����1: �|�C���^
		//����2: �J��������֐��|�C���^
		template<typename ReleaseFunction>
		inline ComWearFunction(WearClass* pointer, ReleaseFunction function)
			: m_function(new FunctionCaller<WearClass, ReleaseFunction>(function)) { this->InitBaseMembers(pointer); }

		//----------------------------------------------------------------------------------
		//[Init]
		//Release���s�㏉�������s��, �����ナ�[�N���N����\������, ���ӂ��ꂽ��
		//template 1: �|�C���^�[���J�������郁���o�֐�
		//����1: �|�C���^
		//����2: �J��������֐��|�C���^
		template<typename ReleaseFunction>
		inline void Init(WearClass* pointer, ReleaseFunction function)
		{
			Release(true);
			m_function = new FunctionCaller<WearClass, ReleaseFunction>(function);
			this->InitBaseMembers(pointer);
		}

		//----------------------------------------------------------------------------------
		//[Release]
		//�J�E���^�����Z�����A0�ɂȂ����ꍇReleaseFunction�ŊJ�����s��
		//return: ���s��̃J�E���^��, �|�C���^�������������ꍇnull
		//����1: �J�E���^��0�ɂȂ�Ȃ������ꍇ�������I��nullptr�������邩, false����
		inline uint Release(bool isForceInitNull)
		{
			if (this->m_pointer == nullptr) return 0;

			//�J�E���^���Z, 0�Ȃ�֐����s
			uint result = AtomicOperation::Subtract(*this->m_counter, 1);
			if (result == 0)
			{
				this->UnInitBaseMembers();
				m_function->Release(this->m_pointer);
				delete m_function;
				m_function = nullptr;
			}
			else if (IS_TRUE(isForceInitNull))
			{
				this->m_pointer = nullptr;
				this->m_counter = nullptr;
				this->m_function = nullptr;
			}

			return result;
		}

	private:
		//�֐��ۑ��p�N���X
		template<class WearClass>
		struct BaseFunctionCaller
		{
			//�R���X�g���N�^
			inline virtual ~BaseFunctionCaller() {}
			//�J��
			virtual void Release(WearClass*& pointer) = 0;
		};
		//�֐��ۑ��p�N���X
		template<class WearClass, typename ReleaseFunction>
		struct FunctionCaller : public BaseFunctionCaller<WearClass>
		{
			//�R���X�g���N�^
			inline FunctionCaller(ReleaseFunction function) : function(function) {}
			//�f�X�g���N�^
			inline ~FunctionCaller() {}

			//�J��
			inline void Release(WearClass*& pointer) override
			{
				(pointer->*function)();
				pointer = nullptr;
			}

			//�֐��|�C���^
			ReleaseFunction function;
		};

		BaseFunctionCaller<WearClass>* m_function;		//�J���p�����o�֐��|�C���^
	};
}

#endif //!SGFRAMEWORK_HEADER_COM_WEAR_HPP_