#include <assert.h>
#include "xcbext.h"
#include "xc_misc.h"

xcb_extension_t xcb_xc_misc_id = { "XC-MISC" };


/*****************************************************************************
 **
 ** xcb_xc_misc_get_version_cookie_t xcb_xc_misc_get_version
 ** 
 ** @param xcb_connection_t *c
 ** @param uint16_t          client_major_version
 ** @param uint16_t          client_minor_version
 ** @returns xcb_xc_misc_get_version_cookie_t
 **
 *****************************************************************************/

xcb_xc_misc_get_version_cookie_t
xcb_xc_misc_get_version (xcb_connection_t *c  /**< */,
                         uint16_t          client_major_version  /**< */,
                         uint16_t          client_minor_version  /**< */)
{
    static const xcb_protocol_request_t xcb_req = {
            /* count */ 2,
            /* ext */ &xcb_xc_misc_id,
            /* opcode */ XCB_XC_MISC_GET_VERSION,
            /* isvoid */ 0
    };

    struct iovec xcb_parts[4];
    xcb_xc_misc_get_version_cookie_t xcb_ret;
    xcb_xc_misc_get_version_request_t xcb_out;

    xcb_out.client_major_version = client_major_version;
    xcb_out.client_minor_version = client_minor_version;

    xcb_parts[2].iov_base = (char *) &xcb_out;
    xcb_parts[2].iov_len = sizeof(xcb_out);
    xcb_parts[3].iov_base = 0;
    xcb_parts[3].iov_len = -xcb_parts[2].iov_len & 3;
    xcb_ret.sequence = xcb_send_request(c, XCB_REQUEST_CHECKED, xcb_parts + 2, &xcb_req);
    return xcb_ret;
}


/*****************************************************************************
 **
 ** xcb_xc_misc_get_version_cookie_t xcb_xc_misc_get_version_unchecked
 ** 
 ** @param xcb_connection_t *c
 ** @param uint16_t          client_major_version
 ** @param uint16_t          client_minor_version
 ** @returns xcb_xc_misc_get_version_cookie_t
 **
 *****************************************************************************/

xcb_xc_misc_get_version_cookie_t
xcb_xc_misc_get_version_unchecked (xcb_connection_t *c  /**< */,
                                   uint16_t          client_major_version  /**< */,
                                   uint16_t          client_minor_version  /**< */)
{
    static const xcb_protocol_request_t xcb_req = {
            /* count */ 2,
            /* ext */ &xcb_xc_misc_id,
            /* opcode */ XCB_XC_MISC_GET_VERSION,
            /* isvoid */ 0
    };

    struct iovec xcb_parts[4];
    xcb_xc_misc_get_version_cookie_t xcb_ret;
    xcb_xc_misc_get_version_request_t xcb_out;

    xcb_out.client_major_version = client_major_version;
    xcb_out.client_minor_version = client_minor_version;

    xcb_parts[2].iov_base = (char *) &xcb_out;
    xcb_parts[2].iov_len = sizeof(xcb_out);
    xcb_parts[3].iov_base = 0;
    xcb_parts[3].iov_len = -xcb_parts[2].iov_len & 3;
    xcb_ret.sequence = xcb_send_request(c, 0, xcb_parts + 2, &xcb_req);
    return xcb_ret;
}


/*****************************************************************************
 **
 ** xcb_xc_misc_get_version_reply_t * xcb_xc_misc_get_version_reply
 ** 
 ** @param xcb_connection_t                  *c
 ** @param xcb_xc_misc_get_version_cookie_t   cookie
 ** @param xcb_generic_error_t              **e
 ** @returns xcb_xc_misc_get_version_reply_t *
 **
 *****************************************************************************/

xcb_xc_misc_get_version_reply_t *
xcb_xc_misc_get_version_reply (xcb_connection_t                  *c  /**< */,
                               xcb_xc_misc_get_version_cookie_t   cookie  /**< */,
                               xcb_generic_error_t              **e  /**< */)
{
    return (xcb_xc_misc_get_version_reply_t *) xcb_wait_for_reply(c, cookie.sequence, e);
}


/*****************************************************************************
 **
 ** xcb_xc_misc_get_xid_range_cookie_t xcb_xc_misc_get_xid_range
 ** 
 ** @param xcb_connection_t *c
 ** @returns xcb_xc_misc_get_xid_range_cookie_t
 **
 *****************************************************************************/

xcb_xc_misc_get_xid_range_cookie_t
xcb_xc_misc_get_xid_range (xcb_connection_t *c  /**< */)
{
    static const xcb_protocol_request_t xcb_req = {
            /* count */ 2,
            /* ext */ &xcb_xc_misc_id,
            /* opcode */ XCB_XC_MISC_GET_XID_RANGE,
            /* isvoid */ 0
    };

    struct iovec xcb_parts[4];
    xcb_xc_misc_get_xid_range_cookie_t xcb_ret;
    xcb_xc_misc_get_xid_range_request_t xcb_out;


    xcb_parts[2].iov_base = (char *) &xcb_out;
    xcb_parts[2].iov_len = sizeof(xcb_out);
    xcb_parts[3].iov_base = 0;
    xcb_parts[3].iov_len = -xcb_parts[2].iov_len & 3;
    xcb_ret.sequence = xcb_send_request(c, XCB_REQUEST_CHECKED, xcb_parts + 2, &xcb_req);
    return xcb_ret;
}


/*****************************************************************************
 **
 ** xcb_xc_misc_get_xid_range_cookie_t xcb_xc_misc_get_xid_range_unchecked
 ** 
 ** @param xcb_connection_t *c
 ** @returns xcb_xc_misc_get_xid_range_cookie_t
 **
 *****************************************************************************/

xcb_xc_misc_get_xid_range_cookie_t
xcb_xc_misc_get_xid_range_unchecked (xcb_connection_t *c  /**< */)
{
    static const xcb_protocol_request_t xcb_req = {
            /* count */ 2,
            /* ext */ &xcb_xc_misc_id,
            /* opcode */ XCB_XC_MISC_GET_XID_RANGE,
            /* isvoid */ 0
    };

    struct iovec xcb_parts[4];
    xcb_xc_misc_get_xid_range_cookie_t xcb_ret;
    xcb_xc_misc_get_xid_range_request_t xcb_out;


    xcb_parts[2].iov_base = (char *) &xcb_out;
    xcb_parts[2].iov_len = sizeof(xcb_out);
    xcb_parts[3].iov_base = 0;
    xcb_parts[3].iov_len = -xcb_parts[2].iov_len & 3;
    xcb_ret.sequence = xcb_send_request(c, 0, xcb_parts + 2, &xcb_req);
    return xcb_ret;
}


/*****************************************************************************
 **
 ** xcb_xc_misc_get_xid_range_reply_t * xcb_xc_misc_get_xid_range_reply
 ** 
 ** @param xcb_connection_t                    *c
 ** @param xcb_xc_misc_get_xid_range_cookie_t   cookie
 ** @param xcb_generic_error_t                **e
 ** @returns xcb_xc_misc_get_xid_range_reply_t *
 **
 *****************************************************************************/

xcb_xc_misc_get_xid_range_reply_t *
xcb_xc_misc_get_xid_range_reply (xcb_connection_t                    *c  /**< */,
                                 xcb_xc_misc_get_xid_range_cookie_t   cookie  /**< */,
                                 xcb_generic_error_t                **e  /**< */)
{
    return (xcb_xc_misc_get_xid_range_reply_t *) xcb_wait_for_reply(c, cookie.sequence, e);
}


/*****************************************************************************
 **
 ** xcb_xc_misc_get_xid_list_cookie_t xcb_xc_misc_get_xid_list
 ** 
 ** @param xcb_connection_t *c
 ** @param uint32_t          count
 ** @returns xcb_xc_misc_get_xid_list_cookie_t
 **
 *****************************************************************************/

xcb_xc_misc_get_xid_list_cookie_t
xcb_xc_misc_get_xid_list (xcb_connection_t *c  /**< */,
                          uint32_t          count  /**< */)
{
    static const xcb_protocol_request_t xcb_req = {
            /* count */ 2,
            /* ext */ &xcb_xc_misc_id,
            /* opcode */ XCB_XC_MISC_GET_XID_LIST,
            /* isvoid */ 0
    };

    struct iovec xcb_parts[4];
    xcb_xc_misc_get_xid_list_cookie_t xcb_ret;
    xcb_xc_misc_get_xid_list_request_t xcb_out;

    xcb_out.count = count;

    xcb_parts[2].iov_base = (char *) &xcb_out;
    xcb_parts[2].iov_len = sizeof(xcb_out);
    xcb_parts[3].iov_base = 0;
    xcb_parts[3].iov_len = -xcb_parts[2].iov_len & 3;
    xcb_ret.sequence = xcb_send_request(c, XCB_REQUEST_CHECKED, xcb_parts + 2, &xcb_req);
    return xcb_ret;
}


/*****************************************************************************
 **
 ** xcb_xc_misc_get_xid_list_cookie_t xcb_xc_misc_get_xid_list_unchecked
 ** 
 ** @param xcb_connection_t *c
 ** @param uint32_t          count
 ** @returns xcb_xc_misc_get_xid_list_cookie_t
 **
 *****************************************************************************/

xcb_xc_misc_get_xid_list_cookie_t
xcb_xc_misc_get_xid_list_unchecked (xcb_connection_t *c  /**< */,
                                    uint32_t          count  /**< */)
{
    static const xcb_protocol_request_t xcb_req = {
            /* count */ 2,
            /* ext */ &xcb_xc_misc_id,
            /* opcode */ XCB_XC_MISC_GET_XID_LIST,
            /* isvoid */ 0
    };

    struct iovec xcb_parts[4];
    xcb_xc_misc_get_xid_list_cookie_t xcb_ret;
    xcb_xc_misc_get_xid_list_request_t xcb_out;

    xcb_out.count = count;

    xcb_parts[2].iov_base = (char *) &xcb_out;
    xcb_parts[2].iov_len = sizeof(xcb_out);
    xcb_parts[3].iov_base = 0;
    xcb_parts[3].iov_len = -xcb_parts[2].iov_len & 3;
    xcb_ret.sequence = xcb_send_request(c, 0, xcb_parts + 2, &xcb_req);
    return xcb_ret;
}


/*****************************************************************************
 **
 ** uint32_t * xcb_xc_misc_get_xid_list_ids
 ** 
 ** @param const xcb_xc_misc_get_xid_list_reply_t *R
 ** @returns uint32_t *
 **
 *****************************************************************************/

uint32_t *
xcb_xc_misc_get_xid_list_ids (const xcb_xc_misc_get_xid_list_reply_t *R  /**< */)
{
    return (uint32_t *) (R + 1);
}


/*****************************************************************************
 **
 ** int xcb_xc_misc_get_xid_list_ids_length
 ** 
 ** @param const xcb_xc_misc_get_xid_list_reply_t *R
 ** @returns int
 **
 *****************************************************************************/

int
xcb_xc_misc_get_xid_list_ids_length (const xcb_xc_misc_get_xid_list_reply_t *R  /**< */)
{
    return R->ids_len;
}


/*****************************************************************************
 **
 ** xcb_generic_iterator_t xcb_xc_misc_get_xid_list_ids_end
 ** 
 ** @param const xcb_xc_misc_get_xid_list_reply_t *R
 ** @returns xcb_generic_iterator_t
 **
 *****************************************************************************/

xcb_generic_iterator_t
xcb_xc_misc_get_xid_list_ids_end (const xcb_xc_misc_get_xid_list_reply_t *R  /**< */)
{
    xcb_generic_iterator_t i;
    i.data = ((uint32_t *) (R + 1)) + (R->ids_len);
    i.rem = 0;
    i.index = (char *) i.data - (char *) R;
    return i;
}


/*****************************************************************************
 **
 ** xcb_xc_misc_get_xid_list_reply_t * xcb_xc_misc_get_xid_list_reply
 ** 
 ** @param xcb_connection_t                   *c
 ** @param xcb_xc_misc_get_xid_list_cookie_t   cookie
 ** @param xcb_generic_error_t               **e
 ** @returns xcb_xc_misc_get_xid_list_reply_t *
 **
 *****************************************************************************/

xcb_xc_misc_get_xid_list_reply_t *
xcb_xc_misc_get_xid_list_reply (xcb_connection_t                   *c  /**< */,
                                xcb_xc_misc_get_xid_list_cookie_t   cookie  /**< */,
                                xcb_generic_error_t               **e  /**< */)
{
    return (xcb_xc_misc_get_xid_list_reply_t *) xcb_wait_for_reply(c, cookie.sequence, e);
}