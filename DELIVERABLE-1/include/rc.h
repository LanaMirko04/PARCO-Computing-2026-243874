#ifndef RC_H
#define RC_H

#define RC_ERR_MSG_LEN 128U /*!< Error message buffer length */

#define RC_FAIL -1 /*!< Undefined error */
#define RC_OK 0    /*!< Everything's OK */

#define RC_INVALID_ARG_ERR 0x101
#define RC_IDX_OUT_OF_BOUNDS_ERR 0x102
#define RC_MEM_ALLOC_ERR 0x103

/*
 * \brief           Set the error message.
 *
 * \param[in]       fmt: Format string.
 * \param[in]       ...: variable arguments.
 * \return          The length of the error message on success, RC_FAIL otherwise.
 */
int rc_set_err_msg(const char *fmt, ...);

/*!
 * \brief           Get the error message.
 *
 * \return          The pointer to the error message.
 */
const char *rc_get_err_msg(void);

#endif /*! RC_H */
