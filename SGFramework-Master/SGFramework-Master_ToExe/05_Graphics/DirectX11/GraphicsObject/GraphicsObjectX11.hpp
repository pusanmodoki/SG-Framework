#ifndef SGFRAMEWORK_HEADER_GRAPHICS_OBJECT_X11_HPP_
#define SGFRAMEWORK_HEADER_GRAPHICS_OBJECT_X11_HPP_
#include "../GraphicsX11/GraphicsX11.hpp"

// Framework namespace
namespace SGFramework
{
	//�t���[�����[�N�̊Ǘ�������Administrator namespace
	namespace Administrator
	{
		class GraphicsX11;
		
		//GraphicsDetail namespace
		namespace GraphicsDetail
		{
			//Graphics�n�N���X�̊��ƂȂ�GraphicsObjectX11 class
			class GraphicsObjectX11
			{
			public:
				//friend
				friend class SGFramework::Administrator::GraphicsX11;

			protected:
				//----------------------------------------------------------------------------------
				//[�R���X�g���N�^]
				//���X�g�o�^
				inline GraphicsObjectX11()
				{
					AtomicOperation::LockAtomic(SGFramework::Administrator::GraphicsX11::m_graphicsObjectsLock);
					SGFramework::Administrator::GraphicsX11::m_addOrEraseGraphicsObjects.emplace_back(this, true);
					AtomicOperation::UnlockAtomic(SGFramework::Administrator::GraphicsX11::m_graphicsObjectsLock);
				};
				//----------------------------------------------------------------------------------
				//[�f�X�g���N�^]
				//���X�g������
				inline virtual ~GraphicsObjectX11()
				{
					AtomicOperation::LockAtomic(SGFramework::Administrator::GraphicsX11::m_graphicsObjectsLock);
					SGFramework::Administrator::GraphicsX11::m_addOrEraseGraphicsObjects.emplace_back(this, false);
					AtomicOperation::UnlockAtomic(SGFramework::Administrator::GraphicsX11::m_graphicsObjectsLock);
				}

				//----------------------------------------------------------------------------------
				//[Sync]
				//���𓯊�����, �R�[���o�b�N�֐�
				virtual void Sync() = 0;
			};
		}
	}
}
#endif //!SGFRAMEWORK_HEADER_GRAPHICS_OBJECT_X11_HPP_