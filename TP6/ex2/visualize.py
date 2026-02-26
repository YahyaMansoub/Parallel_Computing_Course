import csv
import matplotlib.pyplot as plt

P, T = [], []
with open("bench.csv") as f:
    r = csv.DictReader(f)
    for row in r:
        P.append(int(row["P"]))
        T.append(float(row["time_s"]))

# baseline = smallest P (should be 1)
t1 = None
for p, t in zip(P, T):
    if p == 1:
        t1 = t
        break
if t1 is None:
    raise SystemExit("Need P=1 row in bench.csv")

S = [t1 / t for t in T]          # speedup
E = [s / p for s, p in zip(S, P)] # efficiency

plt.figure()
plt.plot(P, S, marker="o")
plt.xlabel("Processes (P)")
plt.ylabel("Speedup")
plt.grid(True)
plt.savefig("speedup.png", dpi=200)

plt.figure()
plt.plot(P, E, marker="o")
plt.xlabel("Processes (P)")
plt.ylabel("Efficiency")
plt.grid(True)
plt.savefig("efficiency.png", dpi=200)

print("Saved speedup.png and efficiency.png")