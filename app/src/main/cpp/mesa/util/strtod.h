#ifndef STRTOD_H
#define STRTOD_H

#ifdef __cplusplus
extern "C" {
#endif

extern void
_mesa_locale_init(void);

extern void
_mesa_locale_fini(void);

extern double
_mesa_strtod(const char *s, char **end);

extern float
_mesa_strtof(const char *s, char **end);


#ifdef __cplusplus
}
#endif


#endif
