#ifndef _RECORDCONST_H_
#define _RECORDCONST_H_

#define RECORD_NAME			"RECORD"
#define RECORD_MAJOR_VERSION		1
#define RECORD_MINOR_VERSION		13
#define RECORD_LOWEST_MAJOR_VERSION	1
#define RECORD_LOWEST_MINOR_VERSION	12

#define XRecordBadContext       0	/* Not a valid RC */

#define RecordNumErrors         (XRecordBadContext + 1)
#define RecordNumEvents		0L

/*
 * Constants for arguments of various requests
 */
#define	XRecordFromServerTime		0x01
#define	XRecordFromClientTime		0x02
#define	XRecordFromClientSequence	0x04

#define XRecordCurrentClients		1
#define XRecordFutureClients		2
#define XRecordAllClients		3

#define XRecordFromServer           	0
#define XRecordFromClient               1
#define XRecordClientStarted           	2
#define XRecordClientDied               3
#define XRecordStartOfData		4
#define XRecordEndOfData		5


#endif /* _RECORD_H_ */
