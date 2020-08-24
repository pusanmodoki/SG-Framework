/*----------------------------------------------------------------------------------
4x4の値を扱えるMatrix 構造体
最も多用する構造体シリーズの1つなので全てinline実装
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_MATRIX_HPP_
#define SGFRAMEWORK_HEADER_MATRIX_HPP_
#include <DirectXMath.h>
#include "../Const/ConstVector.hpp"
#include "../Property/Property.hpp"
#include "../Vector/Vector.hpp"
#include "assimp/matrix3x3.h"
#include "assimp/matrix4x4.h"

//Framework namespace
namespace SGFramework
{
	//4x4の値を扱えるMatrix 構造体
	//クラスのメンバ変数にすることは非推奨！
	struct Matrix final : public DirectX::XMMATRIX
	{
	public:
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//identityに設定される
		inline Matrix();
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//代入初期化する
		//引数1: Vector4x4
		inline Matrix(const Vector4x4& value);
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//代入初期化する
		//引数1: Vector4(_1○)
		//引数2: Vector4(_2○)
		//引数3: Vector4(_3○)
		//引数4: Vector4(_4○)
		inline Matrix(const Vector4& value1, const Vector4& value2,
			const Vector4& value3, const Vector4& value4);
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//代入初期化する
		//引数1: XMMATRIX
		inline Matrix(const DirectX::XMMATRIX value);
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//代入初期化する
		//引数1: XMFLOAT4X4
		inline Matrix(const DirectX::XMFLOAT4X4& value);
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//代入初期化する
		//引数1:PxMat33
		inline Matrix(const physx::PxMat33& value);
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//代入初期化する
		//引数1:PxMat44
		inline Matrix(const physx::PxMat44& value);
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//代入初期化する
		//引数1:aiMatrix3x3
		inline Matrix(const aiMatrix3x3& value);
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//代入初期化する
		//引数1:aiMatrix4x4
		inline Matrix(const aiMatrix4x4& value);
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//代入初期化する
		//引数1 ~ 16: value
		inline Matrix(float _11, float _12, float _13, float _14, 
			float _21, float _22, float _23, float _24, 
			float _31, float _32, float _33, float _34, 
			float _41, float _42, float _43, float _44);
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//代入初期化する
		//引数1: float*
		inline Matrix(float* array);
		
		DEFAULT_COPY_MOVE_FUNCTIONS(Matrix);

		//----------------------------------------------------------------------------------
		//[ToVector4x4]
		//return: Vector4x4
		inline Vector4x4 ToVector4x4() const noexcept;
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
		//[ToVector4x4]
		//引数1: apply
		inline void ToVector4x4(Vector4x4& apply) const noexcept;
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


		//inverse matrix (get function property)
		SGF_FUNCTION_PROPERTY Matrix getInverse() noexcept { return std::move(DirectX::XMMatrixInverse(nullptr, *this)); }
		//inverse matrix (get function property)
		SGF_FUNCTION_PROPERTY const Matrix& setInverse() noexcept { return *this = std::move(DirectX::XMMatrixInverse(nullptr, *this));  }

		//transpose matrix (get function property)
		SGF_FUNCTION_PROPERTY Matrix getTranspose() noexcept { return std::move(DirectX::XMMatrixTranspose(*this)); }
		//transpose matrix (set function property)
		SGF_FUNCTION_PROPERTY const Matrix& setTranspose() noexcept { return *this = std::move(DirectX::XMMatrixTranspose(*this)); }
		

		//this right direction (get function property)
		SGF_FUNCTION_PROPERTY Vector3 getRight() { return this->r[0]; }
		//this up direction (get function property)
		SGF_FUNCTION_PROPERTY Vector3 getUp() { return this->r[1]; }
		//this forward direction (get function property)
		SGF_FUNCTION_PROPERTY Vector3 getForward() { return this->r[2]; }
		//this translate (get function property)
		SGF_FUNCTION_PROPERTY Vector3 getTranslate() { return this->r[3]; }

		//this right direction(XmVector) (get function property)
		SGF_FUNCTION_PROPERTY XmVector getXmRight() { return this->r[0]; }
		//this up direction(XmVector) (get function property)
		SGF_FUNCTION_PROPERTY XmVector getXmUp() { return this->r[1]; }
		//this forward direction(XmVector) (get function property)
		SGF_FUNCTION_PROPERTY XmVector getXmForward() { return this->r[2]; }
		//this translate(XmVector) (get function property)
		SGF_FUNCTION_PROPERTY XmVector getXmTranslate() { return this->r[3]; }
		//this direction vectors(store) (get function property)
		//argument 1: vec reference<-up
		//argument 2: vec reference<-right
		//argument 3: vec reference<-forward
		SGF_FUNCTION_PROPERTY void getDirections(Vector3& up, Vector3& right, Vector3& forward)
		{
			DirectX::XMStoreFloat3(&right, this->r[0]);
			DirectX::XMStoreFloat3(&right, this->r[1]);
			DirectX::XMStoreFloat3(&right, this->r[2]);
		}
		//this direction vectors (get function property)
		//argument 1: xm vec reference<-up
		//argument 2: xm vec reference<-right
		//argument 3: xm vec reference<-forward
		SGF_FUNCTION_PROPERTY constexpr void getDirections(XmVector& up, XmVector& right, XmVector& forward)
		{
			right = this->r[0];
			up = this->r[1];
			forward = this->r[2];
		}

		//matrix[y]->Vector4 (get function property)
		//argument 1: y
		SGF_FUNCTION_PROPERTY Vector4 getVector4(int y) { return Vector4(this->r[y]); }
		//matrix[y]->Vector3 (get function property)
		//argument 1: y
		SGF_FUNCTION_PROPERTY Vector3 getVector3(int y) { return Vector3(this->r[y]); }

		//set Vector4x4, convert matrix (set function property)
		//return: set value
		//argument 1: set Vector4x4
		SGF_FUNCTION_PROPERTY const Vector4x4& setVector4x4(const Vector4x4& setValue) { *this = setValue; return setValue; }
		//set Vector4, matrix[y] = value (set function property)
		//return: set value
		//argument 1: set Vector4
		SGF_FUNCTION_PROPERTY const Vector4& setVector4(int y, const Vector4& setValue) { this->r[y] = DirectX::XMLoadFloat4(&setValue); return setValue; }
		//set Vector3 matrix[y] = value & w (set function property)
		//return: set value
		//argument 1: y
		//argument 2: set Vector4
		//argument 3: set w, default = 0.0f
		SGF_FUNCTION_PROPERTY const Vector3& setVector3(int y, const Vector3 &setValue, float w = 0.0f)
		{
			Vector4 vec(setValue.x, setValue.y, setValue.z, w);
			this->r[y] = DirectX::XMLoadFloat4(&vec);
			return setValue;
		}

		//= オペレーター
		inline const Matrix& operator =(const Vector4x4& value);
		//= オペレーター
		inline const Matrix& operator =(const DirectX::XMFLOAT4X4& value);

		//() operator
		//引数1: セットするVector4x4
		inline const Vector4x4& operator ()(const Vector4x4& setValue);
		//() operator
		//引数1: y
		inline Vector4 operator ()(int y);
		//() operator
		//引数1: y
		//引数2: セットするVector4
		inline const Vector4& operator ()(int y, const Vector4& setValue);
		//() operator
		//引数1: y
		//引数2: セットするVector3(Vector4(x,y,z,0))
		inline Vector3 operator ()(int y, const Vector3& setValue);


		//計算ライブラリ


		//----------------------------------------------------------------------------------
		//[Zero]<Calculation Library Function>
		//return: All 0 Matrix
		inline static Matrix Zero() noexcept;
		//----------------------------------------------------------------------------------
		//[One]<Calculation Library Function>
		//return: All 1 Matrix
		inline static Matrix One() noexcept;
		//----------------------------------------------------------------------------------
		//[Identity]<Calculation Library Function>
		//return: _11, _22, _33,_44 = 1 , other = 0 Matrix
		inline static Matrix Identity() noexcept;
		//----------------------------------------------------------------------------------
		//[Inverse]<Calculation Library Function>
		//return: Inverse Matrix
		//引数1: 基となるMatrix
		inline static Matrix Inverse(const Matrix& matrix) noexcept;
		//----------------------------------------------------------------------------------
		//[Transpose]<Calculation Library Function>
		//return: Transpose Matrix
		//引数1: 転置するmatrix
		inline static Matrix Transpose(const Matrix& transpose) noexcept;
		//----------------------------------------------------------------------------------
		//[Scaleing]<Calculation Library Function>
		//return: Scaleing Matrix
		//引数1: scale
		inline static Matrix Scaleing(const Vector3 & scale) noexcept;
		//----------------------------------------------------------------------------------
		//[Translate]<Calculation Library Function>
		//return: Translate Matrix
		//引数1: 移動量
		inline static Matrix Translate(const Vector3 & trans) noexcept;
		//----------------------------------------------------------------------------------
		//[LookForward]<Calculation Library Function>
		//return: 指定方向を向く回転Matrix
		//引数1: EyePosition
		//引数2: LookPosition
		//引数3: Upwards, default = up
		inline static Matrix LookForward(const Vector3 & forward, const Vector3 & upwards = Const::Vector3::up) noexcept;
		//----------------------------------------------------------------------------------
		//[LookAt]<Calculation Library Function>
		//return: 指定方向を向く回転Matrix
		//引数1: forward
		//引数2: Upwards, default = up
		inline static Matrix LookAt(const Vector3 & position, const Vector3 & target, const Vector3 & upwards = Const::Vector3::up) noexcept;
		//----------------------------------------------------------------------------------
		//[ViewMatrix]<Calculation Library Function>
		//return: カメラ用ViewMatrix
		//引数1: EyePosition
		//引数2: LookPosition
		//引数3: Upwards, default = up
		inline static Matrix ViewMatrix(const Vector3&  eyePosition,
			const Vector3&  lookAtPosition, const Vector3&  upwards = Const::Vector3::up) noexcept;
		//----------------------------------------------------------------------------------
		//[ViewMatrixForDirection]<Calculation Library Function>
		//return: カメラ用ViewMatrix
		//引数1: EyePosition
		//引数2: Direction
		//引数3: Upwards, default = up
		inline static Matrix ViewMatrixForDirection(const Vector3&  eyePosition,
			const Vector3&  direction, const Vector3&  upwards = Const::Vector3::up) noexcept;
		//----------------------------------------------------------------------------------
		//[ProjectionMatrix]<Calculation Library Function>
		//return: カメラ用ProjectionMatrix
		//引数1: Fov
		//引数2: Aspect
		//引数3: NearClip
		//引数4: FarClip
		inline static Matrix ProjectionMatrix(float fov, float aspect, float nearClip, float farClip) noexcept;
	};

	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//identityに設定される
	inline Matrix::Matrix() { *this = DirectX::XMMatrixIdentity(); }
	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//代入初期化する
	//引数1: Vector4x4
	inline Matrix::Matrix(const Vector4x4 & value) { *this = DirectX::XMLoadFloat4x4(&value); }
	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//代入初期化する
	//引数1: Vector4(_1○)
	//引数2: Vector4(_2○)
	//引数3: Vector4(_3○)
	//引数4: Vector4(_4○)
	inline Matrix::Matrix(const Vector4 & value1, const Vector4 & value2,
		const Vector4 & value3, const Vector4 & value4)
	{
		r[0] = DirectX::XMLoadFloat4(&value1);
		r[1] = DirectX::XMLoadFloat4(&value2);
		r[2] = DirectX::XMLoadFloat4(&value3);
		r[3] = DirectX::XMLoadFloat4(&value4);
	}
	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//代入初期化する
	//引数1: XMMATRIX
	inline Matrix::Matrix(DirectX::XMMATRIX value) : XMMATRIX(value) { }
	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//代入初期化する
	//引数1: XMFLOAT4X4
	inline Matrix::Matrix(const DirectX::XMFLOAT4X4 & value) { *this = DirectX::XMLoadFloat4x4(&value); }
	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//代入初期化する
	//引数1:PxMat33
	inline Matrix::Matrix(const physx::PxMat33 & value) { *this = Vector4x4(value); }
	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//代入初期化する
	//引数1:PxMat44
	inline Matrix::Matrix(const physx::PxMat44 & value) { *this = Vector4x4(value); }
	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//代入初期化する
	//引数1:aiMatrix3x3
	inline Matrix::Matrix(const aiMatrix3x3 & value) { *this = Vector4x4(value); }
	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//代入初期化する
	//引数1:aiMatrix4x4
	inline Matrix::Matrix(const aiMatrix4x4 & value) { *this = Vector4x4(value); }
	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//代入初期化する
	//引数1 ~ 16: value
	inline Matrix::Matrix(float _11, float _12, float _13, float _14,
		float _21, float _22, float _23, float _24, 
		float _31, float _32, float _33, float _34, 
		float _41, float _42, float _43, float _44)
	{
		*this = DirectX::XMLoadFloat4x4(&DirectX::XMFLOAT4X4(_11, _12, _13, _14,
			_21, _22, _23, _24, _31, _32, _33, _34, _41, _42, _43, _44));
	}
	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//代入初期化する
	//引数1: float*
	inline Matrix::Matrix(float * array) { *this = DirectX::XMLoadFloat4x4(&DirectX::XMFLOAT4X4(array)); }

	//----------------------------------------------------------------------------------
	//[ToVector4x4]
	//return: Vector4x4
	inline Vector4x4 Matrix::ToVector4x4() const noexcept { Vector4x4 result; DirectX::XMStoreFloat4x4(&result, *this); return std::move(result); }
	//----------------------------------------------------------------------------------
	//[ToQuaternion]
	//return: Quaternion
	inline XmVector Matrix::ToQuaternion() const noexcept { return std::move(DirectX::XMQuaternionRotationMatrix(*this)); }
	//----------------------------------------------------------------------------------
	//[ToPhysXMat33]
	//return: physx::PxMat33
	inline physx::PxMat33 Matrix::ToPhysXMat33() const noexcept { return std::move(ToVector4x4().ToPhysXMat33()); }
	//----------------------------------------------------------------------------------
	//[ToPhysXMat44]
	//return: physx::PxMat44
	inline physx::PxMat44 Matrix::ToPhysXMat44() const noexcept { return std::move(ToVector4x4().ToPhysXMat44()); }
	//----------------------------------------------------------------------------------
	//[ToAssimpMat33]
	//return: aiMatrix3x3
	inline aiMatrix3x3 Matrix::ToAssimpMat33() const noexcept { return std::move(ToVector4x4().ToAssimpMat33()); }
	//----------------------------------------------------------------------------------
	//[ToAssimpMat44]
	//return: aiMatrix4x4
	inline aiMatrix4x4 Matrix::ToAssimpMat44() const noexcept { return std::move(ToVector4x4().ToAssimpMat44()); }
	//----------------------------------------------------------------------------------
	//[ToVector4x4]
	//引数1: apply
	inline void Matrix::ToVector4x4(Vector4x4 & apply) const noexcept { apply = std::move(ToVector4x4()); }
	//----------------------------------------------------------------------------------
	//[ToQuaternion]
	//引数1: apply
	inline void Matrix::ToQuaternion(XmVector & apply) const noexcept { apply = std::move(DirectX::XMQuaternionRotationMatrix(*this)); }
	//----------------------------------------------------------------------------------
	//[ToPhysXMat33]
	//引数1: apply
	inline void Matrix::ToPhysXMat33(physx::PxMat33 & apply) const noexcept { apply = std::move(ToVector4x4().ToPhysXMat33()); }
	//----------------------------------------------------------------------------------
	//[ToPhysXMat44]
	//引数1: apply
	inline void Matrix::ToPhysXMat44(physx::PxMat44 & apply) const noexcept { apply = std::move(ToVector4x4().ToPhysXMat44()); }
	//----------------------------------------------------------------------------------
	//[ToAssimpMat33]
	//引数1: apply
	inline void Matrix::ToAssimpMat33(aiMatrix3x3 & apply) const noexcept { apply = std::move(ToVector4x4().ToAssimpMat33()); }
	//----------------------------------------------------------------------------------
	//[ToAssimpMat44]
	//引数1: apply
	inline void Matrix::ToAssimpMat44(aiMatrix4x4 & apply) const noexcept { apply = std::move(ToVector4x4().ToAssimpMat44()); }

	//----------------------------------------------------------------------------------
	//[Decompose]
	//行列情報を抽出し、nullptrでなければ各引数に代入
	//引数1: position (Vector3)
	//引数1: scale (Vector3)
	//引数1: rotation (Quaternion)
	inline void Matrix::Decompose(DirectX::XMFLOAT3 * position, DirectX::XMFLOAT3 * scale, DirectX::XMFLOAT4 * rotation) const
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

	//= オペレーター
	inline const Matrix& Matrix::operator=(const Vector4x4 & value)
	{
		*this = DirectX::XMLoadFloat4x4(&value);
		return *this;
	}
	//= オペレーター
	inline const Matrix& Matrix::operator=(const DirectX::XMFLOAT4X4 & value)
	{
		*this = DirectX::XMLoadFloat4x4(&value);
		return *this;
	}

	//() operator
	//引数1: セットするVector4x4
	inline const Vector4x4& Matrix::operator()(const Vector4x4& setValue)
	{
		*this = setValue;
		return setValue;
	}
	//() operator
	//引数1: y
	inline Vector4 Matrix::operator()(int y) { return Vector4(this->r[y]);}
	//() operator
	//引数1: y
	//引数2: セットするVector4
	inline const Vector4& Matrix::operator()(int y, const Vector4& setValue)
	{
		this->r[y] = setValue;
		return setValue;
	}	
	//() operator
	//引数1: y
	//引数2: セットするVector3(Vector4(x,y,z,0))
	inline Vector3 Matrix::operator()(int y, const Vector3& setValue)
	{
		r[y] = DirectX::XMLoadFloat4(&Vector4(setValue.x, setValue.y, setValue.z, 0.0f));
		return setValue;
	}

	//----------------------------------------------------------------------------------
	//[Zero]<Calculation Library Function>
	//return: All 0 Matrix
	inline Matrix Matrix::Zero() noexcept { return std::move(Matrix(Const::Vector4x4::zero)); }
	//----------------------------------------------------------------------------------
	//[One]<Calculation Library Function>
	//return: All 1 Matrix
	inline Matrix Matrix::One() noexcept { return std::move(Matrix(Const::Vector4x4::one)); }
	//----------------------------------------------------------------------------------
	//[Identity]<Calculation Library Function>
	//return: _11, _22, _33,_44 = 1 , other = 0 Matrix
	inline Matrix Matrix::Identity() noexcept { return std::move(DirectX::XMMatrixIdentity()); }
	//----------------------------------------------------------------------------------
	//[Inverse]<Calculation Library Function>
	//return: Inverse Matrix
	//引数1: 基となるMatrix
	inline Matrix Matrix::Inverse(const Matrix& matrix) noexcept { return std::move(DirectX::XMMatrixInverse(nullptr, matrix)); }
	//----------------------------------------------------------------------------------
	//[Transpose]<Calculation Library Function>
	//return: Transpose Matrix
	//引数1: 転置するmatrix
	inline Matrix Matrix::Transpose(const Matrix& transpose) noexcept { return std::move(DirectX::XMMatrixTranspose(transpose)); }
	//----------------------------------------------------------------------------------
	//[Scaleing]<Calculation Library Function>
	//return: Scaleing Matrix
	//引数1: scale
	inline Matrix Matrix::Scaleing(const Vector3 & scale) noexcept { return std::move(DirectX::XMMatrixScaling(scale.x, scale.y, scale.z)); }
	//----------------------------------------------------------------------------------
	//[Translate]<Calculation Library Function>
	//return: Translate Matrix
	//引数1: 移動量
	inline Matrix Matrix::Translate(const Vector3 & trans) noexcept { return std::move(DirectX::XMMatrixTranslation(trans.x, trans.y, trans.z)); }
	//----------------------------------------------------------------------------------
	//[LookForward]<Calculation Library Function>
	//return: 指定方向を向く回転Matrix
	//引数1: forward
	//引数2: Upwards, default = up
	inline Matrix Matrix::LookForward(const Vector3 & forward, const Vector3 & upwards) noexcept
	{
		Matrix result;

		//forward
		result.r[2] = forward.Normalized();
		//right
		result.r[0] = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(upwards.Normalized(), result.r[2]));
		//Zero?
		if (DirectX::XMVector3Equal(result.r[0], DirectX::XMVectorZero()))
			result.r[0] = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
		//up
		result.r[1] = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(result.r[2], result.r[0]));

		//Set
		return std::move(result);
	}
	//----------------------------------------------------------------------------------
	//[LookAt]<Calculation Library Function>
	//return: 指定方向を向く回転Matrix
	//引数1: EyePosition
	//引数2: LookPosition
	//引数3: UpDirection, default = up
	inline Matrix Matrix::LookAt(const Vector3 & position, const Vector3 & target, const Vector3 & upwards) noexcept
	{
		Matrix result;

		//forward
		result.r[2] = (target - position).Normalized();
		//right
		result.r[0] = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(upwards, result.r[2]));
		//Zero?
		if (DirectX::XMVector3Equal(result.r[0], DirectX::XMVectorZero()))
			result.r[0] = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
		//up
		result.r[1] = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(result.r[2], result.r[0]));

		//Set
		return std::move(result);
	}
	//----------------------------------------------------------------------------------
	//[ViewMatrix]<Calculation Library Function>
	//return: カメラ用ViewMatrix
	//引数1: EyePosition
	//引数2: LookPosition
	//引数3: UpDirection, default = up
	inline Matrix Matrix::ViewMatrix(const Vector3&  eyePosition,
		const Vector3&  lookAtPosition, const Vector3&  upwards) noexcept { return std::move(DirectX::XMMatrixLookAtLH(eyePosition, lookAtPosition, upwards)); }
	//----------------------------------------------------------------------------------
	//[ViewMatrixForDirection]<Calculation Library Function>
	//return: カメラ用ViewMatrix
	//引数1: EyePosition
	//引数2: Direction
	//引数3: Upwards, default = up
	inline Matrix Matrix::ViewMatrixForDirection(const Vector3&  eyePosition,
		const Vector3&  direction, const Vector3&  upwards) noexcept { return std::move(DirectX::XMMatrixLookToLH(eyePosition, direction, upwards)); }
	//----------------------------------------------------------------------------------
	//[ProjectionMatrix]<Calculation Library Function>
	//return: カメラ用ProjectionMatrix
	//引数1: Fov
	//引数2: Aspect
	//引数3: NearClip
	//引数4: FarClip
	inline Matrix Matrix::ProjectionMatrix(float fov, float aspect, float nearClip, float farClip) noexcept { return std::move(DirectX::XMMatrixPerspectiveFovLH(fov, aspect, nearClip, farClip)); }
}
#endif // !SGFRAMEWORK_HEADER_MATRIX_HPP_