static void TAG(dotprod_vec2)( GLfloat *out,
                               GLuint outstride,
                               const GLvector4f *coord_vec,
                               const GLfloat plane[4] )
{
    GLuint stride = coord_vec->stride;
    GLfloat *coord = coord_vec->start;
    GLuint count = coord_vec->count;

    GLuint i;

    const GLfloat plane0 = plane[0], plane1 = plane[1], plane3 = plane[3];

    for (i=0;i<count;i++,STRIDE_F(coord,stride),STRIDE_F(out,outstride)) {
        *out = (coord[0] * plane0 +
                coord[1] * plane1 +
                plane3);
    }
}

static void TAG(dotprod_vec3)( GLfloat *out,
                               GLuint outstride,
                               const GLvector4f *coord_vec,
                               const GLfloat plane[4] )
{
    GLuint stride = coord_vec->stride;
    GLfloat *coord = coord_vec->start;
    GLuint count = coord_vec->count;

    GLuint i;

    const GLfloat plane0 = plane[0], plane1 = plane[1], plane2 = plane[2];
    const GLfloat plane3 = plane[3];

    for (i=0;i<count;i++,STRIDE_F(coord,stride),STRIDE_F(out,outstride)) {
        *out = (coord[0] * plane0 +
                coord[1] * plane1 +
                coord[2] * plane2 +
                plane3);
    }
}

static void TAG(dotprod_vec4)( GLfloat *out,
                               GLuint outstride,
                               const GLvector4f *coord_vec,
                               const GLfloat plane[4] )
{
    GLuint stride = coord_vec->stride;
    GLfloat *coord = coord_vec->start;
    GLuint count = coord_vec->count;
    GLuint i;

    const GLfloat plane0 = plane[0], plane1 = plane[1], plane2 = plane[2];
    const GLfloat plane3 = plane[3];

    for (i=0;i<count;i++,STRIDE_F(coord,stride),STRIDE_F(out,outstride)) {
        *out = (coord[0] * plane0 +
                coord[1] * plane1 +
                coord[2] * plane2 +
                coord[3] * plane3);
    }
}


static void TAG(init_dotprod)( void )
{
    _mesa_dotprod_tab[2] = TAG(dotprod_vec2);
    _mesa_dotprod_tab[3] = TAG(dotprod_vec3);
    _mesa_dotprod_tab[4] = TAG(dotprod_vec4);
}
