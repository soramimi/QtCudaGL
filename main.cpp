
#include "main.h"
#include "MainWindow.h"
#include "joinpath.h"
#include <QApplication>
#include <QMessageBox>
#include <dlfcn.h>

ApplicationGlobal *global;

int main(int argc, char **argv)
{
	ApplicationGlobal g;
	global = &g;

	QApplication a(argc, argv);

	{
		QString path = a.applicationDirPath() / "libTestCUDA.so";
		void *so = dlopen(path.toStdString().c_str(), RTLD_NOW);
		CUDAIMAGE_API const *(*init_cudaimage)(int);
		*(void **)&init_cudaimage = dlsym(so, "init_cudaimage");
		global->cuda = init_cudaimage(sizeof(CUDAIMAGE_API));
	}
	if (!global->cuda) {
		QMessageBox::critical(nullptr, "QtCudaGL", "Could not load the shared library: libTestCUDA.so");
		return 1;
	}

	MainWindow w;
	w.show();
	return a.exec();
}
