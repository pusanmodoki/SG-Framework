/*----------------------------------------------------------------------------------
AMP�Ɏg�p����GPU�������I������GpuAccelerator
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_GPU_ACCELERATOR_HPP_
#define SGFRAMEWORK_HEADER_GPU_ACCELERATOR_HPP_
#include <memory>
#include <vector>
#include <amp.h>

//Framework namespace
namespace SGFramework
{
	//GPGPU Processing
	namespace GPGPU
	{
		//AMP�Ɏg�p����GPU�������I������GpuAccelerator 
		class GpuAccelerator final
		{
		public:
			//----------------------------------------------------------------------------------
			//[�R���X�g���N�^]
			//���������ő��GPU�������I�����ăA�N�Z�����[�^�𐶐�����
			inline GpuAccelerator();

			//----------------------------------------------------------------------------------
			//[getAccelerator]
			//return: �������ʂ��ő�̃A�N�Z�����[�^
			inline concurrency::accelerator_view getAccelerator() { return m_uniqueAcceleratorView; };

			//() operator
			inline concurrency::accelerator_view operator ()() { return m_uniqueAcceleratorView; }

		private:
			//�A�N�Z�����[�^
			static concurrency::accelerator m_uniqueAccelerator;
			//�A�N�Z�����[�^View
			static concurrency::accelerator_view m_uniqueAcceleratorView;
		};


		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//���������ő��GPU�������I�����ăA�N�Z�����[�^�𐶐�����
		inline GpuAccelerator::GpuAccelerator()
		{
			//�A�N�Z�����[�^���X�g
			std::vector<concurrency::accelerator> accelerators = concurrency::accelerator::get_all();

			//�G�~�����[�^�p�A�N�Z�����[�^���폜
			accelerators.erase(std::remove_if(accelerators.begin(), accelerators.end(), [](concurrency::accelerator& accelerator) {return accelerator.get_is_emulated(); }), accelerators.end());

			//�������ʂ��ő�̃A�N�Z�����[�^���擾
			auto useIt = std::max_element(accelerators.begin(), accelerators.end(),
				[](const concurrency::accelerator& accelerator1, const concurrency::accelerator& accelerator2) {return accelerator1.get_dedicated_memory() < accelerator2.get_dedicated_memory(); });

			//�A�N�Z�����[�^�ݒ�
			m_uniqueAccelerator = *useIt;
			//�A�N�Z�����[�^View�ݒ�
			m_uniqueAcceleratorView = (*useIt).create_view();
		};
	}
}

#endif // !SGFRAMEWORK_HEADER_GPU_ACCELERATOR_HPP_