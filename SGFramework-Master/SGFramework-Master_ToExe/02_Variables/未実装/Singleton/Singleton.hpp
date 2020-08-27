/*----------------------------------------------------------------------------------
インスタンスを一つに限定するSingleton class
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_SINGLETON_HPP_
#define SGFRAMEWORK_HEADER_SINGLETON_HPP_
#include <memory>
#include "../Property/Property.hpp"
#include "../Pointer/Pointer.hpp"

//Framework namespace
namespace SGFramework
{
	//インスタンスを一つに限定するSingleton class
	template <class TKey>
	class Singleton
	{
	public:
		//delete
		Singleton(const Singleton &) = delete;
		//delete
		Singleton& operator=(const Singleton &) = delete;
		//delete
		Singleton(Singleton &&) = delete;
		//delete
		Singleton& operator=(Singleton &&) = delete;

		//----------------------------------------------------------------------------------
		//[Instance]
		//return: インスタンス
		inline static TKey& Instance()
		{
			static UniquePointer<TKey> ptr = TKey::CreateInstance();
			return *ptr;
		}

	protected:
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		inline Singleton() {};
	private:
		//----------------------------------------------------------------------------------
		//[CreateInstance]
		//return: インスタンス
		inline static UniquePointer<TKey> CreateInstance() {return std::move(Pointer::MakeUnique::Auto<TKey>());}
	};
}

#endif // !SGFRAMEWORK_HEADER_SINGLETON_HPP_
