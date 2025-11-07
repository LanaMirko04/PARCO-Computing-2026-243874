/*!
 * \file            coo.c
 * \date            2025-11-07
 * \author          Mirko Lana [lana.mirko@icloud.com]
 *
 * \brief           Implementation of COO matrix operations.
 *
 * \details         This file contains functions for initializing, loading,
 *                  and performing operations on sparse matrices in COO format.
 *
 * \warning         Memory management is handled via an arena allocator; ensure
 *                  that the arena is properly initialized and destroyed.
 */

#include "config.h"
#include "coo.h"
#include "rc.h"
#include "arena.h"
#include "vec.h"

/*! Unused function warning suppression */
static int prv_coo_matrix_mul_vec_serial(const struct CooMatrix *mtx, const struct Vec *vec, struct Vec *result) __attribute__((unused));
static int prv_coo_matrix_mul_vec_omp(const struct CooMatrix *mtx, const struct Vec *vec, struct Vec *result) __attribute__((unused));
static int prv_coo_matrix_mul_vec_pthreads(const struct CooMatrix *mtx, const struct Vec *vec, struct Vec *result) __attribute__((unused));

/*!
 * \brief           Initialize a COO matrix.
 *
 * \param[out]      mtx: Pointer to the COO matrix to initialize.
 * \param[in]       m: Number of rows in the matrix.
 * \param[in]       n: Number of columns in the matrix.
 * \param[in]       nz: Number of non-zero items in the matrix.
 * \param[in]       is_real: Flag indicating if the matrix holds real values.
 * \param[out]      arena: Pointer to the arena handler for memory allocation.
 * \return          RC_OK on success, an error code otherwise:
 *                   - RC_INVALID_ARGUMENT_ERR if any argument is invalid.
 *                   - RC_MEM_ALLOC_ERR if memory allocation fails.
 */
int prv_coo_matrix_init(struct CooMatrix *mtx, size_t m, size_t n, size_t nz, bool is_real, struct ArenaHandler *arena) {
    if (!mtx || !arena) {
        rc_set_err_msg("Invalid NULL argument(s) provided to prv_coo_matrix_init");
        return RC_INVALID_ARG_ERR;
    }

    mtx->m = m;
    mtx->n = n;
    mtx->nz = nz;
    mtx->is_real = is_real;

    mtx->items = arena_calloc(arena, sizeof(struct CooItem), nz);
    if (!mtx->items) {
        rc_set_err_msg("Memory allocation failed in prv_coo_matrix_init");
        return RC_MEM_ALLOC_ERR;
    }

    return RC_OK;
}

/*!
 * \brief           Check if a COO matrix is compatible with a vector for multiplication.
 *
 * \param[in]       mtx: Pointer to the COO matrix.
 * \param[in]       vec: Pointer to the vector.
 * \return          true if compatible, false otherwise.
 */
inline bool prv_coo_matrix_is_compatible_with_vec(const struct CooMatrix *mtx, const struct Vec *vec) {
    if (!mtx || !vec)
        return false;

    return (mtx->n == vec->n) && (mtx->is_real == vec->is_real);
}

/*!
 * \brief           Multiply a COO matrix with a vector (serial implementation).
 *
 * \param[in]       mtx: Pointer to the COO matrix.
 * \param[in]       vec: Pointer to the vector.
 * \param[out]      result: Pointer to the result vector.
 * \return          RC_OK on success, an error code otherwise.
 */
static int prv_coo_matrix_mul_vec_serial(const struct CooMatrix *mtx, const struct Vec *vec, struct Vec *result) {
    (void)mtx;
    (void)vec;
    (void)result;
    return RC_OK;
}

/*!
 * \brief           Multiply a COO matrix with a vector (OpenMP implementation).
 *
 * \param[in]       mtx: Pointer to the COO matrix.
 * \param[in]       vec: Pointer to the vector.
 * \param[out]      result: Pointer to the result vector.
 * \return          RC_OK on success, an error code otherwise.
 */
static int prv_coo_matrix_mul_vec_omp(const struct CooMatrix *mtx, const struct Vec *vec, struct Vec *result) {
    (void)mtx;
    (void)vec;
    (void)result;
    return RC_OK;
}

/*!
 * \brief           Multiply a COO matrix with a vector (Pthreads implementation).
 *
 * \param[in]       mtx: Pointer to the COO matrix.
 * \param[in]       vec: Pointer to the vector.
 * \param[out]      result: Pointer to the result vector.
 * \return          RC_OK on success, an error code otherwise.
 */
static int prv_coo_matrix_mul_vec_pthreads(const struct CooMatrix *mtx, const struct Vec *vec, struct Vec *result) {
    (void)mtx;
    (void)vec;
    (void)result;
    return RC_OK;
}

int coo_matrix_load_from_file(struct CooMatrix *mtx, const char *filename, struct ArenaHandler *arena) {
    if (!mtx || !filename || !arena) {
        rc_set_err_msg("Invalid NULL argument(s) provided to coo_matrix_load_from_file");
        return RC_INVALID_ARG_ERR;
    }

    /*! TODO: use mmio.h to load the sparse matrix */

    return RC_OK;
}

int coo_matrix_mul_vec(const struct CooMatrix *mtx, const struct Vec *vec, struct Vec *result) {
    if (!mtx || !vec || !result) {
        rc_set_err_msg("Invalid NULL argument(s) provided to coo_matrix_mul_vec");
        return RC_INVALID_ARG_ERR;
    }

    if (!prv_coo_matrix_is_compatible_with_vec(mtx, vec)) {
        rc_set_err_msg("Incompatible matrix and vector dimensions or types in coo_matrix_mul_vec");
        return RC_INVALID_ARG_ERR;
    }

    if (vec_size(result) != (int)mtx->m) {
        rc_set_err_msg("Result vector size does not match matrix row count in coo_matrix_mul_vec");
        return RC_INVALID_ARG_ERR;
    }

#ifdef CONFIG_ENABLE_SERIAL_EXECUTION
    return prv_coo_matrix_mul_vec_serial(mtx, vec, result);
#endif /*! CONFIG_ENABLE_SERIAL_EXECUTION */

#ifdef CONFIG_ENABLE_OMP_PARALLELISM
    return prv_coo_matrix_mul_vec_omp(mtx, vec, result);
#endif /*! CONFIG_ENABLE_OMP_PARALLELISM */

#ifdef CONFIG_ENABLE_PTHREADS_PARALLELISM
    return prv_coo_matrix_mul_vec_pthreads(mtx, vec, result);
#endif /*! CONFIG_ENABLE_PTHREADS_PARALLELISM */

    return RC_FAIL; /*! Configuration error: no parallelism mode enabled */
}
