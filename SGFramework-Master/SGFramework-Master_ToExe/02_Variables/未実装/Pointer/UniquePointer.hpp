/*----------------------------------------------------------------------------------
デストラクタで自動的にdeleteされるUniquePointer class
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_UNIQUE_POINTER_HPP_
#define SGFRAMEWORK_HEADER_UNIQUE_POINTER_HPP_
#include "GarbageCollector.hpp"

//Framework namespace
namespace SGFramework
{
	//デストラクタで自動的にdeleteされるUniquePointer class
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
		//[コンストラクタ]
		//nullptr初期化を行う
		inline UniquePointer() noexcept;
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//自身へのMoveを行う
		//引数1: Move元
		inline UniquePointer(UniquePointer<PointerType>&& move) noexcept;
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//初期化を行う
		//引数1: newしたポインター
		//引数2: Auto gabage collection?, default = true
		template<class NewType>
		inline explicit UniquePointer(NewType* newPointer, bool isAutoGabageCollection = true) noexcept;
		//----------------------------------------------------------------------------------
		//[デストラクタ]
		//メンバ変数のデストラクタによりカウンタの計算や開放を行う
		inline ~UniquePointer() noexcept { ReleasePointer(); }

		//----------------------------------------------------------------------------------
		//[Release]
		//ポインタの開放し自身をnullptrに初期化する
		inline void Release() noexcept { ReleasePointer(); }
		//----------------------------------------------------------------------------------
		//[MovingDynamicCast]
		//return: DynamicCastしたSharedPointer, 自身はNullになる
		template<class DifferentType>
		inline UniquePointer<DifferentType> MovingDynamicCast() noexcept;
		//----------------------------------------------------------------------------------
		//[MovingDownCast]
		//return: DownCastしたUniquePointer, 自身はNullになる
		template<class DifferentType>
		inline UniquePointer<DifferentType> MovingDownCast() noexcept;
		
		//this raw pointer, not delete!! (get function property)
		SGF_FUNCTION_PROPERTY PointerType* getPointer() const noexcept { return m_pointer; }
		//pointer is owned(有効)? (get function property)
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

		//ポインタ
		PointerType* m_pointer;
	};


	//デストラクタで自動的にdeleteされるUniquePointer class [Array]
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
		//[コンストラクタ]
		//nullptr初期化を行う
		inline UniquePointer() noexcept;
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//自身へのMoveを行う
		//引数1: Move元
		inline UniquePointer(UniquePointer<PointerType[]>&& move) noexcept;
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//初期化を行う
		//引数1: newしたポインター
		//引数2: 配列サイズ
		//引数2: Auto gabage collection?, default = true
		template<class NewType>
		inline explicit UniquePointer(NewType* pointer, uint arraySize, bool isAutoGabageCollection = true) noexcept;
		//----------------------------------------------------------------------------------
		//[デストラクタ]
		//メンバ変数のデストラクタによりカウンタの計算や開放を行う
		inline ~UniquePointer() noexcept { ReleasePointer(); }

		//----------------------------------------------------------------------------------
		//[Release]
		//ポインタの開放し自身をnullptrに初期化する
		inline void Release() noexcept { ReleasePointer(); }
		//----------------------------------------------------------------------------------
		//[MovingDynamicCast]
		//return: DynamicCastしたSharedPointer, 自身はNullになる
		template<class DifferentType>
		inline UniquePointer<DifferentType[]> MovingDynamicCast() noexcept;
		//----------------------------------------------------------------------------------
		//[MovingDownCast]
		//return: DownCastしたUniquePointer, 自身はNullになる
		template<class DifferentType>
		inline UniquePointer<DifferentType[]> MovingDownCast() noexcept;
		
		//this raw pointer, not delete!! (get function property)
		SGF_FUNCTION_PROPERTY PointerType* getPointer() const noexcept { return m_pointer; }
		//pointer is owned(有効)? (get function property)
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
		//ポインタの開放を試みる
		inline void ReleasePointer() noexcept;

		//ポインタ
		PointerType* m_pointer;
		//arraySize
		uint m_arraySize;
	};



	//----------------------------------------------------------------------------------
	//UniquePointer
	//----------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//nullptr初期化を行う
	template<class PointerType>
	inline UniquePointer<PointerType>::UniquePointer() noexcept
		: m_pointer(nullptr) { }
	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//自身へのMoveを行う
	//引数1: Move元
	template<class PointerType>
	inline UniquePointer<PointerType>::UniquePointer(UniquePointer<PointerType>&& move) noexcept
	 : m_pointer(move.m_pointer)
	{
		move.m_pointer = nullptr;
	}
	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//初期化を行う
	//引数1: newしたポインター
	//引数2: Auto gabage collection?, default = true
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
	//return: DynamicCastしたSharedPointer, 自身はNullになる
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
	//return: DownCastしたUniquePointer, 自身はNullになる
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
		//違うかったらムーブ
		if (m_pointer != move.m_pointer)
		{
			//1度開放
			ReleasePointer();
			//移動
			m_pointer = move.m_pointer;
			move.m_pointer = nullptr;
		}
		return *this;
	}
	
	//ポインタの開放を試みる
	template<class PointerType>
	inline void UniquePointer<PointerType>::ReleasePointer() noexcept
	{
		//ポインターが存在->Destroy予約
		if (m_pointer)
			GabageCollector::DestroyUnique(m_pointer, this);

		//nullptr
		m_pointer = nullptr;
	}


	//----------------------------------------------------------------------------------
	//UniquePointer [Array]
	//----------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//nullptr初期化を行う
	template<class PointerType>
	inline UniquePointer<PointerType[]>::UniquePointer() noexcept
		: m_pointer(nullptr), m_arraySize(0) { }
	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//自身へのMoveを行う
	//引数1: Move元
	template<class PointerType>
	inline UniquePointer<PointerType[]>::UniquePointer(UniquePointer<PointerType[]>&& move) noexcept
		: m_pointer(move.m_pointer), m_arraySize(move.m_arraySize)
	{
		move.m_pointer = nullptr;
		move.m_arraySize = 0;
	}
	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//初期化を行う
	//引数1: newしたポインター
	//引数2: 配列サイズ
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
	//return: DynamicCastしたSharedPointer, 自身はNullになる
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
	//return: DownCastしたUniquePointer, 自身はNullになる
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
		//違うかったらムーブ
		if (m_pointer != move.m_pointer)
		{
			//1度開放
			ReleasePointer();
			//移動
			m_pointer = move.m_pointer;
			m_arraySize = move.m_arraySize;
			move.m_pointer = nullptr;
			move.m_arraySize = 0;
		}
		return *this;
	}
	
	//ポインタの開放を試みる
	template<class PointerType>
	inline void UniquePointer<PointerType[]>::ReleasePointer() noexcept
	{
		//ポインターが存在->Destroy予約
		if (m_pointer)
			GabageCollector::DestroyUnique(m_pointer, this);

		//nullptr
		m_pointer = nullptr;
		m_arraySize = 0;
	}
}
#endif // !SGFRAMEWORK_HEADER_UNIQUE_POINTER_HPP_
