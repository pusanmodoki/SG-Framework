/*----------------------------------------------------------------------------------
回転を扱えるQuaternion 構造体
最も多用する構造体シリーズの1つなので別自作Vectorが関係ないものは全てinline実装
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_QUATERNION_HPP_
#define SGFRAMEWORK_HEADER_QUATERNION_HPP_
#include <DirectXMath.h>
#include <iostream>
#include <assimp/quaternion.h>
#include "../Const/ConstVector.hpp"
#include "../ConstAndUsing/ConstAndUsing.hpp"
#include "../Property/Property.hpp"
#include "../Vector/Vector.hpp"
#include "../Matrix/Matrix.hpp"
#include "../String/String.hpp"

//Framework namespace
namespace SGFramework
{
	//回転を扱えるQuaternion 構造体, 回転順序はZXY
	struct Quaternion final : public DirectX::XMFLOAT4
	{
	public:
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//identityに設定される
		inline constexpr Quaternion();
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//代入する
		//引数1: x
		//引数2: y
		//引数3: z
		//引数4: w
		inline constexpr Quaternion(float x, float y, float z, float w);
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//代入する
		//引数1 : XmVector(XMVECTOR)
		inline Quaternion(const XmVector& xmVector);
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//代入する
		//引数1: Matrix(XMMATRIX)
		inline Quaternion(const Matrix& matrix);
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//代入初期化する
		//引数1: PxVec4
		inline constexpr Quaternion(const physx::PxQuat& quaternion);
		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//代入初期化する
		//引数1: aiQuaternion
		inline constexpr Quaternion(const aiQuaternion& quaternion);
		
		DEFAULT_COPY_MOVE_FUNCTIONS(Quaternion);

		//----------------------------------------------------------------------------------
		//[ToXmVector]
		//return: XMVECTOR
		inline XmVector ToXmVector() const noexcept { return std::move(DirectX::XMLoadFloat4(this)); }
		//----------------------------------------------------------------------------------
		//[ToMatrix]
		//return: Rotation Matrix
		inline Matrix ToMatrix() const noexcept { return std::move(DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(this))); }
		//----------------------------------------------------------------------------------
		//[ToMatrix]
		//引数1: Apply
		inline void ToMatrix(Matrix& apply) const noexcept { apply = std::move(DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(this))); }
		//----------------------------------------------------------------------------------
		//[ToPhysX]
		//return: physx::PxVec4
		inline physx::PxQuat ToPhysX() const noexcept { return std::move(physx::PxQuat(x, y, z, w)); }
		//----------------------------------------------------------------------------------
		//[ToPhysX]
		//引数1: Apply
		inline void ToPhysX(physx::PxQuat& apply) const noexcept { apply = std::move(physx::PxQuat(x, y, z, w)); }
		//----------------------------------------------------------------------------------
		//[ToAssimp]
		//return: aiQuaternion
		inline aiQuaternion ToAssimp() const noexcept { return std::move(aiQuaternion(x, y, z, w)); }
		//----------------------------------------------------------------------------------
		//[ToAssimp]
		//引数1: Apply
		inline void ToAssimp(aiQuaternion apply) const noexcept { apply = std::move(aiQuaternion(x, y, z, w)); }
		//----------------------------------------------------------------------------------
		//[ToAngleAxis]
		//回転を座標に対する角度の値 (AngleAxis) に変換する
		//引数1: axis (書き込み)
		//引数2: angle (書き込み)
		inline void ToAngleAxis(Vector3& axis, float& angle) const  noexcept
		{
			DirectX::XMVECTOR vec;
			//Get
			DirectX::XMQuaternionToAxisAngle(&vec, &angle, DirectX::XMLoadFloat4(this));
			//Set
			axis = vec;
		}
		//----------------------------------------------------------------------------------
		//[ToDirectionVectors]
		//方向ベクトル (up, right, forward) に変換する
		//引数1: right (書き込み)
		//引数2: up (書き込み)
		//引数3: forward (書き込み)
		inline void ToDirectionVectors(Vector3* right, Vector3* up, Vector3* forward) const noexcept
		{
			XmVector quaternion = DirectX::XMLoadFloat4(this);
			//Set
			if (right) *right = DirectX::XMVector3Rotate(DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), quaternion);
			//Set
			if (up) *up = DirectX::XMVector3Rotate(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), quaternion);
			//Set
			if (forward) *forward = DirectX::XMVector3Rotate(DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), quaternion);
		}	
		//----------------------------------------------------------------------------------
		//[ToString]
		//文字列に変換する
		//return: 変換した文字列
		inline sgstring ToString() const noexcept
		{
			//Get
			Vector3 vec = this->getEulerAngles();
			//return
			return vec.ToString();
		}
		//----------------------------------------------------------------------------------
		//[ToStdString]
		//文字列に変換する
		//return: 変換した文字列
		inline std::string ToStdString() const noexcept
		{
			//Get
			Vector3 vec = this->getEulerAngles();
			//return
			return vec.ToStdString();
		}

		//----------------------------------------------------------------------------------
		//[getEulerAngles]
		//オイラー角 (x, y, z) に変換する
		//return: オイラー角 (x, y, z)
		SGF_FUNCTION_PROPERTY Vector3 getEulerAngles() const noexcept
		{
			//Get
			Vector4x4 matrix = DirectX::XMMatrixRotationQuaternion(*this);
			
			//m21 == 1
			if (matrix(2, 1) > 0.999999f)
				return Vector3(-MathAF::cPiDiv2, 0.0f, std::atan2f(matrix(1, 0), matrix(0, 0))) * MathAF::cRadToDeg;
			//m21 == -1
			else if (matrix(2, 1) < -0.999999f)
				return Vector3(MathAF::cPiDiv2, 0.0f, std::atan2f(matrix(1, 0), matrix(0, 0))) * MathAF::cRadToDeg;
			else
				return Vector3(-std::asinf(matrix(2, 1)), std::atan2f(-matrix(2, 0), matrix(2, 2)),
					std::atan2f(-matrix(0, 1), matrix(1, 1))) * MathAF::cRadToDeg;
		}
		//inverse quaternion (get function property)
		SGF_FUNCTION_PROPERTY Quaternion getInverse() const noexcept { return std::move(DirectX::XMQuaternionInverse(DirectX::XMLoadFloat4(this))); }
		//quaternion->right vector (get function property)
		SGF_FUNCTION_PROPERTY Vector3 getRight() const noexcept { return std::move(DirectX::XMVector3Rotate(DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), DirectX::XMLoadFloat4(this))); }
		//quaternion->up vector (get function property)
		SGF_FUNCTION_PROPERTY Vector3 getUp() const noexcept { return std::move(DirectX::XMVector3Rotate(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), DirectX::XMLoadFloat4(this))); }
		//quaternion->forward vector (get function property)
		SGF_FUNCTION_PROPERTY Vector3 getForward() const noexcept { return std::move(DirectX::XMVector3Rotate(DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), DirectX::XMLoadFloat4(this))); }

		//----------------------------------------------------------------------------------
		//operators

		//sgstringオペレーター
		inline operator sgstring() const { return ToString(); }
		//XmVector (XMVECTOR) オペレーター
		inline operator XmVector() const { return DirectX::XMLoadFloat4(this); }
		//Matrixオペレーター
		inline operator Matrix() const { return DirectX::XMMatrixRotationQuaternion(this->operator XmVector()); }
		//float*オペレーター
		inline operator float*() { return (float*)this; }
		//const float*オペレーター
		inline operator const float*() const { return (float*)this; }

		//=オペレーター
		inline const Quaternion& operator = (const XmVector& mathQuaternion) { DirectX::XMStoreFloat4(this, mathQuaternion);  return *this; }
		//=オペレーター
		inline const Quaternion& operator = (const Matrix& matrix) { *this = DirectX::XMQuaternionRotationMatrix(matrix); return *this; }
		//=オペレーター
		inline const Quaternion& operator = (const DirectX::XMMATRIX& matrix) { *this = DirectX::XMQuaternionRotationMatrix(matrix); return *this; }
		// * オペレーター
		inline Quaternion operator * (const Quaternion& quaternion) const { return DirectX::XMQuaternionMultiply(*this, quaternion); }
		// * オペレーター
		inline Vector3 operator * (const Vector3& vec) const { return DirectX::XMVector3Rotate(vec, *this); }
		//*= オペレーター
		inline const Quaternion& operator *= (const Quaternion& quaternion) { *this = DirectX::XMQuaternionMultiply(*this, quaternion); return *this; }
		//== オペレーター
		inline bool operator == (const Quaternion& quaternion) const
		{
			return (fabsf(x - quaternion.x) < MathAF::cEpsilon
				&& fabsf(y - quaternion.y) < MathAF::cEpsilon
				&& fabsf(z - quaternion.z) < MathAF::cEpsilon
				&& fabsf(w - quaternion.w) < MathAF::cEpsilon);
		}
		//!= オペレーター
		inline bool operator != (const Quaternion& quaternion) const
		{
			return (fabsf(x - quaternion.x) > MathAF::cEpsilon
				&& fabsf(y - quaternion.y) > MathAF::cEpsilon
				&& fabsf(z - quaternion.z) > MathAF::cEpsilon
				&& fabsf(w - quaternion.w) > MathAF::cEpsilon);
		}

		//計算ライブラリ


		//----------------------------------------------------------------------------------
		//[Inverse]<Calculation Library Function>
		//逆クォータニオンに変換する
		//return: 逆クォータニオン
		//引数1: 変換するquaternion
		inline static Quaternion Inverse(const Quaternion & quaternion) noexcept;
		//----------------------------------------------------------------------------------
		//[LookForward]<Calculation Library Function>
		//return: forwardを向くQuaternion
		//引数1: forward
		//引数2: 上方向: default = Vector3::up
		inline static Quaternion LookForward(const Vector3 & forward, const Vector3 & upwards = Const::Vector3::up) noexcept;
		//----------------------------------------------------------------------------------
		//[LookAt]<Calculation Library Function>
		//positionからtargetを見る形の回転を計算する
		//return: positionからtargetを見る形の回転
		//引数1: position
		//引数2: target
		//引数3: 上方向: default = Vector3::up
		inline static Quaternion LookAt(const Vector3 & position, const Vector3 & target, const Vector3 & upwards = Const::Vector3::up) noexcept;
		//----------------------------------------------------------------------------------
		//[LookAt2D]<Calculation Library Function>
		//positionからtargetを見る形の回転を計算する(2D版)
		//return: positionからtargetを見る形の回転(軸指定)
		//引数1: position
		//引数2: target
		//引数3: 回転軸 (X or Y): default = true (X)
		inline static Quaternion LookAt2D(const Vector3 & position, const Vector3 & target, bool isAxisX = true) noexcept;
		//----------------------------------------------------------------------------------
		//[FromToRotation]<Calculation Library Function>
		//fromからtoへ回転するQuaternionを計算する
		//return: fromからtoへの回転
		//引数1: from (正規化済)
		//引数2: to (正規化済)
		inline static Quaternion FromToRotation(const Vector3& fromDirection, const Vector3& toDirection) noexcept;
		//----------------------------------------------------------------------------------
		//[AngleAxis]<Calculation Library Function>
		//axisの周りをangle度回転するQuaternionを計算する
		//return: axisの周りをangle度回転するQuaternion
		//引数1: 回転軸
		//引数2: 角度 (degree)
		inline static Quaternion AngleAxis(const Vector3 & axis, float angle) noexcept;
		//----------------------------------------------------------------------------------
		//[Euler]<Calculation Library Function>
		//オイラー角 (x, y, z) を基にQuaternionを計算する, 回転順はYXZ
		//return: オイラー角 (x, y, z) を基にしたQuaternion
		//引数1: 回転 (degrees)
		inline static Quaternion Euler(const Vector3 & eulerAngles) noexcept;
		//----------------------------------------------------------------------------------
		//[Lerp3D]<Calculation Library Function>
		//fromとtoの間をtで線形補完したものを返却する, 補完には3次関数補完を使う
		//return: fromからtoへの線形(3次関数)補間
		//引数1: from
		//引数2: to
		//引数3: 補完の度合い(0 ~ 1)
		inline static Quaternion Lerp3D(const Quaternion & from, const Quaternion& to, float t) noexcept;
		//----------------------------------------------------------------------------------
		//[LerpDirect]<Calculation Library Function>
		//fromとtoの間をtで線形補完したものを返却する, 補完にはただの線形補間を使う
		//return: fromからtoへの線形補間
		//引数1: from
		//引数2: to
		//引数3: 補完の度合い(0 ~ 1)
		inline static Quaternion LerpDirect(const Quaternion& from, const Quaternion& to, float t) noexcept;
		//----------------------------------------------------------------------------------
		//[Slerp]<Calculation Library Function>
		//thisとtoの間をtで球面線形補間したものを計算する
		//return: fromからtoへの球面線形補間
		//引数1: from
		//引数2: to
		//引数3: 補完の度合い(0 ~ 1)
		inline static Quaternion Slerp(const Quaternion& from, const Quaternion& to, float t) noexcept;
		//----------------------------------------------------------------------------------
		//[Dot]<Calculation Library Function>
		//quaternion1とquaternion2の内積を計算する
		//return: quaternion1とquaternion2の内積
		//引数1: quaternion1
		//引数2: quaternion2
		inline float Dot(const Quaternion& quaternion1, const Quaternion& quaternion2) noexcept;
		//----------------------------------------------------------------------------------
		//[Vector3Rotate]<Calculation Library Function>
		//ベクトル方向をquaternionを使って回転させる
		//return: 回転したベクトル
		//引数1: 対象ベクトル
		//引数2: 回転に使うquaternion
		inline static Vector3 Vector3Rotate(const Vector3 & vec, const Quaternion & quaternion) noexcept;
	};

	//----------------------------------------------------------------------------------
	//[wostream operator]
	//引数1: stream
	//引数2: this
	inline std::wostream& operator<<(std::wostream& stream, const Quaternion& value)
	{
		const wchar* string = value.ToString();
		stream << string;
		return stream;
	}

	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//identityに設定される
	inline constexpr Quaternion::Quaternion() : XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) {}
	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//代入する
	//引数1: x
	//引数2: y
	//引数3: z
	//引数4: w
	inline constexpr Quaternion::Quaternion(float x, float y, float z, float w) : XMFLOAT4(x, y, z, w) {}
	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//代入する
	//引数1 : XmVector(XMVECTOR)
	inline Quaternion::Quaternion(const XmVector & mathQuaternion)
	{
		//Set
		DirectX::XMStoreFloat4(this, mathQuaternion);
	}
	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//代入する
	//引数1: Matrix(XMMATRIX)
	inline Quaternion::Quaternion(const Matrix & matrix)
	{
		//Set
		DirectX::XMStoreFloat4(this, DirectX::XMQuaternionRotationMatrix(matrix));
	}
	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//代入初期化する
	//引数1: PxVec4
	inline constexpr Quaternion::Quaternion(const physx::PxQuat & quaternion)
		: Quaternion(quaternion.x, quaternion.y, quaternion.z, quaternion.w) {}
	//----------------------------------------------------------------------------------
	//[コンストラクタ]
	//代入初期化する
	//引数1: aiQuaternion
	inline constexpr Quaternion::Quaternion(const aiQuaternion & quaternion)
		: Quaternion(quaternion.x, quaternion.y, quaternion.z, quaternion.w) {}



	//----------------------------------------------------------------------------------
	//[Inverse]<Calculation Library Function>
	//逆クォータニオンに変換する
	//return: 逆クォータニオン
	//引数1: 変換するquaternion
	inline Quaternion Quaternion::Inverse(const Quaternion & quaternion) noexcept
	{
		return std::move(DirectX::XMQuaternionInverse(DirectX::XMLoadFloat4(&quaternion)));
	}
	//----------------------------------------------------------------------------------
	//[LookForward]<Calculation Library Function>
	//return: forwardを向くQuaternion
	//引数1: forward
	//引数2: 上方向: default = Vector3::up
	inline Quaternion Quaternion::LookForward(const Vector3 & forward, const Vector3 & upwards) noexcept
	{
		return std::move(Matrix::LookForward(forward, upwards));
	}
	//----------------------------------------------------------------------------------
	//[LookAt]<Calculation Library Function>
	//positionからtargetを見る形の回転を計算する
	//return: positionからtargetを見る形の回転
	//引数1: position
	//引数2: target
	//引数3: 上方向: default = Vector3::up
	inline Quaternion Quaternion::LookAt(const Vector3 & position, const Vector3 & target, const Vector3 & upwards) noexcept
	{
		return std::move(Matrix::LookAt(position, target, upwards));
	}
	//----------------------------------------------------------------------------------
	//[LookAt2D]<Calculation Library Function>
	//positionからtargetを見る形の回転を計算する(2D版)
	//return: positionからtargetを見る形の回転(軸指定)
	//引数1: position
	//引数2: target
	//引数3: 回転軸 (X or Y): default = true (X)
	inline Quaternion Quaternion::LookAt2D(const Vector3 & position, const Vector3 & target, bool isAxisX) noexcept
	{
		//方向ベクトル
		Vector3 direction = target - position;
		//角度減算用
		float forwardDiff;
		if (isAxisX)	forwardDiff = 0.0f;
		else forwardDiff = 90.0f;
		//return
		return std::move(DirectX::XMQuaternionRotationAxis(
			DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), (std::atan2(direction.y, direction.x) * MathAF::cRadToDeg) - forwardDiff));
	}
	//----------------------------------------------------------------------------------
	//[FromToRotation]<Calculation Library Function>
	//fromからtoへ回転するQuaternionを計算する
	//return: fromからtoへの回転
	//引数1: from (正規化済)
	//引数2: to (正規化済)
	inline Quaternion Quaternion::FromToRotation(const Vector3 & fromDirection, const Vector3 & toDirection) noexcept
	{
		//temp
		DirectX::XMVECTOR temp1, temp2;
		temp1 = DirectX::XMLoadFloat3(&fromDirection);
		temp2 = DirectX::XMLoadFloat3(&toDirection);
		//acos
		float dotToAcos = std::acos(DirectX::XMVectorGetX(DirectX::XMVector3Dot(temp1, temp2)));

		//Set
		if (dotToAcos > MathAF::cEpsilon)
			return std::move(DirectX::XMQuaternionRotationAxis(
				DirectX::XMVector3Normalize(DirectX::XMVector3Cross(temp1, temp2)), dotToAcos));
		//Identity
		else
			return std::move(Quaternion(0.0f, 0.0f, 0.0f, 1.0f));
	}
	//----------------------------------------------------------------------------------
	//[AngleAxis]<Calculation Library Function>
	//axisの周りをangle度回転するQuaternionを計算する
	//return: axisの周りをangle度回転するQuaternion
	//引数1: 回転軸
	//引数2: 角度 (degree)
	inline Quaternion Quaternion::AngleAxis(const Vector3 & axis, float angle) noexcept
	{
		return std::move(DirectX::XMQuaternionRotationAxis(axis, angle * MathAF::cDegToRad));
	}
	//----------------------------------------------------------------------------------
	//[Euler]<Calculation Library Function>
	//オイラー角 (x, y, z) を基にQuaternionを計算する, 回転順はYXZ
	//return: オイラー角 (x, y, z) を基にしたQuaternion
	//引数1: 回転 (degrees)
	inline Quaternion Quaternion::Euler(const Vector3 & eulerAngles) noexcept
	{
		return std::move(DirectX::XMQuaternionRotationRollPitchYaw(
			eulerAngles.x * MathAF::cDegToRad, eulerAngles.y * MathAF::cDegToRad, eulerAngles.z * MathAF::cDegToRad));
	}
	//----------------------------------------------------------------------------------
	//[Lerp3D]<Calculation Library Function>
	//fromとtoの間をtで線形補完したものを返却する, 補完には3次関数補完を使う
	//return: fromからtoへの線形(3次関数)補間
	//引数1: from
	//引数2: to
	//引数3: 補完の度合い(0 ~ 1)
	inline Quaternion Quaternion::Lerp3D(const Quaternion & from, const Quaternion & to, float t) noexcept
	{
		if (t > 1.0f) t = 1.0f;	else if (t < 0.0f) t = 0.0f;
		//3次関数補間
		float rate = t * t * (3.0f - 2.0f * t);

		return std::move(DirectX::XMQuaternionMultiply(from * (1.0f - rate), to * rate));
	}
	//----------------------------------------------------------------------------------
	//[LerpDirect]<Calculation Library Function>
	//fromとtoの間をtで線形補完したものを返却する, 補完にはただの線形補間を使う
	//return: fromからtoへの線形補間
	//引数1: from
	//引数2: to
	//引数3: 補完の度合い(0 ~ 1)
	inline Quaternion Quaternion::LerpDirect(const Quaternion & from, const Quaternion & to, float t) noexcept
	{
		if (t > 1.0f) t = 1.0f;	else if (t < 0.0f) t = 0.0f;
		return std::move(DirectX::XMQuaternionMultiply(from * (1.0f - t), to * t));
	}
	//----------------------------------------------------------------------------------
	//[Slerp]<Calculation Library Function>
	//thisとtoの間をtで球面線形補間したものを計算する
	//return: fromからtoへの球面線形補間
	//引数1: from
	//引数2: to
	//引数3: 補完の度合い(0 ~ 1)
	inline Quaternion Quaternion::Slerp(const Quaternion & from, const Quaternion & to, float t) noexcept
	{
		if (t > 1.0f) t = 1.0f;	else if (t < 0.0f) t = 0.0f;
		return std::move(DirectX::XMQuaternionSlerp(DirectX::XMLoadFloat4(&from), DirectX::XMLoadFloat4(&to), t));
	}
	//----------------------------------------------------------------------------------
	//[Dot]<Calculation Library Function>
	//quaternion1とquaternion2の内積を計算する
	//return: quaternion1とquaternion2の内積
	//引数1: quaternion1
	//引数2: quaternion2
	inline float Quaternion::Dot(const Quaternion & quaternion1, const Quaternion & quaternion2) noexcept
	{
		return quaternion1.x * quaternion2.x + quaternion1.y * quaternion2.y 
			+ quaternion1.z * quaternion2.z + quaternion1.w * quaternion2.w;
	}
	//----------------------------------------------------------------------------------
	//[Vector3Rotate]<Calculation Library Function>
	//ベクトル方向をquaternionを使って回転させる
	//return: 回転したベクトル
	//引数1: 対象ベクトル
	//引数2: 回転に使うquaternion
	inline Vector3 Quaternion::Vector3Rotate(const Vector3 & vec, const Quaternion & quaternion) noexcept
	{
		return std::move(DirectX::XMVector3Rotate(vec, quaternion));
	}
}
#endif // !SGFRAMEWORK_HEADER_QUATERNION_HPP_
