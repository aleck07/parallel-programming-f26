NVCC = /usr/local/apps/cuda/11.2/bin/nvcc

# `module` is a shell function set up by the login profile, so run recipes in a bash login shell
SHELL := /bin/bash
.SHELLFLAGS := -lc

proj05:     	proj05.cu
	module load gcc/7.5 && $(NVCC)     proj05.cu   -o proj05
