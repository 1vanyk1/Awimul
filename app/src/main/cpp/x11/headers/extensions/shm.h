#ifndef _SHM_H_
#define _SHM_H_

#define SHMNAME "MIT-SHM"

#define SHM_MAJOR_VERSION	1	/* current version numbers */
#define SHM_MINOR_VERSION	2

#define ShmCompletion			0
#define ShmNumberEvents			(ShmCompletion + 1)

#define BadShmSeg			0
#define ShmNumberErrors			(BadShmSeg + 1)


#endif /* _SHM_H_ */