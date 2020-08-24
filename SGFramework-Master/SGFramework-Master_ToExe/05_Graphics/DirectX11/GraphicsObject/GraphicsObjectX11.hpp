#ifndef SGFRAMEWORK_HEADER_GRAPHICS_OBJECT_X11_HPP_
#define SGFRAMEWORK_HEADER_GRAPHICS_OBJECT_X11_HPP_
#include "../GraphicsX11/GraphicsX11.hpp"

// Framework namespace
namespace SGFramework
{
	//フレームワークの管理をするAdministrator namespace
	namespace Administrator
	{
		class GraphicsX11;
		
		//GraphicsDetail namespace
		namespace GraphicsDetail
		{
			//Graphics系クラスの基底となるGraphicsObjectX11 class
			class GraphicsObjectX11
			{
			public:
				//friend
				friend class SGFramework::Administrator::GraphicsX11;

			protected:
				//----------------------------------------------------------------------------------
				//[コンストラクタ]
				//リスト登録
				inline GraphicsObjectX11()
				{
					AtomicOperation::LockAtomic(SGFramework::Administrator::GraphicsX11::m_graphicsObjectsLock);
					SGFramework::Administrator::GraphicsX11::m_addOrEraseGraphicsObjects.emplace_back(this, true);
					AtomicOperation::UnlockAtomic(SGFramework::Administrator::GraphicsX11::m_graphicsObjectsLock);
				};
				//----------------------------------------------------------------------------------
				//[デストラクタ]
				//リスト取り消し
				inline virtual ~GraphicsObjectX11()
				{
					AtomicOperation::LockAtomic(SGFramework::Administrator::GraphicsX11::m_graphicsObjectsLock);
					SGFramework::Administrator::GraphicsX11::m_addOrEraseGraphicsObjects.emplace_back(this, false);
					AtomicOperation::UnlockAtomic(SGFramework::Administrator::GraphicsX11::m_graphicsObjectsLock);
				}

				//----------------------------------------------------------------------------------
				//[Sync]
				//情報を同期する, コールバック関数
				virtual void Sync() = 0;
			};
		}
	}
}
#endif //!SGFRAMEWORK_HEADER_GRAPHICS_OBJECT_X11_HPP_