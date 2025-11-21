/*!
 * \file            main.c
 * \date            2025-11-07
 * \author          Mirko Lana [lana.mirko@icloud.com]
 *
 * \brief           Main entry point for the program.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#include "config.h"
#include "arena.h"
#include "rc.h"
#include "slog.h"
// #include "utils.h"
#include "bench.h"

static struct ArenaHandler g_arena_handler;
#ifdef CONFIG_ENABLE_OMP_PARALLELISM
static omp_lock_t g_omp_lock;
#endif /*! CONFIG_ENABLE_OMP_PARALLELISM */

static void enter_cs(void);
static void exit_cs(void);
static int init(int argc, char *argv[]);

int main(int argc, char *argv[]) {
    assert(!init(argc, argv)); /*! Terminate if initialization fails. */

    int res = bench_warmup();
    if (res != RC_OK) {
        SLOG_ERROR("%s", rc_get_err_msg);
        return res;
    }

    struct BenchResults bench_results;

    res = bench_run(&bench_results, &g_arena_handler);
    if (res != RC_OK) {
        SLOG_ERROR("%s", rc_get_err_msg());
        return res;
    }

    bench_save_result(&bench_results, "bench_results.json");

    return EXIT_SUCCESS;
}

static void enter_cs(void) {
#ifdef CONFIG_ENABLE_OMP_PARALLELISM
    omp_set_lock(&g_omp_lock);
#endif /*! CONFIG_ENABLE_OMP_PARALLELISM */
}

static void exit_cs(void) {
#ifdef CONFIG_ENABLE_OMP_PARALLELISM
    omp_unset_lock(&g_omp_lock);
#endif /*! CONFIG_ENABLE_OMP_PARALLELISM */
}

static int init(int argc, char *argv[]) {
    if (argc < 2) {
        puts("Usage: ./pgm path/to/file.mtx");
        return RC_FAIL;
    }

#ifdef CONFIG_ENABLE_OMP_PARALLELISM
    omp_init_lock(&g_omp_lock);
#endif /*! CONFIG_ENABLE_OMP_PARALLELISM */

    const struct SlogConfig slog_cfg = {
        .default_logger = SLOG_INIT_DEFAULT_LOGGER,
        .enter_cs = enter_cs,
        .exit_cs = exit_cs,
    };

    slog_init(&slog_cfg);
    SLOG_INFO("Initializing the program...");

    enum ArenaReturnCode arena_res = arena_init(&g_arena_handler);
    if (arena_res != ARENA_RC_OK) {
        SLOG_ERROR("Failed to initialize the arena allocator - %d", arena_res);
        return RC_FAIL;
    }

    const struct BenchConfig bench_cfg = {
        .filename = argv[1],
        .warmup_iters = 4,
        .runs = 10,
        .arena = &g_arena_handler,
    };

    srand(time(NULL));
    int res = bench_init(&bench_cfg);
    if (res != RC_OK) {
        SLOG_ERROR("Failed to initialize the benchmark module - %s", rc_get_err_msg());
        return RC_FAIL;
    }

    SLOG_INFO("Initialization completed successfully.");

    return RC_OK;
}
