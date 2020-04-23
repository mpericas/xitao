#!/bin/bash
export XITAO_STEALING_FREQ=100
module load intel 
CC=icc
CXX=icpc
rm fmm-xitao fmm-xitao-rwss fmm-xitao-numa*

## RWSS ##
cd ../../
cp makefile.sched.rwss makefile.sched
CXX=${CXX} CC=${CC} make lib
cd benchmarks/fmm2d/
make clean; CXX=${CXX} CC=${CC} make 
cp fmm-xitao fmm-xitao-rwss


## NUMA ##
cd ../../
cp makefile.sched.numa makefile.sched
CXX=${CXX} CC=${CC} make lib
cd benchmarks/fmm2d/
make clean; CXX=${CXX} CC=${CC} make 
cp fmm-xitao fmm-xitao-numa


## NUMA Auto ##
cd ../../
cp makefile.sched.numa.auto makefile.sched
CXX=${CXX} CC=${CC} make lib
cd benchmarks/fmm2d/
make clean; CXX=${CXX} CC=${CC} make 
cp fmm-xitao fmm-xitao-numa-auto


## NUMA Auto Track STA ##
cd ../../
cp makefile.sched.numa.auto.sta.track makefile.sched
CXX=${CXX} CC=${CC} make lib
cd benchmarks/fmm2d/
make clean; CXX=${CXX} CC=${CC} make 
cp fmm-xitao fmm-xitao-numa-auto-sta-track

