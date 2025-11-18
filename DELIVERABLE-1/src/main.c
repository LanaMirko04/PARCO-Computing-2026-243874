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

#include "arena.h"
#include "rc.h"
// #define SLOG_DISABLE_LOGGING_SYSTEM /*! Uncomment to disable the logging system */
#include "slog.h"
#include "utils.h"
// #include "coo.h"
// #include "csr.h"

static struct ArenaHandler g_arena_handler;

static void enter_cs(void);
static void exit_cs(void);
static int init(void);

int main(int argc, char *argv[]) {
    UNUSED(argc);
    UNUSED(argv);
    assert(!init()); /*! Terminate if initialization fails. */

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
    enum ArenaReturnCode res = arena_init(&g_arena_handler);
    if (res != ARENA_RC_OK) {
        SLOG_ERROR("Failed to initialize arena: %d", res);
        return RC_FAIL;
    }

    SLOG_INFO("Initialization successful.");
    return RC_OK;
}
