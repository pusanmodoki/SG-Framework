#ifndef DEBUG_SCENE_HPP_
#define DEBUG_SCENE_HPP_
#include "../SGFramwork.hpp"
#include "Character.hpp"
#include "Sphere.hpp"
//#include "AcademySource/CModel.h"

using namespace SGFramework;


class DemoScene : public Scene::BaseScene
{
public:
	DemoScene() : Scene::BaseScene(){}
	
	//----------------------------------------------------------------------------------
	//[StartScene]
	//�V�[���쐬�������ŌĂяo�����R�[���o�b�N�֐�
	//����1: �O�V�[������̈����p�����
	void StartScene(Scene::SceneBaton& nextBaton) override;
	
	//----------------------------------------------------------------------------------
	//[EndScene]
	//�V�[���I���������ŌĂяo�����R�[���o�b�N�֐�, �I���������s��
	//�Ԃ�l��null�|�C���^�������ꍇ�A�A�v���P�[�V�������I������
	//����1: ���V�[���ֈ����p�����, Start�ŌĂяo�����
	//����2: true��������Ǝ��V�[���ȊO�̃X�^�b�N��{�V�[���܂߂ăX�^�b�N����N���A����
	//����2: true��������Ɩ{�V�[�����X�^�b�N���炷��
	virtual UniquePointer<BaseScene> EndScene(Scene::SceneBaton& nextBaton, bool& isStackClear, bool& isStackPop)
	{
		COM_RELEASE(Sphere::m_vertexBuffer);
		COM_RELEASE(Sphere::m_indexBuffer);
		DELETE_POINTER(Sphere::m_faces);
		DELETE_POINTER(Sphere::m_vertexes);
		
		Sphere::m_effect.WaiverAndReset();
		bool isNotEqual = true;
		bool isNull = false;
		
		for (size_t i = 0; i < m_spheres.size(); ++i)
		{
			if (m_spheres[i].getPointer() == nullptr)
				isNull = true;
			for (size_t k = i + 1; k < m_spheres.size(); ++k)
			{
				if (m_spheres[i].getPointer() == m_spheres[k].getPointer())
					isNotEqual = false;
			}
		}


		m_spheres.clear();
		
		return UniquePointer<BaseScene>::getNull();
	}

	//----------------------------------------------------------------------------------
	//[RewindScene]
	//�V�[�����I�������V�[���Ȃ� & isPop�Ŗ{�V�[���ɖ߂��Ă����ꍇ��
	//�����ŌĂяo�����R�[���o�b�N�֐�
	//����1: ���V�[������̈����p�����
	inline virtual void RewindScene(Scene::SceneBaton& nextBaton) {}

	//----------------------------------------------------------------------------------
	//[Update]
	//����X�V�������ŌĂяo�����R�[���o�b�N�֐�
	//���̃R�[���o�b�N�Ɠ����ɕ���Ăяo������A�I�u�W�F�N�g�̍X�V�����s��
	void Update();
	//----------------------------------------------------------------------------------
	//[Sync]
	//�����X�V�������ŌĂяo�����R�[���o�b�N�֐�
	//Update��ɃV���O���X���b�h�ŌĂяo����A�ϐ��̋��L�⓯�����s��
	//return: �V�[�����s -> true
	virtual bool Sync() { return true; }

	//----------------------------------------------------------------------------------
	//[LateUpdate]
	//Update & Sync�I����̕���X�V�������ŌĂяo�����R�[���o�b�N�֐�
	//���̃R�[���o�b�N�Ɠ����ɕ���Ăяo������A�I�u�W�F�N�g�̍X�V�����s��
	inline virtual void LateUpdate() {}

	//----------------------------------------------------------------------------------
	//[FixedUpdate]
	//Fixed�t���[���ł̕���X�V�������ŌĂяo�����R�[���o�b�N�֐�
	//���̃R�[���o�b�N�Ɠ����ɕ���Ăяo������A�I�u�W�F�N�g�̍X�V�����s��
	inline virtual void FixedUpdate() {}
	//----------------------------------------------------------------------------------
	//[FixedSync]
	//Fixed�t���[���ł̓����X�V�������ŌĂяo�����R�[���o�b�N�֐�
	//Update��ɌĂяo����A�ϐ��̋��L�⓯�����s��
	inline virtual void FixedSync(){}

	//----------------------------------------------------------------------------------
	//[MakeDrawCommand]
	//�`��X�V�������ŌĂяo�����R�[���o�b�N�֐�
	//�`��o�^�����s��
	inline virtual void MakeDrawCommand();

	//scene name (read function property)
	SGF_FUNCTION_PROPERTY virtual sgstring getSceneName() const { return L"GameScene"; }

private:
	std::vector<SharedPointer<Sphere>> m_spheres;
	SharedPointer<Player> m_character;
	SharedPointer<Asset::ModelAsset> m_modelAsset;
	SharedPointer<Asset::VertexShaderAsset> m_vertexShaderModel;
	SharedPointer<Asset::PixelShaderAsset> m_pixelShaderModel;
	SharedPointer<Asset::VertexShaderAsset> m_vertexShaderPolygon;
	SharedPointer<Asset::PixelShaderAsset> m_pixelShaderPolygon;
	
	std::vector<SharedPointer<GameObject>> m_largeAmountObjects;
	std::vector<SharedPointer<Asset::ModelAsset>> m_largeAmountModelAssets;

	//Polygon class�͈ȑO�̐݌v�v�z�Ő��삳�ꂽ���K�V�[�ȃN���X�����A���Ԃ��Ȃ����߂��̂܂܎g�p
	SharedPointer<PolygonSquare> m_ground;

	TimeProcessing::TimeLimiter m_limiter;
	bool m_isCreateSphereMode = false;
};
#endif // !DEBUG_SCENE_HPP_