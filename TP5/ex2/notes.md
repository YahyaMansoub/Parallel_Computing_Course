


So basically for this one we first take only the process of rank 0 
to take the input and  also if the input is incorrect we set x = -1 . 

if x is negative they break. 
and then  at the end we add a flush  because processes share the same terminal output. 

and of course the infamous finalize. 



mpirun -np 5 ./broadcast
4
Process 0 got 4
Process 2 got 4
Process 1 got 4
Process 3 got 4
Process 4 got 4
10
Process 1 got 10
Process 0 got 10
Process 3 got 10
Process 4 got 10
Process 2 got 10
-2


