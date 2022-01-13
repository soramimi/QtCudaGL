#include "libtestcuda.h"
#include <stdio.h>
#include <vector_types.h>

__global__ void cu_hoge(int tick, uchar4 *bitmap)
{
	int x = threadIdx.x + blockIdx.x * blockDim.x;
	int y = threadIdx.y + blockIdx.y * blockDim.y;
	int offset = x + y * blockDim.x * gridDim.x;

	// 連続的になるように...
	float theta = tick / 60.0f * 2.0f * M_PI;
	float theta_x = x / 60.0f * 2.0f * M_PI;
	float theta_y = y / 60.0f * 2.0f * M_PI;
	float r = fabs(sin(theta + theta_x));
	float g = fabs(cos(theta + theta_y));
	float b = fabs(sin(theta + theta_x) * cos(theta + theta_y));

	bitmap[offset].x = (unsigned char)(r * 255);
	bitmap[offset].y = (unsigned char)(g * 255);
	bitmap[offset].z = (unsigned char)(b * 255);
	bitmap[offset].w = 255;
}

static void hoge(int w, int h, int tick, cudamem_t bitmap)
{
	dim3 threads(8, 8); // 64スレッド/1グリッド
	dim3 grids(w / 8, h / 8); // 各ピクセルに1スレッドが割り振られる
	cu_hoge<<<grids, threads>>>(tick, (uchar4 *)bitmap);
}

cudamem_t cuda_malloc(int len)
{
	cudamem_t mem = nullptr;
	cudaMalloc((void **)&mem, len);
	return mem;
}

void cuda_free(cudamem_t mem)
{
	cudaFree(mem);
}

void cuda_memcpy_htoh(void *dst_h, void *src_h, int len)
{
	cudaMemcpy(dst_h, src_h, len, cudaMemcpyHostToHost);
}

void cuda_memcpy_dtoh(void *dst_h, void *src_d, int len)
{
	cudaMemcpy(dst_h, src_d, len, cudaMemcpyDeviceToHost);
}

void cuda_memcpy_htod(void *dst_d, void *src_h, int len)
{
	cudaMemcpy(dst_d, src_h, len, cudaMemcpyHostToDevice);
}

void cuda_memcpy_dtod(void *dst_d, void *src_d, int len)
{
	cudaMemcpy(dst_d, src_d, len, cudaMemcpyDeviceToDevice);
}

__global__ void cu_init_cudaimage(uint8_t *p)
{
	int i = blockIdx.x;
	int j = threadIdx.x;
	p[i * 9 + j] = (i + 1) * (j + 1);
}

CUDAIMAGE_API api;

extern "C" CUDAIMAGE_API const *init_cudaimage(int n)
{
	if (n != sizeof(CUDAIMAGE_API)) return nullptr;

	uint8_t table[81];
	uint8_t *mem;
	cudaMalloc((void **)&mem, 81);
	dim3 b(9);
	dim3 t(9);
	cu_init_cudaimage<<<b,t>>>(mem);
	cudaMemcpy(table, mem, 81, cudaMemcpyDeviceToHost);
	cudaFree(mem);

	// test
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (table[9 * i + j] != (i + 1) * (j + 1)) {
				return nullptr;
			}
		}
	}

	api.malloc = cuda_malloc;
	api.free = cuda_free;
	api.memcpy_htoh = cuda_memcpy_htoh;
	api.memcpy_dtoh = cuda_memcpy_dtoh;
	api.memcpy_htod = cuda_memcpy_htod;
	api.memcpy_dtod = cuda_memcpy_dtod;
	api.hoge = hoge;
	return &api;
}
