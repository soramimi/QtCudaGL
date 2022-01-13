#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include "main.h"
#include <QGLFunctions>
#include <QGLWidget>

struct cudaGraphicsResource;

class ImageView : public QGLWidget, public QGLFunctions {
	Q_OBJECT
private:
	int width_ = 1024;
	int height_ = 1024;
	GLuint pbo_;
	cudaGraphicsResource *dev_resource_ = nullptr;
	int tick_ = 0;
public:
	explicit ImageView(QWidget *parent = nullptr);
	~ImageView();
protected:
	void initializeGL();
	void paintGL();
	void timerEvent(QTimerEvent *event);

	// QGLWidget interface
protected:
	void resizeGL(int w, int h);
};

#endif // IMAGEVIEW_H
