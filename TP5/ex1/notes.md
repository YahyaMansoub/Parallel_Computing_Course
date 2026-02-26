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

3- 

mpirun -np 7 ./hello_rank0
Rank 3 elapsed: 0.000001 seconds
Rank 1 elapsed: 0.000000 seconds
Rank 6 elapsed: 0.000001 seconds
Rank 5 elapsed: 0.000001 seconds
Rank 4 elapsed: 0.000001 seconds
Rank 2 elapsed: 0.000001 seconds
Hello World (only rank 0). Total processes = 7
Rank 0 elapsed: 0.000122 seconds



4- 



mpirun -np 7 ./omit
Rank 3 elapsed: 0.000000 seconds
Rank 4 elapsed: 0.000000 seconds
Rank 5 elapsed: 0.000000 seconds
Rank 2 elapsed: 0.000000 seconds
Rank 6 elapsed: 0.000000 seconds
Hello World (only rank 0). Total processes = 7
Rank 0 elapsed: 0.000050 seconds
Rank 1 elapsed: 0.000000 seconds
--------------------------------------------------------------------------
prterun has exited due to process rank 3 with PID 617 on node LAPTOP-RT8UKAER exiting
improperly. There are three reasons this could occur:

1. this process did not call "init" before exiting, but others in the
job did. This can cause a job to hang indefinitely while it waits for
all processes to call "init". By rule, if one process calls "init",
then ALL processes must call "init" prior to termination.

2. this process called "init", but exited without calling "finalize".
By rule, all processes that call "init" MUST call "finalize" prior to
exiting or it will be considered an "abnormal termination"

3. this process called "MPI_Abort" or "prte_abort" and the mca
parameter prte_create_session_dirs is set to false. In this case, the
run-time cannot detect that the abort call was an abnormal
termination. Hence, the only error message you will receive is this
one.

This may have caused other processes in the application to be
terminated by signals sent by prterun (as reported here).

You can avoid this message by specifying -quiet on the prterun command
line.
--------------------------------------------------------------------------


When MPI_Finalize() is omitted, processes that initialized MPI terminate “improperly”. The MPI launcher detects that at least one rank exited without finalizing, reports an abnormal termination (here rank 3), and stops the job. Output may appear partial or out of order because processes are killed during shutdown and stdout buffering is not guaranteed.