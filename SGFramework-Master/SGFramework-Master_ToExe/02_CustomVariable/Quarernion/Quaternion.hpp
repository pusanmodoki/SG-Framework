/*----------------------------------------------------------------------------------
��]��������Quaternion �\����
�ł����p����\���̃V���[�Y��1�Ȃ̂ŕʎ���Vector���֌W�Ȃ����̂͑S��inline����
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
	//��]��������Quaternion �\����, ��]������ZXY
	struct Quaternion final : public DirectX::XMFLOAT4
	{
	public:
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//identity�ɐݒ肳���
		inline constexpr Quaternion();
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//�������
		//����1: x
		//����2: y
		//����3: z
		//����4: w
		inline constexpr Quaternion(float x, float y, float z, float w);
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//�������
		//����1 : XmVector(XMVECTOR)
		inline Quaternion(const XmVector& xmVector);
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//�������
		//����1: Matrix(XMMATRIX)
		inline Quaternion(const Matrix& matrix);
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//�������������
		//����1: PxVec4
		inline constexpr Quaternion(const physx::PxQuat& quaternion);
		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//�������������
		//����1: aiQuaternion
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
		//����1: Apply
		inline void ToMatrix(Matrix& apply) const noexcept { apply = std::move(DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(this))); }
		//----------------------------------------------------------------------------------
		//[ToPhysX]
		//return: physx::PxVec4
		inline physx::PxQuat ToPhysX() const noexcept { return std::move(physx::PxQuat(x, y, z, w)); }
		//----------------------------------------------------------------------------------
		//[ToPhysX]
		//����1: Apply
		inline void ToPhysX(physx::PxQuat& apply) const noexcept { apply = std::move(physx::PxQuat(x, y, z, w)); }
		//----------------------------------------------------------------------------------
		//[ToAssimp]
		//return: aiQuaternion
		inline aiQuaternion ToAssimp() const noexcept { return std::move(aiQuaternion(x, y, z, w)); }
		//----------------------------------------------------------------------------------
		//[ToAssimp]
		//����1: Apply
		inline void ToAssimp(aiQuaternion apply) const noexcept { apply = std::move(aiQuaternion(x, y, z, w)); }
		//----------------------------------------------------------------------------------
		//[ToAngleAxis]
		//��]�����W�ɑ΂���p�x�̒l (AngleAxis) �ɕϊ�����
		//����1: axis (��������)
		//����2: angle (��������)
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
		//�����x�N�g�� (up, right, forward) �ɕϊ�����
		//����1: right (��������)
		//����2: up (��������)
		//����3: forward (��������)
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
		//������ɕϊ�����
		//return: �ϊ�����������
		inline sgstring ToString() const noexcept
		{
			//Get
			Vector3 vec = this->getEulerAngles();
			//return
			return vec.ToString();
		}
		//----------------------------------------------------------------------------------
		//[ToStdString]
		//������ɕϊ�����
		//return: �ϊ�����������
		inline std::string ToStdString() const noexcept
		{
			//Get
			Vector3 vec = this->getEulerAngles();
			//return
			return vec.ToStdString();
		}

		//----------------------------------------------------------------------------------
		//[getEulerAngles]
		//�I�C���[�p (x, y, z) �ɕϊ�����
		//return: �I�C���[�p (x, y, z)
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

		//sgstring�I�y���[�^�[
		inline operator sgstring() const { return ToString(); }
		//XmVector (XMVECTOR) �I�y���[�^�[
		inline operator XmVector() const { return DirectX::XMLoadFloat4(this); }
		//Matrix�I�y���[�^�[
		inline operator Matrix() const { return DirectX::XMMatrixRotationQuaternion(this->operator XmVector()); }
		//float*�I�y���[�^�[
		inline operator float*() { return (float*)this; }
		//const float*�I�y���[�^�[
		inline operator const float*() const { return (float*)this; }

		//=�I�y���[�^�[
		inline const Quaternion& operator = (const XmVector& mathQuaternion) { DirectX::XMStoreFloat4(this, mathQuaternion);  return *this; }
		//=�I�y���[�^�[
		inline const Quaternion& operator = (const Matrix& matrix) { *this = DirectX::XMQuaternionRotationMatrix(matrix); return *this; }
		//=�I�y���[�^�[
		inline const Quaternion& operator = (const DirectX::XMMATRIX& matrix) { *this = DirectX::XMQuaternionRotationMatrix(matrix); return *this; }
		// * �I�y���[�^�[
		inline Quaternion operator * (const Quaternion& quaternion) const { return DirectX::XMQuaternionMultiply(*this, quaternion); }
		// * �I�y���[�^�[
		inline Vector3 operator * (const Vector3& vec) const { return DirectX::XMVector3Rotate(vec, *this); }
		//*= �I�y���[�^�[
		inline const Quaternion& operator *= (const Quaternion& quaternion) { *this = DirectX::XMQuaternionMultiply(*this, quaternion); return *this; }
		//== �I�y���[�^�[
		inline bool operator == (const Quaternion& quaternion) const
		{
			return (fabsf(x - quaternion.x) < MathAF::cEpsilon
				&& fabsf(y - quaternion.y) < MathAF::cEpsilon
				&& fabsf(z - quaternion.z) < MathAF::cEpsilon
				&& fabsf(w - quaternion.w) < MathAF::cEpsilon);
		}
		//!= �I�y���[�^�[
		inline bool operator != (const Quaternion& quaternion) const
		{
			return (fabsf(x - quaternion.x) > MathAF::cEpsilon
				&& fabsf(y - quaternion.y) > MathAF::cEpsilon
				&& fabsf(z - quaternion.z) > MathAF::cEpsilon
				&& fabsf(w - quaternion.w) > MathAF::cEpsilon);
		}

		//�v�Z���C�u����


		//----------------------------------------------------------------------------------
		//[Inverse]<Calculation Library Function>
		//�t�N�H�[�^�j�I���ɕϊ�����
		//return: �t�N�H�[�^�j�I��
		//����1: �ϊ�����quaternion
		inline static Quaternion Inverse(const Quaternion & quaternion) noexcept;
		//----------------------------------------------------------------------------------
		//[LookForward]<Calculation Library Function>
		//return: forward������Quaternion
		//����1: forward
		//����2: �����: default = Vector3::up
		inline static Quaternion LookForward(const Vector3 & forward, const Vector3 & upwards = Const::Vector3::up) noexcept;
		//----------------------------------------------------------------------------------
		//[LookAt]<Calculation Library Function>
		//position����target������`�̉�]���v�Z����
		//return: position����target������`�̉�]
		//����1: position
		//����2: target
		//����3: �����: default = Vector3::up
		inline static Quaternion LookAt(const Vector3 & position, const Vector3 & target, const Vector3 & upwards = Const::Vector3::up) noexcept;
		//----------------------------------------------------------------------------------
		//[LookAt2D]<Calculation Library Function>
		//position����target������`�̉�]���v�Z����(2D��)
		//return: position����target������`�̉�](���w��)
		//����1: position
		//����2: target
		//����3: ��]�� (X or Y): default = true (X)
		inline static Quaternion LookAt2D(const Vector3 & position, const Vector3 & target, bool isAxisX = true) noexcept;
		//----------------------------------------------------------------------------------
		//[FromToRotation]<Calculation Library Function>
		//from����to�։�]����Quaternion���v�Z����
		//return: from����to�ւ̉�]
		//����1: from (���K����)
		//����2: to (���K����)
		inline static Quaternion FromToRotation(const Vector3& fromDirection, const Vector3& toDirection) noexcept;
		//----------------------------------------------------------------------------------
		//[AngleAxis]<Calculation Library Function>
		//axis�̎����angle�x��]����Quaternion���v�Z����
		//return: axis�̎����angle�x��]����Quaternion
		//����1: ��]��
		//����2: �p�x (degree)
		inline static Quaternion AngleAxis(const Vector3 & axis, float angle) noexcept;
		//----------------------------------------------------------------------------------
		//[Euler]<Calculation Library Function>
		//�I�C���[�p (x, y, z) �����Quaternion���v�Z����, ��]����YXZ
		//return: �I�C���[�p (x, y, z) ����ɂ���Quaternion
		//����1: ��] (degrees)
		inline static Quaternion Euler(const Vector3 & eulerAngles) noexcept;
		//----------------------------------------------------------------------------------
		//[Lerp3D]<Calculation Library Function>
		//from��to�̊Ԃ�t�Ő��`�⊮�������̂�ԋp����, �⊮�ɂ�3���֐��⊮���g��
		//return: from����to�ւ̐��`(3���֐�)���
		//����1: from
		//����2: to
		//����3: �⊮�̓x����(0 ~ 1)
		inline static Quaternion Lerp3D(const Quaternion & from, const Quaternion& to, float t) noexcept;
		//----------------------------------------------------------------------------------
		//[LerpDirect]<Calculation Library Function>
		//from��to�̊Ԃ�t�Ő��`�⊮�������̂�ԋp����, �⊮�ɂ͂����̐��`��Ԃ��g��
		//return: from����to�ւ̐��`���
		//����1: from
		//����2: to
		//����3: �⊮�̓x����(0 ~ 1)
		inline static Quaternion LerpDirect(const Quaternion& from, const Quaternion& to, float t) noexcept;
		//----------------------------------------------------------------------------------
		//[Slerp]<Calculation Library Function>
		//this��to�̊Ԃ�t�ŋ��ʐ��`��Ԃ������̂��v�Z����
		//return: from����to�ւ̋��ʐ��`���
		//����1: from
		//����2: to
		//����3: �⊮�̓x����(0 ~ 1)
		inline static Quaternion Slerp(const Quaternion& from, const Quaternion& to, float t) noexcept;
		//----------------------------------------------------------------------------------
		//[Dot]<Calculation Library Function>
		//quaternion1��quaternion2�̓��ς��v�Z����
		//return: quaternion1��quaternion2�̓���
		//����1: quaternion1
		//����2: quaternion2
		inline float Dot(const Quaternion& quaternion1, const Quaternion& quaternion2) noexcept;
		//----------------------------------------------------------------------------------
		//[Vector3Rotate]<Calculation Library Function>
		//�x�N�g��������quaternion���g���ĉ�]������
		//return: ��]�����x�N�g��
		//����1: �Ώۃx�N�g��
		//����2: ��]�Ɏg��quaternion
		inline static Vector3 Vector3Rotate(const Vector3 & vec, const Quaternion & quaternion) noexcept;
	};

	//----------------------------------------------------------------------------------
	//[wostream operator]
	//����1: stream
	//����2: this
	inline std::wostream& operator<<(std::wostream& stream, const Quaternion& value)
	{
		const wchar* string = value.ToString();
		stream << string;
		return stream;
	}

	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//identity�ɐݒ肳���
	inline constexpr Quaternion::Quaternion() : XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) {}
	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//�������
	//����1: x
	//����2: y
	//����3: z
	//����4: w
	inline constexpr Quaternion::Quaternion(float x, float y, float z, float w) : XMFLOAT4(x, y, z, w) {}
	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//�������
	//����1 : XmVector(XMVECTOR)
	inline Quaternion::Quaternion(const XmVector & mathQuaternion)
	{
		//Set
		DirectX::XMStoreFloat4(this, mathQuaternion);
	}
	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//�������
	//����1: Matrix(XMMATRIX)
	inline Quaternion::Quaternion(const Matrix & matrix)
	{
		//Set
		DirectX::XMStoreFloat4(this, DirectX::XMQuaternionRotationMatrix(matrix));
	}
	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//�������������
	//����1: PxVec4
	inline constexpr Quaternion::Quaternion(const physx::PxQuat & quaternion)
		: Quaternion(quaternion.x, quaternion.y, quaternion.z, quaternion.w) {}
	//----------------------------------------------------------------------------------
	//[�R���X�g���N�^]
	//�������������
	//����1: aiQuaternion
	inline constexpr Quaternion::Quaternion(const aiQuaternion & quaternion)
		: Quaternion(quaternion.x, quaternion.y, quaternion.z, quaternion.w) {}



	//----------------------------------------------------------------------------------
	//[Inverse]<Calculation Library Function>
	//�t�N�H�[�^�j�I���ɕϊ�����
	//return: �t�N�H�[�^�j�I��
	//����1: �ϊ�����quaternion
	inline Quaternion Quaternion::Inverse(const Quaternion & quaternion) noexcept
	{
		return std::move(DirectX::XMQuaternionInverse(DirectX::XMLoadFloat4(&quaternion)));
	}
	//----------------------------------------------------------------------------------
	//[LookForward]<Calculation Library Function>
	//return: forward������Quaternion
	//����1: forward
	//����2: �����: default = Vector3::up
	inline Quaternion Quaternion::LookForward(const Vector3 & forward, const Vector3 & upwards) noexcept
	{
		return std::move(Matrix::LookForward(forward, upwards));
	}
	//----------------------------------------------------------------------------------
	//[LookAt]<Calculation Library Function>
	//position����target������`�̉�]���v�Z����
	//return: position����target������`�̉�]
	//����1: position
	//����2: target
	//����3: �����: default = Vector3::up
	inline Quaternion Quaternion::LookAt(const Vector3 & position, const Vector3 & target, const Vector3 & upwards) noexcept
	{
		return std::move(Matrix::LookAt(position, target, upwards));
	}
	//----------------------------------------------------------------------------------
	//[LookAt2D]<Calculation Library Function>
	//position����target������`�̉�]���v�Z����(2D��)
	//return: position����target������`�̉�](���w��)
	//����1: position
	//����2: target
	//����3: ��]�� (X or Y): default = true (X)
	inline Quaternion Quaternion::LookAt2D(const Vector3 & position, const Vector3 & target, bool isAxisX) noexcept
	{
		//�����x�N�g��
		Vector3 direction = target - position;
		//�p�x���Z�p
		float forwardDiff;
		if (isAxisX)	forwardDiff = 0.0f;
		else forwardDiff = 90.0f;
		//return
		return std::move(DirectX::XMQuaternionRotationAxis(
			DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), (std::atan2(direction.y, direction.x) * MathAF::cRadToDeg) - forwardDiff));
	}
	//----------------------------------------------------------------------------------
	//[FromToRotation]<Calculation Library Function>
	//from����to�։�]����Quaternion���v�Z����
	//return: from����to�ւ̉�]
	//����1: from (���K����)
	//����2: to (���K����)
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
	//axis�̎����angle�x��]����Quaternion���v�Z����
	//return: axis�̎����angle�x��]����Quaternion
	//����1: ��]��
	//����2: �p�x (degree)
	inline Quaternion Quaternion::AngleAxis(const Vector3 & axis, float angle) noexcept
	{
		return std::move(DirectX::XMQuaternionRotationAxis(axis, angle * MathAF::cDegToRad));
	}
	//----------------------------------------------------------------------------------
	//[Euler]<Calculation Library Function>
	//�I�C���[�p (x, y, z) �����Quaternion���v�Z����, ��]����YXZ
	//return: �I�C���[�p (x, y, z) ����ɂ���Quaternion
	//����1: ��] (degrees)
	inline Quaternion Quaternion::Euler(const Vector3 & eulerAngles) noexcept
	{
		return std::move(DirectX::XMQuaternionRotationRollPitchYaw(
			eulerAngles.x * MathAF::cDegToRad, eulerAngles.y * MathAF::cDegToRad, eulerAngles.z * MathAF::cDegToRad));
	}
	//----------------------------------------------------------------------------------
	//[Lerp3D]<Calculation Library Function>
	//from��to�̊Ԃ�t�Ő��`�⊮�������̂�ԋp����, �⊮�ɂ�3���֐��⊮���g��
	//return: from����to�ւ̐��`(3���֐�)���
	//����1: from
	//����2: to
	//����3: �⊮�̓x����(0 ~ 1)
	inline Quaternion Quaternion::Lerp3D(const Quaternion & from, const Quaternion & to, float t) noexcept
	{
		if (t > 1.0f) t = 1.0f;	else if (t < 0.0f) t = 0.0f;
		//3���֐����
		float rate = t * t * (3.0f - 2.0f * t);

		return std::move(DirectX::XMQuaternionMultiply(from * (1.0f - rate), to * rate));
	}
	//----------------------------------------------------------------------------------
	//[LerpDirect]<Calculation Library Function>
	//from��to�̊Ԃ�t�Ő��`�⊮�������̂�ԋp����, �⊮�ɂ͂����̐��`��Ԃ��g��
	//return: from����to�ւ̐��`���
	//����1: from
	//����2: to
	//����3: �⊮�̓x����(0 ~ 1)
	inline Quaternion Quaternion::LerpDirect(const Quaternion & from, const Quaternion & to, float t) noexcept
	{
		if (t > 1.0f) t = 1.0f;	else if (t < 0.0f) t = 0.0f;
		return std::move(DirectX::XMQuaternionMultiply(from * (1.0f - t), to * t));
	}
	//----------------------------------------------------------------------------------
	//[Slerp]<Calculation Library Function>
	//this��to�̊Ԃ�t�ŋ��ʐ��`��Ԃ������̂��v�Z����
	//return: from����to�ւ̋��ʐ��`���
	//����1: from
	//����2: to
	//����3: �⊮�̓x����(0 ~ 1)
	inline Quaternion Quaternion::Slerp(const Quaternion & from, const Quaternion & to, float t) noexcept
	{
		if (t > 1.0f) t = 1.0f;	else if (t < 0.0f) t = 0.0f;
		return std::move(DirectX::XMQuaternionSlerp(DirectX::XMLoadFloat4(&from), DirectX::XMLoadFloat4(&to), t));
	}
	//----------------------------------------------------------------------------------
	//[Dot]<Calculation Library Function>
	//quaternion1��quaternion2�̓��ς��v�Z����
	//return: quaternion1��quaternion2�̓���
	//����1: quaternion1
	//����2: quaternion2
	inline float Quaternion::Dot(const Quaternion & quaternion1, const Quaternion & quaternion2) noexcept
	{
		return quaternion1.x * quaternion2.x + quaternion1.y * quaternion2.y 
			+ quaternion1.z * quaternion2.z + quaternion1.w * quaternion2.w;
	}
	//----------------------------------------------------------------------------------
	//[Vector3Rotate]<Calculation Library Function>
	//�x�N�g��������quaternion���g���ĉ�]������
	//return: ��]�����x�N�g��
	//����1: �Ώۃx�N�g��
	//����2: ��]�Ɏg��quaternion
	inline Vector3 Quaternion::Vector3Rotate(const Vector3 & vec, const Quaternion & quaternion) noexcept
	{
		return std::move(DirectX::XMVector3Rotate(vec, quaternion));
	}
}
#endif // !SGFRAMEWORK_HEADER_QUATERNION_HPP_
