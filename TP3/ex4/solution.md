


``
export OMP_NUM_THREADS=8
./mm_omp 1024 1024 static 16
``
m=1024 n=1024 threads=8 schedule=static chunk=16 time=1.626481 checksum=1675685888.000000


``
M=1024; N=1024
SCHED=static
CHUNK=16

echo "threads,time" > out.csv
for T in 1 2 4 8 16; do
  export OMP_NUM_THREADS=$T
  line=$(./mm_omp $M $N $SCHED $CHUNK)
  t=$(echo "$line" | awk -F'time=' '{print $2}' | awk '{print $1}')
  echo "$T,$t" >> out.csv
done
cat out.csv

``