/*----------------------------------------------------------------------------------
//�e�|�C���^���R�s�[���A�Q�ƃJ�E���^�������J�E���^ == 0�Ŏ����I��delete�����CloneSharedPointer class
//��Q�ƃJ�E���^������CloneWeakPointer class
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_CLONE_POINTR_HPP_
#define SGFRAMEWORK_HEADER_CLONE_POINTR_HPP_
#include "GarbageCollector.hpp"
#include "UniquePointer.hpp"
#include "SharedPointer.hpp"
#include "LockWeakGuard.hpp"

//Framework namespace
namespace SGFramework
{
	class GabageCollector;

	//�Q�ƃJ�E���^�������J�E���^ == 0�Ŏ����I��delete�����CloneSharedPointer class
	template<class PointerType>
	class CloneSharedPointer;
	//��Q�ƃJ�E���^������CloneWeakPointer class
	template<class PointerType>
	class CloneWeakPointer;

	//�e�|�C���^���R�s�[���A�Q�ƃJ�E���^�������J�E���^ == 0�Ŏ����I��delete�����CloneSharedPointer class
	template<class PointerType>
	class CloneSharedPointer final : public Detail::Pointer::BaseClonePointer
	{
	public:
		//friend
		friend class CloneWeakPointer<PointerType>;
		template<class NewClass> friend class CloneSharedPointer;
		template<class NewClass> friend class CloneWeakPointer;

		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//nullptr���������s��
		inline CloneSharedPointer() noexcept;
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//�Q�ƃR�s�[���s��
		//����1: �R�s�[��
		inline CloneSharedPointer(const CloneSharedPointer<PointerType>& copy) noexcept;
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//���g�ւ�Move���s��
		//����1: Move��
		inline CloneSharedPointer(CloneSharedPointer<PointerType>&& move) noexcept;
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//���������s��
		//����1: clone source pointer (unused AutoUpdate)
		//����2: Used mutex counter (Required for Multi thread), default = true
		//����3: Auto gabage collection?, default = true
		template<class NewType>
		inline explicit CloneSharedPointer(NewType* cloneSourcePointer,
			bool isUsedMutexCounter = true, bool isAutoGabageCollection = true) noexcept;
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//���������s��
		//����1: clone source pointer (used AutoUpdate)
		//����2: Used mutex counter (Required for Multi thread), default = true
		//����3: Auto gabage collection?, default = true
		template<class NewType>
		inline explicit CloneSharedPointer(const UniquePointer<NewType>& cloneSourcePointer,
			bool isUsedMutexCounter = true, bool isAutoGabageCollection = true) noexcept;
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//���������s��
		//����1: clone source pointer (used AutoUpdate)
		//����2: Used mutex counter (Required for Multi thread), default = true
		//����3: Auto gabage collection?, default = true
		template<class NewType>
		inline explicit CloneSharedPointer(const SharedPointer<NewType>& cloneSourcePointer,
			bool isUsedMutexCounter = true, bool isAutoGabageCollection = true) noexcept;
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//���������s��
		//����1: clone source pointer (used AutoUpdate)
		//����2: Used mutex counter (Required for Multi thread), default = true
		//����3: Auto gabage collection?, default = true
		template<class NewType>
		inline explicit CloneSharedPointer(const WeakPointer<NewType>& cloneSourcePointer,
			bool isUsedMutexCounter = true, bool isAutoGabageCollection = true) noexcept;
		//----------------------------------------------------------------------------------
		//[�f�X�g���N�^]
		//�����o�ϐ��̃f�X�g���N�^�ɂ��J�E���^�̌v�Z��J�����s��
		inline ~CloneSharedPointer() noexcept { ReleasePointer(); }

		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//���������X�V����, ���ӁIDirect->Clone�͎���Update���s���܂���
		//�쐬���̃R�s�[�T�C�Y����ɃR�s�[����̂ŁA�N���X�̌p������ł̓N���b�V�����܂��B���ӁB
		//return: ����������true, ���g or copy��nullptr�Ȃ�false
		//����1: copy value
		inline bool UpdateManual(PointerType* copy) const noexcept
		{
			static_assert(!std::is_convertible<Administrator::GraphicsDetail::ComponentX11, PointerType>
				& !std::is_base_of_v<Administrator::GraphicsDetail::ComponentX11, PointerType>,
				"\nClonePointer->UpdateManual Failed.\nComponent class used Component->CloneUpdate.");
			if (m_counter != nullptr && copy != nullptr)
			{
				auto copySize = GabageCollector::m_cloneShareds.at(m_pointer).copySize;
				
				memcpy_s(m_pointer, copySize, copy, copySize);
				return true;
			}
			else
				return false;
		}
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//���������X�V����, ���ӁIDirect->Clone�͎���Update���s���܂���
		//�쐬���̃R�s�[�T�C�Y����ɃR�s�[����̂ŁA�N���X�̌p������ł̓N���b�V�����܂��B���ӁB
		//return: ����������true, ���g or copy��nullptr�Ȃ�false
		//����1: copy value
		template<class DifferentType>
		inline bool UpdateManual(const DifferentType* copy) const noexcept
		{
			static_assert(std::is_convertible_v<DifferentType, PointerType>,
				"\nClonePointer->Update Failed.\nCondition : is_convertible_v<DifferentType, PointerType>");
			return UpdateManual(static_cast<PointerType*>(copy));
		}
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//���������X�V����, ���ӁIUnique->Clone�͎���Update��Sync���ɍs���Ă��܂�
		//�쐬���̃R�s�[�T�C�Y����ɃR�s�[����̂ŁA�N���X�̌p������ł̓N���b�V�����܂��B���ӁB
		//return: ����������true, ���g or copy��nullptr�Ȃ�false
		//����1: copy value
		inline bool UpdateManual(const UniquePointer<PointerType>& copy) const noexcept { return UpdateManual(copy.getPointer()); }
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//���������X�V����, ���ӁIUnique->Clone�͎���Update��Sync���ɍs���Ă��܂�
		//�쐬���̃R�s�[�T�C�Y����ɃR�s�[����̂ŁA�N���X�̌p������ł̓N���b�V�����܂��B���ӁB
		//return: ����������true, ���g or copy��nullptr�Ȃ�false
		//����1: copy value
		template<class DifferentType>
		inline bool UpdateManual(const UniquePointer<DifferentType>& copy) const noexcept
		{
			static_assert(std::is_convertible_v<DifferentType, PointerType>,
				"\nClonePointer->UpdateManual Failed.\nCondition : is_convertible_v<DifferentType, PointerType>");
			return UpdateManual(static_cast<PointerType*>(copy.getPointer()));
		}
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//���������X�V����, ���ӁIShared->Clone�͎���Update��Sync���ɍs���Ă��܂�
		//dynamic_cast���؂͍s��Ȃ����߁A���S�����m�ۂ��邽�߂�sizeof(PointerType)�����R�s�[���܂���
		//return: ����������true, ���g or copy��nullptr�Ȃ�false
		//����1: copy value
		inline bool UpdateManual(const SharedPointer<PointerType>& copy) const noexcept { return UpdateManual(copy.getPointer()); }
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//���������X�V����, ���ӁIShared->Clone�͎���Update��Sync���ɍs���Ă��܂�
		//�쐬���̃R�s�[�T�C�Y����ɃR�s�[����̂ŁA�N���X�̌p������ł̓N���b�V�����܂��B���ӁB
		//return: ����������true, ���g or copy��nullptr�Ȃ�false
		//����1: copy value
		template<class DifferentType>
		inline bool UpdateManual(const SharedPointer<DifferentType>& copy) const noexcept
		{
			static_assert(std::is_convertible_v<DifferentType, PointerType>,
				"\nClonePointer->UpdateManual Failed.\nCondition : is_convertible_v<DifferentType, PointerType>");
			return UpdateManual(static_cast<PointerType*>(copy.getPointer()));
		}
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//���������X�V����, ���ӁIWeak(Shared)->Clone�͎���Update��Sync���ɍs���Ă��܂�
		//�쐬���̃R�s�[�T�C�Y����ɃR�s�[����̂ŁA�N���X�̌p������ł̓N���b�V�����܂��B���ӁB
		//return: ����������true, ���g or copy��nullptr�Ȃ�false
		//����1: copy value
		inline bool UpdateManual(WeakPointer<PointerType> copy) const noexcept
		{
			LockWeakGuard<WeakPointer<PointerType>> guard(copy);
			if (guard) return UpdateManual(copy.TryGetInstance()); 
			else return false;
		}
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//���������X�V����, ���ӁIWeak(Shared)->Clone�͎���Update��Sync���ɍs���Ă��܂�
		//�쐬���̃R�s�[�T�C�Y����ɃR�s�[����̂ŁA�N���X�̌p������ł̓N���b�V�����܂��B���ӁB
		//return: ����������true, ���g or copy��nullptr�Ȃ�false
		//����1: copy value
		template<class DifferentType>
		inline bool UpdateManual(WeakPointer<DifferentType> copy) const noexcept
		{
			static_assert(std::is_convertible_v<DifferentType, PointerType>,
				"\nClonePointer->UpdateManual Failed.\nCondition : is_convertible_v<DifferentType, PointerType>");
			LockWeakGuard<WeakPointer<DifferentType>> guard(copy);
			if (guard) return UpdateManual(static_cast<PointerType*>(copy.TryGetInstance()));
			else return false;
		}
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//���������X�V����, ���ӁIClone->Clone�͎���Update���s���܂���
		//�쐬���̃R�s�[�T�C�Y����ɃR�s�[����̂ŁA�N���X�̌p������ł̓N���b�V�����܂��B���ӁB
		//return: ����������true, ���g or copy��nullptr�Ȃ�false
		//����1: copy value
		inline bool UpdateManual(const CloneSharedPointer<PointerType>& copy) const noexcept { return UpdateManual(copy.getPointer()); }
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//���������X�V����, ���ӁIClone->Clone�͎���Update���s���܂���
		//�쐬���̃R�s�[�T�C�Y����ɃR�s�[����̂ŁA�N���X�̌p������ł̓N���b�V�����܂��B���ӁB
		//return: ����������true, ���g or copy��nullptr�Ȃ�false
		//����1: copy value
		template<class DifferentType>
		inline bool UpdateManual(const CloneSharedPointer<DifferentType>& copy) const noexcept
		{
			static_assert(std::is_convertible_v<DifferentType, PointerType>,
				"\nClonePointer->Update Failed.\nCondition : is_convertible_v<DifferentType, PointerType>");
			return UpdateManual(static_cast<PointerType*>(copy.getPointer()));
		}
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//���������X�V����, ���ӁIClone->Clone�͎���Update���s���܂���
		//�쐬���̃R�s�[�T�C�Y����ɃR�s�[����̂ŁA�N���X�̌p������ł̓N���b�V�����܂��B���ӁB
		//return: ����������true, ���g or copy��nullptr�Ȃ�false
		//����1: copy value
		inline bool UpdateManual(CloneWeakPointer<PointerType> copy) const noexcept;
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//���������X�V����, ���ӁIClone->Clone�͎���Update���s���܂���
		//�쐬���̃R�s�[�T�C�Y����ɃR�s�[����̂ŁA�N���X�̌p������ł̓N���b�V�����܂��B���ӁB
		//return: ����������true, ���g or copy��nullptr�Ȃ�false
		//����1: copy value
		template<class DifferentType>
		inline bool UpdateManual(CloneWeakPointer<DifferentType> copy) const noexcept;

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
		//return: DynamicCast����CloneSharedPointer
		template<class DifferentType>
		inline CloneSharedPointer<DifferentType> DynamicCast() const noexcept;
		//----------------------------------------------------------------------------------
		//[DownCast]
		//return: DownCast����CloneSharedPointer
		template<class DifferentType>
		inline CloneSharedPointer<DifferentType> DownCast() const noexcept;
		
		//this raw pointer, not delete!! (get function property)
		SGF_FUNCTION_PROPERTY PointerType* getPointer() const noexcept { return m_pointer; }
		//shared count (get function property)
		SGF_FUNCTION_PROPERTY ushort getSharedCount() const noexcept { return m_counter ? m_counter->getShared() : 0; }
		//weak count (get function property)
		SGF_FUNCTION_PROPERTY ushort getWeakCount() const noexcept { return m_counter ? m_counter->getWeak() : 0; }
		//pointer is valid(�L��)? (get function property)
		SGF_FUNCTION_PROPERTY bool getIsValid() const noexcept { return m_pointer ? true : false; }
		//pointer is linked(�����������L��)? (get function property)
		SGF_FUNCTION_PROPERTY bool getIsLinked() const noexcept { return m_counter ? m_counter->getIsLinked() : false; }

		//null pointer<static>(get function property)
		SGF_FUNCTION_PROPERTY static CloneSharedPointer<PointerType> getNull() noexcept { return std::move(CloneSharedPointer<PointerType>()); }
		//set null pointer (set function property)
		//return : this pointer (null)
		SGF_FUNCTION_PROPERTY const CloneSharedPointer<PointerType>& setNull() noexcept { *this = CloneSharedPointer<PointerType>(); }

		//= operator, move
		inline CloneSharedPointer<PointerType>& operator=(CloneSharedPointer<PointerType>&& move) noexcept;
		//= operator, addCount
		inline const CloneSharedPointer<PointerType>& operator=(const CloneSharedPointer<PointerType>& copy) noexcept;

		//* operator get pointer
		inline PointerType& operator * () const noexcept { return *m_pointer; }
		//-> operator get pointer dont delete
		inline PointerType* operator -> () const noexcept { return m_pointer; }
		//bool operator is pointer != nullptr
		inline operator bool() const noexcept { return m_pointer != nullptr; }
		//! operator is pointer == nullptr
		inline bool operator ! () const noexcept { return m_pointer == nullptr; }

	private:
		//Release Pointer (Used Gabage Collector if Clone)
		inline void ReleasePointer() noexcept override;

		//�|�C���^
		PointerType* m_pointer;
	};

	//�Q�ƃJ�E���^�������J�E���^ == 0�Ŏ����I��delete�����CloneSharedPointer class [Array]
	template<class PointerType>
	class CloneSharedPointer<PointerType[]> final : public Detail::Pointer::BaseClonePointer
	{
	public:
		//friend
		friend class CloneWeakPointer<PointerType[]>;
		//friend
		template<class NewType> friend class CloneWeakPointer;
		//friend
		template<class NewType> friend class CloneSharedPointer;

		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//nullptr���������s��
		inline CloneSharedPointer() noexcept;
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//�Q�ƃR�s�[���s��
		//����1: �R�s�[��
		inline CloneSharedPointer(const CloneSharedPointer<PointerType[]>& copy) noexcept;
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//���g�ւ�Move���s��
		//����1: Move��
		inline CloneSharedPointer(CloneSharedPointer<PointerType[]>&& move) noexcept;
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//���������s��
		//����1: clone source pointer
		//����2: �z��T�C�Y
		//����3: Used mutex counter (Required for Multi thread), default = true
		//����4: Auto gabage collection?, default = true
		template<class NewType>
		inline explicit CloneSharedPointer(NewType* cloneSourcePointer, ushort arraySize,
			bool isUsedMutexCounter = true, bool isAutoGabageCollection = true) noexcept;
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//���������s��
		//����1: clone source pointer
		//����2: Used mutex counter (Required for Multi thread), default = true
		//����3: Auto gabage collection?, default = true
		template<class NewType>
		inline explicit CloneSharedPointer(const UniquePointer<NewType[]>& cloneSourcePointer,
			bool isUsedMutexCounter = true, bool isAutoGabageCollection = true) noexcept;
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//���������s��
		//����1: clone source pointer
		//����2: Used mutex counter (Required for Multi thread), default = true
		//����3: Auto gabage collection?, default = true
		template<class NewType>
		inline explicit CloneSharedPointer(const SharedPointer<NewType[]>& cloneSourcePointer,
			bool isUsedMutexCounter = true, bool isAutoGabageCollection = true) noexcept;
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//���������s��
		//����1: clone source pointer
		//����2: Used mutex counter (Required for Multi thread), default = true
		//����3: Auto gabage collection?, default = true
		template<class NewType>
		inline explicit CloneSharedPointer(const WeakPointer<NewType[]>& cloneSourcePointer,
			bool isUsedMutexCounter = true, bool isAutoGabageCollection = true) noexcept;
		//----------------------------------------------------------------------------------
		//[�f�X�g���N�^]
		//�����o�ϐ��̃f�X�g���N�^�ɂ��J�E���^�̌v�Z��J�����s��
		inline ~CloneSharedPointer() noexcept { ReleasePointer(); }


		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//���������X�V����, ���ӁIDirect->Clone�͎���Update���s���܂���
		//�쐬���̃R�s�[�T�C�Y����ɃR�s�[����̂ŁA�N���X�̌p������ł̓N���b�V�����܂��B���ӁB
		//return: ����������true, ���g or copy��nullptr�Ȃ�false
		//����1: copy value
		inline bool UpdateManual(PointerType* copy) const noexcept
		{
			static_assert(!std::is_convertible<Administrator::GraphicsDetail::ComponentX11, PointerType>
				& !std::is_base_of_v<Administrator::GraphicsDetail::ComponentX11, PointerType>,
				"\nClonePointer->UpdateManual Failed.\nComponent class used Component->CloneUpdate.");
			if (m_counter != nullptr && copy != nullptr)
			{
				auto copySize = GabageCollector::m_cloneShareds.at(m_pointer).copySize;

				memcpy_s(m_pointer, copySize, copy, copySize);
				return true;
			}
			else
				return false;
		}
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//���������X�V����, ���ӁIDirect->Clone�͎���Update���s���܂���
		//�쐬���̃R�s�[�T�C�Y����ɃR�s�[����̂ŁA�N���X�̌p������ł̓N���b�V�����܂��B���ӁB
		//return: ����������true, ���g or copy��nullptr�Ȃ�false
		//����1: copy value
		template<class DifferentType>
		inline bool UpdateManual(const DifferentType* copy) const noexcept
		{
			static_assert(std::is_convertible_v<DifferentType, PointerType>,
				"\nClonePointer->UpdateManual Failed.\nCondition : is_convertible_v<DifferentType, PointerType>");
			return Update(static_cast<PointerType*>(copy));
		}
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//���������X�V����, ���ӁIUnique->Clone�͎���Update��Sync���ɍs���Ă��܂�
		//�쐬���̃R�s�[�T�C�Y����ɃR�s�[����̂ŁA�N���X�̌p������ł̓N���b�V�����܂��B���ӁB
		//return: ����������true, ���g or copy��nullptr�Ȃ�false
		//����1: copy value
		inline bool UpdateManual(const UniquePointer<PointerType[]>& copy) const noexcept { return UpdateManual(copy.getPointer()); }
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//���������X�V����, ���ӁIUnique->Clone�͎���Update��Sync���ɍs���Ă��܂�
		//�쐬���̃R�s�[�T�C�Y����ɃR�s�[����̂ŁA�N���X�̌p������ł̓N���b�V�����܂��B���ӁB
		//return: ����������true, ���g or copy��nullptr�Ȃ�false
		//����1: copy value
		template<class DifferentType>
		inline bool UpdateManual(const UniquePointer<DifferentType[]>& copy) const noexcept
		{
			static_assert(std::is_convertible_v<DifferentType, PointerType>,
				"\nClonePointer->UpdateManual Failed.\nCondition : is_convertible_v<DifferentType, PointerType>");
			return Update(static_cast<PointerType*>(copy.getPointer()));
		}
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//���������X�V����, ���ӁIShared->Clone�͎���Update��Sync���ɍs���Ă��܂�
		//�쐬���̃R�s�[�T�C�Y����ɃR�s�[����̂ŁA�N���X�̌p������ł̓N���b�V�����܂��B���ӁB
		//return: ����������true, ���g or copy��nullptr�Ȃ�false
		//����1: copy value
		inline bool UpdateManual(const SharedPointer<PointerType[]>& copy) const noexcept { return UpdateManual(copy.getPointer()); }
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//���������X�V����, ���ӁIShared->Clone�͎���Update��Sync���ɍs���Ă��܂�
		//�쐬���̃R�s�[�T�C�Y����ɃR�s�[����̂ŁA�N���X�̌p������ł̓N���b�V�����܂��B���ӁB
		//return: ����������true, ���g or copy��nullptr�Ȃ�false
		//����1: copy value
		template<class DifferentType>
		inline bool UpdateManual(const SharedPointer<DifferentType[]>& copy) const noexcept
		{
			static_assert(std::is_convertible_v<DifferentType, PointerType>,
				"\nClonePointer->Update Failed.\nCondition : is_convertible_v<DifferentType, PointerType>");
			return UpdateManual(static_cast<PointerType*>(copy.getPointer()));
		}
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//���������X�V����, ���ӁIWeak(Shared)->Clone�͎���Update��Sync���ɍs���Ă��܂�
		//�쐬���̃R�s�[�T�C�Y����ɃR�s�[����̂ŁA�N���X�̌p������ł̓N���b�V�����܂��B���ӁB
		//return: ����������true, ���g or copy��nullptr�Ȃ�false
		//����1: copy value
		inline bool UpdateManual(WeakPointer<PointerType[]> copy) const noexcept
		{
			LockWeakGuard<WeakPointer<PointerType>> guard(copy);
			if (guard) return UpdateManual(copy.TryGetInstance());
			else return false;
		}
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//���������X�V����, ���ӁIWeak(Shared)->Clone�͎���Update��Sync���ɍs���Ă��܂�
		//�쐬���̃R�s�[�T�C�Y����ɃR�s�[����̂ŁA�N���X�̌p������ł̓N���b�V�����܂��B���ӁB
		//return: ����������true, ���g or copy��nullptr�Ȃ�false
		//����1: copy value
		template<class DifferentType>
		inline bool UpdateManual(WeakPointer<DifferentType[]> copy) const noexcept
		{
			static_assert(std::is_convertible_v<DifferentType, PointerType>,
				"\nClonePointer->Update Failed.\nCondition : is_convertible_v<DifferentType, PointerType>");
			LockWeakGuard<WeakPointer<DifferentType>> guard(copy);
			if (guard) return UpdateManual(static_cast<PointerType*>(copy.TryGetInstance()));
			else return false;
		}
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//���������X�V����, ���ӁIClone->Clone�͎���Update���s���܂���
		//�쐬���̃R�s�[�T�C�Y����ɃR�s�[����̂ŁA�N���X�̌p������ł̓N���b�V�����܂��B���ӁB
		//return: ����������true, ���g or copy��nullptr�Ȃ�false
		//����1: copy value
		inline bool UpdateManual(const CloneSharedPointer<PointerType[]>& copy) const noexcept { return UpdateManual(copy.getPointer()); }
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//���������X�V����, ���ӁIClone->Clone�͎���Update���s���܂���
		//�쐬���̃R�s�[�T�C�Y����ɃR�s�[����̂ŁA�N���X�̌p������ł̓N���b�V�����܂��B���ӁB
		//return: ����������true, ���g or copy��nullptr�Ȃ�false
		//����1: copy value
		template<class DifferentType>
		inline bool UpdateManual(const CloneSharedPointer<DifferentType[]>& copy) const noexcept
		{
			static_assert(std::is_convertible_v<DifferentType, PointerType>,
				"\nClonePointer->Update Failed.\nCondition : is_convertible_v<DifferentType, PointerType>");
			return UpdateManual(static_cast<PointerType*>(copy.getPointer()));
		}
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//���������X�V����, ���ӁIClone->Clone�͎���Update���s���܂���
		//�쐬���̃R�s�[�T�C�Y����ɃR�s�[����̂ŁA�N���X�̌p������ł̓N���b�V�����܂��B���ӁB
		//return: ����������true, ���g or copy��nullptr�Ȃ�false
		//����1: copy value
		inline bool UpdateManual(CloneWeakPointer<PointerType[]> copy) const noexcept;
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//���������X�V����, ���ӁIClone->Clone�͎���Update���s���܂���
		//�쐬���̃R�s�[�T�C�Y����ɃR�s�[����̂ŁA�N���X�̌p������ł̓N���b�V�����܂��B���ӁB
		//return: ����������true, ���g or copy��nullptr�Ȃ�false
		//����1: copy value
		template<class DifferentType>
		inline bool UpdateManual(CloneWeakPointer<DifferentType[]> copy) const noexcept;

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
		//return: DynamicCast����CloneSharedPointer
		template<class DifferentType>
		inline CloneSharedPointer<DifferentType[]> DynamicCast() const noexcept;
		//----------------------------------------------------------------------------------
		//[DownCast]
		//return: DownCast����CloneSharedPointer
		template<class DifferentType>
		inline CloneSharedPointer<DifferentType[]> DownCast() const noexcept;
		
		//this raw pointer, not delete!! (get function property)
		SGF_FUNCTION_PROPERTY PointerType* getPointer() const noexcept { return m_pointer; }
		//shared count (get function property)
		SGF_FUNCTION_PROPERTY ushort getSharedCount() const noexcept { return m_counter ? m_counter->getShared() : 0; }
		//weak count (get function property)
		SGF_FUNCTION_PROPERTY ushort getWeakCount() const noexcept { return m_counter ? m_counter->getWeak() : 0; }
		//pointer is valid(�L��)? (get function property)
		SGF_FUNCTION_PROPERTY bool getIsValid() const noexcept { return m_pointer ? true : false; }
		//pointer is linked(�����������L��)? (get function property)
		SGF_FUNCTION_PROPERTY bool getIsLinked() const noexcept { return m_counter ? m_counter->getIsLinked() : false; }

		//array size (get function property)
		SGF_FUNCTION_PROPERTY ushort getArraySize() const noexcept { return m_arraySize; }
		//array begin pointer, not delete!!(get function property)
		SGF_FUNCTION_PROPERTY PointerType* begin() { return m_pointer; }
		//array end pointer, not delete!!(get function property)
		SGF_FUNCTION_PROPERTY PointerType* end() { return m_pointer + m_arraySize; }

		//null pointer<static>(get function property)
		SGF_FUNCTION_PROPERTY static CloneSharedPointer<PointerType[]> getNull() noexcept { return std::move(CloneSharedPointer<PointerType[]>()); }
		//set null pointer (set function property)
		//return : this pointer (null)
		SGF_FUNCTION_PROPERTY const CloneSharedPointer<PointerType[]>& setNull() noexcept { return *this = CloneSharedPointer<PointerType[]>(); }

		//= operator, addCount
		inline const CloneSharedPointer<PointerType[]>& operator=(const CloneSharedPointer<PointerType[]>& copy) noexcept;
		//= operator, move
		inline CloneSharedPointer<PointerType[]>& operator=(CloneSharedPointer<PointerType[]>&& move) noexcept;
		//[] operator get pointer dont delete
		inline PointerType& operator [](uint index) const noexcept { return m_pointer[index]; }
		//bool operator is pointer != nullptr
		inline operator bool() const noexcept { return m_pointer != nullptr; }
		//! operator is pointer == nullptr
		inline bool operator ! () const noexcept { return m_pointer == nullptr; }

	private:
		//Release Pointer (Used Gabage Collector if Clone)
		inline void ReleasePointer() noexcept override;
		
		//�|�C���^
		PointerType* m_pointer;
		//ArraySize
		ushort m_arraySize;
	};


	//��Q�ƃJ�E���^������CloneWeakPointer class
	template<class PointerType>
	class CloneWeakPointer final : public Detail::Pointer::BaseClonePointer
	{
	public:
		//friend
		friend class CloneSharedPointer<PointerType>;
		template<class NewClass> friend class CloneSharedPointer;
		template<class NewClass> friend class CloneWeakPointer;

		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//nullptr���������s��
		inline CloneWeakPointer() noexcept;
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//�Q�ƃR�s�[���s��
		//����1: �R�s�[��
		inline CloneWeakPointer(const CloneWeakPointer<PointerType>& copy) noexcept;
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//���g�ւ�Move���s��
		//����1: Move��
		inline CloneWeakPointer(CloneWeakPointer<PointerType>&& move) noexcept;
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//�Q�ƃR�s�[���s��
		//����1: �R�s�[��<Shared>
		inline CloneWeakPointer(const CloneSharedPointer<PointerType>& copy) noexcept;
		//----------------------------------------------------------------------------------
		//[�f�X�g���N�^]
		//�����o�ϐ��̃f�X�g���N�^�ɂ��J�E���^�̌v�Z��J�����s��
		inline ~CloneWeakPointer() noexcept { ReleasePointer(); }


		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//���������X�V����, ���ӁIDirect->Clone�͎���Update���s���܂���
		//�쐬���̃R�s�[�T�C�Y����ɃR�s�[����̂ŁA�N���X�̌p������ł̓N���b�V�����܂��B���ӁB
		//return: ����������true, ���g or copy��nullptr�Ȃ�false
		//����1: copy value
		inline bool UpdateManual(PointerType* copy) const noexcept
		{
			static_assert(!std::is_convertible<Administrator::GraphicsDetail::ComponentX11, PointerType>
				& !std::is_base_of_v<Administrator::GraphicsDetail::ComponentX11, PointerType>,
				"\nClonePointer->Update Failed.\nComponent class used Component->CloneUpdate.");

			if ((m_isImLocked | LockShared()) && m_counter != nullptr && copy != nullptr)
			{
				auto copySize = GabageCollector::m_cloneShareds.at(m_pointer).copySize;

				memcpy_s(m_pointer, copySize, copy, copySize);
				return true;
			}
			else
				return false;
		}
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//���������X�V����, ���ӁIDirect->Clone�͎���Update���s���܂���
		//�쐬���̃R�s�[�T�C�Y����ɃR�s�[����̂ŁA�N���X�̌p������ł̓N���b�V�����܂��B���ӁB
		//return: ����������true, ���g or copy��nullptr�Ȃ�false
		//����1: copy value
		template<class DifferentType>
		inline bool UpdateManual(const DifferentType* copy) const noexcept
		{
			static_assert(std::is_convertible_v<DifferentType, PointerType>,
				"\nClonePointer->Update Failed.\nCondition : is_convertible_v<DifferentType, PointerType>");
			return UpdateManual(static_cast<PointerType*>(copy));
		}
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//���������X�V����, ���ӁIUnique->Clone�͎���Update��Sync���ɍs���Ă��܂�
		//�쐬���̃R�s�[�T�C�Y����ɃR�s�[����̂ŁA�N���X�̌p������ł̓N���b�V�����܂��B���ӁB
		//return: ����������true, ���g or copy��nullptr�Ȃ�false
		//����1: copy value
		inline bool UpdateManual(const UniquePointer<PointerType>& copy) const noexcept { return UpdateManual(copy.getPointer()); }
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//���������X�V����, ���ӁIUnique->Clone�͎���Update��Sync���ɍs���Ă��܂�
		//�쐬���̃R�s�[�T�C�Y����ɃR�s�[����̂ŁA�N���X�̌p������ł̓N���b�V�����܂��B���ӁB
		//return: ����������true, ���g or copy��nullptr�Ȃ�false
		//����1: copy value
		template<class DifferentType>
		inline bool UpdateManual(const UniquePointer<DifferentType>& copy) const noexcept
		{
			static_assert(std::is_convertible_v<DifferentType, PointerType>,
				"\nClonePointer->Update Failed.\nCondition : is_convertible_v<DifferentType, PointerType>");
			return Update(static_cast<PointerType*>(copy.getPointer()));
		}
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//���������X�V����, ���ӁIShared->Clone�͎���Update��Sync���ɍs���Ă��܂�
		//�쐬���̃R�s�[�T�C�Y����ɃR�s�[����̂ŁA�N���X�̌p������ł̓N���b�V�����܂��B���ӁB
		//return: ����������true, ���g or copy��nullptr�Ȃ�false
		//����1: copy value
		inline bool UpdateManual(const SharedPointer<PointerType>& copy) const noexcept { return UpdateManual(copy.getPointer()); }
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//���������X�V����, ���ӁIShared->Clone�͎���Update��Sync���ɍs���Ă��܂�
		//�쐬���̃R�s�[�T�C�Y����ɃR�s�[����̂ŁA�N���X�̌p������ł̓N���b�V�����܂��B���ӁB
		//return: ����������true, ���g or copy��nullptr�Ȃ�false
		//����1: copy value
		template<class DifferentType>
		inline bool UpdateManual(const SharedPointer<DifferentType>& copy) const noexcept
		{
			static_assert(std::is_convertible_v<DifferentType, PointerType>,
				"\nClonePointer->UpdateManual Failed.\nCondition : is_convertible_v<DifferentType, PointerType>");
			return UpdateManual(static_cast<PointerType*>(copy.getPointer()));
		}
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//���������X�V����, ���ӁIWeak(Shared)->Clone�͎���Update��Sync���ɍs���Ă��܂�
		//�쐬���̃R�s�[�T�C�Y����ɃR�s�[����̂ŁA�N���X�̌p������ł̓N���b�V�����܂��B���ӁB
		//return: ����������true, ���g or copy��nullptr�Ȃ�false
		//����1: copy value
		inline bool UpdateManual(WeakPointer<PointerType> copy) const noexcept
		{
			LockWeakGuard<WeakPointer<PointerType>> guard(copy);
			if (guard) return UpdateManual(copy.TryGetInstance());
			else return false;
		}
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//���������X�V����, ���ӁIWeak(Shared)->Clone�͎���Update��Sync���ɍs���Ă��܂�
		//�쐬���̃R�s�[�T�C�Y����ɃR�s�[����̂ŁA�N���X�̌p������ł̓N���b�V�����܂��B���ӁB
		//return: ����������true, ���g or copy��nullptr�Ȃ�false
		//����1: copy value
		template<class DifferentType>
		inline bool UpdateManual(WeakPointer<DifferentType> copy) const noexcept
		{
			static_assert(std::is_convertible_v<DifferentType, PointerType>,
				"\nClonePointer->UpdateManual Failed.\nCondition : is_convertible_v<DifferentType, PointerType>");
			LockWeakGuard<WeakPointer<DifferentType>> guard(copy);
			if (guard) return UpdateManual(static_cast<PointerType*>(copy.TryGetInstance()));
			else return false;
		}
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//���������X�V����, ���ӁIClone->Clone�͎���Update���s���܂���
		//�쐬���̃R�s�[�T�C�Y����ɃR�s�[����̂ŁA�N���X�̌p������ł̓N���b�V�����܂��B���ӁB
		//return: ����������true, ���g or copy��nullptr�Ȃ�false
		//����1: copy value
		inline bool UpdateManual(const CloneSharedPointer<PointerType>& copy) const noexcept { return UpdateManual(copy.getPointer()); }
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//���������X�V����, ���ӁIClone->Clone�͎���Update���s���܂���
		//�쐬���̃R�s�[�T�C�Y����ɃR�s�[����̂ŁA�N���X�̌p������ł̓N���b�V�����܂��B���ӁB
		//return: ����������true, ���g or copy��nullptr�Ȃ�false
		//����1: copy value
		template<class DifferentType>
		inline bool UpdateManual(const CloneSharedPointer<DifferentType>& copy) const noexcept
		{
			static_assert(std::is_convertible_v<DifferentType, PointerType>,
				"\nClonePointer->UpdateManual Failed.\nCondition : is_convertible_v<DifferentType, PointerType>");
			return UpdateManual(static_cast<PointerType*>(copy.getPointer()));
		}
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//���������X�V����, ���ӁIClone->Clone�͎���Update���s���܂���
		//�쐬���̃R�s�[�T�C�Y����ɃR�s�[����̂ŁA�N���X�̌p������ł̓N���b�V�����܂��B���ӁB
		//return: ����������true, ���g or copy��nullptr�Ȃ�false
		//����1: copy value
		inline bool UpdateManual(CloneWeakPointer<PointerType> copy) const noexcept
		{
			LockWeakGuard<CloneWeakPointer<PointerType>> guard(copy);
			if (guard) return UpdateManual(copy.TryGetInstance());
			else return false;
		}
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//���������X�V����, ���ӁIClone->Clone�͎���Update���s���܂���
		//�쐬���̃R�s�[�T�C�Y����ɃR�s�[����̂ŁA�N���X�̌p������ł̓N���b�V�����܂��B���ӁB
		//return: ����������true, ���g or copy��nullptr�Ȃ�false
		//����1: copy value
		template<class DifferentType>
		inline bool UpdateManual(CloneWeakPointer<DifferentType> copy) const noexcept
		{
			static_assert(std::is_convertible_v<DifferentType, PointerType>,
				"\nClonePointer->UpdateManual Failed.\nCondition : is_convertible_v<DifferentType, PointerType>");
			LockWeakGuard<CloneWeakPointer<DifferentType>> guard(copy);
			if (guard) return UpdateManual(static_cast<PointerType*>(copy.TryGetInstance()));
			else return false;
		}

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
		//return: DynamicCast����CloneWeakPointer
		template<class DifferentType>
		inline CloneWeakPointer<DifferentType> DynamicCast() const noexcept;
		//----------------------------------------------------------------------------------
		//[DownCast]
		//return: DownCast����CloneWeakPointer
		template<class DifferentType>
		inline CloneWeakPointer<DifferentType> DownCast() const noexcept;
		//----------------------------------------------------------------------------------
		//[TryGetInstance]
		//�C���X�^���X�擾�����݂�
		//return: �C���X�^���X�|�C���^, SharedCount == 0�̏ꍇ��nullptr
		inline PointerType* TryGetInstance() const noexcept { return m_counter && m_counter->getShared() > 0 ? m_pointer : nullptr; }
		//----------------------------------------------------------------------------------
		//[InstantiateShared]
		//return: ��Q�Ƃ���ɍ쐬����CloneSharedPointer(SharedCount == 0�̏ꍇ��nullptr)
		inline CloneSharedPointer<PointerType> InstantiateShared() const noexcept;
		//----------------------------------------------------------------------------------
		//[InstantiateShared]
		//��Q�Ƃ���ɍ쐬����CloneSharedPointer���쐬����(SharedCount == 0�̏ꍇ��nullptr)
		//����1: �쐬����CloneSharedPointer��������|�C���^
		inline void InstantiateShared(CloneSharedPointer<PointerType>& pointer) const noexcept;
	
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
		//pointer is linked(�����������L��)? (get function property)
		SGF_FUNCTION_PROPERTY bool getIsLinked() const noexcept { return m_counter ? m_counter->getIsLinked() : false; }

		//get null pointer property (read only) return : null
		SGF_FUNCTION_PROPERTY static CloneWeakPointer<PointerType> getNull() noexcept { return std::move(CloneWeakPointer<PointerType>()); }
		//set null pointer property (set only) return : null
		SGF_FUNCTION_PROPERTY const CloneWeakPointer<PointerType>& setNull() noexcept { return *this = CloneWeakPointer<PointerType>(); }

		//* operator get pointer, worning! Plese be sure to execute LockShared or LockSharedGuard(*this)
		inline PointerType& operator * () const noexcept { return *m_pointer; }
		//-> operator get pointer, dont delete and worning! Plese be sure to execute LockShared or LockSharedGuard(*this)
		inline PointerType* operator ->() const noexcept { return m_pointer; }


		//= operator, addCount
		inline const CloneWeakPointer<PointerType>& operator=(const CloneWeakPointer<PointerType>& copy) noexcept;
		//= operator, move
		inline CloneWeakPointer<PointerType>& operator=(CloneWeakPointer<PointerType>&& move) noexcept;

		//= operator, addCount
		inline const CloneSharedPointer<PointerType>& operator=(const CloneSharedPointer<PointerType>& copt) noexcept;

		//bool operator is useCount != 0
		inline operator bool() const noexcept { return m_counter && m_counter->getShared() > 0 ? true : false; }
		//! operator is useCount ==  0
		inline bool operator ! () const noexcept { return m_counter && m_counter->getShared() > 0 ? false : true; }

	private:
		//Release Pointer (Used Gabage Collector if Clone)
		inline void ReleasePointer() noexcept override;

		//�|�C���^
		PointerType* m_pointer;
		//Lock�s������
		bool m_isImLocked;
	};



	//��Q�ƃJ�E���^������CloneWeakPointer class
	template<class PointerType>
	class CloneWeakPointer<PointerType[]> final : public Detail::Pointer::BaseClonePointer
	{
	public:
		//friend
		friend class CloneSharedPointer<PointerType[]>;
		//friend
		template<class NewType> friend class CloneSharedPointer;
		//friend
		template<class NewType> friend class CloneWeakPointer;

		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//nullptr���������s��
		inline CloneWeakPointer() noexcept;
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//�Q�ƃR�s�[���s��
		//����1: �R�s�[��
		inline CloneWeakPointer(const CloneWeakPointer<PointerType[]>& copy) noexcept;
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//���g�ւ�Move���s��
		//����1: Move��
		inline CloneWeakPointer(CloneWeakPointer<PointerType[]>&& move) noexcept;
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//�Q�ƃR�s�[���s��
		//����1: �R�s�[��<Shared>
		inline CloneWeakPointer(const CloneSharedPointer<PointerType[]>& copy) noexcept;
		//----------------------------------------------------------------------------------
		//[�f�X�g���N�^]
		//�����o�ϐ��̃f�X�g���N�^�ɂ��J�E���^�̌v�Z��J�����s��
		inline ~CloneWeakPointer() noexcept { ReleasePointer(); }

		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//���������X�V����, ���ӁIDirect->Clone�͎���Update���s���܂���
		//�쐬���̃R�s�[�T�C�Y����ɃR�s�[����̂ŁA�N���X�̌p������ł̓N���b�V�����܂��B���ӁB
		//return: ����������true, ���g or copy��nullptr�Ȃ�false
		//����1: copy value
		inline bool UpdateManual(PointerType* copy) const noexcept
		{
			static_assert(!std::is_convertible<Administrator::GraphicsDetail::ComponentX11, PointerType>
				& !std::is_base_of_v<Administrator::GraphicsDetail::ComponentX11, PointerType>,
				"\nClonePointer->Update Failed.\nComponent class used Component->CloneUpdate.");
		
			if ((m_isImLocked | LockShared()) && m_counter != nullptr && copy != nullptr)
			{
				auto copySize = GabageCollector::m_cloneShareds.at(m_pointer).copySize;

				memcpy_s(m_pointer, copySize, copy, copySize);
				return true;
			}
			else
				return false;
		}
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//���������X�V����, ���ӁIDirect->Clone�͎���Update���s���܂���
		//�쐬���̃R�s�[�T�C�Y����ɃR�s�[����̂ŁA�N���X�̌p������ł̓N���b�V�����܂��B���ӁB
		//return: ����������true, ���g or copy��nullptr�Ȃ�false
		//����1: copy value
		template<class DifferentType>
		inline bool UpdateManual(const DifferentType* copy) const noexcept
		{
			static_assert(std::is_convertible_v<DifferentType, PointerType>,
				"\nClonePointer->UpdateManual Failed.\nCondition : is_convertible_v<DifferentType, PointerType>");
			return UpdateManual(static_cast<PointerType*>(copy));
		}
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//���������X�V����, ���ӁIUnique->Clone�͎���Update��Sync���ɍs���Ă��܂�
		//�쐬���̃R�s�[�T�C�Y����ɃR�s�[����̂ŁA�N���X�̌p������ł̓N���b�V�����܂��B���ӁB
		//return: ����������true, ���g or copy��nullptr�Ȃ�false
		//����1: copy value
		inline bool UpdateManual(const UniquePointer<PointerType[]>& copy) const noexcept { return UpdateManual(copy.getPointer()); }
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//���������X�V����, ���ӁIUnique->Clone�͎���Update��Sync���ɍs���Ă��܂�
		//�쐬���̃R�s�[�T�C�Y����ɃR�s�[����̂ŁA�N���X�̌p������ł̓N���b�V�����܂��B���ӁB
		//return: ����������true, ���g or copy��nullptr�Ȃ�false
		//����1: copy value
		template<class DifferentType>
		inline bool UpdateManual(const UniquePointer<DifferentType[]>& copy) const noexcept
		{
			static_assert(std::is_convertible_v<DifferentType, PointerType>,
				"\nClonePointer->UpdateManual Failed.\nCondition : is_convertible_v<DifferentType, PointerType>");
			return UpdateManual(static_cast<PointerType*>(copy.getPointer()));
		}
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//���������X�V����, ���ӁIShared->Clone�͎���Update��Sync���ɍs���Ă��܂�
		//�쐬���̃R�s�[�T�C�Y����ɃR�s�[����̂ŁA�N���X�̌p������ł̓N���b�V�����܂��B���ӁB
		//return: ����������true, ���g or copy��nullptr�Ȃ�false
		//����1: copy value
		inline bool UpdateManual(const SharedPointer<PointerType[]>& copy) const noexcept { return UpdateManual(copy.getPointer()); }
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//���������X�V����, ���ӁIShared->Clone�͎���Update��Sync���ɍs���Ă��܂�
		//�쐬���̃R�s�[�T�C�Y����ɃR�s�[����̂ŁA�N���X�̌p������ł̓N���b�V�����܂��B���ӁB
		//return: ����������true, ���g or copy��nullptr�Ȃ�false
		//����1: copy value
		template<class DifferentType>
		inline bool UpdateManual(const SharedPointer<DifferentType[]>& copy) const noexcept
		{
			static_assert(std::is_convertible_v<DifferentType, PointerType>,
				"\nClonePointer->UpdateManual Failed.\nCondition : is_convertible_v<DifferentType, PointerType>");
			return UpdateManual(static_cast<PointerType*>(copy.getPointer()));
		}
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//���������X�V����, ���ӁIWeak(Shared)->Clone�͎���Update��Sync���ɍs���Ă��܂�
		//�쐬���̃R�s�[�T�C�Y����ɃR�s�[����̂ŁA�N���X�̌p������ł̓N���b�V�����܂��B���ӁB
		//return: ����������true, ���g or copy��nullptr�Ȃ�false
		//����1: copy value
		inline bool UpdateManual(WeakPointer<PointerType[]> copy) const noexcept
		{
			LockWeakGuard<WeakPointer<PointerType>> guard(copy);
			if (guard) return UpdateManual(copy.TryGetInstance());
			else return false;
		}
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//���������X�V����, ���ӁIWeak(Shared)->Clone�͎���Update��Sync���ɍs���Ă��܂�
		//�쐬���̃R�s�[�T�C�Y����ɃR�s�[����̂ŁA�N���X�̌p������ł̓N���b�V�����܂��B���ӁB
		//return: ����������true, ���g or copy��nullptr�Ȃ�false
		//����1: copy value
		template<class DifferentType>
		inline bool UpdateManual(WeakPointer<DifferentType[]> copy) const noexcept
		{
			static_assert(std::is_convertible_v<DifferentType, PointerType>,
				"\nClonePointer->UpdateManual Failed.\nCondition : is_convertible_v<DifferentType, PointerType>");
			LockWeakGuard<WeakPointer<DifferentType>> guard(copy);
			if (guard) return UpdateManual(static_cast<PointerType*>(copy.TryGetInstance()));
			else return false;
		}
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//���������X�V����, ���ӁIClone->Clone�͎���Update���s���܂���
		//�쐬���̃R�s�[�T�C�Y����ɃR�s�[����̂ŁA�N���X�̌p������ł̓N���b�V�����܂��B���ӁB
		//return: ����������true, ���g or copy��nullptr�Ȃ�false
		//����1: copy value
		inline bool UpdateManual(const CloneSharedPointer<PointerType[]>& copy) const noexcept { return UpdateManual(copy.getPointer()); }
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//���������X�V����, ���ӁIClone->Clone�͎���Update���s���܂���
		//�쐬���̃R�s�[�T�C�Y����ɃR�s�[����̂ŁA�N���X�̌p������ł̓N���b�V�����܂��B���ӁB
		//return: ����������true, ���g or copy��nullptr�Ȃ�false
		//����1: copy value
		template<class DifferentType>
		inline bool UpdateManual(const CloneSharedPointer<DifferentType[]>& copy) const noexcept
		{
			static_assert(std::is_convertible_v<DifferentType, PointerType>,
				"\nClonePointer->UpdateManual Failed.\nCondition : is_convertible_v<DifferentType, PointerType>");
			return UpdateManual(static_cast<PointerType*>(copy.getPointer()));
		}
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//���������X�V����, ���ӁIClone->Clone�͎���Update���s���܂���
		//�쐬���̃R�s�[�T�C�Y����ɃR�s�[����̂ŁA�N���X�̌p������ł̓N���b�V�����܂��B���ӁB
		//return: ����������true, ���g or copy��nullptr�Ȃ�false
		//����1: copy value
		inline bool UpdateManual(CloneWeakPointer<PointerType[]> copy) const noexcept
		{
			LockWeakGuard<CloneWeakPointer<PointerType>> guard(copy);
			if (guard) return UpdateManual(copy.TryGetInstance());
			else return false;
		}
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//���������X�V����, ���ӁIClone->Clone�͎���Update���s���܂���
		//�쐬���̃R�s�[�T�C�Y����ɃR�s�[����̂ŁA�N���X�̌p������ł̓N���b�V�����܂��B���ӁB
		//return: ����������true, ���g or copy��nullptr�Ȃ�false
		//����1: copy value
		template<class DifferentType>
		inline bool UpdateManual(CloneWeakPointer<DifferentType[]> copy) const noexcept
		{
			static_assert(std::is_convertible_v<DifferentType, PointerType>,
				"\nClonePointer->UpdateManual Failed.\nCondition : is_convertible_v<DifferentType, PointerType>");
			LockWeakGuard<CloneWeakPointer<DifferentType>> guard(copy);
			if (guard) return UpdateManual(static_cast<PointerType*>(copy.TryGetInstance()));
			else return false;
		}

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
		//return: DynamicCast����CloneWeakPointer
		template<class DifferentType>
		inline CloneWeakPointer<DifferentType[]> DynamicCast() const noexcept;
		//----------------------------------------------------------------------------------
		//[DownCast]
		//return: DownCast����CloneWeakPointer
		template<class DifferentType>
		inline CloneWeakPointer<DifferentType[]> DownCast() const noexcept;
		//----------------------------------------------------------------------------------
		//[TryGetInstance]
		//�C���X�^���X�擾�����݂�
		//return: �C���X�^���X�|�C���^, SharedCount == 0�̏ꍇ��nullptr
		inline PointerType* TryGetInstance() const noexcept { return m_counter && m_counter->getShared() > 0 ? m_pointer : nullptr; }
		//----------------------------------------------------------------------------------
		//[InstantiateShared]
		//return: ��Q�Ƃ���ɍ쐬����CloneSharedPointer(SharedCount == 0�̏ꍇ��nullptr)
		inline CloneSharedPointer<PointerType[]> InstantiateShared() const noexcept;
		//----------------------------------------------------------------------------------
		//[InstantiateShared]
		//��Q�Ƃ���ɍ쐬����CloneSharedPointer���쐬����(SharedCount == 0�̏ꍇ��nullptr)
		//����1: �쐬����CloneSharedPointer��������|�C���^
		inline void InstantiateShared(CloneSharedPointer<PointerType[]>& pointer) const noexcept;

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
		//pointer is linked(�����������L��)? (get function property)
		SGF_FUNCTION_PROPERTY bool getIsLinked() const noexcept { return m_counter ? m_counter->getIsLinked() : false; }

		//get null pointer property (read only) return : null
		SGF_FUNCTION_PROPERTY static CloneWeakPointer<PointerType[]> getNull() noexcept { return std::move(CloneWeakPointer<PointerType[]>()); }
		//set null pointer property (set only) return : null
		SGF_FUNCTION_PROPERTY const CloneWeakPointer<PointerType[]>& setNull() noexcept { return *this = CloneWeakPointer<PointerType[]>(); }

		//get array size property (read only)
		SGF_FUNCTION_PROPERTY ushort getArraySize() const noexcept { return m_arraySize; }
		//get array begin property (read only)
		SGF_FUNCTION_PROPERTY PointerType* begin() { return m_pointer; }
		//get array end property (read only)
		SGF_FUNCTION_PROPERTY PointerType* end() { return m_pointer + m_arraySize; }

		//[] operator get pointer, worning! Plese be sure to execute LockShared or LockSharedGuard(*this)
		inline PointerType & operator[](uint index) const noexcept { return m_pointer[index]; }

		//= operator, addCount
		inline const CloneWeakPointer<PointerType[]>& operator=(const CloneWeakPointer<PointerType[]>& copy) noexcept;
		//= operator, move
		inline CloneWeakPointer<PointerType[]>& operator=(CloneWeakPointer<PointerType[]>&& move) noexcept;

		//= operator, addCount
		inline const CloneSharedPointer<PointerType[]>& operator=(const CloneSharedPointer<PointerType[]>& copt) noexcept;

		//bool operator is useCount != 0
		inline operator bool() const noexcept { return m_counter && m_counter->getShared() > 0 ? true : false; }
		//! operator is useCount ==  0
		inline bool operator ! () const noexcept { return m_counter && m_counter->getShared() > 0 ? false : true; }

	private:
		//���L�������
		inline void ReleasePointer() noexcept override;
		
		//�|�C���^
		PointerType* m_pointer;
		ushort m_arraySize;		//arraySize
		bool m_isImLocked;	//Lock�s������
	};





	//----------------------------------------------------------------------------------
	//CloneSharedPointer
	//----------------------------------------------------------------------------------



	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//nullptr���������s��
	template<class PointerType>
	inline CloneSharedPointer<PointerType>::CloneSharedPointer() noexcept
		: BaseClonePointer(nullptr, 0), m_pointer(nullptr) {}
	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//�Q�ƃR�s�[���s��
	//����1: �R�s�[��
	template<class PointerType>
	inline CloneSharedPointer<PointerType>::CloneSharedPointer(const CloneSharedPointer<PointerType>& copy) noexcept
		: BaseClonePointer(copy.m_counter), m_pointer(copy.m_pointer)
	{
		if (m_counter != nullptr) m_counter->addShared();
		else m_pointer = nullptr;
	}
	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//���g�ւ�Move���s��
	//����1: Move��
	template<class PointerType>
	inline CloneSharedPointer<PointerType>::CloneSharedPointer(CloneSharedPointer<PointerType>&& move) noexcept
		: BaseClonePointer(move.m_counter), m_pointer(move.m_pointer)
	{
		move.m_pointer = nullptr;
		move.m_counter = nullptr;
	}
	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//���������s��
	//����1: clone source pointer
	//����2: Used mutex counter (Required for Multi thread), default = true
	//����3: Auto gabage collection?, default = true
	template<class PointerType>
	template<class NewType>
	inline CloneSharedPointer<PointerType>::CloneSharedPointer(NewType* cloneSourcePointer,
		bool isUsedMutexCounter, bool isAutoGabageCollection) noexcept
		: BaseClonePointer(nullptr), m_pointer(nullptr)
	{
		if (cloneSourcePointer == nullptr) return;

		//copy size
		uint64 copySize = sizeof(NewType);

		//new clone
		m_pointer = reinterpret_cast<PointerType*>(malloc(copySize));
		memcpy_s(m_pointer, copySize, cloneSourcePointer, copySize);

		//new counter
		if (IS_TRUE(isUsedMutexCounter))
			m_counter = new Detail::Pointer::MutexCloneCounter(false);
		else
			m_counter = new Detail::Pointer::SimpleCloneCounter(false);

		//Register
		GabageCollector::RegisterCloneShared(m_pointer, cloneSourcePointer,
			m_counter, copySize, Pointer::CloneMode::CloneModeDirect, isAutoGabageCollection);
	}
	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//���������s��
	//����1: clone source pointer
	//����2: Used mutex counter (Required for Multi thread), default = true
	//����3: Auto gabage collection?, default = true
	template<class PointerType>
	template<class NewType>
	inline CloneSharedPointer<PointerType>::CloneSharedPointer(const UniquePointer<NewType>& cloneSourcePointer,
		bool isUsedMutexCounter, bool isAutoGabageCollection) noexcept
		: BaseClonePointer(nullptr), m_pointer(nullptr)
	{
		if (IS_FALSE(cloneSourcePointer.getIsOwned())) return;
		
		//copy size
		uint64 copySize = sizeof(NewType);

		//new clone
		m_pointer = reinterpret_cast<PointerType*>(malloc(copySize));
		memcpy_s(m_pointer, copySize, cloneSourcePointer.getPointer(), copySize);

		//new counter
		if (IS_TRUE(isUsedMutexCounter))
			m_counter = new Detail::Pointer::MutexCloneCounter(true);
		else
			m_counter = new Detail::Pointer::SimpleCloneCounter(true);

		//Register
		GabageCollector::RegisterCloneShared(m_pointer, cloneSourcePointer.getPointer(),
			m_counter, copySize, Pointer::CloneMode::CloneModeUnique, isAutoGabageCollection);
	}
	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//���������s��
	//����1: clone source pointer
	//����2: Used mutex counter (Required for Multi thread), default = true
	//����3: Auto gabage collection?, default = true
	template<class PointerType>
	template<class NewType>
	inline CloneSharedPointer<PointerType>::CloneSharedPointer(const SharedPointer<NewType>& cloneSourcePointer,
		bool isUsedMutexCounter, bool isAutoGabageCollection) noexcept
		: BaseClonePointer(nullptr), m_pointer(nullptr)
	{
		if (IS_FALSE(cloneSourcePointer.getIsValid())) return;

		//copy size
		uint64 copySize = sizeof(NewType);

		//new clone
		m_pointer = reinterpret_cast<PointerType*>(malloc(copySize));
		memcpy_s(m_pointer, copySize, cloneSourcePointer.getPointer(), copySize);

		//new counter
		if (IS_TRUE(isUsedMutexCounter))
			m_counter = new Detail::Pointer::MutexCounter();
		else
			m_counter = new Detail::Pointer::SimpleCounter();

		//Register
		GabageCollector::RegisterCloneShared(m_pointer, cloneSourcePointer.getPointer(),
			m_counter, copySize, Pointer::CloneMode::CloneModeShared, isAutoGabageCollection);
	}
	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//���������s��
	//����1: clone source pointer
	//����2: Used mutex counter (Required for Multi thread), default = true
	//����3: Auto gabage collection?, default = true
	template<class PointerType>
	template<class NewType>
	inline CloneSharedPointer<PointerType>::CloneSharedPointer(const WeakPointer<NewType>& cloneSourcePointer,
		bool isUsedMutexCounter, bool isAutoGabageCollection) noexcept
		: BaseClonePointer(nullptr), m_pointer(nullptr)
	{
		//Lock
		if (IS_FALSE(cloneSourcePointer.getIsLocked() | cloneSourcePointer.LockShared())) return;

		auto pointer = cloneSourcePointer.TryGetInstance();
		//copy size
		uint64 copySize = sizeof(NewType);

		//new clone
		m_pointer = reinterpret_cast<PointerType*>(malloc(copySize));
		memcpy_s(m_pointer, copySize, pointer, copySize);

		//new counter
		if (IS_TRUE(isUsedMutexCounter))
			m_counter = new Detail::Pointer::MutexCounter();
		else
			m_counter = new Detail::Pointer::SimpleCounter();

		GabageCollector::RegisterCloneShared(m_pointer, cloneSourcePointer.getPointer(),
			m_counter, copySize, Pointer::CloneMode::CloneModeShared, isAutoGabageCollection);

		//Unlock
		cloneSourcePointer.UnlockShared();
	}

	//----------------------------------------------------------------------------------
	//[Update]
	//���������X�V����, ���ӁIClone->Clone�͎���Update���s���܂���
	//�쐬���̃R�s�[�T�C�Y����ɃR�s�[����̂ŁA�N���X�̌p������ł̓N���b�V�����܂��B���ӁB
	//return: ����������true, ���g or copy��nullptr�Ȃ�false
	//����1: copy value
	template<class PointerType>
	inline bool CloneSharedPointer<PointerType>::UpdateManual(CloneWeakPointer<PointerType> copy) const noexcept
	{
		LockWeakGuard<CloneWeakPointer<PointerType>> guard(copy);
		if (guard) return UpdateManual(static_cast<PointerType*>(copy.TryGetInstance()));
		else return false;
	}
	//----------------------------------------------------------------------------------
	//[Update]
	//���������X�V����, ���ӁIClone->Clone�͎���Update���s���܂���
	//�쐬���̃R�s�[�T�C�Y����ɃR�s�[����̂ŁA�N���X�̌p������ł̓N���b�V�����܂��B���ӁB
	//return: ����������true, ���g or copy��nullptr�Ȃ�false
	//����1: copy value
	template<class PointerType>
	template<class DifferentType>
	inline bool CloneSharedPointer<PointerType>::UpdateManual(CloneWeakPointer<DifferentType> copy) const noexcept
	{
		static_assert(std::is_convertible_v<DifferentType, PointerType>,
			"\nClonePointer->Update Failed.\nCondition : is_convertible_v<DifferentType, PointerType>");
		LockWeakGuard<CloneWeakPointer<DifferentType>> guard(copy);
		if (guard) return UpdateManual(static_cast<PointerType*>(copy.TryGetInstance()));
		else return false;
	}

	//----------------------------------------------------------------------------------
	//[DynamicCast]
	//return: DynamicCast����CloneSharedPointer
	template<class PointerType>
	template<class DifferentType>
	inline CloneSharedPointer<DifferentType> CloneSharedPointer<PointerType>::DynamicCast() const noexcept
	{
		static_assert(std::is_base_of_v<PointerType, DifferentType> | std::is_base_of_v<DifferentType, PointerType>,
			"\nCloneSharedPointer::DynamicCast Failed.\nSuccess Condition : is_base_of<PointerType, DifferentType> or is_convertible<DifferentType, PointerType>");

		CloneSharedPointer<DifferentType> result;
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
	//return: DownCast����CloneSharedPointer
	template<class PointerType>
	template<class DifferentType>
	inline CloneSharedPointer<DifferentType> CloneSharedPointer<PointerType>::DownCast() const noexcept
	{
		static_assert(std::is_base_of_v<DifferentType, PointerType>,
			"\nCloneSharedPointer::DynamicCast Failed.\nSuccess Condition : is_base_of<DifferentType, PointerType>");

		CloneSharedPointer<DifferentType> result;
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
	inline const CloneSharedPointer<PointerType>& CloneSharedPointer<PointerType>::operator=(const CloneSharedPointer<PointerType>& copy) noexcept
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
	inline CloneSharedPointer<PointerType>& CloneSharedPointer<PointerType>::operator=(CloneSharedPointer<PointerType>&& move) noexcept
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

	//Release Pointer (Used Gabage Collector if Clone)
	template<class PointerType>
	inline void CloneSharedPointer<PointerType>::ReleasePointer() noexcept
	{
		//not null -> �폜�\��
		if (m_pointer != nullptr)
			GabageCollector::DestroyCloneShared(m_pointer, this);

		//nullptr
		m_counter = nullptr;
		m_pointer = nullptr;
	}



	//----------------------------------------------------------------------------------
	//CloneSharedPointer [Array]
	//----------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//nullptr���������s��
	template<class PointerType>
	inline CloneSharedPointer<PointerType[]>::CloneSharedPointer() noexcept
		: BaseClonePointer(nullptr, 0), m_pointer(nullptr), m_arraySize(0) {}
	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//�Q�ƃR�s�[���s��
	//����1: �R�s�[��
	template<class PointerType>
	inline CloneSharedPointer<PointerType[]>::CloneSharedPointer(const CloneSharedPointer<PointerType[]>& copy) noexcept
		: BaseClonePointer(copy.m_counter), m_pointer(copy.m_pointer), m_arraySize(copy.m_arraySize)
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
	inline CloneSharedPointer<PointerType[]>::CloneSharedPointer(CloneSharedPointer<PointerType[]>&& move) noexcept
		: BaseClonePointer(move.m_counter), m_pointer(move.m_pointer), m_arraySize(move.m_arraySize)
	{
		move.m_pointer = nullptr;
		move.m_counter = nullptr;
		move.m_arraySize = 0;
	}
	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//���������s��
	//����1: clone source pointer
	//����2: �z��T�C�Y
	//����3: Used mutex counter (Required for Multi thread), default = true
	//����4: Auto gabage collection?, default = true
	template<class PointerType>
	template<class NewType>
	inline CloneSharedPointer<PointerType[]>::CloneSharedPointer(NewType* cloneSourcePointer, ushort arraySize,
		bool isUsedMutexCounter, bool isAutoGabageCollection) noexcept
		: BaseClonePointer(nullptr), m_pointer(nullptr), m_arraySize(0)
	{
		if (cloneSourcePointer == nullptr) return;

		//copy size
		uint64 copySize = sizeof(NewType) * arraySize;
		//array size
		m_arraySize = arraySize;

		//new clone
		m_pointer = reinterpret_cast<PointerType*>(malloc(copySize));
		memcpy_s(m_pointer, copySize, cloneSourcePointer, copySize);

		//new counter
		if (IS_TRUE(isUsedMutexCounter))
			m_counter = new Detail::Pointer::MutexCloneCounter(false);
		else
			m_counter = new Detail::Pointer::SimpleCloneCounter(false);

		//Register
		GabageCollector::RegisterCloneShared(m_pointer, cloneSourcePointer,
			m_counter, copySize, Pointer::CloneMode::CloneModeDirect, isAutoGabageCollection);
	}
	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//���������s��
	//����1: clone source pointer
	//����2: Used mutex counter (Required for Multi thread), default = true
	//����3: Auto gabage collection?, default = true
	template<class PointerType>
	template<class NewType>
	inline CloneSharedPointer<PointerType[]>::CloneSharedPointer(const UniquePointer<NewType[]>& cloneSourcePointer,
		bool isUsedMutexCounter, bool isAutoGabageCollection) noexcept
		: BaseClonePointer(nullptr), m_pointer(nullptr), m_arraySize(0)
	{
		if (IS_FALSE(cloneSourcePointer.getIsOwned())) return;

		//array size
		m_arraySize = cloneSourcePointer.getArraySize();
		//copy size
		uint64 copySize = sizeof(NewType) * m_arraySize;
		
		//new clone
		m_pointer = reinterpret_cast<PointerType*>(malloc(copySize));
		memcpy_s(m_pointer, copySize, cloneSourcePointer.getPointer(), copySize);

		//new counter
		if (IS_TRUE(isUsedMutexCounter))
			m_counter = new Detail::Pointer::MutexCloneCounter(true);
		else
			m_counter = new Detail::Pointer::SimpleCloneCounter(true);

		//Register
		GabageCollector::RegisterCloneShared(m_pointer, cloneSourcePointer.getPointer(),
			m_counter, copySize, Pointer::CloneMode::CloneModeUnique, isAutoGabageCollection);
	}
	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//���������s��
	//����1: clone source pointer
	//����2: Used mutex counter (Required for Multi thread), default = true
	//����3: Auto gabage collection?, default = true
	template<class PointerType>
	template<class NewType>
	inline CloneSharedPointer<PointerType[]>::CloneSharedPointer(const SharedPointer<NewType[]>& cloneSourcePointer,
		bool isUsedMutexCounter, bool isAutoGabageCollection) noexcept
		: BaseClonePointer(nullptr), m_pointer(nullptr), m_arraySize(0)
	{
		if (IS_FALSE(cloneSourcePointer.getIsValid())) return;
		//array size
		m_arraySize = cloneSourcePointer.getArraySize();
		//copy size
		uint64 copySize = sizeof(NewType) * m_arraySize;

		//new clone
		m_pointer = reinterpret_cast<PointerType*>(malloc(copySize));
		memcpy_s(m_pointer, copySize, cloneSourcePointer.getPointer(), copySize);

		//new counter
		if (IS_TRUE(isUsedMutexCounter))
			m_counter = new Detail::Pointer::MutexCloneCounter(true);
		else
			m_counter = new Detail::Pointer::SimpleCloneCounter(true);

		//Register
		GabageCollector::RegisterCloneShared(m_pointer, cloneSourcePointer.getPointer(),
			m_counter, copySize, Pointer::CloneMode::CloneModeShared, isAutoGabageCollection);
	}
	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//���������s��
	//����1: clone source pointer
	//����2: Used mutex counter (Required for Multi thread), default = true
	//����3: Auto gabage collection?, default = true
	template<class PointerType>
	template<class NewType>
	inline CloneSharedPointer<PointerType[]>::CloneSharedPointer(const WeakPointer<NewType[]>& cloneSourcePointer,
		bool isUsedMutexCounter, bool isAutoGabageCollection) noexcept
		: BaseClonePointer(nullptr), m_pointer(nullptr), m_arraySize(0)
	{
		//Lock
		if (IS_FALSE(cloneSourcePointer.getIsLocked() | cloneSourcePointer.LockShared())) return;

		auto pointer = cloneSourcePointer.TryGetInstance();
		//array size
		m_arraySize = cloneSourcePointer.getArraySize();
		//copy size
		uint64 copySize = sizeof(NewType) * m_arraySize;

		//new clone
		m_pointer = reinterpret_cast<PointerType*>(malloc(copySize));
		memcpy_s(m_pointer, copySize, pointer, copySize);

		//new counter
		if (IS_TRUE(isUsedMutexCounter))
			m_counter = new Detail::Pointer::MutexCounter();
		else
			m_counter = new Detail::Pointer::SimpleCounter();

		//Register
		GabageCollector::RegisterCloneShared(m_pointer, pointer,
			m_counter, copySize, Pointer::CloneMode::CloneModeShared, isAutoGabageCollection);

		//Unlock
		cloneSourcePointer.UnlockShared();
	}

	//----------------------------------------------------------------------------------
	//[Update]
	//���������X�V����, ���ӁIClone->Clone�͎���Update���s���܂���
	//�쐬���̃R�s�[�T�C�Y����ɃR�s�[����̂ŁA�N���X�̌p������ł̓N���b�V�����܂��B���ӁB
	//return: ����������true, ���g or copy��nullptr�Ȃ�false
	//����1: copy value
	template<class PointerType>
	inline bool CloneSharedPointer<PointerType[]>::UpdateManual(CloneWeakPointer<PointerType[]> copy) const noexcept
	{
		LockWeakGuard<CloneWeakPointer<PointerType>> guard(copy);
		if (guard) return UpdateManual(static_cast<PointerType*>(copy.TryGetInstance()));
		else return false;
	}
	//----------------------------------------------------------------------------------
	//[Update]
	//���������X�V����, ���ӁIClone->Clone�͎���Update���s���܂���
	//�쐬���̃R�s�[�T�C�Y����ɃR�s�[����̂ŁA�N���X�̌p������ł̓N���b�V�����܂��B���ӁB
	//return: ����������true, ���g or copy��nullptr�Ȃ�false
	//����1: copy value
	template<class PointerType>
	template<class DifferentType>
	inline bool CloneSharedPointer<PointerType[]>::UpdateManual(CloneWeakPointer<DifferentType[]> copy) const noexcept
	{
		static_assert(std::is_convertible_v<DifferentType, PointerType>,
			"\nClonePointer->Update Failed.\nCondition : is_convertible_v<DifferentType, PointerType>");
		LockWeakGuard<CloneWeakPointer<DifferentType>> guard(copy);
		if (guard) return UpdateManual(static_cast<PointerType*>(copy.TryGetInstance()));
		else return false;
	}

	//----------------------------------------------------------------------------------
	//[DynamicCast]
	//return: DynamicCast����CloneSharedPointer
	template<class PointerType>
	template<class DifferentType>
	inline CloneSharedPointer<DifferentType[]> CloneSharedPointer<PointerType[]>::DynamicCast() const noexcept
	{
		static_assert(std::is_base_of_v<PointerType, DifferentType> | std::is_base_of_v<DifferentType, PointerType>,
			"\nCloneSharedPointer::DynamicCast Failed.\nSuccess Condition : is_base_of<PointerType, DifferentType> or is_convertible<DifferentType, PointerType>");

		CloneSharedPointer<DifferentType[]> result;
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
	//return: DownCast����CloneSharedPointer
	template<class PointerType>
	template<class DifferentType>
	inline CloneSharedPointer<DifferentType[]> CloneSharedPointer<PointerType[]>::DownCast() const noexcept
	{
		static_assert(std::is_base_of_v<DifferentType, PointerType>,
			"\nCloneSharedPointer::DynamicCast Failed.\nSuccess Condition : is_base_of<DifferentType, PointerType>");

		CloneSharedPointer<DifferentType[]> result;
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
	inline const CloneSharedPointer<PointerType[]>& CloneSharedPointer<PointerType[]>::operator=(const CloneSharedPointer<PointerType[]>& copy) noexcept
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
	inline CloneSharedPointer<PointerType[]>& CloneSharedPointer<PointerType[]>::operator=(CloneSharedPointer<PointerType[]>&& move) noexcept
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

	//�|�C���^�̊J�������݂�
	template<class PointerType>
	inline void CloneSharedPointer<PointerType[]>::ReleasePointer() noexcept
	{
		//not null -> �폜�\��
		if (m_pointer != nullptr)
			GabageCollector::DestroyCloneShared(m_pointer, this);

		//nullptr
		m_counter = nullptr;
		m_pointer = nullptr;
		m_arraySize = 0;
	}



	//----------------------------------------------------------------------------------
	//CloneWeakPointer
	//----------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//nullptr���������s��
	template<class PointerType>
	inline CloneWeakPointer<PointerType>::CloneWeakPointer() noexcept
		: BaseClonePointer(nullptr, 0), m_pointer(nullptr), m_isImLocked(false) {}
	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//�Q�ƃR�s�[���s��
	//����1: �R�s�[��
	template<class PointerType>
	inline CloneWeakPointer<PointerType>::CloneWeakPointer(const CloneWeakPointer<PointerType>& copy) noexcept
		: BaseClonePointer(copy.m_counter), m_pointer(copy.m_pointer), m_isImLocked(false)
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
	inline CloneWeakPointer<PointerType>::CloneWeakPointer(CloneWeakPointer<PointerType>&& move) noexcept
		: BaseClonePointer(move.m_counter, 0), m_pointer(move.m_pointer), m_isImLocked(move.m_isImLocked)
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
	inline CloneWeakPointer<PointerType>::CloneWeakPointer(const CloneSharedPointer<PointerType>& copy) noexcept
		: BaseClonePointer(copy.m_counter), m_pointer(copy.m_pointer), m_isImLocked(false)
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
	inline bool CloneWeakPointer<PointerType>::LockShared() noexcept
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
	inline bool CloneWeakPointer<PointerType>::UnlockShared() noexcept
	{
		//���b�N���Ă�
		if (IS_TRUE(m_isImLocked))
		{
			//Destroy
			GabageCollector::DestroyCloneShared(m_pointer, this);
			//return true
			return m_isImLocked = false;
		}

		return false;
	}
	//----------------------------------------------------------------------------------
	//[DynamicCast]
	//return: DynamicCast����CloneWeakPointer
	template<class PointerType>
	template<class DifferentType>
	inline CloneWeakPointer<DifferentType> CloneWeakPointer<PointerType>::DynamicCast() const noexcept
	{
		static_assert(std::is_base_of_v<PointerType, DifferentType> | std::is_base_of_v<DifferentType, PointerType>,
			"\nCloneSharedPointer::DynamicCast Failed.\nSuccess Condition : is_base_of<PointerType, DifferentType> or is_convertible<DifferentType, PointerType>");

		CloneWeakPointer<DifferentType> result;
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
	//return: DownCast����CloneWeakPointer
	template<class PointerType>
	template<class DifferentType>
	inline CloneWeakPointer<DifferentType> CloneWeakPointer<PointerType>::DownCast() const noexcept
	{
		static_assert(std::is_base_of_v<DifferentType, PointerType>,
			"\nCloneSharedPointer::DynamicCast Failed.\nSuccess Condition : is_base_of<DifferentType, PointerType>");

		CloneWeakPointer<DifferentType> result;
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
	//��Q�Ƃ���ɍ쐬����CloneSharedPointer���쐬����(SharedCount == 0�̏ꍇ��nullptr)
	//����1: �쐬����CloneSharedPointer��������
	template<class PointerType>
	inline CloneSharedPointer<PointerType> CloneWeakPointer<PointerType>::InstantiateShared() const noexcept
	{
		CloneSharedPointer<PointerType> result;

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
	//��Q�Ƃ���ɍ쐬����CloneSharedPointer���쐬����(SharedCount == 0�̏ꍇ��nullptr)
	//����1: �쐬����CloneSharedPointer��������|�C���^
	template<class PointerType>
	inline void CloneWeakPointer<PointerType>::InstantiateShared(CloneSharedPointer<PointerType>& pointer) const noexcept
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
	inline const CloneWeakPointer<PointerType>& CloneWeakPointer<PointerType>::operator=(const CloneWeakPointer<PointerType>& copy) noexcept
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
	inline CloneWeakPointer<PointerType>& CloneWeakPointer<PointerType>::operator=(CloneWeakPointer<PointerType>&& move) noexcept
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
	inline const CloneSharedPointer<PointerType>& CloneWeakPointer<PointerType>::operator=(const CloneSharedPointer<PointerType>& copy) noexcept
	{
		//�l�������R�s�[
		if (m_counter != copy.m_counter)
		{
			//��x�J��
			ReleasePointer();
			//null�łȂ���΃J�E���^���Z
			if (m_counter != nullptr)
				m_counter->addWeak();

			//copy
			m_pointer = copy.m_pointer;
			m_counter = copy.m_counter;
		}
		return copy;
	}

	//Release Pointer (Used Gabage Collector if Clone)
	template<class PointerType>
	inline void CloneWeakPointer<PointerType>::ReleasePointer() noexcept
	{
		//���b�N���Ă���J��
		if (IS_TRUE(m_isImLocked)) UnlockShared();

		//Destroy
		GabageCollector::DestroyCloneWeak(m_pointer, this);

		//nullptr
		m_pointer = nullptr;
		m_counter = nullptr;
		m_isImLocked = false;
	}



	//----------------------------------------------------------------------------------
	//CloneWeakPointer (array)
	//----------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//nullptr���������s��
	template<class PointerType>
	inline CloneWeakPointer<PointerType[]>::CloneWeakPointer() noexcept
		: BaseClonePointer(nullptr, 0), m_pointer(nullptr), m_arraySize(0), m_isImLocked(false) {}
	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//�Q�ƃR�s�[���s��
	//����1: �R�s�[��
	template<class PointerType>
	inline CloneWeakPointer<PointerType[]>::CloneWeakPointer(const CloneWeakPointer<PointerType[]>& copy) noexcept
		: BaseClonePointer(copy.m_counter), m_pointer(copy.m_pointer),
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
	//[�R���X�g���N�^]
	//���g�ւ�Move���s��
	//����1: Move��
	template<class PointerType>
	inline CloneWeakPointer<PointerType[]>::CloneWeakPointer(CloneWeakPointer<PointerType[]>&& move) noexcept
		: BaseClonePointer(move.m_counter), m_pointer(move.m_pointer),
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
	inline CloneWeakPointer<PointerType[]>::CloneWeakPointer(const CloneSharedPointer<PointerType[]>& copy) noexcept
		: BaseClonePointer(copy.m_counter), m_pointer(copy.m_pointer),
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
	inline bool CloneWeakPointer<PointerType[]>::LockShared() noexcept
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
	inline bool CloneWeakPointer<PointerType[]>::UnlockShared() noexcept
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
	//return: DynamicCast����CloneWeakPointer
	template<class PointerType>
	template<class DifferentType>
	inline CloneWeakPointer<DifferentType[]> CloneWeakPointer<PointerType[]>::DynamicCast() const noexcept
	{
		static_assert(std::is_base_of_v<PointerType, DifferentType> | std::is_base_of_v<DifferentType, PointerType>,
			"\nCloneSharedPointer::DynamicCast Failed.\nSuccess Condition : is_base_of<PointerType, DifferentType> or is_convertible<DifferentType, PointerType>");

		CloneWeakPointer<DifferentType> result;
		//dynamic cast
		DifferentType* cast = dynamic_cast<DifferentType*>(TryGetInstance());

		//not nullptr = success
		if (cast != nullptr)
		{
			result.m_pointer = cast;
			result.m_counter = m_counter;
			result.m_arraySize = m_arraySize;
			result.m_isImLocked = m_isImLocked;
			m_counter->addWeak();
		}

		return std::move(result);
	}
	//----------------------------------------------------------------------------------
	//[DownCast]
	//return: DownCast����CloneWeakPointer
	template<class PointerType>
	template<class DifferentType>
	inline CloneWeakPointer<DifferentType[]> CloneWeakPointer<PointerType[]>::DownCast() const noexcept
	{
		static_assert(std::is_base_of_v<DifferentType, PointerType>,
			"\nCloneSharedPointer::DynamicCast Failed.\nSuccess Condition : is_base_of<DifferentType, PointerType>");

		CloneWeakPointer<DifferentType> result;
		//down cast
		DifferentType* cast = static_cast<DifferentType*>(TryGetInstance());

		//not nullptr = success
		if (cast != nullptr)
		{
			result.m_pointer = cast;
			result.m_counter = m_counter;
			result.m_arraySize = m_arraySize;
			result.m_isImLocked = m_isImLocked;
			m_counter->addWeak();
		}

		return std::move(result);
	}
	//----------------------------------------------------------------------------------
	//[InstantiateShared]
	//��Q�Ƃ���ɍ쐬����CloneSharedPointer���쐬����(SharedCount == 0�̏ꍇ��nullptr)
	//����1: �쐬����CloneSharedPointer��������
	template<class PointerType>
	inline CloneSharedPointer<PointerType[]> CloneWeakPointer<PointerType[]>::InstantiateShared() const noexcept
	{
		CloneSharedPointer<PointerType> result;

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
	//��Q�Ƃ���ɍ쐬����CloneSharedPointer���쐬����(SharedCount == 0�̏ꍇ��nullptr)
	//����1: �쐬����CloneSharedPointer��������|�C���^
	template<class PointerType>
	inline void CloneWeakPointer<PointerType[]>::InstantiateShared(CloneSharedPointer<PointerType[]>& pointer) const noexcept
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
	inline const CloneWeakPointer<PointerType[]>& CloneWeakPointer<PointerType[]>::operator=(const CloneWeakPointer<PointerType[]>& copy) noexcept
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
	inline CloneWeakPointer<PointerType[]>& CloneWeakPointer<PointerType[]>::operator=(CloneWeakPointer<PointerType[]>&& move) noexcept
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
	inline const CloneSharedPointer<PointerType[]>& CloneWeakPointer<PointerType[]>::operator=(const CloneSharedPointer<PointerType[]>& copy) noexcept
	{
		//�l�������R�s�[
		if (m_counter != copy.m_counter)
		{
			//��x�J��
			ReleasePointer();
			//null�łȂ���΃J�E���^���Z
			if (m_counter != nullptr)
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
	inline void CloneWeakPointer<PointerType[]>::ReleasePointer() noexcept
	{
		//���b�N���Ă���J��
		if (IS_TRUE(m_isImLocked)) UnlockShared();

		//Destroy
		GabageCollector::DestroyCloneWeak(m_pointer, this);

		//nullptr
		m_pointer = nullptr;
		m_counter = nullptr;
		m_arraySize = 0;
		m_isImLocked = false;
	}
}
#endif // !SGFRAMEWORK_HEADER_CLONE_POINTR_HPP_