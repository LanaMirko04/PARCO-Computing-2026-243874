# Deliverable 1 - Sparse Matrix-Vector Multiplication (SpMV)

## Table of Contents
- [Instructions](#instructions)
- [Project Structure](#project-structure)
- [Requitements](#requitements)
- [How to Compile and Run the Program](#how-to-compile-and-run-the-program)

## Instructions

> [!NOTE]
> These instructions are from the slides of the course.

**Develop** a code:
1. Read a matrix in the Matrix Format and convert to CSR
2. Multiply the Matrix with a randomly generated array (sequential code)
3. Design a parallel code on a shared-memory system (e.g., using OpenMP)

**Benchmarking**:
- Report the results of the multiplication in Milliseconds (at least 10 runs, report the 90% percentile)
- Select at least five matrices with different degrees of sparsity
- Sequential code vs Parallel Code by increasing the number of threads
- Evaluate different scheduling strategies
- Identify the bottleneck

**Bonus**: try to optimise the code by exploring the scientific papers.

## Project Structure
The project is structured as follows:

```
DELIBERABLE-1/
├── src/
│   ├── bench.c
│   ├── cli.c
│   ├── coo.c
│   ├── csr.c
│   ├── main.c
│   ├── mmio.c
│   ├── rc.c
│   └── vec.c
├── include/
│   ├── bench.h
│   ├── cli.h
│   ├── config.h
│   ├── coo.h
│   ├── csr.h
│   ├── mmio.h
│   ├── rc.h
│   ├── utils.h
│   └── vec.h
├── lib/
│   ├── arena
│   └── slog
├── matrices/
│   ├── mm_matrix_1000x1000_density0.10.mtx
│   ├── mm_matrix_100x100_density0.10.mtx
│   ├── mm_matrix_10x4_density0.10.mtx
│   └── README.md
├── tools/
│   ├── mmgen.py
│   ├── README.md
│   └── requirements.txt
├── Makefile
└── README.md
```

## Requitements
- C compiler (e.g., `gcc`) having support for C11 standard
- Make
- OpenMP library
- Python 3 (for running the tools)
- Tools dependencies (listed in `tools/requirements.txt`)

This program was tested on macOS and Linux systems. On macOS, I've used `clang-17.0.0` as C compiler and `python-3.14.0`. On Linux, I've used `gcc-9.x.x` and `python-3.x.x`.

## How to Compile and Run the Program
To compile the code, run the following command in the terminal:

```shell
$ make deps
$ make
```

To run the code, use the following command:

```shell
$ ./spmv -i <matrix_file>
```

This will execute the sparse Matrix-Vector multiplication using the specified matrix file in Matrix Format and the default values.
You can also specify additional options such as the number of threads, the number of warmup iterations, the number of runs and the logging level. Use the `-h` option to see all available options:

```shell
$ ./spmv -h
Usage: ./build/spvm -i <matrix_file> [-t num_threads] [-w warmup] [-r runs] [-v | -q]
Options:
  -i <matrix_file>     Input file containing the sparse matrix in Matrix Market format (required)
  -t <num_threads>     Number of threads to use (Default: 16)
  -w <warmup>          Number of warm-up runs before benchmarking (Default: 5)
  -r <runs>            Number of benchmark runs (Default: 10)
  -v                   Enable DEBUG logging level
  -q                   Enable only ERROR logging level
  -h                   Show this help message
```

> [!NOTE]
By default, the progam will compile and run the parallel version. To run the sequntial version, uncomment `CONFIG_ENABLE_SERIAL_EXECUTION` in `include/config.h` before compiling the code, and comment `CONFIG_ENABLE_OMP_PARALLELISM`.

...
