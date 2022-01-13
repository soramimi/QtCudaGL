
#include "ImageView.h"
#include <cuda_gl_interop.h>
#include <cuda_runtime.h>

ImageView::ImageView(QWidget *parent)
	: QGLWidget(parent)
{
	startTimer(16);
}

ImageView::~ImageView()
{
//	cudaGLUnregisterBufferObject(pbo_); // deprecated
	cudaGraphicsUnregisterResource(dev_resource_);
//	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
//	glDeleteBuffers(1, &pbo);
}

void ImageView::initializeGL()
{
	initializeGLFunctions();

	// バッファを作成
	glGenBuffers(1, &pbo_);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo_);

	// OpenGLのバッファをCudaと共有する設定
	cudaGraphicsGLRegisterBuffer(&dev_resource_, pbo_, cudaGraphicsMapFlagsNone);
}

void ImageView::resizeGL(int w, int h)
{
	width_ = w;
	height_ = h;

	// 8の倍数にする
	width_ += 7;
	height_ += 7;
	width_ -= width_ & 7;
	height_ -= height_ & 7;

	glBufferData(GL_PIXEL_UNPACK_BUFFER, sizeof(uchar4) * width_ * height_, nullptr, GL_DYNAMIC_DRAW);
	cudaGraphicsGLRegisterBuffer(&dev_resource_, pbo_, cudaGraphicsMapFlagsNone);
}

void ImageView::paintGL()
{
	uchar4 *dev_bitmap;
	size_t size;

	// フレームバッファをマップしてアドレスを取得
	cudaGraphicsMapResources(1, &dev_resource_, nullptr);
	cudaGraphicsResourceGetMappedPointer((void **)&dev_bitmap, &size, dev_resource_);

	// カーネル実行
	global->cuda->hoge(width_, height_, tick_, (cudamem_t)dev_bitmap);

	// カーネル関数の終了を待つ
	cudaDeviceSynchronize();
	// リソースの開放
	cudaGraphicsUnmapResources(1, &dev_resource_, nullptr);

	glDrawPixels(width_, height_, GL_RGBA, GL_UNSIGNED_BYTE, 0);
}

void ImageView::timerEvent(QTimerEvent *event)
{
	tick_ = (tick_ + 1) % 60;
	update();
}

