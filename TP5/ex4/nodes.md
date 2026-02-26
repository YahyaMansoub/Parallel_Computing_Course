

- now for the code without mpi 
it was a bit hard to understand it but yah i get it now how the matrix and the vector are initialized. 



./baseline 1000
N=1000 serial_time=0.000858 s
x[0]=24.646387


- Now let's start thinking about how to implement it in MPI. 

A first idea that comes to mind is to make each process take care of one row , as rows are contiguous. 




