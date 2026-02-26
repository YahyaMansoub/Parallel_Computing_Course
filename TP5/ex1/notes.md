1-  


First run for  4 processes 


mpirun -np 4 ./hello
Hello World
Elapsed (this process): 0.000036 seconds
Hello World
Elapsed (this process): 0.000012 seconds
Hello World
Elapsed (this process): 0.000027 seconds
Hello World
Elapsed (this process): 0.000079 seconds


2- 

mpirun -np 3 ./hello_rank
Hello World from rank 1 out of 3 processes
Rank 1 elapsed: 0.000055 seconds
Hello World from rank 2 out of 3 processes
Rank 2 elapsed: 0.000049 seconds
Hello World from rank 0 out of 3 processes
Rank 0 elapsed: 0.000062 seconds

