/*----------------------------------------------------------------------------------
�C���X�^���X����Ɍ��肷��Singleton class
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_SINGLETON_HPP_
#define SGFRAMEWORK_HEADER_SINGLETON_HPP_
#include <memory>
#include "../Property/Property.hpp"
#include "../Pointer/Pointer.hpp"

//Framework namespace
namespace SGFramework
{
	//�C���X�^���X����Ɍ��肷��Singleton class
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
		//return: �C���X�^���X
		inline static TKey& Instance()
		{
			static UniquePointer<TKey> ptr = TKey::CreateInstance();
			return *ptr;
		}

	protected:
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		inline Singleton() {};
	private:
		//----------------------------------------------------------------------------------
		//[CreateInstance]
		//return: �C���X�^���X
		inline static UniquePointer<TKey> CreateInstance() {return std::move(Pointer::MakeUnique::Auto<TKey>());}
	};
}

#endif // !SGFRAMEWORK_HEADER_SINGLETON_HPP_
