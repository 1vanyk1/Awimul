static void
TAG(transform_points1_general)( GLvector4f *to_vec,
                                const GLfloat m[16],
                                const GLvector4f *from_vec )
{
    const GLuint stride = from_vec->stride;
    GLfloat *from = from_vec->start;
    GLfloat (*to)[4] = (GLfloat (*)[4])to_vec->start;
    GLuint count = from_vec->count;
    const GLfloat m0 = m[0],  m12 = m[12];
    const GLfloat m1 = m[1],  m13 = m[13];
    const GLfloat m2 = m[2],  m14 = m[14];
    const GLfloat m3 = m[3],  m15 = m[15];
    GLuint i;
    STRIDE_LOOP {
            const GLfloat ox = from[0];
            to[i][0] = m0 * ox + m12;
            to[i][1] = m1 * ox + m13;
            to[i][2] = m2 * ox + m14;
            to[i][3] = m3 * ox + m15;
    }
    to_vec->size = 4;
    to_vec->flags |= VEC_SIZE_4;
    to_vec->count = from_vec->count;
}

static void
TAG(transform_points1_identity)( GLvector4f *to_vec,
                                 const GLfloat m[16],
                                 const GLvector4f *from_vec )
{
    const GLuint stride = from_vec->stride;
    GLfloat *from = from_vec->start;
    GLuint count = from_vec->count;
    GLfloat (*to)[4] = (GLfloat (*)[4])to_vec->start;
    GLuint i;
    (void) m;
    if (to_vec == from_vec) return;
    STRIDE_LOOP {
            to[i][0] = from[0];
    }
    to_vec->size = 1;
    to_vec->flags |= VEC_SIZE_1;
    to_vec->count = from_vec->count;
}

static void
TAG(transform_points1_2d)( GLvector4f *to_vec,
                           const GLfloat m[16],
                           const GLvector4f *from_vec )
{
    const GLuint stride = from_vec->stride;
    GLfloat *from = from_vec->start;
    GLfloat (*to)[4] = (GLfloat (*)[4])to_vec->start;
    GLuint count = from_vec->count;
    const GLfloat m0 = m[0], m1 = m[1];
    const GLfloat m12 = m[12], m13 = m[13];
    GLuint i;
    STRIDE_LOOP {
            const GLfloat ox = from[0];
            to[i][0] = m0 * ox + m12;
            to[i][1] = m1 * ox + m13;
    }
    to_vec->size = 2;
    to_vec->flags |= VEC_SIZE_2;
    to_vec->count = from_vec->count;
}

static void
TAG(transform_points1_2d_no_rot)( GLvector4f *to_vec,
                                  const GLfloat m[16],
                                  const GLvector4f *from_vec )
{
    const GLuint stride = from_vec->stride;
    GLfloat *from = from_vec->start;
    GLfloat (*to)[4] = (GLfloat (*)[4])to_vec->start;
    GLuint count = from_vec->count;
    const GLfloat m0 = m[0], m12 = m[12], m13 = m[13];
    GLuint i;
    STRIDE_LOOP {
            const GLfloat ox = from[0];
            to[i][0] = m0 * ox + m12;
            to[i][1] =           m13;
    }
    to_vec->size = 2;
    to_vec->flags |= VEC_SIZE_2;
    to_vec->count = from_vec->count;
}

static void
TAG(transform_points1_3d)( GLvector4f *to_vec,
                           const GLfloat m[16],
                           const GLvector4f *from_vec )
{
    const GLuint stride = from_vec->stride;
    GLfloat *from = from_vec->start;
    GLfloat (*to)[4] = (GLfloat (*)[4])to_vec->start;
    GLuint count = from_vec->count;
    const GLfloat m0 = m[0], m1 = m[1], m2 = m[2];
    const GLfloat m12 = m[12], m13 = m[13], m14 = m[14];
    GLuint i;
    STRIDE_LOOP {
            const GLfloat ox = from[0];
            to[i][0] = m0 * ox + m12;
            to[i][1] = m1 * ox + m13;
            to[i][2] = m2 * ox + m14;
    }
    to_vec->size = 3;
    to_vec->flags |= VEC_SIZE_3;
    to_vec->count = from_vec->count;
}


static void
TAG(transform_points1_3d_no_rot)( GLvector4f *to_vec,
                                  const GLfloat m[16],
                                  const GLvector4f *from_vec )
{
    const GLuint stride = from_vec->stride;
    GLfloat *from = from_vec->start;
    GLfloat (*to)[4] = (GLfloat (*)[4])to_vec->start;
    GLuint count = from_vec->count;
    const GLfloat m0 = m[0];
    const GLfloat m12 = m[12], m13 = m[13], m14 = m[14];
    GLuint i;
    STRIDE_LOOP {
            const GLfloat ox = from[0];
            to[i][0] = m0 * ox           + m12;
            to[i][1] =                     m13;
            to[i][2] =                     m14;
    }
    to_vec->size = 3;
    to_vec->flags |= VEC_SIZE_3;
    to_vec->count = from_vec->count;
}

static void
TAG(transform_points1_perspective)( GLvector4f *to_vec,
                                    const GLfloat m[16],
                                    const GLvector4f *from_vec )
{
    const GLuint stride = from_vec->stride;
    GLfloat *from = from_vec->start;
    GLfloat (*to)[4] = (GLfloat (*)[4])to_vec->start;
    GLuint count = from_vec->count;
    const GLfloat m0 = m[0], m14 = m[14];
    GLuint i;
    STRIDE_LOOP {
            const GLfloat ox = from[0];
            to[i][0] = m0 * ox                ;
            to[i][1] =           0            ;
            to[i][2] =                     m14;
            to[i][3] = 0;
    }
    to_vec->size = 4;
    to_vec->flags |= VEC_SIZE_4;
    to_vec->count = from_vec->count;
}




/* 2-vectors, which are a lot more relevant than 1-vectors, are
 * present early in the geometry pipeline and throughout the
 * texture pipeline.
 */
static void
TAG(transform_points2_general)( GLvector4f *to_vec,
                                const GLfloat m[16],
                                const GLvector4f *from_vec )
{
    const GLuint stride = from_vec->stride;
    GLfloat *from = from_vec->start;
    GLfloat (*to)[4] = (GLfloat (*)[4])to_vec->start;
    GLuint count = from_vec->count;
    const GLfloat m0 = m[0],  m4 = m[4],  m12 = m[12];
    const GLfloat m1 = m[1],  m5 = m[5],  m13 = m[13];
    const GLfloat m2 = m[2],  m6 = m[6],  m14 = m[14];
    const GLfloat m3 = m[3],  m7 = m[7],  m15 = m[15];
    GLuint i;
    STRIDE_LOOP {
            const GLfloat ox = from[0], oy = from[1];
            to[i][0] = m0 * ox + m4 * oy + m12;
            to[i][1] = m1 * ox + m5 * oy + m13;
            to[i][2] = m2 * ox + m6 * oy + m14;
            to[i][3] = m3 * ox + m7 * oy + m15;
    }
    to_vec->size = 4;
    to_vec->flags |= VEC_SIZE_4;
    to_vec->count = from_vec->count;
}

static void
TAG(transform_points2_identity)( GLvector4f *to_vec,
                                 const GLfloat m[16],
                                 const GLvector4f *from_vec )
{
    const GLuint stride = from_vec->stride;
    GLfloat *from = from_vec->start;
    GLfloat (*to)[4] = (GLfloat (*)[4])to_vec->start;
    GLuint count = from_vec->count;
    GLuint i;
    (void) m;
    if (to_vec == from_vec) return;
    STRIDE_LOOP {
            to[i][0] = from[0];
            to[i][1] = from[1];
    }
    to_vec->size = 2;
    to_vec->flags |= VEC_SIZE_2;
    to_vec->count = from_vec->count;
}

static void
TAG(transform_points2_2d)( GLvector4f *to_vec,
                           const GLfloat m[16],
                           const GLvector4f *from_vec )
{
    const GLuint stride = from_vec->stride;
    GLfloat *from = from_vec->start;
    GLfloat (*to)[4] = (GLfloat (*)[4])to_vec->start;
    GLuint count = from_vec->count;
    const GLfloat m0 = m[0], m1 = m[1], m4 = m[4], m5 = m[5];
    const GLfloat m12 = m[12], m13 = m[13];
    GLuint i;
    STRIDE_LOOP {
            const GLfloat ox = from[0], oy = from[1];
            to[i][0] = m0 * ox + m4 * oy + m12;
            to[i][1] = m1 * ox + m5 * oy + m13;
    }
    to_vec->size = 2;
    to_vec->flags |= VEC_SIZE_2;
    to_vec->count = from_vec->count;
}

static void
TAG(transform_points2_2d_no_rot)( GLvector4f *to_vec,
                                  const GLfloat m[16],
                                  const GLvector4f *from_vec )
{
    const GLuint stride = from_vec->stride;
    GLfloat *from = from_vec->start;
    GLfloat (*to)[4] = (GLfloat (*)[4])to_vec->start;
    GLuint count = from_vec->count;
    const GLfloat m0 = m[0], m5 = m[5], m12 = m[12], m13 = m[13];
    GLuint i;
    STRIDE_LOOP {
            const GLfloat ox = from[0], oy = from[1];
            to[i][0] = m0 * ox           + m12;
            to[i][1] =           m5 * oy + m13;
    }
    to_vec->size = 2;
    to_vec->flags |= VEC_SIZE_2;
    to_vec->count = from_vec->count;
}

static void
TAG(transform_points2_3d)( GLvector4f *to_vec,
                           const GLfloat m[16],
                           const GLvector4f *from_vec )
{
    const GLuint stride = from_vec->stride;
    GLfloat *from = from_vec->start;
    GLfloat (*to)[4] = (GLfloat (*)[4])to_vec->start;
    GLuint count = from_vec->count;
    const GLfloat m0 = m[0], m1 = m[1], m2 = m[2], m4 = m[4], m5 = m[5];
    const GLfloat m6 = m[6], m12 = m[12], m13 = m[13], m14 = m[14];
    GLuint i;
    STRIDE_LOOP {
            const GLfloat ox = from[0], oy = from[1];
            to[i][0] = m0 * ox + m4 * oy + m12;
            to[i][1] = m1 * ox + m5 * oy + m13;
            to[i][2] = m2 * ox + m6 * oy + m14;
    }
    to_vec->size = 3;
    to_vec->flags |= VEC_SIZE_3;
    to_vec->count = from_vec->count;
}


/* I would actually say this was a fairly important function, from
 * a texture transformation point of view.
 */
static void
TAG(transform_points2_3d_no_rot)( GLvector4f *to_vec,
                                  const GLfloat m[16],
                                  const GLvector4f *from_vec )
{
    const GLuint stride = from_vec->stride;
    GLfloat *from = from_vec->start;
    GLfloat (*to)[4] = (GLfloat (*)[4])to_vec->start;
    GLuint count = from_vec->count;
    const GLfloat m0 = m[0], m5 = m[5];
    const GLfloat m12 = m[12], m13 = m[13], m14 = m[14];
    GLuint i;
    STRIDE_LOOP {
            const GLfloat ox = from[0], oy = from[1];
            to[i][0] = m0 * ox           + m12;
            to[i][1] =           m5 * oy + m13;
            to[i][2] =                     m14;
    }
    if (m14 == 0) {
        to_vec->size = 2;
        to_vec->flags |= VEC_SIZE_2;
    } else {
        to_vec->size = 3;
        to_vec->flags |= VEC_SIZE_3;
    }
    to_vec->count = from_vec->count;
}


static void
TAG(transform_points2_perspective)( GLvector4f *to_vec,
                                    const GLfloat m[16],
                                    const GLvector4f *from_vec )
{
    const GLuint stride = from_vec->stride;
    GLfloat *from = from_vec->start;
    GLfloat (*to)[4] = (GLfloat (*)[4])to_vec->start;
    GLuint count = from_vec->count;
    const GLfloat m0 = m[0], m5 = m[5], m14 = m[14];
    GLuint i;
    STRIDE_LOOP {
            const GLfloat ox = from[0], oy = from[1];
            to[i][0] = m0 * ox                ;
            to[i][1] =           m5 * oy      ;
            to[i][2] =                     m14;
            to[i][3] = 0;
    }
    to_vec->size = 4;
    to_vec->flags |= VEC_SIZE_4;
    to_vec->count = from_vec->count;
}



static void
TAG(transform_points3_general)( GLvector4f *to_vec,
                                const GLfloat m[16],
                                const GLvector4f *from_vec )
{
    const GLuint stride = from_vec->stride;
    GLfloat *from = from_vec->start;
    GLfloat (*to)[4] = (GLfloat (*)[4])to_vec->start;
    GLuint count = from_vec->count;
    const GLfloat m0 = m[0],  m4 = m[4],  m8 = m[8],  m12 = m[12];
    const GLfloat m1 = m[1],  m5 = m[5],  m9 = m[9],  m13 = m[13];
    const GLfloat m2 = m[2],  m6 = m[6],  m10 = m[10],  m14 = m[14];
    const GLfloat m3 = m[3],  m7 = m[7],  m11 = m[11],  m15 = m[15];
    GLuint i;
    STRIDE_LOOP {
            const GLfloat ox = from[0], oy = from[1], oz = from[2];
            to[i][0] = m0 * ox + m4 * oy + m8  * oz + m12;
            to[i][1] = m1 * ox + m5 * oy + m9  * oz + m13;
            to[i][2] = m2 * ox + m6 * oy + m10 * oz + m14;
            to[i][3] = m3 * ox + m7 * oy + m11 * oz + m15;
    }
    to_vec->size = 4;
    to_vec->flags |= VEC_SIZE_4;
    to_vec->count = from_vec->count;
}

static void
TAG(transform_points3_identity)( GLvector4f *to_vec,
                                 const GLfloat m[16],
                                 const GLvector4f *from_vec )
{
    const GLuint stride = from_vec->stride;
    GLfloat *from = from_vec->start;
    GLfloat (*to)[4] = (GLfloat (*)[4])to_vec->start;
    GLuint count = from_vec->count;
    GLuint i;
    (void) m;
    if (to_vec == from_vec) return;
    STRIDE_LOOP {
            to[i][0] = from[0];
            to[i][1] = from[1];
            to[i][2] = from[2];
    }
    to_vec->size = 3;
    to_vec->flags |= VEC_SIZE_3;
    to_vec->count = from_vec->count;
}

static void
TAG(transform_points3_2d)( GLvector4f *to_vec,
                           const GLfloat m[16],
                           const GLvector4f *from_vec )
{
    const GLuint stride = from_vec->stride;
    GLfloat *from = from_vec->start;
    GLfloat (*to)[4] = (GLfloat (*)[4])to_vec->start;
    GLuint count = from_vec->count;
    const GLfloat m0 = m[0], m1 = m[1], m4 = m[4], m5 = m[5];
    const GLfloat m12 = m[12], m13 = m[13];
    GLuint i;
    STRIDE_LOOP {
            const GLfloat ox = from[0], oy = from[1], oz = from[2];
            to[i][0] = m0 * ox + m4 * oy            + m12       ;
            to[i][1] = m1 * ox + m5 * oy            + m13       ;
            to[i][2] =                   +       oz             ;
    }
    to_vec->size = 3;
    to_vec->flags |= VEC_SIZE_3;
    to_vec->count = from_vec->count;
}

static void
TAG(transform_points3_2d_no_rot)( GLvector4f *to_vec,
                                  const GLfloat m[16],
                                  const GLvector4f *from_vec )
{
    const GLuint stride = from_vec->stride;
    GLfloat *from = from_vec->start;
    GLfloat (*to)[4] = (GLfloat (*)[4])to_vec->start;
    GLuint count = from_vec->count;
    const GLfloat m0 = m[0], m5 = m[5], m12 = m[12], m13 = m[13];
    GLuint i;
    STRIDE_LOOP {
            const GLfloat ox = from[0], oy = from[1], oz = from[2];
            to[i][0] = m0 * ox                      + m12       ;
            to[i][1] =           m5 * oy            + m13       ;
            to[i][2] =                   +       oz             ;
    }
    to_vec->size = 3;
    to_vec->flags |= VEC_SIZE_3;
    to_vec->count = from_vec->count;
}

static void
TAG(transform_points3_3d)( GLvector4f *to_vec,
                           const GLfloat m[16],
                           const GLvector4f *from_vec )
{
    const GLuint stride = from_vec->stride;
    GLfloat *from = from_vec->start;
    GLfloat (*to)[4] = (GLfloat (*)[4])to_vec->start;
    GLuint count = from_vec->count;
    const GLfloat m0 = m[0], m1 = m[1], m2 = m[2], m4 = m[4], m5 = m[5];
    const GLfloat m6 = m[6], m8 = m[8], m9 = m[9], m10 = m[10];
    const GLfloat m12 = m[12], m13 = m[13], m14 = m[14];
    GLuint i;
    STRIDE_LOOP {
            const GLfloat ox = from[0], oy = from[1], oz = from[2];
            to[i][0] = m0 * ox + m4 * oy +  m8 * oz + m12       ;
            to[i][1] = m1 * ox + m5 * oy +  m9 * oz + m13       ;
            to[i][2] = m2 * ox + m6 * oy + m10 * oz + m14       ;
    }
    to_vec->size = 3;
    to_vec->flags |= VEC_SIZE_3;
    to_vec->count = from_vec->count;
}

/* previously known as ortho...
 */
static void
TAG(transform_points3_3d_no_rot)( GLvector4f *to_vec,
                                  const GLfloat m[16],
                                  const GLvector4f *from_vec )
{
    const GLuint stride = from_vec->stride;
    GLfloat *from = from_vec->start;
    GLfloat (*to)[4] = (GLfloat (*)[4])to_vec->start;
    GLuint count = from_vec->count;
    const GLfloat m0 = m[0], m5 = m[5];
    const GLfloat m10 = m[10], m12 = m[12], m13 = m[13], m14 = m[14];
    GLuint i;
    STRIDE_LOOP {
            const GLfloat ox = from[0], oy = from[1], oz = from[2];
            to[i][0] = m0 * ox                      + m12       ;
            to[i][1] =           m5 * oy            + m13       ;
            to[i][2] =                     m10 * oz + m14       ;
    }
    to_vec->size = 3;
    to_vec->flags |= VEC_SIZE_3;
    to_vec->count = from_vec->count;
}

static void
TAG(transform_points3_perspective)( GLvector4f *to_vec,
                                    const GLfloat m[16],
                                    const GLvector4f *from_vec )
{
    const GLuint stride = from_vec->stride;
    GLfloat *from = from_vec->start;
    GLfloat (*to)[4] = (GLfloat (*)[4])to_vec->start;
    GLuint count = from_vec->count;
    const GLfloat m0 = m[0], m5 = m[5], m8 = m[8], m9 = m[9];
    const GLfloat m10 = m[10], m14 = m[14];
    GLuint i;
    STRIDE_LOOP {
            const GLfloat ox = from[0], oy = from[1], oz = from[2];
            to[i][0] = m0 * ox           + m8  * oz       ;
            to[i][1] =           m5 * oy + m9  * oz       ;
            to[i][2] =                     m10 * oz + m14 ;
            to[i][3] =                          -oz       ;
    }
    to_vec->size = 4;
    to_vec->flags |= VEC_SIZE_4;
    to_vec->count = from_vec->count;
}



static void
TAG(transform_points4_general)( GLvector4f *to_vec,
                                const GLfloat m[16],
                                const GLvector4f *from_vec )
{
    const GLuint stride = from_vec->stride;
    GLfloat *from = from_vec->start;
    GLfloat (*to)[4] = (GLfloat (*)[4])to_vec->start;
    GLuint count = from_vec->count;
    const GLfloat m0 = m[0],  m4 = m[4],  m8 = m[8],  m12 = m[12];
    const GLfloat m1 = m[1],  m5 = m[5],  m9 = m[9],  m13 = m[13];
    const GLfloat m2 = m[2],  m6 = m[6],  m10 = m[10],  m14 = m[14];
    const GLfloat m3 = m[3],  m7 = m[7],  m11 = m[11],  m15 = m[15];
    GLuint i;
    STRIDE_LOOP {
            const GLfloat ox = from[0], oy = from[1], oz = from[2], ow = from[3];
            to[i][0] = m0 * ox + m4 * oy + m8  * oz + m12 * ow;
            to[i][1] = m1 * ox + m5 * oy + m9  * oz + m13 * ow;
            to[i][2] = m2 * ox + m6 * oy + m10 * oz + m14 * ow;
            to[i][3] = m3 * ox + m7 * oy + m11 * oz + m15 * ow;
    }
    to_vec->size = 4;
    to_vec->flags |= VEC_SIZE_4;
    to_vec->count = from_vec->count;
}

static void
TAG(transform_points4_identity)( GLvector4f *to_vec,
                                 const GLfloat m[16],
                                 const GLvector4f *from_vec )
{
    const GLuint stride = from_vec->stride;
    GLfloat *from = from_vec->start;
    GLfloat (*to)[4] = (GLfloat (*)[4])to_vec->start;
    GLuint count = from_vec->count;
    GLuint i;
    (void) m;
    if (to_vec == from_vec) return;
    STRIDE_LOOP {
            to[i][0] = from[0];
            to[i][1] = from[1];
            to[i][2] = from[2];
            to[i][3] = from[3];
    }
    to_vec->size = 4;
    to_vec->flags |= VEC_SIZE_4;
    to_vec->count = from_vec->count;
}

static void
TAG(transform_points4_2d)( GLvector4f *to_vec,
                           const GLfloat m[16],
                           const GLvector4f *from_vec )
{
    const GLuint stride = from_vec->stride;
    GLfloat *from = from_vec->start;
    GLfloat (*to)[4] = (GLfloat (*)[4])to_vec->start;
    GLuint count = from_vec->count;
    const GLfloat m0 = m[0], m1 = m[1], m4 = m[4], m5 = m[5];
    const GLfloat m12 = m[12], m13 = m[13];
    GLuint i;
    STRIDE_LOOP {
            const GLfloat ox = from[0], oy = from[1], oz = from[2], ow = from[3];
            to[i][0] = m0 * ox + m4 * oy            + m12 * ow;
            to[i][1] = m1 * ox + m5 * oy            + m13 * ow;
            to[i][2] =                   +       oz           ;
            to[i][3] =                                      ow;
    }
    to_vec->size = 4;
    to_vec->flags |= VEC_SIZE_4;
    to_vec->count = from_vec->count;
}

static void
TAG(transform_points4_2d_no_rot)( GLvector4f *to_vec,
                                  const GLfloat m[16],
                                  const GLvector4f *from_vec )
{
    const GLuint stride = from_vec->stride;
    GLfloat *from = from_vec->start;
    GLfloat (*to)[4] = (GLfloat (*)[4])to_vec->start;
    GLuint count = from_vec->count;
    const GLfloat m0 = m[0], m5 = m[5], m12 = m[12], m13 = m[13];
    GLuint i;
    STRIDE_LOOP {
            const GLfloat ox = from[0], oy = from[1], oz = from[2], ow = from[3];
            to[i][0] = m0 * ox                      + m12 * ow;
            to[i][1] =           m5 * oy            + m13 * ow;
            to[i][2] =                   +       oz           ;
            to[i][3] =                                      ow;
    }
    to_vec->size = 4;
    to_vec->flags |= VEC_SIZE_4;
    to_vec->count = from_vec->count;
}

static void
TAG(transform_points4_3d)( GLvector4f *to_vec,
                           const GLfloat m[16],
                           const GLvector4f *from_vec )
{
    const GLuint stride = from_vec->stride;
    GLfloat *from = from_vec->start;
    GLfloat (*to)[4] = (GLfloat (*)[4])to_vec->start;
    GLuint count = from_vec->count;
    const GLfloat m0 = m[0], m1 = m[1], m2 = m[2], m4 = m[4], m5 = m[5];
    const GLfloat m6 = m[6], m8 = m[8], m9 = m[9], m10 = m[10];
    const GLfloat m12 = m[12], m13 = m[13], m14 = m[14];
    GLuint i;
    STRIDE_LOOP {
            const GLfloat ox = from[0], oy = from[1], oz = from[2], ow = from[3];
            to[i][0] = m0 * ox + m4 * oy +  m8 * oz + m12 * ow;
            to[i][1] = m1 * ox + m5 * oy +  m9 * oz + m13 * ow;
            to[i][2] = m2 * ox + m6 * oy + m10 * oz + m14 * ow;
            to[i][3] =                                      ow;
    }
    to_vec->size = 4;
    to_vec->flags |= VEC_SIZE_4;
    to_vec->count = from_vec->count;
}

static void
TAG(transform_points4_3d_no_rot)( GLvector4f *to_vec,
                                  const GLfloat m[16],
                                  const GLvector4f *from_vec )
{
    const GLuint stride = from_vec->stride;
    GLfloat *from = from_vec->start;
    GLfloat (*to)[4] = (GLfloat (*)[4])to_vec->start;
    GLuint count = from_vec->count;
    const GLfloat m0 = m[0], m5 = m[5];
    const GLfloat m10 = m[10], m12 = m[12], m13 = m[13], m14 = m[14];
    GLuint i;
    STRIDE_LOOP {
            const GLfloat ox = from[0], oy = from[1], oz = from[2], ow = from[3];
            to[i][0] = m0 * ox                      + m12 * ow;
            to[i][1] =           m5 * oy            + m13 * ow;
            to[i][2] =                     m10 * oz + m14 * ow;
            to[i][3] =                                      ow;
    }
    to_vec->size = 4;
    to_vec->flags |= VEC_SIZE_4;
    to_vec->count = from_vec->count;
}

static void
TAG(transform_points4_perspective)( GLvector4f *to_vec,
                                    const GLfloat m[16],
                                    const GLvector4f *from_vec )
{
    const GLuint stride = from_vec->stride;
    GLfloat *from = from_vec->start;
    GLfloat (*to)[4] = (GLfloat (*)[4])to_vec->start;
    GLuint count = from_vec->count;
    const GLfloat m0 = m[0], m5 = m[5], m8 = m[8], m9 = m[9];
    const GLfloat m10 = m[10], m14 = m[14];
    GLuint i;
    STRIDE_LOOP {
            const GLfloat ox = from[0], oy = from[1], oz = from[2], ow = from[3];
            to[i][0] = m0 * ox           + m8  * oz            ;
            to[i][1] =           m5 * oy + m9  * oz            ;
            to[i][2] =                     m10 * oz + m14 * ow ;
            to[i][3] =                          -oz            ;
    }
    to_vec->size = 4;
    to_vec->flags |= VEC_SIZE_4;
    to_vec->count = from_vec->count;
}

static transform_func TAG(transform_tab_1)[7];
static transform_func TAG(transform_tab_2)[7];
static transform_func TAG(transform_tab_3)[7];
static transform_func TAG(transform_tab_4)[7];

/* Similar functions could be called several times, with more highly
 * optimized routines overwriting the arrays.  This only occurs during
 * startup.
 */
static void TAG(init_c_transformations)( void )
{
#define TAG_TAB   _mesa_transform_tab
#define TAG_TAB_1 TAG(transform_tab_1)
#define TAG_TAB_2 TAG(transform_tab_2)
#define TAG_TAB_3 TAG(transform_tab_3)
#define TAG_TAB_4 TAG(transform_tab_4)

    TAG_TAB[1] = TAG_TAB_1;
    TAG_TAB[2] = TAG_TAB_2;
    TAG_TAB[3] = TAG_TAB_3;
    TAG_TAB[4] = TAG_TAB_4;

    /* 1-D points (ie texcoords) */
    TAG_TAB_1[MATRIX_GENERAL]     = TAG(transform_points1_general);
    TAG_TAB_1[MATRIX_IDENTITY]    = TAG(transform_points1_identity);
    TAG_TAB_1[MATRIX_3D_NO_ROT]   = TAG(transform_points1_3d_no_rot);
    TAG_TAB_1[MATRIX_PERSPECTIVE] = TAG(transform_points1_perspective);
    TAG_TAB_1[MATRIX_2D]          = TAG(transform_points1_2d);
    TAG_TAB_1[MATRIX_2D_NO_ROT]   = TAG(transform_points1_2d_no_rot);
    TAG_TAB_1[MATRIX_3D]          = TAG(transform_points1_3d);

    /* 2-D points */
    TAG_TAB_2[MATRIX_GENERAL]     = TAG(transform_points2_general);
    TAG_TAB_2[MATRIX_IDENTITY]    = TAG(transform_points2_identity);
    TAG_TAB_2[MATRIX_3D_NO_ROT]   = TAG(transform_points2_3d_no_rot);
    TAG_TAB_2[MATRIX_PERSPECTIVE] = TAG(transform_points2_perspective);
    TAG_TAB_2[MATRIX_2D]          = TAG(transform_points2_2d);
    TAG_TAB_2[MATRIX_2D_NO_ROT]   = TAG(transform_points2_2d_no_rot);
    TAG_TAB_2[MATRIX_3D]          = TAG(transform_points2_3d);

    /* 3-D points */
    TAG_TAB_3[MATRIX_GENERAL]     = TAG(transform_points3_general);
    TAG_TAB_3[MATRIX_IDENTITY]    = TAG(transform_points3_identity);
    TAG_TAB_3[MATRIX_3D_NO_ROT]   = TAG(transform_points3_3d_no_rot);
    TAG_TAB_3[MATRIX_PERSPECTIVE] = TAG(transform_points3_perspective);
    TAG_TAB_3[MATRIX_2D]          = TAG(transform_points3_2d);
    TAG_TAB_3[MATRIX_2D_NO_ROT]   = TAG(transform_points3_2d_no_rot);
    TAG_TAB_3[MATRIX_3D]          = TAG(transform_points3_3d);

    /* 4-D points */
    TAG_TAB_4[MATRIX_GENERAL]     = TAG(transform_points4_general);
    TAG_TAB_4[MATRIX_IDENTITY]    = TAG(transform_points4_identity);
    TAG_TAB_4[MATRIX_3D_NO_ROT]   = TAG(transform_points4_3d_no_rot);
    TAG_TAB_4[MATRIX_PERSPECTIVE] = TAG(transform_points4_perspective);
    TAG_TAB_4[MATRIX_2D]          = TAG(transform_points4_2d);
    TAG_TAB_4[MATRIX_2D_NO_ROT]   = TAG(transform_points4_2d_no_rot);
    TAG_TAB_4[MATRIX_3D]          = TAG(transform_points4_3d);

#undef TAG_TAB
#undef TAG_TAB_1
#undef TAG_TAB_2
#undef TAG_TAB_3
#undef TAG_TAB_4
}
