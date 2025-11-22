/*!
 * \file            config.h
 * \date            2025-11-07
 * \author          Mirko Lana [lana.mirko@icloud.com]
 *
 * \brief           Configuration header.
 */

#ifndef CONFIG_H
#define CONFIG_H

/*!
 * \defgroup        Configuration Constants
 * @{
 */

#define CONFIG_RAND_MAX 99                 /*! Maximum random value */
#define CONFIG_RAND_MIN 0                  /*! Minimum random value */
#define CONFIG_BENCH_FILENAME_MAX_LEN 256U /*! Maximum length for benchmark filename */
#define CONFIG_DEFAULT_WARMUP_ITERS 5      /*! Default number of warm-up iterations */
#define CONFIG_DEFAULT_RUNS 10             /*! Default number of runs */
#define CONFIG_DEFAULT_LOG_LV 0b0111       /*! Default logging level */

/*!
 * @}
 */

/*!
 * \defgroup        Parallelism Configuration
 * @{
 */

// #define CONFIG_ENABLE_SERIAL_EXECUTION     /*! Enable serial execution mode */
// #define CONFIG_ENABLE_PTHREADS_PARALLELISM /*! Enable Pthreads parallelism (not implemented) */
#define CONFIG_ENABLE_OMP_PARALLELISM /*! Enable OpenMP parallelism */
#define CONFIG_NUM_THREADS 12         /*! Number of threads to use */
#define CONFIG_OMP_SCHEDULE guided    /*! OpenMP scheduling strategy */

/*!
  * @}
  */

#endif /* CONFIG_H */
