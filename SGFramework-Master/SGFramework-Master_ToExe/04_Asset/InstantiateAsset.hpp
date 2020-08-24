#ifndef SGFRAMEWORK_HEADER_INSTANTIATE_ASSET_HPP_
#define SGFRAMEWORK_HEADER_INSTANTIATE_ASSET_HPP_
#include "../02_CustomVariable/CustomVariable.hpp"
#include "../04_Asset/AssetManager.hpp"
#include "../05_Graphics/Graphics.hpp"
#include "../06_Audio/AudioAsset/AudioAssetEffect.hpp"
#include "../06_Audio/AudioAsset/AudioAssetEffect3D.hpp"
#include "../06_Audio/AudioAsset/AudioAssetMusic.hpp"

// Framework namespace
namespace SGFramework
{
	//Asset関連を格納したAsset namespace
	namespace Asset
	{
		//----------------------------------------------------------------------------------
		//[InstantiateAsset]
		//アセットクラスを生成する, (Openは別途関数を実行)
		//template: asset class
		//return: make_shared pointer
		//引数1: file path
		//引数2: asset is register shared? 
		//			(既にShared設定の同名同型ポインタがあった場合それを返却する), default = true
		template <typename TAsset>
		inline SharedPointer<TAsset> InstantiateAsset(const sgstring& filePath, bool isRegisterSharedAsset = true)
		{
			static_assert(std::is_base_of_v<SGFramework::BaseClass::BaseAsset, TAsset>,
				"FindAssetType template -> derived class only!!");
			
			//type
			AssetType type = FindAssetType<TAsset>();

			//既に登録があるか確認
			if (IS_TRUE(isRegisterSharedAsset))
			{
				SharedPointer<BaseClass::BaseAsset> result = Administrator::AssetManager::FindAsset(filePath, type);
				if (IS_TRUE(result.getIsValid()))
					return result.DynamicCast<TAsset>();
			}

			try
			{
				//make_shared
				auto result = SharedPointer<TAsset>(new TAsset());
				//name登録
				result->setName(filePath);
				//ポインタ登録
				result->m_thisPointer = result.DownCast<BaseClass::BaseAsset>();

				//監視リストに登録
				Administrator::AssetManager::RegisterAsset(result->m_thisPointer, type, isRegisterSharedAsset);
		
				//return
				return std::move(result);
			}
			catch (...)
			{
				throw;
			}
		}
	}
}
#endif //!SGFRAMEWORK_HEADER_INSTANTIATE_ASSET_HPP_