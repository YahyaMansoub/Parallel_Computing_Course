#!/usr/bin/env python3
"""
Performance analysis for loop unrolling benchmarks.
Analyzes bandwidth limits, ILP, and optimization effects.
"""

import csv
import sys
from collections import defaultdict

# Type sizes in bytes
TYPE_SIZES = {
    'double': 8,
    'float': 4,
    'int': 4,
    'short': 2
}

# Assumed memory bandwidth in GB/s (adjust based on your system)
# Typical values: DDR4 ~25-50 GB/s, DDR5 ~40-80 GB/s
MEMORY_BANDWIDTH_GB_S = 30.0  # Conservative estimate
N = 1000000

def read_results(filename):
    """Read benchmark results from CSV file."""
    results = defaultdict(lambda: defaultdict(dict))
    
    with open(filename, 'r') as f:
        reader = csv.DictReader(f)
        for row in reader:
            dtype = row['type']
            unroll = int(row['U'])
            time_ms = float(row['ms'])
            results[dtype][unroll] = time_ms
    
    return results

def calculate_bandwidth_limit(dtype):
    """Calculate theoretical minimum time based on bandwidth limit."""
    size_bytes = TYPE_SIZES[dtype]
    total_bytes = N * size_bytes
    bandwidth_bytes_per_ms = MEMORY_BANDWIDTH_GB_S * 1e9 / 1000.0  # Convert GB/s to B/ms
    min_time_ms = total_bytes / bandwidth_bytes_per_ms
    return min_time_ms

def analyze_results(results, opt_level):
    """Analyze benchmark results and print insights."""
    print(f"\n{'='*80}")
    print(f"ANALYSIS FOR OPTIMIZATION LEVEL: {opt_level}")
    print(f"{'='*80}\n")
    
    for dtype in ['double', 'float', 'int', 'short']:
        if dtype not in results:
            continue
            
        print(f"\n{dtype.upper()} TYPE (size: {TYPE_SIZES[dtype]} bytes)")
        print(f"{'-'*80}")
        
        # Calculate bandwidth limit
        bw_limit_ms = calculate_bandwidth_limit(dtype)
        total_mb = (N * TYPE_SIZES[dtype]) / (1024.0 * 1024.0)
        
        print(f"Total data: {total_mb:.2f} MB")
        print(f"Assumed bandwidth: {MEMORY_BANDWIDTH_GB_S:.1f} GB/s")
        print(f"Theoretical minimum time (bandwidth-limited): {bw_limit_ms:.3f} ms")
        print()
        
        # Find best performance
        unroll_factors = sorted(results[dtype].keys())
        times = [results[dtype][u] for u in unroll_factors]
        
        best_u = min(results[dtype].items(), key=lambda x: x[1])
        worst_u = max(results[dtype].items(), key=lambda x: x[1])
        
        print(f"Performance Summary:")
        print(f"  U=1  (no unroll):     {results[dtype][1]:.3f} ms")
        print(f"  Best (U={best_u[0]}):          {best_u[1]:.3f} ms  (speedup: {results[dtype][1]/best_u[1]:.2f}x)")
        print(f"  Worst (U={worst_u[0]}):         {worst_u[1]:.3f} ms")
        print()
        
        # Calculate efficiency vs bandwidth limit
        efficiency = (bw_limit_ms / best_u[1]) * 100
        print(f"Efficiency vs bandwidth limit: {efficiency:.1f}%")
        
        if efficiency > 80:
            print(f"  → BANDWIDTH-LIMITED: Performance close to memory bandwidth limit")
        elif efficiency > 50:
            print(f"  → PARTIALLY BANDWIDTH-LIMITED: Some bandwidth utilization")
        else:
            print(f"  → COMPUTE-LIMITED: Loop overhead and ILP dominate")
        
        print()
        
        # Analyze unrolling effectiveness
        speedup_u2 = results[dtype][1] / results[dtype][2]
        speedup_u4 = results[dtype][1] / results[dtype][4]
        speedup_u8 = results[dtype][1] / results[dtype][8]
        speedup_u16 = results[dtype][1] / results[dtype][16]
        speedup_u32 = results[dtype][1] / results[dtype][32]
        
        print(f"Unrolling Speedups (vs U=1):")
        print(f"  U=2:   {speedup_u2:.2f}x")
        print(f"  U=4:   {speedup_u4:.2f}x")
        print(f"  U=8:   {speedup_u8:.2f}x")
        print(f"  U=16:  {speedup_u16:.2f}x")
        print(f"  U=32:  {speedup_u32:.2f}x")
        print()
        
        # Saturation analysis
        if best_u[1] < results[dtype][32] * 1.05:  # Within 5%
            print(f"Performance saturates around U={best_u[0]}")
            print(f"Further unrolling provides diminishing returns")
        else:
            print(f"Performance continues improving with higher unroll factors")
        
        print()

def compare_optimizations(results_o0, results_o2):
    """Compare -O0 vs -O2 results."""
    print(f"\n{'='*80}")
    print(f"COMPARISON: -O0 vs -O2")
    print(f"{'='*80}\n")
    
    for dtype in ['double', 'float', 'int', 'short']:
        if dtype not in results_o0 or dtype not in results_o2:
            continue
        
        print(f"\n{dtype.upper()}:")
        print(f"  U │   -O0 (ms) │   -O2 (ms) │ Compiler Speedup │ Manual helps at -O2?")
        print(f"────┼────────────┼────────────┼──────────────────┼─────────────────────")
        
        for u in [1, 2, 4, 8, 16, 32]:
            if u in results_o0[dtype] and u in results_o2[dtype]:
                time_o0 = results_o0[dtype][u]
                time_o2 = results_o2[dtype][u]
                compiler_speedup = time_o0 / time_o2
                
                # Check if manual unrolling still helps at -O2
                if u > 1:
                    manual_speedup_o2 = results_o2[dtype][1] / time_o2
                    helps = "✓ Yes" if manual_speedup_o2 > 1.1 else "✗ No"
                else:
                    manual_speedup_o2 = 1.0
                    helps = "─"
                
                print(f" {u:2d} │ {time_o0:10.3f} │ {time_o2:10.3f} │      {compiler_speedup:6.2f}x     │ {helps}")
        
        # Summary
        best_o0 = min(results_o0[dtype].values())
        best_o2 = min(results_o2[dtype].values())
        
        manual_speedup_o0 = results_o0[dtype][1] / best_o0
        manual_speedup_o2 = results_o2[dtype][1] / best_o2
        
        print()
        print(f"  Best manual speedup at -O0: {manual_speedup_o0:.2f}x")
        print(f"  Best manual speedup at -O2: {manual_speedup_o2:.2f}x")
        
        if manual_speedup_o2 > 1.1:
            print(f"  → Manual unrolling STILL BENEFICIAL with -O2")
        else:
            print(f"  → Compiler auto-vectorization makes manual unrolling UNNECESSARY")
        print()

def explain_performance_trends():
    """Explain why performance improves then saturates."""
    print(f"\n{'='*80}")
    print(f"WHY DOES PERFORMANCE IMPROVE THEN SATURATE?")
    print(f"{'='*80}\n")
    
    print("1. LOOP OVERHEAD REDUCTION")
    print("   - Each loop iteration has overhead: increment i, compare, branch")
    print("   - U=1:  N iterations → N × overhead")
    print("   - U=4:  N/4 iterations → (N/4) × overhead")
    print("   - Unrolling by U reduces loop overhead by factor of U")
    print()
    
    print("2. INSTRUCTION-LEVEL PARALLELISM (ILP)")
    print("   - Modern CPUs can execute multiple independent instructions simultaneously")
    print("   - Unrolling exposes more independent additions: sum += a[i] + a[i+1] + ...")
    print("   - CPU can pipeline/parallelize these additions")
    print("   - Benefit saturates when ILP reaches CPU's execution width (~4-8 ops/cycle)")
    print()
    
    print("3. MEMORY BANDWIDTH SATURATION")
    print("   - As loop overhead decreases, memory fetches dominate")
    print("   - Performance becomes limited by memory bandwidth:")
    print(f"     T_min ≈ (N × sizeof(type)) / BW")
    print("   - Example: 1M doubles × 8 bytes / 30 GB/s ≈ 0.27 ms")
    print("   - No amount of unrolling can go faster than this limit")
    print()
    
    print("4. REGISTER PRESSURE")
    print("   - Very high unrolling (U=32) may cause register spills")
    print("   - Intermediate sums need to be stored in memory")
    print("   - Can actually hurt performance beyond optimal U")
    print()
    
    print("TYPICAL PATTERN:")
    print("  U=1→2:   Large speedup (loop overhead reduction)")
    print("  U=2→8:   Moderate speedup (ILP exploitation)")
    print("  U=8→16:  Small speedup (approaching bandwidth limit)")
    print("  U=16→32: Minimal/negative (bandwidth-limited, possible register pressure)")
    print()

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print("Usage: python3 analyze_performance.py <results_O0.csv> [results_O2.csv]")
        sys.exit(1)
    
    results_o0_file = sys.argv[1]
    results_o0 = read_results(results_o0_file)
    analyze_results(results_o0, "-O0")
    
    if len(sys.argv) > 2:
        results_o2_file = sys.argv[2]
        results_o2 = read_results(results_o2_file)
        analyze_results(results_o2, "-O2")
        compare_optimizations(results_o0, results_o2)
    
    explain_performance_trends()
    
    print(f"\n{'='*80}")
    print("ANALYSIS COMPLETE")
    print(f"{'='*80}\n")
