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
	//return: ���g��Call�����Callback Flags, GameObject::CallbackFunctions�Q��
	inline uint EnableCallbacks() { return CallbackFunctions::Awake | CallbackFunctions::Update 
		| CallbackFunctions::MakeDrawCommand | CallbackFunctions::OnCollisionEnter; }

	//----------------------------------------------------------------------------------
	//[OnCollisionEnter]
	//Collider�����������u�Ԃ�Callback�����֐�
	//����1: �ڐG�I�u�W�F�N�g�̏��
	inline virtual void OnCollisionEnter(const ContactInfo& contactInfo) 
	{
		//transform->getWorldPosition();
		m_effect->Play(transform->getWorldPosition(), -Const::Vector3::up * Time::fixedDeltaTime, Const::Quaternion::identity);
	}
	//----------------------------------------------------------------------------------
	//[Awake]
	//�C���X�^���X�������u�ԂɌĂ΂��Callback�֐�
	inline void Awake() 
	{
		InitModel();
		AddComponent<RigidBodyDynamic>();
		auto collider = AddComponent<SphereCollider>();
		collider->BuildCollider(1.0f);
	}

	//----------------------------------------------------------------------------------
	//[Update]
	//���t���[��Update�u���b�N��Callback�����֐�
	inline virtual void Update() 	
	{
	//	for (int i = 0; i < 1000; ++i) {}
	}

	//----------------------------------------------------------------------------------
	//[MakeDrawCommand]
	//Update, Sync, LateUpdate �u���b�N���Callback�����,
	//		DrawCommand���쐬���邽�߂̊֐�
	inline void MakeDrawCommand()
	{
		Graphics::SendDrawFunction(MAKE_COMMAND(
			transform->setDrawMatrix();

		// ���_�o�b�t�@���Z�b�g����
		unsigned int stride = sizeof(Vertex::Polygon);
		unsigned  offset = 0;
		Graphics::context->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

		Graphics::context->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);			// �C���f�b�N�X�o�b�t�@���Z�b�g
		Graphics::context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);		// �g�|���W�[���Z�b�g�i���v���~�e�B�u�^�C�v

		Graphics::context->DrawIndexed(200 * 200 * 2 * 3,		// �`�悷��C���f�b�N�X���i�ʐ��~�R�j
			0,									// �ŏ��̃C���f�b�N�X�o�b�t�@�̈ʒu
			0);									// ���_�o�b�t�@�̍ŏ�����g��
	
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