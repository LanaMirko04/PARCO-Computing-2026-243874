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

#ifndef CSR_H
#define CSR_H

#include "arena.h"
#include "coo.h"
#include "vec.h"

#include <stdbool.h>

/*!
 * \brief           Structure representing a sparse matrix in CSR format.
 */
struct CsrMatrix {
    int m;        /*< Number of rows in the matrix */
    int n;        /*< Number of columns in the matrix */
    int nz;       /*< Number of non-zero items in the matrix */
    bool is_real; /*< Flag indicating if the matrix holds real (true) or integer (false) values */
    int col;
    int row;
    union {
        double *dval;
        int *ival;
    };
};

/*!
 * \brief           Initialize a CSR matrix by loading it from a Matrix Market file.
 *
 * \param[out]      dest: Pointer to the CSR matrix to initialize.
 * \param[in]       src: Pointer to the COO matrix used as source.
 * \param[out]      arena: Pointer to the arena handler for memory allocation.
 * \return          RC_OK on success, an error code otherwise:
 *                   - RC_INVALID_ARGUMENT_ERR if any argument is invalid.
 *                   - RC_MEM_ALLOC_ERR if memory allocation fails.
 */
int csr_matrix_from_coo(struct CsrMatrix *dest, const struct CooMatrix *src, struct ArenaHandler *arena);

/*!
 * \brief           Initialize a CSR matrix by loading it from a Matrix Market file.
 *
 * \param[out]      mtx: Pointer to the CSR matrix to initialize.
 * \param[in]       filename: Path to the Matrix Market file.
 * \param[out]      arena: Pointer to the arena handler for memory allocation.
 * \return          RC_OK on success, an error code otherwise:
 *                   - RC_INVALID_ARGUMENT_ERR if any argument is invalid.
 *                   - RC_FILE_IO_ERR if the file could not be opened.
 *                   - RC_FILE_INVALID_FMT_ERR if a parsing error occurs.
 *                   - RC_MEM_ALLOC_ERR if memory allocation fails.
 */
int csr_matrix_load_from_file(struct CsrMatrix *mtx, const char *filename, struct ArenaHandler *arena);

/*!
 * \brief           Multiply a CSR matrix with a vector.
 *
 * \param[in]       mtx: Pointer to the CSR matrix.
 * \param[in]       vec: Pointer to the input vector.
 * \param[out]      result: Pointer to the output vector to store the result.
 * \return          RC_OK on success, an error code otherwise:
 *                   - RC_INVALID_ARGUMENT_ERR if any argument is invalid.
 *                   - ...
 */
int csr_matrix_mul_vec(const struct CsrMatrix *mtx, const struct Vec *vec, struct Vec *result);

#endif /*! CSR_H */
