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

#include "arena.h"
#include "rc.h"
// #define SLOG_DISABLE_LOGGING_SYSTEM /*! Uncomment to disable the logging system */
#include "slog.h"
// #include "utils.h"
#include "bench.h"

static struct ArenaHandler g_arena_handler;
static char *filename;

static void enter_cs(void);
static void exit_cs(void);
static int init(void);

int main(int argc, char *argv[]) {
    if (argc < 2) {
        puts("Usage: pgm <filename>");
        return EXIT_FAILURE;
    }

    filename = argv[1];
    assert(!init()); /*! Terminate if initialization fails. */

    int res = bench_warmup();
    if (res != RC_OK) {
        SLOG_ERROR("%s", rc_get_err_msg);
        return res;
    }

    return EXIT_SUCCESS;
}

static void enter_cs(void) {
}

static void exit_cs(void) {
}

static int init(void) {
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
        .filename = filename,
        .warmup_iters = 0,
        .runs = 0,
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
