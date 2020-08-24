#ifndef SGFRAMEWORK_HEADER_BASE_POINTER_HPP_
#define SGFRAMEWORK_HEADER_BASE_POINTER_HPP_
#include <atomic>
#include "../Atomic/Atomic.hpp"

#define sgnew
#define sgdelete

//Framework namespace
namespace SGFramework
{
	class GabageCollector;

	//Pointer
	namespace Pointer
	{
		//ClonePointer->CloneMode
		enum class CloneMode : uint
		{
			//Clone Pointer->Direct
			CloneModeDirect = 0x01,
			//Clone Pointer->UniquePointer
			CloneModeUnique = 0x02,
			//Clone Pointer->SharedPointer
			CloneModeShared = 0x04,
		};
	}

	//�p�[�c
	namespace Detail
	{
		//�|�C���^�n
		namespace Pointer
		{
			//prototypes
			class BasePointer;
			class BaseCountPointer;
			struct BaseCounter;
			struct SimpleCounter;
			struct MutexCounter;
			struct BaseCloneCounter;
			struct SimpleCloneCounter;
			struct MutexCloneCounter;
			class BaseDeallocator;
			template <class PointerType, class DestroyType, class IsArray>
			class Deallocator;

			//Unique, Shared, Weak, ClonePointer��Base�ƂȂ�BasePointer class
			class BasePointer
			{
			public:
				//friend
				friend class GabageCollector;
				//Destructor
				inline virtual ~BasePointer() {}

			protected:
				//Full Setting Constructor
				inline BasePointer() {}
				//Release Pointer (Used Gabage Collector if Clone)
				virtual void ReleasePointer() = 0;
			};

			//Shared, WeakPointer��Base�ƂȂ�BaseCountPointer class
			class BaseCountPointer : protected BasePointer
			{
			public:
				//friend
				friend class GabageCollector;
				//Destructor
				inline virtual ~BaseCountPointer() {}

				//----------------------------------------------------------------------------------
				//[AddRef]
				//�Q�ƃJ�E���^�𑝂₷, memcpy�������p�����A�\�Ȍ���g���ׂ��炸
				inline virtual void AddRef() const noexcept = 0;
				
			protected:
				//Full Setting Constructor
				BaseCountPointer(BaseCounter* counter)
					: BasePointer(), m_counter(counter) {}
				//Reference Counter
				BaseCounter* m_counter = nullptr;
			};

			//CloneShared, CloneWeakPointer��Base�ƂȂ�BaseClonePointer class
			class BaseClonePointer : protected BasePointer
			{
			public:
				//friend
				friend class GabageCollector;
				//Destructor
				inline virtual ~BaseClonePointer() {}

			protected:
				//Full Setting Constructor
				BaseClonePointer(BaseCloneCounter* counter)
					: BasePointer(), m_counter(counter) {}

				//Reference Counter
				BaseCloneCounter* m_counter = nullptr;
			};

			//�Q�ƃJ�E���^�Ǝ�Q�ƃJ�E���^������BaseCounter
			struct BaseCounter
			{
				//Full Setting Constructor
				inline BaseCounter(){}
				//Destructor
				inline virtual ~BaseCounter() {}

				//get shared counter
				virtual short getShared() = 0;
				//get weak counter
				virtual short getWeak() = 0;
				//set shared counter, return shared counter
				virtual short setShared(short value) = 0;
				//set shared counter, return shared counter
				virtual short setWeak(short value) = 0;
				//add shared counter, return shared counter
				virtual short addShared() = 0;
				//sub shared counter, return shared counter
				virtual short subShared() = 0;
				//add weak counter, return weak counter
				virtual short addWeak() = 0;
				//sub weak counter, return weak counter
				virtual short subWeak() = 0;
				//lock shared counter, return success = true
				virtual bool LockShared() = 0;
				//is destroy shared counter?, yes -> return true, no -> subShared, return false
				virtual bool IsDestroyShared() = 0;
				//Destroy counter?
				virtual bool IsDestroyCounter() = 0;
			};
			//�Q�ƃJ�E���^�Ǝ�Q�ƃJ�E���^�����ANotMutex��SimpleCounter
			struct SimpleCounter : public BaseCounter
			{
				//SharedCount = 1, WeakCount = 0
				inline SimpleCounter() : BaseCounter(), m_sharedCounter(1), m_weakCounter(0), 
					m_isDestroyPointer(false), m_isDestroyCounter(false) {}

				//get shared counter
				SGF_FUNCTION_PROPERTY short getShared() override { return m_sharedCounter; }
				//get weak counter
				SGF_FUNCTION_PROPERTY short getWeak() override { return m_weakCounter; }
				//set shared counter, return shared counter
				SGF_FUNCTION_PROPERTY short setShared(short value) override { return m_sharedCounter = value; }
				//set shared counter, return shared counter
				SGF_FUNCTION_PROPERTY short setWeak(short value) override { return m_weakCounter = value; }
				//add shared counter, return shared counter
				SGF_FUNCTION_PROPERTY short addShared() override { return ++m_sharedCounter; }
				//sub shared counter, return shared counter
				SGF_FUNCTION_PROPERTY short subShared() override { return --m_sharedCounter; }
				//add weak counter, return weak counter
				SGF_FUNCTION_PROPERTY short addWeak() override { return  ++m_weakCounter; }
				//sub weak counter, return weak counter
				SGF_FUNCTION_PROPERTY short subWeak() override { return --m_weakCounter; }
				//lock shared counter, return success = true
				inline bool LockShared() override 
				{
					if (m_isDestroyPointer || m_sharedCounter <= 0)
						return false;
					else
					{
						++m_sharedCounter;
						return true;
					}		
				}
				//is destroy shared counter?, yes -> return true, no -> subShared, return false
				inline bool IsDestroyShared() override 
				{
					if (m_sharedCounter == 1 && IS_FALSE(m_isDestroyPointer))
						return m_isDestroyPointer = true;
					else
					{
						--m_sharedCounter;
						return false;
					}
				}
				//Destroy counter?
				inline bool IsDestroyCounter() override
				{
					if (m_sharedCounter == 0 && m_weakCounter == 0 && IS_FALSE(m_isDestroyCounter))
						return m_isDestroyCounter = true;
					else
						return false;
				}

			private:
				short m_sharedCounter;
				short m_weakCounter;
				bool m_isDestroyPointer;
				bool m_isDestroyCounter;
			};
			//�Q�ƃJ�E���^�Ǝ�Q�ƃJ�E���^�����AMutex��MutexCounter
			struct MutexCounter : public BaseCounter
			{
				//SharedCount = 1, WeakCount = 0
				inline MutexCounter() : BaseCounter(), m_sharedCounter(1), m_weakCounter(0), 
					m_lock(false), m_isDestroyPointer(false), m_isDestroyCounter(false) {}

				//get shared counter
				SGF_FUNCTION_PROPERTY short getShared() override { std::lock_guard<std::mutex> g(mutex); AtomicOperation::LockGuard guard(m_lock); return m_sharedCounter; }
				//get weak counter
				SGF_FUNCTION_PROPERTY short getWeak() override { std::lock_guard<std::mutex> g(mutex); AtomicOperation::LockGuard guard(m_lock); return m_weakCounter; }
				//set shared counter, return shared counter
				SGF_FUNCTION_PROPERTY short setShared(short value) override { std::lock_guard<std::mutex> g(mutex); AtomicOperation::LockGuard guard(m_lock); return m_sharedCounter = value; }
				//set shared counter, return shared counter
				SGF_FUNCTION_PROPERTY short setWeak(short value) override { std::lock_guard<std::mutex> g(mutex); AtomicOperation::LockGuard guard(m_lock); return m_weakCounter = value; }
				//add shared counter, return shared counter
				SGF_FUNCTION_PROPERTY short addShared() override { std::lock_guard<std::mutex> g(mutex); AtomicOperation::LockGuard guard(m_lock); return ++m_sharedCounter; }
				//sub shared counter, return shared counter
				SGF_FUNCTION_PROPERTY short subShared() override { std::lock_guard<std::mutex> g(mutex); AtomicOperation::LockGuard guard(m_lock); return --m_sharedCounter; }
				//add weak counter, return weak counter
				SGF_FUNCTION_PROPERTY short addWeak() override { std::lock_guard<std::mutex> g(mutex); AtomicOperation::LockGuard guard(m_lock); return ++m_weakCounter; }
				//sub weak counter, return weak counter
				SGF_FUNCTION_PROPERTY short subWeak() override { std::lock_guard<std::mutex> g(mutex); AtomicOperation::LockGuard guard(m_lock); return --m_weakCounter; }
				//lock shared counter, return success = true
				inline bool LockShared() override 
				{
					std::lock_guard<std::mutex> g(mutex);
					AtomicOperation::LockGuard guard(m_lock); 

					if (m_isDestroyPointer.load() || m_sharedCounter <= 0) 
						return false;
					else
					{
						++m_sharedCounter;
						return true;
					}

					return true;
				}
				//is destroy shared counter?, yes -> return true, no -> subShared, return false
				inline bool IsDestroyShared() override
				{
					std::lock_guard<std::mutex> g(mutex);
					AtomicOperation::LockGuard guard(m_lock); 
					
					if (m_sharedCounter == 1 && IS_FALSE(m_isDestroyPointer))
						return AtomicOperation::Init(m_isDestroyPointer, true);
					else
					{
						--m_sharedCounter;
						return false;
					}
				}
				//Destroy counter?
				inline bool IsDestroyCounter() override
				{
					std::lock_guard<std::mutex> g(mutex);
					AtomicOperation::LockGuard guard(m_lock);

					if (m_sharedCounter == 0 && m_weakCounter == 0
						&& IS_FALSE(m_isDestroyCounter.load()))
						return AtomicOperation::Init(m_isDestroyCounter, true);
					else
						return false;
				}

			private:
				std::atomic_short m_sharedCounter;
				std::atomic_short m_weakCounter;
				std::atomic_bool m_lock;
				std::mutex mutex;
				std::atomic_bool m_isDestroyPointer;
				std::atomic_bool m_isDestroyCounter;
			};


			//�Q�ƃJ�E���^�Ǝ�Q�ƃJ�E���^������Clone�pBaseCloneCounter
			struct BaseCloneCounter : public BaseCounter
			{
				//Full Setting Constructor
				inline BaseCloneCounter() : BaseCounter() {}
				//Destructor
				inline virtual ~BaseCloneCounter() {}

				//return isLinked
				virtual bool getIsLinked() = 0;
				//set isLinked value, return isLinked
				virtual bool setIsLinked(bool value) = 0;
			};
			//�Q�ƃJ�E���^�Ǝ�Q�ƃJ�E���^�����ANotMutex��SimpleCloneCounter
			struct SimpleCloneCounter : public BaseCloneCounter
			{
				//SharedCount = 1, WeakCount = 0
				inline SimpleCloneCounter(bool isLinked) : BaseCloneCounter(), m_sharedCounter(1), m_weakCounter(0),
					m_isLinked(isLinked), m_isDestroyPointer(false), m_isDestroyCounter(false) {}

				//get shared counter
				SGF_FUNCTION_PROPERTY short getShared() override { return m_sharedCounter; }
				//get weak counter
				SGF_FUNCTION_PROPERTY short getWeak() override { return m_weakCounter; }
				//set shared counter, return shared counter
				SGF_FUNCTION_PROPERTY short setShared(short value) override { return m_sharedCounter = value; }
				//set shared counter, return shared counter
				SGF_FUNCTION_PROPERTY short setWeak(short value) override { return m_weakCounter = value; }
				//add shared counter, return shared counter
				SGF_FUNCTION_PROPERTY short addShared() override { return ++m_sharedCounter; }
				//sub shared counter, return shared counter
				SGF_FUNCTION_PROPERTY short subShared() override { return --m_sharedCounter; }
				//add weak counter, return weak counter
				SGF_FUNCTION_PROPERTY short addWeak() override { return  ++m_weakCounter; }
				//sub weak counter, return weak counter
				SGF_FUNCTION_PROPERTY short subWeak() override { return --m_weakCounter; }
				//lock shared counter, return success = true
				inline bool LockShared() override 
				{
					if (m_isDestroyPointer || m_sharedCounter <= 0)
						return false;
					else
					{
						++m_sharedCounter;
						return true;
					}
				}
				//is destroy shared counter?, yes -> return true, no -> subShared, return false
				inline bool IsDestroyShared() override
				{
					if (m_sharedCounter == 1 && IS_FALSE(m_isDestroyPointer))
						return m_isDestroyPointer = true;
					else
					{
						--m_sharedCounter;
						return false;
					}
				}
				//Destroy counter?
				inline bool IsDestroyCounter() override
				{
					if (m_sharedCounter == 0 && m_weakCounter == 0 && IS_FALSE(m_isDestroyCounter))
						return m_isDestroyCounter = true;
					else
						return false;
				}

				//return isLinked
				SGF_FUNCTION_PROPERTY bool getIsLinked() override { return m_isLinked; }
				//set isLinked value, return isLinked
				SGF_FUNCTION_PROPERTY bool setIsLinked(bool value) override { return m_isLinked = value; }

			private:
				short m_sharedCounter;
				short m_weakCounter;
				bool m_isLinked;
				bool m_isDestroyPointer;
				bool m_isDestroyCounter;
			};
			//�Q�ƃJ�E���^�Ǝ�Q�ƃJ�E���^�����AMutex��MutexCloneCounter
			struct MutexCloneCounter : public BaseCloneCounter
			{
				//SharedCount = 1, WeakCount = 0
				inline MutexCloneCounter(bool isLinked) : BaseCloneCounter(), m_sharedCounter(1), m_weakCounter(0), 
					m_isLinked(isLinked), m_lock(false), m_isDestroyPointer(false), m_isDestroyCounter(false) {}
				
				//get shared counter
				SGF_FUNCTION_PROPERTY short getShared() override { AtomicOperation::LockGuard guard(m_lock); return m_sharedCounter; }
				//get weak counter
				SGF_FUNCTION_PROPERTY short getWeak() override { AtomicOperation::LockGuard guard(m_lock); return m_weakCounter; }
				//set shared counter, return shared counter
				SGF_FUNCTION_PROPERTY short setShared(short value) override { AtomicOperation::LockGuard guard(m_lock); return m_sharedCounter = value; }
				//set shared counter, return shared counter
				SGF_FUNCTION_PROPERTY short setWeak(short value) override { AtomicOperation::LockGuard guard(m_lock); return m_weakCounter = value; }
				//add shared counter, return shared counter
				SGF_FUNCTION_PROPERTY short addShared() override { AtomicOperation::LockGuard guard(m_lock); return ++m_sharedCounter; }
				//sub shared counter, return shared counter
				SGF_FUNCTION_PROPERTY short subShared() override { AtomicOperation::LockGuard guard(m_lock); return --m_sharedCounter; }
				//add weak counter, return weak counter
				SGF_FUNCTION_PROPERTY short addWeak() override { AtomicOperation::LockGuard guard(m_lock); return ++m_weakCounter; }
				//sub weak counter, return weak counter
				SGF_FUNCTION_PROPERTY short subWeak() override { AtomicOperation::LockGuard guard(m_lock); return --m_weakCounter; }
				//lock shared counter, return success = true
				inline bool LockShared() override
				{
					AtomicOperation::LockGuard guard(m_lock);

					if (m_isDestroyPointer.load() || m_sharedCounter <= 0)
						return false;
					else
					{
						++m_sharedCounter;
						return true;
					}

					return true;
				}
				//is destroy shared counter?, yes -> return true, no -> subShared, return false
				inline bool IsDestroyShared() override
				{
					AtomicOperation::LockGuard guard(m_lock);

					if (m_sharedCounter == 1 && IS_FALSE(m_isDestroyPointer))
						return AtomicOperation::Init(m_isDestroyPointer, true);
					else
					{
						--m_sharedCounter;
						return false;
					}
				}
				//Destroy counter?
				inline bool IsDestroyCounter() override
				{
					AtomicOperation::LockGuard guard(m_lock);

					if (m_sharedCounter == 0 && m_weakCounter == 0
						&& IS_FALSE(m_isDestroyCounter.load()))
						return AtomicOperation::Init(m_isDestroyCounter, true);
					else
						return false;
				}

				//return isLinked
				SGF_FUNCTION_PROPERTY bool getIsLinked() override { AtomicOperation::LockGuard guard(m_lock); return m_isLinked.load(); }
				//set isLinked value, return isLinked
				SGF_FUNCTION_PROPERTY bool setIsLinked(bool value) override { AtomicOperation::LockGuard guard(m_lock); return AtomicOperation::Init(m_isLinked, value); }


			private:
				short m_sharedCounter;
				short m_weakCounter;
				std::atomic_bool m_isLinked;
				std::atomic_bool m_lock;
				std::atomic_bool m_isDestroyPointer;
				std::atomic_bool m_isDestroyCounter;
			};

			//Deallocator�����o�錾�p�N���X
			class BaseDeallocator
			{
			public:
				//�������Ȃ�
				inline BaseDeallocator() = default;
				//�������Ȃ�
				inline virtual ~BaseDeallocator() {}

				//�|�C���^�폜
				virtual void DestoroyPointer(void* pointer) = 0;
			};
			//freede�폜����DeallocatorFree class
			class DeallocatorFree : public BaseDeallocator
			{
			public:
				//default
				DeallocatorFree() = default;
				//delete
				DeallocatorFree(const DeallocatorFree& copy) = delete;

				//�|�C���^�폜
				void DestoroyPointer(void* pointer) override
				{
					free(pointer);
				}
			};
			//�^���o���č폜����Deallocator class
			template <class PointerType, class DestroyType, class IsArray>
			class Deallocator : public BaseDeallocator
			{
			public:
				//default[
				Deallocator() = default;
				//delete
				Deallocator(const Deallocator& copy) = delete;

				//�|�C���^�폜
				inline void DestoroyPointer(void* pointer) override
				{
					if (!std::is_same_v<IsArray, std::true_type>) delete reinterpret_cast<DestroyType*>(pointer);
					else delete[] reinterpret_cast<DestroyType*>(pointer);
				}
			};
		}
	}
}
#endif // !SGFRAMEWORK_HEADER_BASE_POINTER_HPP_