#ifndef PLAYER_HPP_
#define PLAYER_HPP_
#include "../SGFramwork.hpp"

class Player : public GameObject
{
public:

	//----------------------------------------------------------------------------------
	//[EnabledCallbacks]
	//return: 自身がCallされるCallback Flags, GameObject::CallbackFunctions参照
	inline uint EnableCallbacks() override { return CallbackFunctions::Awake | CallbackFunctions::MakeDrawCommand
		| CallbackFunctions::Update | CallbackFunctions::FixedUpdate; }

	//----------------------------------------------------------------------------------
	//[Awake]
	//インスタンス化した瞬間に呼ばれるCallback関数
	void Awake() override;

	//----------------------------------------------------------------------------------
	//[Update]
	//毎フレームUpdateブロックでCallbackされる関数
	void Update();

	//----------------------------------------------------------------------------------
	//[FixedUpdate]
	//定期的に呼ばれるFixedフレーム内でUpdateとしてCallbackされる関数
	void FixedUpdate() override;

	//----------------------------------------------------------------------------------
	//[MakeDrawCommand]
	//Update, Sync, LateUpdate ブロック後にCallbackされる,
	//		DrawCommandを作成するための関数
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