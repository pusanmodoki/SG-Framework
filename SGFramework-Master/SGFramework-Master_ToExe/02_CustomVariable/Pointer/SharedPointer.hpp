/*----------------------------------------------------------------------------------
�Q�ƃJ�E���^�������J�E���^ == 0�Ŏ����I��delete�����SharedPointer class
��Q�ƃJ�E���^������WeakPointer class
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_SHARED_POINTR_HPP_
#define SGFRAMEWORK_HEADER_SHARED_POINTR_HPP_
#include "GarbageCollector.hpp"

//Framework namespace
namespace SGFramework
{
	//�Q�ƃJ�E���^�������J�E���^ == 0�Ŏ����I��delete�����SharedPointer class
	template<class PointerType>
	class SharedPointer;
	//��Q�ƃJ�E���^������WeakPointer class
	template<class PointerType>
	class WeakPointer;

	//�Q�ƃJ�E���^�������J�E���^ == 0�Ŏ����I��delete�����SharedPointer class
	template<class PointerType>
	class SharedPointer final : public Detail::Pointer::BaseCountPointer
	{
	public:
		//friend
		friend class WeakPointer<PointerType>;
		template<class NewClass> friend class SharedPointer;
		template<class NewClass> friend class WeakPointer;

		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//nullptr���������s��
		inline SharedPointer() noexcept;
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//�Q�ƃR�s�[���s��
		//����1: �R�s�[��
		inline SharedPointer(const SharedPointer<PointerType>& copy) noexcept;
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//���g�ւ�Move���s��
		//����1: Move��
		inline SharedPointer(SharedPointer<PointerType>&& move) noexcept;
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//���������s��
		//����1: new�����|�C���^�[
		//����2: Used mutex counter (Required for Multi thread), default = true
		//����3: Auto gabage collection?, default = true
		template<class NewType>
		inline explicit SharedPointer(NewType* newPointer, 
			bool isUsedMutexCounter = true, bool isAutoGabageCollection = true) noexcept;
		//----------------------------------------------------------------------------------
		//[�f�X�g���N�^]
		//�����o�ϐ��̃f�X�g���N�^�ɂ��J�E���^�̌v�Z��J�����s��
		inline ~SharedPointer() noexcept { ReleasePointer(); }

		//----------------------------------------------------------------------------------
		//[AddRef]
		//�Q�ƃJ�E���^�𑝂₷, memcpy�������p�����A�\�Ȍ���g���ׂ��炸
		inline void AddRef() const noexcept { if (m_counter) m_counter->addShared(); }
		//----------------------------------------------------------------------------------
		//[WaiverAndReset]
		//�|�C���^�̏��L������������g��nullptr�ɏ���������
		inline void WaiverAndReset() noexcept { ReleasePointer(); }
		//----------------------------------------------------------------------------------
		//[DynamicCast]
		//return: DynamicCast����SharedPointer
		template<class DifferentType>
		inline SharedPointer<DifferentType> DynamicCast() const noexcept;
		//----------------------------------------------------------------------------------
		//[DownCast]
		//return: DownCast����SharedPointer
		template<class DifferentType>
		inline SharedPointer<DifferentType> DownCast() const noexcept;
		
		//this raw pointer, not delete!! (get function property)
		SGF_FUNCTION_PROPERTY PointerType* getPointer() const noexcept { return m_pointer; }
		//shared count (get function property)
		SGF_FUNCTION_PROPERTY ushort getSharedCount() const noexcept { return m_counter ? m_counter->getShared() : 0; }
		//weak count (get function property)
		SGF_FUNCTION_PROPERTY ushort getWeakCount() const noexcept { return m_counter ? m_counter->getWeak() : 0; }
		//pointer is valid(�L��)? (get function property)
		SGF_FUNCTION_PROPERTY bool getIsValid() const noexcept { return m_pointer ? true : false; }

		//null pointer<static>(get function property)
		SGF_FUNCTION_PROPERTY static SharedPointer<PointerType> getNull() noexcept { return std::move(SharedPointer<PointerType>()); }
		//set null pointer (set function property)
		//return : this pointer (null)
		SGF_FUNCTION_PROPERTY const SharedPointer<PointerType>& setNull() noexcept { return *this = SharedPointer<PointerType>(); }


		//= operator, move
		inline SharedPointer<PointerType>& operator=(SharedPointer<PointerType>&& move) noexcept;
		//= operator, addCount
		inline const SharedPointer<PointerType>& operator=(const SharedPointer<PointerType>& copy) noexcept;

		//* operator get pointer
		inline PointerType& operator * () const noexcept { return *m_pointer; }
		//-> operator get pointer dont delete
		inline PointerType* operator -> () const noexcept { return m_pointer; }
		//bool operator is pointer != nullptr
		inline operator bool() const noexcept { return m_pointer != nullptr; }
		//! operator is pointer == nullptr
		inline bool operator ! () const noexcept { return m_pointer == nullptr; }

		//== operator
		inline bool operator == (const SharedPointer<PointerType>& pointer) { return m_pointer == pointer.m_pointer; }
		//== operator
		template<class T>
		inline bool operator == (const SharedPointer<T>& pointer) { return m_pointer == pointer.m_pointer; }
		//!= operator
		inline bool operator != (const SharedPointer<PointerType>& pointer) { return m_pointer != pointer.m_pointer; }
		//!= operator
		template<class T>
		inline bool operator != (const SharedPointer<T>& pointer) { return m_pointer != pointer.m_pointer; }


		//== operator
		inline bool operator == (const WeakPointer<PointerType>& pointer);
		//== operator
		template<class T>
		inline bool operator == (const WeakPointer<T>& pointer);
		//!= operator
		inline bool operator != (const WeakPointer<PointerType>& pointer);
		//!= operator
		template<class T>
		inline bool operator != (const WeakPointer<T>& pointer);

	private:
		//Release Pointer (Used Gabage Collector if Clone)
		inline void ReleasePointer() noexcept override;

		//�|�C���^
		PointerType* m_pointer;
	};

	//�Q�ƃJ�E���^�������J�E���^ == 0�Ŏ����I��delete�����SharedPointer class [Array]
	template<class PointerType>
	class SharedPointer<PointerType[]> final : public Detail::Pointer::BaseCountPointer
	{
	public:
		//friend
		friend class WeakPointer<PointerType[]>;
		//friend
		template<class NewType> friend class WeakPointer;
		//friend
		template<class NewType> friend class SharedPointer;

		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//nullptr���������s��
		inline SharedPointer() noexcept;
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//�Q�ƃR�s�[���s��
		//����1: �R�s�[��
		inline SharedPointer(const SharedPointer<PointerType[]>& copy) noexcept;
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//���g�ւ�Move���s��
		//����1: Move��
		inline SharedPointer(SharedPointer<PointerType[]>&& move) noexcept;
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//���������s��
		//����1: new�����|�C���^�[
		//����2: �z��T�C�Y
		//����3: Used mutex counter (Required for Multi thread), default = true
		//����4: Auto gabage collection?, default = true
		template<class NewType>
		inline explicit SharedPointer(NewType* newPointer, ushort arraySize, 
			bool isUsedMutexCounter = true, bool isAutoGabageCollection = true) noexcept;
		//----------------------------------------------------------------------------------
		//[�f�X�g���N�^]
		//�����o�ϐ��̃f�X�g���N�^�ɂ��J�E���^�̌v�Z��J�����s��
		inline ~SharedPointer() noexcept { ReleasePointer(); }

		//----------------------------------------------------------------------------------
		//[AddRef]
		//�Q�ƃJ�E���^�𑝂₷, memcpy�������p�����A�\�Ȍ���g���ׂ��炸
		inline void AddRef() const noexcept { if (m_counter) m_counter->addShared(); }
		//----------------------------------------------------------------------------------
		//[WaiverAndReset]
		//�|�C���^�̏��L������������g��nullptr�ɏ���������
		inline void WaiverAndReset() noexcept { ReleasePointer(); }
		//----------------------------------------------------------------------------------
		//[DynamicCast]
		//return: DynamicCast����SharedPointer
		template<class DifferentType>
		inline SharedPointer<DifferentType[]> DynamicCast() const noexcept;
		//----------------------------------------------------------------------------------
		//[DownCast]
		//return: DownCast����SharedPointer
		template<class DifferentType>
		inline SharedPointer<DifferentType[]> DownCast() const noexcept;
	
		//this raw pointer, not delete!! (get function property)
		SGF_FUNCTION_PROPERTY PointerType* getPointer() const noexcept { return m_pointer; }
		//shared count (get function property)
		SGF_FUNCTION_PROPERTY ushort getSharedCount() const noexcept { return m_counter ? m_counter->getShared() : 0; }
		//weak count (get function property)
		SGF_FUNCTION_PROPERTY ushort getWeakCount() const noexcept { return m_counter ? m_counter->getWeak() : 0; }
		//pointer is valid(�L��)? (get function property)
		SGF_FUNCTION_PROPERTY bool getIsValid() const noexcept { return m_pointer ? true : false; }

		//array size (get function property)
		SGF_FUNCTION_PROPERTY ushort getArraySize() const noexcept { return m_arraySize; }
		//array begin pointer, not delete!!(get function property)
		SGF_FUNCTION_PROPERTY PointerType* begin() { return m_pointer; }
		//array end pointer, not delete!!(get function property)
		SGF_FUNCTION_PROPERTY PointerType* end() { return m_pointer + m_arraySize; }

		//null pointer<static>(get function property)
		SGF_FUNCTION_PROPERTY static SharedPointer<PointerType[]> getNull() noexcept { return std::move(SharedPointer<PointerType[]>()); }
		//set null pointer (set function property)
		//return : this pointer (null)
		SGF_FUNCTION_PROPERTY const SharedPointer<PointerType[]>& setNull() noexcept { return *this = SharedPointer<PointerType[]>(); }

		//= operator, addCount
		inline const SharedPointer<PointerType[]>& operator=(const SharedPointer<PointerType[]>& copy) noexcept;
		//= operator, move
		inline SharedPointer<PointerType[]>& operator=(SharedPointer<PointerType[]>&& move) noexcept;
		//[] operator get pointer dont delete
		inline PointerType& operator [](uint index) const noexcept { return m_pointer[index]; }
		//bool operator is pointer != nullptr
		inline operator bool() const noexcept { return m_pointer != nullptr; }
		//! operator is pointer == nullptr
		inline bool operator ! () const noexcept { return m_pointer == nullptr; }

		//== operator
		inline bool operator == (const SharedPointer<PointerType[]>& pointer) { return m_pointer == pointer.m_pointer; }
		//== operator
		template<class T>
		inline bool operator == (const SharedPointer<T[]>& pointer) { return m_pointer == pointer.m_pointer; }
		//!= operator
		inline bool operator != (const SharedPointer<PointerType[]>& pointer) { return m_pointer != pointer.m_pointer; }
		//!= operator
		template<class T>
		inline bool operator != (const SharedPointer<T[]>& pointer) { return m_pointer != pointer.m_pointer; }

		//== operator
		inline bool operator == (const WeakPointer<PointerType[]>& pointer);
		//== operator
		template<class T>
		inline bool operator == (const WeakPointer<T[]>& pointer);
		//!= operator
		inline bool operator != (const WeakPointer<PointerType[]>& pointer);
		//!= operator
		template<class T>
		inline bool operator != (const WeakPointer<T[]>& pointer);

	private:
		//Release Pointer (Used Gabage Collector if Clone)
		inline void ReleasePointer() noexcept override;

		//�|�C���^
		PointerType* m_pointer;
		//ArraySize
		ushort m_arraySize;
	};


	//��Q�ƃJ�E���^������WeakPointer class
	template<class PointerType>
	class WeakPointer final : public Detail::Pointer::BaseCountPointer
	{
	public:
		//friend
		friend class SharedPointer<PointerType>;
		template<class NewClass> friend class SharedPointer;
		template<class NewClass> friend class WeakPointer;

		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//nullptr���������s��
		inline WeakPointer() noexcept;
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//�Q�ƃR�s�[���s��
		//����1: �R�s�[��
		inline WeakPointer(const WeakPointer<PointerType>& copy) noexcept;
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//���g�ւ�Move���s��
		//����1: Move��
		inline WeakPointer(WeakPointer<PointerType>&& move) noexcept;
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//�Q�ƃR�s�[���s��
		//����1: �R�s�[��<Shared>
		inline WeakPointer(const SharedPointer<PointerType>& copy) noexcept;
		//----------------------------------------------------------------------------------
		//[�f�X�g���N�^]
		//�����o�ϐ��̃f�X�g���N�^�ɂ��J�E���^�̌v�Z��J�����s��
		inline ~WeakPointer() noexcept { ReleasePointer(); }


		//----------------------------------------------------------------------------------
		//[AddRef]
		//�Q�ƃJ�E���^�𑝂₷, memcpy�������p�����A�\�Ȍ���g���ׂ��炸
		inline void AddRef() const noexcept { if (m_counter) m_counter->addWeak(); }
		//----------------------------------------------------------------------------------
		//[Reset]
		//�Q�Ƃ���������g��nullptr�ɏ���������
		inline void Reset() noexcept { ReleasePointer(); }
		//----------------------------------------------------------------------------------
		//[LockShared]
		//�Q�Ƃ��L���ȏꍇ�͎Q�ƃJ�E���g���ꎞ�I��1���₵�I�[�g�폜�@�\�����b�N����
		//�I�[�g�폜�@�\�𖳌������邾���ŕ����X���b�h�ɂ��Q�Ƃ͉������Ȃ�
		//return: ���b�N������
		inline bool LockShared() noexcept;
		//----------------------------------------------------------------------------------
		//[UnlockShared]
		//Lock�����s���Ă����ꍇ�I�[�g�폜�@�\���b�N�������s��
		//return: ���b�N����������
		inline bool UnlockShared() noexcept;
		//----------------------------------------------------------------------------------
		//[DynamicCast]
		//return: DynamicCast����WeakPointer
		template<class DifferentType>
		inline WeakPointer<DifferentType> DynamicCast() const noexcept;
		//----------------------------------------------------------------------------------
		//[DownCast]
		//return: DownCast����WeakPointer
		template<class DifferentType>
		inline WeakPointer<DifferentType> DownCast() const noexcept;
		//----------------------------------------------------------------------------------
		//[TryGetInstance]
		//�C���X�^���X�擾�����݂�
		//return: �C���X�^���X�|�C���^, SharedCount == 0�̏ꍇ��nullptr
		inline PointerType* TryGetInstance() const noexcept { return m_counter && m_counter->getShared() > 0 ? m_pointer : nullptr; }
		//----------------------------------------------------------------------------------
		//[InstantiateShared]
		//return: ��Q�Ƃ���ɍ쐬����SharedPointer(SharedCount == 0�̏ꍇ��nullptr)
		inline SharedPointer<PointerType> InstantiateShared() const noexcept;
		//----------------------------------------------------------------------------------
		//[InstantiateShared]
		//��Q�Ƃ���ɍ쐬����SharedPointer���쐬����(SharedCount == 0�̏ꍇ��nullptr)
		//����1: �쐬����SharedPointer��������|�C���^
		inline void InstantiateShared(SharedPointer<PointerType>& pointer) const noexcept;

		//shared count (get function property)
		SGF_FUNCTION_PROPERTY ushort getSharedCount() const noexcept { return m_counter ? m_counter->getShared() : 0; }
		//weak count (get function property)
		SGF_FUNCTION_PROPERTY ushort getWeakCount() const noexcept { return m_counter ? m_counter->getWeak() : 0; }
		//pointer is valid(�L��)? (get function property)
		SGF_FUNCTION_PROPERTY bool getIsValid() const noexcept { return m_pointer ? true : false; }
		//pointer is revocatoin(����)? (get function property)
		SGF_FUNCTION_PROPERTY bool getIsRevocatoin() const noexcept { return m_counter && m_counter->getShared() == 0 ? true : false; }
		//pointer is locked? (get function property)
		SGF_FUNCTION_PROPERTY bool getIsLocked() const noexcept { return m_isImLocked; }

		//null pointer<static>(get function property)
		SGF_FUNCTION_PROPERTY static WeakPointer<PointerType> getNull() noexcept { return std::move(WeakPointer<PointerType>()); }
		//set null pointer (set function property)
		//return : this pointer (null)
		SGF_FUNCTION_PROPERTY const WeakPointer<PointerType>& setNull() noexcept { return *this = WeakPointer<PointerType>(); }
		
		//* operator get pointer, worning! Plese be sure to execute LockShared or LockSharedGuard(*this)
		inline PointerType& operator * () const noexcept { return *m_pointer; }
		//-> operator get pointer, dont delete and worning! Plese be sure to execute LockShared or LockSharedGuard(*this)
		inline PointerType* operator ->() const noexcept { return m_pointer; }


		//= operator, addCount
		inline const WeakPointer<PointerType>& operator=(const WeakPointer<PointerType>& copy) noexcept;
		//= operator, move
		inline WeakPointer<PointerType>& operator=(WeakPointer<PointerType>&& move) noexcept;

		//= operator, addCount
		inline const SharedPointer<PointerType>& operator=(const SharedPointer<PointerType>& copt) noexcept;

		//bool operator is useCount != 0
		inline operator bool() const noexcept { return m_counter && m_counter->getShared() > 0 ? true : false; }
		//! operator is useCount ==  0
		inline bool operator ! () const noexcept { return m_counter && m_counter->getShared() > 0 ? false : true; }

		//== operator
		inline bool operator == (const SharedPointer<PointerType>& pointer) { return m_pointer == pointer.m_pointer; }
		//== operator
		template<class T>
		inline bool operator == (const SharedPointer<T>& pointer) { return m_pointer == pointer.m_pointer; }
		//== operator
		inline bool operator == (const WeakPointer<PointerType>& pointer) { return m_pointer == pointer.m_pointer; }
		//== operator
		template<class T>
		inline bool operator == (const WeakPointer<T>& pointer) { return m_pointer == pointer.m_pointer; }
		//!= operator
		inline bool operator != (const SharedPointer<PointerType>& pointer) { return m_pointer != pointer.m_pointer; }
		//!= operator
		template<class T>
		inline bool operator != (const SharedPointer<T>& pointer) { return m_pointer != pointer.m_pointer; }
		//!= operator
		inline bool operator != (const WeakPointer<PointerType>& pointer) { return m_pointer != pointer.m_pointer; }
		//!= operator
		template<class T>
		inline bool operator != (const WeakPointer<T>& pointer) { return m_pointer != pointer.m_pointer; }

	private:
		//Release Pointer (Used Gabage Collector if Clone)
		inline void ReleasePointer() noexcept override;

		//�|�C���^
		PointerType* m_pointer;
		//Lock�s������
		bool m_isImLocked;
	};



	//��Q�ƃJ�E���^������WeakPointer class
	template<class PointerType>
	class WeakPointer<PointerType[]> final : public Detail::Pointer::BaseCountPointer
	{
	public:
		//friend
		friend class SharedPointer<PointerType[]>;
		//friend
		template<class NewType> friend class SharedPointer;
		//friend
		template<class NewType> friend class WeakPointer;

		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//nullptr���������s��
		inline WeakPointer() noexcept;
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//�Q�ƃR�s�[���s��
		//����1: �R�s�[��
		inline WeakPointer(const WeakPointer<PointerType[]>& copy) noexcept;
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//���g�ւ�Move���s��
		//����1: Move��
		inline WeakPointer(WeakPointer<PointerType[]>&& move) noexcept;
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//�Q�ƃR�s�[���s��
		//����1: �R�s�[��<Shared>
		inline WeakPointer(const SharedPointer<PointerType[]>& copy) noexcept;
		//----------------------------------------------------------------------------------
		//[�f�X�g���N�^]
		//�����o�ϐ��̃f�X�g���N�^�ɂ��J�E���^�̌v�Z��J�����s��
		inline ~WeakPointer() noexcept { ReleasePointer(); }


		//----------------------------------------------------------------------------------
		//[AddRef]
		//�Q�ƃJ�E���^�𑝂₷, memcpy�������p�����A�\�Ȍ���g���ׂ��炸
		inline void AddRef() const noexcept { if (m_counter) m_counter->addWeak(); }
		//----------------------------------------------------------------------------------
		//[Reset]
		//�Q�Ƃ���������g��nullptr�ɏ���������
		inline void Reset() noexcept { ReleasePointer(); }
		//----------------------------------------------------------------------------------
		//[LockShared]
		//�Q�Ƃ��L���ȏꍇ�͎Q�ƃJ�E���g���ꎞ�I��1���₵�I�[�g�폜�@�\�����b�N����
		//�I�[�g�폜�@�\�𖳌������邾���ŕ����X���b�h�ɂ��Q�Ƃ͉������Ȃ�
		//return: ���b�N������
		inline bool LockShared() noexcept;
		//----------------------------------------------------------------------------------
		//[UnlockShared]
		//Lock�����s���Ă����ꍇ�I�[�g�폜�@�\���b�N�������s��
		//return: ���b�N����������
		inline bool UnlockShared() noexcept;
		//----------------------------------------------------------------------------------
		//[DynamicCast]
		//return: DynamicCast����WeakPointer
		template<class DifferentType>
		inline WeakPointer<DifferentType[]> DynamicCast() const noexcept;
		//----------------------------------------------------------------------------------
		//[DownCast]
		//return: DownCast����WeakPointer
		template<class DifferentType>
		inline WeakPointer<DifferentType[]> DownCast() const noexcept;
		//----------------------------------------------------------------------------------
		//[TryGetInstance]
		//�C���X�^���X�擾�����݂�
		//return: �C���X�^���X�|�C���^, SharedCount == 0�̏ꍇ��nullptr
		inline PointerType* TryGetInstance() const noexcept { return m_counter && m_counter->getShared() > 0 ? m_pointer : nullptr; }
		//----------------------------------------------------------------------------------
		//[InstantiateShared]
		//return: ��Q�Ƃ���ɍ쐬����SharedPointer(SharedCount == 0�̏ꍇ��nullptr)
		inline SharedPointer<PointerType[]> InstantiateShared() const noexcept;
		//----------------------------------------------------------------------------------
		//[InstantiateShared]
		//��Q�Ƃ���ɍ쐬����SharedPointer���쐬����(SharedCount == 0�̏ꍇ��nullptr)
		//����1: �쐬����SharedPointer��������|�C���^
		inline void InstantiateShared(SharedPointer<PointerType[]>& pointer) const noexcept;
		
		//shared count (get function property)
		SGF_FUNCTION_PROPERTY ushort getSharedCount() const noexcept { return m_counter ? m_counter->getShared() : 0; }
		//weak count (get function property)
		SGF_FUNCTION_PROPERTY ushort getWeakCount() const noexcept { return m_counter ? m_counter->getWeak() : 0; }
		//pointer is valid(�L��)? (get function property)
		SGF_FUNCTION_PROPERTY bool getIsValid() const noexcept { return m_pointer ? true : false; }
		//pointer is revocatoin(����)? (get function property)
		SGF_FUNCTION_PROPERTY bool getIsRevocatoin() const noexcept { return m_counter && m_counter->getShared() == 0 ? true : false; }
		//pointer is locked? (get function property)
		SGF_FUNCTION_PROPERTY bool getIsLocked() const noexcept { return m_isImLocked; }

		//array size (get function property)
		SGF_FUNCTION_PROPERTY ushort getArraySize() const noexcept { return m_arraySize; }
		//array begin pointer, not delete!!(get function property)
		SGF_FUNCTION_PROPERTY PointerType* begin() { return m_pointer; }
		//array end pointer, not delete!!(get function property)
		SGF_FUNCTION_PROPERTY PointerType* end() { return m_pointer + m_arraySize; }

		//null pointer<static>(get function property)
		SGF_FUNCTION_PROPERTY static WeakPointer<PointerType[]> getNull() noexcept { return std::move(WeakPointer<PointerType[]>()); }
		//set null pointer (set function property)
		//return : this pointer (null)
		SGF_FUNCTION_PROPERTY const WeakPointer<PointerType[]>& setNull() noexcept { return *this = WeakPointer<PointerType[]>(); }
		

		//[] operator get pointer, worning! Plese be sure to execute LockShared or LockSharedGuard(*this)
		inline PointerType & operator[](uint index) const noexcept { return m_pointer[index]; }

		//= operator, addCount
		inline const WeakPointer<PointerType[]>& operator=(const WeakPointer<PointerType[]>& copy) noexcept;
		//= operator, move
		inline WeakPointer<PointerType[]>& operator=(WeakPointer<PointerType[]>&& move) noexcept;

		//= operator, addCount
		inline const SharedPointer<PointerType[]>& operator=(const SharedPointer<PointerType[]>& copt) noexcept;

		//bool operator is useCount != 0
		inline operator bool() const noexcept { return m_counter && m_counter->getShared() > 0 ? true : false; }
		//! operator is useCount ==  0
		inline bool operator ! () const noexcept { return m_counter && m_counter->getShared() > 0 ? false : true; }

		//== operator
		inline bool operator == (const SharedPointer<PointerType[]>& pointer) { return m_pointer == pointer.m_pointer; }
		//== operator
		template<class T>
		inline bool operator == (const SharedPointer<T[]>& pointer) { return m_pointer == pointer.m_pointer; }
		//== operator
		inline bool operator == (const WeakPointer<PointerType[]>& pointer) { return m_pointer == pointer.m_pointer; }
		//== operator
		template<class T>
		inline bool operator == (const WeakPointer<T[]>& pointer) { return m_pointer == pointer.m_pointer; }
		//!= operator
		inline bool operator != (const SharedPointer<PointerType[]>& pointer) { return m_pointer != pointer.m_pointer; }
		//!= operator
		template<class T>
		inline bool operator != (const SharedPointer<T[]>& pointer) { return m_pointer != pointer.m_pointer; }
		//!= operator
		inline bool operator != (const WeakPointer<PointerType[]>& pointer) { return m_pointer != pointer.m_pointer; }
		//!= operator
		template<class T>
		inline bool operator != (const WeakPointer<T[]>& pointer) { return m_pointer != pointer.m_pointer; }
	
private:
		//���L�������
		inline void ReleasePointer() noexcept override;

		//�|�C���^
		PointerType* m_pointer;
		ushort m_arraySize;		//arraySize
		bool m_isImLocked;	//Lock�s������
	};





	//----------------------------------------------------------------------------------
	//SharedPointer
	//----------------------------------------------------------------------------------



	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//nullptr���������s��
	template<class PointerType>
	inline SharedPointer<PointerType>::SharedPointer() noexcept
		: BaseCountPointer(nullptr), m_pointer(nullptr) {}
	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//�Q�ƃR�s�[���s��
	//����1: �R�s�[��
	template<class PointerType>
	inline SharedPointer<PointerType>::SharedPointer(const SharedPointer<PointerType>& copy) noexcept
		: BaseCountPointer(copy.m_counter), m_pointer(copy.m_pointer)
	{
		if (m_counter != nullptr) m_counter->addShared();
		else m_pointer = nullptr;
	}
	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//���g�ւ�Move���s��
	//����1: Move��
	template<class PointerType>
	inline SharedPointer<PointerType>::SharedPointer(SharedPointer<PointerType>&& move) noexcept
		: BaseCountPointer(move.m_counter), m_pointer(move.m_pointer)
	{
		move.m_pointer = nullptr;
		move.m_counter = nullptr;
	}
	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//���������s��
	//����1: new�����|�C���^�[
	//����2: Used mutex counter (Required for Multi thread), default = true
	//����3: Auto gabage collection?, default = true
	template<class PointerType>
	template<class NewType>
	inline SharedPointer<PointerType>::SharedPointer(NewType* newPointer,
		bool isUsedMutexCounter, bool isAutoGabageCollection) noexcept
		: BaseCountPointer(nullptr), m_pointer(newPointer) 
	{
		if (m_pointer == nullptr) return;

		//new counter
		if (IS_TRUE(isUsedMutexCounter))
			m_counter = new Detail::Pointer::MutexCounter(); 
		else
			m_counter = new Detail::Pointer::SimpleCounter();

		//Register
		GabageCollector::RegisterShared(m_pointer,
			new Detail::Pointer::Deallocator<PointerType, NewType, std::false_type>(),
			m_counter, isAutoGabageCollection);
	}


	//----------------------------------------------------------------------------------
	//[DynamicCast]
	//return: DynamicCast����SharedPointer
	template<class PointerType>
	template<class DifferentType>
	inline SharedPointer<DifferentType> SharedPointer<PointerType>::DynamicCast() const noexcept
	{
		static_assert(std::is_base_of_v<PointerType, DifferentType> | std::is_base_of_v<DifferentType, PointerType>,
			"\nSharedPointer::DynamicCast Failed.\nSuccess Condition : is_base_of<PointerType, DifferentType> or is_convertible<DifferentType, PointerType>");
		
		SharedPointer<DifferentType> result;
		//dynamic cast
		DifferentType* cast = dynamic_cast<DifferentType*>(m_pointer);

		//not nullptr = success
		if (cast != nullptr)
		{
			result.m_pointer = cast;
			result.m_counter = m_counter;
			m_counter->addShared();
		}

		return std::move(result);
	}
	//----------------------------------------------------------------------------------
	//[DowmCast]
	//return: DownCast����SharedPointer
	template<class PointerType>
	template<class DifferentType>
	inline SharedPointer<DifferentType> SharedPointer<PointerType>::DownCast() const noexcept
	{
		static_assert(std::is_base_of_v<DifferentType, PointerType>,
			"\nSharedPointer::DynamicCast Failed.\nSuccess Condition : is_base_of<DifferentType, PointerType>");
		
		SharedPointer<DifferentType> result;
		//static cast
		DifferentType* cast = static_cast<DifferentType*>(m_pointer);

		//not nullptr = success
		if (cast != nullptr)
		{
			result.m_pointer = cast;
			result.m_counter = m_counter;
			m_counter->addShared();
		}

		return std::move(result);
	}

	//= operator, addCount
	template<class PointerType>
	inline const SharedPointer<PointerType>& SharedPointer<PointerType>::operator=(const SharedPointer<PointerType>& copy) noexcept
	{
		//�Ⴄ��������R�s�[
		if (copy.m_pointer != m_pointer)
		{
			//1�x�J��
			ReleasePointer();
			//null�łȂ���΃J�E���^���Z
			if (copy.m_pointer != nullptr) copy.m_counter->addShared();
			//���
			m_pointer = copy.m_pointer;
			m_counter = copy.m_counter;
		}
		return *this;
	}
	//= operator, move
	template<class PointerType>
	inline SharedPointer<PointerType>& SharedPointer<PointerType>::operator=(SharedPointer<PointerType>&& move) noexcept
	{
		//�Ⴄ�������烀�[�u
		if (m_pointer != move.m_pointer)
		{
			//1�x�J��
			ReleasePointer();
			//�ړ�
			m_pointer = move.m_pointer;
			m_counter = move.m_counter;
			move.m_pointer = nullptr;
			move.m_counter = nullptr;
		}
		return *this;
	}

	//== operator
	template<class PointerType>
	inline bool SharedPointer<PointerType>::operator==(const WeakPointer<PointerType>& pointer)
	{
		return m_pointer == pointer.m_pointer;
	}
	//== operator
	template<class PointerType>
	template<class T>
	inline bool SharedPointer<PointerType>::operator==(const WeakPointer<T>& pointer)
	{
		return m_pointer == pointer.m_pointer;
	}
	//!= operator
	template<class PointerType>
	inline bool SharedPointer<PointerType>::operator!=(const WeakPointer<PointerType>& pointer)
	{
		return m_pointer != pointer.m_pointer;
	}
	//!= operator
	template<class PointerType>
	template<class T>
	inline bool SharedPointer<PointerType>::operator!=(const WeakPointer<T>& pointer)
	{
		return m_pointer != pointer.m_pointer;
	}

	//Release Pointer (Used Gabage Collector if Clone)
	template<class PointerType>
	inline void SharedPointer<PointerType>::ReleasePointer() noexcept
	{
		//not null -> �폜�\��
		if (m_pointer != nullptr)
			GabageCollector::DestroyShared(m_pointer, this);

		//nullptr
		m_counter = nullptr;
		m_pointer = nullptr;
	}



	//----------------------------------------------------------------------------------
	//SharedPointer [Array]
	//----------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//nullptr���������s��
	template<class PointerType>
	inline SharedPointer<PointerType[]>::SharedPointer() noexcept
		: BaseCountPointer(nullptr), m_pointer(nullptr), m_arraySize(0) {}
	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//�Q�ƃR�s�[���s��
	//����1: �R�s�[��
	template<class PointerType>
	inline SharedPointer<PointerType[]>::SharedPointer(const SharedPointer<PointerType[]>& copy) noexcept
		: BaseCountPointer(copy.m_counter), m_pointer(copy.m_pointer), m_arraySize(copy.m_arraySize)
	{
		if (m_counter != nullptr) m_counter->addShared();
		else
		{
			m_pointer = nullptr;
			m_arraySize = 0;
		}
	}
	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//���g�ւ�Move���s��
	//����1: Move��
	template<class PointerType>
	inline SharedPointer<PointerType[]>::SharedPointer(SharedPointer<PointerType[]>&& move) noexcept
		: BaseCountPointer(move.m_counter), m_pointer(move.m_pointer), m_arraySize(move.m_arraySize)
	{
		move.m_pointer = nullptr;
		move.m_counter = nullptr;
		move.m_arraySize = 0;
	}
	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//���������s��
	//����1: new�����|�C���^�[
	//����2: �z��T�C�Y
	//����3: Used mutex counter (Required for Multi thread), default = true
	//����4: Auto gabage collection?, default = true
	template<class PointerType>
	template<class NewType>
	inline SharedPointer<PointerType[]>::SharedPointer(NewType* newPointer, ushort arraySize,
		bool isUsedMutexCounter, bool isAutoGabageCollection) noexcept
		: BaseCountPointer(nullptr), m_pointer(newPointer), m_arraySize(0)
	{
		if (m_pointer == nullptr) return;
		m_arraySize = arraySize;

		//new counter
		if (IS_TRUE(isUsedMutexCounter))
			m_counter = new Detail::Pointer::MutexCounter();
		else
			m_counter = new Detail::Pointer::SimpleCounter();
		
		//Register
		GabageCollector::RegisterShared(m_pointer,
			new Detail::Pointer::Deallocator<PointerType, NewType, std::true_type>(),
			m_counter, isAutoGabageCollection);
	}

	//----------------------------------------------------------------------------------
	//[DynamicCast]
	//return: DynamicCast����SharedPointer
	template<class PointerType>
	template<class DifferentType>
	inline SharedPointer<DifferentType[]> SharedPointer<PointerType[]>::DynamicCast() const noexcept
	{
		static_assert(std::is_base_of_v<PointerType, DifferentType> | std::is_base_of_v<DifferentType, PointerType>,
			"\nSharedPointer::DynamicCast Failed.\nSuccess Condition : is_base_of<PointerType, DifferentType> or is_convertible<DifferentType, PointerType>");
			
		SharedPointer<DifferentType[]> result;
		//dynamic cast
		DifferentType* cast = dynamic_cast<DifferentType*>(m_pointer);

		//not nullptr = success
		if (cast != nullptr)
		{
			result.m_pointer = cast;
			result.m_counter = m_counter;
			result.m_arraySize = m_arraySize;
			m_counter->addShared();
		}

		return std::move(result);
	}
	//----------------------------------------------------------------------------------
	//[DownCast]
	//return: DownCast����SharedPointer
	template<class PointerType>
	template<class DifferentType>
	inline SharedPointer<DifferentType[]> SharedPointer<PointerType[]>::DownCast() const noexcept
	{
		static_assert(std::is_base_of_v<DifferentType, PointerType>,
			"\nSharedPointer::DynamicCast Failed.\nSuccess Condition : is_base_of<DifferentType, PointerType>");
		
		SharedPointer<DifferentType[]> result;
		//static cast
		DifferentType* cast = static_cast<DifferentType*>(m_pointer);

		//not nullptr = success
		if (cast != nullptr)
		{
			result.m_pointer = cast;
			result.m_counter = m_counter;
			result.m_arraySize = m_arraySize;
			m_counter->addShared();
		}

		return std::move(result);
	}

	//= operator, addCount
	template<class PointerType>
	inline const SharedPointer<PointerType[]>& SharedPointer<PointerType[]>::operator=(const SharedPointer<PointerType[]>& copy) noexcept
	{
		//�Ⴄ��������R�s�[
		if (copy.m_pointer != m_pointer)
		{
			//1�x�J��
			ReleasePointer();
			//null�łȂ���΃J�E���^���Z
			if (copy.m_counter != nullptr) copy.m_counter->addShared();
			//���
			m_pointer = copy.m_pointer;
			m_counter = copy.m_counter;
			m_arraySize = copy.m_arraySize;
		}
		return *this;
	}
	//= operator, move
	template<class PointerType>
	inline SharedPointer<PointerType[]>& SharedPointer<PointerType[]>::operator=(SharedPointer<PointerType[]>&& move) noexcept
	{
		//�Ⴄ�������烀�[�u
		if (m_pointer != move.m_pointer)
		{
			//1�x�J��
			ReleasePointer();
			//�ړ�
			m_pointer = move.m_pointer;
			m_counter = move.m_counter;
			m_arraySize = move.m_arraySize;
			move.m_pointer = nullptr;
			move.m_counter = nullptr;
			move.m_arraySize = 0;
		}
		return *this;
	}

	//== operator
	template<class PointerType>
	inline bool SharedPointer<PointerType[]>::operator==(const WeakPointer<PointerType[]>& pointer)
	{
		return m_pointer == pointer.m_pointer;
	}
	//== operator
	template<class PointerType>
	template<class T>
	inline bool SharedPointer<PointerType[]>::operator==(const WeakPointer<T[]>& pointer)
	{
		return m_pointer == pointer.m_pointer;
	}
	//!= operator
	template<class PointerType>
	inline bool SharedPointer<PointerType[]>::operator!=(const WeakPointer<PointerType[]>& pointer)
	{
		return m_pointer != pointer.m_pointer;
	}
	//!= operator
	template<class PointerType>
	template<class T>
	inline bool SharedPointer<PointerType[]>::operator!=(const WeakPointer<T[]>& pointer)
	{
		return m_pointer != pointer.m_pointer;
	}

	//�|�C���^�̊J�������݂�
	template<class PointerType>
	inline void SharedPointer<PointerType[]>::ReleasePointer() noexcept
	{
		//not null -> �폜�\��
		if (m_pointer != nullptr)
			GabageCollector::DestroyShared(m_pointer, this);

		//nullptr
		m_counter = nullptr;
		m_pointer = nullptr;
		m_arraySize = 0;
	}



	//----------------------------------------------------------------------------------
	//WeakPointer
	//----------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//nullptr���������s��
	template<class PointerType>
	inline WeakPointer<PointerType>::WeakPointer() noexcept
		: BaseCountPointer(nullptr), m_pointer(nullptr), m_isImLocked(false) {}
	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//�Q�ƃR�s�[���s��
	//����1: �R�s�[��
	template<class PointerType>
	inline WeakPointer<PointerType>::WeakPointer(const WeakPointer<PointerType>& copy) noexcept
		: BaseCountPointer(copy.m_counter), m_pointer(copy.m_pointer), m_isImLocked(false)
	{
		if (m_counter != nullptr)
			m_counter->addWeak();
		else
			m_pointer = nullptr;
	}
	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//���g�ւ�Move���s��
	//����1: Move��
	template<class PointerType>
	inline WeakPointer<PointerType>::WeakPointer(WeakPointer<PointerType>&& move) noexcept
		: BaseCountPointer(move.m_counter), m_pointer(move.m_pointer), m_isImLocked(move.m_isImLocked)
	{
		move.m_pointer = nullptr;
		move.m_counter = nullptr;
		move.m_isImLocked = false;
	}
	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//�Q�ƃR�s�[���s��
	//����1: �R�s�[��<Shared>
	template<class PointerType>
	inline WeakPointer<PointerType>::WeakPointer(const SharedPointer<PointerType>& copy) noexcept
		: BaseCountPointer(copy.m_counter), m_pointer(copy.m_pointer), m_isImLocked(false)
	{
		if (m_counter != nullptr)
			m_counter->addWeak();
		else
			m_pointer = nullptr;
	}

	//----------------------------------------------------------------------------------
	//[LockShared]
	//�Q�Ƃ��L���ȏꍇ�͎Q�ƃJ�E���g���ꎞ�I��1���₵�I�[�g�폜�@�\�����b�N����
	//�I�[�g�폜�@�\�𖳌������邾���ŕ����X���b�h�ɂ��Q�Ƃ͉������Ȃ�
	template<class PointerType>
	inline bool WeakPointer<PointerType>::LockShared() noexcept
	{
		//���b�N���ĂȂ� & ���̂���->�J�E���^++������return true
		if (IS_FALSE(m_isImLocked) && m_counter)
			return m_isImLocked = m_counter->LockShared();
		
		return false;
	}
	//----------------------------------------------------------------------------------
	//[UnlockShared]
	//Lock�����s���Ă����ꍇ�I�[�g�폜�@�\���b�N�������s��
	//return: ���b�N����������
	template<class PointerType>
	inline bool WeakPointer<PointerType>::UnlockShared() noexcept
	{
		//���b�N���Ă�
		if (IS_TRUE(m_isImLocked))
		{
			//Destroy
			GabageCollector::DestroyShared(m_pointer, this);
			//return true
			return m_isImLocked = false;
		}

		return false;
	}
	//----------------------------------------------------------------------------------
	//[DynamicCast]
	//return: DynamicCast����WeakPointer
	template<class PointerType>
	template<class DifferentType>
	inline WeakPointer<DifferentType> WeakPointer<PointerType>::DynamicCast() const noexcept
	{
		static_assert(std::is_base_of_v<PointerType, DifferentType> | std::is_base_of_v<DifferentType, PointerType>,
			"\nSharedPointer::DynamicCast Failed.\nSuccess Condition : is_base_of<PointerType, DifferentType> or is_convertible<DifferentType, PointerType>");
		
		WeakPointer<DifferentType> result;
		//dynamic cast
		DifferentType* cast = dynamic_cast<DifferentType*>(TryGetInstance());

		//not nullptr = success
		if (cast != nullptr)
		{
			result.m_pointer = cast;
			result.m_counter = m_counter;
			result.m_isImLocked = m_isImLocked;
			m_counter->addWeak();
		}

		return std::move(result);
	}
	//----------------------------------------------------------------------------------
	//[DownCast]
	//return: DownCast����WeakPointer
	template<class PointerType>
	template<class DifferentType>
	inline WeakPointer<DifferentType> WeakPointer<PointerType>::DownCast() const noexcept
	{
		static_assert(std::is_base_of_v<DifferentType, PointerType>,
			"\nSharedPointer::DynamicCast Failed.\nSuccess Condition : is_base_of<DifferentType, PointerType>");

		WeakPointer<DifferentType> result;
		//down cast
		DifferentType* cast = static_cast<DifferentType*>(TryGetInstance());

		//not nullptr = success
		if (cast != nullptr)
		{
			result.m_pointer = cast;
			result.m_counter = m_counter;
			result.m_isImLocked = m_isImLocked;
			m_counter->addWeak();
		}

		return std::move(result);
	}
	//----------------------------------------------------------------------------------
	//[InstantiateShared]
	//��Q�Ƃ���ɍ쐬����SharedPointer���쐬����(SharedCount == 0�̏ꍇ��nullptr)
	//����1: �쐬����SharedPointer��������
	template<class PointerType>
	inline SharedPointer<PointerType> WeakPointer<PointerType>::InstantiateShared() const noexcept
	{
		SharedPointer<PointerType> result;

		//�C���X�^���X������
		if (m_counter && IS_TRUE(m_counter->LockShared()))
		{
			result.m_pointer = m_pointer;
			result.m_counter = m_counter;
		}

		return std::move(result);
	}
	//----------------------------------------------------------------------------------
	//[InstantiateShared]
	//��Q�Ƃ���ɍ쐬����SharedPointer���쐬����(SharedCount == 0�̏ꍇ��nullptr)
	//����1: �쐬����SharedPointer��������|�C���^
	template<class PointerType>
	inline void WeakPointer<PointerType>::InstantiateShared(SharedPointer<PointerType>& pointer) const noexcept
	{
		//�J��
		pointer.ReleasePointer();

		//�C���X�^���X������
		if (m_counter && IS_TRUE(m_counter->LockShared()))
		{
			pointer.m_pointer = m_pointer;
			pointer.m_counter = m_counter;
		}
	}

	//= operator, addCount
	template<class PointerType>
	inline const WeakPointer<PointerType>& WeakPointer<PointerType>::operator=(const WeakPointer<PointerType>& copy) noexcept
	{
		//�������R�s�[
		if (copy.m_pointer != m_pointer)
		{
			//��x�J��
			ReleasePointer();
			//null�łȂ���΃J�E���^���Z
			if (copy.m_counter != nullptr) copy.m_counter->addWeak();

			//copy
			m_pointer = copy.m_pointer;
			m_counter = copy.m_counter;
		}
		return *this;
	}
	//= operator, move
	template<class PointerType>
	inline WeakPointer<PointerType>& WeakPointer<PointerType>::operator=(WeakPointer<PointerType>&& move) noexcept
	{
		//������烀�[�u
		if (m_counter != move.m_counter)
		{
			//��x�J��
			ReleasePointer();
			//move
			m_pointer = move.m_pointer;
			m_counter = move.m_counter;
			m_isImLocked = move.m_isImLocked;
			move.m_pointer = nullptr;
			move.m_counter = nullptr;
			move.m_isImLocked = false;
		}
		return *this;
	}

	//= operator, addCount
	template<class PointerType>
	inline const SharedPointer<PointerType>& WeakPointer<PointerType>::operator=(const SharedPointer<PointerType>& copy) noexcept
	{
		//�l�������R�s�[
		if (m_counter != copy.m_counter)
		{
			//��x�J��
			ReleasePointer();
			//null�łȂ���΃J�E���^���Z
			if (copy.m_counter != nullptr)
				copy.m_counter->addWeak();

			//copy
			m_pointer = copy.m_pointer;
			m_counter = copy.m_counter;
		}
		return copy;
	}

	//Release Pointer (Used Gabage Collector if Clone)
	template<class PointerType>
	inline void WeakPointer<PointerType>::ReleasePointer() noexcept
	{
		//���b�N���Ă���J��
		if (IS_TRUE(m_isImLocked)) UnlockShared();
	
		//Destroy
		GabageCollector::DestroyWeak(m_pointer, this);
		
		//nullptr
		m_pointer = nullptr;
		m_counter = nullptr;
		m_isImLocked = false;
	}



	//----------------------------------------------------------------------------------
	//WeakPointer (array)
	//----------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//nullptr���������s��
	template<class PointerType>
	inline WeakPointer<PointerType[]>::WeakPointer() noexcept
		: BaseCountPointer(nullptr), m_pointer(nullptr), m_arraySize(0), m_isImLocked(false) {}
	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//�Q�ƃR�s�[���s��
	//����1: �R�s�[��
	template<class PointerType>
	inline WeakPointer<PointerType[]>::WeakPointer(const WeakPointer<PointerType[]>& copy) noexcept
		: BaseCountPointer(copy.m_counter), m_pointer(copy.m_pointer),
		m_arraySize(copy.m_arraySize), m_isImLocked(false)
	{
		if (m_counter != nullptr) m_counter->addWeak();
		else
		{
			m_pointer = m_counter = nullptr;
			m_arraySize = 0;
		}
	}
	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//���g�ւ�Move���s��
	//����1: Move��
	template<class PointerType>
	inline WeakPointer<PointerType[]>::WeakPointer(WeakPointer<PointerType[]>&& move) noexcept
		: BaseCountPointer(move.m_counter), m_pointer(move.m_pointer), 
		m_arraySize(move.m_arraySize), m_isImLocked(move.m_isImLocked)
	{
		move.m_pointer = nullptr;
		move.m_counter = nullptr;
		move.m_arraySize = 0;
		move.m_isImLocked = false;
	}
	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//�Q�ƃR�s�[���s��
	//����1: �R�s�[��<Shared>
	template<class PointerType>
	inline WeakPointer<PointerType[]>::WeakPointer(const SharedPointer<PointerType[]>& copy) noexcept
		: BaseCountPointer(copy.m_counter), m_pointer(copy.m_pointer),
		m_arraySize(copy.m_arraySize), m_isImLocked(false)
	{
		if (m_counter != nullptr) m_counter->addWeak();
		else
		{
			m_pointer = nullptr;
			m_arraySize = 0;
		}
	}

	//----------------------------------------------------------------------------------
	//[LockShared]
	//�Q�Ƃ��L���ȏꍇ�͎Q�ƃJ�E���g���ꎞ�I��1���₵�I�[�g�폜�@�\�����b�N����
	//�I�[�g�폜�@�\�𖳌������邾���ŕ����X���b�h�ɂ��Q�Ƃ͉������Ȃ�
	template<class PointerType>
	inline bool WeakPointer<PointerType[]>::LockShared() noexcept
	{
		//���b�N���ĂȂ� & ���̂���->�J�E���^++������return true
		if (IS_FALSE(m_isImLocked))
			if (m_counter)
				return m_isImLocked = m_counter->LockShared();

		return false;
	}
	//----------------------------------------------------------------------------------
	//[UnlockShared]
	//Lock�����s���Ă����ꍇ�I�[�g�폜�@�\���b�N�������s��
	//return: ���b�N����������
	template<class PointerType>
	inline bool WeakPointer<PointerType[]>::UnlockShared() noexcept
	{
		//���b�N���Ă�
		if (IS_TRUE(m_isImLocked))
		{
			//Destroy
			GabageCollector::DestroyShared(m_pointer, this);
			//return true
			return m_isImLocked = false;
		}

		return false;
	}
	//----------------------------------------------------------------------------------
	//[DynamicCast]
	//return: DynamicCast����WeakPointer
	template<class PointerType>
	template<class DifferentType>
	inline WeakPointer<DifferentType[]> WeakPointer<PointerType[]>::DynamicCast() const noexcept
	{
		static_assert(std::is_base_of_v<PointerType, DifferentType> | std::is_base_of_v<DifferentType, PointerType>,
			"\nSharedPointer::DynamicCast Failed.\nSuccess Condition : is_base_of<PointerType, DifferentType> or is_convertible<DifferentType, PointerType>");

		WeakPointer<DifferentType> result;
		//dynamic cast
		DifferentType* cast = dynamic_cast<DifferentType*>(TryGetInstance());

		//not nullptr = success
		if (cast != nullptr)
		{
			result.m_pointer = cast;
			result.m_counter = m_counter;
			result.m_isImLocked = m_isImLocked;
			result.m_arraySize = m_arraySize;
			m_counter->addWeak();
		}

		return std::move(result);
	}
	//----------------------------------------------------------------------------------
	//[DownCast]
	//return: DownCast����WeakPointer
	template<class PointerType>
	template<class DifferentType>
	inline WeakPointer<DifferentType[]> WeakPointer<PointerType[]>::DownCast() const noexcept
	{
		static_assert(std::is_base_of_v<DifferentType, PointerType>,
			"\nSharedPointer::DynamicCast Failed.\nSuccess Condition : is_base_of<DifferentType, PointerType>");

		WeakPointer<DifferentType> result;
		//down cast
		DifferentType* cast = static_cast<DifferentType*>(TryGetInstance());

		//not nullptr = success
		if (cast != nullptr)
		{
			result.m_pointer = cast;
			result.m_counter = m_counter;
			result.m_isImLocked = m_isImLocked;
			result.m_arraySize = m_arraySize;
			m_counter->addWeak();
		}

		return std::move(result);
	}
	//----------------------------------------------------------------------------------
	//[InstantiateShared]
	//��Q�Ƃ���ɍ쐬����SharedPointer���쐬����(SharedCount == 0�̏ꍇ��nullptr)
	//����1: �쐬����SharedPointer��������
	template<class PointerType>
	inline SharedPointer<PointerType[]> WeakPointer<PointerType[]>::InstantiateShared() const noexcept
	{
		SharedPointer<PointerType> result;

		//�C���X�^���X������
		if (m_counter && IS_TRUE(m_counter->LockShared()))
		{
			result.m_pointer = m_pointer;
			result.m_counter = m_counter;
		}

		return std::move(result);
	}
	//----------------------------------------------------------------------------------
	//[InstantiateShared]
	//��Q�Ƃ���ɍ쐬����SharedPointer���쐬����(SharedCount == 0�̏ꍇ��nullptr)
	//����1: �쐬����SharedPointer��������|�C���^
	template<class PointerType>
	inline void WeakPointer<PointerType[]>::InstantiateShared(SharedPointer<PointerType[]>& pointer) const noexcept
	{
		//�J��
		pointer.ReleasePointers();

		//�C���X�^���X������
		if (m_counter && IS_TRUE(m_counter->LockShared()))
		{
			pointer.m_pointer = m_pointer;
			pointer.m_counter = m_counter;
			pointer.m_arraySize = m_arraySize;
		}
	}
	
	//= operator, addCount
	template<class PointerType>
	inline const WeakPointer<PointerType[]>& WeakPointer<PointerType[]>::operator=(const WeakPointer<PointerType[]>& copy) noexcept
	{
		//�������R�s�[
		if (copy.m_pointer != m_pointer)
		{
			//��x�J��
			ReleasePointer();
			//null�łȂ���΃J�E���^���Z
			if (copy.m_counter != nullptr) copy.m_counter->addWeak();

			//copy
			m_pointer = copy.m_pointer;
			m_counter = copy.m_counter;
			m_arraySize = copy.m_arraySize;
		}
		return *this;
	}
	//= operator, move
	template<class PointerType>
	inline WeakPointer<PointerType[]>& WeakPointer<PointerType[]>::operator=(WeakPointer<PointerType[]>&& move) noexcept
	{
		//������烀�[�u
		if (m_counter != move.m_counter)
		{
			//��x�J��
			ReleasePointer();
			//move
			m_pointer = move.m_pointer;
			m_counter = move.m_counter;
			m_arraySize = move.m_arraySize;
			m_isImLocked = move.m_isImLocked;
			move.m_pointer = nullptr;
			move.m_counter = nullptr;
			move.m_arraySize = 0;
			move.m_isImLocked = false;
		}
		return *this;
	}

	//= operator, addCount
	template<class PointerType>
	inline const SharedPointer<PointerType[]>& WeakPointer<PointerType[]>::operator=(const SharedPointer<PointerType[]>& copy) noexcept
	{
		//�l�������R�s�[
		if (m_counter != copy.m_counter)
		{
			//��x�J��
			ReleasePointer();
			//null�łȂ���΃J�E���^���Z
			if (copy.m_counter != nullptr)
				m_counter->addWeak();

			//copy
			m_pointer = copy.m_pointer;
			m_counter = copy.m_counter;
			m_arraySize = copy.m_arraySize;
		}
		return copy;
	}

	//���L�������
	template<class PointerType>
	inline void WeakPointer<PointerType[]>::ReleasePointer() noexcept
	{
		//���b�N���Ă���J��
		if (IS_TRUE(m_isImLocked)) UnlockShared();

		//Destroy
		GabageCollector::DestroyWeak(m_pointer, this);

		//nullptr
		m_pointer = nullptr;
		m_counter = nullptr;
		m_isImLocked = false;
		m_arraySize = 0;
	}
}
#endif // !SGFRAMEWORK_HEADER_SHARED_POINTR_HPP_