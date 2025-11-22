/*!
 * \file            cli.h
 * \date            2025-11-22
 * \author          Mirko Lana [lana.mirko@icloud.com]
 *
 * \brief           Command-Line-Interface (CLI) module.
 */

#ifndef CLI_H
#define CLI_H

#include <stdint.h>

/*!
 * \brief          Command-Line Arguments
 */
struct CliArguments {
    char *input_file; /*!< Path to the input file */
    int warmup_iters; /*!< Number of warm-up iterations */
    int runs;         /*!< Number of benchmark runs */
    uint8_t log_lv;   /*!< Logging level */
};

/*!
 * \brief           Parse command-line arguments.
 *
 * \param           argc: Argument count.
 * \param           argv: Argument vector.
 * \return          Pointer to a structure containing parsed arguments.
 */
const struct CliArguments *cli_parse_args(int argc, char *argv[]);

#endif /*! CLI_H */
