XITAO_HOME=../..
include ${XITAO_HOME}/makefile.sched
#CXX = g++ 
CPPFLAGS+= -g -O3 -Wall -Wfatal-errors -fopenmp  

all:
	@make kernel
	@make fmm-xitao

kernel: kernel.cxx
	$(CXX) $? -o $@
	./kernel 10
	./kernel 20
	./kernel 30

fmm-xitao: fmm.cxx
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -I${XITAO_HOME}/include $? -o $@ -L${XITAO_HOME}/lib -lxitao -DUSE_XITAO

fmm-omp:$(CXX) $(CPPFLAGS) $? -o fmm-omp 

clean:
	$(RM) ./*.o ./kernel ./fmm ./fmm_omp ./fmm_xitao
