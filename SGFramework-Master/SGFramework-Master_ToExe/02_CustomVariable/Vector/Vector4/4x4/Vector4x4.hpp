/*----------------------------------------------------------------------------------
4x4の値を扱えるVector4x4 構造体
最も多用する構造体シリーズの1つなので別自作Vectorが関係ないものは全てinline実装
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_VECTOR_VECTOR4X4_HPP_
#define SGFRAMEWORK_HEADER_VECTOR_VECTOR4X4_HPP_
#include <DirectXMath.h>
#include <PxPhysicsAPI.h>
#include "assimp/matrix3x3.h"
#include "assimp/matrix4x4.h"
#include "../Vector4.hpp"
#include "../../Vector3/Vector3.hpp"
#include "../../../GpuAccelerator/GpuAccelerator.hpp"

//Framework namespace
namespace SGFramework
{
	//プロトタイプ宣言
	struct Matrix;
	struct Quaternion;

	//4x4の値を扱えるVector4x4 構造体
	//Matrixはメンバ変数にしてはいけないので、その場合はこちらをどうぞ
	struct Vector4x4 final : public DirectX::XMFLOAT4X4
	{
		//----------------------------------------------------------------------------------
		//inlne実装

		
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//identityに設定される
		inline constexpr Vector4x4();
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//代入初期化する
		//引数1:XMFLOAT4x4
		inline constexpr Vector4x4(const DirectX::XMFLOAT4X4& value);
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//代入初期化する
		//引数1:PxMat33
		inline constexpr Vector4x4(const physx::PxMat33& value);
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//代入初期化する
		//引数1:PxMat44
		inline constexpr Vector4x4(const physx::PxMat44& value);
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//代入初期化する
		//引数1:aiMatrix3x3
		inline constexpr Vector4x4(const aiMatrix3x3& value);
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//代入初期化する
		//引数1:aiMatrix4x4
		inline constexpr Vector4x4(const aiMatrix4x4& value);
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//代入初期化する
		//引数1: Copy
		inline constexpr Vector4x4(const Vector4x4& copy);
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//代入初期化する
		//引数1: Move
		inline constexpr Vector4x4(Vector4x4&& move);
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//代入初期化する
		//引数1: Vector4_1
		//引数2: Vector4_2
		//引数3: Vector4_3
		//引数4: Vector4_4
		inline constexpr Vector4x4(const Vector4& value1, const Vector4& value2,
			const Vector4& value3, const Vector4& value4);
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//代入初期化する
		//引数1 ~ 16: value
		inline constexpr Vector4x4(float _11, float _12, float _13, float _14,
			float _21, float _22, float _23, float _24,
			float _31, float _32, float _33, float _34,
			float _41, float _42, float _43, float _44);
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//代入初期化する
		//引数1: XMMATRIX
		inline Vector4x4(const DirectX::XMMATRIX& value);
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//代入初期化する
		//引数1: float*
		inline Vector4x4(const float* array);
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//代入初期化する
		//引数1: double*
		inline Vector4x4(const double* array);

		//----------------------------------------------------------------------------------
		//[ToMatrix]
		//return: Matrix
		inline XmMatrix ToMatrix() const noexcept;
		//----------------------------------------------------------------------------------
		//[ToQuaternion]
		//return: Quaternion
		inline XmVector ToQuaternion() const noexcept;
		//----------------------------------------------------------------------------------
		//[ToPhysXMat33]
		//return: physx::PxMat33
		inline physx::PxMat33 ToPhysXMat33() const noexcept;
		//----------------------------------------------------------------------------------
		//[ToPhysXMat44]
		//return: physx::PxMat44
		inline physx::PxMat44 ToPhysXMat44() const noexcept;
		//----------------------------------------------------------------------------------
		//[ToAssimpMat33]
		//return: aiMatrix3x3
		inline aiMatrix3x3 ToAssimpMat33() const noexcept;
		//----------------------------------------------------------------------------------
		//[ToAssimpMat44]
		//return: aiMatrix4x4
		inline aiMatrix4x4 ToAssimpMat44() const noexcept;

		//----------------------------------------------------------------------------------
		//[ToMatrix]
		//引数1: apply
		inline void ToMatrix(XmMatrix& apply) const noexcept;
		//----------------------------------------------------------------------------------
		//[ToQuaternion]
		//引数1: apply
		inline void ToQuaternion(XmVector& apply) const noexcept;
		//----------------------------------------------------------------------------------
		//[ToPhysXMat33]
		//引数1: apply
		inline void ToPhysXMat33(physx::PxMat33& apply) const noexcept;
		//----------------------------------------------------------------------------------
		//[ToPhysXMat44]
		//引数1: apply
		inline void ToPhysXMat44(physx::PxMat44& apply) const noexcept;
		//----------------------------------------------------------------------------------
		//[ToAssimpMat33]
		//引数1: apply
		inline void ToAssimpMat33(aiMatrix3x3& apply) const noexcept;
		//----------------------------------------------------------------------------------
		//[ToAssimpMat44]
		//引数1: apply
		inline void ToAssimpMat44(aiMatrix4x4& apply) const noexcept;

		//----------------------------------------------------------------------------------
		//[Decompose]
		//行列情報を抽出し、nullptrでなければ各引数に代入
		//引数1: position (Vector3)
		//引数1: scale (Vector3)
		//引数1: rotation (Quaternion)
		inline void Decompose(DirectX::XMFLOAT3* position, DirectX::XMFLOAT3* scale, DirectX::XMFLOAT4* rotation) const;

		//inverse matrix(matrix convert) (get function property)
		SGF_FUNCTION_PROPERTY Vector4x4 getInverse() noexcept { return std::move(DirectX::XMMatrixInverse(nullptr, *this)); }
		//inverse matrix(matrix convert) (set function property)
		SGF_FUNCTION_PROPERTY const Vector4x4& setInverse() noexcept { return *this = std::move(DirectX::XMMatrixInverse(nullptr, *this)); }

		//get transpose matrix (get function property)
		SGF_FUNCTION_PROPERTY Vector4x4 getTranspose() const noexcept 
		{
			return std::move(Vector4x4(
				_11, _21, _31, _41,
				_12, _22, _32, _42,
				_13, _23, _33, _43,
				_14, _24, _34, _44));
		}
		//set transpose matrix (get function property)
		SGF_FUNCTION_PROPERTY const Vector4x4& setTranspose() noexcept
		{
			Vector4x4 copy(*this);
			_11 = copy._11; _12 = copy._21; _13 = copy._31; _14 = copy._41;
			_21 = copy._12; _22 = copy._22; _23 = copy._32; _24 = copy._42;
			_31 = copy._13; _32 = copy._23; _33 = copy._33; _34 = copy._43;
			_41 = copy._14; _42 = copy._24; _43 = copy._34; _44 = copy._44;
			return *this;
		}

		//this right direction (get function property)
		SGF_FUNCTION_PROPERTY constexpr Vector3 getRight() { return Vector3(_11, _12, _13); }
		//this up direction (get function property)
		SGF_FUNCTION_PROPERTY constexpr Vector3 getUp() { return Vector3(_21, _22, _23); }
		//this forward direction (get function property)
		SGF_FUNCTION_PROPERTY constexpr Vector3 getForward() { return Vector3(_31, _32, _33); }
		//this translate (get function property)
		SGF_FUNCTION_PROPERTY constexpr Vector3 getTranslate() { return Vector3(_41, _42, _43); }
		//this direction vectors (get function property)
		//argument 1: vec reference<-up
		//argument 2: vec reference<-right
		//argument 3: vec reference<-forward
		SGF_FUNCTION_PROPERTY constexpr void getDirections(Vector3& up, Vector3& right, Vector3& forward)
		{
			right.x = _11; right.y = _12; right.z = _13;
			up.x = _21; up.y = _22; up.z = _23;
			forward.x = _31; forward.y = _32; forward.z = _33;
		}

		//matrix[y]->Vector4 (get function property)
		//argument 1: y
		SGF_FUNCTION_PROPERTY constexpr Vector4 getVector4(int y) { return Vector4(m[y][0], m[y][1], m[y][2], m[y][3]); }
		//matrix[y]->Vector3 (get function property)
		//argument 1: y
		SGF_FUNCTION_PROPERTY constexpr Vector3 getVector3(int y) { return Vector3(m[y][0], m[y][1], m[y][2]); }

		//set Vector4, matrix[y] = value (set function property)
		//return: set value
		//argument 1: set Vector4
		SGF_FUNCTION_PROPERTY Vector4 setVector4(int y, const Vector4& value)
		{
			for (int i = 0; i < 4; i++)		m[y][i] = value[i];
			return value;
		}
		//set Vector3 matrix[y] = value & w (set function property)
		//return: set value
		//argument 1: y
		//argument 2: set Vector4
		//argument 3: set w, default = 0.0f
		SGF_FUNCTION_PROPERTY Vector3 setVector3(int y, const Vector3& value, float w = 0.0f)
		{
			for (int i = 0; i < 3; i++)		m[y][i] = value[i];
			m[y][3] = w;
			return value;
		}

		//float*オペレーター
		inline operator float*();
		//const float*オペレーター
		inline operator const float*() const;
		//XmMatrix オペレーター
		inline operator XmMatrix() const noexcept;

		//copy オペレーター
		inline const Vector4x4& operator=(const Vector4x4& copy);
		//move オペレーター
		inline const Vector4x4& operator=(Vector4x4&& move);
		// = オペレーター
		inline const Vector4x4& operator = (const DirectX::XMMATRIX& matrix);

		// () オペレーター
		inline float & operator ()(int y, int x);
		// + オペレーター
		inline Vector4x4 operator +() const;
		// - オペレーター
		inline Vector4x4 operator -() const;

		// + オペレーター
		inline Vector4x4 operator +(const Vector4x4& vec) const;
		// - オペレーター
		inline Vector4x4 operator -(const Vector4x4& vec) const;
		// * オペレーター
		inline Vector4x4 operator *(const Vector4x4& vec) const;
		// / オペレーター
		inline Vector4x4 operator /(const Vector4x4& vec) const;
		
		// += オペレーター
		inline const Vector4x4& operator +=(const Vector4x4& vec);
		// -= オペレーター
		inline const Vector4x4& operator -=(const Vector4x4& vec);
		// *= オペレーター
		inline const Vector4x4& operator *=(const Vector4x4& vec);
		// /= オペレーター
		inline const Vector4x4& operator /=(const Vector4x4& vec);

		// * オペレーター
		inline Vector4x4 operator *(float value) const;
		// *= オペレーター
		inline const Vector4x4& operator *=(float value);
		// / オペレーター
		inline Vector4x4 operator /(float value) const;
		// /= オペレーター
		inline const Vector4x4& operator /=(float value);
	};


	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//identityに設定される
	inline constexpr Vector4x4::Vector4x4() : DirectX::XMFLOAT4X4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f) { }
	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//代入初期化する
	//引数1:XMFLOAT4x4
	inline constexpr Vector4x4::Vector4x4(const DirectX::XMFLOAT4X4& value) : DirectX::XMFLOAT4X4(value) { }
	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//代入初期化する
	//引数1:PxMat33
	inline constexpr Vector4x4::Vector4x4(const physx::PxMat33 & value)
		: DirectX::XMFLOAT4X4(
			value(0, 0), value(1, 0), value(2, 0), 0.0f,
			value(0, 1), value(1, 1), value(2, 1), 0.0f,
			value(0, 2), value(1, 2), value(2, 2), 0.0f, 
			0.0f, 0.0f, 0.0f, 1.0f) { }
	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//代入初期化する
	//引数1:PxMat33
	inline constexpr Vector4x4::Vector4x4(const physx::PxMat44 & value)
		: DirectX::XMFLOAT4X4(
			value(0, 0), value(1, 0), value(2, 0), value(3, 0),
			value(0, 1), value(1, 1), value(2, 1), value(3, 1),
			value(0, 2), value(1, 2), value(2, 2), value(3, 2),
			value(0, 3), value(1, 3), value(2, 3), value(3, 3)) {}
	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//代入初期化する
	//引数1:aiMatrix3x3
	inline constexpr Vector4x4::Vector4x4(const aiMatrix3x3 & value)
		: DirectX::XMFLOAT4X4(
			value.a1, value.b1, value.c1, 0.0f,
			value.a2, value.b2, value.c2, 0.0f,
			value.a3, value.b3, value.c3, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f) {}
	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//代入初期化する
	//引数1:aiMatrix4x4
	inline constexpr Vector4x4::Vector4x4(const aiMatrix4x4 & value)
		: DirectX::XMFLOAT4X4(
			value.a1, value.b1, value.c1, value.d1,
			value.a2, value.b2, value.c2, value.d2,
			value.a3, value.b3, value.c3, value.d3,
			value.a4, value.b4, value.c4, value.d4) {}
	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//代入初期化する
	//引数1: Copy
	inline constexpr Vector4x4::Vector4x4(const Vector4x4 & copy) : DirectX::XMFLOAT4X4(copy) {}
	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//代入初期化する
	//引数1: Move
	inline constexpr Vector4x4::Vector4x4(Vector4x4 && move) : DirectX::XMFLOAT4X4(move) {}
	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//代入初期化する
	//引数1: Vector4_1
	//引数2: Vector4_2
	//引数3: Vector4_3
	//引数4: Vector4_4
	inline constexpr Vector4x4::Vector4x4(const Vector4& value1, const Vector4& value2,
		const Vector4& value3, const Vector4& value4)
	{
		for (int i = 0; i < 4; i++)
		{
			m[0][i] = value1[i];
			m[1][i] = value1[i];
			m[2][i] = value2[i];
			m[3][i] = value3[i];
		}
	}
	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//代入初期化する
	//引数1 ~ 16: value
	inline constexpr Vector4x4::Vector4x4(float _11, float _12, float _13, float _14,
		float _21, float _22, float _23, float _24,
		float _31, float _32, float _33, float _34,
		float _41, float _42, float _43, float _44)
		: DirectX::XMFLOAT4X4(_11, _12, _13, _14, _21, _22, _23, _24, _31, _32, _33, _34, _41, _42, _43, _44) { }
	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//代入初期化する
	//引数1: XMMATRIX
	inline Vector4x4::Vector4x4(const DirectX::XMMATRIX& value)
	{
		DirectX::XMStoreFloat4x4(this, value);
	}
	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//代入初期化する
	//引数1: float*
	inline Vector4x4::Vector4x4(const float * array) 
	{
		memcpy_s(this, sizeof(Vector4x4), array, sizeof(DirectX::XMFLOAT4X4));
	}
	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//代入初期化する
	//引数1: double*
	inline Vector4x4::Vector4x4(const double * array)
	{
		for (int i = 0; i < 4; i++)
		{
			m[i][0] = static_cast<float>(array[i * 4 + 0]);
			m[i][1] = static_cast<float>(array[i * 4 + 1]);
			m[i][2] = static_cast<float>(array[i * 4 + 2]);
			m[i][3] = static_cast<float>(array[i * 4 + 3]);
		}
	}

	//----------------------------------------------------------------------------------
	//[ToMatrix]
	//return: Matrix
	XmMatrix Vector4x4::ToMatrix() const noexcept
	{
		//Matrixに変換
		return std::move(DirectX::XMLoadFloat4x4(this));
	}
	//----------------------------------------------------------------------------------
	//[ToQuaternion]
	//return: Quaternion
	inline XmVector Vector4x4::ToQuaternion() const noexcept
	{
		return std::move(DirectX::XMQuaternionRotationMatrix(DirectX::XMLoadFloat4x4(this)));
	}
	//----------------------------------------------------------------------------------
	//[ToPhysXMat33]
	//return: physx::PxMat33
	inline physx::PxMat33 Vector4x4::ToPhysXMat33() const noexcept
	{
		return std::move(physx::PxMat33(
			physx::PxVec3(m[0][0], m[0][1], m[0][2]),
			physx::PxVec3(m[1][0], m[1][1], m[1][2]),
			physx::PxVec3(m[2][0], m[2][1], m[2][2])));
	}
	//----------------------------------------------------------------------------------
	//[ToPhysXMat44]
	//return: physx::PxMat44
	inline physx::PxMat44 Vector4x4::ToPhysXMat44() const noexcept
	{
		return std::move(physx::PxMat44(
			physx::PxVec4(m[0][0], m[0][1], m[0][2], m[0][3]),
			physx::PxVec4(m[1][0], m[1][1], m[1][2], m[1][3]),
			physx::PxVec4(m[2][0], m[2][1], m[2][2], m[2][3]),
			physx::PxVec4(m[3][0], m[3][1], m[3][2], m[3][3])));
	}
	//----------------------------------------------------------------------------------
	//[ToAssimpMat33]
	//return: aiMatrix3x3
	inline aiMatrix3x3 Vector4x4::ToAssimpMat33() const noexcept
	{
		return std::move(aiMatrix3x3(
			m[0][0], m[1][0], m[2][0],
			m[0][1], m[1][1], m[2][1],
			m[0][2], m[1][2], m[2][2]));
	}
	//----------------------------------------------------------------------------------
	//[ToAssimpMat44]
	//return: aiMatrix4x4
	inline aiMatrix4x4 Vector4x4::ToAssimpMat44() const noexcept
	{
		return std::move(aiMatrix4x4(
			m[0][0], m[1][0], m[2][0], m[3][0],
			m[0][1], m[1][1], m[2][1], m[3][1],
			m[0][2], m[1][2], m[2][2], m[3][2],
			m[0][3], m[1][3], m[2][3], m[3][3]));
	}

	//----------------------------------------------------------------------------------
	//[ToMatrix]
	//引数1: apply
	inline void Vector4x4::ToMatrix(XmMatrix & apply) const noexcept
	{
		//Matrixに変換
		apply = DirectX::XMLoadFloat4x4(this);
	}
	//----------------------------------------------------------------------------------
	//[ToQuaternion]
	//引数1: apply
	inline void Vector4x4::ToQuaternion(XmVector & apply) const noexcept
	{
		apply = DirectX::XMQuaternionRotationMatrix(DirectX::XMLoadFloat4x4(this));
	}
	//----------------------------------------------------------------------------------
	//[ToPhysXMat33]
	//引数1: apply
	inline void Vector4x4::ToPhysXMat33(physx::PxMat33 & apply) const noexcept
	{
		apply.column0 = physx::PxVec3(m[0][0], m[0][1], m[0][2]);
		apply.column1 = physx::PxVec3(m[1][0], m[1][1], m[1][2]);
		apply.column2 = physx::PxVec3(m[2][0], m[2][1], m[2][2]);
	}
	//----------------------------------------------------------------------------------
	//[ToPhysXMat44]
	//引数1: apply
	inline void Vector4x4::ToPhysXMat44(physx::PxMat44 & apply) const noexcept
	{
		apply.column0 = physx::PxVec4(m[0][0], m[0][1], m[0][2], m[0][3]);
		apply.column1 = physx::PxVec4(m[1][0], m[1][1], m[1][2], m[1][3]);
		apply.column2 = physx::PxVec4(m[2][0], m[2][1], m[2][2], m[2][3]);
		apply.column3 = physx::PxVec4(m[3][0], m[3][1], m[3][2], m[3][3]);
	}
	//----------------------------------------------------------------------------------
	//[ToAssimpMat33]
	//引数1: apply
	inline void Vector4x4::ToAssimpMat33(aiMatrix3x3 & apply) const noexcept
	{
		apply.a1 = m[0][0]; apply.a2 = m[1][0]; apply.a3 = m[2][0]; 
		apply.b1 = m[0][1]; apply.b2 = m[1][1]; apply.b3 = m[2][1]; 
		apply.c1 = m[0][2]; apply.c2 = m[1][2]; apply.c3 = m[2][2];
	}
	//----------------------------------------------------------------------------------
	//[ToAssimpMat44]
	//引数1: apply
	inline void Vector4x4::ToAssimpMat44(aiMatrix4x4 & apply) const noexcept
	{
		apply.a1 = m[0][0]; apply.a2 = m[1][0]; apply.a3 = m[2][0]; apply.a4 = m[3][0];
		apply.b1 = m[0][1]; apply.b2 = m[1][1]; apply.b3 = m[2][1]; apply.b4 = m[3][1];
		apply.c1 = m[0][2]; apply.c2 = m[1][2]; apply.c3 = m[2][2]; apply.c4 = m[3][2];
		apply.d1 = m[0][3]; apply.d2 = m[1][3]; apply.d3 = m[2][3]; apply.d4 = m[3][3];
	}

	//----------------------------------------------------------------------------------
	//[Decompose]
	//行列情報を抽出し、nullptrでなければ各引数に代入
	//引数1: position (Vector3)
	//引数1: scale (Vector3)
	//引数1: rotation (Quaternion)
	inline void Vector4x4::Decompose(DirectX::XMFLOAT3 * position, DirectX::XMFLOAT3 * scale, DirectX::XMFLOAT4 * rotation) const
	{
		Vector4x4 copy(*this);
		Vector3 extractionPosition, extractionScale;

		extractionPosition = copy.getVector4(3);
		copy.setVector3(3, Vector3(0.0f, 0.0f, 0.0f), 1.0f);

		extractionScale.x = copy.getVector3(0).Magnitude();
		extractionScale.y = copy.getVector3(1).Magnitude();
		extractionScale.z = copy.getVector3(2).Magnitude();
		copy(0, 0) -= extractionScale.x;
		copy(1, 1) -= extractionScale.y;
		copy(2, 2) -= extractionScale.z;

		if (position != nullptr) *position = extractionPosition;
		if (scale != nullptr) *scale = extractionScale;
		if (rotation != nullptr) DirectX::XMStoreFloat4(rotation, DirectX::XMQuaternionRotationMatrix(copy));
	}

	//float*オペレーター
	inline Vector4x4::operator float*() { return (float*)this; }
	//const float*オペレーター
	inline Vector4x4::operator const float*() const { return (float*)this; }
	//XmMatrix オペレーター
	inline Vector4x4::operator XmMatrix() const noexcept
	{
		return DirectX::XMLoadFloat4x4(this);
	}
	
	//copy オペレーター
	inline const Vector4x4 & Vector4x4::operator=(const Vector4x4 & copy)
	{
		memcpy_s(this, sizeof(Vector4x4), &copy, sizeof(Vector4x4));
		return *this;
	}
	//move オペレーター
	inline const Vector4x4 & Vector4x4::operator=(Vector4x4 && move)
	{
		memcpy_s(this, sizeof(Vector4x4), &move, sizeof(Vector4x4));
		return *this;
	}
	// = オペレーター
	inline const Vector4x4 & Vector4x4::operator=(const DirectX::XMMATRIX& matrix)
	{
		DirectX::XMStoreFloat4x4(this, matrix);
		return *this;
	}

	// () オペレーター
	inline float & Vector4x4::operator()(int y, int x) { return m[y][x];}
	// + オペレーター
	inline Vector4x4 Vector4x4::operator+() const { return *this; }
	// - オペレーター
	inline Vector4x4 Vector4x4::operator-() const
	{
		XmMatrix matrix = DirectX::XMLoadFloat4x4(this);
		return -matrix;
	}


	// + オペレーター
	inline Vector4x4 Vector4x4::operator+(const Vector4x4& vec) const
	{
		XmMatrix left = DirectX::XMLoadFloat4x4(this),
			right = DirectX::XMLoadFloat4x4(&vec);
		return left + right;
	}
	// - オペレーター
	inline Vector4x4 Vector4x4::operator-(const Vector4x4& vec) const
	{
		XmMatrix left = DirectX::XMLoadFloat4x4(this),
			right = DirectX::XMLoadFloat4x4(&vec);
		return left - right;
	}
	// * オペレーター
	inline Vector4x4 Vector4x4::operator*(const Vector4x4& vec) const
	{
		XmMatrix left = DirectX::XMLoadFloat4x4(this),
			right = DirectX::XMLoadFloat4x4(&vec);
		return left * right;
	}
	// / オペレーター
	inline Vector4x4 Vector4x4::operator/(const Vector4x4& vec) const
	{
		Vector4x4 ret;
		for (int i = 0; i < 4; i++)
			for (int k = 0; k < 4; k++)
				ret.m[i][k] = m[i][k] / vec.m[i][k];
		return ret;
	}


	// += オペレーター
	inline const Vector4x4& Vector4x4::operator+=(const Vector4x4& vec)
	{
		XmMatrix left = DirectX::XMLoadFloat4x4(this),
			right = DirectX::XMLoadFloat4x4(&vec);
		*this = left + right;
		return *this;
	}
	// -= オペレーター
	inline const Vector4x4& Vector4x4::operator-=(const Vector4x4& vec)
	{
		XmMatrix left = DirectX::XMLoadFloat4x4(this),
			right = DirectX::XMLoadFloat4x4(&vec);
		*this = left - right;
		return *this;
	}
	// *= オペレーター
	inline const Vector4x4& Vector4x4::operator*=(const Vector4x4& vec)
	{
		XmMatrix left = DirectX::XMLoadFloat4x4(this),
			right = DirectX::XMLoadFloat4x4(&vec);
		*this = left * right;
		return *this;
	}
	// /= オペレーター
	inline const Vector4x4& Vector4x4::operator/=(const Vector4x4& vec)
	{
		for (int i = 0; i < 4; i++)
			for (int k = 0; k < 4; k++)
				m[i][k] /= vec.m[i][k];
		return *this;
	}


	// * オペレーター
	inline Vector4x4 Vector4x4::operator*(float value) const
	{
		XmMatrix left = DirectX::XMLoadFloat4x4(this);
		return left * value;
	}
	// *= オペレーター
	inline const Vector4x4& Vector4x4::operator*=(float value)
	{
		XmMatrix left = DirectX::XMLoadFloat4x4(this);
		*this = left * value;
		return *this;
	}
	// / オペレーター
	inline Vector4x4 Vector4x4::operator/(float value) const
	{
		XmMatrix left = DirectX::XMLoadFloat4x4(this);
		return left / value;
	}
	// /= オペレーター
	inline const Vector4x4& Vector4x4::operator/=(float value)
	{
		XmMatrix left = DirectX::XMLoadFloat4x4(this);
		*this = left / value;
		return *this;
	}
}
#endif // !SGFRAMEWORK_HEADER_VECTOR_VECTOR4X4_HPP_
