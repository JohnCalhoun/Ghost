#!/bin/bash
#
#SBATCH --job-name=Julia-john
#SBATCH --output=output.txt
#
#SBATCH --ntasks=1

sbcast test.txt /tmp

srun cd /tmp
srun mkdir juliatmp

srun touch test.txt
srun scp test.txt john@head:~/p*/g*


srun cd ./juliatmp
srun cd ..
srun rm -r ./juliatmp

