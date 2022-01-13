
#ifndef CUDAIMAGE_H
#define CUDAIMAGE_H

#include <cstdint>

typedef uint8_t *cudamem_t;

struct CUDAIMAGE_API {
	cudamem_t (*malloc)(int len);
	void (*free)(cudamem_t mem);
	void (*memcpy_htoh)(void *dst_h, void *src_h, int len);
	void (*memcpy_dtoh)(void *dst_h, void *src_d, int len);
	void (*memcpy_htod)(void *dst_d, void *src_h, int len);
	void (*memcpy_dtod)(void *dst_d, void *src_d, int len);
	void (*hoge)(int w, int h, int tick, cudamem_t mem);
};

#endif
