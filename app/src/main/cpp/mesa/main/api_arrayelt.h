#ifndef API_ARRAYELT_H
#define API_ARRAYELT_H

#include <stdbool.h>
#include "dd.h"

struct _glapi_table;

extern void _mesa_array_element(struct gl_context *ctx, GLint elt);
extern void GLAPIENTRY _ae_ArrayElement( GLint elt );

extern void
_mesa_install_arrayelt_vtxfmt(struct _glapi_table *disp,
                              const GLvertexformat *vfmt);


#endif /* API_ARRAYELT_H */
