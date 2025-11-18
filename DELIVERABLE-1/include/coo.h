/*!
 * \file            coo.h
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

#ifndef COO_H
#define COO_H

#include "arena.h"
#include "vec.h"

#include <stdbool.h>

/*!
 * \brief           Structure representing a sparse matrix in COO format.
 */
struct CooMatrix {
    int m;        /*< Number of rows in the matrix */
    int n;        /*< Number of columns in the matrix */
    int nz;       /*< Number of non-zero items in the matrix */
    bool is_real; /*< Flag indicating if the matrix holds real (true) or integer (false) values */
    struct ArenaObj col;
    struct ArenaObj row;
    struct ArenaObj val;
};

/*!
 * \brief           Initialize a COO matrix by loading it from a Matrix Market file.
 *
 * \param[out]      mtx: Pointer to the COO matrix to initialize.
 * \param[in]       filename: Path to the Matrix Market file.
 * \param[out]      arena: Pointer to the arena handler for memory allocation.
 * \return          RC_OK on success, an error code otherwise:
 *                   - RC_INVALID_ARGUMENT_ERR if any argument is invalid.
 *                   - RC_FILE_IO_ERR if the file could not be opened.
 *                   - RC_FILE_INVALID_FMT_ERR if a parsing error occurs.
 *                   - RC_MEM_ALLOC_ERR if memory allocation fails.
 */
int coo_matrix_load_from_file(struct CooMatrix *mtx, const char *filename, struct ArenaHandler *arena);

/*!
 * \brief           Multiply a COO matrix with a vector.
 *
 * \param[in]       mtx: Pointer to the COO matrix.
 * \param[in]       vec: Pointer to the input vector.
 * \param[out]      result: Pointer to the output vector to store the result.
 * \return          RC_OK on success, an error code otherwise:
 *                   - RC_INVALID_ARGUMENT_ERR if any argument is invalid.
 *                   - ...
 */
int coo_matrix_mul_vec(const struct CooMatrix *mtx, const struct Vec *vec, struct Vec *result);

#endif /*! COO_H */
