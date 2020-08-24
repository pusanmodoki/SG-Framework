/*----------------------------------------------------------------------------------
ConditionVariable系クラスのベースとなるBaseConditionVariable class
----------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_COM_WEAR_HPP_
#define SGFRAMEWORK_HEADER_COM_WEAR_HPP_
#include "../ConstAndUsing/ConstAndUsing.hpp"
#include "../Atomic/Atomic.hpp"

//Framework namespace
namespace SGFramework
{
	//Base Classes
	namespace BaseClass
	{
		//PointerをCOM化させるBaseComWear class
		//template 1: COM化させるポインターの型
		template<class WearClass>
		class BaseComWear
		{
		public:
			//----------------------------------------------------------------------------------
			//[AddRef]
			//カウンタを加算させる
			//return: 実行後のカウンタ数, ポインタが無効だった場合null
			inline uint AddRef() { if (m_pointer == nullptr) return 0; return AtomicOperation::Add(m_counter, 1); }

			//<property> com pointer
			SGF_PROPERTY WearClass* _getPointer() const { return m_pointer; }
			//<property> now refecence count
			SGF_PROPERTY WearClass* _getNumReference() const { return m_counter != nullptr ? m_counter->load() : 0; }
			//<property> pointer is valid(有効)?
			SGF_PROPERTY bool _getIsValid() const { return m_counter != nullptr && *m_counter > 0; }
			
			//pointer operator
			inline operator WearClass* () const { return m_pointer; }
			//pointer operator
			inline WearClass* operator -> () const { return m_pointer; }
			//pointer operator
			inline WearClass* operator () () const { return m_pointer; }
			//pointer operator
			inline WearClass& operator * () const { return *m_pointer; }

		protected:
			//----------------------------------------------------------------------------------
			//[コンストラクタ]
			BaseComWear() : m_pointer(nullptr), m_counter(nullptr) {}

			//----------------------------------------------------------------------------------
			//[InitBaseMembers]
			//ポインタ, カウンタ初期化
			//引数1: ポインタ
			inline void InitBaseMembers(WearClass* pointer)
			{
				m_pointer = pointer;
				m_counter = pointer != nullptr ? new std::atomic<uint>(1) : nullptr;
			}
			//----------------------------------------------------------------------------------
			//[UnInitBaseMembers]
			//カウンタ削除
			inline void UnInitBaseMembers() { DELETE_POINTER(m_counter); }

			WearClass* m_pointer;					//ポインタ
			std::atomic<uint>* m_counter;		//カウンタ
		};
	}

	//PointerをCOM化させ、Release時にdeleteするComWearDelete class
	//template 1: COM化させるポインターの型
	template<class WearClass>
	class ComWearDelete : public BaseClass::BaseComWear<WearClass>
	{
	public:
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//引数1: ポインタ
		inline ComWearDelete(WearClass* pointer) { this->InitBaseMembers(pointer); }

		//----------------------------------------------------------------------------------
		//[Init]
		//Release実行後初期化を行う, 処理上リークが起こる可能性あり, 注意されたし
		//引数1: ポインタ
		//引数2: now pointer is array?, default = false
		inline void Init(WearClass* pointer, bool isArray = false)
		{
			Release(isArray, true);
			this->InitBaseMembers(pointer);
		}

		//----------------------------------------------------------------------------------
		//[Release]
		//カウンタを減算させ、0になった場合deleteで開放を行う
		//return: 実行後のカウンタ数, ポインタが無効だった場合null
		//引数1: array pointer?, default = false
		//引数2: カウンタが0にならなかった場合も強制的にnullptrを代入するか, default = false
		inline uint Release(bool isArray = false, bool isForceInitNull = false)
		{ 
			//null?
			if (this->m_pointer == nullptr) return 0;

			//カウンタ減算, 0ならdelete
			uint result = AtomicOperation::Subtract(*this->m_counter, 1);
			if (result == 0)
			{
				this->UnInitBaseMembers();
				if (IS_FALSE(isArray)) delete this->m_pointer;
				else delete[] this->m_pointer;
				this->m_pointer = nullptr;
			}
			else if (IS_TRUE(isForceInitNull))
			{
				this->m_pointer = nullptr;
				this->m_counter = nullptr;
			}

			return result;
		}
	};

	//PointerをCOM化させ、Release時にReleaseFunctionを実行するComWearFunction class
	//template 1: COM化させるポインターの型
	template<class WearClass>
	class ComWearFunction : public BaseClass::BaseComWear<WearClass>
	{
		template<class WearClass>
		struct BaseFunctionCaller;
		template<class WearClass, typename ReleaseFunction>
		struct FunctionCaller;
	public:
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		inline ComWearFunction() : m_function(nullptr) {}
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//template 1: ポインターを開放させるメンバ関数
		//引数1: ポインタ
		//引数2: 開放させる関数ポインタ
		template<typename ReleaseFunction>
		inline ComWearFunction(WearClass* pointer, ReleaseFunction function)
			: m_function(new FunctionCaller<WearClass, ReleaseFunction>(function)) { this->InitBaseMembers(pointer); }

		//----------------------------------------------------------------------------------
		//[Init]
		//Release実行後初期化を行う, 処理上リークが起こる可能性あり, 注意されたし
		//template 1: ポインターを開放させるメンバ関数
		//引数1: ポインタ
		//引数2: 開放させる関数ポインタ
		template<typename ReleaseFunction>
		inline void Init(WearClass* pointer, ReleaseFunction function)
		{
			Release(true);
			m_function = new FunctionCaller<WearClass, ReleaseFunction>(function);
			this->InitBaseMembers(pointer);
		}

		//----------------------------------------------------------------------------------
		//[Release]
		//カウンタを減算させ、0になった場合ReleaseFunctionで開放を行う
		//return: 実行後のカウンタ数, ポインタが無効だった場合null
		//引数1: カウンタが0にならなかった場合も強制的にnullptrを代入するか, false推奨
		inline uint Release(bool isForceInitNull)
		{
			if (this->m_pointer == nullptr) return 0;

			//カウンタ減算, 0なら関数実行
			uint result = AtomicOperation::Subtract(*this->m_counter, 1);
			if (result == 0)
			{
				this->UnInitBaseMembers();
				m_function->Release(this->m_pointer);
				delete m_function;
				m_function = nullptr;
			}
			else if (IS_TRUE(isForceInitNull))
			{
				this->m_pointer = nullptr;
				this->m_counter = nullptr;
				this->m_function = nullptr;
			}

			return result;
		}

	private:
		//関数保存用クラス
		template<class WearClass>
		struct BaseFunctionCaller
		{
			//コンストラクタ
			inline virtual ~BaseFunctionCaller() {}
			//開放
			virtual void Release(WearClass*& pointer) = 0;
		};
		//関数保存用クラス
		template<class WearClass, typename ReleaseFunction>
		struct FunctionCaller : public BaseFunctionCaller<WearClass>
		{
			//コンストラクタ
			inline FunctionCaller(ReleaseFunction function) : function(function) {}
			//デストラクタ
			inline ~FunctionCaller() {}

			//開放
			inline void Release(WearClass*& pointer) override
			{
				(pointer->*function)();
				pointer = nullptr;
			}

			//関数ポインタ
			ReleaseFunction function;
		};

		BaseFunctionCaller<WearClass>* m_function;		//開放用メンバ関数ポインタ
	};
}

#endif //!SGFRAMEWORK_HEADER_COM_WEAR_HPP_