#ifndef _GE_H_
#define _GE_H_

#define GE_NAME         "Generic Event Extension"
#define GE_MAJOR        1
#define GE_MINOR        0

/*********************************************************
 *
 * Requests
 *
 */

#define X_GEQueryVersion        0

#define GENumberRequests       (X_GEQueryVersion + 1)

/*********************************************************
 *
 * Events
 *
 */

#define X_GenericEvent        0

#define GENumberEvents        (X_GenericEvent + 1)

/*********************************************************
 *
 * Errors
 *
 */

#define GENumberErrors        0

#endif /* _GE_H_ */