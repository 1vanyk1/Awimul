#ifndef _Xmu_Misc_h
#define _Xmu_Misc_h

#define MAXDIMENSION	((1 << 31)-1)

#define Max(x, y)	(((x) > (y)) ? (x) : (y))
#define Min(x, y)	(((x) < (y)) ? (x) : (y))
#define AssignMax(x, y)	{if ((y) > (x)) x = (y);}
#define AssignMin(x, y)	{if ((y) < (x)) x = (y);}

#endif /*_Xmu_Misc_h*/

