import csv
import matplotlib.pyplot as plt

P=[]; speedup=[]; eff=[]
with open("results.csv") as f:
    r = csv.reader(f)
    for row in r:
        # CSV,P,N,tser,tpar,speedup,eff
        if row and row[0] == "CSV":
            P.append(int(row[1]))
            speedup.append(float(row[5]))
            eff.append(float(row[6]))

plt.figure()
plt.plot(P, speedup, marker='o')
plt.xlabel("Processes (P)")
plt.ylabel("Speedup")
plt.grid(True)
plt.savefig("speedup.png", dpi=200)

plt.figure()
plt.plot(P, eff, marker='o')
plt.xlabel("Processes (P)")
plt.ylabel("Efficiency")
plt.grid(True)
plt.savefig("efficiency.png", dpi=200)
print("Saved speedup.png and efficiency.png")