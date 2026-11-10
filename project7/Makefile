NVCC = /usr/local/apps/cuda/11.2/bin/nvcc
NVCCFLAGS = -O3 -Xcompiler -fopenmp
LDLIBS = -lgomp

# `module` is a shell function set up by the login profile, so run recipes in a bash login shell
SHELL := /bin/bash
.SHELLFLAGS := -lc

all: countcoins

countcoins: countcoins.cu
	module load gcc/7.5 && $(NVCC) $(NVCCFLAGS) countcoins.cu -o countcoins $(LDLIBS)

clean:
	rm -f countcoins bounding-box.pgm

.PHONY: all clean
