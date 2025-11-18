# Deliverable 1

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

## How to run and compile
To compile the code, run the following command in the terminal:

```shell
$ make deps
$ make
```

To run the code, use the following command:

```shell
$ ./build/pgm # TODO: define CLI options
```
