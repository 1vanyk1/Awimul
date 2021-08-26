#include "../include/c99_math.h"
#include "../main/glheader.h"
#include "../main/macros.h"

#include "m_eval.h"
#include "m_matrix.h"
#include "m_translate.h"
#include "m_xform.h"


#ifdef DEBUG_MATH
#include "m_debug.h"
#endif

#ifdef USE_X86_ASM
#include "x86/common_x86_asm.h"
#endif

#ifdef USE_X86_64_ASM
#include "x86-64/x86-64.h"
#endif

#ifdef USE_SPARC_ASM
#include "sparc/sparc.h"
#endif

clip_func _mesa_clip_tab[5];
clip_func _mesa_clip_np_tab[5];
dotprod_func _mesa_dotprod_tab[5];
vec_copy_func _mesa_copy_tab[0x10];
normal_func _mesa_normal_tab[0xf];
transform_func *_mesa_transform_tab[5];


/* Raw data format used for:
 *    - Object-to-eye transform prior to culling, although this too
 *      could be culled under some circumstances.
 *    - Eye-to-clip transform (via the function above).
 *    - Cliptesting
 *    - And everything else too, if culling happens to be disabled.
 *
 * GH: It's used for everything now, as clipping/culling is done
 *     elsewhere (most often by the driver itself).
 */
#define TAG(x) x
#define TAG2(x,y) x##y
#define STRIDE_LOOP for ( i = 0 ; i < count ; i++, STRIDE_F(from, stride) )
#define LOOP for ( i = 0 ; i < n ; i++ )
#define ARGS
#include "m_xform_tmp.h"
#include "m_clip_tmp.h"
#include "m_norm_tmp.h"
#include "m_dotprod_tmp.h"
#include "m_copy_tmp.h"
#undef TAG
#undef TAG2
#undef LOOP
#undef ARGS


/*
 * This is called only once.  It initializes several tables with pointers
 * to optimized transformation functions.  This is where we can test for
 * AMD 3Dnow! capability, Intel SSE, etc. and hook in the right code.
 */
void
_math_init_transformation( void )
{
    init_c_transformations();
    init_c_norm_transform();
    init_c_cliptest();
    init_copy0();
    init_dotprod();

#ifdef DEBUG_MATH
    _math_test_all_transform_functions( "default" );
   _math_test_all_normal_transform_functions( "default" );
   _math_test_all_cliptest_functions( "default" );
#endif

#ifdef USE_X86_ASM
    _mesa_init_all_x86_transform_asm();
#elif defined( USE_SPARC_ASM )
    _mesa_init_all_sparc_transform_asm();
#elif defined( USE_X86_64_ASM )
    _mesa_init_all_x86_64_transform_asm();
#endif
}
