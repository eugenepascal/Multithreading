#include <cstdlib>
#include <cstdint>

#include <iostream>

#include <amp.h>

int64_t GetTimeMs64 ()
{
	/* Windows */
	FILETIME ft;
	LARGE_INTEGER li;

	/* Get the amount of 100 nano seconds intervals elapsed since January 1, 1601 (UTC) and copy it
	* to a LARGE_INTEGER structure. */
	GetSystemTimeAsFileTime (&ft);
	li.LowPart = ft.dwLowDateTime;
	li.HighPart = ft.dwHighDateTime;

	uint64_t ret = li.QuadPart;
	ret -= 116444736000000000LL; /* Convert from file time to UNIX epoch time. */
	ret /= 10000; /* From 100 nano seconds (10^-7) to 1 millisecond (10^-3) intervals */

	return ret;
}

concurrency::accelerator pick_accelerator ()
{
	std::vector<concurrency::accelerator> accelerators = concurrency::accelerator::get_all ();
	concurrency::accelerator ret;

	/*
	std::wcout << "Default accelerator: " << concurrency::accelerator::default_accelerator << std::endl;
	std::wcout << "CPU accelerator: " << concurrency::accelerator::cpu_accelerator << std::endl;
	std::wcout << "Direct3D Ref accelerator: " << concurrency::accelerator::direct3d_ref << std::endl;
	std::wcout << "Direct3D Warp accelerator: " << concurrency::accelerator::direct3d_warp << std::endl;

	std::for_each(accelerators.begin(), accelerators.end(), [](concurrency::accelerator acc)
	{
		std::wcout << acc.description << " " << acc.device_path << " " << acc.is_emulated << " " << acc.version << " " << std::endl;
	});
	*/

	auto result = std::find_if (accelerators.begin (), accelerators.end (), [](concurrency::accelerator acc)
		{
			return acc.device_path == concurrency::accelerator::direct3d_warp;
		});

	if(result != accelerators.end ())
	{
		//std::cout << "Found" << std::endl;
		ret = *(result);
	}

	//concurrency::accelerator::set_default(ret.device_path);
	return ret;
}

void matadd (std::vector<int>& vA, std::vector<int>& vB, std::vector<int>& vC, int M, int N)
{
	for(int i = 0; i < M; i++)
	{
		for(int j = 0; j < N; j++)
		{
			vC[i * N + j] = vA[i * N + j] + vB[i * N + j];
		}
	}
}

void matadd2 (std::vector<int>& vA, std::vector<int>& vB, std::vector<int>& vC, int M, int N)
{
	concurrency::extent<2> e (M, N);
	concurrency::array_view<int, 2> a (e, vA);
	concurrency::array_view<int, 2> b (e, vB);
	concurrency::array_view<int, 2> c (e, vC);

	for(int i = 0; i < M; i++)
	{
		for(int j = 0; j < N; j++)
		{
			c (i, j) = a (i, j) + b (i, j);
		}
	}
}

void matadd3 (std::vector<int>& vA, std::vector<int>& vB, std::vector<int>& vC, int M, int N)
{
	concurrency::extent<2> e (M, N);
	concurrency::array_view<int, 2> a (e, vA);
	concurrency::array_view<int, 2> b (e, vB);
	concurrency::array_view<int, 2> c (e, vC);

	concurrency::index<2> idx (0, 0);

	for(idx[0] = 0; idx[0] < e[0]; idx[0]++)
	{
		for(idx[1] = 0; idx[1] < e[1]; idx[1]++)
		{
			c (idx[0], idx[1]) = a (idx[0], idx[1]) + b (idx[0], idx[1]);
		}
	}
}

void matadd_amp (std::vector<int>& vA, std::vector<int>& vB, std::vector<int>& vC, int M, int N)
{
	concurrency::extent<2> e (M, N);
	concurrency::array_view<const int, 2> a (e, vA); // no need to copy back data so constify
	concurrency::array_view<const int, 2> b (e, vB); // no need to copy back data so constify
	concurrency::array_view<int, 2> c (e, vC);

	c.discard_data (); // no need to copy content to accelerator

	concurrency::index<2> idx (0, 0);

	concurrency::parallel_for_each (pick_accelerator ().default_view, e, [=](concurrency::index<2> idx) restrict (amp)
		{
			c[idx] = a[idx] + b[idx];
		});

	c.synchronize ();
}

void matmult (std::vector<int>& vC, const std::vector<int>& vA, const std::vector<int>& vB, int M, int N, int W)
{
	for(int row = 0; row < M; row++)
	{
		for(int col = 0; col < N; col++)
		{
			int sum = 0;
			for(int i = 0; i < W; i++)
			{
				sum += vA[row * W + i] * vB[i * N + col];
			}
			vC[row * N + col] = sum;
		}
	}
}

void matmult_amp (std::vector<int>& vC, const std::vector<int>& vA, const std::vector<int>& vB, int M, int N, int W)
{
	concurrency::array_view<const int, 2> a (M, W, vA);
	concurrency::array_view<const int, 2> b (M, W, vB);
	concurrency::array_view<int, 2> c (M, W, vC);

	c.discard_data ();

	concurrency::parallel_for_each (c.extent, [=](concurrency::index<2> idx) restrict (amp)
		{
			int row = idx[0];
			int col = idx[1];
			int sum = 0;

			for(int i = 0; i < b.extent[0]; i++)
			{
				sum += a (row, i) * b (i, col);
			}
			c[idx] = sum;
		});

	c.synchronize ();
}

#define TS 16

void matmult_amp_tiled (std::vector<int>& vC, const std::vector<int>& vA, const std::vector<int>& vB, int M, int N, int W)
{
	concurrency::array_view<const int, 2> a (M, W, vA);
	concurrency::array_view<const int, 2> b (M, W, vB);
	concurrency::array_view<int, 2> c (M, W, vC);

	c.discard_data ();

	concurrency::parallel_for_each (c.extent.tile<TS, TS> (), [=](concurrency::tiled_index<TS, TS> tidx) restrict (amp)
		{
			//int row = tidx.global[0];
			//int col = tidx.global[1];
			int row = tidx.local[0];
			int col = tidx.local[1];
			int sum = 0;
			tile_static int locA[TS][TS];
			tile_static int locB[TS][TS];

			for(int i = 0; i < a.extent[1]; i += TS)
			{
				// copy data to local memory
				locA[row][col] = a (tidx.global[0], col + i);
				locB[row][col] = b (row + i, tidx.global[1]);

				tidx.barrier.wait ();

				for(int k = 0; k < TS; k++)
				{
					sum += locA[row][k] * locB[k][col];
				}
				tidx.barrier.wait ();
			}
			c[tidx.global] = sum;

			/*
			int row = t_idx.global[0]; int col = t_idx.global[1];
			int sum = 0;

			for(int i = 0 ; i < b.extent[0] ; i++)
			{
				sum += a(row, i) * b(i, col);
			}
			c[tidx.global] = sum;
			*/
		});

	c.synchronize ();
}

void do_it ()
{
	// rows and columns for matrix
	const int M = 1024;
	const int N = 1024;
	int64_t start = 0;
	int64_t end = 0;

	// create storage for a matrix of above size
	std::vector<int> vA (M * N);
	std::vector<int> vB (M * N);
	// output storage for matrix calculation
	std::vector<int> vC (M * N);

	// populate matrix objects
	int i = 0;
	std::generate (vA.begin (), vA.end (), [&i]()
		{
			return i++;
		});
	std::generate (vB.begin (), vB.end (), [&i]()
		{
			return i--;
		});

	std::cout << "Matmult" << std::endl;
	start = GetTimeMs64 ();
	matmult (vA, vB, vC, M, N, M);
	end = GetTimeMs64 ();
	std::cout << "Execution time : " << (end - start) << " ms" << std::endl;

	std::cout << "Matmult AMP" << std::endl;
	start = GetTimeMs64 ();
	matmult_amp (vA, vB, vC, M, N, M);
	end = GetTimeMs64 ();
	std::cout << "Execution time : " << (end - start) << " ms" << std::endl;

	/*
	std::cout << "Matmult AMP tiled" << std::endl;
	start = GetTimeMs64 ();
	matmult_amp_tiled (vA, vB, vC, M, N, M);
	end = GetTimeMs64 ();
	std::cout << "Execution time : " << (end - start) << " ms" << std::endl;
	*/

	std::cout << "Mataddition" << std::endl;
	start = GetTimeMs64 ();
	matadd (vA, vB, vC, M, N);
	end = GetTimeMs64 ();
	std::cout << "Execution time : " << (end - start) << " ms" << std::endl;

	std::cout << "Mataddition2" << std::endl;
	start = GetTimeMs64 ();
	matadd2 (vA, vB, vC, M, N);
	end = GetTimeMs64 ();
	std::cout << "Execution time : " << (end - start) << " ms" << std::endl;

	std::cout << "Mataddition3" << std::endl;
	start = GetTimeMs64 ();
	matadd3 (vA, vB, vC, M, N);
	end = GetTimeMs64 ();
	std::cout << "Execution time : " << (end - start) << " ms" << std::endl;

	std::cout << "Mataddition AMP" << std::endl;
	start = GetTimeMs64 ();
	matadd_amp (vA, vB, vC, M, N);
	end = GetTimeMs64 ();
	std::cout << "Execution time : " << (end - start) << " ms" << std::endl;
}

int main (int argc, char** argv)
{
	do_it ();
	std::cout << "Hit any key to exit..." << std::endl;
	std::cin.get ();

	return EXIT_SUCCESS;
}
