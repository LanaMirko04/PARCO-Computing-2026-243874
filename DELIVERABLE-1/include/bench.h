/*!
 * \file            bench.h
 * \date            2025-11-19
 * \author          Mirko Lana[lana.mirko@icloud.com]
 *
 * \brief           Benchmark module.
 */

#ifndef BENCH_H
#define BENCH_H

#include "arena.h"

#include <stdint.h>

/*!
 * \brief           Structure containing the configuration for a benchmark.
 */
struct BenchConfig {
    char *filename;             /*!< The name of the Matrix Market file to be used. */
    int thread_count;           /*!< The number of threads to use. */
    int warmup_iters;           /*!< The number of warmup iterations to perform. */
    int runs;                   /*!< The number of benchmark runs to perform. */
    struct ArenaHandler *arena; /*!< The arena handler to use for memory management. */
};

/*!
 * \brief           Structure containing the results of a benchmark.
 */
struct BenchResults {
    int warmup_iters;        /*!< The number of warmups done. */
    int runs;                /*!< The number of runs. */
    struct ArenaObj samples; /*!< The array containing the times of each run. */
    uint64_t mean;           /*!< The mean time of all runs. */
    uint64_t stddev;         /*!< The standard deviation of all runs. */
    uint64_t min;            /*!< The minimum time of all runs. */
    uint64_t max;            /*!< The maximum time of all runs. */
};

/*!
 * \brief           Initialize the benchmark module with the given configuration.
 *
 * \param[in]       cfg: Pointer to the benchmark configuration structure.
 * \return          RC_OK on success, an error code otherwise.
 *                   - RC_INVALID_ARG_ERR if any of the provided arguments is NULL.
 *
 */
int bench_init(const struct BenchConfig *cfg);

/*!
 * \brief           Perform the benchmark warmup iterations.
 *
 * \return          RC_OK on success, an error code otherwise.
 *                   - RC_INVALID_ARG_ERR if the benchmark configuration is invalid (NULL).
 *                   - RC_FILE_IO_ERR if the file could not be opened.
 *                   - RC_FILE_INVALID_FMT_ERR if a parsing error occurs.
 *                   - RC_MEM_ALLOC_ERR if memory allocation fails.

 *
 */
int bench_warmup(void);

/*!
 * \brief           Run the benchamerk routine.
 *
 * \param[in]       results: Pointer to rhe benchmark result structure.
 * \param[out]      arena: Pointer to the arena handler for memory management.
 * \return          RC_OK on success, an error code otherwise.
 *                   - RC_INVALID_ARG_ERR if the benchmark configuration is invalid (NULL).
 *                   - RC_MEM_ALLOC_ERR if memory allocation fails.
 */
int bench_run(struct BenchResults *results, struct ArenaHandler *arena);

/*!
 * \brief           Save the benchmark results to a file (JSON format).
 *
 * \param[in]       results: Pointer to the benchmark result structure.
 * \param[in]       filename: The name of the file to save the results to.
 * \return          RC_OK on success, an error code otherwise.
 *                   - RC_INVALID_ARG_ERR if any of the provided arguments is NULL.
 *                   - RC_FILE_IO_ERR if there was an error opening/writing to the file
 *                     specified by filename.
 */
int bench_save_result(const struct BenchResults *results, const char *filename);

#endif /*! BENCH_H */
