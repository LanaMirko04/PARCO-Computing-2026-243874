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
#include "arena.h"
#include "vec.h"

#include <stdlib.h>

int vec_init(struct Vec *vec, int n, bool is_real, struct ArenaHandler *arena) {
    if (!vec || !arena) {
        rc_set_err_msg("Invalid NULL argument(s) provided to vec_init");
        return RC_INVALID_ARG_ERR;
    }

    vec->n = n;
    vec->is_real = is_real;

    enum ArenaReturnCode res = arena_calloc(arena, is_real ? sizeof(double) : sizeof(int), n, &vec->val);
    if (res != ARENA_RC_OK) {
        rc_set_err_msg("Memory allocation failed in vec_init");
        return RC_MEM_ALLOC_ERR;
    }

    return RC_OK;
}

int vec_rand_fill(struct Vec *vec) {
    if (!vec) {
        rc_set_err_msg("Invalid NULL argument(s) provided to vec_rand_fill");
        return RC_INVALID_ARG_ERR;
    }

    void *val = arena_get_ptr(&vec->val);
    for (int i = 0; i < vec->n; i++) {
        if (vec->is_real) {
            ((double *)val)[i] = RAND_DOUBLE((double)CONFIG_RAND_MIN, (double)CONFIG_RAND_MAX);
        } else {
            ((int *)val)[i] = RAND_INT(CONFIG_RAND_MIN, CONFIG_RAND_MAX);
        }
    }

    return RC_OK;
}

int vel_fill_with_real(struct Vec *vec, double val) {
    if (!vec) {
        rc_set_err_msg("Invalid NULL argument(s) provided to vec_fill_with_real");
        return RC_INVALID_ARG_ERR;
    }

    if (!vec->is_real) {
        rc_set_err_msg("Attempting to fill integer vector with real values in vec_fill_with_real");
        return RC_INVALID_ARG_ERR;
    }

    double *dval = (double *)arena_get_ptr(&vec->val);
    for (int i = 0; i < vec->n; i++) {
        dval[i] = val;
    }

    return RC_OK;
}

int vec_fill_with_integer(struct Vec *vec, int val) {
    if (!vec) {
        rc_set_err_msg("Invalid NULL argument(s) provided to vec_fill_with_integer");
        return RC_INVALID_ARG_ERR;
    }

    if (vec->is_real) {
        rc_set_err_msg("Attempting to fill real vector with integer values in vec_fill_with_integer");
        return RC_INVALID_ARG_ERR;
    }

    int *ival = (int *)arena_get_ptr(&vec->val);
    for (int i = 0; i < vec->n; i++) {
        ival[i] = val;
    }

    return RC_OK;
}

inline int vec_size(const struct Vec *vec) {
    return vec ? (int)vec->n : RC_FAIL;
}

inline int vec_set_real_item(struct Vec *vec, int idx, double val) {
    if (!vec) {
        rc_set_err_msg("Invalid NULL argument(s) provided to vec_set_real_item");
        return RC_INVALID_ARG_ERR;
    }

    if (!vec->is_real) {
        rc_set_err_msg("Attempting to set real item in integer vector in vec_set_real_item");
        return RC_INVALID_ARG_ERR;
    }

    if (idx >= vec->n || idx < 0) {
        rc_set_err_msg("Index out of bounds in vec_set_real_item (%d >= %d)", idx, vec->n);
        return RC_IDX_OUT_OF_BOUNDS_ERR;
    }

    double *dval = (double *)arena_get_ptr(&vec->val);
    dval[idx] = val;

    return RC_OK;
}

inline int vec_set_integer_item(struct Vec *vec, int idx, int val) {
    if (!vec) {
        rc_set_err_msg("Invalid NULL argument(s) provided to vec_set_integer_item");
        return RC_INVALID_ARG_ERR;
    }

    if (vec->is_real) {
        rc_set_err_msg("Attempting to set integer item in real vector in vec_set_integer_item");
        return RC_INVALID_ARG_ERR;
    }

    if (idx >= vec->n || idx < 0) {
        rc_set_err_msg("Index out of bounds in vec_set_integer_item (%d >= %d)", idx, vec->n);
        return RC_IDX_OUT_OF_BOUNDS_ERR;
    }

    int *ival = (int *)arena_get_ptr(&vec->val);
    ival[idx] = val;

    return RC_OK;
}

inline int vec_get_real_item(const struct Vec *vec, int idx, double *val) {
    if (!vec || !val) {
        rc_set_err_msg("Invalid NULL argument(s) provided to vec_get_real_item");
        return RC_INVALID_ARG_ERR;
    }

    if (!vec->is_real) {
        rc_set_err_msg("Attempting to get real item from integer vector in vec_get_real_item");
        return RC_INVALID_ARG_ERR;
    }

    if (idx >= vec->n || idx < 0) {
        rc_set_err_msg("Index out of bounds in vec_get_real_item (%d >= %d)", idx, vec->n);
        return RC_IDX_OUT_OF_BOUNDS_ERR;
    }

    double *dval = (double *)arena_get_ptr(&vec->val);
    *val = dval[idx];

    return RC_OK;
}

inline int vec_get_integer_item(const struct Vec *vec, int idx, int *val) {
    if (!vec || !val) {
        rc_set_err_msg("Invalid NULL argument(s) provided to vec_get_integer_item");
        return RC_INVALID_ARG_ERR;
    }

    if (vec->is_real) {
        rc_set_err_msg("Attempting to get integer item from real vector in vec_get_integer_item");
        return RC_INVALID_ARG_ERR;
    }

    if (idx >= vec->n || idx < 0) {
        rc_set_err_msg("Index out of bounds in vec_get_integer_item (%d >= %d)", idx, vec->n);
        return RC_IDX_OUT_OF_BOUNDS_ERR;
    }

    int *ival = (int *)arena_get_ptr(&vec->val);
    *val = ival[idx];

    return RC_OK;
}
