#ifndef _DAMAGEWIRE_H_
#define _DAMAGEWIRE_H_

#define	DAMAGE_NAME	"DAMAGE"
#define DAMAGE_MAJOR	1
#define DAMAGE_MINOR	1

/************* Version 1 ****************/

/* Constants */
#define XDamageReportRawRectangles	0
#define XDamageReportDeltaRectangles	1
#define XDamageReportBoundingBox	2
#define XDamageReportNonEmpty		3

/* Requests */
#define X_DamageQueryVersion		0
#define X_DamageCreate			1
#define X_DamageDestroy			2
#define X_DamageSubtract		3
#define X_DamageAdd			4

#define XDamageNumberRequests		(X_DamageAdd + 1)

/* Events */
#define XDamageNotify			0

#define XDamageNumberEvents		(XDamageNotify + 1)

/* Errors */
#define BadDamage			0
#define XDamageNumberErrors		(BadDamage + 1)

#endif /* _DAMAGEWIRE_H_ */
