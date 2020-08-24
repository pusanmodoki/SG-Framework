/*----------------------------------------------------------------------------------
ポインター系クラスをまとめるPointer.hpp
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_POINTER_HPP_
#define SGFRAMEWORK_HEADER_POINTER_HPP_
#include "GarbageCollector.hpp"
#include "SharedPointer.hpp"
#include "UniquePointer.hpp"
#include "ClonePointer.hpp"
#include "LockWeakGuard.hpp"


//Framework namespace
namespace SGFramework
{
	//Make Pointer
	namespace Pointer
	{
		//Make Unique
		namespace MakeUnique
		{
			//----------------------------------------------------------------------------------
			//[MakeUnique::Auto]
			//return: 生成したUniquePointer (Auto Gabage Collection)
			template<class PointerType>
			inline UniquePointer<PointerType> Auto()
			{
				return std::move(UniquePointer<PointerType>(new PointerType(), true));
			}
			//----------------------------------------------------------------------------------
			//[MakeUnique::Auto]
			//return: 生成したUniquePointer (Auto Gabage Collection)
			//引数1: コンストラクタに入れる引数パック
			template<class PointerType, class... Args>
			inline UniquePointer<PointerType> Auto(Args&... args)
			{
				return std::move(UniquePointer<PointerType>(new PointerType(args...), true));
			}
			//----------------------------------------------------------------------------------
			//[MakeUnique::Auto]
			//return: 生成したUniquePointer (Auto Gabage Collection)
			//引数1: コンストラクタに入れる引数パック
			template<class PointerType, class... Args>
			inline UniquePointer<PointerType> Auto(const Args&... args)
			{
				return std::move(UniquePointer<PointerType>(new PointerType(args...), true));
			}
			//----------------------------------------------------------------------------------
			//[MakeUnique::Manual]
			//return: 生成したUniquePointer (Manual Gabage Collection)
			template<class PointerType>
			inline UniquePointer<PointerType> Manual()
			{
				return std::move(UniquePointer<PointerType>(new PointerType(), false));
			}
			//----------------------------------------------------------------------------------
			//[MakeUnique::Manual]
			//return: 生成したUniquePointer (Manual Gabage Collection)
			//引数1: コンストラクタに入れる引数パック
			template<class PointerType, class... Args>
			inline UniquePointer<PointerType> Manual(Args&... args)
			{
				return std::move(UniquePointer<PointerType>(new PointerType(args...), false));
			}
			//----------------------------------------------------------------------------------
			//[MakeUnique::Manual]
			//return: 生成したUniquePointer (Manual Gabage Collection)
			//引数1: コンストラクタに入れる引数パック
			template<class PointerType, class... Args>
			inline UniquePointer<PointerType> Manual(const Args&... args)
			{
				return std::move(UniquePointer<PointerType>(new PointerType(args...), false));
			}
		}
		

		//Make Unique Array
		namespace MakeUniqueArray
		{
			//----------------------------------------------------------------------------------
			//[MakeUniqueArray::Auto]
			//return: 生成したUniquePointer[Array] (Auto Gabage Collection)
			//引数1: 配列サイズ
			template<class PointerType>
			inline UniquePointer<PointerType[]> Auto(uint size)
			{
				return std::move(UniquePointer<PointerType[]>(new PointerType[size], size, true));
			}

			//----------------------------------------------------------------------------------
			//[MakeUniqueArray::Auto]
			//return: 生成したUniquePointer[Array] (Manual Gabage Collection)
			//引数1: 配列サイズ
			template<class PointerType>
			inline UniquePointer<PointerType[]> Manual(uint size)
			{
				return std::move(UniquePointer<PointerType[]>(new PointerType[size], size, false));
			}
		}


		//Make Shared (Used Single Thread)
		namespace MakeSingleShared
		{
			//----------------------------------------------------------------------------------
			//[MakeSingleShared::Auto]
			//return: 生成したSharedPointer (Used Single Thread, Auto Gabage Collection)
			template<class PointerType>
			inline SharedPointer<PointerType> Auto()
			{
				return std::move(SharedPointer<PointerType>(new PointerType(), false, true));
			}
			//----------------------------------------------------------------------------------
			//[MakeSingleShared::Auto]
			//return: 生成したSharedPointer (Used Single Thread, Auto Gabage Collection)
			//引数1: コンストラクタに入れる引数パック
			template<class PointerType, class... Args>
			inline SharedPointer<PointerType> Auto(Args&... args)
			{
				return std::move(SharedPointer<PointerType>(new PointerType(args...), false, true));
			}
			//----------------------------------------------------------------------------------
			//[MakeSingleShared::Auto]
			//return: 生成したSharedPointer (Used Single Thread, Auto Gabage Collection)
			//引数1: コンストラクタに入れる引数パック
			template<class PointerType, class... Args>
			inline SharedPointer<PointerType> Auto(const Args&... args)
			{
				return std::move(SharedPointer<PointerType>(new PointerType(args...), false, true));
			}

			//----------------------------------------------------------------------------------
			//[MakeSingleShared::Manual]
			//return: 生成したSharedPointer (Used Single Thread, Manual Gabage Collection)
			template<class PointerType>
			inline SharedPointer<PointerType> Manual()
			{
				return std::move(SharedPointer<PointerType>(new PointerType(), false, false));
			}
			//----------------------------------------------------------------------------------
			//[MakeSingleShared::Manual]
			//return: 生成したSharedPointer (Used Single Thread, Manual Gabage Collection)
			//引数1: コンストラクタに入れる引数パック
			template<class PointerType, class... Args>
			inline SharedPointer<PointerType> Manual(Args&... args)
			{
				return std::move(SharedPointer<PointerType>(new PointerType(args...), false, false));
			}
			//----------------------------------------------------------------------------------
			//[MakeSingleShared::Manual]
			//return: 生成したSharedPointer (Used Single Thread, Manual Gabage Collection)
			//引数1: コンストラクタに入れる引数パック
			template<class PointerType, class... Args>
			inline SharedPointer<PointerType> Manual(const Args&... args)
			{
				return std::move(SharedPointer<PointerType>(new PointerType(args...), false, false));
			}
		}


		//Make Shared (Used Multi Thread)
		namespace MakeMultiShared
		{
			//----------------------------------------------------------------------------------
			//[MaleMultiShared::Auto]
			//return: 生成したSharedPointer (Used Multi Thread, Auto Gabage Collection)
			template<class PointerType>
			inline SharedPointer<PointerType> Auto()
			{
				return std::move(SharedPointer<PointerType>(new PointerType(), true, true));
			}
			//----------------------------------------------------------------------------------
			//[MaleMultiShared::Auto]
			//return: 生成したSharedPointer (Used Multi Thread, Auto Gabage Collection)
			//引数1: コンストラクタに入れる引数パック
			template<class PointerType, class... Args>
			inline SharedPointer<PointerType> Auto(Args&... args)
			{
				return std::move(SharedPointer<PointerType>(new PointerType(args...), true, true));
			}
			//----------------------------------------------------------------------------------
			//[MaleMultiShared::Auto]
			//return: 生成したSharedPointer (Used Multi Thread, Auto Gabage Collection)
			//引数1: コンストラクタに入れる引数パック
			template<class PointerType, class... Args>
			inline SharedPointer<PointerType> Auto(const Args&... args)
			{
				return std::move(SharedPointer<PointerType>(new PointerType(args...), true, true));
			}

			//----------------------------------------------------------------------------------
			//[MaleMultiShared::Manual]
			//return: 生成したSharedPointer (Used Multi Thread, Manual Gabage Collection)
			template<class PointerType>
			inline SharedPointer<PointerType> Manual()
			{
				return std::move(SharedPointer<PointerType>(new PointerType(), true, false));
			}
			//----------------------------------------------------------------------------------
			//[MaleMultiShared::Manual]
			//return: 生成したSharedPointer (Used Multi Thread, Manual Gabage Collection)
			//引数1: コンストラクタに入れる引数パック
			template<class PointerType, class... Args>
			inline SharedPointer<PointerType> Manual(Args&... args)
			{
				return std::move(SharedPointer<PointerType>(new PointerType(args...), true, false));
			}
			//----------------------------------------------------------------------------------
			//[MaleMultiShared::Manual]
			//return: 生成したSharedPointer (Used Multi Thread, Manual Gabage Collection)
			//引数1: コンストラクタに入れる引数パック
			template<class PointerType, class... Args>
			inline SharedPointer<PointerType> Manual(const Args&... args)
			{
				return std::move(SharedPointer<PointerType>(new PointerType(args...), true, false));
			}
		}

		//Make Shared Array(Used Single Thread)
		namespace MakeSingleSharedArray
		{
			//----------------------------------------------------------------------------------
			//[MakeSingleSharedArray::Auto]
			//return: 生成したSharedPointer[Array] (Used Single Thread, Auto Gabage Collection)
			//引数1: 配列サイズ
			template<class PointerType, class... Args>
			inline SharedPointer<PointerType> Auto(uint size)
			{
				return std::move(SharedPointer<PointerType[]>(new PointerType[size], size, false, true));
			}

			//----------------------------------------------------------------------------------
			//[MakeSingleSharedArray::Manual]
			//return: 生成したSharedPointer[Array] (Used Single Thread, Manual Gabage Collection)
			//引数1: 配列サイズ
			template<class PointerType>
			inline SharedPointer<PointerType> Manual(uint size)
			{
				return std::move(SharedPointer<PointerType[]>(new PointerType[size], size, false, false));
			}
		}

		//Make Shared Array(Used Multi Thread)
		namespace MakeMultiSharedArray
		{
			//----------------------------------------------------------------------------------
			//[MakeMultiSharedArray::Auto]
			//return: 生成したSharedPointer[Array] (Used Multi Thread, Auto Gabage Collection)
			//引数1: 配列サイズ
			template<class PointerType>
			inline SharedPointer<PointerType[]> Auto(uint size)
			{
				return std::move(SharedPointer<PointerType[]>(new PointerType[size], size, true, true));
			}

			//----------------------------------------------------------------------------------
			//[MakeMultiSharedArray::Manual]
			//return: 生成したSharedPointer[Array] (Used Multi Thread, Manual Gabage Collection)
			//引数1: 配列サイズ
			template<class PointerType>
				inline SharedPointer<PointerType[]> Manual(uint size)
			{
				return std::move(SharedPointer<PointerType[]>(new PointerType(), size, true, false));
			}
		}


		//Make Clone Shared (Used Single Thread)
		namespace MakeSingleClone
		{
			//----------------------------------------------------------------------------------
			//[MakeSingleClone::Auto]
			//return: 生成したCloneSharedPointer, Required UpdateManual (Used Single Thread, Auto Gabage Collection)
			//引数1: clone source pointer
			template<class PointerType, class CloneBaseType>
			inline SharedPointer<PointerType> Auto(const CloneBaseType* cloneSourcePointer)
			{
				return std::move(CloneSharedPointer<PointerType>(cloneSourcePointer, false, true));
			}
			//----------------------------------------------------------------------------------
			//[MakeSingleClone::Auto]
			//return: 生成したCloneSharedPointer (Used Single Thread, Auto Gabage Collection)
			//引数1: clone source pointer
			template<class PointerType, class CloneBaseType>
			inline SharedPointer<PointerType> Auto(const UniquePointer<CloneBaseType>& cloneSourcePointer)
			{
				return std::move(CloneSharedPointer<PointerType>(cloneSourcePointer, false, true));
			}
			//----------------------------------------------------------------------------------
			//[MakeSingleClone::Auto]
			//return: 生成したCloneSharedPointer (Used Single Thread, Auto Gabage Collection)
			//引数1: clone source pointer
			template<class PointerType, class CloneBaseType>
			inline SharedPointer<PointerType> Auto(const SharedPointer<CloneBaseType>& cloneSourcePointer)
			{
				return std::move(CloneSharedPointer<PointerType>(cloneSourcePointer, false, true));
			}
			//----------------------------------------------------------------------------------
			//[MakeSingleClone::Auto]
			//return: 生成したCloneSharedPointer, Not Pointer = null (Used Single Thread, Auto Gabage Collection)
			//引数1: clone source pointer
			template<class PointerType, class CloneBaseType>
			inline SharedPointer<PointerType> Auto(WeakPointer<CloneBaseType>& cloneSourcePointer)
			{
				return std::move(CloneSharedPointer<PointerType>(cloneSourcePointer, false, true));
			}
			//----------------------------------------------------------------------------------
			//[MakeSingleClone::Auto]
			//return: 生成したCloneSharedPointer, Required UpdateManual (Used Single Thread, Auto Gabage Collection)
			//引数1: clone source pointer
			template<class PointerType, class CloneBaseType>
			inline SharedPointer<PointerType> Auto(const CloneSharedPointer<CloneBaseType>& cloneSourcePointer)
			{
				return std::move(CloneSharedPointer<PointerType>(cloneSourcePointer.getPointer(), false, true));
			}
			//----------------------------------------------------------------------------------
			//[MakeSingleClone::Auto]
			//return: 生成したCloneSharedPointer, Required UpdateManual (Used Single Thread, Auto Gabage Collection)
			//引数1: clone source pointer
			template<class PointerType, class CloneBaseType>
			inline SharedPointer<PointerType> Auto(CloneWeakPointer<CloneBaseType>& cloneSourcePointer)
			{
				LockWeakGuard<CloneWeakPointer<CloneBaseType>> guard(cloneSourcePointer);
				if (guard.getIsLocked())
					return std::move(CloneSharedPointer<PointerType>(cloneSourcePointer.TryGetInstance(), false, true));
				else
					return std::move(CloneSharedPointer<PointerType>());
			}


			//----------------------------------------------------------------------------------
			//[MakeSingleClone::Manual]
			//return: 生成したCloneSharedPointer, Required UpdateManual (Used Single Thread, Manual Gabage Collection)
			//引数1: clone source pointer
			template<class PointerType, class CloneBaseType>
			inline SharedPointer<PointerType> Manual(const CloneBaseType* cloneSourcePointer)
			{
				return std::move(CloneSharedPointer<PointerType>(cloneSourcePointer, false, false));
			}
			//----------------------------------------------------------------------------------
			//[MakeSingleClone::Manual]
			//return: 生成したCloneSharedPointer (Used Single Thread, Manual Gabage Collection)
			//引数1: clone source pointer
			template<class PointerType, class CloneBaseType>
			inline SharedPointer<PointerType> Manual(const UniquePointer<CloneBaseType>& cloneSourcePointer)
			{
				return std::move(CloneSharedPointer<PointerType>(cloneSourcePointer, false, false));
			}
			//----------------------------------------------------------------------------------
			//[MakeSingleClone::Manual]
			//return: 生成したCloneSharedPointer (Used Single Thread, Manual Gabage Collection)
			//引数1: clone source pointer
			template<class PointerType, class CloneBaseType>
			inline SharedPointer<PointerType> Manual(const SharedPointer<CloneBaseType>& cloneSourcePointer)
			{
				return std::move(CloneSharedPointer<PointerType>(cloneSourcePointer, false, false));
			}
			//----------------------------------------------------------------------------------
			//[MakeSingleClone::Manual]
			//return: 生成したCloneSharedPointer, Not Pointer = null (Used Single Thread, Manual Gabage Collection)
			//引数1: clone source pointer
			template<class PointerType, class CloneBaseType>
			inline SharedPointer<PointerType> Manual(WeakPointer<CloneBaseType>& cloneSourcePointer)
			{
				return std::move(CloneSharedPointer<PointerType>(cloneSourcePointer, false, false));
			}
			//----------------------------------------------------------------------------------
			//[MakeSingleClone::Manual]
			//return: 生成したCloneSharedPointer, Required UpdateManual (Used Single Thread, Manual Gabage Collection)
			//引数1: clone source pointer
			template<class PointerType, class CloneBaseType>
			inline SharedPointer<PointerType> Manual(const CloneSharedPointer<CloneBaseType>& cloneSourcePointer)
			{
				return std::move(CloneSharedPointer<PointerType>(cloneSourcePointer.getPointer(), false, false));
			}
			//----------------------------------------------------------------------------------
			//[MakeSingleClone::Manual]
			//return: 生成したCloneSharedPointer, Required UpdateManual (Used Single Thread, Manual Gabage Collection)
			//引数1: clone source pointer
			template<class PointerType, class CloneBaseType>
			inline SharedPointer<PointerType> Manual(CloneWeakPointer<CloneBaseType>& cloneSourcePointer)
			{
				LockWeakGuard<CloneWeakPointer<CloneBaseType>> guard(cloneSourcePointer);
				if (guard.getIsLocked())
					return std::move(CloneSharedPointer<PointerType>(cloneSourcePointer.TryGetInstance(), false, false));
				else
					return std::move(CloneSharedPointer<PointerType>());
			}
		}


		//Make Clone Shared (Used Multi Thread)
		namespace MakeMultiClone
		{
			//----------------------------------------------------------------------------------
			//[MakeMultiClone::Auto]
			//return: 生成したCloneSharedPointer, Required UpdateManual (Used Multi Thread, Auto Gabage Collection)
			//引数1: clone source pointer
			template<class PointerType, class CloneBaseType>
			inline SharedPointer<PointerType> Auto(const CloneBaseType* cloneSourcePointer)
			{
				return std::move(CloneSharedPointer<PointerType>(cloneSourcePointer, true, true));
			}
			//----------------------------------------------------------------------------------
			//[MakeMultiClone::Auto]
			//return: 生成したCloneSharedPointer (Used Multi Thread, Auto Gabage Collection)
			//引数1: clone source pointer
			template<class PointerType, class CloneBaseType>
			inline SharedPointer<PointerType> Auto(const UniquePointer<CloneBaseType>& cloneSourcePointer)
			{
				return std::move(CloneSharedPointer<PointerType>(cloneSourcePointer, true, true));
			}
			//----------------------------------------------------------------------------------
			//[MakeMultiClone::Auto]
			//return: 生成したCloneSharedPointer (Used Multi Thread, Auto Gabage Collection)
			//引数1: clone source pointer
			template<class PointerType, class CloneBaseType>
			inline SharedPointer<PointerType> Auto(const SharedPointer<CloneBaseType>& cloneSourcePointer)
			{
				return std::move(CloneSharedPointer<PointerType>(cloneSourcePointer, true, true));
			}
			//----------------------------------------------------------------------------------
			//[MakeMultiClone::Auto]
			//return: 生成したCloneSharedPointer, Not Pointer = null (Used Multi Thread, Auto Gabage Collection)
			//引数1: clone source pointer
			template<class PointerType, class CloneBaseType>
			inline SharedPointer<PointerType> Auto(WeakPointer<CloneBaseType>& cloneSourcePointer)
			{
				return std::move(CloneSharedPointer<PointerType>(cloneSourcePointer, true, true));
			}
			//----------------------------------------------------------------------------------
			//[MakeMultiClone::Auto]
			//return: 生成したCloneSharedPointer, Required UpdateManual (Used Multi Thread, Auto Gabage Collection)
			//引数1: clone source pointer
			template<class PointerType, class CloneBaseType>
			inline SharedPointer<PointerType> Auto(const CloneSharedPointer<CloneBaseType>& cloneSourcePointer)
			{
				return std::move(CloneSharedPointer<PointerType>(cloneSourcePointer.getPointer(), true, true));
			}
			//----------------------------------------------------------------------------------
			//[MakeMultiClone::Auto]
			//return: 生成したCloneSharedPointer, Required UpdateManual (Used Multi Thread, Auto Gabage Collection)
			//引数1: clone source pointer
			template<class PointerType, class CloneBaseType>
			inline SharedPointer<PointerType> Auto(CloneWeakPointer<CloneBaseType>& cloneSourcePointer)
			{
				LockWeakGuard<CloneWeakPointer<CloneBaseType>> guard(cloneSourcePointer);
				if (guard.getIsLocked())
					return std::move(CloneSharedPointer<PointerType>(cloneSourcePointer.TryGetInstance(), true, true));
				else
					return std::move(CloneSharedPointer<PointerType>());
			}


			//----------------------------------------------------------------------------------
			//[MakeMultiClone::Manual]
			//return: 生成したCloneSharedPointer, Required UpdateManual (Used Multi Thread, Manual Gabage Collection)
			//引数1: clone source pointer
			template<class PointerType, class CloneBaseType>
			inline SharedPointer<PointerType> Manual(const CloneBaseType* cloneSourcePointer)
			{
				return std::move(CloneSharedPointer<PointerType>(cloneSourcePointer, true, false));
			}
			//----------------------------------------------------------------------------------
			//[MakeMultiClone::Manual]
			//return: 生成したCloneSharedPointer (Used Multi Thread, Manual Gabage Collection)
			//引数1: clone source pointer
			template<class PointerType, class CloneBaseType>
			inline SharedPointer<PointerType> Manual(const UniquePointer<CloneBaseType>& cloneSourcePointer)
			{
				return std::move(CloneSharedPointer<PointerType>(cloneSourcePointer, true, false));
			}
			//----------------------------------------------------------------------------------
			//[MakeMultiClone::Manual]
			//return: 生成したCloneSharedPointer (Used Multi Thread, Manual Gabage Collection)
			//引数1: clone source pointer
			template<class PointerType, class CloneBaseType>
			inline SharedPointer<PointerType> Manual(const SharedPointer<CloneBaseType>& cloneSourcePointer)
			{
				return std::move(CloneSharedPointer<PointerType>(cloneSourcePointer, true, false));
			}
			//----------------------------------------------------------------------------------
			//[MakeMultiClone::Manual]
			//return: 生成したCloneSharedPointer, Not Pointer = null (Used Multi Thread, Manual Gabage Collection)
			//引数1: clone source pointer
			template<class PointerType, class CloneBaseType>
			inline SharedPointer<PointerType> Manual(WeakPointer<CloneBaseType>& cloneSourcePointer)
			{
				return std::move(CloneSharedPointer<PointerType>(cloneSourcePointer, true, false));
			}
			//----------------------------------------------------------------------------------
			//[MakeMultiClone::Manual]
			//return: 生成したCloneSharedPointer, Required UpdateManual (Used Multi Thread, Manual Gabage Collection)
			//引数1: clone source pointer
			template<class PointerType, class CloneBaseType>
			inline SharedPointer<PointerType> Manual(const CloneSharedPointer<CloneBaseType>& cloneSourcePointer)
			{
				return std::move(CloneSharedPointer<PointerType>(cloneSourcePointer.getPointer(), true, false));
			}
			//----------------------------------------------------------------------------------
			//[MakeMultiClone::Manual]
			//return: 生成したCloneSharedPointer, Required UpdateManual (Used Multi Thread, Manual Gabage Collection)
			//引数1: clone source pointer
			template<class PointerType, class CloneBaseType>
			inline SharedPointer<PointerType> Manual(CloneWeakPointer<CloneBaseType>& cloneSourcePointer)
			{
				LockWeakGuard<CloneWeakPointer<CloneBaseType>> guard(cloneSourcePointer);
				if (guard.getIsLocked())
					return std::move(CloneSharedPointer<PointerType>(cloneSourcePointer.TryGetInstance(), true, false));
				else
					return std::move(CloneSharedPointer<PointerType>());
			}
		}


		//Make Clone Shared[Array] (Used Single Thread)
		namespace MakeSingleCloneArray
		{
			//----------------------------------------------------------------------------------
			//[MakeSingleCloneArray::Auto]
			//return: 生成したCloneSharedPointer[Array], Required UpdateManual (Used Single Thread, Auto Gabage Collection)
			//引数1: clone source pointer
			//引数2: 配列サイズ
			template<class PointerType, class CloneBaseType>
			inline SharedPointer<PointerType[]> Auto(const CloneBaseType* cloneSourcePointer, uint arraySize)
			{
				return std::move(CloneSharedPointer<PointerType[]>(cloneSourcePointer, arraySize, false, true));
			}
			//----------------------------------------------------------------------------------
			//[MakeSingleCloneArray::Auto]
			//return: 生成したCloneSharedPointer[Array] (Used Single Thread, Auto Gabage Collection)
			//引数1: clone source pointer
			template<class PointerType, class CloneBaseType>
			inline SharedPointer<PointerType[]> Auto(const UniquePointer<CloneBaseType[]>& cloneSourcePointer)
			{
				return std::move(CloneSharedPointer<PointerType[]>(cloneSourcePointer, false, true));
			}
			//----------------------------------------------------------------------------------
			//[MakeSingleCloneArray::Auto]
			//return: 生成したCloneSharedPointer[Array] (Used Single Thread, Auto Gabage Collection)
			//引数1: clone source pointer
			template<class PointerType, class CloneBaseType>
			inline SharedPointer<PointerType[]> Auto(const SharedPointer<CloneBaseType[]>& cloneSourcePointer)
			{
				return std::move(CloneSharedPointer<PointerType[]>(cloneSourcePointer, false, true));
			}
			//----------------------------------------------------------------------------------
			//[MakeSingleCloneArray::Auto]
			//return: 生成したCloneSharedPointer[Array], Not Pointer = null (Used Single Thread, Auto Gabage Collection)
			//引数1: clone source pointer
			template<class PointerType, class CloneBaseType>
			inline SharedPointer<PointerType[]> Auto(WeakPointer<CloneBaseType[]>& cloneSourcePointer)
			{
				return std::move(CloneSharedPointer<PointerType[]>(cloneSourcePointer, false, true));
			}
			//----------------------------------------------------------------------------------
			//[MakeSingleCloneArray::Auto]
			//return: 生成したCloneSharedPointer[Array], Required UpdateManual (Used Single Thread, Auto Gabage Collection)
			//引数1: clone source pointer
			template<class PointerType, class CloneBaseType>
			inline SharedPointer<PointerType[]> Auto(const CloneSharedPointer<CloneBaseType[]>& cloneSourcePointer)
			{
				return std::move(CloneSharedPointer<PointerType[]>(cloneSourcePointer.getPointer(), 
					cloneSourcePointer.getArraySize(), false, true));
			}
			//----------------------------------------------------------------------------------
			//[MakeSingleCloneArray::Auto]
			//return: 生成したCloneSharedPointer[Array], Required UpdateManual (Used Single Thread, Auto Gabage Collection)
			//引数1: clone source pointer
			template<class PointerType, class CloneBaseType>
			inline SharedPointer<PointerType[]> Auto(CloneWeakPointer<CloneBaseType[]>& cloneSourcePointer)
			{
				LockWeakGuard<CloneWeakPointer<CloneBaseType[]>> guard(cloneSourcePointer);
				if (guard.getIsLocked())
					return std::move(CloneSharedPointer<PointerType[]>(cloneSourcePointer.TryGetInstance(),
						cloneSourcePointer.getArraySize(), false, true));
				else
					return std::move(CloneSharedPointer<PointerType[]>());
			}


			//----------------------------------------------------------------------------------
			//[MakeSingleCloneArray::Manual]
			//return: 生成したCloneSharedPointer[Array], Required UpdateManual (Used Single Thread, Manual Gabage Collection)
			//引数1: clone source pointer
			//引数2: 配列サイズ
			template<class PointerType, class CloneBaseType>
			inline SharedPointer<PointerType[]> Manual(const CloneBaseType* cloneSourcePointer, uint arraySize)
			{
				return std::move(CloneSharedPointer<PointerType[]>(cloneSourcePointer, arraySize, false, false));
			}
			//----------------------------------------------------------------------------------
			//[MakeSingleCloneArray::Manual]
			//return: 生成したCloneSharedPointer[Array], (Used Single Thread, Manual Gabage Collection)
			//引数1: clone source pointer
			template<class PointerType, class CloneBaseType>
			inline SharedPointer<PointerType[]> Manual(const UniquePointer<CloneBaseType[]>& cloneSourcePointer)
			{
				return std::move(CloneSharedPointer<PointerType[]>(cloneSourcePointer, false, false));
			}
			//----------------------------------------------------------------------------------
			//[MakeSingleCloneArray::Manual]
			//return: 生成したCloneSharedPointer[Array], (Used Single Thread, Manual Gabage Collection)
			//引数1: clone source pointer
			template<class PointerType, class CloneBaseType>
			inline SharedPointer<PointerType[]> Manual(const SharedPointer<CloneBaseType[]>& cloneSourcePointer)
			{
				return std::move(CloneSharedPointer<PointerType[]>(cloneSourcePointer, false, false));
			}
			//----------------------------------------------------------------------------------
			//[MakeSingleCloneArray::Manual]
			//return: 生成したCloneSharedPointer[Array], Not Pointer = null (Used Single Thread, Manual Gabage Collection)
			//引数1: clone source pointer
			template<class PointerType, class CloneBaseType>
			inline SharedPointer<PointerType[]> Manual(WeakPointer<CloneBaseType[]>& cloneSourcePointer)
			{
				return std::move(CloneSharedPointer<PointerType[]>(cloneSourcePointer, false, false));
			}
			//----------------------------------------------------------------------------------
			//[MakeSingleCloneArray::Manual]
			//return: 生成したCloneSharedPointer[Array], Required UpdateManual (Used Single Thread, Manual Gabage Collection)
			//引数1: clone source pointer
			template<class PointerType, class CloneBaseType>
			inline SharedPointer<PointerType[]> Manual(const CloneSharedPointer<CloneBaseType[]>& cloneSourcePointer)
			{
				return std::move(CloneSharedPointer<PointerType[]>(cloneSourcePointer.getPointer(), 
					cloneSourcePointer.getArraySize(), false, false));
			}
			//----------------------------------------------------------------------------------
			//[MakeSingleCloneArray::Manual]
			//return: 生成したCloneSharedPointer[Array], Required UpdateManual (Used Single Thread, Manual Gabage Collection)
			//引数1: clone source pointer
			template<class PointerType, class CloneBaseType>
			inline SharedPointer<PointerType[]> Manual(CloneWeakPointer<CloneBaseType[]>& cloneSourcePointer)
			{
				LockWeakGuard<CloneWeakPointer<CloneBaseType[]>> guard(cloneSourcePointer);
				if (guard.getIsLocked())
					return std::move(CloneSharedPointer<PointerType[]>(cloneSourcePointer.TryGetInstance(),
						cloneSourcePointer.getArraySize(), false, false));
				else
					return std::move(CloneSharedPointer<PointerType[]>());
			}
		}



		//Make Clone Shared[Array] (Used Multi Thread)
		namespace MakeMultiCloneArray
		{
			//----------------------------------------------------------------------------------
			//[MakeMultiCloneArray::Auto]
			//return: 生成したCloneSharedPointer[Array], Required UpdateManual (Used Multi Thread, Auto Gabage Collection)
			//引数1: clone source pointer
			//引数2: 配列サイズ
			template<class PointerType, class CloneBaseType>
			inline SharedPointer<PointerType[]> Auto(const CloneBaseType* cloneSourcePointer, uint arraySize)
			{
				return std::move(CloneSharedPointer<PointerType[]>(cloneSourcePointer, arraySize, true, true));
			}
			//----------------------------------------------------------------------------------
			//[MakeMultiCloneArray::Auto]
			//return: 生成したCloneSharedPointer[Array] (Used Multi Thread, Auto Gabage Collection)
			//引数1: clone source pointer
			template<class PointerType, class CloneBaseType>
			inline SharedPointer<PointerType[]> Auto(const UniquePointer<CloneBaseType[]>& cloneSourcePointer)
			{
				return std::move(CloneSharedPointer<PointerType[]>(cloneSourcePointer, true, true));
			}
			//----------------------------------------------------------------------------------
			//[MakeMultiCloneArray::Auto]
			//return: 生成したCloneSharedPointer[Array] (Used Multi Thread, Auto Gabage Collection)
			//引数1: clone source pointer
			template<class PointerType, class CloneBaseType>
			inline SharedPointer<PointerType[]> Auto(const SharedPointer<CloneBaseType[]>& cloneSourcePointer)
			{
				return std::move(CloneSharedPointer<PointerType[]>(cloneSourcePointer, true, true));
			}
			//----------------------------------------------------------------------------------
			//[MakeMultiCloneArray::Auto]
			//return: 生成したCloneSharedPointer[Array], Not Pointer = null (Used Multi Thread, Auto Gabage Collection)
			//引数1: clone source pointer
			template<class PointerType, class CloneBaseType>
			inline SharedPointer<PointerType[]> Auto(WeakPointer<CloneBaseType[]>& cloneSourcePointer)
			{
				return std::move(CloneSharedPointer<PointerType[]>(cloneSourcePointer, true, true));
			}
			//----------------------------------------------------------------------------------
			//[MakeMultiCloneArray::Auto]
			//return: 生成したCloneSharedPointer[Array], Required UpdateManual (Used Multi Thread, Auto Gabage Collection)
			//引数1: clone source pointer
			template<class PointerType, class CloneBaseType>
			inline SharedPointer<PointerType[]> Auto(const CloneSharedPointer<CloneBaseType[]>& cloneSourcePointer)
			{
				return std::move(CloneSharedPointer<PointerType[]>(cloneSourcePointer.getPointer(),
					cloneSourcePointer.getArraySize(), true, true));
			}
			//----------------------------------------------------------------------------------
			//[MakeMultiCloneArray::Auto]
			//return: 生成したCloneSharedPointer[Array], Required UpdateManual (Used Multi Thread, Auto Gabage Collection)
			//引数1: clone source pointer
			template<class PointerType, class CloneBaseType>
			inline SharedPointer<PointerType[]> Auto(CloneWeakPointer<CloneBaseType[]>& cloneSourcePointer)
			{
				LockWeakGuard<CloneWeakPointer<CloneBaseType[]>> guard(cloneSourcePointer);
				if (guard.getIsLocked())
					return std::move(CloneSharedPointer<PointerType[]>(cloneSourcePointer.TryGetInstance(),
						cloneSourcePointer.getArraySize(), true, true));
				else
					return std::move(CloneSharedPointer<PointerType[]>());
			}


			//----------------------------------------------------------------------------------
			//[MakeMultiCloneArray::Manual]
			//return: 生成したCloneSharedPointer[Array], Required UpdateManual (Used Multi Thread, Manual Gabage Collection)
			//引数1: clone source pointer
			//引数2: 配列サイズ
			template<class PointerType, class CloneBaseType>
			inline SharedPointer<PointerType[]> Manual(const CloneBaseType* cloneSourcePointer, uint arraySize)
			{
				return std::move(CloneSharedPointer<PointerType[]>(cloneSourcePointer, arraySize, true, false));
			}
			//----------------------------------------------------------------------------------
			//[MakeMultiCloneArray::Manual]
			//return: 生成したCloneSharedPointer[Array], (Used Multi Thread, Manual Gabage Collection)
			//引数1: clone source pointer
			template<class PointerType, class CloneBaseType>
			inline SharedPointer<PointerType[]> Manual(const UniquePointer<CloneBaseType[]>& cloneSourcePointer)
			{
				return std::move(CloneSharedPointer<PointerType[]>(cloneSourcePointer, true, false));
			}
			//----------------------------------------------------------------------------------
			//[MakeMultiCloneArray::Manual]
			//return: 生成したCloneSharedPointer[Array], (Used Multi Thread, Manual Gabage Collection)
			//引数1: clone source pointer
			template<class PointerType, class CloneBaseType>
			inline SharedPointer<PointerType[]> Manual(const SharedPointer<CloneBaseType[]>& cloneSourcePointer)
			{
				return std::move(CloneSharedPointer<PointerType[]>(cloneSourcePointer, true, false));
			}
			//----------------------------------------------------------------------------------
			//[MakeMultiCloneArray::Manual]
			//return: 生成したCloneSharedPointer[Array], Not Pointer = null (Used Multi Thread, Manual Gabage Collection)
			//引数1: clone source pointer
			template<class PointerType, class CloneBaseType>
			inline SharedPointer<PointerType[]> Manual(WeakPointer<CloneBaseType[]>& cloneSourcePointer)
			{
				return std::move(CloneSharedPointer<PointerType[]>(cloneSourcePointer, true, false));
			}
			//----------------------------------------------------------------------------------
			//[MakeMultiCloneArray::Manual]
			//return: 生成したCloneSharedPointer[Array], Required UpdateManual (Used Multi Thread, Manual Gabage Collection)
			//引数1: clone source pointer
			template<class PointerType, class CloneBaseType>
			inline SharedPointer<PointerType[]> Manual(const CloneSharedPointer<CloneBaseType[]>& cloneSourcePointer)
			{
				return std::move(CloneSharedPointer<PointerType[]>(cloneSourcePointer.getPointer(),
					cloneSourcePointer.getArraySize(), true, false));
			}
			//----------------------------------------------------------------------------------
			//[MakeMultiCloneArray::Manual]
			//return: 生成したCloneSharedPointer[Array], Required UpdateManual (Used Multi Thread, Manual Gabage Collection)
			//引数1: clone source pointer
			template<class PointerType, class CloneBaseType>
			inline SharedPointer<PointerType[]> Manual(CloneWeakPointer<CloneBaseType[]>& cloneSourcePointer)
			{
				LockWeakGuard<CloneWeakPointer<CloneBaseType[]>> guard(cloneSourcePointer);
				if (guard.getIsLocked())
					return std::move(CloneSharedPointer<PointerType[]>(cloneSourcePointer.TryGetInstance(),
						cloneSourcePointer.getArraySize(), true, false));
				else
					return std::move(CloneSharedPointer<PointerType[]>());
			}
		}
	}
}
#endif // !SGFRAMEWORK_HEADER_POINTER_HPP_