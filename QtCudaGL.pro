TARGET = QtCudaGL
TEMPLATE = app
QT += core gui widgets opengl
CONFIG += c++11
CONFIG -= app_bundle

DESTDIR = $$PWD/_bin

INCLUDEPATH += -I /opt/cuda/targets/x86_64-linux/include
LIBS += -L /opt/cuda/targets/x86_64-linux/lib
LIBS += -lglut -lcudart
LIBS += -ldl

SOURCES += \
        ImageView.cpp \
        MainWindow.cpp \
        joinpath.cpp \
        main.cpp

FORMS += \
	MainWindow.ui

HEADERS += \
	ImageView.h \
	MainWindow.h \
	joinpath.h \
	libTestCUDA/libtestcuda.h \
	main.h

DISTFILES += \
	libTestCUDA/libtestcuda.cu
