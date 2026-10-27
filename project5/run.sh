#!/bin/bash
module load gcc/7.5
for t in 1024 4096 16384 65536 262144 1048576 2097152
do
        for b in 8 32 64 128 256
        do
                /usr/local/apps/cuda/11.2/bin/nvcc -DNUMTRIALS=$t -DBLOCKSIZE=$b -o proj05 proj05.cu
                ./proj05
        done
done