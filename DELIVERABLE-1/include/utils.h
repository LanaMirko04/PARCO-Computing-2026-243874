/*!
 * \file            utils.h
 * \date            2025-11-07
 * \author          Mirko Lana [lana.mirko@icloud.com]
 *
 * \brief           Utility macros and functions.
 */

#ifndef UTILS_H
#define UTILS_H

/*!
 * \brief           Generate a random integer between min and max (inclusive).
 *
 * \param[in]       min: Minimum integer value.
 * \param[in]       max: Maximum integer value.
 * \return          Random integer between min and max.
 */
#define RAND_INT(min, max) (rand() % ((max) - (min) + 1) + (min))

/*!
 * \brief           Generate a random double between min and max.
 *
 * \param[in]       min: Minimum double value.
 * \param[in]       max: Maximum double value.
 * \return          Random double between min and max.
 */
#define RAND_DOUBLE(min, max) (((double)rand() / RAND_MAX) * ((max) - (min)) + (min))

#endif /*! UTILS_H */
