#!/bin/bash
export XITAO_STEALING_FREQ=100
module load intel 
CC=icc
CXX=icpc
rm fmm-xitao-idleness* fmm-xitao-rwss-idleness fmm-xitao-numa-idleness fmm-xitao-numa-auto-idleness fmm-xitao-numa-auto-sta-track-idleness

## RWSS ##
cd ../../
cp makefile.sched.rwss makefile.sched
CXX=${CXX} CC=${CC} CXXFLAGS=-DMEASURE_IDLENESS make lib
cd benchmarks/fmm2d/
make clean; CXX=${CXX} CC=${CC} make 
cp fmm-xitao fmm-xitao-rwss-idleness


## NUMA ##
cd ../../
cp makefile.sched.numa makefile.sched
CXX=${CXX} CC=${CC} CXXFLAGS=-DMEASURE_IDLENESS make lib
cd benchmarks/fmm2d/
make clean; CXX=${CXX} CC=${CC} CPPFLAGS=-DNUMA_AWARE LDFLAGS=-lnuma make 
cp fmm-xitao fmm-xitao-numa-idleness


## NUMA Auto ##
cd ../../
cp makefile.sched.numa.auto makefile.sched
CXX=${CXX} CC=${CC} CXXFLAGS=-DMEASURE_IDLENESS make lib
cd benchmarks/fmm2d/
make clean; CXX=${CXX} CC=${CC} make 
cp fmm-xitao fmm-xitao-numa-auto-idleness


## NUMA Auto Track STA ##
cd ../../
cp makefile.sched.numa.auto.sta.track makefile.sched
CXX=${CXX} CC=${CC} CXXFLAGS=-DMEASURE_IDLENESS make lib
cd benchmarks/fmm2d/
make clean; CXX=${CXX} CC=${CC} make 
cp fmm-xitao fmm-xitao-numa-auto-sta-track-idleness

