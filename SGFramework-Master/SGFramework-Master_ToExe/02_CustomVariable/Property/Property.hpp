/*----------------------------------------------------------------------------------
C#�̃v���p�e�B�I�ȐU�镑�����ł���Property.h
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_PROPERTY_HPP_
#define SGFRAMEWORK_HEADER_PROPERTY_HPP_

//Framework namespace
namespace SGFramework
{
	//get, set���ł���AccessorProperty class
	template<typename T>
	class AccessorProperty final
	{
	public:
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//����1: �Q�Ƃ���ϐ�
		inline AccessorProperty(T& t) : m_value(t) {}

		//----------------------------------------------------------------------------------
		// get operator
		inline operator T& ()  { return m_value; }

		//----------------------------------------------------------------------------------
		// get operator
		inline T& operator () () { return m_value; }

		//----------------------------------------------------------------------------------
		// get operator
		inline T* operator -> () { return &m_value; }

		//have value reference (get function property)
		SGF_FUNCTION_PROPERTY T& getValue() { return m_value; }

		//----------------------------------------------------------------------------------
		// set operator
		inline const T& operator = (const T& value) { m_value = value; return *this; }

	private:
		T& m_value;	//value
	};
	//get, set���ł���AccessorProperty class (pointer)
	template<typename T>
	class AccessorProperty<T*> final
	{
	public:
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//����1: �Q�Ƃ���ϐ�
		inline AccessorProperty(T*& t) : m_value(t) {}

		//----------------------------------------------------------------------------------
		// get operator
		inline operator T* () { return m_value; }

		//----------------------------------------------------------------------------------
		// get operator
		inline T* operator () () { return m_value; }

		//----------------------------------------------------------------------------------
		// get operator
		inline T* operator -> () { return m_value; }

		//have value reference (get function property)
		SGF_FUNCTION_PROPERTY T* getValue() { return m_value; }

		//----------------------------------------------------------------------------------
		// set operator
		inline const T*& operator = (const T* value) { m_value = value; return m_value; }

	private:
		T* m_value;	//value
	};

	//get, set���ł�, �A�N�Z�X�t���O������AccessorPropertyNotify class
	template<typename T>
	class AccessorPropertyNotify final
	{
	public:
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//����1: �Q�Ƃ���ϐ�
		inline AccessorPropertyNotify(T& t, bool& notify) : m_value(t), m_isAccess(notify) {}

		//----------------------------------------------------------------------------------
		// get operator
		inline operator T& () { m_isAccess = true; return m_value; }

		//----------------------------------------------------------------------------------
		// get operator
		inline T& operator () () { m_isAccess = true;  return m_value; }

		//----------------------------------------------------------------------------------
		// get operator
		inline T* operator -> () { m_isAccess = true;  return &m_value; }

		//have value reference (get function property)
		SGF_FUNCTION_PROPERTY T& getValue() { m_isAccess = true;  return m_value; }

		//----------------------------------------------------------------------------------
		// set operator
		inline const T& operator = (const T& value) { m_isAccess = true; m_value = value; return *this; }

		//have value accessed? (get function property)
		SGF_FUNCTION_PROPERTY bool getIsAccessed() { return m_isAccess; }

	private:
		T& m_value;	//value
		bool& m_isAccess;
	};
	//get, set���ł�, �A�N�Z�X�t���O������AccessorPropertyNotify class(pointer)
	template<typename T>
	class AccessorPropertyNotify<T*> final
	{
	public:
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//����1: �Q�Ƃ���ϐ�
		inline AccessorPropertyNotify(T*& t, bool& notify) : m_value(t), m_isAccess(false) {}

		//----------------------------------------------------------------------------------
		// get operator
		inline operator T* () { m_isAccess = true; return m_value; }

		//----------------------------------------------------------------------------------
		// get operator
		inline T* operator () () { m_isAccess = true; return m_value; }

		//----------------------------------------------------------------------------------
		// get operator
		inline T* operator -> () { m_isAccess = true;  return m_value; }

		//have value reference (get function property)
		SGF_FUNCTION_PROPERTY T* getValue() { m_isAccess = true; return m_value; }

		//----------------------------------------------------------------------------------
		// set operator
		inline const T& operator = (const T& value) { m_isAccess = true; *m_value = value; return *m_value; }

		//have value accessed? (get function property)
		SGF_FUNCTION_PROPERTY bool getIsAccessed() { return m_isAccess; }

	private:
		T* m_value;	//value
		bool& m_isAccess;
	};

	//get���ł���GetOnlyProperty class
	template<typename T>
	class GetOnlyProperty final
	{
	public:
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//����1: �Q�Ƃ���ϐ�
		inline GetOnlyProperty(T& t) : m_value(t) {}

		//----------------------------------------------------------------------------------
		// get operator
		inline operator const T& () const { return m_value; }
		
		//----------------------------------------------------------------------------------
		// get operator
		inline const T& operator () ()const { return m_value; }

		//----------------------------------------------------------------------------------
		// get operator
		inline const T* operator -> () const { return &m_value; }

		//have value reference (get function property)
		SGF_FUNCTION_PROPERTY const T& getValue() const {  return m_value; }

	private:
		//----------------------------------------------------------------------------------
		// set operator
		inline void operator = (const T& value) {}

		T& m_value;	//value
	};
	//get���ł���GetOnlyProperty class (Pointer)
	template<typename T>
	class GetOnlyProperty<T*> final
	{
	public:
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//����1: �Q�Ƃ���ϐ�
		inline GetOnlyProperty(T*& t) : m_value(t) {}

		//----------------------------------------------------------------------------------
		// get operator
		inline operator const T* () const { return m_value; }

		//----------------------------------------------------------------------------------
		// get operator
		inline const T* operator () ()const { return m_value; }

		//----------------------------------------------------------------------------------
		// get operator
		inline const T* operator -> () const { return &m_value; }

		//have value reference (get function property)
		SGF_FUNCTION_PROPERTY const T* getValue() const { return m_value; }

	private:
		//----------------------------------------------------------------------------------
		// set operator
		inline void operator = (const T*& value) {}

		T* m_value;	//value
	};
	//get(Not Const)���ł���PointerProperty class
	template<typename T>
	class PointerProperty final
	{
	};

	//get(Not Const)���ł���PointerProperty class
	template<typename T>
	class PointerProperty<T*> final
	{
	public:
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//����1: �Q�Ƃ���ϐ�
		inline PointerProperty(T*& t) : m_value(t) {}

		//delete 
		PointerProperty<T*>& operator =(const PointerProperty<T*>& copy) = delete;
		PointerProperty<T*>& operator =(const PointerProperty<T*>&& move) = delete;

		//----------------------------------------------------------------------------------
		// get operator
		inline operator T* () const { return m_value; }

		//----------------------------------------------------------------------------------
		// get operator
		inline T* operator () ()const { return m_value; }

		//----------------------------------------------------------------------------------
		// get operator
		inline T* operator -> () const { return m_value; }

		//----------------------------------------------------------------------------------
		// get operator
		inline T& operator * () const { return *m_value; }

		//have value reference (get function property)
		SGF_FUNCTION_PROPERTY T* getValue() const { return m_value; }

	private:
		//----------------------------------------------------------------------------------
		// set operator
		inline void operator = (const T*& value) {}

		T*& m_value;	//value
	};

	//set���ł���SetOnlyProperty class
	template<typename T>
	class SetOnlyProperty final
	{
	public:
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//����1: �Q�Ƃ���ϐ�
		inline SetOnlyProperty(T& t) : m_value(t) {}

		//----------------------------------------------------------------------------------
		// set operator
		inline const T& operator = (const T value) { m_value = value; return m_value; }

	private:
		//----------------------------------------------------------------------------------
		// get operator
		inline operator T() {}

		T& m_value;	//value
	};
}
#endif // !SGFRAMEWORK_HEADER_PROPERTY_HPP_