/*!
 * \file            bench.c
 * \date            2025-11-19
 * \author          Mirko Lana[lana.mirko@icloud.com]
 *
 * \brief           Benchmark module.
 */

#include "rc.h"
#include "arena.h"
#include "bench.h"
#include "csr.h"
#include "vec.h"
#include "slog.h"
#include "utils.h"

#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <math.h>

/*!
 * \struct          Benchmark handler structure.
 */
struct BenchHandler {
    struct CsrMatrix mtx; /*!< Input matrix. */
    struct Vec vec;       /*!< Input vector. */
    struct Vec result;    /*!< Result matrix. */
    int warmup_iters;     /*!< Number of warmup iterations. */
    int runs;             /*!< Number of benchmark runs. */
};

static struct BenchHandler g_bench_handler; /*!< Global benchmark handler. */

/*!
 * \brief           Get current time in microseconds.
 *
 * \return          Current time in microseconds.
 */
static inline uint64_t prv_bench_get_us(void) {
    SLOG_DEBUG("Entering prv_bench_get_us");
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000000U + (uint64_t)(ts.tv_nsec / 1000U);
}

/*!
 * \brief           Compute standard deviation of samples.
 *
 * \param[in]       samples: Array of samples.
 * \param[in]       runs: Number of samples.
 * \param[in]       mean: Mean of samples.
 *
 * \return          Standard deviation of samples.
 */
static inline uint64_t prv_bench_compute_stddev(const uint64_t *samples, int runs, uint64_t mean) {
    SLOG_DEBUG("Entering prv_bench_compute_stddev");
    uint64_t sum = 0U;
    for (int i = 0; i < runs; ++i) {
        int64_t diff = (int64_t)samples[i] - (int64_t)mean;
        sum += (uint64_t)(diff * diff);
    }
    return (uint64_t)sqrt((double)sum / (double)runs);
}

int bench_init(const struct BenchConfig *cfg) {
    SLOG_DEBUG("Entering bench_init");

    if (!cfg) {
        rc_set_err_msg("Invalid NULL argument(s) provided to bench_init");
        return RC_INVALID_ARG_ERR;
    }

    SLOG_DEBUG("Setting warmup iterations to: %d", cfg->warmup_iters);
    g_bench_handler.warmup_iters = cfg->warmup_iters;

    SLOG_DEBUG("Setting benchmark runs to: %d", cfg->runs);
    g_bench_handler.runs = cfg->runs;

    SLOG_DEBUG("Loading input matrix from file: %s", cfg->filename);
    int res = csr_matrix_load_from_file(&g_bench_handler.mtx, cfg->filename, cfg->arena);
    if (res != RC_OK)
        return res;
    SLOG_DEBUG("Martix loaded: rows=%d, cols=%d, non-zero=%d", g_bench_handler.mtx.m, g_bench_handler.mtx.n, g_bench_handler.mtx.nz);

    SLOG_DEBUG("Initializing input vector of size: %d", g_bench_handler.mtx.n);
    res = vec_init(&g_bench_handler.vec, g_bench_handler.mtx.n, g_bench_handler.mtx.is_real, cfg->arena);
    if (res != RC_OK)
        return res;
    SLOG_DEBUG("Input vector initialized");

    SLOG_DEBUG("Filling input vector with random values");
    res = vec_rand_fill(&g_bench_handler.vec);
    if (res != RC_OK)
        return res;
    if (g_bench_handler.vec.is_real) {
        double v1, v2;
        vec_get_real_item(&g_bench_handler.vec, 0, &v1);
        vec_get_real_item(&g_bench_handler.vec, g_bench_handler.vec.n - 1, &v2);
        SLOG_DEBUG("Input vector filled. 2 random-picked values [%f, %f]", v1, v2);
    } else {
        int v1, v2;
        vec_get_integer_item(&g_bench_handler.vec, 0, &v1);
        vec_get_integer_item(&g_bench_handler.vec, g_bench_handler.vec.n - 1, &v2);
        SLOG_DEBUG("Input vector filled. 2 random-picked values [%d, %d]", v1, v2);
    }

    SLOG_DEBUG("Initializing result vector of size: %d", g_bench_handler.mtx.m);
    res = vec_init(&g_bench_handler.result, g_bench_handler.mtx.m, g_bench_handler.mtx.is_real, cfg->arena);
    if (res != RC_OK)
        return res;
    SLOG_DEBUG("Result vector initialized");

    return RC_OK;
}

int bench_warmup(void) {
    SLOG_DEBUG("Entering bench_warmup");

    SLOG_INFO("Starting warmup with %d iterations", g_bench_handler.warmup_iters);
    for (int i = 0; i < g_bench_handler.warmup_iters; ++i)
        csr_matrix_mul_vec(&g_bench_handler.mtx, &g_bench_handler.vec, &g_bench_handler.result);

    return RC_OK;
}

int bench_run(struct BenchResults *results, struct ArenaHandler *arena) {
    SLOG_DEBUG("Entering bench_run");
    if (!results) {
        rc_set_err_msg("Invalid NULL argument(s) provided to bench_run");
        return RC_INVALID_ARG_ERR;
    }

    SLOG_DEBUG("Initializing empty benchmark results structure");
    *results = (struct BenchResults){
        .warmup_iters = g_bench_handler.warmup_iters,
        .runs = g_bench_handler.runs,
        .samples = { 0 },
        .mean = 0U,
        .stddev = 0U,
        .min = UINT64_MAX,
        .max = 0U
    };

    SLOG_DEBUG("Allocating memory for benchmark samples array");
    enum ArenaReturnCode arena_res = arena_calloc(arena, sizeof(uint64_t), g_bench_handler.runs, &results->samples);
    if (arena_res != ARENA_RC_OK) {
        rc_set_err_msg("Memory array allocation failed in bench_run");
        return RC_MEM_ALLOC_ERR;
    }
    SLOG_DEBUG("Memory allocated for benchmark samples array");

    SLOG_INFO("Starting benchmark with %d runs", g_bench_handler.runs);
    uint64_t *samples = arena_get_ptr(&results->samples);
    for (int i = 0; i < g_bench_handler.runs; ++i) {
        uint64_t start = prv_bench_get_us();

        int res = csr_matrix_mul_vec(&g_bench_handler.mtx, &g_bench_handler.vec, &g_bench_handler.result);
        if (res != RC_OK)
            return res;

        uint64_t end = prv_bench_get_us();

        SLOG_DEBUG("Run %d completed in %llu us", i + 1, (end - start));

        /*! Update benchmark results. */
        samples[i] = end - start;
        results->mean += samples[i];
        results->min = GET_MIN(results->min, samples[i]);
        results->max = GET_MAX(results->max, samples[i]);
    }

    results->mean /= (uint64_t)g_bench_handler.runs;
    results->stddev = prv_bench_compute_stddev(samples, g_bench_handler.runs, results->mean);

    SLOG_INFO("Benchmark completed: mean=%llu us, stddev=%llu us, min=%llu us, max=%llu us",
              results->mean,
              results->stddev,
              results->min,
              results->max);

    return RC_OK;
}

int bench_save_result(const struct BenchResults *results, const char *filename) {
    if (!results || !filename) {
        rc_set_err_msg("Invalid NULL argument(s) provided to bench_run");
        return RC_INVALID_ARG_ERR;
    }

    SLOG_INFO("Saving benchmark results to file: %s", filename);
    SLOG_INFO("Openning file for writing");
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        rc_set_err_msg("Invalid file name provided to fopen - %s", strerror(errno));
        return RC_FILE_IO_ERR;
    }
    SLOG_INFO("File opened correctly");

    fprintf(fp, "{\n\t\"warmup-iters\": %d,\n\t\"runs\": %d,\n\t\"samples\": [", results->warmup_iters, results->runs);

    int i = 0;
    uint64_t *samples = arena_get_ptr(&results->samples);
    for (; i < results->runs - 1; ++i)
        fprintf(fp, "%llu, ", samples[i]);

    fprintf(fp, "%llu],\n\t\"mean\": %llu,\n\t\"stddev\": %llu,\n\t\"min\": %llu,\n\t\"max\": %llu,\n}", samples[i], results->mean, results->stddev, results->min, results->max);
    fclose(fp);

    return RC_OK;
}
