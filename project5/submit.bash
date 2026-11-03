#!/bin/bash
#SBATCH -J proj05test
#SBATCH -A cs475-575
#SBATCH -p classgpufinal
#SBATCH --constraint=v100
#SBATCH --gres=gpu:1
#SBATCH -o proj05DGX.out
#SBATCH -e proj05DGX.err
#SBATCH --mail-type=BEGIN,END,FAIL
#SBATCH --mail-user=krseka@oregonstate.edu

run.sh