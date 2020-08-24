/*----------------------------------------------------------------------------------
AMPに使用するGPUを自動選択するGpuAccelerator
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
		//AMPに使用するGPUを自動選択するGpuAccelerator 
		class GpuAccelerator final
		{
		public:
			//----------------------------------------------------------------------------------
			//[コンストラクタ]
			//メモリが最大のGPUを自動選択してアクセラレータを生成する
			inline GpuAccelerator();

			//----------------------------------------------------------------------------------
			//[getAccelerator]
			//return: メモリ量が最大のアクセラレータ
			inline concurrency::accelerator_view getAccelerator() { return m_uniqueAcceleratorView; };

			//() operator
			inline concurrency::accelerator_view operator ()() { return m_uniqueAcceleratorView; }

		private:
			//アクセラレータ
			static concurrency::accelerator m_uniqueAccelerator;
			//アクセラレータView
			static concurrency::accelerator_view m_uniqueAcceleratorView;
		};


		//----------------------------------------------------------------------------------
		//[コンストラクタ]
		//メモリが最大のGPUを自動選択してアクセラレータを生成する
		inline GpuAccelerator::GpuAccelerator()
		{
			//アクセラレータリスト
			std::vector<concurrency::accelerator> accelerators = concurrency::accelerator::get_all();

			//エミュレータ用アクセラレータを削除
			accelerators.erase(std::remove_if(accelerators.begin(), accelerators.end(), [](concurrency::accelerator& accelerator) {return accelerator.get_is_emulated(); }), accelerators.end());

			//メモリ量が最大のアクセラレータを取得
			auto useIt = std::max_element(accelerators.begin(), accelerators.end(),
				[](const concurrency::accelerator& accelerator1, const concurrency::accelerator& accelerator2) {return accelerator1.get_dedicated_memory() < accelerator2.get_dedicated_memory(); });

			//アクセラレータ設定
			m_uniqueAccelerator = *useIt;
			//アクセラレータView設定
			m_uniqueAcceleratorView = (*useIt).create_view();
		};
	}
}

#endif // !SGFRAMEWORK_HEADER_GPU_ACCELERATOR_HPP_