#!/bin/bash
export XITAO_STEALING_FREQ=5000
CC=gcc
CXX=g++
rm numabench-*
#CXXFLAGS=-DMEASURE_IDLENESS

## RWSS ##
cd ../../
cp makefile.sched.rwss makefile.sched
CXXFLAGS=${CXXFLAGS} CXX=${CXX} CC=${CC} make lib
cd benchmarks/syntheticNUMADAGs/
make clean; CXX=${CXX} CC=${CC} make 
cp numabench numabench-rwss


## NUMA ##
cd ../../
cp makefile.sched.numa makefile.sched
CXXFLAGS=${CXXFLAGS} CXX=${CXX} CC=${CC} make lib
cd benchmarks/syntheticNUMADAGs/
make clean; CXX=${CXX} CC=${CC} make 
cp numabench numabench-numa


## NUMA Auto ##
cd ../../
cp makefile.sched.numa.auto makefile.sched
CXXFLAGS=${CXXFLAGS} CXX=${CXX} CC=${CC} make lib
cd benchmarks/syntheticNUMADAGs/
make clean; CXX=${CXX} CC=${CC} make 
cp numabench numabench-numa-auto


## NUMA Auto Track STA ##
cd ../../
cp makefile.sched.numa.auto.sta.track makefile.sched
CXXFLAGS=${CXXFLAGS} CXX=${CXX} CC=${CC} make lib
cd benchmarks/syntheticNUMADAGs/
make clean; CXX=${CXX} CC=${CC} make 
cp numabench numabench-numa-auto-sta-track

