#!/bin/bash
export XITAO_STEALING_FREQ=2000
module load intel 
CC=icc
CXX=icpc
rm heat-tao heat-tao-rwss heat-tao-numa*

## RWSS ##
cd ../../
cp makefile.sched.rwss makefile.sched
CXX=${CXX} CC=${CC} make lib
cd benchmarks/heat/
make clean; CXX=${CXX} CC=${CC} make 
cp heat-tao heat-tao-rwss


## NUMA ##
cd ../../
cp makefile.sched.numa makefile.sched
CXX=${CXX} CC=${CC} make lib
cd benchmarks/heat/
make clean; CXX=${CXX} CC=${CC} make 
cp heat-tao heat-tao-numa


## NUMA Auto ##
cd ../../
cp makefile.sched.numa.auto makefile.sched
CXX=${CXX} CC=${CC} make lib
cd benchmarks/heat/
make clean; CXX=${CXX} CC=${CC} make 
cp heat-tao heat-tao-numa-auto


## NUMA Auto Track STA ##
cd ../../
cp makefile.sched.numa.auto.sta.track makefile.sched
CXX=${CXX} CC=${CC} make lib
cd benchmarks/heat/
make clean; CXX=${CXX} CC=${CC} make 
cp heat-tao heat-tao-numa-auto-sta-track

