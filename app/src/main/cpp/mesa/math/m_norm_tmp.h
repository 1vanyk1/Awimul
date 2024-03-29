static void
TAG(transform_normalize_normals)( const GLmatrix *mat,
                                  GLfloat scale,
                                  const GLvector4f *in,
                                  const GLfloat *lengths,
                                  GLvector4f *dest )
{
    GLfloat (*out)[4] = (GLfloat (*)[4])dest->start;
    const GLfloat *from = in->start;
    const GLuint stride = in->stride;
    const GLuint count = in->count;
    const GLfloat *m = mat->inv;
    GLfloat m0 = m[0],  m4 = m[4],  m8 = m[8];
    GLfloat m1 = m[1],  m5 = m[5],  m9 = m[9];
    GLfloat m2 = m[2],  m6 = m[6],  m10 = m[10];
    GLuint i;

    if (!lengths) {
        STRIDE_LOOP {
                GLfloat tx, ty, tz;
                {
                    const GLfloat ux = from[0],  uy = from[1],  uz = from[2];
                    tx = ux * m0 + uy * m1 + uz * m2;
                    ty = ux * m4 + uy * m5 + uz * m6;
                    tz = ux * m8 + uy * m9 + uz * m10;
                }
                {
                    GLdouble len = tx*tx + ty*ty + tz*tz;
                    if (len > 1e-20) {
                        GLfloat scale = 1.0f / sqrtf(len);
                        out[i][0] = tx * scale;
                        out[i][1] = ty * scale;
                        out[i][2] = tz * scale;
                    }
                    else {
                        out[i][0] = out[i][1] = out[i][2] = 0;
                    }
                }
        }
    }
    else {
        if (scale != 1.0f) {
            m0 *= scale,  m4 *= scale,  m8 *= scale;
            m1 *= scale,  m5 *= scale,  m9 *= scale;
            m2 *= scale,  m6 *= scale,  m10 *= scale;
        }

        STRIDE_LOOP {
                GLfloat tx, ty, tz;
                {
                    const GLfloat ux = from[0],  uy = from[1],  uz = from[2];
                    tx = ux * m0 + uy * m1 + uz * m2;
                    ty = ux * m4 + uy * m5 + uz * m6;
                    tz = ux * m8 + uy * m9 + uz * m10;
                }
                {
                    GLfloat len = lengths[i];
                    out[i][0] = tx * len;
                    out[i][1] = ty * len;
                    out[i][2] = tz * len;
                }
        }
    }
    dest->count = in->count;
}


static void
TAG(transform_normalize_normals_no_rot)( const GLmatrix *mat,
                                         GLfloat scale,
                                         const GLvector4f *in,
                                         const GLfloat *lengths,
                                         GLvector4f *dest )
{
    GLfloat (*out)[4] = (GLfloat (*)[4])dest->start;
    const GLfloat *from = in->start;
    const GLuint stride = in->stride;
    const GLuint count = in->count;
    const GLfloat *m = mat->inv;
    GLfloat m0 = m[0];
    GLfloat m5 = m[5];
    GLfloat m10 = m[10];
    GLuint i;

    if (!lengths) {
        STRIDE_LOOP {
                GLfloat tx, ty, tz;
                {
                    const GLfloat ux = from[0],  uy = from[1],  uz = from[2];
                    tx = ux * m0                    ;
                    ty =           uy * m5          ;
                    tz =                     uz * m10;
                }
                {
                    GLdouble len = tx*tx + ty*ty + tz*tz;
                    if (len > 1e-20) {
                        GLfloat scale = 1.0f / sqrtf(len);
                        out[i][0] = tx * scale;
                        out[i][1] = ty * scale;
                        out[i][2] = tz * scale;
                    }
                    else {
                        out[i][0] = out[i][1] = out[i][2] = 0;
                    }
                }
        }
    }
    else {
        m0 *= scale;
        m5 *= scale;
        m10 *= scale;

        STRIDE_LOOP {
                GLfloat tx, ty, tz;
                {
                    const GLfloat ux = from[0],  uy = from[1],  uz = from[2];
                    tx = ux * m0                    ;
                    ty =           uy * m5          ;
                    tz =                     uz * m10;
                }
                {
                    GLfloat len = lengths[i];
                    out[i][0] = tx * len;
                    out[i][1] = ty * len;
                    out[i][2] = tz * len;
                }
        }
    }
    dest->count = in->count;
}


static void
TAG(transform_rescale_normals_no_rot)( const GLmatrix *mat,
                                       GLfloat scale,
                                       const GLvector4f *in,
                                       const GLfloat *lengths,
                                       GLvector4f *dest )
{
    GLfloat (*out)[4] = (GLfloat (*)[4])dest->start;
    const GLfloat *from = in->start;
    const GLuint stride = in->stride;
    const GLuint count = in->count;
    const GLfloat *m = mat->inv;
    const GLfloat m0 = scale*m[0];
    const GLfloat m5 = scale*m[5];
    const GLfloat m10 = scale*m[10];
    GLuint i;

    (void) lengths;

    STRIDE_LOOP {
            GLfloat ux = from[0],  uy = from[1],  uz = from[2];
            out[i][0] = ux * m0;
            out[i][1] =           uy * m5;
            out[i][2] =                     uz * m10;
    }
    dest->count = in->count;
}


static void
TAG(transform_rescale_normals)( const GLmatrix *mat,
                                GLfloat scale,
                                const GLvector4f *in,
                                const GLfloat *lengths,
                                GLvector4f *dest )
{
    GLfloat (*out)[4] = (GLfloat (*)[4])dest->start;
    const GLfloat *from = in->start;
    const GLuint stride = in->stride;
    const GLuint count = in->count;
    /* Since we are unlikely to have < 3 vertices in the buffer,
     * it makes sense to pre-multiply by scale.
     */
    const GLfloat *m = mat->inv;
    const GLfloat m0 = scale*m[0],  m4 = scale*m[4],  m8 = scale*m[8];
    const GLfloat m1 = scale*m[1],  m5 = scale*m[5],  m9 = scale*m[9];
    const GLfloat m2 = scale*m[2],  m6 = scale*m[6],  m10 = scale*m[10];
    GLuint i;

    (void) lengths;

    STRIDE_LOOP {
            GLfloat ux = from[0],  uy = from[1],  uz = from[2];
            out[i][0] = ux * m0 + uy * m1 + uz * m2;
            out[i][1] = ux * m4 + uy * m5 + uz * m6;
            out[i][2] = ux * m8 + uy * m9 + uz * m10;
    }
    dest->count = in->count;
}


static void
TAG(transform_normals_no_rot)( const GLmatrix *mat,
                               GLfloat scale,
                               const GLvector4f *in,
                               const GLfloat *lengths,
                               GLvector4f *dest )
{
    GLfloat (*out)[4] = (GLfloat (*)[4])dest->start;
    const GLfloat *from = in->start;
    const GLuint stride = in->stride;
    const GLuint count = in->count;
    const GLfloat *m = mat->inv;
    const GLfloat m0 = m[0];
    const GLfloat m5 = m[5];
    const GLfloat m10 = m[10];
    GLuint i;

    (void) scale;
    (void) lengths;

    STRIDE_LOOP {
            GLfloat ux = from[0],  uy = from[1],  uz = from[2];
            out[i][0] = ux * m0;
            out[i][1] =           uy * m5;
            out[i][2] =                     uz * m10;
    }
    dest->count = in->count;
}


static void
TAG(transform_normals)( const GLmatrix *mat,
                        GLfloat scale,
                        const GLvector4f *in,
                        const GLfloat *lengths,
                        GLvector4f *dest )
{
    GLfloat (*out)[4] = (GLfloat (*)[4])dest->start;
    const GLfloat *from = in->start;
    const GLuint stride = in->stride;
    const GLuint count = in->count;
    const GLfloat *m = mat->inv;
    const GLfloat m0 = m[0],  m4 = m[4],  m8 = m[8];
    const GLfloat m1 = m[1],  m5 = m[5],  m9 = m[9];
    const GLfloat m2 = m[2],  m6 = m[6],  m10 = m[10];
    GLuint i;

    (void) scale;
    (void) lengths;

    STRIDE_LOOP {
            GLfloat ux = from[0],  uy = from[1],  uz = from[2];
            out[i][0] = ux * m0 + uy * m1 + uz * m2;
            out[i][1] = ux * m4 + uy * m5 + uz * m6;
            out[i][2] = ux * m8 + uy * m9 + uz * m10;
    }
    dest->count = in->count;
}


static void
TAG(normalize_normals)( const GLmatrix *mat,
                        GLfloat scale,
                        const GLvector4f *in,
                        const GLfloat *lengths,
                        GLvector4f *dest )
{
    GLfloat (*out)[4] = (GLfloat (*)[4])dest->start;
    const GLfloat *from = in->start;
    const GLuint stride = in->stride;
    const GLuint count = in->count;
    GLuint i;

    (void) mat;
    (void) scale;

    if (lengths) {
        STRIDE_LOOP {
                const GLfloat x = from[0], y = from[1], z = from[2];
                GLfloat invlen = lengths[i];
                out[i][0] = x * invlen;
                out[i][1] = y * invlen;
                out[i][2] = z * invlen;
        }
    }
    else {
        STRIDE_LOOP {
                const GLfloat x = from[0], y = from[1], z = from[2];
                GLdouble len = x * x + y * y + z * z;
                if (len > 1e-50) {
                    len = 1.0f / sqrtf(len);
                    out[i][0] = (GLfloat)(x * len);
                    out[i][1] = (GLfloat)(y * len);
                    out[i][2] = (GLfloat)(z * len);
                }
                else {
                    out[i][0] = x;
                    out[i][1] = y;
                    out[i][2] = z;
                }
        }
    }
    dest->count = in->count;
}


static void
TAG(rescale_normals)( const GLmatrix *mat,
                      GLfloat scale,
                      const GLvector4f *in,
                      const GLfloat *lengths,
                      GLvector4f *dest )
{
    GLfloat (*out)[4] = (GLfloat (*)[4])dest->start;
    const GLfloat *from = in->start;
    const GLuint stride = in->stride;
    const GLuint count = in->count;
    GLuint i;

    (void) mat;
    (void) lengths;

    STRIDE_LOOP {
            SCALE_SCALAR_3V( out[i], scale, from );
    }
    dest->count = in->count;
}


static void
TAG(init_c_norm_transform)( void )
{
    _mesa_normal_tab[NORM_TRANSFORM_NO_ROT] =
            TAG(transform_normals_no_rot);

    _mesa_normal_tab[NORM_TRANSFORM_NO_ROT | NORM_RESCALE] =
            TAG(transform_rescale_normals_no_rot);

    _mesa_normal_tab[NORM_TRANSFORM_NO_ROT | NORM_NORMALIZE] =
            TAG(transform_normalize_normals_no_rot);

    _mesa_normal_tab[NORM_TRANSFORM] =
            TAG(transform_normals);

    _mesa_normal_tab[NORM_TRANSFORM | NORM_RESCALE] =
            TAG(transform_rescale_normals);

    _mesa_normal_tab[NORM_TRANSFORM | NORM_NORMALIZE] =
            TAG(transform_normalize_normals);

    _mesa_normal_tab[NORM_RESCALE] =
            TAG(rescale_normals);

    _mesa_normal_tab[NORM_NORMALIZE] =
            TAG(normalize_normals);
}
