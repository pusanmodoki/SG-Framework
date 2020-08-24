#ifndef PLAYER_HPP_
#define PLAYER_HPP_
#include "../SGFramwork.hpp"

class Player : public GameObject
{
public:

	//----------------------------------------------------------------------------------
	//[EnabledCallbacks]
	//return: ���g��Call�����Callback Flags, GameObject::CallbackFunctions�Q��
	inline uint EnableCallbacks() override { return CallbackFunctions::Awake | CallbackFunctions::MakeDrawCommand
		| CallbackFunctions::Update | CallbackFunctions::FixedUpdate; }

	//----------------------------------------------------------------------------------
	//[Awake]
	//�C���X�^���X�������u�ԂɌĂ΂��Callback�֐�
	void Awake() override;

	//----------------------------------------------------------------------------------
	//[Update]
	//���t���[��Update�u���b�N��Callback�����֐�
	void Update();

	//----------------------------------------------------------------------------------
	//[FixedUpdate]
	//����I�ɌĂ΂��Fixed�t���[������Update�Ƃ���Callback�����֐�
	void FixedUpdate() override;

	//----------------------------------------------------------------------------------
	//[MakeDrawCommand]
	//Update, Sync, LateUpdate �u���b�N���Callback�����,
	//		DrawCommand���쐬���邽�߂̊֐�
	void MakeDrawCommand();

private:
	SharedPointer<GameObject> m_model;
	SharedPointer<GameObject> m_camera;
	WeakPointer<Camera> m_cameraComponent;
	WeakPointer<RigidBodyDynamic> m_rigidBody;

	SharedPointer<Asset::ModelAsset> m_modelAsset;
	SharedPointer<Asset::VertexShaderAsset> m_vertexShaderModel;
	SharedPointer<Asset::PixelShaderAsset> m_pixelShaderModel;

	bool m_isPlayAnimation;
};

#endif