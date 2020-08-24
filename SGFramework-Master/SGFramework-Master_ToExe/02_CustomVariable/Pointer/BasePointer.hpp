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

	//パーツ
	namespace Detail
	{
		//ポインタ系
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

			//Unique, Shared, Weak, ClonePointerのBaseとなるBasePointer class
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

			//Shared, WeakPointerのBaseとなるBaseCountPointer class
			class BaseCountPointer : protected BasePointer
			{
			public:
				//friend
				friend class GabageCollector;
				//Destructor
				inline virtual ~BaseCountPointer() {}

				//----------------------------------------------------------------------------------
				//[AddRef]
				//参照カウンタを増やす, memcpyした時用だが、可能な限り使うべからず
				inline virtual void AddRef() const noexcept = 0;
				
			protected:
				//Full Setting Constructor
				BaseCountPointer(BaseCounter* counter)
					: BasePointer(), m_counter(counter) {}
				//Reference Counter
				BaseCounter* m_counter = nullptr;
			};

			//CloneShared, CloneWeakPointerのBaseとなるBaseClonePointer class
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

			//参照カウンタと弱参照カウンタを持つBaseCounter
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
			//参照カウンタと弱参照カウンタを持つ、NotMutexなSimpleCounter
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
			//参照カウンタと弱参照カウンタを持つ、MutexなMutexCounter
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


			//参照カウンタと弱参照カウンタを持つClone用BaseCloneCounter
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
			//参照カウンタと弱参照カウンタを持つ、NotMutexなSimpleCloneCounter
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
			//参照カウンタと弱参照カウンタを持つ、MutexなMutexCloneCounter
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

			//Deallocatorメンバ宣言用クラス
			class BaseDeallocator
			{
			public:
				//何もしない
				inline BaseDeallocator() = default;
				//何もしない
				inline virtual ~BaseDeallocator() {}

				//ポインタ削除
				virtual void DestoroyPointer(void* pointer) = 0;
			};
			//freede削除するDeallocatorFree class
			class DeallocatorFree : public BaseDeallocator
			{
			public:
				//default
				DeallocatorFree() = default;
				//delete
				DeallocatorFree(const DeallocatorFree& copy) = delete;

				//ポインタ削除
				void DestoroyPointer(void* pointer) override
				{
					free(pointer);
				}
			};
			//型を覚えて削除するDeallocator class
			template <class PointerType, class DestroyType, class IsArray>
			class Deallocator : public BaseDeallocator
			{
			public:
				//default[
				Deallocator() = default;
				//delete
				Deallocator(const Deallocator& copy) = delete;

				//ポインタ削除
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