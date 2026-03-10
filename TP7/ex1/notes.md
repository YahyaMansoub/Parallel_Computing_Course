


mpicc -O2 -Wall -Wextra -std=c11 game_of_life_mpi.c -o gol



mpirun -np 1 ./gol 8 8 5 block 1
Process grid: 1 x 1
Global alive cells after 5 generations: 4

Rank 0 - Generation 5:
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 1 1 0 0 0
0 0 0 1 1 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0




blinker 


mpirun -np 1 ./gol 7 7 1 blinker 1
mpirun -np 1 ./gol 7 7 2 blinker 1
Process grid: 1 x 1
Global alive cells after 1 generations: 3

Rank 0 - Generation 1:
0 0 0 0 0 0 0
0 0 0 0 0 0 0
0 0 0 1 0 0 0
0 0 0 1 0 0 0
0 0 0 1 0 0 0
0 0 0 0 0 0 0
0 0 0 0 0 0 0

Process grid: 1 x 1
Global alive cells after 2 generations: 3

Rank 0 - Generation 2:
0 0 0 0 0 0 0
0 0 0 0 0 0 0
0 0 0 0 0 0 0
0 0 1 1 1 0 0
0 0 0 0 0 0 0
0 0 0 0 0 0 0
0 0 0 0 0 0 0


Single live cell 


mpirun -np 1 ./gol 7 7 1 single 1

mpirun -np 1 ./gol 7 7 1 single 1
Process grid: 1 x 1
Global alive cells after 1 generations: 0

Rank 0 - Generation 1:
0 0 0 0 0 0 0
0 0 0 0 0 0 0
0 0 0 0 0 0 0
0 0 0 0 0 0 0
0 0 0 0 0 0 0
0 0 0 0 0 0 0
0 0 0 0 0 0 0   

block 


mpirun -np 1 ./gol 8 8 4 block 1
mpirun -np 4 ./gol 8 8 4 block 1
Process grid: 1 x 1
Global alive cells after 4 generations: 4

Rank 0 - Generation 4:
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 1 1 0 0 0
0 0 0 1 1 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0

Process grid: 2 x 2
Global alive cells after 4 generations: 4

Rank 0 - Generation 4:
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 1 1 0 0 0
0 0 0 1 1 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0



glider :   


mpirun -np 4 ./gol 8 8 6 glider 1
mpirun -np 4 ./gol 8 8 n glider 1  , and we keep changing n  
Process grid: 2 x 2
Global alive cells after 6 generations: 5

Rank 0 - Generation 6:
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 1 0 0 0
0 0 1 0 1 0 0 0
0 0 0 1 1 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0



Rank 0 - Generation 7:
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 1 0 0 0 0
0 0 0 0 1 1 0 0
0 0 0 1 1 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0



Rank 0 - Generation 8:
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 1 0 0 0
0 0 0 0 0 1 0 0
0 0 0 1 1 1 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0



Rank 0 - Generation 9:
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 1 0 1 0 0
0 0 0 0 1 1 0 0
0 0 0 0 1 0 0 0
0 0 0 0 0 0 0 0



Rank 0 - Generation 10:
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 1 0 0
0 0 0 1 0 1 0 0
0 0 0 0 1 1 0 0
0 0 0 0 0 0 0 0



Rank 0 - Generation 11:
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 1 0 0 0
0 0 0 0 0 1 1 0
0 0 0 0 1 1 0 0
0 0 0 0 0 0 0 0



Rank 0 - Generation 12:
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 1 0 0
0 0 0 0 0 0 1 0
0 0 0 0 1 1 1 0
0 0 0 0 0 0 0 0
