include ../../makefile.sched

CXXFLAGS += -I../../include 
CXXFLAGS += -O3
CXXFLAGS += --std=c++11  ${EXTRAEINC}

CXXFLAGS += -DDOUBLE
#CXXFLAGS += -DSINGLE

LIBS = -lm ${EXTRAELIBS} -L../../lib -lxitao -pthread
SYNTH_OBJS=numabench.o  

all: numabench

%.o : %.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

numabench: $(SYNTH_OBJS)
	$(CXX) $^ -o $@ ${LIBS}

chains: $(CHAINS_OBJS)
	$(CXX) $^ -o $@ ${LIBS}

run: all 
	./numabench 64 4 5000 1000 4

clean:
	rm -rf numabench $(SYNTH_OBJS)  graph.txt 


