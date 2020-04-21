#!/bin/bash
CC=gcc-8
CXX=g++-8
rm heat-tao heat-tao-rwss heat-tao-numa*

## RWSS ##
cd ../../
cp makefile.sched.rwss makefile.sched
CXX=${CXX} CC=${CC} make lib
cd benchmarks/heat/
make clean; make 
cp heat-tao heat-tao-rwss


## NUMA ##
cd ../../
cp makefile.sched.numa makefile.sched
CXX=${CXX} CC=${CC} make lib
cd benchmarks/heat/
make clean; make 
cp heat-tao heat-tao-numa


## NUMA Auto ##
cd ../../
cp makefile.sched.numa.auto makefile.sched
CXX=${CXX} CC=${CC} make lib
cd benchmarks/heat/
make clean; make 
cp heat-tao heat-tao-numa-auto


## NUMA Auto Track STA ##
cd ../../
cp makefile.sched.numa.auto.sta.track makefile.sched
CXX=${CXX} CC=${CC} make lib
cd benchmarks/heat/
make clean; make 
cp heat-tao heat-tao-numa-auto-sta-track

