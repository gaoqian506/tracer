


CUDA_ROOT = /usr/local/cuda-8.0
OPTIX_ROOT = /home/gq/Documents/tools/nvidia/optix/NVIDIA-OptiX-SDK-5.0.1-linux64

INCLUDE_DIR = -Iinclude 
INCLUDE_DIR += -I$(OPTIX_ROOT)/SDK/sutil
INCLUDE_DIR += -I$(OPTIX_ROOT)/include
INCLUDE_DIR += -I$(CUDA_ROOT)/include

LIBS = -L$(OPTIX_ROOT)/lib64
LIBS += -loptix -optix_prime

LIBS += -L$(OPTIX_ROOT)/SDK-precompiled-samples
LIBS += -lsutil_sdk
LIBS += -lglut -lGL

	
RPATH = -Wl,-rpath=$(OPTIX_ROOT)/lib64
RPATH += -Wl,-rpath=$(OPTIX_ROOT)/SDK-precompiled-samples

SRCS = $(wildcard  src/tracer/*.cpp)
OBJS = $(SRCS:%.cpp=%.o)

MAINS = $(wildcard  src/*.cpp)
TARGETS = $(MAINS:%.cpp=%)

CUS = $(wildcard  ptx/*.cu)
PTXS = $(CUS:%.cu=%.ptx)



all : $(OBJS) $(TARGETS) $(PTXS)

$(TARGETS) : % : %.cpp
	g++ -g $< $(OBJS) $(INCLUDE_DIR) $(LIBS) $(RPATH) -o $@
	
%.o : %.cpp
	g++ -c -g $< $(INCLUDE_DIR) $(LIBS) $(RPATH) -o $@
	
%.ptx : %.cu
	$(CUDA_ROOT)/bin/nvcc -ptx $(INCLUDE_DIR) $< -Wno-deprecated-gpu-targets  -o $@ 
	
clean:
	rm -f $(OBJS) $(PTXS) $(TARGETS)
	
	

#-Wno-deprecated-gpu-targets 	
#	 src/test.cpp
#all : trace
#
#trace : trace.o
#	g++ -g $< -o $@
#trace.o : trace.cpp
#	g++ -c -g $< -o $@
#
#	
#clean:
#	rm -f trace.o trace

#	@echo --------OBJS--------
#	@echo $(OBJS)
#	@echo $(TARGETS)