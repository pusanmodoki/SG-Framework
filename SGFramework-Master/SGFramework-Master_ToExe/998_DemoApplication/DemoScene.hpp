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
	//シーン作成時自動で呼び出されるコールバック関数
	//引数1: 前シーンからの引き継ぎ情報
	void StartScene(Scene::SceneBaton& nextBaton) override;
	
	//----------------------------------------------------------------------------------
	//[EndScene]
	//シーン終了時自動で呼び出されるコールバック関数, 終了処理を行う
	//返り値がnullポインタだった場合、アプリケーションが終了する
	//引数1: 次シーンへ引き継ぐ情報, Startで呼び出される
	//引数2: trueを代入すると次シーン以外のスタックを本シーン含めてスタックからクリアする
	//引数2: trueを代入すると本シーンをスタックからする
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
	//シーンが終了し次シーンなし & isPopで本シーンに戻ってきた場合に
	//自動で呼び出されるコールバック関数
	//引数1: 次シーンからの引き継ぐ情報
	inline virtual void RewindScene(Scene::SceneBaton& nextBaton) {}

	//----------------------------------------------------------------------------------
	//[Update]
	//並列更新時自動で呼び出されるコールバック関数
	//他のコールバックと同時に並列呼び出しされ、オブジェクトの更新等を行う
	void Update();
	//----------------------------------------------------------------------------------
	//[Sync]
	//同期更新時自動で呼び出されるコールバック関数
	//Update後にシングルスレッドで呼び出され、変数の共有や同期を行う
	//return: シーン続行 -> true
	virtual bool Sync() { return true; }

	//----------------------------------------------------------------------------------
	//[LateUpdate]
	//Update & Sync終了後の並列更新時自動で呼び出されるコールバック関数
	//他のコールバックと同時に並列呼び出しされ、オブジェクトの更新等を行う
	inline virtual void LateUpdate() {}

	//----------------------------------------------------------------------------------
	//[FixedUpdate]
	//Fixedフレームでの並列更新時自動で呼び出されるコールバック関数
	//他のコールバックと同時に並列呼び出しされ、オブジェクトの更新等を行う
	inline virtual void FixedUpdate() {}
	//----------------------------------------------------------------------------------
	//[FixedSync]
	//Fixedフレームでの同期更新時自動で呼び出されるコールバック関数
	//Update後に呼び出され、変数の共有や同期を行う
	inline virtual void FixedSync(){}

	//----------------------------------------------------------------------------------
	//[MakeDrawCommand]
	//描画更新時自動で呼び出されるコールバック関数
	//描画登録等を行う
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

	//Polygon classは以前の設計思想で制作されたレガシーなクラスだが、時間がないためそのまま使用
	SharedPointer<PolygonSquare> m_ground;

	TimeProcessing::TimeLimiter m_limiter;
	bool m_isCreateSphereMode = false;
};
#endif // !DEBUG_SCENE_HPP_