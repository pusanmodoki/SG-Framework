#include "DemoScene.hpp"
#include "DemoMusicPlayer.hpp"
#include "../06_Audio/AudioDetail/AudioMessage.hpp"

Scene::SceneManager manager(std::move(Scene::SceneManager::InstantiateFirstScene<DemoScene>()), true);



//----------------------------------------------------------------------------------
//[StartScene]
//シーン作成時自動で呼び出されるコールバック関数
//引数1: 前シーンからの引き継ぎ情報
void DemoScene::StartScene(Scene::SceneBaton& nextBaton)
{
	Light::Directional::setParameter(0, Light::Directional::Element(Vector3(0.0f, -0.5f, 0.5f), Const::Color::white, 9.5f, true));

	m_vertexShaderModel = Asset::InstantiateAsset<Asset::VertexShaderAsset>(L"Assets/Shader/VertexModelStandard.hlsl");
	m_vertexShaderModel->Open(Asset::VertexShaderAsset::getModelVertexInputElements());

	m_pixelShaderModel = Asset::InstantiateAsset<Asset::PixelShaderAsset>(L"Assets/Shader/PixelModelStandard.hlsl");
	m_pixelShaderModel->Open();

	m_character = GameObject::Instantiate<Player>();

	m_modelAsset = Asset::InstantiateAsset<Asset::ModelAsset>(L"Assets/fbx/Dragon 2.5_fbx.fbx");

	m_vertexShaderPolygon = Asset::InstantiateAsset<Asset::VertexShaderAsset>(L"Assets/Shader/VertexDirect3D.hlsl");
	m_vertexShaderPolygon->Open(Asset::VertexShaderAsset::getPolygonVertexInputElements());

	m_pixelShaderPolygon = Asset::InstantiateAsset<Asset::PixelShaderAsset>(L"Assets/Shader/PixelDirect.hlsl");
	m_pixelShaderPolygon->Open();

	m_ground = PolygonSquare::Instantiate(Vector3(0.0f, -5.0f, 0.0f), Vector3(200.0f, 200.0f, 0.1f), Matrix::LookAt(Const::Vector3::zero.ToXmVector(), (Const::Vector3::up).ToXmVector()));
	m_ground->SetColorForAllVertexes(Const::Color::white);
	m_ground->transform->AddComponent<RigidBodyStatic>();
	auto collider = m_ground->transform->AddComponent<BoxCollider>();
	collider->BuildCollider(Vector3(1.0f, 1.0f, 0.1f));

//	m_effect3D = Asset::InstantiateAsset<Asset::AudioAssetMusic>();

	auto pack  = Application::applicationSetting().SearchPack(L"numDemoCopyModels");

	for (int i = 0; i < pack[0].valueInt; ++i)
	{
		auto obj  = GameObject::Instantiate<GameObject>(L"Large Amount Model : " + i);
		obj->transform->setWorldScale(Vector3(0.3f, 0.3f, 0.3f));
		obj->transform->Rotate(Quaternion::AngleAxis(Const::Vector3::right, 90) * Quaternion::AngleAxis(Const::Vector3::up, 180));
		obj->transform->setWorldPosition(Vector3(
			Random::Range(-100, 100), 1.0f, Random::Range(-100, 100)));

		m_largeAmountObjects.emplace_back(obj);
	
		auto model = Asset::InstantiateAsset<Asset::ModelAsset>(L"Assets/fbx/Dragon 2.5_fbx.fbx", false);
		model->Open(obj, m_vertexShaderModel, m_pixelShaderModel);
		model->OpenAnimation(L"Assets/fbx/Dragon_Baked_Actions_fbx_7.4_binary.fbx");

		m_largeAmountModelAssets.emplace_back(model);

		obj->FindComponent<Animator>()->PlayAnimation(i % 5);
	}

}

//----------------------------------------------------------------------------------
//[Update]
//並列更新時自動で呼び出されるコールバック関数
//他のコールバックと同時に並列呼び出しされ、オブジェクトの更新等を行う
void DemoScene::Update()
{
	if (Input::Keybord::GetKey(KeyCode::C) && Input::Keybord::GetKeyDown(KeyCode::Num1))
	{
		//for (int i = 0; i < 10; ++i)
		//{
		//	auto result = GameObject::Instantiate<Sphere>(Vector3(
		//		Random::Range(-200, 200), 10.0f, Random::Range(-200, 200)), Quaternion());
		//	
		//	m_spheres.emplace_back(result);
		//}
		m_isCreateSphereMode = !m_isCreateSphereMode;
		m_limiter.Start(0.1f);
	}
	else if (Input::Keybord::GetKey(KeyCode::C) && Input::Keybord::GetKeyDown(KeyCode::Num2))
	{
		for (auto& e : m_spheres)
			GameObject::DestroyObject(e);
		m_spheres.clear();
	}


	if (Input::Keybord::GetKey(KeyCode::V && Input::Keybord::GetKeyDown(KeyCode::Num1)))
	{
		GUI::getLogStream() << GUI::cLogFrameworkMessageHeader << L"Hello!!";
	}

	if (m_isCreateSphereMode && m_limiter())
	{
		for (int i = 0; i < 15; ++i)
		{
			auto result = GameObject::Instantiate<Sphere>(Vector3(
				Random::Range(-200, 200), 10.0f, Random::Range(-200, 200)), Quaternion());
			m_spheres.emplace_back(result);
		}
	}
}
//----------------------------------------------------------------------------------
//[MakeDrawCommand]
//描画更新時自動で呼び出されるコールバック関数
//描画登録等を行う
void DemoScene::MakeDrawCommand()
{
	Graphics::SendDrawFunction(MAKE_COMMAND(m_pixelShaderPolygon->setShader()), 2);
	Graphics::SendDrawFunction(MAKE_COMMAND(m_vertexShaderPolygon->setShader()), 2);
	Graphics::SendDrawFunction(MAKE_COMMAND(m_ground->Draw3D()), 3);
}
