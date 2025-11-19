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

/*!
 * \brief           Structure containing the configuration for a benchmark.
 */
struct BenchConfig {
    char *filename;             /*!< The name of the Matrix Market file to be used. */
    int warmup_iters;           /*!< The number of warmup iterations to perform. */
    int runs;                   /*!< The number of benchmark runs to perform. */
    struct ArenaHandler *arena; /*!< The arena handler to use for memory management. */
};

/*!
 * \brief           Structure containing the results of a benchmark.
 */
struct BenchResults {
    int warmup_iters;         /*!< The number of warmups done. */
    int runs;                 /*!< The number of runs. */
    struct ArenaObj *samples; /*!< The array containing the times of each run. */
    double mean;              /*!< The mean time of all runs. */
    double stddev;            /*!< The standard deviation of all runs. */
    double min;               /*!< The minimum time of all runs. */
    double max;               /*!< The maximum time of all runs. */
};

/*!
 * \brief           Initializes the benchmark module with the given configuration.
 *
 * \param[in]       cfg     Pointer to the benchmark configuration structure.
 */
int bench_init(const struct BenchConfig *cfg);

/*!
 * \brief           Performs the benchmark warmup iterations.
 *
 * \return          RC_OK on success, an error code otherwise.
 *                   - ...
 */
int bench_warmup(void);

#endif /*! BENCH_H */
