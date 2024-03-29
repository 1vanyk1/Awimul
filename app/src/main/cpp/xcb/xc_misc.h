#ifndef __XC_MISC_H
#define __XC_MISC_H

#include "xcb.h"

#define XCB_XCMISC_MAJOR_VERSION 1
#define XCB_XCMISC_MINOR_VERSION 1

extern xcb_extension_t xcb_xc_misc_id;

/**
 * @brief xcb_xc_misc_get_version_cookie_t
 **/
typedef struct xcb_xc_misc_get_version_cookie_t {
    unsigned int sequence; /**<  */
} xcb_xc_misc_get_version_cookie_t;

/** Opcode for xcb_xc_misc_get_version. */
#define XCB_XC_MISC_GET_VERSION 0

/**
 * @brief xcb_xc_misc_get_version_request_t
 **/
typedef struct xcb_xc_misc_get_version_request_t {
    uint8_t  major_opcode; /**<  */
    uint8_t  minor_opcode; /**<  */
    uint16_t length; /**<  */
    uint16_t client_major_version; /**<  */
    uint16_t client_minor_version; /**<  */
} xcb_xc_misc_get_version_request_t;

/**
 * @brief xcb_xc_misc_get_version_reply_t
 **/
typedef struct xcb_xc_misc_get_version_reply_t {
    uint8_t  response_type; /**<  */
    uint8_t  pad0; /**<  */
    uint16_t sequence; /**<  */
    uint32_t length; /**<  */
    uint16_t server_major_version; /**<  */
    uint16_t server_minor_version; /**<  */
} xcb_xc_misc_get_version_reply_t;

/**
 * @brief xcb_xc_misc_get_xid_range_cookie_t
 **/
typedef struct xcb_xc_misc_get_xid_range_cookie_t {
    unsigned int sequence; /**<  */
} xcb_xc_misc_get_xid_range_cookie_t;

/** Opcode for xcb_xc_misc_get_xid_range. */
#define XCB_XC_MISC_GET_XID_RANGE 1

/**
 * @brief xcb_xc_misc_get_xid_range_request_t
 **/
typedef struct xcb_xc_misc_get_xid_range_request_t {
    uint8_t  major_opcode; /**<  */
    uint8_t  minor_opcode; /**<  */
    uint16_t length; /**<  */
} xcb_xc_misc_get_xid_range_request_t;

/**
 * @brief xcb_xc_misc_get_xid_range_reply_t
 **/
typedef struct xcb_xc_misc_get_xid_range_reply_t {
    uint8_t  response_type; /**<  */
    uint8_t  pad0; /**<  */
    uint16_t sequence; /**<  */
    uint32_t length; /**<  */
    uint32_t start_id; /**<  */
    uint32_t count; /**<  */
} xcb_xc_misc_get_xid_range_reply_t;

/**
 * @brief xcb_xc_misc_get_xid_list_cookie_t
 **/
typedef struct xcb_xc_misc_get_xid_list_cookie_t {
    unsigned int sequence; /**<  */
} xcb_xc_misc_get_xid_list_cookie_t;

/** Opcode for xcb_xc_misc_get_xid_list. */
#define XCB_XC_MISC_GET_XID_LIST 2

/**
 * @brief xcb_xc_misc_get_xid_list_request_t
 **/
typedef struct xcb_xc_misc_get_xid_list_request_t {
    uint8_t  major_opcode; /**<  */
    uint8_t  minor_opcode; /**<  */
    uint16_t length; /**<  */
    uint32_t count; /**<  */
} xcb_xc_misc_get_xid_list_request_t;

/**
 * @brief xcb_xc_misc_get_xid_list_reply_t
 **/
typedef struct xcb_xc_misc_get_xid_list_reply_t {
    uint8_t  response_type; /**<  */
    uint8_t  pad0; /**<  */
    uint16_t sequence; /**<  */
    uint32_t length; /**<  */
    uint32_t ids_len; /**<  */
    uint8_t  pad1[20]; /**<  */
} xcb_xc_misc_get_xid_list_reply_t;


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
                         uint16_t          client_minor_version  /**< */);


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
                                   uint16_t          client_minor_version  /**< */);


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
                               xcb_generic_error_t              **e  /**< */);


/*****************************************************************************
 **
 ** xcb_xc_misc_get_xid_range_cookie_t xcb_xc_misc_get_xid_range
 **
 ** @param xcb_connection_t *c
 ** @returns xcb_xc_misc_get_xid_range_cookie_t
 **
 *****************************************************************************/

xcb_xc_misc_get_xid_range_cookie_t
xcb_xc_misc_get_xid_range (xcb_connection_t *c  /**< */);


/*****************************************************************************
 **
 ** xcb_xc_misc_get_xid_range_cookie_t xcb_xc_misc_get_xid_range_unchecked
 **
 ** @param xcb_connection_t *c
 ** @returns xcb_xc_misc_get_xid_range_cookie_t
 **
 *****************************************************************************/

xcb_xc_misc_get_xid_range_cookie_t
xcb_xc_misc_get_xid_range_unchecked (xcb_connection_t *c  /**< */);


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
                                 xcb_generic_error_t                **e  /**< */);


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
                          uint32_t          count  /**< */);


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
                                    uint32_t          count  /**< */);


/*****************************************************************************
 **
 ** uint32_t * xcb_xc_misc_get_xid_list_ids
 **
 ** @param const xcb_xc_misc_get_xid_list_reply_t *R
 ** @returns uint32_t *
 **
 *****************************************************************************/

uint32_t *
xcb_xc_misc_get_xid_list_ids (const xcb_xc_misc_get_xid_list_reply_t *R  /**< */);


/*****************************************************************************
 **
 ** int xcb_xc_misc_get_xid_list_ids_length
 **
 ** @param const xcb_xc_misc_get_xid_list_reply_t *R
 ** @returns int
 **
 *****************************************************************************/

int
xcb_xc_misc_get_xid_list_ids_length (const xcb_xc_misc_get_xid_list_reply_t *R  /**< */);


/*****************************************************************************
 **
 ** xcb_generic_iterator_t xcb_xc_misc_get_xid_list_ids_end
 **
 ** @param const xcb_xc_misc_get_xid_list_reply_t *R
 ** @returns xcb_generic_iterator_t
 **
 *****************************************************************************/

xcb_generic_iterator_t
xcb_xc_misc_get_xid_list_ids_end (const xcb_xc_misc_get_xid_list_reply_t *R  /**< */);


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
                                xcb_generic_error_t               **e  /**< */);


#endif
