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

/*!
 * \struct          Benchmark handler structure.
 */
struct BenchHandler {
    struct CsrMatrix mtx;    /*!< Input matrix. */
    struct Vec vec;          /*!< Input vector. */
    struct CsrMatrix result; /*!< Result matrix. */
    int warmup_iters;        /*!< Number of warmup iterations. */
    int runs;                /*!< Number of benchmark runs. */
};

static struct BenchHandler g_bench_handler;

int bench_init(const struct BenchConfig *cfg) {
    SLOG_DEBUG("Entering bench_init");

    if (!cfg) {
        rc_set_err_msg("Invalid NULL argument(s) provided to bench_init");
        return RC_INVALID_ARG_ERR;
    }

    SLOG_DEBUG("Loading input matrix from file: %s", cfg->filename);
    int res = csr_matrix_load_from_file(&g_bench_handler.mtx, cfg->filename, cfg->arena);
    if (res != RC_OK)
        return res;
    SLOG_DEBUG("Martix loaded: rows=%d, cols=%d, non-zero=%d", g_bench_handler.mtx.m, g_bench_handler.mtx.n, g_bench_handler.mtx.nz);

    SLOG_DEBUG("Initializing input vector of size: %d", g_bench_handler.mtx.n);
    res = vec_init(&g_bench_handler.vec, g_bench_handler.mtx.m, g_bench_handler.mtx.is_real, cfg->arena);
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

    SLOG_WARN("TODO: Initialize result matrix properly (if needed, currently we don't know how multiplication will be implemented)");

    return RC_OK;
}

int bench_warmup(void) {
    SLOG_DEBUG("Entering bench_warmup");
    SLOG_WARN("TODO: Implement warmup iterations properly (currently just an empty loop)");

    for (int i = 0; i < g_bench_handler.warmup_iters; ++i)
        ;

    return RC_OK;
}
