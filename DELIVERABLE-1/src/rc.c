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

    bzero(rc_err_msg, RC_ERR_MSG_LEN);
    int len = vsnprintf(rc_err_msg, RC_ERR_MSG_LEN, fmt, args);

    va_end(args);

    return len < 0 ? RC_FAIL : len;
}

const char *rc_get_err_msg(void) {
    return rc_err_msg;
}
