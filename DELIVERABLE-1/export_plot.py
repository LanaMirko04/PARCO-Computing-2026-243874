import os
import json5
import re
import matplotlib.pyplot as plt

FOLDER = "./results/threads_1"
FOLDER_OUT = FOLDER + "/plots"

pattern = re.compile(
    r"mm_matrix_(\d+)x(\d+)_density([0-9.]+)\.json"
)

json_files = [f for f in os.listdir(FOLDER) if f.endswith(".json")]

elements_list = []
tflops_list = []

if not os.path.exists(FOLDER_OUT):
    os.makedirs(FOLDER_OUT)

for filename in json_files:
    match = pattern.match(filename)
    if not match:
        print(f"Skipping unrecognized filename: {filename}")
        continue

    print(f"Opening {filename}")

    m = int(match.group(1))
    n = int(match.group(2))
    density = float(match.group(3))

    filepath = os.path.join(FOLDER, filename)

    with open(filepath, "r") as f:
        data = json5.load(f)

    samples = data["samples"]
    mean = data["mean"]  # microseconds

    title = f"{m}x{n}_density{density}"
    out_png = os.path.join(FOLDER_OUT, filename.replace(".json", ".png"))

    plt.figure(figsize=(8, 4))
    plt.plot(samples, label="samples")
    plt.axhline(mean, linestyle="--", label=f"mean = {mean} µs")
    plt.title(title)
    plt.xlabel("Sample index")
    plt.ylabel("Time (µs)")
    plt.legend()
    plt.tight_layout()
    plt.savefig(out_png)
    plt.close()

    nnz = m * n * density

    time_sec = mean * 1e-6
    tflops = nnz / time_sec / 1e12

    elements_list.append(nnz)
    tflops_list.append(tflops)

plt.figure(figsize=(8, 5))
plt.scatter(elements_list, tflops_list)
plt.title("Elements vs TFLOPS")
plt.xlabel("Number of Elements (including density)")
plt.ylabel("TFLOPS")
plt.tight_layout()
plt.savefig(os.path.join(FOLDER_OUT, "elements_vs_tflops.png"))
plt.close()

print("Plots generated.")
