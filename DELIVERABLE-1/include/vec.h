/*!
 * \file            vec.h
 * \date            2025-11-07
 * \author          Mirko Lana [lana.mirko@icloud.com]
 *
 * \brief           Implementation of vector operations.
 *
 * \details         This file contains functions for initializing, filling,
 *                  and manipulating vectors that can hold either real or integer values.
 *
 * \warning         Memory management is handled via an arena allocator; ensure
 *                  that the arena is properly initialized and destroyed.
 */

#ifndef VEC_H
#define VEC_H

#include "arena.h"

#include <stddef.h>
#include <stdbool.h>

/*!
 * \brief           Structure representing a vector.
 */
struct Vec {
    size_t n;     /*< Number of elements in the vector */
    bool is_real; /*< Flag indicating if the vector holds real (true) or integer (false) values */
    union {
        double *dvals; /*< Array of real values */
        int *ivals;    /*< Array of integer values */
    };
};

/*!
 * \brief           Initialize a vector.
 *
 * \param[out]      vec: Pointer to the vector to initialize.
 * \param[in]       n: Number of elements in the vector.
 * \param[in]       is_real: Flag indicating if the vector holds real values.
 * \param[out]      arena: Pointer to the arena handler for memory allocation.
 * \return          RC_OK on success, an error code otherwise:
 *                   - RC_INVALID_ARGUMENT_ERR if any argument is invalid.
 *                   - RC_MEM_ALLOC_ERR if memory allocation fails.
 */
int vec_init(struct Vec *vec, size_t n, bool is_real, struct ArenaHandler *arena);

/*!
 * \brief           Fill the vector with random values.
 *
 * \param[out]      vec: Pointer to the vector to fill.
 * \return          RC_OK on success, an error code otherwise:
 *                   - RC_INVALID_ARGUMENT_ERR if vec is NULL.
 */
int vec_rand_fill(struct Vec *vec);

/*!
 * \brief           Get the size of the vector.
 *
 * \param[in]       vec: Pointer to the vector.
 * \return          Size of the vector, or RC_FAIL if vec is NULL.
 */
int vec_size(const struct Vec *vec);

/*!
 * \brief           Set a real item in the vector.
 *
 * \param[out]      vec: Pointer to the vector.
 * \param[in]       idx: Index of the item to set (0 <= idx < n).
 * \param[in]       val: Real value to set.
 * \return          RC_OK on success, an error code otherwise:
 *                   - RC_INVALID_ARGUMENT_ERR if vec is NULL or not real.
 *                   - RC_IDX_OUT_OF_BOUNDS_ERR if idx is out of bounds.
 */
int vec_set_real_item(struct Vec *vec, size_t idx, double val);

/*!
 * \brief           Set an integer item in the vector.
 *
 * \param[out]      vec: Pointer to the vector.
 * \param[in]       idx: Index of the item to set (0 <= idx < n).
 * \param[in]       val: Integer value to set.
 * \return          RC_OK on success, an error code otherwise:
 *                   - RC_INVALID_ARGUMENT_ERR if vec is NULL or not integer.
 *                   - RC_IDX_OUT_OF_BOUNDS_ERR if idx is out of bounds.
 */
int vec_set_integer_item(struct Vec *vec, size_t idx, int val);

/*!
 * \brief           Get a real item from the vector.
 *
 * \param[in]       vec: Pointer to the vector.
 * \param[in]       idx: Index of the item to get (0 <= idx < n).
 * \param[out]      val: Pointer to store the retrieved real value.
 * \return          RC_OK on success, an error code otherwise:
 *                   - RC_INVALID_ARGUMENT_ERR if vec is NULL or not real.
 *                   - RC_IDX_OUT_OF_BOUNDS_ERR if idx is out of bounds.
 */
int vec_get_real_item(const struct Vec *vec, size_t idx, double *val);

/*!
 * \brief           Get an integer item from the vector.
 *
 * \param[in]       vec: Pointer to the vector.
 * \param[in]       idx: Index of the item to get (0 <= idx < n).
 * \param[out]      val: Pointer to store the retrieved integer value.
 * \return          RC_OK on success, an error code otherwise:
 *                   - RC_INVALID_ARGUMENT_ERR if vec is NULL or not integer.
 *                   - RC_IDX_OUT_OF_BOUNDS_ERR if idx is out of bounds.
 */
int vec_get_integer_item(const struct Vec *vec, size_t idx, int *val);

#endif /* VEC_H */
