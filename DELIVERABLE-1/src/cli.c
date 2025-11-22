/*!
 * \file            cli.c
 * \date            2025-11-22
 * \author          Mirko Lana [lana.mirko@icloud.com]
 *
 * \brief           Command-Line-Interface (CLI) module.
 */

#include "config.h"
#include "cli.h"
#include "slog.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>

static struct CliArguments g_cli_args; /*!< Global CLI arguments structure */

/*!
 * \brief           Print usage information to the specified output stream.
 *
 * \param           os: Output stream (e.g., stdout, stderr).
 * \param           pgm_name: Name of the program.
 */
static void prv_cli_print_usage(FILE *os, const char *pgm_name) {
    fprintf(os, "Usage: %s -i <matrix_file> [-t num_threads] [-w warmup] [-r runs] [-v | -q]\n", pgm_name);
    fprintf(os, "Options:\n");
    fprintf(os, "  -i <matrix_file>     Input file containing the sparse matrix in Matrix Market format (required)\n");
    fprintf(os, "  -t <num_threads>           Number of threads to use (Default: %d)\n", CONFIG_DEFAULT_NUM_THREADS);
    fprintf(os, "  -w <warmup>          Number of warm-up runs before benchmarking (Default: %d)\n", CONFIG_DEFAULT_WARMUP_ITERS);
    fprintf(os, "  -r <runs>            Number of benchmark runs (Default: %d)\n", CONFIG_DEFAULT_RUNS);
    fprintf(os, "  -v                   Enable DEBUG logging level\n");
    fprintf(os, "  -q                   Enable only ERROR logging level\n");
    fprintf(os, "  -h                   Show this help message\n");
}

const struct CliArguments *cli_parse_args(int argc, char *argv[]) {
    SLOG_DEBUG("Entering cli_parse_args");
    g_cli_args.input_file = NULL;
    g_cli_args.num_threads = CONFIG_DEFAULT_NUM_THREADS;
    g_cli_args.warmup_iters = CONFIG_DEFAULT_WARMUP_ITERS;
    g_cli_args.runs = CONFIG_DEFAULT_RUNS;
    g_cli_args.log_lv = CONFIG_DEFAULT_LOG_LV;

    if (argc < 2) {
        prv_cli_print_usage(stderr, argv[0]);
        exit(EXIT_FAILURE);
    }

    int opt;
    bool has_v = false;
    bool has_q = false;

    while ((opt = getopt(argc, argv, "i:o:t:w:r:vqh")) != EOF) {
        switch (opt) {
            case 'i':
                g_cli_args.input_file = optarg;
                break;

            case 't':
                g_cli_args.num_threads = atoi(optarg);
                if (g_cli_args.num_threads < 0) {
                    fprintf(stderr, "Error: The number of threads must be >= 0\n");
                    exit(EXIT_FAILURE);
                }
                break;

            case 'w':
                g_cli_args.warmup_iters = atoi(optarg);
                if (g_cli_args.warmup_iters < 0) {
                    fprintf(stderr, "Error: Warmup iterations must be >= 0\n");
                    exit(EXIT_FAILURE);
                }
                break;

            case 'r':
                g_cli_args.runs = atoi(optarg);
                if (g_cli_args.runs < 0) {
                    fprintf(stderr, "Error: Benchmark runs must be >= 1\n");
                    exit(EXIT_FAILURE);
                }
                break;

            case 'v':
                if (has_q) {
                    fprintf(stderr, "Error: Options -v (verbose) and -q (quiet) cannot be used together.\n");
                    prv_cli_print_usage(stderr, argv[0]);
                    exit(EXIT_FAILURE);
                }
                g_cli_args.log_lv ^= SLOG_LEVEL_DEBUG;
                has_v = true;
                break;

            case 'q':
                if (has_v) {
                    fprintf(stderr, "Error: Options -v (verbose) and -q (quiet) cannot be used together.\n");
                    prv_cli_print_usage(stderr, argv[0]);
                    exit(EXIT_FAILURE);
                }
                g_cli_args.log_lv = SLOG_LEVEL_ERROR;
                has_q = true;
                break;

            case 'h':
                prv_cli_print_usage(stdout, argv[0]);
                exit(EXIT_SUCCESS);

            case '?':
                prv_cli_print_usage(stderr, argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (!g_cli_args.input_file) {
        fprintf(stderr, "Error: Input matrix file (-i) is required.\n");
        prv_cli_print_usage(stderr, argv[0]);
        exit(EXIT_FAILURE);
    }

    return &g_cli_args;
}
