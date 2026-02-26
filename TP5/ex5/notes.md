
mpicc -O3 -march=native -o pi_mpi pi_mpi.c
mpirun -np 4 ./pi_mpi 100000000
N=100000000  P=4
pi_parallel = 3.141592653589683
pi_serial   = 3.141592653590426
abs_error   = 7.438e-13
time_parallel = 0.031300 s
time_serial   = 0.124444 s
speedup = 3.9759
efficiency = 0.9940
CSV,4,100000000,0.124444,0.031300,3.975871,0.993968




then i did 

 for p in 1 2 4 8; do   mpirun -np $p ./pi_mpi 200000000 | grep '^CSV'; done > results.csv