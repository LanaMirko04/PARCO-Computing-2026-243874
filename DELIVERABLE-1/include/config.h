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

#define CONFIG_RAND_MAX 99 /*! Maximum random value */
#define CONFIG_RAND_MIN 0  /*! Minimum random value */

/*!
 * @}
 */

/*!
 * \defgroup        Parallelism Configuration
 * @{
 */

// #define CONFIG_ENABLE_SERIAL_EXECUTION /*! Enable serial execution mode */
#define CONFIG_ENABLE_OMP_PARALLELISM /*! Enable OpenMP parallelism */
// #define CONFIG_ENABLE_PTHREADS_PARALLELISM /*! Enable Pthreads parallelism */
#define CONFIG_NUM_THREADS 12
#define CONFIG_OMP_SCHEDULE guided

/*!
  * @}
  */

#endif /* CONFIG_H */
