
TEMPLATE = lib
TARGET = EuclaseCUDA

DESTDIR = $$PWD/../_bin

libeuclasecuda.target = libeuclasecuda
libeuclasecuda.commands = cd $$PWD && make libeuclasecuda.o
QMAKE_EXTRA_TARGETS += libeuclasecuda
PRE_TARGETDEPS += libeuclasecuda

LIBS += $$PWD/libeuclasecuda.o
LIBS += -lcudart

HEADERS += libeuclasecuda.h

DISTFILES += \
	libeuclasecuda.cu

