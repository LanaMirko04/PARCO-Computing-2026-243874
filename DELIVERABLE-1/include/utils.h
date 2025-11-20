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

/*!
 * \brief           Suppress unused variable warnings.
 *
 * \param[in]       x: Variable to mark as unused.
 */
#define UNUSED(x) (void)(x)

/*!
 * \brief           Get the maximum of two values.
 *
 * \param[in]       x: First value.
 * \param[in]       y: Second value.
 * \return          Maximum of x and y.
 */
#define GET_MAX(x, y) ((x) > (y) ? (x) : (y))

/*!
 * \brief           Get the minimum of two values.
 *
 * \param[in]       x: First value.
 * \param[in]       y: Second value.
 * \return          Minimum of x and y.
 */
#define GET_MIN(x, y) ((x) > (y) ? (y) : (x))

#endif /*! UTILS_H */
