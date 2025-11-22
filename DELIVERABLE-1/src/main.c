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
#include <string.h>
#include <time.h>
#include <omp.h>

#include "cli.h"
#include "config.h"
#include "arena.h"
#include "rc.h"
#include "slog.h"
#include "bench.h"

static struct ArenaHandler g_arena_handler;
static char g_bench_results_filename[CONFIG_BENCH_FILENAME_MAX_LEN];
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

    bench_save_result(&bench_results, g_bench_results_filename);

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
    const struct CliArguments *cli_args = cli_parse_args(argc, argv);

    struct Slogger logger = SLOG_INIT_DEFAULT_LOGGER;
    logger.lv = cli_args->log_lv;

    const struct SlogConfig slog_cfg = {
        .default_logger = logger,
        .enter_cs = enter_cs,
        .exit_cs = exit_cs,
    };

#ifdef CONFIG_ENABLE_OMP_PARALLELISM
    omp_init_lock(&g_omp_lock);

    SLOG_INFO("Set OpenMP threads count to: %d", cli_args->num_threads);
    omp_set_num_threads(cli_args->num_threads);
#endif /*! CONFIG_ENABLE_OMP_PARALLELISM */

    slog_init(&slog_cfg);
    SLOG_INFO("Initializing the program...");

    enum ArenaReturnCode arena_res = arena_init(&g_arena_handler);
    if (arena_res != ARENA_RC_OK) {
        SLOG_ERROR("Failed to initialize the arena allocator - %d", arena_res);
        return RC_FAIL;
    }

    const struct BenchConfig bench_cfg = {
        .filename = cli_args->input_file,
        .warmup_iters = cli_args->warmup_iters,
        .runs = cli_args->runs,
        .arena = &g_arena_handler,
    };

    srand(time(NULL));
    int res = bench_init(&bench_cfg);
    if (res != RC_OK) {
        SLOG_ERROR("Failed to initialize the benchmark module - %s", rc_get_err_msg());
        return RC_FAIL;
    }

    /*! Get the filename only (removing path/to/file and extension) */
    const char *last_slash = strrchr(cli_args->input_file, '/');
    if (last_slash)
        strncpy(g_bench_results_filename, last_slash + 1, strlen(last_slash + 1) - 3);
    else
        strncpy(g_bench_results_filename, cli_args->input_file, strlen(cli_args->input_file) - 3);
    strncat(g_bench_results_filename, "json", 5);

    SLOG_INFO("Benchmark results will be saved to '%s'", g_bench_results_filename);
    SLOG_INFO("Initialization completed successfully.");

    return RC_OK;
}
