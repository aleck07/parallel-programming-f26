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

clean:
	rm -f countcoins countcoins_gpu bounding-box.pgm

.PHONY: all clean
