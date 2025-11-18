/*!
 * \file            rc.c
 * \date            2025-10-00
 * \author          Mirko Lana [lana.mirko@icloud.com]
 *
 * \brief           Implementation of return code and error message handling.
 */
#include "rc.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

static char rc_err_msg[RC_ERR_MSG_LEN] = { 0 }; /*!< The error message buffer */

int rc_set_err_msg(const char *fmt, ...) {
    if (!fmt)
        return RC_INVALID_ARG_ERR;

    va_list args;
    va_start(args, fmt);

    memset(rc_err_msg, 0U, RC_ERR_MSG_LEN);
    int len = vsnprintf(rc_err_msg, RC_ERR_MSG_LEN, fmt, args);

    va_end(args);

    return len < 0 ? RC_FAIL : len;
}

const char *rc_get_err_msg(void) {
    return rc_err_msg;
}
