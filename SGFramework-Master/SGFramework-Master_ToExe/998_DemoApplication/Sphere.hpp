#ifndef SPHERE_HPP_
#define SPHERE_HPP_
#include "../SGFramwork.hpp"

class DemoScene;

class Sphere : public GameObject
{
public:
	friend class DemoScene;

	//----------------------------------------------------------------------------------
	//[EnabledCallbacks]
	//return: 自身がCallされるCallback Flags, GameObject::CallbackFunctions参照
	inline uint EnableCallbacks() { return CallbackFunctions::Awake | CallbackFunctions::Update 
		| CallbackFunctions::MakeDrawCommand | CallbackFunctions::OnCollisionEnter; }

	//----------------------------------------------------------------------------------
	//[OnCollisionEnter]
	//Colliderが当たった瞬間にCallbackされる関数
	//引数1: 接触オブジェクトの情報
	inline virtual void OnCollisionEnter(const ContactInfo& contactInfo) 
	{
		//transform->getWorldPosition();
		m_effect->Play(transform->getWorldPosition(), -Const::Vector3::up * Time::fixedDeltaTime, Const::Quaternion::identity);
	}
	//----------------------------------------------------------------------------------
	//[Awake]
	//インスタンス化した瞬間に呼ばれるCallback関数
	inline void Awake() 
	{
		InitModel();
		AddComponent<RigidBodyDynamic>();
		auto collider = AddComponent<SphereCollider>();
		collider->BuildCollider(1.0f);
	}

	//----------------------------------------------------------------------------------
	//[Update]
	//毎フレームUpdateブロックでCallbackされる関数
	inline virtual void Update() 	
	{
	//	for (int i = 0; i < 1000; ++i) {}
	}

	//----------------------------------------------------------------------------------
	//[MakeDrawCommand]
	//Update, Sync, LateUpdate ブロック後にCallbackされる,
	//		DrawCommandを作成するための関数
	inline void MakeDrawCommand()
	{
		Graphics::SendDrawFunction(MAKE_COMMAND(
			transform->setDrawMatrix();

		// 頂点バッファをセットする
		unsigned int stride = sizeof(Vertex::Polygon);
		unsigned  offset = 0;
		Graphics::context->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

		Graphics::context->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);			// インデックスバッファをセット
		Graphics::context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);		// トポロジーをセット（旧プリミティブタイプ

		Graphics::context->DrawIndexed(200 * 200 * 2 * 3,		// 描画するインデックス数（面数×３）
			0,									// 最初のインデックスバッファの位置
			0);									// 頂点バッファの最初から使う
	
		), 4);
	}

private:
	struct Face
	{
		uint indexes[3];
	};

	static void InitModel();

	static Face* m_faces;
	static Vertex::Polygon* m_vertexes;
	static ID3D11Buffer* m_vertexBuffer;
	static ID3D11Buffer* m_indexBuffer;
	static SharedPointer<Asset::AudioAssetEffect3D> m_effect;
};

#endif //!SPHERE_HPP_