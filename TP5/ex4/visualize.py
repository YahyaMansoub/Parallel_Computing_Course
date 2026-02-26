#!/usr/bin/env python3
"""
Visualize MPI Matrix-Vector Multiplication Performance
Generates speedup and efficiency plots from benchmark results
"""

import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import sys
import os

def load_results(filename='results.csv'):
    """Load benchmark results from CSV file"""
    if not os.path.exists(filename):
        print(f"Error: {filename} not found. Please run the benchmark first.")
        sys.exit(1)
    
    df = pd.read_csv(filename)
    print(f"Loaded {len(df)} benchmark results")
    print(f"Matrix sizes: {sorted(df['N'].unique())}")
    print(f"Process counts: {sorted(df['P'].unique())}")
    return df

def calculate_metrics(df):
    """Calculate speedup and efficiency metrics"""
    results = []
    
    for N in df['N'].unique():
        subset = df[df['N'] == N].copy()
        
        # Get serial time (P=1)
        serial_row = subset[subset['P'] == 1]
        if len(serial_row) == 0:
            print(f"Warning: No serial (P=1) result found for N={N}, skipping...")
            continue
        
        t_serial = serial_row['time_s'].values[0]
        
        for _, row in subset.iterrows():
            P = row['P']
            t_parallel = row['time_s']
            speedup = t_serial / t_parallel
            efficiency = speedup / P
            
            results.append({
                'N': N,
                'P': P,
                't_serial': t_serial,
                't_parallel': t_parallel,
                'speedup': speedup,
                'efficiency': efficiency * 100  # as percentage
            })
    
    return pd.DataFrame(results)

def plot_speedup(metrics_df, output_file='speedup.png'):
    """Plot speedup vs number of processes"""
    fig, ax = plt.subplots(figsize=(10, 6))
    
    for N in sorted(metrics_df['N'].unique()):
        subset = metrics_df[metrics_df['N'] == N]
        ax.plot(subset['P'], subset['speedup'], marker='o', label=f'N={N}')
    
    # Add ideal speedup line
    max_P = metrics_df['P'].max()
    P_range = np.array([1, max_P])
    ax.plot(P_range, P_range, 'k--', label='Ideal Speedup', linewidth=2)
    
    ax.set_xlabel('Number of Processes (P)', fontsize=12)
    ax.set_ylabel('Speedup', fontsize=12)
    ax.set_title('MPI Matrix-Vector Multiplication - Speedup', fontsize=14, fontweight='bold')
    ax.legend()
    ax.grid(True, alpha=0.3)
    ax.set_xscale('log', base=2)
    ax.set_yscale('log', base=2)
    
    plt.tight_layout()
    plt.savefig(output_file, dpi=300, bbox_inches='tight')
    print(f"Saved speedup plot to {output_file}")
    plt.close()

def plot_efficiency(metrics_df, output_file='efficiency.png'):
    """Plot efficiency vs number of processes"""
    fig, ax = plt.subplots(figsize=(10, 6))
    
    for N in sorted(metrics_df['N'].unique()):
        subset = metrics_df[metrics_df['N'] == N]
        ax.plot(subset['P'], subset['efficiency'], marker='s', label=f'N={N}')
    
    # Add 100% efficiency line
    ax.axhline(y=100, color='k', linestyle='--', label='Ideal (100%)', linewidth=2)
    
    ax.set_xlabel('Number of Processes (P)', fontsize=12)
    ax.set_ylabel('Efficiency (%)', fontsize=12)
    ax.set_title('MPI Matrix-Vector Multiplication - Efficiency', fontsize=14, fontweight='bold')
    ax.legend()
    ax.grid(True, alpha=0.3)
    ax.set_xscale('log', base=2)
    ax.set_ylim([0, 110])
    
    plt.tight_layout()
    plt.savefig(output_file, dpi=300, bbox_inches='tight')
    print(f"Saved efficiency plot to {output_file}")
    plt.close()

def plot_execution_time(metrics_df, output_file='execution_time.png'):
    """Plot execution time vs number of processes"""
    fig, ax = plt.subplots(figsize=(10, 6))
    
    for N in sorted(metrics_df['N'].unique()):
        subset = metrics_df[metrics_df['N'] == N]
        ax.plot(subset['P'], subset['t_parallel'], marker='^', label=f'N={N}')
    
    ax.set_xlabel('Number of Processes (P)', fontsize=12)
    ax.set_ylabel('Execution Time (s)', fontsize=12)
    ax.set_title('MPI Matrix-Vector Multiplication - Execution Time', fontsize=14, fontweight='bold')
    ax.legend()
    ax.grid(True, alpha=0.3)
    ax.set_xscale('log', base=2)
    ax.set_yscale('log')
    
    plt.tight_layout()
    plt.savefig(output_file, dpi=300, bbox_inches='tight')
    print(f"Saved execution time plot to {output_file}")
    plt.close()

def print_summary(metrics_df):
    """Print summary statistics"""
    print("\n" + "="*70)
    print("PERFORMANCE SUMMARY")
    print("="*70)
    
    for N in sorted(metrics_df['N'].unique()):
        subset = metrics_df[metrics_df['N'] == N]
        print(f"\nMatrix Size N={N}:")
        print(f"  {'P':>4} {'Time(s)':>12} {'Speedup':>12} {'Efficiency(%)':>15}")
        print("  " + "-"*50)
        
        for _, row in subset.iterrows():
            print(f"  {int(row['P']):>4} {row['t_parallel']:>12.6f} "
                  f"{row['speedup']:>12.2f} {row['efficiency']:>14.1f}%")
    
    print("\n" + "="*70)
    
    # Best speedup
    best = metrics_df.loc[metrics_df['speedup'].idxmax()]
    print(f"Best Speedup: {best['speedup']:.2f}x at N={int(best['N'])}, P={int(best['P'])}")
    
    # Best efficiency
    best_eff = metrics_df[metrics_df['P'] > 1].loc[
        metrics_df[metrics_df['P'] > 1]['efficiency'].idxmax()
    ]
    print(f"Best Efficiency: {best_eff['efficiency']:.1f}% at N={int(best_eff['N'])}, P={int(best_eff['P'])}")
    print("="*70 + "\n")

def main():
    """Main function"""
    # Load and process results
    df = load_results('results.csv')
    metrics_df = calculate_metrics(df)
    
    if len(metrics_df) == 0:
        print("Error: No valid metrics calculated. Check your results.csv file.")
        sys.exit(1)
    
    # Generate plots
    plot_speedup(metrics_df)
    plot_efficiency(metrics_df)
    plot_execution_time(metrics_df)
    
    # Print summary
    print_summary(metrics_df)
    
    # Save detailed metrics
    metrics_df.to_csv('metrics.csv', index=False)
    print("Detailed metrics saved to metrics.csv")

if __name__ == '__main__':
    main()
