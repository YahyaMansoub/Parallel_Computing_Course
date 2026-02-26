# run_bench.sh
#!/usr/bin/env bash
set -euo pipefail

N=${1:-2000000}        
EPOCHS=${2:-2000}
LR=${3:-0.05}
TH=${4:-1e-2}

OUT=bench.csv
echo "P,N,epochs,lr,thresh,time_s" > "$OUT"

for P in $(seq 1 56); do
  echo "Running P=$P ..."
  t=$(mpirun -np "$P" ./distrib_grad "$N" "$EPOCHS" "$LR" "$TH" \
      | awk '/Training time:/ {print $(NF-2)}' | tail -n 1)
  echo "$P,$N,$EPOCHS,$LR,$TH,$t" >> "$OUT"
done

echo "Wrote $OUT"