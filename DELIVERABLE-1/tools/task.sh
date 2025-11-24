#!/bin/bash

# --- CONFIGURATION ---
PROJECT_ROOT=$(dirname $(dirname $(realpath $0)))
BUILD_DIR="${PROJECT_ROOT}/build"
RESULTS_DIR="${PROJECT_ROOT}/results"
CSV_SUMMARY="${RESULTS_DIR}/summary.csv"
EXECUTABLE="${BUILD_DIR}/spvm"

NUM_THREADS=(1 2 4 8 16 32 64)
WARMUP_ITERATIONS=5
RUNS=10

MATRICES=($(find "${PROJECT_ROOT}/matrices/" -name "*.mtx"))

PERF_EVENTS="L1-dcache-loads,L1-dcache-load-misses,LLC-loads,LLC-load-misses"

# --- FUNCTIONS ---
load_modules() {
    module load gcc-9.1.0
    module load python-3.10.14
    module load perf
    echo "Modules loaded"
}

print_job_config() {
    echo "========================================"
    echo "Job Configuration"
    echo "========================================"
    echo "Project Root:      ${PROJECT_ROOT}"
    echo "Results Directory: ${RESULTS_DIR}"
    echo "Threads:           [ ${NUM_THREADS[*]} ]"
    echo "Matrices:          ${#MATRICES[@]} found"
    echo "========================================"
    echo "Matrices paths:"
    for matrix in "${MATRICES[@]}"; do
        echo " - $matrix"
    done
    echo "========================================"
}

load_modules
print_job_config

for num_threads in "${NUM_THREADS[@]}"; do
    OUTPUT_DIR="${RESULTS_DIR}/threads_${num_threads}"
    mkdir -p "${OUTPUT_DIR}"

for file in "${MATRICES[@]}"; do
        filename=$(basename "$file")
        matrix_name="${filename%.*}"

        LOG_FILE="${OUTPUT_DIR}/${matrix_name}_perf.txt"

        echo "[Running] Matrix: ${matrix_name} | Threads: ${num_threads}"

        echo "--- BENCHMARK RUN: $(date) ---" > "${LOG_FILE}"
        echo "File: $file" >> "${LOG_FILE}"
        echo "Threads: $num_threads" >> "${LOG_FILE}"
        echo "--------------------------------" >> "${LOG_FILE}"

        perf stat -e "$PERF_EVENTS" -x, -o perf_temp.csv \
            "${EXECUTABLE}" -i "${file}" -t "${num_threads}" -w ${WARMUP_ITERATIONS} -r ${RUNS} \
            >> "${LOG_FILE}" 2>&1

        echo -e "\n--- PERF REPORT ---" >> "${LOG_FILE}"
        cat perf_temp.csv >> "${LOG_FILE}"

        l1_loads=$(grep "L1-dcache-loads" perf_temp.csv | awk -F, '{print $1}')
        l1_misses=$(grep "L1-dcache-load-misses" perf_temp.csv | awk -F, '{print $1}')
        llc_loads=$(grep "LLC-loads" perf_temp.csv | awk -F, '{print $1}')
        llc_misses=$(grep "LLC-load-misses" perf_temp.csv | awk -F, '{print $1}')

        exec_time=$(grep -i "Time" "${LOG_FILE}" | tail -1 | awk '{print $NF}' || echo "N/A")

        l1_loads=${l1_loads:-0}
        l1_misses=${l1_misses:-0}
        llc_loads=${llc_loads:-0}
        llc_misses=${llc_misses:-0}

        echo "${matrix_name},${num_threads},${l1_loads},${l1_misses},${llc_loads},${llc_misses},${exec_time}" >> "$CSV_SUMMARY"

        rm perf_temp.csv
        mv *.json $(OUTPUT_DIR)
    done
done

echo "Benchmark complete! Output file: ${RESULTS_DIR}"
echo "CSV summary: ${CSV_SUMMARY}"
