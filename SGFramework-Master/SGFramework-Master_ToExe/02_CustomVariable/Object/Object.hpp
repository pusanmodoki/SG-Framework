/*----------------------------------------------------------------------------------
Object系クラスの基底となるObject class
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_OBJECT_HPP_
#define SGFRAMEWORK_HEADER_OBJECT_HPP_
#include "../String/String.hpp"
#include "../Property/Property.hpp"
#include "../ConstAndUsing/ConstAndUsing.hpp"

//Framework namespace
namespace SGFramework
{
	class GameObject;
	//Base Classes
	namespace BaseClass
	{
		class BaseAsset;
	}

	//フレームワークの管理をするAdministrator namespace
	namespace Administrator
	{
		//GraphicsDetail namespace
		namespace GraphicsDetail
		{
			class TransformX11;
		}
	}
	//Object系クラスの基底となるObject class
	class Object
	{
	public:
		//friend
		friend class GameObject;
		friend class BaseClass::BaseAsset;
		friend class Administrator::GraphicsDetail::TransformX11;

		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//初期化を行う
		//引数1: オブジェクト名
		inline Object(const sgstring& name);
		//----------------------------------------------------------------------------------
		//[デストラクタ]
		inline virtual ~Object() { AtomicOperation::Subtract(getNowCounter(), 1); }

		//all instantiate objects<static> (read function property)
		SGF_FUNCTION_PROPERTY static uint getNumTotaleInstantiateObjects() { return getMasterID().load(); }
		//all instantiate objects(now)<static> (read function property)
		SGF_FUNCTION_PROPERTY static uint getNumNowTotaleInstantiateObjects() { return getNowCounter().load(); }

		//set object name, using with care!! (set function property)
		//argument 1: name
		SGF_FUNCTION_PROPERTY void setName(const sgstring& name) { m_name = name; }

		// == operator
		inline bool operator ==(const Object& obj);
		// != operator
		inline bool operator !=(const Object& obj);

		// == operator (template pointer function)
		template <class Left, class Right>
		inline static bool IsEqualPointer(const Left& left, const Right& right);
		// != operator (template pointer function)
		template <class Left, class Right>
		inline static bool IsNotEqualPointer(const Left& left, const Right& right);

		GetOnlyProperty<sgstring> name;		//オブジェクト名
		GetOnlyProperty<uint> instanceID;		//インスタンスID
	
	protected:
		//----------------------------------------------------------------------------------
		//[CreateCopyName]
		//return: Copy Object用Name
		//引数1: オブジェクト名
		inline static sgstring CreateCopyName(const sgstring& name);
	
	private:
		//----------------------------------------------------------------------------------
		//[RegisterMyInstanceID]
		//return: 自身のID
		inline uint RegisterMyInstanceID();

		//object instance master id<static> (reference property)
		SGF_FUNCTION_PROPERTY static std::atomic<uint>& getMasterID() { static std::atomic<uint> masterID = 0;  return masterID; }
		//num now object <static> (reference property)
		SGF_FUNCTION_PROPERTY static std::atomic<uint>& getNowCounter() { static std::atomic<uint> counter = 0;  return counter; }

		sgstring m_name;		//オブジェクト名
		uint m_instanceID;		//インスタンスID
	};

	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//初期化を行う
	//引数1: オブジェクト名
	inline Object::Object(const sgstring & name) : name(m_name), 
		instanceID(m_instanceID), m_name(name), m_instanceID(RegisterMyInstanceID()) 
	{
		AtomicOperation::Add(getNowCounter(), 1);
	}
	
	// == operator
	inline bool Object::operator==(const Object & obj) { return (this->instanceID == obj.instanceID); }
	// != operator
	inline bool Object::operator!=(const Object & obj) { return (this->instanceID != obj.instanceID); }

	// == operator (template pointer function)
	template<class Left, class Right>
	inline bool Object::IsEqualPointer(const Left& left, const Right& right)
	{
		return (left->instanceID == right->instanceID);
	}
	// != operator (template pointer function)
	template<class Left, class Right>
	inline bool Object::IsNotEqualPointer(const Left& left, const Right& right)
	{
		return (left->instanceID != right->instanceID);
	}

	//----------------------------------------------------------------------------------
	//[CreateCopyName]
	//return: Copy Object用Name
	//引数1: オブジェクト名
	inline sgstring Object::CreateCopyName(const sgstring& name)
	{
		//result
		sgstring result = name;

		//恐らくコピー済み
		if (result.back() == L')' && result[result.length() - 3] == L'(')
		{
			//一旦文字列で受け取る
			sgstring number = result.substr(result.length() - 3, 1);
			//数字??
			if (number.my_functions.IsFirstOfNumbers())
			{
				//加算
				number = number.my_functions.ToInt() + 1;
				result.pop_back();
				result += number + L")";
				return result;
			}
		}
		//恐らく初コピー
		result += L" (1)";
		return result;
	}

	//----------------------------------------------------------------------------------
	//[RegisterMyInstanceID]
	//return: 自身のID
	inline uint Object::RegisterMyInstanceID()
	{
		return AtomicOperation::Add(getMasterID(), 1);
	}
}
#endif // !SGFRAMEWORK_HEADER_OBJECT_HPP_
