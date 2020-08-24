/*----------------------------------------------------------------------------------
�f�X�g���N�^�Ŏ����I��delete�����UniquePointer class
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_UNIQUE_POINTER_HPP_
#define SGFRAMEWORK_HEADER_UNIQUE_POINTER_HPP_
#include "GarbageCollector.hpp"

//Framework namespace
namespace SGFramework
{
	//�f�X�g���N�^�Ŏ����I��delete�����UniquePointer class
	template<class PointerType>
	class UniquePointer final : public Detail::Pointer::BasePointer
	{
	public:
		//friend
		friend class UniquePointer<PointerType>;
		template<class NewType> friend class UniquePointer;

		//delete
		inline UniquePointer(const UniquePointer<PointerType>& copy) = delete;
		template<class DifferentType>
		inline UniquePointer(const UniquePointer<DifferentType>& copy) = delete;
		inline const UniquePointer<PointerType>& operator=(const UniquePointer<PointerType>& copy) = delete;
		template<class DifferentType>
		inline const UniquePointer<DifferentType>& operator=(const UniquePointer<DifferentType>& copy) = delete;

		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//nullptr���������s��
		inline UniquePointer() noexcept;
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//���g�ւ�Move���s��
		//����1: Move��
		inline UniquePointer(UniquePointer<PointerType>&& move) noexcept;
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//���������s��
		//����1: new�����|�C���^�[
		//����2: Auto gabage collection?, default = true
		template<class NewType>
		inline explicit UniquePointer(NewType* newPointer, bool isAutoGabageCollection = true) noexcept;
		//----------------------------------------------------------------------------------
		//[�f�X�g���N�^]
		//�����o�ϐ��̃f�X�g���N�^�ɂ��J�E���^�̌v�Z��J�����s��
		inline ~UniquePointer() noexcept { ReleasePointer(); }

		//----------------------------------------------------------------------------------
		//[Release]
		//�|�C���^�̊J�������g��nullptr�ɏ���������
		inline void Release() noexcept { ReleasePointer(); }
		//----------------------------------------------------------------------------------
		//[MovingDynamicCast]
		//return: DynamicCast����SharedPointer, ���g��Null�ɂȂ�
		template<class DifferentType>
		inline UniquePointer<DifferentType> MovingDynamicCast() noexcept;
		//----------------------------------------------------------------------------------
		//[MovingDownCast]
		//return: DownCast����UniquePointer, ���g��Null�ɂȂ�
		template<class DifferentType>
		inline UniquePointer<DifferentType> MovingDownCast() noexcept;
		
		//this raw pointer, not delete!! (get function property)
		SGF_FUNCTION_PROPERTY PointerType* getPointer() const noexcept { return m_pointer; }
		//pointer is owned(�L��)? (get function property)
		SGF_FUNCTION_PROPERTY bool getIsOwned() const noexcept { return (m_pointer != nullptr); }

		//null pointer<static>(get function property)
		SGF_FUNCTION_PROPERTY static UniquePointer<PointerType> getNull() noexcept { return std::move(UniquePointer<PointerType>()); }
		//set null pointer (set function property)
		//return : this pointer (null)
		SGF_FUNCTION_PROPERTY const UniquePointer<PointerType>& setNull() noexcept { return *this = UniquePointer<PointerType>(); }
		
		//= operator, move
		inline UniquePointer<PointerType>& operator=(UniquePointer<PointerType>&& move) noexcept;
		
		//* operator get pointer dont delete
		inline PointerType& operator * () const noexcept { return *m_pointer; }
		//-> operator get pointer dont delete
		inline PointerType* operator -> () const noexcept { return m_pointer; }
		//bool operator is pointer != nullptr
		inline operator bool() const noexcept { return m_pointer != nullptr; }
		//! operator is pointer == nullptr
		inline bool operator ! () const noexcept { return m_pointer == nullptr; }

		//== operator
		inline bool operator == (const UniquePointer<PointerType>& pointer) { return m_pointer == pointer.m_pointer; }
		//== operator
		template<class T>
		inline bool operator == (const UniquePointer<T>& pointer) { return m_pointer == pointer.m_pointer; }
		//!= operator
		inline bool operator != (const UniquePointer<PointerType>& pointer) { return m_pointer != pointer.m_pointer; }
		//!= operator
		template<class T>
		inline bool operator != (const UniquePointer<T>& pointer) { return m_pointer != pointer.m_pointer; }

	private:
		//Release Pointer (Used Gabage Collector if Clone)
		inline void ReleasePointer() noexcept;

		//�|�C���^
		PointerType* m_pointer;
	};


	//�f�X�g���N�^�Ŏ����I��delete�����UniquePointer class [Array]
	template<class PointerType>
	class UniquePointer<PointerType[]> final : public Detail::Pointer::BasePointer
	{
	public:
		//friend
		friend class UniquePointer<PointerType[]>;
		//friend
		template<class NewType> friend class UniquePointer;

		//delete
		inline UniquePointer(const UniquePointer<PointerType[]>& copy) = delete;
		//delete
		template<class DifferentType>
		inline UniquePointer(const UniquePointer<DifferentType[]>& copy) = delete;
		//delete
		inline const UniquePointer<PointerType[]>& operator=(const UniquePointer<PointerType[]>& copy) = delete;
		//delete
		template<class DifferentType>
		inline const UniquePointer<DifferentType[]>& operator=(const UniquePointer<DifferentType[]>& copy) = delete;

		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//nullptr���������s��
		inline UniquePointer() noexcept;
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//���g�ւ�Move���s��
		//����1: Move��
		inline UniquePointer(UniquePointer<PointerType[]>&& move) noexcept;
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//���������s��
		//����1: new�����|�C���^�[
		//����2: �z��T�C�Y
		//����2: Auto gabage collection?, default = true
		template<class NewType>
		inline explicit UniquePointer(NewType* pointer, uint arraySize, bool isAutoGabageCollection = true) noexcept;
		//----------------------------------------------------------------------------------
		//[�f�X�g���N�^]
		//�����o�ϐ��̃f�X�g���N�^�ɂ��J�E���^�̌v�Z��J�����s��
		inline ~UniquePointer() noexcept { ReleasePointer(); }

		//----------------------------------------------------------------------------------
		//[Release]
		//�|�C���^�̊J�������g��nullptr�ɏ���������
		inline void Release() noexcept { ReleasePointer(); }
		//----------------------------------------------------------------------------------
		//[MovingDynamicCast]
		//return: DynamicCast����SharedPointer, ���g��Null�ɂȂ�
		template<class DifferentType>
		inline UniquePointer<DifferentType[]> MovingDynamicCast() noexcept;
		//----------------------------------------------------------------------------------
		//[MovingDownCast]
		//return: DownCast����UniquePointer, ���g��Null�ɂȂ�
		template<class DifferentType>
		inline UniquePointer<DifferentType[]> MovingDownCast() noexcept;
		
		//this raw pointer, not delete!! (get function property)
		SGF_FUNCTION_PROPERTY PointerType* getPointer() const noexcept { return m_pointer; }
		//pointer is owned(�L��)? (get function property)
		SGF_FUNCTION_PROPERTY bool getIsOwned() const noexcept { return (m_pointer != nullptr); }

		//array size (get function property)
		SGF_FUNCTION_PROPERTY ushort getArraySize() const noexcept { return m_arraySize; }
		//array begin pointer, not delete!!(get function property)
		SGF_FUNCTION_PROPERTY PointerType* begin() { return m_pointer; }
		//array end pointer, not delete!!(get function property)
		SGF_FUNCTION_PROPERTY PointerType* end() { return m_pointer + m_arraySize; }

		//null pointer<static>(get function property)
		SGF_FUNCTION_PROPERTY static UniquePointer<PointerType[]> getNull() noexcept { return std::move(UniquePointer<PointerType[]>()); }
		//set null pointer (set function property)
		//return : this pointer (null)
		SGF_FUNCTION_PROPERTY const UniquePointer<PointerType[]>& setNull() noexcept { return *this = UniquePointer<PointerType[]>(); }

		//= operator, move
		inline UniquePointer<PointerType[]>& operator=(UniquePointer<PointerType[]>&& move) noexcept;
	

		//[] operator get pointer dont delete
		inline PointerType& operator [](uint index) const noexcept { return m_pointer[index]; }
		//bool operator is pointer != nullptr
		inline operator bool() const noexcept { return m_pointer != nullptr; }
		//! operator is pointer == nullptr
		inline bool operator ! () const noexcept { return m_pointer == nullptr; }

	private:
		//�|�C���^�̊J�������݂�
		inline void ReleasePointer() noexcept;

		//�|�C���^
		PointerType* m_pointer;
		//arraySize
		uint m_arraySize;
	};



	//----------------------------------------------------------------------------------
	//UniquePointer
	//----------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//nullptr���������s��
	template<class PointerType>
	inline UniquePointer<PointerType>::UniquePointer() noexcept
		: m_pointer(nullptr) { }
	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//���g�ւ�Move���s��
	//����1: Move��
	template<class PointerType>
	inline UniquePointer<PointerType>::UniquePointer(UniquePointer<PointerType>&& move) noexcept
	 : m_pointer(move.m_pointer)
	{
		move.m_pointer = nullptr;
	}
	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//���������s��
	//����1: new�����|�C���^�[
	//����2: Auto gabage collection?, default = true
	template<class PointerType>
	template<class NewType>
	inline UniquePointer<PointerType>::UniquePointer(NewType * newPointer, bool isAutoGabageCollection) noexcept
		: m_pointer(newPointer)
	{
		if (m_pointer == nullptr) return;

		//Register
		GabageCollector::RegisterUnique(newPointer,
			new Detail::Pointer::Deallocator<PointerType, NewType, std::false_type>(), isAutoGabageCollection);
	}

	//----------------------------------------------------------------------------------
	//[MovingDynamicCast]
	//return: DynamicCast����SharedPointer, ���g��Null�ɂȂ�
	template<class PointerType>
	template<class DifferentType>
	inline UniquePointer<DifferentType> UniquePointer<PointerType>::MovingDynamicCast() noexcept
	{
		static_assert(std::is_base_of_v<PointerType, DifferentType> | std::is_base_of_v<DifferentType, PointerType>,
			"\nSharedPointer::DynamicCast Failed.\nSuccess Condition : is_base_of<PointerType, DifferentType> or is_convertible<DifferentType, PointerType>");

		UniquePointer<DifferentType> result;
		DifferentType* cast = dynamic_cast<DifferentType*>(m_pointer);

		if (cast != nullptr)
		{
			result.m_pointer = cast;
			this->m_pointer = nullptr;
		}

		return std::move(result);
	}
	//----------------------------------------------------------------------------------
	//[MovingDownCast]
	//return: DownCast����UniquePointer, ���g��Null�ɂȂ�
	template<class PointerType>
	template<class DifferentType>
	inline UniquePointer<DifferentType> UniquePointer<PointerType>::MovingDownCast() noexcept
	{
		static_assert(std::is_base_of_v<DifferentType, PointerType>,
			"\nSharedPointer::DynamicCast Failed.\nSuccess Condition : is_base_of<DifferentType, PointerType>");

		UniquePointer<DifferentType> result;
		DifferentType* cast = static_cast<DifferentType*>(m_pointer);

		if (cast != nullptr)
		{
			result.m_pointer = cast;
			this->m_pointer = nullptr;
		}

		return std::move(result);
	}

	
	//= operator, move
	template<class PointerType>
	inline UniquePointer<PointerType>& UniquePointer<PointerType>::operator=(UniquePointer<PointerType>&& move) noexcept
	{
		//�Ⴄ�������烀�[�u
		if (m_pointer != move.m_pointer)
		{
			//1�x�J��
			ReleasePointer();
			//�ړ�
			m_pointer = move.m_pointer;
			move.m_pointer = nullptr;
		}
		return *this;
	}
	
	//�|�C���^�̊J�������݂�
	template<class PointerType>
	inline void UniquePointer<PointerType>::ReleasePointer() noexcept
	{
		//�|�C���^�[������->Destroy�\��
		if (m_pointer)
			GabageCollector::DestroyUnique(m_pointer, this);

		//nullptr
		m_pointer = nullptr;
	}


	//----------------------------------------------------------------------------------
	//UniquePointer [Array]
	//----------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//nullptr���������s��
	template<class PointerType>
	inline UniquePointer<PointerType[]>::UniquePointer() noexcept
		: m_pointer(nullptr), m_arraySize(0) { }
	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//���g�ւ�Move���s��
	//����1: Move��
	template<class PointerType>
	inline UniquePointer<PointerType[]>::UniquePointer(UniquePointer<PointerType[]>&& move) noexcept
		: m_pointer(move.m_pointer), m_arraySize(move.m_arraySize)
	{
		move.m_pointer = nullptr;
		move.m_arraySize = 0;
	}
	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//���������s��
	//����1: new�����|�C���^�[
	//����2: �z��T�C�Y
	template<class PointerType>
	template<class NewType>
	inline UniquePointer<PointerType[]>::UniquePointer(NewType * pointer, 
		uint arraySize, bool isAutoGabageCollection) noexcept
		: m_pointer(pointer), m_arraySize(0)
	{
		if (m_pointer == nullptr) return;
		m_arraySize = arraySize;

		//Register
		GabageCollector::RegisterUnique(pointer,
			new Detail::Pointer::Deallocator<PointerType, NewType, std::true_type>(), isAutoGabageCollection);
	}
	
	//----------------------------------------------------------------------------------
	//[MovingDynamicCast]
	//return: DynamicCast����SharedPointer, ���g��Null�ɂȂ�
	template<class PointerType>
	template<class DifferentType>
	inline UniquePointer<DifferentType[]> UniquePointer<PointerType[]>::MovingDynamicCast() noexcept
	{
		static_assert(std::is_base_of_v<PointerType, DifferentType> | std::is_base_of_v<DifferentType, PointerType>,
			"\nSharedPointer::DynamicCast Failed.\nSuccess Condition : is_base_of<PointerType, DifferentType> or is_convertible<DifferentType, PointerType>");

		UniquePointer<DifferentType[]> result;
		DifferentType* cast = dynamic_cast<DifferentType*>(m_pointer);

		if (cast != nullptr)
		{
			result.m_pointer = cast;
			result.m_arraySize = m_arraySize;
			this->m_pointer = nullptr;
			this->m_arraySize = 0;
		}

		return std::move(result);
	}
	//----------------------------------------------------------------------------------
	//[MovingDownCast]
	//return: DownCast����UniquePointer, ���g��Null�ɂȂ�
	template<class PointerType>
	template<class DifferentType>
	inline UniquePointer<DifferentType[]> UniquePointer<PointerType[]>::MovingDownCast() noexcept
	{
		static_assert(std::is_base_of_v<DifferentType, PointerType>,
			"\nSharedPointer::DynamicCast Failed.\nSuccess Condition : is_base_of<DifferentType, PointerType>");

		UniquePointer<DifferentType[]> result;
		DifferentType* cast = static_cast<DifferentType*>(m_pointer);

		if (cast != nullptr)
		{
			result.m_pointer = cast;
			result.m_arraySize = m_arraySize;
			this->m_pointer = nullptr;
			this->m_arraySize = 0;
		}

		return std::move(result);
	}
	
	//= operator, move
	template<class PointerType>
	inline UniquePointer<PointerType[]>& UniquePointer<PointerType[]>::operator=(UniquePointer<PointerType[]>&& move) noexcept
	{
		//�Ⴄ�������烀�[�u
		if (m_pointer != move.m_pointer)
		{
			//1�x�J��
			ReleasePointer();
			//�ړ�
			m_pointer = move.m_pointer;
			m_arraySize = move.m_arraySize;
			move.m_pointer = nullptr;
			move.m_arraySize = 0;
		}
		return *this;
	}
	
	//�|�C���^�̊J�������݂�
	template<class PointerType>
	inline void UniquePointer<PointerType[]>::ReleasePointer() noexcept
	{
		//�|�C���^�[������->Destroy�\��
		if (m_pointer)
			GabageCollector::DestroyUnique(m_pointer, this);

		//nullptr
		m_pointer = nullptr;
		m_arraySize = 0;
	}
}
#endif // !SGFRAMEWORK_HEADER_UNIQUE_POINTER_HPP_
