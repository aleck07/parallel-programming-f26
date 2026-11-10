NVCC = /usr/local/apps/cuda/11.2/bin/nvcc
NVCCFLAGS = -O3 -Xcompiler -fopenmp
LDLIBS = -lgomp

# `module` is a shell function set up by the login profile, so run recipes in a bash login shell
SHELL := /bin/bash
.SHELLFLAGS := -lc

all: countcoins countcoins_gpu

# CPU (OpenMP) build -- uses whatever #defines are set in the source
countcoins: countcoins.cu
	module load gcc/7.5 && $(NVCC) $(NVCCFLAGS) countcoins.cu -o countcoins $(LDLIBS)

# GPU build -- same source with GPU defined on the command line
countcoins_gpu: countcoins.cu
	module load gcc/7.5 && $(NVCC) $(NVCCFLAGS) -DGPU countcoins.cu -o countcoins_gpu $(LDLIBS)

# AMD build (HIP) for an RX 7800 XT (gfx1101) -- run on your own machine, not the cluster
# Requires: rocm-hip-devel (HIP headers) and an LLVM OpenMP omp.h for ROCm's clang
HIPCC = hipcc
HIPFLAGS = -O3 -fopenmp --offload-arch=gfx1101

countcoins_hip: countcoins_hip.cpp
	$(HIPCC) $(HIPFLAGS) countcoins_hip.cpp -o countcoins_hip

clean:
	rm -f countcoins countcoins_gpu countcoins_hip bounding-box.pgm

.PHONY: all clean
