import csv

# Profiling results for different N values
profiling_data = [
    {'N': 5_000_000, 'total': 81_391_967, 'add_noise': 25_000_002, 'init_b': 13_750_009, 
     'compute_addition': 30_000_004, 'reduction': 12_500_006},
    {'N': 10_000_000, 'total': 162_642_161, 'add_noise': 50_000_002, 'init_b': 27_500_009,
     'compute_addition': 60_000_004, 'reduction': 25_000_006},
    {'N': 100_000_000, 'total': 1_626_421_610, 'add_noise': 500_000_020, 'init_b': 275_000_090,
     'compute_addition': 600_000_040, 'reduction': 250_000_060},  # Extrapolated
]

# Calculate sequential fraction for each N
for data in profiling_data:
    data['fs'] = data['add_noise'] / data['total']
    data['fp'] = (data['init_b'] + data['compute_addition'] + data['reduction']) / data['total']

print("=" * 80)
print("PROFILING RESULTS")
print("=" * 80)
print(f"{'N':<15} {'Total Instr':<15} {'add_noise':<15} {'init_b':<15} {'compute_add':<15} {'reduction':<15} {'fs':<10} {'fp':<10}")
print("-" * 120)
for data in profiling_data:
    print(f"{data['N']:<15,} {data['total']:<15,} {data['add_noise']:<15,} {data['init_b']:<15,} "
          f"{data['compute_addition']:<15,} {data['reduction']:<15,} {data['fs']:<10.4f} {data['fp']:<10.4f}")
print()

# Save to CSV
with open('profiling_results.csv', 'w', newline='') as f:
    writer = csv.DictWriter(f, fieldnames=['N', 'total', 'add_noise', 'init_b', 'compute_addition', 'reduction', 'fs', 'fp'])
    writer.writeheader()
    writer.writerows(profiling_data)
print("Saved to: profiling_results.csv")
print()


# Average sequential fraction
avg_fs = sum(data['fs'] for data in profiling_data) / len(profiling_data)
print(f"Average sequential fraction (fs): {avg_fs:.4f} ({avg_fs*100:.2f}%)")
print()

# Amdahl's Law calculations
processors = [1, 2, 4, 8, 16, 32, 64]

def amdahl_speedup(p, fs):
    """Calculate Amdahl's Law speedup"""
    return 1 / (fs + (1 - fs) / p)

def gustafson_speedup(p, fs):
    """Calculate Gustafson's Law speedup"""
    return p - fs * (p - 1)

# Calculate speedups for each N value
results = []
for data in profiling_data:
    n = data['N']
    fs = data['fs']
    
    for p in processors:
        amdahl = amdahl_speedup(p, fs)
        gustafson = gustafson_speedup(p, fs)
        efficiency = amdahl / p
        
        results.append({
            'N': n,
            'Processors': p,
            'fs': fs,
            'Amdahl_Speedup': amdahl,
            'Gustafson_Speedup': gustafson,
            'Efficiency': efficiency
        })

# Save detailed results
with open('scaling_results.csv', 'w', newline='') as f:
    fieldnames = ['N', 'Processors', 'fs', 'Amdahl_Speedup', 'Gustafson_Speedup', 'Efficiency']
    writer = csv.DictWriter(f, fieldnames=fieldnames)
    writer.writeheader()
    writer.writerows(results)

print("=" * 80)
print("SCALING RESULTS (Amdahl & Gustafson)")
print("=" * 80)
print(f"{'N':<15} {'Procs':<7} {'fs':<8} {'Amdahl':<10} {'Gustafson':<12} {'Efficiency':<12}")
print("-" * 80)
for result in results:
    print(f"{result['N']:<15,} {result['Processors']:<7} {result['fs']:<8.4f} "
          f"{result['Amdahl_Speedup']:<10.3f} {result['Gustafson_Speedup']:<12.3f} {result['Efficiency']:<12.3f}")
print()
print("Saved to: scaling_results.csv")
print()

# Maximum theoretical speedup
print("=" * 80)
print("MAXIMUM THEORETICAL SPEEDUP (Amdahl)")
print("=" * 80)
for data in profiling_data:
    n = data['N']
    fs = data['fs']
    max_speedup = 1 / fs
    print(f"N = {n:>11,}: S_max = {max_speedup:.2f}x (fs = {fs:.4f})")
print()

