#include <iostream>

#include <amp.h>

concurrency::accelerator pick_accelerator()
{
	std::vector<concurrency::accelerator> accelerators = concurrency::accelerator::get_all ();
	concurrency::accelerator ret;
	
	/*
	std::wcout << "Default accelerator: " << concurrency::accelerator::default_accelerator << std::endl;
	std::wcout << "CPU accelerator: " << concurrency::accelerator::cpu_accelerator << std::endl;
	std::wcout << "Direct3D Ref accelerator: " << concurrency::accelerator::direct3d_ref << std::endl;
	std::wcout << "Direct3D Warp accelerator: " << concurrency::accelerator::direct3d_warp << std::endl;
	*/

	std::for_each(accelerators.begin(), accelerators.end(), [](concurrency::accelerator acc)
	{
		std::wcout << acc.description << " " << acc.device_path << " " << acc.is_emulated << " " << acc.version << " " << std::endl;
	});

	auto result = std::find_if (accelerators.begin (), accelerators.end (), [](concurrency::accelerator acc)
		{
			return acc.device_path == concurrency::accelerator::direct3d_warp;
		});

	if(result != accelerators.end ())
	{
		ret = *(result);
	}

	//concurrency::accelerator::set_default(ret.device_path);
	return ret;
}

int main (int argc, char** argv)
{
    concurrency::accelerator acc = pick_accelerator();
	int v[11] = { 'G', 'd', 'k', 'k', 'n', 31, 'v', 'n', 'q', 'k', 'c' };

	std::wcout << "Use " << acc.description << std::endl;
	
	concurrency::array_view<int> av (11, v);

	concurrency::parallel_for_each (av.extent, [=](concurrency::index<1> idx) restrict (amp)
		{
			av[idx] += 1;
		});

	for(unsigned int i = 0; i < 11; i++)
	{
		std::cout << static_cast<char>(av[i]);
	}
	std::cout << std::endl;
}
