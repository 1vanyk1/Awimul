#if IDX & LIGHT_TWOSIDE
#  define NR_SIDES 2
#else
#  define NR_SIDES 1
#endif


/* define TRACE to trace lighting code */
/* #define TRACE 1 */

/*
 * ctx is the current context
 * VB is the vertex buffer
 * stage is the lighting stage-private data
 * input is the vector of eye or object-space vertex coordinates
 */
static void TAG(light_rgba_spec)( struct gl_context *ctx,
                                  struct vertex_buffer *VB,
                                  struct tnl_pipeline_stage *stage,
                                  GLvector4f *input )
{
    struct light_stage_data *store = LIGHT_STAGE_DATA(stage);
    GLfloat (*base)[3] = ctx->Light._BaseColor;
    GLfloat sumA[2];
    GLuint j;

    const GLuint vstride = input->stride;
    const GLfloat *vertex = (GLfloat *)input->data;
    const GLuint nstride = VB->AttribPtr[_TNL_ATTRIB_NORMAL]->stride;
    const GLfloat *normal = (GLfloat *)VB->AttribPtr[_TNL_ATTRIB_NORMAL]->data;

    GLfloat (*Fcolor)[4] = (GLfloat (*)[4]) store->LitColor[0].data;
    GLfloat (*Fspec)[4] = (GLfloat (*)[4]) store->LitSecondary[0].data;
#if IDX & LIGHT_TWOSIDE
    GLfloat (*Bcolor)[4] = (GLfloat (*)[4]) store->LitColor[1].data;
   GLfloat (*Bspec)[4] = (GLfloat (*)[4]) store->LitSecondary[1].data;
#endif

    const GLuint nr = VB->Count;

#ifdef TRACE
    fprintf(stderr, "%s\n", __func__ );
#endif

    VB->AttribPtr[_TNL_ATTRIB_COLOR0] = &store->LitColor[0];
    VB->AttribPtr[_TNL_ATTRIB_COLOR1] = &store->LitSecondary[0];
    sumA[0] = ctx->Light.Material.Attrib[MAT_ATTRIB_FRONT_DIFFUSE][3];

#if IDX & LIGHT_TWOSIDE
    VB->BackfaceColorPtr = &store->LitColor[1];
   VB->BackfaceSecondaryColorPtr = &store->LitSecondary[1];
   sumA[1] = ctx->Light.Material.Attrib[MAT_ATTRIB_BACK_DIFFUSE][3];
#endif


    store->LitColor[0].stride = 16;
    store->LitColor[1].stride = 16;

    for (j = 0; j < nr; j++,STRIDE_F(vertex,vstride),STRIDE_F(normal,nstride)) {
        GLfloat sum[2][3], spec[2][3];
        GLbitfield mask;

#if IDX & LIGHT_MATERIAL
        update_materials( ctx, store );
      sumA[0] = ctx->Light.Material.Attrib[MAT_ATTRIB_FRONT_DIFFUSE][3];
#if IDX & LIGHT_TWOSIDE
      sumA[1] = ctx->Light.Material.Attrib[MAT_ATTRIB_BACK_DIFFUSE][3];
#endif
#endif

        COPY_3V(sum[0], base[0]);
        ZERO_3V(spec[0]);

#if IDX & LIGHT_TWOSIDE
        COPY_3V(sum[1], base[1]);
      ZERO_3V(spec[1]);
#endif

        /* Add contribution from each enabled light source */
        mask = ctx->Light._EnabledLights;
        while (mask) {
            const int l = u_bit_scan(&mask);
            struct gl_light *light = &ctx->Light.Light[l];
            struct gl_light_uniforms *lu = &ctx->Light.LightSource[l];
            GLfloat n_dot_h;
            GLfloat correction;
            GLint side;
            GLfloat contrib[3];
            GLfloat attenuation;
            GLfloat VP[3];          /* unit vector from vertex to light */
            GLfloat n_dot_VP;       /* n dot VP */
            GLfloat *h;

            /* compute VP and attenuation */
            if (!(light->_Flags & LIGHT_POSITIONAL)) {
                /* directional light */
                COPY_3V(VP, light->_VP_inf_norm);
                attenuation = light->_VP_inf_spot_attenuation;
            }
            else {
                GLfloat d;     /* distance from vertex to light */

                SUB_3V(VP, light->_Position, vertex);

                d = (GLfloat) LEN_3FV( VP );

                if (d > 1e-6F) {
                    GLfloat invd = 1.0F / d;
                    SELF_SCALE_SCALAR_3V(VP, invd);
                }

                attenuation = 1.0F / (lu->ConstantAttenuation + d *
                                                                (lu->LinearAttenuation + d *
                                                                                         lu->QuadraticAttenuation));

                /* spotlight attenuation */
                if (light->_Flags & LIGHT_SPOT) {
                    GLfloat PV_dot_dir = - DOT3(VP, light->_NormSpotDirection);

                    if (PV_dot_dir<lu->_CosCutoff) {
                        continue; /* this light makes no contribution */
                    }
                    else {
                        GLfloat spot = powf(PV_dot_dir, lu->SpotExponent);
                        attenuation *= spot;
                    }
                }
            }

            if (attenuation < 1e-3F)
                continue;		/* this light makes no contribution */

            /* Compute dot product or normal and vector from V to light pos */
            n_dot_VP = DOT3( normal, VP );

            /* Which side gets the diffuse & specular terms? */
            if (n_dot_VP < 0.0F) {
                ACC_SCALE_SCALAR_3V(sum[0], attenuation, light->_MatAmbient[0]);
#if IDX & LIGHT_TWOSIDE
                side = 1;
	    correction = -1;
	    n_dot_VP = -n_dot_VP;
#else
                continue;
#endif
            }
            else {
#if IDX & LIGHT_TWOSIDE
                ACC_SCALE_SCALAR_3V( sum[1], attenuation, light->_MatAmbient[1]);
#endif
                side = 0;
                correction = 1;
            }

            /* diffuse term */
            COPY_3V(contrib, light->_MatAmbient[side]);
            ACC_SCALE_SCALAR_3V(contrib, n_dot_VP, light->_MatDiffuse[side]);
            ACC_SCALE_SCALAR_3V(sum[side], attenuation, contrib );

            /* specular term - cannibalize VP... */
            if (ctx->Light.Model.LocalViewer) {
                GLfloat v[3];
                COPY_3V(v, vertex);
                NORMALIZE_3FV(v);
                SUB_3V(VP, VP, v);                /* h = VP + VPe */
                h = VP;
                NORMALIZE_3FV(h);
            }
            else if (light->_Flags & LIGHT_POSITIONAL) {
                h = VP;
                ACC_3V(h, ctx->_EyeZDir);
                NORMALIZE_3FV(h);
            }
            else {
                h = light->_h_inf_norm;
            }

            n_dot_h = correction * DOT3(normal, h);

            if (n_dot_h > 0.0F) {
                GLfloat spec_coef = lookup_shininess(ctx, side, n_dot_h);
                if (spec_coef > 1.0e-10F) {
                    spec_coef *= attenuation;
                    ACC_SCALE_SCALAR_3V( spec[side], spec_coef,
                                         light->_MatSpecular[side]);
                }
            }
        } /*loop over lights*/

        COPY_3V( Fcolor[j], sum[0] );
        COPY_3V( Fspec[j], spec[0] );
        Fcolor[j][3] = sumA[0];

#if IDX & LIGHT_TWOSIDE
        COPY_3V( Bcolor[j], sum[1] );
      COPY_3V( Bspec[j], spec[1] );
      Bcolor[j][3] = sumA[1];
#endif
    }
}


static void TAG(light_rgba)( struct gl_context *ctx,
                             struct vertex_buffer *VB,
                             struct tnl_pipeline_stage *stage,
                             GLvector4f *input )
{
    struct light_stage_data *store = LIGHT_STAGE_DATA(stage);
    GLuint j;

    GLfloat (*base)[3] = ctx->Light._BaseColor;
    GLfloat sumA[2];

    const GLuint vstride = input->stride;
    const GLfloat *vertex = (GLfloat *) input->data;
    const GLuint nstride = VB->AttribPtr[_TNL_ATTRIB_NORMAL]->stride;
    const GLfloat *normal = (GLfloat *)VB->AttribPtr[_TNL_ATTRIB_NORMAL]->data;

    GLfloat (*Fcolor)[4] = (GLfloat (*)[4]) store->LitColor[0].data;
#if IDX & LIGHT_TWOSIDE
    GLfloat (*Bcolor)[4] = (GLfloat (*)[4]) store->LitColor[1].data;
#endif

    const GLuint nr = VB->Count;

#ifdef TRACE
    fprintf(stderr, "%s\n", __func__ );
#endif

    VB->AttribPtr[_TNL_ATTRIB_COLOR0] = &store->LitColor[0];
    sumA[0] = ctx->Light.Material.Attrib[MAT_ATTRIB_FRONT_DIFFUSE][3];

#if IDX & LIGHT_TWOSIDE
    VB->BackfaceColorPtr = &store->LitColor[1];
   sumA[1] = ctx->Light.Material.Attrib[MAT_ATTRIB_BACK_DIFFUSE][3];
#endif

    store->LitColor[0].stride = 16;
    store->LitColor[1].stride = 16;

    for (j = 0; j < nr; j++,STRIDE_F(vertex,vstride),STRIDE_F(normal,nstride)) {
        GLfloat sum[2][3];
        GLbitfield mask;

#if IDX & LIGHT_MATERIAL
        update_materials( ctx, store );
      sumA[0] = ctx->Light.Material.Attrib[MAT_ATTRIB_FRONT_DIFFUSE][3];
#if IDX & LIGHT_TWOSIDE
      sumA[1] = ctx->Light.Material.Attrib[MAT_ATTRIB_BACK_DIFFUSE][3];
#endif
#endif

        COPY_3V(sum[0], base[0]);

#if IDX & LIGHT_TWOSIDE
        COPY_3V(sum[1], base[1]);
#endif

        /* Add contribution from each enabled light source */
        mask = ctx->Light._EnabledLights;
        while (mask) {
            const int l = u_bit_scan(&mask);
            struct gl_light *light = &ctx->Light.Light[l];
            struct gl_light_uniforms *lu = &ctx->Light.LightSource[l];
            GLfloat n_dot_h;
            GLfloat correction;
            GLint side;
            GLfloat contrib[3];
            GLfloat attenuation;
            GLfloat VP[3];          /* unit vector from vertex to light */
            GLfloat n_dot_VP;       /* n dot VP */
            GLfloat *h;

            /* compute VP and attenuation */
            if (!(light->_Flags & LIGHT_POSITIONAL)) {
                /* directional light */
                COPY_3V(VP, light->_VP_inf_norm);
                attenuation = light->_VP_inf_spot_attenuation;
            }
            else {
                GLfloat d;     /* distance from vertex to light */

                SUB_3V(VP, light->_Position, vertex);

                d = (GLfloat) LEN_3FV( VP );

                if (d > 1e-6F) {
                    GLfloat invd = 1.0F / d;
                    SELF_SCALE_SCALAR_3V(VP, invd);
                }

                attenuation = 1.0F / (lu->ConstantAttenuation + d *
                                                                (lu->LinearAttenuation + d *
                                                                                         lu->QuadraticAttenuation));

                /* spotlight attenuation */
                if (light->_Flags & LIGHT_SPOT) {
                    GLfloat PV_dot_dir = - DOT3(VP, light->_NormSpotDirection);

                    if (PV_dot_dir<lu->_CosCutoff) {
                        continue; /* this light makes no contribution */
                    }
                    else {
                        GLfloat spot = powf(PV_dot_dir, lu->SpotExponent);
                        attenuation *= spot;
                    }
                }
            }

            if (attenuation < 1e-3F)
                continue;		/* this light makes no contribution */

            /* Compute dot product or normal and vector from V to light pos */
            n_dot_VP = DOT3( normal, VP );

            /* which side are we lighting? */
            if (n_dot_VP < 0.0F) {
                ACC_SCALE_SCALAR_3V(sum[0], attenuation, light->_MatAmbient[0]);
#if IDX & LIGHT_TWOSIDE
                side = 1;
	    correction = -1;
	    n_dot_VP = -n_dot_VP;
#else
                continue;
#endif
            }
            else {
#if IDX & LIGHT_TWOSIDE
                ACC_SCALE_SCALAR_3V( sum[1], attenuation, light->_MatAmbient[1]);
#endif
                side = 0;
                correction = 1;
            }

            COPY_3V(contrib, light->_MatAmbient[side]);

            /* diffuse term */
            ACC_SCALE_SCALAR_3V(contrib, n_dot_VP, light->_MatDiffuse[side]);

            /* specular term - cannibalize VP... */
            {
                if (ctx->Light.Model.LocalViewer) {
                    GLfloat v[3];
                    COPY_3V(v, vertex);
                    NORMALIZE_3FV(v);
                    SUB_3V(VP, VP, v);                /* h = VP + VPe */
                    h = VP;
                    NORMALIZE_3FV(h);
                }
                else if (light->_Flags & LIGHT_POSITIONAL) {
                    h = VP;
                    ACC_3V(h, ctx->_EyeZDir);
                    NORMALIZE_3FV(h);
                }
                else {
                    h = light->_h_inf_norm;
                }

                n_dot_h = correction * DOT3(normal, h);

                if (n_dot_h > 0.0F) {
                    GLfloat spec_coef = lookup_shininess(ctx, side, n_dot_h);
                    ACC_SCALE_SCALAR_3V( contrib, spec_coef,
                                         light->_MatSpecular[side]);
                }
            }

            ACC_SCALE_SCALAR_3V( sum[side], attenuation, contrib );
        }

        COPY_3V( Fcolor[j], sum[0] );
        Fcolor[j][3] = sumA[0];

#if IDX & LIGHT_TWOSIDE
        COPY_3V( Bcolor[j], sum[1] );
      Bcolor[j][3] = sumA[1];
#endif
    }
}




/* As below, but with just a single light.
 */
static void TAG(light_fast_rgba_single)( struct gl_context *ctx,
                                         struct vertex_buffer *VB,
                                         struct tnl_pipeline_stage *stage,
                                         GLvector4f *input )

{
    struct light_stage_data *store = LIGHT_STAGE_DATA(stage);
    const GLuint nstride = VB->AttribPtr[_TNL_ATTRIB_NORMAL]->stride;
    const GLfloat *normal = (GLfloat *)VB->AttribPtr[_TNL_ATTRIB_NORMAL]->data;
    GLfloat (*Fcolor)[4] = (GLfloat (*)[4]) store->LitColor[0].data;
#if IDX & LIGHT_TWOSIDE
    GLfloat (*Bcolor)[4] = (GLfloat (*)[4]) store->LitColor[1].data;
#endif
    const struct gl_light *light =
            &ctx->Light.Light[ffs(ctx->Light._EnabledLights) - 1];
    GLuint j = 0;
    GLfloat base[2][4];
#if IDX & LIGHT_MATERIAL
    const GLuint nr = VB->Count;
#else
    const GLuint nr = VB->AttribPtr[_TNL_ATTRIB_NORMAL]->count;
#endif

#ifdef TRACE
    fprintf(stderr, "%s\n", __func__ );
#endif

    (void) input;		/* doesn't refer to Eye or Obj */

    VB->AttribPtr[_TNL_ATTRIB_COLOR0] = &store->LitColor[0];
#if IDX & LIGHT_TWOSIDE
    VB->BackfaceColorPtr = &store->LitColor[1];
#endif

    if (nr > 1) {
        store->LitColor[0].stride = 16;
        store->LitColor[1].stride = 16;
    }
    else {
        store->LitColor[0].stride = 0;
        store->LitColor[1].stride = 0;
    }

    for (j = 0; j < nr; j++, STRIDE_F(normal,nstride)) {

        GLfloat n_dot_VP;

#if IDX & LIGHT_MATERIAL
        update_materials( ctx, store );
#endif

        /* No attenuation, so incoporate _MatAmbient into base color.
         */
#if !(IDX & LIGHT_MATERIAL)
        if ( j == 0 )
#endif
        {
            COPY_3V(base[0], light->_MatAmbient[0]);
            ACC_3V(base[0], ctx->Light._BaseColor[0] );
            base[0][3] = ctx->Light.Material.Attrib[MAT_ATTRIB_FRONT_DIFFUSE][3];

#if IDX & LIGHT_TWOSIDE
            COPY_3V(base[1], light->_MatAmbient[1]);
         ACC_3V(base[1], ctx->Light._BaseColor[1]);
         base[1][3] = ctx->Light.Material.Attrib[MAT_ATTRIB_BACK_DIFFUSE][3];
#endif
        }

        n_dot_VP = DOT3(normal, light->_VP_inf_norm);

        if (n_dot_VP < 0.0F) {
#if IDX & LIGHT_TWOSIDE
            GLfloat n_dot_h = -DOT3(normal, light->_h_inf_norm);
         GLfloat sum[3];
         COPY_3V(sum, base[1]);
         ACC_SCALE_SCALAR_3V(sum, -n_dot_VP, light->_MatDiffuse[1]);
         if (n_dot_h > 0.0F) {
            GLfloat spec = lookup_shininess(ctx, 1, n_dot_h);
            ACC_SCALE_SCALAR_3V(sum, spec, light->_MatSpecular[1]);
         }
         COPY_3V(Bcolor[j], sum );
         Bcolor[j][3] = base[1][3];
#endif
            COPY_4FV(Fcolor[j], base[0]);
        }
        else {
            GLfloat n_dot_h = DOT3(normal, light->_h_inf_norm);
            GLfloat sum[3];
            COPY_3V(sum, base[0]);
            ACC_SCALE_SCALAR_3V(sum, n_dot_VP, light->_MatDiffuse[0]);
            if (n_dot_h > 0.0F) {
                GLfloat spec = lookup_shininess(ctx, 0, n_dot_h);
                ACC_SCALE_SCALAR_3V(sum, spec, light->_MatSpecular[0]);
            }
            COPY_3V(Fcolor[j], sum );
            Fcolor[j][3] = base[0][3];
#if IDX & LIGHT_TWOSIDE
            COPY_4FV(Bcolor[j], base[1]);
#endif
        }
    }
}


/* Light infinite lights
 */
static void TAG(light_fast_rgba)( struct gl_context *ctx,
                                  struct vertex_buffer *VB,
                                  struct tnl_pipeline_stage *stage,
                                  GLvector4f *input )
{
    struct light_stage_data *store = LIGHT_STAGE_DATA(stage);
    GLfloat sumA[2];
    const GLuint nstride = VB->AttribPtr[_TNL_ATTRIB_NORMAL]->stride;
    const GLfloat *normal = (GLfloat *)VB->AttribPtr[_TNL_ATTRIB_NORMAL]->data;
    GLfloat (*Fcolor)[4] = (GLfloat (*)[4]) store->LitColor[0].data;
#if IDX & LIGHT_TWOSIDE
    GLfloat (*Bcolor)[4] = (GLfloat (*)[4]) store->LitColor[1].data;
#endif
    GLuint j = 0;
#if IDX & LIGHT_MATERIAL
    const GLuint nr = VB->Count;
#else
    const GLuint nr = VB->AttribPtr[_TNL_ATTRIB_NORMAL]->count;
#endif

#ifdef TRACE
    fprintf(stderr, "%s %d\n", __func__, nr );
#endif

    (void) input;

    sumA[0] = ctx->Light.Material.Attrib[MAT_ATTRIB_FRONT_DIFFUSE][3];
    sumA[1] = ctx->Light.Material.Attrib[MAT_ATTRIB_BACK_DIFFUSE][3];

    VB->AttribPtr[_TNL_ATTRIB_COLOR0] = &store->LitColor[0];
#if IDX & LIGHT_TWOSIDE
    VB->BackfaceColorPtr = &store->LitColor[1];
#endif

    if (nr > 1) {
        store->LitColor[0].stride = 16;
        store->LitColor[1].stride = 16;
    }
    else {
        store->LitColor[0].stride = 0;
        store->LitColor[1].stride = 0;
    }

    for (j = 0; j < nr; j++, STRIDE_F(normal,nstride)) {

        GLfloat sum[2][3];
        GLbitfield mask;

#if IDX & LIGHT_MATERIAL
        update_materials( ctx, store );

      sumA[0] = ctx->Light.Material.Attrib[MAT_ATTRIB_FRONT_DIFFUSE][3];
#if IDX & LIGHT_TWOSIDE
      sumA[1] = ctx->Light.Material.Attrib[MAT_ATTRIB_BACK_DIFFUSE][3];
#endif
#endif


        COPY_3V(sum[0], ctx->Light._BaseColor[0]);
#if IDX & LIGHT_TWOSIDE
        COPY_3V(sum[1], ctx->Light._BaseColor[1]);
#endif

        mask = ctx->Light._EnabledLights;
        while (mask) {
            const int l = u_bit_scan(&mask);
            const struct gl_light *light = &ctx->Light.Light[l];
            GLfloat n_dot_h, n_dot_VP, spec;

            ACC_3V(sum[0], light->_MatAmbient[0]);
#if IDX & LIGHT_TWOSIDE
            ACC_3V(sum[1], light->_MatAmbient[1]);
#endif

            n_dot_VP = DOT3(normal, light->_VP_inf_norm);

            if (n_dot_VP > 0.0F) {
                ACC_SCALE_SCALAR_3V(sum[0], n_dot_VP, light->_MatDiffuse[0]);
                n_dot_h = DOT3(normal, light->_h_inf_norm);
                if (n_dot_h > 0.0F) {
                    spec = lookup_shininess(ctx, 0, n_dot_h);
                    ACC_SCALE_SCALAR_3V( sum[0], spec, light->_MatSpecular[0]);
                }
            }
#if IDX & LIGHT_TWOSIDE
            else {
	    ACC_SCALE_SCALAR_3V(sum[1], -n_dot_VP, light->_MatDiffuse[1]);
	    n_dot_h = -DOT3(normal, light->_h_inf_norm);
	    if (n_dot_h > 0.0F) {
               spec = lookup_shininess(ctx, 1, n_dot_h);
	       ACC_SCALE_SCALAR_3V( sum[1], spec, light->_MatSpecular[1]);
	    }
	 }
#endif
        }

        COPY_3V( Fcolor[j], sum[0] );
        Fcolor[j][3] = sumA[0];

#if IDX & LIGHT_TWOSIDE
        COPY_3V( Bcolor[j], sum[1] );
      Bcolor[j][3] = sumA[1];
#endif
    }
}




static void TAG(init_light_tab)( void )
{
    _tnl_light_tab[IDX] = TAG(light_rgba);
    _tnl_light_fast_tab[IDX] = TAG(light_fast_rgba);
    _tnl_light_fast_single_tab[IDX] = TAG(light_fast_rgba_single);
    _tnl_light_spec_tab[IDX] = TAG(light_rgba_spec);
}


#undef TAG
#undef IDX
#undef NR_SIDES