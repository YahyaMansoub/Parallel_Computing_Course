import csv

# Profiling results for different N values (matrix size)
# For matrix multiplication: O(N^3) operations, noise generation: O(N)
profiling_data = [
    {
        'N': 128,
        'total': 15_322_709,
        'generate_noise': 641,  # Very small sequential part
        'init_matrix': 352_282,  # 2 calls
        'matmul': 14_828_682
    },
    {
        'N': 256,
        'total': 119_584_319,
        'generate_noise': 1_281,  # Very small sequential part
        'init_matrix': 1_409_050,  # 2 calls
        'matmul': 118_032_650
    },
    {
        'N': 512,
        'total': None,  # Will extrapolate
        'generate_noise': None,
        'init_matrix': None,
        'matmul': None
    }
]

# Extrapolate for N=512 (8x larger than 128)
# Matrix operations scale as N^3, noise generation scales as N
profiling_data[2]['generate_noise'] = profiling_data[1]['generate_noise'] * 2
profiling_data[2]['init_matrix'] = profiling_data[1]['init_matrix'] * 4  # N^2 scaling
profiling_data[2]['matmul'] = profiling_data[1]['matmul'] * 8  # N^3 scaling
profiling_data[2]['total'] = (profiling_data[2]['generate_noise'] + 
                               profiling_data[2]['init_matrix'] + 
                               profiling_data[2]['matmul'])

# Calculate sequential fraction for each N
for data in profiling_data:
    # Sequential: generate_noise only (data dependency)
    # Parallelizable: init_matrix (2 calls) + matmul
    data['fs'] = data['generate_noise'] / data['total']
    data['fp'] = (data['init_matrix'] + data['matmul']) / data['total']

print("=" * 80)
print("PROFILING RESULTS - MATRIX MULTIPLICATION")
print("=" * 80)
print(f"{'N':<10} {'Total Instr':<15} {'gen_noise':<15} {'init_matrix':<15} {'matmul':<15} {'fs':<10} {'fp':<10}")
print("-" * 100)
for data in profiling_data:
    print(f"{data['N']:<10} {data['total']:<15,} {data['generate_noise']:<15,} "
          f"{data['init_matrix']:<15,} {data['matmul']:<15,} "
          f"{data['fs']:<10.6f} {data['fp']:<10.6f}")
print()

# Save to CSV
with open('profiling_results.csv', 'w', newline='') as f:
    writer = csv.DictWriter(f, fieldnames=['N', 'total', 'generate_noise', 'init_matrix', 'matmul', 'fs', 'fp'])
    writer.writeheader()
    writer.writerows(profiling_data)
print("Saved to: profiling_results.csv")
print()

# Average sequential fraction
avg_fs = sum(data['fs'] for data in profiling_data) / len(profiling_data)
print(f"Average sequential fraction (fs): {avg_fs:.6f} ({avg_fs*100:.4f}%)")
print(f"Note: fs decreases as N increases (sequential part becomes negligible)")
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
print(f"{'N':<10} {'Procs':<7} {'fs':<12} {'Amdahl':<10} {'Gustafson':<12} {'Efficiency':<12}")
print("-" * 80)
for result in results:
    print(f"{result['N']:<10} {result['Processors']:<7} {result['fs']:<12.6f} "
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
    print(f"N = {n:>4}: S_max = {max_speedup:>8.2f}x (fs = {fs:.6f} = {fs*100:.4f}%)")
print()

print("=" * 80)
print("KEY INSIGHT")
print("=" * 80)
print("As matrix size N increases:")
print("  - Sequential part (noise generation): O(N)")
print("  - Parallelizable part (matrix multiplication): O(N^3)")
print("  - Sequential fraction fs → 0 as N → ∞")
print("  - Better scalability for larger problems!")
print()
