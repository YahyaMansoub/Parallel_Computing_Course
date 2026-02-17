# serial
gcc ex3_serial.c -O2 -lm -o ex3_serial
./ex3_serial

# OpenMP sections
gcc ex3_sections.c -O2 -fopenmp -lm -o ex3_sections
OMP_NUM_THREADS=3 ./ex3_sections      # naive fits 3 threads
OMP_NUM_THREADS=4 ./ex3_sections      # optimized fits 4 threads (better)
