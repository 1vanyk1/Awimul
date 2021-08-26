#ifndef _XKB_CONFIG_H_
#define _XKB_CONFIG_H_

#include "../../main_wm.h"

/* Default set of XKB rules. */
#define XKB_DFLT_RULES "base"

/* Default XKB model. */
#define	XKB_DFLT_MODEL	"pc102"

/* Default XKB layout. */
#define	XKB_DFLT_LAYOUT	"us"

/* Default XKB variant. */
#define	XKB_DFLT_VARIANT	""

/* Default XKB options. */
#define	XKB_DFLT_OPTIONS	""

/* Path to XKB definitions. */
//#define	XKB_BASE_DIRECTORY	"/usr/X11R6/lib/X11/xkb"
#define	XKB_BASE_DIRECTORY AWIMUL_FILES_PATH "/xkb"

/* Path to xkbcomp. */
#define	XKB_BIN_DIRECTORY	XKB_BASE_DIRECTORY

/* XKB output dir for compiled keymaps. */
#define	XKM_OUTPUT_DIR	AWIMUL_FILES_PATH"/compiled/"

#endif /* _XKB_CONFIG_H_ */
