#ifndef SGFRAMEWORK_HEADER_GABAGE_COLLECTOR_HPP_
#define SGFRAMEWORK_HEADER_GABAGE_COLLECTOR_HPP_
#include <vector>
#include <mutex>
#include <unordered_map>
#include "../ConstAndUsing/ConstAndUsing.hpp"
#include "../VariableLimit/VariableLimit.hpp"
#include "../ReadElement/ReadElement.hpp"
#include "../Tuple/Tuple.hpp"
#include "BasePointer.hpp"

//Framework namespace
namespace SGFramework
{
	//prototype
	namespace Administrator { class GraphicsX11; }

	//削除ポイントをSync時にする->Release時vectorに追加
	//カウンタにローカル、グローバル(スレッドライン)の概念を追加 (mutex)
	//Cloneを自動更新->CloneModeの概念を追加, CloneModeがスマポならリストに追加, スマポをリストに登録(削除とは別、インスタンスリスト)して、検索をかける
	//DeleteStack追加時に削除することを明示する何かしらがポインタ側に必要

	class Application;
	template<class PointerType>
	class UniquePointer;
	template<class PointerType>
	class SharedPointer;
	template<class PointerType>
	class WeakPointer;
	template<class PointerType>
	class CloneSharedPointer;
	template<class PointerType>
	class CloneWeakPointer;

	//スマートポインタのDeleteを行うGabageCollector class
	class GabageCollector
	{
	public:
		//firend
		friend class Application;
		template<class PointerType> friend class UniquePointer;
		template<class PointerType> friend class SharedPointer;
		template<class PointerType> friend class WeakPointer;
		template<class PointerType> friend class CloneSharedPointer;
		template<class PointerType> friend class CloneWeakPointer;
		friend class Administrator::GraphicsX11;

		friend int WINAPI ::WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow);
			

		//----------------------------------------------------------------------------------
		//[ReserveManualCollection]
		//ManualDelete設定のポインタを次フレームでDeleteするように登録する
		inline static void ReserveManualCollection() { AtomicOperation::LockAtomic(m_lock); m_isManualCollectionUsedNextFrame = true; AtomicOperation::UnlockAtomic(m_lock); }

		//num stack delete pointer capacity (get function property)
		SGF_FUNCTION_PROPERTY static uint getNumDeleteCapacity() { return m_numDeleteCapacity; }
		//num 1 frame delete pointers (get function property)
		SGF_FUNCTION_PROPERTY static uint getNumDeleteOfOneFlame() { return m_numDeleteOfOneFlame; }

	private:
		//cloneSharedsに使用するCloneSharedInfos
		struct CloneSharedInfos
		{
			//Full setting constructor
			CloneSharedInfos(Detail::Pointer::BaseCloneCounter* counter, 
				void* cloneSourcePointer, uint64 copySize, byte cloneMode, bool isAutoGabageCollection)
				: counter(counter), cloneSourcePointer(cloneSourcePointer), 
				copySize(copySize), cloneMode(cloneMode), isAutoGabageCollection(isAutoGabageCollection) {}

			Detail::Pointer::BaseCloneCounter* counter;
			void* cloneSourcePointer;
			uint64 copySize;
			byte cloneMode;
			bool isAutoGabageCollection;
		};

		//ClonePointer CloneMode
		static constexpr byte m_cCloneModeDirect = 0x01;
		//ClonePointer CloneMode
		static constexpr byte m_cCloneModeUnique = 0x02;
		//ClonePointer CloneMode
		static constexpr byte m_cCloneModeShared = 0x04;
		
		//----------------------------------------------------------------------------------
		//[StartUp]
		//変数の設定を行う
		//引数1: numDeleteCapacity
		//引数2: numDeleteofOneFrame
		inline static void StartUp(const ReadElement::Pack& numDeleteCapacity, const ReadElement::Pack& numDeleteOfOneFlame);
		//----------------------------------------------------------------------------------
		//[ShutDown]
		//スタックにあるポインタを全て開放し、以降のポインタ破棄を即時に変更
		inline static void ShutDown();

		//----------------------------------------------------------------------------------
		//[GabageCollection]
		//ポインタの削除を行う, UpdateFlame初期に行うこと
		inline static void GabageCollection();
		//----------------------------------------------------------------------------------
		//[DeletePointers]
		inline static void DeletePointers(uint& deleteCounter, uint& numDeleteMax);

		//----------------------------------------------------------------------------------
		//[RegisterUnique]
		//Register Unique Pointer
		//引数1: getPointer()
		//引数2: this Deallocator
		//引数3: isAutoGabageCollection
		inline static void RegisterUnique(void* pointer, Detail::Pointer::BaseDeallocator* thisDeallocator, bool isAutoGabageCollection);
		//----------------------------------------------------------------------------------
		//[RegisterShared]
		//Register Shared Pointer
		//引数1: getPointer()
		//引数2: this Deallocator
		//引数3: this Counter
		//引数4: isAutoGabageCollection
		inline static void RegisterShared(void* pointer,
			Detail::Pointer::BaseDeallocator* thisDeallocator, Detail::Pointer::BaseCounter* thisCounter, bool isAutoGabageCollection);
		//----------------------------------------------------------------------------------
		//[RegisterCloneShared]
		//Register Clone Shared Pointer
		//引数1: getPointer()
		//引数2: Clone source pointer
		//引数3: this Deallocator
		//引数4: this Counter
		//引数5: copySize
		//引数6: cloneMode
		//引数7: isAutoGabageCollection
		inline static void RegisterCloneShared(void * pointer, void* cloneSource,
			Detail::Pointer::BaseCloneCounter* thisCounter,  uint64 copySize, 
			Pointer::CloneMode cloneMode, bool isAutoGabageCollection);

		//----------------------------------------------------------------------------------
		//[DestroyUnique]
		//Reserve Destroy Unique Pointer
		//引数1: getPointer()
		//引数2: this UniquePointer
		inline static void DestroyUnique(void* pointer, Detail::Pointer::BasePointer* thisPointer);
		//----------------------------------------------------------------------------------
		//[DestroyShared]
		//Reserve Destroy Shared Pointer
		//引数1: getPointer()
		//引数2: this SharedPointer
		inline static void DestroyShared(void* pointer, Detail::Pointer::BaseCountPointer* thisPointer);
		//----------------------------------------------------------------------------------
		//[DestroyWeak]
		//Reserve Destroy Weak Pointer
		//引数1: getPointer()
		//引数2: this WeakPointer
		inline static void DestroyWeak(void* pointer, Detail::Pointer::BaseCountPointer* thisPointer);
		//----------------------------------------------------------------------------------
		//[DestroyCloneShared]
		//Reserve Destroy Clone Shared Pointer
		//引数1: getPointer()
		//引数2: this CloneSharedPointer
		inline static void DestroyCloneShared(void* pointer, Detail::Pointer::BaseCountPointer* thisPointer);
		//----------------------------------------------------------------------------------
		//[DestroyCloneWeak]
		//Reserve Destroy Clone Weak Pointer
		//引数1: getPointer()
		//引数2: this CloneWeakPointer
		inline static void DestroyCloneWeak(void* pointer, Detail::Pointer::BaseCountPointer* thisPointer);

		//Delete Flag->AutoCollection Unique
		static constexpr byte m_cDeleteAutoUnique = 0x01;
		//Delete Flag->AutoCollection Shared
		static constexpr byte m_cDeleteAutoShared = 0x02;
		//Delete Flag->AutoCollection Clone Shared
		static constexpr byte m_cDeleteAutoCloneShared = 0x04;
		//Delete Flag->ManualCollection Unique
		static constexpr byte m_cDeleteManualUnique = 0x08;
		//Delete Flag->ManualCollection Shared
		static constexpr byte m_cDeleteManualShared = 0x10;
		//Delete Flag->ManualCollection Clone Shared
		static constexpr byte m_cDeleteManualCloneShared = 0x20;
		//Delete Flag->Weak
		static constexpr byte m_cDeleteWeak = 0x40;
		//Delete Flag ->All
		static constexpr byte m_cExcludeAll = 0xFF;
		//Delete Flag ->All Manual Flags
		static constexpr byte m_cExcludeNotManual = (m_cDeleteManualUnique | m_cDeleteManualShared | m_cDeleteManualCloneShared);

		//Unique Pointers, Tuple1: Deallocator, Tuple2: isAutoGabageCollection
		static std::unordered_map<void*, Tuple::Two<Detail::Pointer::BaseDeallocator*, bool>> m_uniques;
		//Shared Pointers, Tuple1: Deallocator, Tuple2: Counter, Tuple3: isAutoGabageCollection
		static std::unordered_map<void*, Tuple::Three<Detail::Pointer::BaseDeallocator*, Detail::Pointer::BaseCounter*, bool>> m_shareds;
		//Clone Shared Pointers
		static std::unordered_map<void*, CloneSharedInfos> m_cloneShareds;
		//Clome References
		static std::unordered_map<void*, std::unordered_set<void*>> m_cloneSharedReferences;

		//Delete AutoCollection Uniques
		static std::unordered_set<void*> m_deleteAutoUniques;
		//Delete ManualCollection Uniques
		static std::unordered_set<void*> m_deleteManualUniques;

		//Delete AutoCollection Shareds
		static std::unordered_set<void*> m_deleteAutoShareds;
		//Delete ManualCollection Shareds
		static std::unordered_set<void*> m_deleteManualShareds;
		
		//Delete AutoCollection Clone Shareds
		static std::unordered_set<void*> m_deleteAutoCloneShareds;
		//Delete ManualCollection Clone Shareds
		static std::unordered_set<void*> m_deleteManualCloneShareds;
		
		//Delete Weaks
		static std::unordered_set<Detail::Pointer::BaseCounter*> m_deleteWeaks;

		static uint m_numDeleteCapacity;			//delete capacity (Objects)
		static uint m_numDeleteOfOneFlame;		//num delete object (one frame)
		static uint m_numDeleteObjects;				//num delete stack objects
		static std::atomic_bool m_lock;					//lock
		static std::atomic_bool m_isShutDown;	//shut down flag	
		static byte m_deleteFlags;						//delete type flags
		//manual collection reserve next frame
		static bool m_isManualCollectionUsedNextFrame;
		//now gabage collection flag
		static bool m_isNowGabageCollection;
	};

	//----------------------------------------------------------------------------------
	//[StartUp]
	//変数の設定を行う
	//引数1: numDeleteCapacity
	//引数2: numDeleteofOneFrame
	inline void GabageCollector::StartUp(const ReadElement::Pack & numDeleteCapacity, const ReadElement::Pack & numDeleteOfOneFlame)
	{
		//setting values
		m_numDeleteCapacity = static_cast<uint>(numDeleteCapacity.elements[0].valueInt);
		m_numDeleteOfOneFlame = static_cast<uint>(numDeleteOfOneFlame.elements[0].valueInt);
	}
	//----------------------------------------------------------------------------------
	//[ShutDown]
	//スタックにあるポインタを全て開放し、以降のポインタ破棄を即時に変更
	inline void GabageCollector::ShutDown()
	{
		//capacity and num delete objects -> Max
		m_numDeleteCapacity = VariableLimit::cUintMax;
		m_numDeleteOfOneFlame = VariableLimit::cUintMax;

		GabageCollection();

		//shut down flags = true
		AtomicOperation::Init(m_isShutDown, true);
	}

	//----------------------------------------------------------------------------------
	//[GabageCollection]
	//ポインタの削除を行う, UpdateFlame初期に行うこと
	inline void GabageCollector::GabageCollection()
	{
		//Lock
		AtomicOperation::LockGuard guard(m_lock);

		//Start Gabage coollection
		m_isNowGabageCollection = true;
		//num deleteing objects
		uint deleteCounter = 0;
		//num delete max
		uint numDeleteMax = m_numDeleteOfOneFlame + ((m_numDeleteCapacity < deleteCounter + m_numDeleteObjects) ? deleteCounter + m_numDeleteObjects : 0);
		size_t allSize = 0;

		do
		{
			DeletePointers(deleteCounter, numDeleteMax);

			allSize = m_deleteAutoUniques.size() + m_deleteAutoShareds.size() 
				+ m_deleteAutoCloneShareds.size() + m_deleteWeaks.size();
			
			if (IS_TRUE(m_isManualCollectionUsedNextFrame))
				allSize += m_deleteManualUniques.size() + m_deleteManualShareds.size()
					+ m_deleteManualCloneShareds.size();
		} while (numDeleteMax > deleteCounter && allSize > 0);

		//Clone Update
		auto cloneReferenceEnd = m_cloneSharedReferences.end();
		for (auto& e : m_cloneShareds)
		{
			//Clone Unique & Alive Pointer -> Update
			if ((e.second.cloneMode & (m_cCloneModeUnique | m_cCloneModeShared))
				&& m_cloneSharedReferences.find(e.second.cloneSourcePointer) != cloneReferenceEnd)
				memcpy_s(e.first, e.second.copySize, e.second.cloneSourcePointer, e.second.copySize);
		}

		//Init Delete flags
		m_deleteFlags = (IS_TRUE(m_isManualCollectionUsedNextFrame) ? (m_deleteFlags & m_cExcludeNotManual) : 0);
		//Init Manual flags
		m_isManualCollectionUsedNextFrame = false;	
		//End gabage collection
		m_isNowGabageCollection = false;
	}


	//----------------------------------------------------------------------------------
	//[DeletePointers]
	inline void GabageCollector::DeletePointers(uint& deleteCounter, uint& numDeleteMax)
	{
		//Delete AutoUnique
		if (m_deleteFlags & m_cDeleteAutoUnique)
		{
			//Delete loop
			while (m_deleteAutoUniques.size() > 0)
			{
				//Delete limit -> return
				if (deleteCounter >= numDeleteMax) return;
				void* back = *(--m_deleteAutoUniques.end());

				//Cloneが存在->Release
				auto find = m_cloneSharedReferences.find(back);
				if (find != m_cloneSharedReferences.end())
				{
					auto clones = m_cloneSharedReferences.at(back);
					for (auto& clonePointer : clones)
						m_cloneShareds.at(clonePointer).counter->setIsLinked(false);

					m_cloneSharedReferences.erase(find);
				}

				//Deallocator->Delete
				auto& at = m_uniques.at(back);
				at.e1->DestoroyPointer(back);
				delete at.e1;
				//カウンタ調整
				--m_numDeleteObjects;
				++deleteCounter;

				m_uniques.erase(back);
				m_deleteAutoUniques.erase(back);
			}
			//Deleteリミット調整
			numDeleteMax = m_numDeleteOfOneFlame + ((m_numDeleteCapacity < deleteCounter + m_numDeleteObjects) ? deleteCounter + m_numDeleteObjects : 0);
		}

		//Delete AutoShared
		if (m_deleteFlags & m_cDeleteAutoShared)
		{
			//Delete loop
			while (m_deleteAutoShareds.size() > 0)
			{
				//Delete limit -> return
				if (deleteCounter >= numDeleteMax) return;
				void* back = *(--m_deleteAutoShareds.end());

				//Cloneが存在->Release
				auto find = m_cloneSharedReferences.find(back);
				if (find != m_cloneSharedReferences.end())
				{
					auto clones = m_cloneSharedReferences.at(back);
					for (auto& clonePointer : clones)
						m_cloneShareds.at(clonePointer).counter->setIsLinked(false);

					m_cloneSharedReferences.erase(find);
				}

				static std::unordered_set<void*> testSet;

				//Deallocator->Delete
				auto& at = m_shareds.at(back);
				at.e1->DestoroyPointer(back);
				at.e2->subShared();
				delete at.e1;
				//Weakが居ない->Delete counter
				if (IS_TRUE(at.e2->IsDestroyCounter()))
					delete at.e2;
				//カウンタ調整
				--m_numDeleteObjects;
				++deleteCounter;

				m_shareds.erase(back);
				m_deleteAutoShareds.erase(back);
			}
			//Deleteリミット調整
			numDeleteMax = m_numDeleteOfOneFlame + ((m_numDeleteCapacity < deleteCounter + m_numDeleteObjects) ? deleteCounter + m_numDeleteObjects : 0);
		}

		//Manual delete frame
		if (IS_TRUE(m_isManualCollectionUsedNextFrame))
		{
			//Delete ManualUnique
			if (m_deleteFlags & m_cDeleteManualUnique)
			{
				//Delete loop
				while (m_deleteManualUniques.size() > 0)
				{
					//Delete limit -> return
					if (deleteCounter >= numDeleteMax) return;
					void* back = *(--m_deleteManualUniques.end());

					//Cloneが存在->Release
					auto find = m_cloneSharedReferences.find(back);
					if (find != m_cloneSharedReferences.end())
					{
						auto clones = m_cloneSharedReferences.at(back);
						for (auto& clonePointer : clones)
							m_cloneShareds.at(clonePointer).counter->setIsLinked(false);

						m_cloneSharedReferences.erase(find);
					}

					//Deallocator->Delete
					auto& at = m_uniques.at(back);
					at.e1->DestoroyPointer(back);
					delete at.e1;
					//カウンタ調整
					--m_numDeleteObjects;
					++deleteCounter;

					m_uniques.erase(back);
					m_deleteManualUniques.erase(back);
				}
				//Deleteリミット調整
				numDeleteMax = m_numDeleteOfOneFlame + ((m_numDeleteCapacity < deleteCounter + m_numDeleteObjects) ? deleteCounter + m_numDeleteObjects : 0);
			}

			//Delete ManualShared
			if (m_deleteFlags & m_cDeleteManualShared)
			{
				//Delete loop
				while (m_deleteManualShareds.size() > 0)
				{
					//Delete limit -> return
					if (deleteCounter >= numDeleteMax) return;
					void* back = *(--m_deleteManualShareds.end());

					//Cloneが存在->Release
					auto find = m_cloneSharedReferences.find(back);
					if (find != m_cloneSharedReferences.end())
					{
						auto clones = m_cloneSharedReferences.at(back);
						for (auto& clonePointer : clones)
							m_cloneShareds.at(clonePointer).counter->setIsLinked(false);

						m_cloneSharedReferences.erase(find);
					}

					//Deallocator->Delete
					auto& at = m_shareds.at(back);
					at.e2->subShared();
					at.e1->DestoroyPointer(back);
					delete at.e1;
					//Weakが居ない->Delete counter
					if (IS_TRUE(at.e2->IsDestroyCounter()))
						delete at.e2;
					//カウンタ調整
					--m_numDeleteObjects;
					++deleteCounter;

					m_shareds.erase(back);
					m_deleteManualShareds.erase(back);
				}
				//Deleteリミット調整
				numDeleteMax = m_numDeleteOfOneFlame + ((m_numDeleteCapacity < deleteCounter + m_numDeleteObjects) ? deleteCounter + m_numDeleteObjects : 0);
			}

			//Delete ManualCloneShared
			if (m_deleteFlags & m_cDeleteManualCloneShared)
			{
				//Delete loop
				while (m_deleteManualCloneShareds.size() > 0)
				{
					//Delete limit -> return
					if (deleteCounter >= numDeleteMax) return;
					//delete pointer
					void* back = *(--m_deleteManualCloneShareds.end());;
					//infos
					auto& at = m_cloneShareds.at(back);
					at.counter->subShared();

					//free pointer
					free(back);

					//Weakが居ない->Delete counter
					if (IS_TRUE(at.counter->IsDestroyCounter()))
						delete at.counter;
					//カウンタ調整
					--m_numDeleteObjects;
					++deleteCounter;

					if (at.cloneMode & (m_cCloneModeShared | m_cCloneModeUnique))
						m_cloneSharedReferences.at(at.cloneSourcePointer).erase(back);
					m_cloneShareds.erase(back);
					m_deleteManualCloneShareds.erase(back);
				}
				//Deleteリミット調整
				numDeleteMax = m_numDeleteOfOneFlame + ((m_numDeleteCapacity < deleteCounter + m_numDeleteObjects) ? deleteCounter + m_numDeleteObjects : 0);
			}
		}

		//Delete AutoCloneShared
		if (m_deleteFlags & m_cDeleteAutoCloneShared)
		{
			//Delete loop
			while (m_deleteAutoCloneShareds.size() > 0)
			{
				//Delete limit -> return
				if (deleteCounter >= numDeleteMax) return;
				//delete pointer
				void* back = *(--m_deleteAutoCloneShareds.end());
				//infos
				auto& at = m_cloneShareds.at(back);
				at.counter->subShared();

				//free pointer
				free(back);

				//Weakが居ない->Delete counter
				if (IS_TRUE(at.counter->IsDestroyCounter()))
					delete at.counter;
				//カウンタ調整
				--m_numDeleteObjects;
				++deleteCounter;

				if (at.cloneMode & (m_cCloneModeShared | m_cCloneModeUnique))
					m_cloneSharedReferences.at(at.cloneSourcePointer).erase(back);
				m_cloneShareds.erase(back);
				m_deleteManualCloneShareds.erase(back);
			}
			//Deleteリミット調整
			numDeleteMax = m_numDeleteOfOneFlame + ((m_numDeleteCapacity < deleteCounter + m_numDeleteObjects) ? deleteCounter + m_numDeleteObjects : 0);
		}

		//Delete Weak
		if (m_deleteFlags & m_cDeleteWeak)
		{
			//Delete loop
			while (m_deleteWeaks.size() > 0)
			{
				//Delete limit -> return
				if (deleteCounter >= numDeleteMax) return;
				Detail::Pointer::BaseCounter* back = *(--m_deleteWeaks.end());

				//Delete
				delete back;
				//カウンタ調整
				--m_numDeleteObjects;
				++deleteCounter;

				m_deleteWeaks.erase(back);
			}
			//Deleteリミット調整
			numDeleteMax = m_numDeleteOfOneFlame + ((m_numDeleteCapacity < deleteCounter + m_numDeleteObjects) ? deleteCounter + m_numDeleteObjects : 0);
		}
	}

	//----------------------------------------------------------------------------------
	//[RegisterUnique]
	//Register Unique Pointer
	//引数1: getPointer()
	//引数2: this Deallocator
	//引数3: isAutoGabageCollection
	inline void GabageCollector::RegisterUnique(void* pointer, Detail::Pointer::BaseDeallocator* thisDeallocator, bool isAutoGabageCollection)
	{
		//lock mutex
		AtomicOperation::LockAtomic(m_lock);
		//リストに存在しなければemplace
		if (m_uniques.find(pointer) == m_uniques.end())
			m_uniques.try_emplace(pointer, Tuple::Two<Detail::Pointer::BaseDeallocator*, bool>(thisDeallocator, isAutoGabageCollection));
		AtomicOperation::UnlockAtomic(m_lock);
	}
	//----------------------------------------------------------------------------------
	//[RegisterShared]
	//Register Shared Pointer
	//引数1: getPointer()
	//引数2: this Deallocator
	//引数3: this Counter
	//引数4: isAutoGabageCollection
	inline void GabageCollector::RegisterShared(void* pointer, 
		Detail::Pointer::BaseDeallocator* thisDeallocator, Detail::Pointer::BaseCounter* thisCounter, bool isAutoGabageCollection)
	{
		//lock mutex
		AtomicOperation::LockAtomic(m_lock);
		//リストに存在しなければemplace
		if (m_shareds.find(pointer) == m_shareds.end())
			m_shareds.try_emplace(pointer, Tuple::Three<Detail::Pointer::BaseDeallocator*, Detail::Pointer::BaseCounter*, bool>(
				thisDeallocator, thisCounter, isAutoGabageCollection));
		AtomicOperation::UnlockAtomic(m_lock);
	}
	//----------------------------------------------------------------------------------
	//[RegisterCloneShared]
	//Register Clone Shared Pointer
	//引数1: getPointer()
	//引数2: Clone source pointer
	//引数3: this Deallocator
	//引数4: this Counter
	//引数5: copySize
	//引数6: cloneMode
	//引数7: isAutoGabageCollection
	inline void GabageCollector::RegisterCloneShared(void * pointer, void* cloneSource,
		Detail::Pointer::BaseCloneCounter* thisCounter, uint64 copySize,
		Pointer::CloneMode cloneMode, bool isAutoGabageCollection)
	{
		//lock mutex
		AtomicOperation::LockAtomic(m_lock);
		//リストに存在しなければemplace
		if (m_cloneShareds.find(pointer) == m_cloneShareds.end())
			m_cloneShareds.try_emplace(pointer, CloneSharedInfos(thisCounter,
				cloneSource, copySize, static_cast<byte>(cloneMode), isAutoGabageCollection));
	//リストに存在しなければemplace (reference)
		if (m_cloneSharedReferences.find(cloneSource) == m_cloneSharedReferences.end())
			m_cloneSharedReferences.try_emplace(cloneSource, std::unordered_set<void*>());
		//Register Reference
		m_cloneSharedReferences.at(cloneSource).emplace(pointer);
		AtomicOperation::UnlockAtomic(m_lock);
	}


	//----------------------------------------------------------------------------------
	//[DestroyUnique]
	//Reserve Destroy Unique Pointer
	//引数1: getPointer()
	//引数2: this UniquePointer
	inline void GabageCollector::DestroyUnique(void* pointer, Detail::Pointer::BasePointer* thisPointer)
	{
		if (pointer == nullptr) return;
	
		//Shutdown実行済みなら即時delete
		if (IS_TRUE(m_isShutDown))
		{
			AtomicOperation::LockGuard lock(m_lock);
			m_uniques.at(pointer).e1->DestoroyPointer(pointer);
			delete m_uniques.at(pointer).e1;
		
			return;
		}

		//GabageCollection実行中でなければLock mutex
		if (IS_FALSE(m_isNowGabageCollection)) AtomicOperation::LockAtomic(m_lock);
		
		//AutoCollection
		if (IS_TRUE(m_uniques.at(pointer).e2))
		{
			//emplace, addFlag
			m_deleteAutoUniques.emplace(pointer);
			m_deleteFlags |= m_cDeleteAutoUnique;
		}
		//ManualCollection
		else
		{
			//emplace, addFlag
			m_deleteManualUniques.emplace(pointer);
			m_deleteFlags |= m_cDeleteManualUnique;
		}
		//カウンタ加算
		++m_numDeleteObjects;

		//GabageCollection実行中でなければUnlock mutex
		if (IS_FALSE(m_isNowGabageCollection)) AtomicOperation::UnlockAtomic(m_lock);
	}
	//----------------------------------------------------------------------------------
	//[DestroyShared]
	//Reserve Destroy Shared Pointer
	//引数1: getPointer()
	//引数2: this SharedPointer
	inline void GabageCollector::DestroyShared(void* pointer, Detail::Pointer::BaseCountPointer* thisPointer)
	{
		if (pointer == nullptr || thisPointer->m_counter == nullptr) return;
		
		//Shutdown実行済みなら即時delete
		if (IS_TRUE(m_isShutDown))
		{
			AtomicOperation::LockGuard lock(m_lock);
			auto& at = m_shareds.at(pointer);

			//get counter, not deleted->return
			if (IS_FALSE(at.e2->IsDestroyShared()))
				return;

			//Used Deallocator
			at.e1->DestoroyPointer(pointer);
			delete at.e1;
			at.e2->subShared();

			//Used Counter
			if (IS_TRUE(at.e2->IsDestroyCounter()))
				delete at.e2;
			
			return;
		}

		//sub counter, not deleted->return
		if (IS_FALSE(thisPointer->m_counter->IsDestroyShared())) return;

		//Lock 
		AtomicOperation::LockAtomic(m_lock);

		//AutoCollection
		if (IS_TRUE(m_shareds.at(pointer).e3))
		{
			//emplace, addFlag
			m_deleteAutoShareds.emplace(pointer);
			m_deleteFlags |= m_cDeleteAutoShared;
		}
		//ManualCollection
		else
		{
			//emplace, addFlag
			m_deleteManualShareds.emplace(pointer);
			m_deleteFlags |= m_cDeleteManualShared;
		}
		//カウンタ加算
		++m_numDeleteObjects;
		
		//Unlock
		AtomicOperation::UnlockAtomic(m_lock);
	}
	//----------------------------------------------------------------------------------
	//[DestroyWeak]
	//Reserve Destroy Weak Pointer
	//引数1: getPointer()
	//引数2: this WeakPointer
	inline void GabageCollector::DestroyWeak(void * pointer, Detail::Pointer::BaseCountPointer * thisPointer)
	{
		if (pointer == nullptr || thisPointer->m_counter == nullptr) return;

		//Shutdown実行済みなら即時delete
		if (IS_TRUE(m_isShutDown))
		{
			AtomicOperation::LockGuard lock(m_lock);
			//sub counter
			thisPointer->m_counter->subWeak();
			
			//delete?
			if (IS_TRUE(thisPointer->m_counter->IsDestroyCounter()))
				delete thisPointer->m_counter;
			return;
		}
		
		//sub counter
		auto nowWeakCount = thisPointer->m_counter->subWeak();

		//lock
		AtomicOperation::LockGuard guard(m_lock);

		//not deleted->return
		if (IS_FALSE(thisPointer->m_counter->IsDestroyCounter())) return;

		//emplace, addFlag
		if (m_deleteWeaks.find(thisPointer->m_counter) == m_deleteWeaks.end())
			m_deleteWeaks.emplace(thisPointer->m_counter);
		m_deleteFlags |= m_cDeleteWeak;
		//カウンタ加算
		++m_numDeleteObjects;
	}
	//----------------------------------------------------------------------------------
	//[DestroyCloneShared]
	//Reserve Destroy Clone Shared Pointer
	//引数1: getPointer()
	//引数2: this CloneSharedPointer
	inline void GabageCollector::DestroyCloneShared(void * pointer, Detail::Pointer::BaseCountPointer * thisPointer)
	{
		if (pointer == nullptr || thisPointer->m_counter == nullptr) return;

		//Shutdown実行済みなら即時delete
		if (IS_TRUE(m_isShutDown))
		{
			AtomicOperation::LockGuard lock(m_lock);
			auto& at = m_cloneShareds.at(pointer);

			//get counter, not deleted->return
			if (IS_FALSE(at.counter->IsDestroyShared()))
				return;

			//Free
			free(pointer);
			at.counter->subShared();

			//Used Counter
			if (IS_TRUE(at.counter->IsDestroyCounter()))
				delete at.counter;

			return;
		}

		//sub counter, not deleted->return
		if (IS_FALSE(thisPointer->m_counter->IsDestroyShared())) return;

		//Lock 
		AtomicOperation::LockAtomic(m_lock);

		//AutoCollection
		if (IS_TRUE(m_cloneShareds.at(pointer).isAutoGabageCollection))
		{
			//emplace, addFlag
			m_deleteAutoCloneShareds.emplace(pointer);
			m_deleteFlags |= m_cDeleteAutoCloneShared;
		}
		//ManualCollection
		else
		{
			//emplace, addFlag
			m_deleteAutoCloneShareds.emplace(pointer);
			m_deleteFlags |= m_cDeleteAutoCloneShared;
		}
		//カウンタ加算
		++m_numDeleteObjects;

		//Unlock
		AtomicOperation::UnlockAtomic(m_lock);
	}
	//----------------------------------------------------------------------------------
	//[DestroyCloneWeak]
	//Reserve Destroy Clone Weak Pointer
	//引数1: getPointer()
	//引数2: this CloneWeakPointer
	inline void GabageCollector::DestroyCloneWeak(void * pointer, Detail::Pointer::BaseCountPointer * thisPointer)
	{
		if (pointer == nullptr || thisPointer->m_counter == nullptr) return;
	
		//Shutdown実行済みなら即時delete
		if (IS_TRUE(m_isShutDown))
		{
			AtomicOperation::LockGuard lock(m_lock);
			//sub counter
			thisPointer->m_counter->subWeak();

			if (IS_TRUE(thisPointer->m_counter->IsDestroyCounter()))
				delete thisPointer->m_counter;
			return;
		}

		//sub counter
		auto nowWeakCount = thisPointer->m_counter->subWeak();

		//lock
		AtomicOperation::LockGuard guard(m_lock);

		//not deleted->return
		if (IS_FALSE(thisPointer->m_counter->IsDestroyCounter())) return;

		//emplace, addFlag
		if (m_deleteWeaks.find(thisPointer->m_counter) == m_deleteWeaks.end())
			m_deleteWeaks.emplace(thisPointer->m_counter);
		m_deleteFlags |= m_cDeleteWeak;
		//カウンタ加算
		++m_numDeleteObjects;
	}
}
#endif