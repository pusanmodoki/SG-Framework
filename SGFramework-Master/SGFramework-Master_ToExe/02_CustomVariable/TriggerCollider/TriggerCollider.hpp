/*----------------------------------------------------------------------------------
Collider��Trigger���w�肷��TriggerCollider.h
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_COLLIDER_TRIGGER_HPP_
#define SGFRAMEWORK_HEADER_COLLIDER_TRIGGER_HPP_

//Framework namespace
namespace SGFramework
{
	//�R���C�_�[�̃g���K�[
	enum class Triggers
	{
		TriggerNull = 0x0,	//�����l
		TriggerFalse = 0x1,	//�����Ȃ�
		TriggerTrue = 0x2,	//��������
	};

	//�R���C�_�[�̎��
	enum class Colliders
	{
		Mesh,					//���b�V���R���C�_�[
		Ray,						//���C�L���X�g
		Box3D,					//�{�b�N�X�R���C�_�[ (3D)
		Box2D,					//�{�b�N�X�R���C�_�[ (2D)
		Sphere,					//�X�t�B�A�R���C�_�[ (3D)
		Circle,					//�T�[�N���R���C�_�[ (2D)
	};
}
#endif // !SGFRAMEWORK_HEADER_COLLIDER_TRIGGER_HPP_
