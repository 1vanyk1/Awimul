#ifndef PIXMAN_VERSION_H__
#define PIXMAN_VERSION_H__

#ifndef PIXMAN_H__
#  error pixman-version.h should only be included by pixman.h
#endif

#define PIXMAN_VERSION_MAJOR 1
#define PIXMAN_VERSION_MINOR 1
#define PIXMAN_VERSION_MICRO 2

#define PIXMAN_VERSION_STRING "@PIXMAN_VERSION_MAJOR@.@PIXMAN_VERSION_MINOR@.@PIXMAN_VERSION_MICRO@"

#define PIXMAN_VERSION_ENCODE(major, minor, micro) (	\
	  ((major) * 10000)				\
	+ ((minor) *   100)				\
	+ ((micro) *     1))

#define PIXMAN_VERSION PIXMAN_VERSION_ENCODE(	\
	PIXMAN_VERSION_MAJOR,			\
	PIXMAN_VERSION_MINOR,			\
	PIXMAN_VERSION_MICRO)

#ifndef PIXMAN_API
# define PIXMAN_API
#endif


#endif /* PIXMAN_VERSION_H__ */