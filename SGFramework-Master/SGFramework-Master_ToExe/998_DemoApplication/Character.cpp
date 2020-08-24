#include "Character.hpp"
#include "DemoMusicPlayer.hpp"

//----------------------------------------------------------------------------------
//[Awake]
//インスタンス化した瞬間に呼ばれるCallback関数
void Player::Awake()
{
	AddComponent<AudioListener>();
	AddComponent<DemoMusicPlayer>();

	m_vertexShaderModel = Asset::InstantiateAsset<Asset::VertexShaderAsset>(L"Assets/Shader/VertexModelStandard.hlsl");
	m_vertexShaderModel->WaitLoading();

	m_pixelShaderModel = Asset::InstantiateAsset<Asset::PixelShaderAsset>(L"Assets/Shader/PixelModelStandard.hlsl");
	m_pixelShaderModel->WaitLoading();

	transform->setWorldPosition(Const::Vector3::up * 1.0f);

	m_model = GameObject::Instantiate<GameObject>(transform.InstantiateShared(),
		true, Const::Vector3::zero, Quaternion::AngleAxis(Const::Vector3::right, 90) * Quaternion::AngleAxis(Const::Vector3::up, 180));

	m_model->transform->setWorldScale(Vector3(0.3f, 0.3f, 0.3f));
	m_modelAsset = Asset::InstantiateAsset<Asset::ModelAsset>(L"Assets/fbx/Dragon 2.5_fbx.fbx");
	m_modelAsset->Open(m_model, m_vertexShaderModel, m_pixelShaderModel);
	m_modelAsset->OpenAnimation(L"Assets/fbx/Dragon_Baked_Actions_fbx_7.4_binary.fbx");

	m_camera = GameObject::InstantiateManual<GameObject>(transform.InstantiateShared(), true, 
		Vector3(0.0f, 20.0f, -20.0f), Const::Vector3::one,
		Quaternion::LookAt(Vector3(0.0f, 15.0f, -20.0f), Vector3(0.0f, 0.0f, 0.0f)), Time::rootTimeLayer(), L"Camera");

	m_cameraComponent = m_camera->AddComponent<Camera>();
	m_cameraComponent->StartUp();

	m_rigidBody = AddComponent<RigidBodyDynamic>();
	auto collider = AddComponent<BoxCollider>();
	collider->BuildCollider(Const::Vector3::one * 5.0f);

	m_rigidBody->setLockFlags(RigidBodyLock::RotationAll);
	m_isPlayAnimation = false;
}

//----------------------------------------------------------------------------------
//[Update]
//毎フレームUpdateブロックでCallbackされる関数
void Player::Update()
{
}

void Player::FixedUpdate()
{
	Vector2 axis(Input::Axis::GetAxisHorizontal(), Input::Axis::GetAxisVertical());

	if (axis.x != 0.0f || axis.y != 0.0f)
	{
		if (!m_isPlayAnimation)
		{
			m_model->FindComponent<Animator>()->PlayAnimation(2);
			m_isPlayAnimation = true;
		}
		m_rigidBody->AddForce(transform->getForward() * 30000 * Time::fixedDeltaTime);
	}
	else
	{
		if (m_isPlayAnimation)
		{
			m_model->FindComponent<Animator>()->StopAnimation();
			m_model->FindComponent<Animator>()->PlayAnimation(1);
			m_isPlayAnimation = false;
		}
	}

	float cameraSpeed = 0.0f;
	if (Input::Keybord::GetKey(KeyCode::Left))
		cameraSpeed -= 1.0f;
	if (Input::Keybord::GetKey(KeyCode::Right))
		cameraSpeed += 1.0f;

	if (cameraSpeed != 0.0f)
		transform->Rotate(Quaternion::AngleAxis(Const::Vector3::up, 30 * cameraSpeed * Time::fixedDeltaTime));
}

//----------------------------------------------------------------------------------
//[MakeDrawCommand]
//Update, Sync, LateUpdate ブロック後にCallbackされる,
//		DrawCommandを作成するための関数
void Player::MakeDrawCommand()
{
	Graphics::SendDrawFunction(MAKE_COMMAND(m_cameraComponent->setCameraMatrix()), 0);
}
