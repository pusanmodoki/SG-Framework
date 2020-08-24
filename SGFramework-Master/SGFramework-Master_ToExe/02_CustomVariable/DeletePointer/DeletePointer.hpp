/*----------------------------------------------------------------------------------
ポインタをdeleteするためだけに作成したDeletePointer.h
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_DELETE_POINTER_HPP_
#define SGFRAMEWORK_HEADER_DELETE_POINTER_HPP_
#include <vector>
#include <list>
#include <map>
#include <deque>
#include <Windows.h>
#include <algorithm>

//Framework namespace
namespace SGFramework
{
	//削除するための関数を格納するnamespace Deleting
	namespace Deleteing
	{
		//プロトタイプ宣言
		//単体か配列か選択
		template<class T>
		inline void DeletePointer(T *& pt, const bool isArray);
		//単体版
		template<class T>
		inline void DeletePointer(T *& pt);
		//配列版
		template<class T>
		inline void DeleteArrayPointer(T *& pt);
		//vector単体版
		template<class T>
		inline void DeletePointer(std::vector<T*>& pt);
		//deque単体版
		template<class T>
		inline void DeletePointer(std::deque<T*>& pt);
		//list単体版
		template<class T>
		inline void DeletePointer(std::list<T*>& pt);
		//map単体版
		template<class T>
		inline void DeletePointer(std::map<T, T*>& pt);

		//----------------------------------------------------------------------------------
		//[DeletePointer]
		//ポインターをdeleteし、NULLを代入する
		//引数1: 削除するポインタ(参照)
		//引数2: 配列かどうか
		template<class T>
		inline void DeletePointer(T *& pt, const bool isArray)
		{
			//delete
			if (IS_FALSE(isArray))	delete pt;
			else							delete[] pt;
			pt = nullptr;		//NULL代入
		}
		//----------------------------------------------------------------------------------
		//[DeletePointer]
		//ポインターをdeleteし、NULLを代入する
		//※配列でない場合のみの対応, 配列はオーバーロードかDeleteArrayPointer
		//引数1: 削除するポインタ(参照)
		template<class T>
		inline void DeletePointer(T *& pt)
		{
			delete pt;			//delete
			pt = nullptr;		//NULL代入
		}
		//----------------------------------------------------------------------------------
		//[DeleteArrayPointer]
		//ポインターをdeleteし、NULLを代入する
		//※配列の場合のみの対応, 配列でない場合はDeletePointer
		//引数1: 削除するポインタ(参照)
		template<class T>
		inline void DeleteArrayPointer(T *& pt)
		{
			delete[] pt;		//delete
			pt = nullptr;		//NULL代入
		}

		//----------------------------------------------------------------------------------
		//[DeletePointer]
		//ポインターをdeleteし、リストをクリアする
		//※要素にnewしたポインタは単体であること
		//引数1: 削除するポインタリスト(参照)
		template<class T>
		inline void DeletePointer(std::vector<T*>& pt)
		{
			for (T*& t : pt) delete t;
			pt.clear();
		}
		//----------------------------------------------------------------------------------
		//[DeletePointer]
		//ポインターをdeleteし、リストをクリアする
		//※要素にnewしたポインタは単体であること
		//引数1: 削除するポインタリスト(参照)
		template<class T>
		void DeletePointer(std::deque<T*>& pt)
		{
			for (T*& t : pt) delete t;
			pt.clear();
		}
		//----------------------------------------------------------------------------------
		//[DeletePointer]
		//ポインターをdeleteし、リストをクリアする
		//※要素にnewしたポインタは単体であること
		//引数1: 削除するポインタリスト(参照)
		template<class T>
		inline void DeletePointer(std::list<T*>& pt)
		{
			for (T*& t : pt) delete t;
			pt.clear();
		}
		//----------------------------------------------------------------------------------
		//[DeletePointer]
		//ポインターをdeleteし、リストをクリアする
		//※要素にnewしたポインタは単体であること
		//引数1: 削除するポインタリスト(参照)
		template<class T>
		inline void DeletePointer(std::map<T, T*>& pt)
		{
			for (T*& t : pt) delete t;
			pt.clear();
		}
	}
}
#endif // !SGFRAMEWORK_HEADER_DELETE_POINTER_HPP_