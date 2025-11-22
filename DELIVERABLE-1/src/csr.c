/*!
 * \file            csr.h
 * \date            2025-11-10
 * \author          Mirko Lana [lana.mirko@icloud.com]
 *
 * \brief           Implementation of CSR matrix operations.
 *
 * \details         This file contains functions for initializing, loading,
 *                  and performing operations on sparse matrices in CSR format.
 *
 * \warning         Memory management is handled via an arena allocator; ensure
 *                  that the arena is properly initialized and destroyed.
 */

#include "config.h"
#include "csr.h"
#include "rc.h"
#include "arena.h"
#include "coo.h"
#include "vec.h"
#include "utils.h"
#include "slog.h"

#include <string.h>
#include <stdbool.h>

/*! Unused function warning suppression */
static int prv_csr_matrix_mul_vec_serial(const struct CsrMatrix *mtx, const struct Vec *vec, struct Vec *result) __attribute__((unused));
static int prv_csr_matrix_mul_vec_omp(const struct CsrMatrix *mtx, const struct Vec *vec, struct Vec *result) __attribute__((unused));
static int prv_csr_matrix_mul_vec_pthreads(const struct CsrMatrix *mtx, const struct Vec *vec, struct Vec *result) __attribute__((unused));

/*!
 * \brief           Check if a CSR matrix is compatible with a vector for multiplication.
 *
 * \param[in]       mtx: Pointer to the COO matrix.
 * \param[in]       vec: Pointer to the vector.
 * \return          true if compatible, false otherwise.
 */
static inline bool prv_csr_matrix_is_compatible_with_vec(const struct CsrMatrix *mtx, const struct Vec *vec) {
    SLOG_DEBUG("Entering prv_csr_matrix_is_compatible_with_vec");
    if (!mtx || !vec)
        return false;

    return (mtx->n == vec->n) && (mtx->is_real == vec->is_real);
}

/*!
 * \brief           Multiply a CSR matrix with a vector (serial implementation).
 *
 * \param[in]       mtx: Pointer to the COO matrix.
 * \param[in]       vec: Pointer to the vector.
 * \param[out]      result: Pointer to the result vector.
 * \return          RC_OK on success, an error code otherwise.
 */
static int prv_csr_matrix_mul_vec_serial(const struct CsrMatrix *mtx, const struct Vec *vec, struct Vec *result) {
    // SLOG_DEBUG("Entering prv_csr_matrix_mul_vec_serial"); /*! disable logging for performance */
    int *row = arena_get_ptr(&mtx->row);
    int *col = arena_get_ptr(&mtx->col);

    // SLOG_DEBUG("Matrix dimensions: %d x %d, Non-zeros: %d", mtx->m, mtx->n, mtx->nz);
    // SLOG_DEBUG("Vector size: %d", vec_size(vec));
    // SLOG_DEBUG("Result vector size: %d", vec_size(result));

    if (mtx->is_real) {
        double *mtx_val = arena_get_ptr(&mtx->val);
        double *vec_val = arena_get_ptr(&vec->val);
        double *res_val = arena_get_ptr(&result->val);
        for (int i = 0; i <= mtx->m - 1; ++i) {
            double sum = 0.0;

            for (int k = row[i]; k < row[i + 1]; ++k) {
                sum += mtx_val[k] * vec_val[col[k]];
                // SLOG_DEBUG("mtx_val[%d] = %f, vec_val[%d] = %f, partial sum = %f", k, mtx_val[k], col[k], vec_val[col[k]], sum);
            }

            res_val[i] = sum;
            // SLOG_DEBUG("res_val[%d] = %f", i, res_val[i]);
        }
    } else {
        int *mtx_val = arena_get_ptr(&mtx->val);
        int *vec_val = arena_get_ptr(&vec->val);
        int *res_val = arena_get_ptr(&result->val);
        for (int i = 0; i <= mtx->m - 1; ++i) {
            int sum = 0;

            for (int k = row[i]; k < row[i + 1]; ++k) {
                sum += mtx_val[k] * vec_val[col[k]];
                // SLOG_DEBUG("mtx_val[%d] = %d, vec_val[%d] = %d, partial sum = %d", k, mtx_val[k], col[k], vec_val[col[k]], sum);
            }

            res_val[i] = sum;
            // SLOG_DEBUG("res_val[%d] = %d", i, res_val[i]);
        }
    }
    return RC_OK;
}

/*!
 * \brief           Multiply a CSR matrix with a vector (OpenMP implementation).
 *
 * \param[in]       mtx: Pointer to the COO matrix.
 * \param[in]       vec: Pointer to the vector.
 * \param[out]      result: Pointer to the result vector.
 * \return          RC_OK on success, an error code otherwise.
 */
static int prv_csr_matrix_mul_vec_omp(const struct CsrMatrix *mtx, const struct Vec *vec, struct Vec *result) {
    // SLOG_DEBUG("Entering prv_csr_matrix_mul_vec_omp"); /*! disable logging for performance */
    int *row = arena_get_ptr(&mtx->row);
    int *col = arena_get_ptr(&mtx->col);

    if (mtx->is_real) {
        double *mtx_val = arena_get_ptr(&mtx->val);
        double *vec_val = arena_get_ptr(&vec->val);
        double *res_val = arena_get_ptr(&result->val);

#pragma omp parallel for schedule(CONFIG_OMP_SCHEDULE)
        for (int i = 0; i <= mtx->m - 1; ++i) {
            double sum = 0.0;

#pragma omp simd reduction(+ : sum)
            for (int k = row[i]; k < row[i + 1]; ++k)
                sum += mtx_val[k] * vec_val[col[k]];

            res_val[i] = sum;
        }
    } else {
        int *mtx_val = arena_get_ptr(&mtx->val);
        int *vec_val = arena_get_ptr(&vec->val);
        int *res_val = arena_get_ptr(&result->val);

#pragma omp parallel for schedule(CONFIG_OMP_SCHEDULE)
        for (int i = 0; i <= mtx->m - 1; ++i) {
            int sum = 0;

#pragma omp simd reduction(+ : sum)
            for (int k = row[i]; k < row[i + 1]; ++k)
                sum += mtx_val[k] * vec_val[col[k]];

            res_val[i] = sum;
        }
    }
    return RC_OK;
}

/*!
 * \brief           Multiply a CSR matrix with a vector (Pthreads implementation).
 *
 * \param[in]       mtx: Pointer to the COO matrix.
 * \param[in]       vec: Pointer to the vector.
 * \param[out]      result: Pointer to the result vector.
 * \return          RC_OK on success, an error code otherwise.
 */
static int prv_csr_matrix_mul_vec_pthreads(const struct CsrMatrix *mtx, const struct Vec *vec, struct Vec *result) {
    SLOG_DEBUG("Entering prv_csr_matrix_mul_vec_pthreads");
    SLOG_WARN("Pthreads parallelism is not yet implemented for CSR matrix-vector multiplication.");
    UNUSED(mtx);
    UNUSED(vec);
    UNUSED(result);
    return RC_OK;
}

int csr_matrix_from_coo(struct CsrMatrix *dest, const struct CooMatrix *src, struct ArenaHandler *arena) {
    SLOG_DEBUG("Entering csr_matrix_from_coo");
    if (!dest || !src || !arena)
        return RC_INVALID_ARG_ERR;

    dest->m = src->m;
    dest->n = src->n;
    dest->nz = src->nz;
    dest->is_real = src->is_real;
    dest->col = src->col;
    dest->val = src->val;

    SLOG_DEBUG("Allocating memory for CSR row pointer array of size: %d", dest->m + 1);
    enum ArenaReturnCode res = arena_calloc(arena, sizeof(int), dest->m + 1, &dest->row);
    if (res != ARENA_RC_OK) {
        rc_set_err_msg("Memory array allocation failed in csr_matrix_from_coo");
        return RC_MEM_ALLOC_ERR;
    }
    SLOG_DEBUG("Memory allocated for CSR row pointer array");

    int *coo_row = arena_get_ptr(&src->row);
    int *csr_row = arena_get_ptr(&dest->row);
    int i = 0;

    for (; i < src->nz; ++i)
        csr_row[coo_row[i] + 1]++;

    for (i = 0; i < dest->m; ++i)
        csr_row[i + 1] += csr_row[i];

    return RC_OK;
}

int csr_matrix_load_from_file(struct CsrMatrix *mtx, const char *filename, struct ArenaHandler *arena) {
    SLOG_DEBUG("Entering csr_matrix_load_from_file");
    struct CooMatrix coo;

    int res = coo_matrix_load_from_file(&coo, filename, arena);
    if (res != RC_OK)
        return res;

    return csr_matrix_from_coo(mtx, &coo, arena);
}

int csr_matrix_mul_vec(const struct CsrMatrix *mtx, const struct Vec *vec, struct Vec *result) {
    // SLOG_DEBUG("Entering csr_matrix_mul_vec"); /*! disable logging for performance */
    if (!mtx || !vec || !result) {
        rc_set_err_msg("Invalid NULL argument(s) provided to coo_matrix_mul_vec");
        return RC_INVALID_ARG_ERR;
    }

    if (!prv_csr_matrix_is_compatible_with_vec(mtx, vec)) {
        rc_set_err_msg("Incompatible matrix and vector dimensions or types in coo_matrix_mul_vec");
        return RC_INVALID_ARG_ERR;
    }

    if (vec_size(result) != (int)mtx->m) {
        rc_set_err_msg("Result vector size does not match matrix row count in coo_matrix_mul_vec");
        return RC_INVALID_ARG_ERR;
    }

#ifdef CONFIG_ENABLE_SERIAL_EXECUTION
    return prv_csr_matrix_mul_vec_serial(mtx, vec, result);
#endif /*! CONFIG_ENABLE_SERIAL_EXECUTION */

#ifdef CONFIG_ENABLE_OMP_PARALLELISM
    return prv_csr_matrix_mul_vec_omp(mtx, vec, result);
#endif /*! CONFIG_ENABLE_OMP_PARALLELISM */

#ifdef CONFIG_ENABLE_PTHREADS_PARALLELISM
    return prv_csr_matrix_mul_vec_pthreads(mtx, vec, result);
#endif /*! CONFIG_ENABLE_PTHREADS_PARALLELISM */

    return RC_FAIL; /*! Configuration error: no parallelism mode enabled */
}
