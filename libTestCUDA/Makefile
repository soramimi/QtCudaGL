
NVCC=nvcc
NVCCFLAGS=-I /opt/cuda/include
LDFLAGS=-L/opt/cuda/lib64 -I/opt/cuda/targets/x86_64-linux/include

all: libTestCUDA.so
	@-mkdir ../_bin 2>/dev/null
	cp libTestCUDA.so ../_bin/

libtestcuda.o: libtestcuda.cu
	$(NVCC) $(NVCCFLAGS) -c -O3 -g -shared -Xcompiler -fPIC $^

libTestCUDA.so: libtestcuda.cu
	$(NVCC) $(LDFLAGS) -O3 -g -shared -Xcompiler -fPIC -o $@ $^

clean:
	rm -fr *.o libTestCUDA.so
