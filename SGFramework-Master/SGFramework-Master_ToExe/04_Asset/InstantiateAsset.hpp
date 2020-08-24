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
	//AssetŠÖ˜A‚ğŠi”[‚µ‚½Asset namespace
	namespace Asset
	{
		//----------------------------------------------------------------------------------
		//[InstantiateAsset]
		//ƒAƒZƒbƒgƒNƒ‰ƒX‚ğ¶¬‚·‚é, (Open‚Í•Ê“rŠÖ”‚ğÀs)
		//template: asset class
		//return: make_shared pointer
		//ˆø”1: file path
		//ˆø”2: asset is register shared? 
		//			(Šù‚ÉSharedİ’è‚Ì“¯–¼“¯Œ^ƒ|ƒCƒ“ƒ^‚ª‚ ‚Á‚½ê‡‚»‚ê‚ğ•Ô‹p‚·‚é), default = true
		template <typename TAsset>
		inline SharedPointer<TAsset> InstantiateAsset(const sgstring& filePath, bool isRegisterSharedAsset = true)
		{
			static_assert(std::is_base_of_v<SGFramework::BaseClass::BaseAsset, TAsset>,
				"FindAssetType template -> derived class only!!");
			
			//type
			AssetType type = FindAssetType<TAsset>();

			//Šù‚É“o˜^‚ª‚ ‚é‚©Šm”F
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
				//name“o˜^
				result->setName(filePath);
				//ƒ|ƒCƒ“ƒ^“o˜^
				result->m_thisPointer = result.DownCast<BaseClass::BaseAsset>();

				//ŠÄ‹ƒŠƒXƒg‚É“o˜^
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