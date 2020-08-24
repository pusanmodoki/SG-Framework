#ifndef SGFRAMEWORK_HEADER_MODEL_ASSET_HPP_
#define SGFRAMEWORK_HEADER_MODEL_ASSET_HPP_
#include <assimp/scene.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/cimport.h>
#include <assimp/cfileio.h>
#include "../../../02_CustomVariable/CustomVariable.hpp"
#include "../../../11_GameObject/GameObject.hpp"
#include "../Material/MaterialX11.hpp"
#include "../Mesh/MeshX11.hpp"
#include "../../Vertex/Vertex.hpp"

#ifdef _DEBUG
#pragma comment (lib, "assimp-vc141-mtd.lib")
#else // !_DEBUG
#pragma comment (lib, "assimp-vc141-mt.lib")
#endif

// Framework namespace
namespace SGFramework
{
	class Animator;
	class ModelNodeObject;

	//フレームワークの管理をするAdministrator namespace
	namespace Administrator
	{
		class AssetManager;
		//GraphicsDetail namespace
		namespace GraphicsDetail
		{
			class ModelAssetX11 final : public BaseClass::BaseAsset
			{
			public:
				friend class AssetManager;
				template <typename TAsset>
				friend SharedPointer<TAsset> Asset::InstantiateAsset(const sgstring& filePath, bool isRegisterSharedAsset);

				//----------------------------------------------------------------------------------
				//[デストラクタ]
				inline ~ModelAssetX11()
				{
					for (auto& e : m_meshs)
						e->ReleaseBuffers();

					aiReleaseImport(m_scene);
					CloseBaseAsset(getType(), true);
				}

				//----------------------------------------------------------------------------------
				//[Open]
				void Open(WeakPointer<GameObject> loadObject,
					SharedPointer<Administrator::GraphicsDetail::VertexShaderAssetX11> defaultVertexShader,
					SharedPointer<Administrator::GraphicsDetail::PixelShaderAssetX11> defaultPixelShader,
					uint drawPriority = 1);

				//----------------------------------------------------------------------------------
				//[OpenAnimation]
				void OpenAnimation(const sgstring& filePath);

				//----------------------------------------------------------------------------------
				//[Reload]
				//アセットのリロードを行う
				void Reload() override;
				//----------------------------------------------------------------------------------
				//[Close]
				//ファイルを閉じる
				void Close() override;

				//get asset type (read function property)
				SGF_FUNCTION_PROPERTY Asset::AssetType getType() override { return Asset::AssetType::Model; }


	
			private:
				inline ModelAssetX11() : m_scene(nullptr), m_rootObject(), 
					m_rootAnimator(), m_meshs(), m_materials(),
					m_nodeObjectsKeyNode(), m_nodeObjectsKeyName(), m_drawPriority(1) {}

				void ProcessNodeCreateObject(aiNode* node);
				void ProcessNodeCreateMesh(aiNode* node);

				void CreateMesh(aiNode* node, aiMesh * mesh, int meshidx);

				void FindTexture(Administrator::GraphicsDetail::MaterialX11* material,
					aiMaterial* assimpMaterial, aiTextureType type, const sgstring& filePath);

				const aiScene* m_scene;
				WeakPointer<GameObject> m_rootObject;
				WeakPointer<Animator> m_rootAnimator;
				std::vector<SharedPointer<MeshX11<Vertex::Model, MaterialX11*>>> m_meshs;
				std::vector<SharedPointer<MaterialX11>> m_materials;
				std::unordered_map<std::wstring, const aiScene*> m_animationScenes;
				std::unordered_map<aiNode*, WeakPointer<ModelNodeObject>> m_nodeObjectsKeyNode;
				std::unordered_map<std::string, WeakPointer<ModelNodeObject>> m_nodeObjectsKeyName;
				uint m_drawPriority;
			};
		}
	}
}

#endif //!SGFRAMEWORK_HEADER_MODEL_ASSET_HPP_