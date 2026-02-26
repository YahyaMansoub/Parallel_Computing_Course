


Let's go through the first step  

we initialize the matrix and display it on process 0 

mpirun -np 2 ./step_12
Process 0 - Matrix a:
1 2 3 4 5
6 7 8 9 10
11 12 13 14 15
16 17 18 19 20 


now for the 3rd   part to initialize the matrix in process 1 

mpirun -np 2 ./step_3
Process 1 - Matrix at (allocated, before recv):
0 0 0 0
0 0 0 0
0 0 0 0
0 0 0 0
0 0 0 0
Process 0 - Matrix a (before send):
1 2 3 4 5
6 7 8 9 10
11 12 13 14 15
16 17 18 19 20


Now after creating the derived data type  


mpirun -np 2 ./step_4
Process 1 - Matrix at (before recv):
0 0 0 0
0 0 0 0
0 0 0 0
0 0 0 0
0 0 0 0
Process 1 - Derived datatype built + committed.
Process 0 - Matrix a (before send):
1 2 3 4 5
6 7 8 9 10
11 12 13 14 15
16 17 18 19 20



Final result 


mpicc -O2 ex1_transpose_dt.c -o ex1_transpose_dt
mpirun -np 2 ./ex1_transpose_dt
Process 0 - Matrix a:
1 2 3 4 5
6 7 8 9 10
11 12 13 14 15
16 17 18 19 20
Process 1 - Matrix transposee at:
1 6 11 16
2 7 12 17
3 8 13 18
4 9 14 19
5 10 15 20