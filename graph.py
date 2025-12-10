import matplotlib.pyplot as plt
import pandas as pd
# data
size_matrix = [5, 10, 20, 100, "100_unbal01", "100_unbal02", 200]
time_seq = [0.000036, 0.001514, 0.052709, 8.622553, 6.937852, 8.434988, 34.002142]
time_par_1 = [0.000109, 0.002118, 0.061571, 10.215725, 8.042134, 9.942877, 39.609188]
time_par_2 = [0.000263, 0.002616, 0.055108, 7.134671, 6.151776, 7.473184, 29.459213]
time_par_4 = [0.000500, 0.002715, 0.047844, 5.439832, 4.539538, 5.215368, 20.708845]
time_par_8 = [0.000932, 0.003459, 0.049824, 4.616095, 4.243098, 4.525379, 17.781714]
time_par_16 = [0.001807, 0.004817, 0.055425, 4.509151, 4.792775, 4.426284, 17.455141]
time_par_32 = [0.006312, 0.008817, 0.072567, 4.979203, 7.595659, 4.926578, 19.593398]
time_par_64 = [0.013820, 0.416194, 0.107006, 5.864683, 12.563988, 6.641087, 26.577784]

time_seq_fox = [0.000002, 0.000004, 0.000010, 0.000068, 0.000141, 0.000071, 0.000287]
time_par_fox_1 = [0.000006, 0.000007, 0.000014, 0.000081, 0.000173, 0.000084, 0.000341]
time_par_fox_2 = [0.000006, 0.000008, 0.000012, 0.000050, 0.000112, 0.000056, 0.000210]
time_par_fox_4 = [0.000006, 0.000008, 0.000012, 0.000032, 0.000086, 0.000037, 0.000127]
time_par_fox_8 = [0.000009, 0.000009, 0.000015, 0.000025, 0.000094, 0.000027, 0.000094]
time_par_fox_16 = [0.000012, 0.000013, 0.000018, 0.000031, 0.000175, 0.000034, 0.000147]
time_par_fox_32 = [0.000220, 0.000025, 0.000028, 0.000063, 0.000444, 0.000059, 0.000350]
time_par_fox_64 = [0.000478, 0.002026, 0.000043, 0.000103, 0.000872, 0.000141, 0.000792]

time_seq_rabbit = [0.000002, 0.000007, 0.000030, 0.000632, 0.000390, 0.000611, 0.002468]
time_par_rabbit_1 = [0.000009, 0.000010, 0.000036, 0.000778, 0.000468, 0.000747, 0.002977]
time_par_rabbit_2 = [0.000035, 0.000012, 0.000028, 0.000493, 0.000320, 0.000514, 0.002031]
time_par_rabbit_4 = [0.000073, 0.000014, 0.000020, 0.000329, 0.000190, 0.000306, 0.001235]
time_par_rabbit_8 = [0.000143, 0.000019, 0.000020, 0.000249, 0.000141, 0.000239, 0.000943]
time_par_rabbit_16 = [0.000285, 0.000029, 0.000022, 0.000230, 0.000117, 0.000220, 0.000857]
time_par_rabbit_32 = [0.000827, 0.000057, 0.000030, 0.000243, 0.000125, 0.000228, 0.000862]
time_par_rabbit_64 = [0.001817, 0.002130, 0.000048, 0.000251, 0.000163, 0.000269, 0.000850]





methods = {
    "Sequential": time_seq,
    "run-parallel (1)":  time_par_1,
    "run-parallel (2)":  time_par_2,
    "run-parallel (4)":  time_par_4,
    "run-parallel (8)":  time_par_8,
    "run-parallel (16)": time_par_16,
    "run-parallel (32)": time_par_32,
    "run-parallel (64)": time_par_64,
    # "Sequential move_foxes":  time_seq_fox,
    # "run-parallel-move_foxes (1)":  time_par_fox_1,
    # "run-parallel-move_foxes (2)":  time_par_fox_2,
    # "run-parallel-move_foxes (4)":  time_par_fox_4,
    # "run-parallel-move_foxes (8)":  time_par_fox_8,
    # "run-parallel-move_foxes (16)": time_par_fox_16,
    # "run-parallel-move_foxes (32)": time_par_fox_32,
    # "run-parallel-move_foxes (64)": time_par_fox_64,
    # "Sequential move_rabbits":  time_seq_rabbit,
    # "run-parallel-move_rabbits (1)":  time_par_rabbit_1,
    # "run-parallel-move_rabbits (2)":  time_par_rabbit_2,
    # "run-parallel-move_rabbits (4)":  time_par_rabbit_4,
    # "run-parallel-move_rabbits (8)":  time_par_rabbit_8,
    # "run-parallel-move_rabbits (16)": time_par_rabbit_16,
    # "run-parallel-move_rabbits (32)": time_par_rabbit_32,
    # "run-parallel-move_rabbits (64)": time_par_rabbit_64,
}

plt.figure(figsize=(9, 6))

for name, times in methods.items():
    # (assumes missing smallest ones)
    x_vals = size_matrix[-len(times):]
    if "Sequential" in name:
        plt.plot(x_vals, times, label=name, color='red', linewidth=4, marker='o', markersize=8)
    else:
        plt.plot(x_vals, times, label=name, linewidth=2, marker='o')

plt.xlabel("Matrix Size (N)", fontsize=12)
plt.ylabel("Execution Time (s)", fontsize=12)
plt.title("Performance Comparison of Sequential vs Parallel Methods", fontsize=14, fontweight='bold')
plt.legend()
plt.grid(True, linestyle='--', alpha=0.6)
plt.tight_layout()

#plt.yscale('log')

plt.show()



# --- calculate speedups ---
speedups = {}
for name, times in methods.items():
    if "Sequential" not in name:
        x_vals = size_matrix[-len(times):]

        if "move_foxes" in name:
            seq_vals = time_seq_fox[-len(times):]
        elif "move_rabbits" in name:
            seq_vals = time_seq_rabbit[-len(times):]
        else:
            seq_vals = time_seq[-len(times):]

        s = [seq / t for seq, t in zip(seq_vals, times)]
        speedups[name] = (x_vals, s)

#  graph speedups 
plt.figure(figsize=(9, 6))
for name, (sizes, spd) in speedups.items():
    plt.plot(sizes, spd, label=name, linewidth=2, marker='o')

plt.xlabel("Matrix Size (N)", fontsize=12)
plt.ylabel("Speedup (T_seq / T_parallel)", fontsize=12)
plt.title("Speedup of Parallel Methods vs Sequential", fontsize=14, fontweight='bold')
plt.axhline(1.0, color='gray', linestyle='--', linewidth=1)
plt.grid(True, linestyle='--', alpha=0.6)
plt.legend()
plt.tight_layout()
plt.show()

# table of speedups 
speedup_table = pd.DataFrame({
    size: {name: spds[i] if i < len(spds) else None
           for name, (_, spds) in speedups.items()}
    for i, size in enumerate(size_matrix)
})
print("Speedup Table (Sequential / Parallel times):")
print(speedup_table.round(2))
