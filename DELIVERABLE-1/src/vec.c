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

#include "config.h"
#include "utils.h"
#include "rc.h"
#include "vec.h"
#include "arena.h"

#include <stdlib.h>

int vec_init(struct Vec *vec, size_t n, bool is_real, struct ArenaHandler *arena) {
    if (!vec || !arena) {
        rc_set_err_msg("Invalid NULL argument(s) provided to vec_init");
        return RC_INVALID_ARG_ERR;
    }

    vec->n = n;
    vec->is_real = is_real;

    if (is_real) {
        vec->dvals = arena_calloc(arena, sizeof(double), n);
        if (!vec->dvals) {
            rc_set_err_msg("Memory allocation failed in vec_init");
            return RC_MEM_ALLOC_ERR;
        }
    } else {
        vec->ivals = arena_calloc(arena, sizeof(int), n);
        if (!vec->ivals) {
            rc_set_err_msg("Memory allocation failed in vec_init");
            return RC_MEM_ALLOC_ERR;
        }
    }

    return RC_OK;
}

int vec_rand_fill(struct Vec *vec) {
    if (!vec) {
        rc_set_err_msg("Invalid NULL argument(s) provided to vec_rand_fill");
        return RC_INVALID_ARG_ERR;
    }

    for (size_t i = 0; i < vec->n; i++) {
        if (vec->is_real) {
            vec->dvals[i] = RAND_DOUBLE((double)CONFIG_RAND_MIN, (double)CONFIG_RAND_MAX);
        } else {
            vec->ivals[i] = RAND_INT(CONFIG_RAND_MIN, CONFIG_RAND_MAX);
        }
    }

    return RC_OK;
}

inline int vec_size(const struct Vec *vec) {
    return vec ? (int)vec->n : RC_FAIL;
}

inline int vec_set_real_item(struct Vec *vec, size_t idx, double val) {
    if (!vec) {
        rc_set_err_msg("Invalid NULL argument(s) provided to vec_set_real_item");
        return RC_INVALID_ARG_ERR;
    }

    if (!vec->is_real) {
        rc_set_err_msg("Attempting to set real item in integer vector in vec_set_real_item");
        return RC_INVALID_ARG_ERR;
    }

    if (idx >= vec->n) {
        rc_set_err_msg("Index out of bounds in vec_set_real_item (%zu >= %zu)", idx, vec->n);
        return RC_IDX_OUT_OF_BOUNDS_ERR;
    }

    vec->dvals[idx] = val;

    return RC_OK;
}

inline int vec_set_integer_item(struct Vec *vec, size_t idx, int val) {
    if (!vec) {
        rc_set_err_msg("Invalid NULL argument(s) provided to vec_set_integer_item");
        return RC_INVALID_ARG_ERR;
    }

    if (vec->is_real) {
        rc_set_err_msg("Attempting to set integer item in real vector in vec_set_integer_item");
        return RC_INVALID_ARG_ERR;
    }

    if (idx >= vec->n) {
        rc_set_err_msg("Index out of bounds in vec_set_integer_item (%zu >= %zu)", idx, vec->n);
        return RC_IDX_OUT_OF_BOUNDS_ERR;
    }

    vec->ivals[idx] = val;

    return RC_OK;
}

inline int vec_get_real_item(const struct Vec *vec, size_t idx, double *val) {
    if (!vec || !val) {
        rc_set_err_msg("Invalid NULL argument(s) provided to vec_get_real_item");
        return RC_INVALID_ARG_ERR;
    }

    if (!vec->is_real) {
        rc_set_err_msg("Attempting to get real item from integer vector in vec_get_real_item");
        return RC_INVALID_ARG_ERR;
    }

    *val = vec->dvals[idx];

    return RC_OK;
}

inline int vec_get_integer_item(const struct Vec *vec, size_t idx, int *val) {
    if (!vec || !val) {
        rc_set_err_msg("Invalid NULL argument(s) provided to vec_get_integer_item");
        return RC_INVALID_ARG_ERR;
    }

    if (vec->is_real) {
        rc_set_err_msg("Attempting to get integer item from real vector in vec_get_integer_item");
        return RC_INVALID_ARG_ERR;
    }

    *val = vec->ivals[idx];

    return RC_OK;
}
