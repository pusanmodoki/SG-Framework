/*----------------------------------------------------------------------------------
//各ポインタをコピーし、参照カウンタを持ちカウンタ == 0で自動的にdeleteされるCloneSharedPointer class
//弱参照カウンタを持つCloneWeakPointer class
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

	//参照カウンタを持ちカウンタ == 0で自動的にdeleteされるCloneSharedPointer class
	template<class PointerType>
	class CloneSharedPointer;
	//弱参照カウンタを持つCloneWeakPointer class
	template<class PointerType>
	class CloneWeakPointer;

	//各ポインタをコピーし、参照カウンタを持ちカウンタ == 0で自動的にdeleteされるCloneSharedPointer class
	template<class PointerType>
	class CloneSharedPointer final : public Detail::Pointer::BaseClonePointer
	{
	public:
		//friend
		friend class CloneWeakPointer<PointerType>;
		template<class NewClass> friend class CloneSharedPointer;
		template<class NewClass> friend class CloneWeakPointer;

		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//nullptr初期化を行う
		inline CloneSharedPointer() noexcept;
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//参照コピーを行う
		//引数1: コピー元
		inline CloneSharedPointer(const CloneSharedPointer<PointerType>& copy) noexcept;
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//自身へのMoveを行う
		//引数1: Move元
		inline CloneSharedPointer(CloneSharedPointer<PointerType>&& move) noexcept;
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//初期化を行う
		//引数1: clone source pointer (unused AutoUpdate)
		//引数2: Used mutex counter (Required for Multi thread), default = true
		//引数3: Auto gabage collection?, default = true
		template<class NewType>
		inline explicit CloneSharedPointer(NewType* cloneSourcePointer,
			bool isUsedMutexCounter = true, bool isAutoGabageCollection = true) noexcept;
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//初期化を行う
		//引数1: clone source pointer (used AutoUpdate)
		//引数2: Used mutex counter (Required for Multi thread), default = true
		//引数3: Auto gabage collection?, default = true
		template<class NewType>
		inline explicit CloneSharedPointer(const UniquePointer<NewType>& cloneSourcePointer,
			bool isUsedMutexCounter = true, bool isAutoGabageCollection = true) noexcept;
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//初期化を行う
		//引数1: clone source pointer (used AutoUpdate)
		//引数2: Used mutex counter (Required for Multi thread), default = true
		//引数3: Auto gabage collection?, default = true
		template<class NewType>
		inline explicit CloneSharedPointer(const SharedPointer<NewType>& cloneSourcePointer,
			bool isUsedMutexCounter = true, bool isAutoGabageCollection = true) noexcept;
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//初期化を行う
		//引数1: clone source pointer (used AutoUpdate)
		//引数2: Used mutex counter (Required for Multi thread), default = true
		//引数3: Auto gabage collection?, default = true
		template<class NewType>
		inline explicit CloneSharedPointer(const WeakPointer<NewType>& cloneSourcePointer,
			bool isUsedMutexCounter = true, bool isAutoGabageCollection = true) noexcept;
		//----------------------------------------------------------------------------------
		//[デストラクタ]
		//メンバ変数のデストラクタによりカウンタの計算や開放を行う
		inline ~CloneSharedPointer() noexcept { ReleasePointer(); }

		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//内部情報を更新する, 注意！Direct->Cloneは自動Updateが行われません
		//作成時のコピーサイズを基にコピーするので、クラスの継承次第ではクラッシュします。注意。
		//return: 成功したらtrue, 自身 or copyがnullptrならfalse
		//引数1: copy value
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
		//内部情報を更新する, 注意！Direct->Cloneは自動Updateが行われません
		//作成時のコピーサイズを基にコピーするので、クラスの継承次第ではクラッシュします。注意。
		//return: 成功したらtrue, 自身 or copyがnullptrならfalse
		//引数1: copy value
		template<class DifferentType>
		inline bool UpdateManual(const DifferentType* copy) const noexcept
		{
			static_assert(std::is_convertible_v<DifferentType, PointerType>,
				"\nClonePointer->Update Failed.\nCondition : is_convertible_v<DifferentType, PointerType>");
			return UpdateManual(static_cast<PointerType*>(copy));
		}
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//内部情報を更新する, 注意！Unique->Cloneは自動UpdateがSync時に行われています
		//作成時のコピーサイズを基にコピーするので、クラスの継承次第ではクラッシュします。注意。
		//return: 成功したらtrue, 自身 or copyがnullptrならfalse
		//引数1: copy value
		inline bool UpdateManual(const UniquePointer<PointerType>& copy) const noexcept { return UpdateManual(copy.getPointer()); }
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//内部情報を更新する, 注意！Unique->Cloneは自動UpdateがSync時に行われています
		//作成時のコピーサイズを基にコピーするので、クラスの継承次第ではクラッシュします。注意。
		//return: 成功したらtrue, 自身 or copyがnullptrならfalse
		//引数1: copy value
		template<class DifferentType>
		inline bool UpdateManual(const UniquePointer<DifferentType>& copy) const noexcept
		{
			static_assert(std::is_convertible_v<DifferentType, PointerType>,
				"\nClonePointer->UpdateManual Failed.\nCondition : is_convertible_v<DifferentType, PointerType>");
			return UpdateManual(static_cast<PointerType*>(copy.getPointer()));
		}
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//内部情報を更新する, 注意！Shared->Cloneは自動UpdateがSync時に行われています
		//dynamic_cast検証は行わないため、安全性を確保するためにsizeof(PointerType)しかコピーしません
		//return: 成功したらtrue, 自身 or copyがnullptrならfalse
		//引数1: copy value
		inline bool UpdateManual(const SharedPointer<PointerType>& copy) const noexcept { return UpdateManual(copy.getPointer()); }
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//内部情報を更新する, 注意！Shared->Cloneは自動UpdateがSync時に行われています
		//作成時のコピーサイズを基にコピーするので、クラスの継承次第ではクラッシュします。注意。
		//return: 成功したらtrue, 自身 or copyがnullptrならfalse
		//引数1: copy value
		template<class DifferentType>
		inline bool UpdateManual(const SharedPointer<DifferentType>& copy) const noexcept
		{
			static_assert(std::is_convertible_v<DifferentType, PointerType>,
				"\nClonePointer->UpdateManual Failed.\nCondition : is_convertible_v<DifferentType, PointerType>");
			return UpdateManual(static_cast<PointerType*>(copy.getPointer()));
		}
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//内部情報を更新する, 注意！Weak(Shared)->Cloneは自動UpdateがSync時に行われています
		//作成時のコピーサイズを基にコピーするので、クラスの継承次第ではクラッシュします。注意。
		//return: 成功したらtrue, 自身 or copyがnullptrならfalse
		//引数1: copy value
		inline bool UpdateManual(WeakPointer<PointerType> copy) const noexcept
		{
			LockWeakGuard<WeakPointer<PointerType>> guard(copy);
			if (guard) return UpdateManual(copy.TryGetInstance()); 
			else return false;
		}
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//内部情報を更新する, 注意！Weak(Shared)->Cloneは自動UpdateがSync時に行われています
		//作成時のコピーサイズを基にコピーするので、クラスの継承次第ではクラッシュします。注意。
		//return: 成功したらtrue, 自身 or copyがnullptrならfalse
		//引数1: copy value
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
		//内部情報を更新する, 注意！Clone->Cloneは自動Updateが行われません
		//作成時のコピーサイズを基にコピーするので、クラスの継承次第ではクラッシュします。注意。
		//return: 成功したらtrue, 自身 or copyがnullptrならfalse
		//引数1: copy value
		inline bool UpdateManual(const CloneSharedPointer<PointerType>& copy) const noexcept { return UpdateManual(copy.getPointer()); }
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//内部情報を更新する, 注意！Clone->Cloneは自動Updateが行われません
		//作成時のコピーサイズを基にコピーするので、クラスの継承次第ではクラッシュします。注意。
		//return: 成功したらtrue, 自身 or copyがnullptrならfalse
		//引数1: copy value
		template<class DifferentType>
		inline bool UpdateManual(const CloneSharedPointer<DifferentType>& copy) const noexcept
		{
			static_assert(std::is_convertible_v<DifferentType, PointerType>,
				"\nClonePointer->Update Failed.\nCondition : is_convertible_v<DifferentType, PointerType>");
			return UpdateManual(static_cast<PointerType*>(copy.getPointer()));
		}
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//内部情報を更新する, 注意！Clone->Cloneは自動Updateが行われません
		//作成時のコピーサイズを基にコピーするので、クラスの継承次第ではクラッシュします。注意。
		//return: 成功したらtrue, 自身 or copyがnullptrならfalse
		//引数1: copy value
		inline bool UpdateManual(CloneWeakPointer<PointerType> copy) const noexcept;
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//内部情報を更新する, 注意！Clone->Cloneは自動Updateが行われません
		//作成時のコピーサイズを基にコピーするので、クラスの継承次第ではクラッシュします。注意。
		//return: 成功したらtrue, 自身 or copyがnullptrならfalse
		//引数1: copy value
		template<class DifferentType>
		inline bool UpdateManual(CloneWeakPointer<DifferentType> copy) const noexcept;

		//----------------------------------------------------------------------------------
		//[AddRef]
		//参照カウンタを増やす, memcpyした時用だが、可能な限り使うべからず
		inline void AddRef() const noexcept { if (m_counter) m_counter->addShared(); }
		//----------------------------------------------------------------------------------
		//[WaiverAndReset]
		//ポインタの所有権を放棄し自身をnullptrに初期化する
		inline void WaiverAndReset() noexcept { ReleasePointer(); }
		//----------------------------------------------------------------------------------
		//[DynamicCast]
		//return: DynamicCastしたCloneSharedPointer
		template<class DifferentType>
		inline CloneSharedPointer<DifferentType> DynamicCast() const noexcept;
		//----------------------------------------------------------------------------------
		//[DownCast]
		//return: DownCastしたCloneSharedPointer
		template<class DifferentType>
		inline CloneSharedPointer<DifferentType> DownCast() const noexcept;
		
		//this raw pointer, not delete!! (get function property)
		SGF_FUNCTION_PROPERTY PointerType* getPointer() const noexcept { return m_pointer; }
		//shared count (get function property)
		SGF_FUNCTION_PROPERTY ushort getSharedCount() const noexcept { return m_counter ? m_counter->getShared() : 0; }
		//weak count (get function property)
		SGF_FUNCTION_PROPERTY ushort getWeakCount() const noexcept { return m_counter ? m_counter->getWeak() : 0; }
		//pointer is valid(有効)? (get function property)
		SGF_FUNCTION_PROPERTY bool getIsValid() const noexcept { return m_pointer ? true : false; }
		//pointer is linked(自動同期が有効)? (get function property)
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

		//ポインタ
		PointerType* m_pointer;
	};

	//参照カウンタを持ちカウンタ == 0で自動的にdeleteされるCloneSharedPointer class [Array]
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
		//[コンストラクタ]
		//nullptr初期化を行う
		inline CloneSharedPointer() noexcept;
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//参照コピーを行う
		//引数1: コピー元
		inline CloneSharedPointer(const CloneSharedPointer<PointerType[]>& copy) noexcept;
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//自身へのMoveを行う
		//引数1: Move元
		inline CloneSharedPointer(CloneSharedPointer<PointerType[]>&& move) noexcept;
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//初期化を行う
		//引数1: clone source pointer
		//引数2: 配列サイズ
		//引数3: Used mutex counter (Required for Multi thread), default = true
		//引数4: Auto gabage collection?, default = true
		template<class NewType>
		inline explicit CloneSharedPointer(NewType* cloneSourcePointer, ushort arraySize,
			bool isUsedMutexCounter = true, bool isAutoGabageCollection = true) noexcept;
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//初期化を行う
		//引数1: clone source pointer
		//引数2: Used mutex counter (Required for Multi thread), default = true
		//引数3: Auto gabage collection?, default = true
		template<class NewType>
		inline explicit CloneSharedPointer(const UniquePointer<NewType[]>& cloneSourcePointer,
			bool isUsedMutexCounter = true, bool isAutoGabageCollection = true) noexcept;
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//初期化を行う
		//引数1: clone source pointer
		//引数2: Used mutex counter (Required for Multi thread), default = true
		//引数3: Auto gabage collection?, default = true
		template<class NewType>
		inline explicit CloneSharedPointer(const SharedPointer<NewType[]>& cloneSourcePointer,
			bool isUsedMutexCounter = true, bool isAutoGabageCollection = true) noexcept;
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//初期化を行う
		//引数1: clone source pointer
		//引数2: Used mutex counter (Required for Multi thread), default = true
		//引数3: Auto gabage collection?, default = true
		template<class NewType>
		inline explicit CloneSharedPointer(const WeakPointer<NewType[]>& cloneSourcePointer,
			bool isUsedMutexCounter = true, bool isAutoGabageCollection = true) noexcept;
		//----------------------------------------------------------------------------------
		//[デストラクタ]
		//メンバ変数のデストラクタによりカウンタの計算や開放を行う
		inline ~CloneSharedPointer() noexcept { ReleasePointer(); }


		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//内部情報を更新する, 注意！Direct->Cloneは自動Updateが行われません
		//作成時のコピーサイズを基にコピーするので、クラスの継承次第ではクラッシュします。注意。
		//return: 成功したらtrue, 自身 or copyがnullptrならfalse
		//引数1: copy value
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
		//内部情報を更新する, 注意！Direct->Cloneは自動Updateが行われません
		//作成時のコピーサイズを基にコピーするので、クラスの継承次第ではクラッシュします。注意。
		//return: 成功したらtrue, 自身 or copyがnullptrならfalse
		//引数1: copy value
		template<class DifferentType>
		inline bool UpdateManual(const DifferentType* copy) const noexcept
		{
			static_assert(std::is_convertible_v<DifferentType, PointerType>,
				"\nClonePointer->UpdateManual Failed.\nCondition : is_convertible_v<DifferentType, PointerType>");
			return Update(static_cast<PointerType*>(copy));
		}
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//内部情報を更新する, 注意！Unique->Cloneは自動UpdateがSync時に行われています
		//作成時のコピーサイズを基にコピーするので、クラスの継承次第ではクラッシュします。注意。
		//return: 成功したらtrue, 自身 or copyがnullptrならfalse
		//引数1: copy value
		inline bool UpdateManual(const UniquePointer<PointerType[]>& copy) const noexcept { return UpdateManual(copy.getPointer()); }
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//内部情報を更新する, 注意！Unique->Cloneは自動UpdateがSync時に行われています
		//作成時のコピーサイズを基にコピーするので、クラスの継承次第ではクラッシュします。注意。
		//return: 成功したらtrue, 自身 or copyがnullptrならfalse
		//引数1: copy value
		template<class DifferentType>
		inline bool UpdateManual(const UniquePointer<DifferentType[]>& copy) const noexcept
		{
			static_assert(std::is_convertible_v<DifferentType, PointerType>,
				"\nClonePointer->UpdateManual Failed.\nCondition : is_convertible_v<DifferentType, PointerType>");
			return Update(static_cast<PointerType*>(copy.getPointer()));
		}
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//内部情報を更新する, 注意！Shared->Cloneは自動UpdateがSync時に行われています
		//作成時のコピーサイズを基にコピーするので、クラスの継承次第ではクラッシュします。注意。
		//return: 成功したらtrue, 自身 or copyがnullptrならfalse
		//引数1: copy value
		inline bool UpdateManual(const SharedPointer<PointerType[]>& copy) const noexcept { return UpdateManual(copy.getPointer()); }
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//内部情報を更新する, 注意！Shared->Cloneは自動UpdateがSync時に行われています
		//作成時のコピーサイズを基にコピーするので、クラスの継承次第ではクラッシュします。注意。
		//return: 成功したらtrue, 自身 or copyがnullptrならfalse
		//引数1: copy value
		template<class DifferentType>
		inline bool UpdateManual(const SharedPointer<DifferentType[]>& copy) const noexcept
		{
			static_assert(std::is_convertible_v<DifferentType, PointerType>,
				"\nClonePointer->Update Failed.\nCondition : is_convertible_v<DifferentType, PointerType>");
			return UpdateManual(static_cast<PointerType*>(copy.getPointer()));
		}
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//内部情報を更新する, 注意！Weak(Shared)->Cloneは自動UpdateがSync時に行われています
		//作成時のコピーサイズを基にコピーするので、クラスの継承次第ではクラッシュします。注意。
		//return: 成功したらtrue, 自身 or copyがnullptrならfalse
		//引数1: copy value
		inline bool UpdateManual(WeakPointer<PointerType[]> copy) const noexcept
		{
			LockWeakGuard<WeakPointer<PointerType>> guard(copy);
			if (guard) return UpdateManual(copy.TryGetInstance());
			else return false;
		}
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//内部情報を更新する, 注意！Weak(Shared)->Cloneは自動UpdateがSync時に行われています
		//作成時のコピーサイズを基にコピーするので、クラスの継承次第ではクラッシュします。注意。
		//return: 成功したらtrue, 自身 or copyがnullptrならfalse
		//引数1: copy value
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
		//内部情報を更新する, 注意！Clone->Cloneは自動Updateが行われません
		//作成時のコピーサイズを基にコピーするので、クラスの継承次第ではクラッシュします。注意。
		//return: 成功したらtrue, 自身 or copyがnullptrならfalse
		//引数1: copy value
		inline bool UpdateManual(const CloneSharedPointer<PointerType[]>& copy) const noexcept { return UpdateManual(copy.getPointer()); }
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//内部情報を更新する, 注意！Clone->Cloneは自動Updateが行われません
		//作成時のコピーサイズを基にコピーするので、クラスの継承次第ではクラッシュします。注意。
		//return: 成功したらtrue, 自身 or copyがnullptrならfalse
		//引数1: copy value
		template<class DifferentType>
		inline bool UpdateManual(const CloneSharedPointer<DifferentType[]>& copy) const noexcept
		{
			static_assert(std::is_convertible_v<DifferentType, PointerType>,
				"\nClonePointer->Update Failed.\nCondition : is_convertible_v<DifferentType, PointerType>");
			return UpdateManual(static_cast<PointerType*>(copy.getPointer()));
		}
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//内部情報を更新する, 注意！Clone->Cloneは自動Updateが行われません
		//作成時のコピーサイズを基にコピーするので、クラスの継承次第ではクラッシュします。注意。
		//return: 成功したらtrue, 自身 or copyがnullptrならfalse
		//引数1: copy value
		inline bool UpdateManual(CloneWeakPointer<PointerType[]> copy) const noexcept;
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//内部情報を更新する, 注意！Clone->Cloneは自動Updateが行われません
		//作成時のコピーサイズを基にコピーするので、クラスの継承次第ではクラッシュします。注意。
		//return: 成功したらtrue, 自身 or copyがnullptrならfalse
		//引数1: copy value
		template<class DifferentType>
		inline bool UpdateManual(CloneWeakPointer<DifferentType[]> copy) const noexcept;

		//----------------------------------------------------------------------------------
		//[AddRef]
		//参照カウンタを増やす, memcpyした時用だが、可能な限り使うべからず
		inline void AddRef() const noexcept { if (m_counter) m_counter->addShared(); }
		//----------------------------------------------------------------------------------
		//[WaiverAndReset]
		//ポインタの所有権を放棄し自身をnullptrに初期化する
		inline void WaiverAndReset() noexcept { ReleasePointer(); }
		//----------------------------------------------------------------------------------
		//[DynamicCast]
		//return: DynamicCastしたCloneSharedPointer
		template<class DifferentType>
		inline CloneSharedPointer<DifferentType[]> DynamicCast() const noexcept;
		//----------------------------------------------------------------------------------
		//[DownCast]
		//return: DownCastしたCloneSharedPointer
		template<class DifferentType>
		inline CloneSharedPointer<DifferentType[]> DownCast() const noexcept;
		
		//this raw pointer, not delete!! (get function property)
		SGF_FUNCTION_PROPERTY PointerType* getPointer() const noexcept { return m_pointer; }
		//shared count (get function property)
		SGF_FUNCTION_PROPERTY ushort getSharedCount() const noexcept { return m_counter ? m_counter->getShared() : 0; }
		//weak count (get function property)
		SGF_FUNCTION_PROPERTY ushort getWeakCount() const noexcept { return m_counter ? m_counter->getWeak() : 0; }
		//pointer is valid(有効)? (get function property)
		SGF_FUNCTION_PROPERTY bool getIsValid() const noexcept { return m_pointer ? true : false; }
		//pointer is linked(自動同期が有効)? (get function property)
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
		
		//ポインタ
		PointerType* m_pointer;
		//ArraySize
		ushort m_arraySize;
	};


	//弱参照カウンタを持つCloneWeakPointer class
	template<class PointerType>
	class CloneWeakPointer final : public Detail::Pointer::BaseClonePointer
	{
	public:
		//friend
		friend class CloneSharedPointer<PointerType>;
		template<class NewClass> friend class CloneSharedPointer;
		template<class NewClass> friend class CloneWeakPointer;

		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//nullptr初期化を行う
		inline CloneWeakPointer() noexcept;
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//参照コピーを行う
		//引数1: コピー元
		inline CloneWeakPointer(const CloneWeakPointer<PointerType>& copy) noexcept;
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//自身へのMoveを行う
		//引数1: Move元
		inline CloneWeakPointer(CloneWeakPointer<PointerType>&& move) noexcept;
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//参照コピーを行う
		//引数1: コピー元<Shared>
		inline CloneWeakPointer(const CloneSharedPointer<PointerType>& copy) noexcept;
		//----------------------------------------------------------------------------------
		//[デストラクタ]
		//メンバ変数のデストラクタによりカウンタの計算や開放を行う
		inline ~CloneWeakPointer() noexcept { ReleasePointer(); }


		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//内部情報を更新する, 注意！Direct->Cloneは自動Updateが行われません
		//作成時のコピーサイズを基にコピーするので、クラスの継承次第ではクラッシュします。注意。
		//return: 成功したらtrue, 自身 or copyがnullptrならfalse
		//引数1: copy value
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
		//内部情報を更新する, 注意！Direct->Cloneは自動Updateが行われません
		//作成時のコピーサイズを基にコピーするので、クラスの継承次第ではクラッシュします。注意。
		//return: 成功したらtrue, 自身 or copyがnullptrならfalse
		//引数1: copy value
		template<class DifferentType>
		inline bool UpdateManual(const DifferentType* copy) const noexcept
		{
			static_assert(std::is_convertible_v<DifferentType, PointerType>,
				"\nClonePointer->Update Failed.\nCondition : is_convertible_v<DifferentType, PointerType>");
			return UpdateManual(static_cast<PointerType*>(copy));
		}
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//内部情報を更新する, 注意！Unique->Cloneは自動UpdateがSync時に行われています
		//作成時のコピーサイズを基にコピーするので、クラスの継承次第ではクラッシュします。注意。
		//return: 成功したらtrue, 自身 or copyがnullptrならfalse
		//引数1: copy value
		inline bool UpdateManual(const UniquePointer<PointerType>& copy) const noexcept { return UpdateManual(copy.getPointer()); }
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//内部情報を更新する, 注意！Unique->Cloneは自動UpdateがSync時に行われています
		//作成時のコピーサイズを基にコピーするので、クラスの継承次第ではクラッシュします。注意。
		//return: 成功したらtrue, 自身 or copyがnullptrならfalse
		//引数1: copy value
		template<class DifferentType>
		inline bool UpdateManual(const UniquePointer<DifferentType>& copy) const noexcept
		{
			static_assert(std::is_convertible_v<DifferentType, PointerType>,
				"\nClonePointer->Update Failed.\nCondition : is_convertible_v<DifferentType, PointerType>");
			return Update(static_cast<PointerType*>(copy.getPointer()));
		}
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//内部情報を更新する, 注意！Shared->Cloneは自動UpdateがSync時に行われています
		//作成時のコピーサイズを基にコピーするので、クラスの継承次第ではクラッシュします。注意。
		//return: 成功したらtrue, 自身 or copyがnullptrならfalse
		//引数1: copy value
		inline bool UpdateManual(const SharedPointer<PointerType>& copy) const noexcept { return UpdateManual(copy.getPointer()); }
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//内部情報を更新する, 注意！Shared->Cloneは自動UpdateがSync時に行われています
		//作成時のコピーサイズを基にコピーするので、クラスの継承次第ではクラッシュします。注意。
		//return: 成功したらtrue, 自身 or copyがnullptrならfalse
		//引数1: copy value
		template<class DifferentType>
		inline bool UpdateManual(const SharedPointer<DifferentType>& copy) const noexcept
		{
			static_assert(std::is_convertible_v<DifferentType, PointerType>,
				"\nClonePointer->UpdateManual Failed.\nCondition : is_convertible_v<DifferentType, PointerType>");
			return UpdateManual(static_cast<PointerType*>(copy.getPointer()));
		}
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//内部情報を更新する, 注意！Weak(Shared)->Cloneは自動UpdateがSync時に行われています
		//作成時のコピーサイズを基にコピーするので、クラスの継承次第ではクラッシュします。注意。
		//return: 成功したらtrue, 自身 or copyがnullptrならfalse
		//引数1: copy value
		inline bool UpdateManual(WeakPointer<PointerType> copy) const noexcept
		{
			LockWeakGuard<WeakPointer<PointerType>> guard(copy);
			if (guard) return UpdateManual(copy.TryGetInstance());
			else return false;
		}
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//内部情報を更新する, 注意！Weak(Shared)->Cloneは自動UpdateがSync時に行われています
		//作成時のコピーサイズを基にコピーするので、クラスの継承次第ではクラッシュします。注意。
		//return: 成功したらtrue, 自身 or copyがnullptrならfalse
		//引数1: copy value
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
		//内部情報を更新する, 注意！Clone->Cloneは自動Updateが行われません
		//作成時のコピーサイズを基にコピーするので、クラスの継承次第ではクラッシュします。注意。
		//return: 成功したらtrue, 自身 or copyがnullptrならfalse
		//引数1: copy value
		inline bool UpdateManual(const CloneSharedPointer<PointerType>& copy) const noexcept { return UpdateManual(copy.getPointer()); }
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//内部情報を更新する, 注意！Clone->Cloneは自動Updateが行われません
		//作成時のコピーサイズを基にコピーするので、クラスの継承次第ではクラッシュします。注意。
		//return: 成功したらtrue, 自身 or copyがnullptrならfalse
		//引数1: copy value
		template<class DifferentType>
		inline bool UpdateManual(const CloneSharedPointer<DifferentType>& copy) const noexcept
		{
			static_assert(std::is_convertible_v<DifferentType, PointerType>,
				"\nClonePointer->UpdateManual Failed.\nCondition : is_convertible_v<DifferentType, PointerType>");
			return UpdateManual(static_cast<PointerType*>(copy.getPointer()));
		}
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//内部情報を更新する, 注意！Clone->Cloneは自動Updateが行われません
		//作成時のコピーサイズを基にコピーするので、クラスの継承次第ではクラッシュします。注意。
		//return: 成功したらtrue, 自身 or copyがnullptrならfalse
		//引数1: copy value
		inline bool UpdateManual(CloneWeakPointer<PointerType> copy) const noexcept
		{
			LockWeakGuard<CloneWeakPointer<PointerType>> guard(copy);
			if (guard) return UpdateManual(copy.TryGetInstance());
			else return false;
		}
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//内部情報を更新する, 注意！Clone->Cloneは自動Updateが行われません
		//作成時のコピーサイズを基にコピーするので、クラスの継承次第ではクラッシュします。注意。
		//return: 成功したらtrue, 自身 or copyがnullptrならfalse
		//引数1: copy value
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
		//参照カウンタを増やす, memcpyした時用だが、可能な限り使うべからず
		inline void AddRef() const noexcept { if (m_counter) m_counter->addWeak(); }
		//----------------------------------------------------------------------------------
		//[Reset]
		//参照を放棄し自身をnullptrに初期化する
		inline void Reset() noexcept { ReleasePointer(); }
		//----------------------------------------------------------------------------------
		//[LockShared]
		//参照が有効な場合は参照カウントを一時的に1増やしオート削除機能をロックする
		//オート削除機能を無効化するだけで複数スレッドによる参照は解決しない
		//return: ロック成功可否
		inline bool LockShared() noexcept;
		//----------------------------------------------------------------------------------
		//[UnlockShared]
		//Lockを実行していた場合オート削除機能ロック解除を行う
		//return: ロック解除成功可否
		inline bool UnlockShared() noexcept;
		//----------------------------------------------------------------------------------
		//[DynamicCast]
		//return: DynamicCastしたCloneWeakPointer
		template<class DifferentType>
		inline CloneWeakPointer<DifferentType> DynamicCast() const noexcept;
		//----------------------------------------------------------------------------------
		//[DownCast]
		//return: DownCastしたCloneWeakPointer
		template<class DifferentType>
		inline CloneWeakPointer<DifferentType> DownCast() const noexcept;
		//----------------------------------------------------------------------------------
		//[TryGetInstance]
		//インスタンス取得を試みる
		//return: インスタンスポインタ, SharedCount == 0の場合はnullptr
		inline PointerType* TryGetInstance() const noexcept { return m_counter && m_counter->getShared() > 0 ? m_pointer : nullptr; }
		//----------------------------------------------------------------------------------
		//[InstantiateShared]
		//return: 弱参照を基に作成したCloneSharedPointer(SharedCount == 0の場合はnullptr)
		inline CloneSharedPointer<PointerType> InstantiateShared() const noexcept;
		//----------------------------------------------------------------------------------
		//[InstantiateShared]
		//弱参照を基に作成したCloneSharedPointerを作成する(SharedCount == 0の場合はnullptr)
		//引数1: 作成したCloneSharedPointerを代入するポインタ
		inline void InstantiateShared(CloneSharedPointer<PointerType>& pointer) const noexcept;
	
		//shared count (get function property)
		SGF_FUNCTION_PROPERTY ushort getSharedCount() const noexcept { return m_counter ? m_counter->getShared() : 0; }
		//weak count (get function property)
		SGF_FUNCTION_PROPERTY ushort getWeakCount() const noexcept { return m_counter ? m_counter->getWeak() : 0; }
		//pointer is valid(有効)? (get function property)
		SGF_FUNCTION_PROPERTY bool getIsValid() const noexcept { return m_pointer ? true : false; }
		//pointer is revocatoin(失効)? (get function property)
		SGF_FUNCTION_PROPERTY bool getIsRevocatoin() const noexcept { return m_counter && m_counter->getShared() == 0 ? true : false; }
		//pointer is locked? (get function property)
		SGF_FUNCTION_PROPERTY bool getIsLocked() const noexcept { return m_isImLocked; }
		//pointer is linked(自動同期が有効)? (get function property)
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

		//ポインタ
		PointerType* m_pointer;
		//Lock行ったか
		bool m_isImLocked;
	};



	//弱参照カウンタを持つCloneWeakPointer class
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
		//[コンストラクタ]
		//nullptr初期化を行う
		inline CloneWeakPointer() noexcept;
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//参照コピーを行う
		//引数1: コピー元
		inline CloneWeakPointer(const CloneWeakPointer<PointerType[]>& copy) noexcept;
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//自身へのMoveを行う
		//引数1: Move元
		inline CloneWeakPointer(CloneWeakPointer<PointerType[]>&& move) noexcept;
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//参照コピーを行う
		//引数1: コピー元<Shared>
		inline CloneWeakPointer(const CloneSharedPointer<PointerType[]>& copy) noexcept;
		//----------------------------------------------------------------------------------
		//[デストラクタ]
		//メンバ変数のデストラクタによりカウンタの計算や開放を行う
		inline ~CloneWeakPointer() noexcept { ReleasePointer(); }

		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//内部情報を更新する, 注意！Direct->Cloneは自動Updateが行われません
		//作成時のコピーサイズを基にコピーするので、クラスの継承次第ではクラッシュします。注意。
		//return: 成功したらtrue, 自身 or copyがnullptrならfalse
		//引数1: copy value
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
		//内部情報を更新する, 注意！Direct->Cloneは自動Updateが行われません
		//作成時のコピーサイズを基にコピーするので、クラスの継承次第ではクラッシュします。注意。
		//return: 成功したらtrue, 自身 or copyがnullptrならfalse
		//引数1: copy value
		template<class DifferentType>
		inline bool UpdateManual(const DifferentType* copy) const noexcept
		{
			static_assert(std::is_convertible_v<DifferentType, PointerType>,
				"\nClonePointer->UpdateManual Failed.\nCondition : is_convertible_v<DifferentType, PointerType>");
			return UpdateManual(static_cast<PointerType*>(copy));
		}
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//内部情報を更新する, 注意！Unique->Cloneは自動UpdateがSync時に行われています
		//作成時のコピーサイズを基にコピーするので、クラスの継承次第ではクラッシュします。注意。
		//return: 成功したらtrue, 自身 or copyがnullptrならfalse
		//引数1: copy value
		inline bool UpdateManual(const UniquePointer<PointerType[]>& copy) const noexcept { return UpdateManual(copy.getPointer()); }
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//内部情報を更新する, 注意！Unique->Cloneは自動UpdateがSync時に行われています
		//作成時のコピーサイズを基にコピーするので、クラスの継承次第ではクラッシュします。注意。
		//return: 成功したらtrue, 自身 or copyがnullptrならfalse
		//引数1: copy value
		template<class DifferentType>
		inline bool UpdateManual(const UniquePointer<DifferentType[]>& copy) const noexcept
		{
			static_assert(std::is_convertible_v<DifferentType, PointerType>,
				"\nClonePointer->UpdateManual Failed.\nCondition : is_convertible_v<DifferentType, PointerType>");
			return UpdateManual(static_cast<PointerType*>(copy.getPointer()));
		}
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//内部情報を更新する, 注意！Shared->Cloneは自動UpdateがSync時に行われています
		//作成時のコピーサイズを基にコピーするので、クラスの継承次第ではクラッシュします。注意。
		//return: 成功したらtrue, 自身 or copyがnullptrならfalse
		//引数1: copy value
		inline bool UpdateManual(const SharedPointer<PointerType[]>& copy) const noexcept { return UpdateManual(copy.getPointer()); }
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//内部情報を更新する, 注意！Shared->Cloneは自動UpdateがSync時に行われています
		//作成時のコピーサイズを基にコピーするので、クラスの継承次第ではクラッシュします。注意。
		//return: 成功したらtrue, 自身 or copyがnullptrならfalse
		//引数1: copy value
		template<class DifferentType>
		inline bool UpdateManual(const SharedPointer<DifferentType[]>& copy) const noexcept
		{
			static_assert(std::is_convertible_v<DifferentType, PointerType>,
				"\nClonePointer->UpdateManual Failed.\nCondition : is_convertible_v<DifferentType, PointerType>");
			return UpdateManual(static_cast<PointerType*>(copy.getPointer()));
		}
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//内部情報を更新する, 注意！Weak(Shared)->Cloneは自動UpdateがSync時に行われています
		//作成時のコピーサイズを基にコピーするので、クラスの継承次第ではクラッシュします。注意。
		//return: 成功したらtrue, 自身 or copyがnullptrならfalse
		//引数1: copy value
		inline bool UpdateManual(WeakPointer<PointerType[]> copy) const noexcept
		{
			LockWeakGuard<WeakPointer<PointerType>> guard(copy);
			if (guard) return UpdateManual(copy.TryGetInstance());
			else return false;
		}
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//内部情報を更新する, 注意！Weak(Shared)->Cloneは自動UpdateがSync時に行われています
		//作成時のコピーサイズを基にコピーするので、クラスの継承次第ではクラッシュします。注意。
		//return: 成功したらtrue, 自身 or copyがnullptrならfalse
		//引数1: copy value
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
		//内部情報を更新する, 注意！Clone->Cloneは自動Updateが行われません
		//作成時のコピーサイズを基にコピーするので、クラスの継承次第ではクラッシュします。注意。
		//return: 成功したらtrue, 自身 or copyがnullptrならfalse
		//引数1: copy value
		inline bool UpdateManual(const CloneSharedPointer<PointerType[]>& copy) const noexcept { return UpdateManual(copy.getPointer()); }
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//内部情報を更新する, 注意！Clone->Cloneは自動Updateが行われません
		//作成時のコピーサイズを基にコピーするので、クラスの継承次第ではクラッシュします。注意。
		//return: 成功したらtrue, 自身 or copyがnullptrならfalse
		//引数1: copy value
		template<class DifferentType>
		inline bool UpdateManual(const CloneSharedPointer<DifferentType[]>& copy) const noexcept
		{
			static_assert(std::is_convertible_v<DifferentType, PointerType>,
				"\nClonePointer->UpdateManual Failed.\nCondition : is_convertible_v<DifferentType, PointerType>");
			return UpdateManual(static_cast<PointerType*>(copy.getPointer()));
		}
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//内部情報を更新する, 注意！Clone->Cloneは自動Updateが行われません
		//作成時のコピーサイズを基にコピーするので、クラスの継承次第ではクラッシュします。注意。
		//return: 成功したらtrue, 自身 or copyがnullptrならfalse
		//引数1: copy value
		inline bool UpdateManual(CloneWeakPointer<PointerType[]> copy) const noexcept
		{
			LockWeakGuard<CloneWeakPointer<PointerType>> guard(copy);
			if (guard) return UpdateManual(copy.TryGetInstance());
			else return false;
		}
		//----------------------------------------------------------------------------------
		//[UpdateManual]
		//内部情報を更新する, 注意！Clone->Cloneは自動Updateが行われません
		//作成時のコピーサイズを基にコピーするので、クラスの継承次第ではクラッシュします。注意。
		//return: 成功したらtrue, 自身 or copyがnullptrならfalse
		//引数1: copy value
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
		//参照カウンタを増やす, memcpyした時用だが、可能な限り使うべからず
		inline void AddRef() const noexcept { if (m_counter) m_counter->addWeak(); }
		//----------------------------------------------------------------------------------
		//[Reset]
		//参照を放棄し自身をnullptrに初期化する
		inline void Reset() noexcept { ReleasePointer(); }
		//----------------------------------------------------------------------------------
		//[LockShared]
		//参照が有効な場合は参照カウントを一時的に1増やしオート削除機能をロックする
		//オート削除機能を無効化するだけで複数スレッドによる参照は解決しない
		//return: ロック成功可否
		inline bool LockShared() noexcept;
		//----------------------------------------------------------------------------------
		//[UnlockShared]
		//Lockを実行していた場合オート削除機能ロック解除を行う
		//return: ロック解除成功可否
		inline bool UnlockShared() noexcept;
		//----------------------------------------------------------------------------------
		//[DynamicCast]
		//return: DynamicCastしたCloneWeakPointer
		template<class DifferentType>
		inline CloneWeakPointer<DifferentType[]> DynamicCast() const noexcept;
		//----------------------------------------------------------------------------------
		//[DownCast]
		//return: DownCastしたCloneWeakPointer
		template<class DifferentType>
		inline CloneWeakPointer<DifferentType[]> DownCast() const noexcept;
		//----------------------------------------------------------------------------------
		//[TryGetInstance]
		//インスタンス取得を試みる
		//return: インスタンスポインタ, SharedCount == 0の場合はnullptr
		inline PointerType* TryGetInstance() const noexcept { return m_counter && m_counter->getShared() > 0 ? m_pointer : nullptr; }
		//----------------------------------------------------------------------------------
		//[InstantiateShared]
		//return: 弱参照を基に作成したCloneSharedPointer(SharedCount == 0の場合はnullptr)
		inline CloneSharedPointer<PointerType[]> InstantiateShared() const noexcept;
		//----------------------------------------------------------------------------------
		//[InstantiateShared]
		//弱参照を基に作成したCloneSharedPointerを作成する(SharedCount == 0の場合はnullptr)
		//引数1: 作成したCloneSharedPointerを代入するポインタ
		inline void InstantiateShared(CloneSharedPointer<PointerType[]>& pointer) const noexcept;

		//shared count (get function property)
		SGF_FUNCTION_PROPERTY ushort getSharedCount() const noexcept { return m_counter ? m_counter->getShared() : 0; }
		//weak count (get function property)
		SGF_FUNCTION_PROPERTY ushort getWeakCount() const noexcept { return m_counter ? m_counter->getWeak() : 0; }
		//pointer is valid(有効)? (get function property)
		SGF_FUNCTION_PROPERTY bool getIsValid() const noexcept { return m_pointer ? true : false; }
		//pointer is revocatoin(失効)? (get function property)
		SGF_FUNCTION_PROPERTY bool getIsRevocatoin() const noexcept { return m_counter && m_counter->getShared() == 0 ? true : false; }
		//pointer is locked? (get function property)
		SGF_FUNCTION_PROPERTY bool getIsLocked() const noexcept { return m_isImLocked; }
		//pointer is linked(自動同期が有効)? (get function property)
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
		//所有権を放棄
		inline void ReleasePointer() noexcept override;
		
		//ポインタ
		PointerType* m_pointer;
		ushort m_arraySize;		//arraySize
		bool m_isImLocked;	//Lock行ったか
	};





	//----------------------------------------------------------------------------------
	//CloneSharedPointer
	//----------------------------------------------------------------------------------



	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//nullptr初期化を行う
	template<class PointerType>
	inline CloneSharedPointer<PointerType>::CloneSharedPointer() noexcept
		: BaseClonePointer(nullptr, 0), m_pointer(nullptr) {}
	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//参照コピーを行う
	//引数1: コピー元
	template<class PointerType>
	inline CloneSharedPointer<PointerType>::CloneSharedPointer(const CloneSharedPointer<PointerType>& copy) noexcept
		: BaseClonePointer(copy.m_counter), m_pointer(copy.m_pointer)
	{
		if (m_counter != nullptr) m_counter->addShared();
		else m_pointer = nullptr;
	}
	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//自身へのMoveを行う
	//引数1: Move元
	template<class PointerType>
	inline CloneSharedPointer<PointerType>::CloneSharedPointer(CloneSharedPointer<PointerType>&& move) noexcept
		: BaseClonePointer(move.m_counter), m_pointer(move.m_pointer)
	{
		move.m_pointer = nullptr;
		move.m_counter = nullptr;
	}
	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//初期化を行う
	//引数1: clone source pointer
	//引数2: Used mutex counter (Required for Multi thread), default = true
	//引数3: Auto gabage collection?, default = true
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
	//[コンストラクタ]
	//初期化を行う
	//引数1: clone source pointer
	//引数2: Used mutex counter (Required for Multi thread), default = true
	//引数3: Auto gabage collection?, default = true
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
	//[コンストラクタ]
	//初期化を行う
	//引数1: clone source pointer
	//引数2: Used mutex counter (Required for Multi thread), default = true
	//引数3: Auto gabage collection?, default = true
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
	//[コンストラクタ]
	//初期化を行う
	//引数1: clone source pointer
	//引数2: Used mutex counter (Required for Multi thread), default = true
	//引数3: Auto gabage collection?, default = true
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
	//内部情報を更新する, 注意！Clone->Cloneは自動Updateが行われません
	//作成時のコピーサイズを基にコピーするので、クラスの継承次第ではクラッシュします。注意。
	//return: 成功したらtrue, 自身 or copyがnullptrならfalse
	//引数1: copy value
	template<class PointerType>
	inline bool CloneSharedPointer<PointerType>::UpdateManual(CloneWeakPointer<PointerType> copy) const noexcept
	{
		LockWeakGuard<CloneWeakPointer<PointerType>> guard(copy);
		if (guard) return UpdateManual(static_cast<PointerType*>(copy.TryGetInstance()));
		else return false;
	}
	//----------------------------------------------------------------------------------
	//[Update]
	//内部情報を更新する, 注意！Clone->Cloneは自動Updateが行われません
	//作成時のコピーサイズを基にコピーするので、クラスの継承次第ではクラッシュします。注意。
	//return: 成功したらtrue, 自身 or copyがnullptrならfalse
	//引数1: copy value
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
	//return: DynamicCastしたCloneSharedPointer
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
	//return: DownCastしたCloneSharedPointer
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
		//違うかったらコピー
		if (copy.m_pointer != m_pointer)
		{
			//1度開放
			ReleasePointer();
			//nullでなければカウンタ加算
			if (copy.m_pointer != nullptr) copy.m_counter->addShared();
			//代入
			m_pointer = copy.m_pointer;
			m_counter = copy.m_counter;
		}
		return *this;
	}
	//= operator, move
	template<class PointerType>
	inline CloneSharedPointer<PointerType>& CloneSharedPointer<PointerType>::operator=(CloneSharedPointer<PointerType>&& move) noexcept
	{
		//違うかったらムーブ
		if (m_pointer != move.m_pointer)
		{
			//1度開放
			ReleasePointer();
			//移動
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
		//not null -> 削除予約
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
	//[コンストラクタ]
	//nullptr初期化を行う
	template<class PointerType>
	inline CloneSharedPointer<PointerType[]>::CloneSharedPointer() noexcept
		: BaseClonePointer(nullptr, 0), m_pointer(nullptr), m_arraySize(0) {}
	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//参照コピーを行う
	//引数1: コピー元
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
	//[コンストラクタ]
	//自身へのMoveを行う
	//引数1: Move元
	template<class PointerType>
	inline CloneSharedPointer<PointerType[]>::CloneSharedPointer(CloneSharedPointer<PointerType[]>&& move) noexcept
		: BaseClonePointer(move.m_counter), m_pointer(move.m_pointer), m_arraySize(move.m_arraySize)
	{
		move.m_pointer = nullptr;
		move.m_counter = nullptr;
		move.m_arraySize = 0;
	}
	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//初期化を行う
	//引数1: clone source pointer
	//引数2: 配列サイズ
	//引数3: Used mutex counter (Required for Multi thread), default = true
	//引数4: Auto gabage collection?, default = true
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
	//[コンストラクタ]
	//初期化を行う
	//引数1: clone source pointer
	//引数2: Used mutex counter (Required for Multi thread), default = true
	//引数3: Auto gabage collection?, default = true
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
	//[コンストラクタ]
	//初期化を行う
	//引数1: clone source pointer
	//引数2: Used mutex counter (Required for Multi thread), default = true
	//引数3: Auto gabage collection?, default = true
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
	//[コンストラクタ]
	//初期化を行う
	//引数1: clone source pointer
	//引数2: Used mutex counter (Required for Multi thread), default = true
	//引数3: Auto gabage collection?, default = true
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
	//内部情報を更新する, 注意！Clone->Cloneは自動Updateが行われません
	//作成時のコピーサイズを基にコピーするので、クラスの継承次第ではクラッシュします。注意。
	//return: 成功したらtrue, 自身 or copyがnullptrならfalse
	//引数1: copy value
	template<class PointerType>
	inline bool CloneSharedPointer<PointerType[]>::UpdateManual(CloneWeakPointer<PointerType[]> copy) const noexcept
	{
		LockWeakGuard<CloneWeakPointer<PointerType>> guard(copy);
		if (guard) return UpdateManual(static_cast<PointerType*>(copy.TryGetInstance()));
		else return false;
	}
	//----------------------------------------------------------------------------------
	//[Update]
	//内部情報を更新する, 注意！Clone->Cloneは自動Updateが行われません
	//作成時のコピーサイズを基にコピーするので、クラスの継承次第ではクラッシュします。注意。
	//return: 成功したらtrue, 自身 or copyがnullptrならfalse
	//引数1: copy value
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
	//return: DynamicCastしたCloneSharedPointer
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
	//return: DownCastしたCloneSharedPointer
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
		//違うかったらコピー
		if (copy.m_pointer != m_pointer)
		{
			//1度開放
			ReleasePointer();
			//nullでなければカウンタ加算
			if (copy.m_counter != nullptr) copy.m_counter->addShared();
			//代入
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
		//違うかったらムーブ
		if (m_pointer != move.m_pointer)
		{
			//1度開放
			ReleasePointer();
			//移動
			m_pointer = move.m_pointer;
			m_counter = move.m_counter;
			m_arraySize = move.m_arraySize;
			move.m_pointer = nullptr;
			move.m_counter = nullptr;
			move.m_arraySize = 0;
		}
		return *this;
	}

	//ポインタの開放を試みる
	template<class PointerType>
	inline void CloneSharedPointer<PointerType[]>::ReleasePointer() noexcept
	{
		//not null -> 削除予約
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
	//[コンストラクタ]
	//nullptr初期化を行う
	template<class PointerType>
	inline CloneWeakPointer<PointerType>::CloneWeakPointer() noexcept
		: BaseClonePointer(nullptr, 0), m_pointer(nullptr), m_isImLocked(false) {}
	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//参照コピーを行う
	//引数1: コピー元
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
	//[コンストラクタ]
	//自身へのMoveを行う
	//引数1: Move元
	template<class PointerType>
	inline CloneWeakPointer<PointerType>::CloneWeakPointer(CloneWeakPointer<PointerType>&& move) noexcept
		: BaseClonePointer(move.m_counter, 0), m_pointer(move.m_pointer), m_isImLocked(move.m_isImLocked)
	{
		move.m_pointer = nullptr;
		move.m_counter = nullptr;
		move.m_isImLocked = false;
	}
	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//参照コピーを行う
	//引数1: コピー元<Shared>
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
	//参照が有効な場合は参照カウントを一時的に1増やしオート削除機能をロックする
	//オート削除機能を無効化するだけで複数スレッドによる参照は解決しない
	template<class PointerType>
	inline bool CloneWeakPointer<PointerType>::LockShared() noexcept
	{
		//ロックしてない & 実体あり->カウンタ++成功でreturn true
		if (IS_FALSE(m_isImLocked))
			if (m_counter)
				return m_isImLocked = m_counter->LockShared();

		return false;
	}
	//----------------------------------------------------------------------------------
	//[UnlockShared]
	//Lockを実行していた場合オート削除機能ロック解除を行う
	//return: ロック解除成功可否
	template<class PointerType>
	inline bool CloneWeakPointer<PointerType>::UnlockShared() noexcept
	{
		//ロックしてる
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
	//return: DynamicCastしたCloneWeakPointer
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
	//return: DownCastしたCloneWeakPointer
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
	//弱参照を基に作成したCloneSharedPointerを作成する(SharedCount == 0の場合はnullptr)
	//引数1: 作成したCloneSharedPointerを代入する
	template<class PointerType>
	inline CloneSharedPointer<PointerType> CloneWeakPointer<PointerType>::InstantiateShared() const noexcept
	{
		CloneSharedPointer<PointerType> result;

		//インスタンスがある
		if (m_counter && IS_TRUE(m_counter->LockShared()))
		{
			result.m_pointer = m_pointer;
			result.m_counter = m_counter;
		}

		return std::move(result);
	}
	//----------------------------------------------------------------------------------
	//[InstantiateShared]
	//弱参照を基に作成したCloneSharedPointerを作成する(SharedCount == 0の場合はnullptr)
	//引数1: 作成したCloneSharedPointerを代入するポインタ
	template<class PointerType>
	inline void CloneWeakPointer<PointerType>::InstantiateShared(CloneSharedPointer<PointerType>& pointer) const noexcept
	{
		//開放
		pointer.ReleasePointer();

		//インスタンスがある
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
		//違ったらコピー
		if (copy.m_pointer != m_pointer)
		{
			//一度開放
			ReleasePointer();
			//nullでなければカウンタ加算
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
		//違ったらムーブ
		if (m_counter != move.m_counter)
		{
			//一度開放
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
		//値違ったらコピー
		if (m_counter != copy.m_counter)
		{
			//一度開放
			ReleasePointer();
			//nullでなければカウンタ加算
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
		//ロックしてたら開放
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
	//[コンストラクタ]
	//nullptr初期化を行う
	template<class PointerType>
	inline CloneWeakPointer<PointerType[]>::CloneWeakPointer() noexcept
		: BaseClonePointer(nullptr, 0), m_pointer(nullptr), m_arraySize(0), m_isImLocked(false) {}
	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//参照コピーを行う
	//引数1: コピー元
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
	//[コンストラクタ]
	//自身へのMoveを行う
	//引数1: Move元
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
	//[コンストラクタ]
	//参照コピーを行う
	//引数1: コピー元<Shared>
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
	//参照が有効な場合は参照カウントを一時的に1増やしオート削除機能をロックする
	//オート削除機能を無効化するだけで複数スレッドによる参照は解決しない
	template<class PointerType>
	inline bool CloneWeakPointer<PointerType[]>::LockShared() noexcept
	{
		//ロックしてない & 実体あり->カウンタ++成功でreturn true
		if (IS_FALSE(m_isImLocked))
			if (m_counter)
				return m_isImLocked = m_counter->LockShared();

		return false;
	}
	//----------------------------------------------------------------------------------
	//[UnlockShared]
	//Lockを実行していた場合オート削除機能ロック解除を行う
	//return: ロック解除成功可否
	template<class PointerType>
	inline bool CloneWeakPointer<PointerType[]>::UnlockShared() noexcept
	{
		//ロックしてる
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
	//return: DynamicCastしたCloneWeakPointer
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
	//return: DownCastしたCloneWeakPointer
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
	//弱参照を基に作成したCloneSharedPointerを作成する(SharedCount == 0の場合はnullptr)
	//引数1: 作成したCloneSharedPointerを代入する
	template<class PointerType>
	inline CloneSharedPointer<PointerType[]> CloneWeakPointer<PointerType[]>::InstantiateShared() const noexcept
	{
		CloneSharedPointer<PointerType> result;

		//インスタンスがある
		if (m_counter && IS_TRUE(m_counter->LockShared()))
		{
			result.m_pointer = m_pointer;
			result.m_counter = m_counter;
		}

		return std::move(result);
	}
	//----------------------------------------------------------------------------------
	//[InstantiateShared]
	//弱参照を基に作成したCloneSharedPointerを作成する(SharedCount == 0の場合はnullptr)
	//引数1: 作成したCloneSharedPointerを代入するポインタ
	template<class PointerType>
	inline void CloneWeakPointer<PointerType[]>::InstantiateShared(CloneSharedPointer<PointerType[]>& pointer) const noexcept
	{
		//開放
		pointer.ReleasePointers();

		//インスタンスがある
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
		//違ったらコピー
		if (copy.m_pointer != m_pointer)
		{
			//一度開放
			ReleasePointer();
			//nullでなければカウンタ加算
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
		//違ったらムーブ
		if (m_counter != move.m_counter)
		{
			//一度開放
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
		//値違ったらコピー
		if (m_counter != copy.m_counter)
		{
			//一度開放
			ReleasePointer();
			//nullでなければカウンタ加算
			if (m_counter != nullptr)
				m_counter->addWeak();

			//copy
			m_pointer = copy.m_pointer;
			m_counter = copy.m_counter;
			m_arraySize = copy.m_arraySize;
		}
		return copy;
	}

	//所有権を放棄
	template<class PointerType>
	inline void CloneWeakPointer<PointerType[]>::ReleasePointer() noexcept
	{
		//ロックしてたら開放
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