#include "dispatch.h"
#include "remap.h"

/* this is internal to remap.c */
#ifndef need_MESA_remap_table
//#error Only remap.c should include this file!
#endif /* need_MESA_remap_table */


static const char _mesa_function_pool[] =
        /* _mesa_function_pool[0]: NewList (dynamic) */
        "ii\0"
        "glNewList\0"
        "\0"
        /* _mesa_function_pool[14]: EndList (offset 1) */
        "\0"
        "glEndList\0"
        "\0"
        /* _mesa_function_pool[26]: CallList (offset 2) */
        "i\0"
        "glCallList\0"
        "\0"
        /* _mesa_function_pool[40]: CallLists (offset 3) */
        "iip\0"
        "glCallLists\0"
        "\0"
        /* _mesa_function_pool[57]: DeleteLists (offset 4) */
        "ii\0"
        "glDeleteLists\0"
        "\0"
        /* _mesa_function_pool[75]: GenLists (offset 5) */
        "i\0"
        "glGenLists\0"
        "\0"
        /* _mesa_function_pool[89]: ListBase (offset 6) */
        "i\0"
        "glListBase\0"
        "\0"
        /* _mesa_function_pool[103]: Begin (offset 7) */
        "i\0"
        "glBegin\0"
        "\0"
        /* _mesa_function_pool[114]: Bitmap (offset 8) */
        "iiffffp\0"
        "glBitmap\0"
        "\0"
        /* _mesa_function_pool[132]: Color3b (offset 9) */
        "iii\0"
        "glColor3b\0"
        "\0"
        /* _mesa_function_pool[147]: Color3bv (offset 10) */
        "p\0"
        "glColor3bv\0"
        "\0"
        /* _mesa_function_pool[161]: Color3d (offset 11) */
        "ddd\0"
        "glColor3d\0"
        "\0"
        /* _mesa_function_pool[176]: Color3dv (offset 12) */
        "p\0"
        "glColor3dv\0"
        "\0"
        /* _mesa_function_pool[190]: Color3f (offset 13) */
        "fff\0"
        "glColor3f\0"
        "\0"
        /* _mesa_function_pool[205]: Color3fv (offset 14) */
        "p\0"
        "glColor3fv\0"
        "\0"
        /* _mesa_function_pool[219]: Color3i (offset 15) */
        "iii\0"
        "glColor3i\0"
        "\0"
        /* _mesa_function_pool[234]: Color3iv (offset 16) */
        "p\0"
        "glColor3iv\0"
        "\0"
        /* _mesa_function_pool[248]: Color3s (offset 17) */
        "iii\0"
        "glColor3s\0"
        "\0"
        /* _mesa_function_pool[263]: Color3sv (offset 18) */
        "p\0"
        "glColor3sv\0"
        "\0"
        /* _mesa_function_pool[277]: Color3ub (offset 19) */
        "iii\0"
        "glColor3ub\0"
        "\0"
        /* _mesa_function_pool[293]: Color3ubv (offset 20) */
        "p\0"
        "glColor3ubv\0"
        "\0"
        /* _mesa_function_pool[308]: Color3ui (offset 21) */
        "iii\0"
        "glColor3ui\0"
        "\0"
        /* _mesa_function_pool[324]: Color3uiv (offset 22) */
        "p\0"
        "glColor3uiv\0"
        "\0"
        /* _mesa_function_pool[339]: Color3us (offset 23) */
        "iii\0"
        "glColor3us\0"
        "\0"
        /* _mesa_function_pool[355]: Color3usv (offset 24) */
        "p\0"
        "glColor3usv\0"
        "\0"
        /* _mesa_function_pool[370]: Color4b (offset 25) */
        "iiii\0"
        "glColor4b\0"
        "\0"
        /* _mesa_function_pool[386]: Color4bv (offset 26) */
        "p\0"
        "glColor4bv\0"
        "\0"
        /* _mesa_function_pool[400]: Color4d (offset 27) */
        "dddd\0"
        "glColor4d\0"
        "\0"
        /* _mesa_function_pool[416]: Color4dv (offset 28) */
        "p\0"
        "glColor4dv\0"
        "\0"
        /* _mesa_function_pool[430]: Color4f (offset 29) */
        "ffff\0"
        "glColor4f\0"
        "\0"
        /* _mesa_function_pool[446]: Color4fv (offset 30) */
        "p\0"
        "glColor4fv\0"
        "\0"
        /* _mesa_function_pool[460]: Color4i (offset 31) */
        "iiii\0"
        "glColor4i\0"
        "\0"
        /* _mesa_function_pool[476]: Color4iv (offset 32) */
        "p\0"
        "glColor4iv\0"
        "\0"
        /* _mesa_function_pool[490]: Color4s (offset 33) */
        "iiii\0"
        "glColor4s\0"
        "\0"
        /* _mesa_function_pool[506]: Color4sv (offset 34) */
        "p\0"
        "glColor4sv\0"
        "\0"
        /* _mesa_function_pool[520]: Color4ub (offset 35) */
        "iiii\0"
        "glColor4ub\0"
        "\0"
        /* _mesa_function_pool[537]: Color4ubv (offset 36) */
        "p\0"
        "glColor4ubv\0"
        "\0"
        /* _mesa_function_pool[552]: Color4ui (offset 37) */
        "iiii\0"
        "glColor4ui\0"
        "\0"
        /* _mesa_function_pool[569]: Color4uiv (offset 38) */
        "p\0"
        "glColor4uiv\0"
        "\0"
        /* _mesa_function_pool[584]: Color4us (offset 39) */
        "iiii\0"
        "glColor4us\0"
        "\0"
        /* _mesa_function_pool[601]: Color4usv (offset 40) */
        "p\0"
        "glColor4usv\0"
        "\0"
        /* _mesa_function_pool[616]: EdgeFlag (offset 41) */
        "i\0"
        "glEdgeFlag\0"
        "\0"
        /* _mesa_function_pool[630]: EdgeFlagv (offset 42) */
        "p\0"
        "glEdgeFlagv\0"
        "\0"
        /* _mesa_function_pool[645]: End (offset 43) */
        "\0"
        "glEnd\0"
        "\0"
        /* _mesa_function_pool[653]: Indexd (offset 44) */
        "d\0"
        "glIndexd\0"
        "\0"
        /* _mesa_function_pool[665]: Indexdv (offset 45) */
        "p\0"
        "glIndexdv\0"
        "\0"
        /* _mesa_function_pool[678]: Indexf (offset 46) */
        "f\0"
        "glIndexf\0"
        "\0"
        /* _mesa_function_pool[690]: Indexfv (offset 47) */
        "p\0"
        "glIndexfv\0"
        "\0"
        /* _mesa_function_pool[703]: Indexi (offset 48) */
        "i\0"
        "glIndexi\0"
        "\0"
        /* _mesa_function_pool[715]: Indexiv (offset 49) */
        "p\0"
        "glIndexiv\0"
        "\0"
        /* _mesa_function_pool[728]: Indexs (offset 50) */
        "i\0"
        "glIndexs\0"
        "\0"
        /* _mesa_function_pool[740]: Indexsv (offset 51) */
        "p\0"
        "glIndexsv\0"
        "\0"
        /* _mesa_function_pool[753]: Normal3b (offset 52) */
        "iii\0"
        "glNormal3b\0"
        "\0"
        /* _mesa_function_pool[769]: Normal3bv (offset 53) */
        "p\0"
        "glNormal3bv\0"
        "\0"
        /* _mesa_function_pool[784]: Normal3d (offset 54) */
        "ddd\0"
        "glNormal3d\0"
        "\0"
        /* _mesa_function_pool[800]: Normal3dv (offset 55) */
        "p\0"
        "glNormal3dv\0"
        "\0"
        /* _mesa_function_pool[815]: Normal3f (offset 56) */
        "fff\0"
        "glNormal3f\0"
        "\0"
        /* _mesa_function_pool[831]: Normal3fv (offset 57) */
        "p\0"
        "glNormal3fv\0"
        "\0"
        /* _mesa_function_pool[846]: Normal3i (offset 58) */
        "iii\0"
        "glNormal3i\0"
        "\0"
        /* _mesa_function_pool[862]: Normal3iv (offset 59) */
        "p\0"
        "glNormal3iv\0"
        "\0"
        /* _mesa_function_pool[877]: Normal3s (offset 60) */
        "iii\0"
        "glNormal3s\0"
        "\0"
        /* _mesa_function_pool[893]: Normal3sv (offset 61) */
        "p\0"
        "glNormal3sv\0"
        "\0"
        /* _mesa_function_pool[908]: RasterPos2d (offset 62) */
        "dd\0"
        "glRasterPos2d\0"
        "\0"
        /* _mesa_function_pool[926]: RasterPos2dv (offset 63) */
        "p\0"
        "glRasterPos2dv\0"
        "\0"
        /* _mesa_function_pool[944]: RasterPos2f (offset 64) */
        "ff\0"
        "glRasterPos2f\0"
        "\0"
        /* _mesa_function_pool[962]: RasterPos2fv (offset 65) */
        "p\0"
        "glRasterPos2fv\0"
        "\0"
        /* _mesa_function_pool[980]: RasterPos2i (offset 66) */
        "ii\0"
        "glRasterPos2i\0"
        "\0"
        /* _mesa_function_pool[998]: RasterPos2iv (offset 67) */
        "p\0"
        "glRasterPos2iv\0"
        "\0"
        /* _mesa_function_pool[1016]: RasterPos2s (offset 68) */
        "ii\0"
        "glRasterPos2s\0"
        "\0"
        /* _mesa_function_pool[1034]: RasterPos2sv (offset 69) */
        "p\0"
        "glRasterPos2sv\0"
        "\0"
        /* _mesa_function_pool[1052]: RasterPos3d (offset 70) */
        "ddd\0"
        "glRasterPos3d\0"
        "\0"
        /* _mesa_function_pool[1071]: RasterPos3dv (offset 71) */
        "p\0"
        "glRasterPos3dv\0"
        "\0"
        /* _mesa_function_pool[1089]: RasterPos3f (offset 72) */
        "fff\0"
        "glRasterPos3f\0"
        "\0"
        /* _mesa_function_pool[1108]: RasterPos3fv (offset 73) */
        "p\0"
        "glRasterPos3fv\0"
        "\0"
        /* _mesa_function_pool[1126]: RasterPos3i (offset 74) */
        "iii\0"
        "glRasterPos3i\0"
        "\0"
        /* _mesa_function_pool[1145]: RasterPos3iv (offset 75) */
        "p\0"
        "glRasterPos3iv\0"
        "\0"
        /* _mesa_function_pool[1163]: RasterPos3s (offset 76) */
        "iii\0"
        "glRasterPos3s\0"
        "\0"
        /* _mesa_function_pool[1182]: RasterPos3sv (offset 77) */
        "p\0"
        "glRasterPos3sv\0"
        "\0"
        /* _mesa_function_pool[1200]: RasterPos4d (offset 78) */
        "dddd\0"
        "glRasterPos4d\0"
        "\0"
        /* _mesa_function_pool[1220]: RasterPos4dv (offset 79) */
        "p\0"
        "glRasterPos4dv\0"
        "\0"
        /* _mesa_function_pool[1238]: RasterPos4f (offset 80) */
        "ffff\0"
        "glRasterPos4f\0"
        "\0"
        /* _mesa_function_pool[1258]: RasterPos4fv (offset 81) */
        "p\0"
        "glRasterPos4fv\0"
        "\0"
        /* _mesa_function_pool[1276]: RasterPos4i (offset 82) */
        "iiii\0"
        "glRasterPos4i\0"
        "\0"
        /* _mesa_function_pool[1296]: RasterPos4iv (offset 83) */
        "p\0"
        "glRasterPos4iv\0"
        "\0"
        /* _mesa_function_pool[1314]: RasterPos4s (offset 84) */
        "iiii\0"
        "glRasterPos4s\0"
        "\0"
        /* _mesa_function_pool[1334]: RasterPos4sv (offset 85) */
        "p\0"
        "glRasterPos4sv\0"
        "\0"
        /* _mesa_function_pool[1352]: Rectd (offset 86) */
        "dddd\0"
        "glRectd\0"
        "\0"
        /* _mesa_function_pool[1366]: Rectdv (offset 87) */
        "pp\0"
        "glRectdv\0"
        "\0"
        /* _mesa_function_pool[1379]: Rectf (offset 88) */
        "ffff\0"
        "glRectf\0"
        "\0"
        /* _mesa_function_pool[1393]: Rectfv (offset 89) */
        "pp\0"
        "glRectfv\0"
        "\0"
        /* _mesa_function_pool[1406]: Recti (offset 90) */
        "iiii\0"
        "glRecti\0"
        "\0"
        /* _mesa_function_pool[1420]: Rectiv (offset 91) */
        "pp\0"
        "glRectiv\0"
        "\0"
        /* _mesa_function_pool[1433]: Rects (offset 92) */
        "iiii\0"
        "glRects\0"
        "\0"
        /* _mesa_function_pool[1447]: Rectsv (offset 93) */
        "pp\0"
        "glRectsv\0"
        "\0"
        /* _mesa_function_pool[1460]: TexCoord1d (offset 94) */
        "d\0"
        "glTexCoord1d\0"
        "\0"
        /* _mesa_function_pool[1476]: TexCoord1dv (offset 95) */
        "p\0"
        "glTexCoord1dv\0"
        "\0"
        /* _mesa_function_pool[1493]: TexCoord1f (offset 96) */
        "f\0"
        "glTexCoord1f\0"
        "\0"
        /* _mesa_function_pool[1509]: TexCoord1fv (offset 97) */
        "p\0"
        "glTexCoord1fv\0"
        "\0"
        /* _mesa_function_pool[1526]: TexCoord1i (offset 98) */
        "i\0"
        "glTexCoord1i\0"
        "\0"
        /* _mesa_function_pool[1542]: TexCoord1iv (offset 99) */
        "p\0"
        "glTexCoord1iv\0"
        "\0"
        /* _mesa_function_pool[1559]: TexCoord1s (offset 100) */
        "i\0"
        "glTexCoord1s\0"
        "\0"
        /* _mesa_function_pool[1575]: TexCoord1sv (offset 101) */
        "p\0"
        "glTexCoord1sv\0"
        "\0"
        /* _mesa_function_pool[1592]: TexCoord2d (offset 102) */
        "dd\0"
        "glTexCoord2d\0"
        "\0"
        /* _mesa_function_pool[1609]: TexCoord2dv (offset 103) */
        "p\0"
        "glTexCoord2dv\0"
        "\0"
        /* _mesa_function_pool[1626]: TexCoord2f (offset 104) */
        "ff\0"
        "glTexCoord2f\0"
        "\0"
        /* _mesa_function_pool[1643]: TexCoord2fv (offset 105) */
        "p\0"
        "glTexCoord2fv\0"
        "\0"
        /* _mesa_function_pool[1660]: TexCoord2i (offset 106) */
        "ii\0"
        "glTexCoord2i\0"
        "\0"
        /* _mesa_function_pool[1677]: TexCoord2iv (offset 107) */
        "p\0"
        "glTexCoord2iv\0"
        "\0"
        /* _mesa_function_pool[1694]: TexCoord2s (offset 108) */
        "ii\0"
        "glTexCoord2s\0"
        "\0"
        /* _mesa_function_pool[1711]: TexCoord2sv (offset 109) */
        "p\0"
        "glTexCoord2sv\0"
        "\0"
        /* _mesa_function_pool[1728]: TexCoord3d (offset 110) */
        "ddd\0"
        "glTexCoord3d\0"
        "\0"
        /* _mesa_function_pool[1746]: TexCoord3dv (offset 111) */
        "p\0"
        "glTexCoord3dv\0"
        "\0"
        /* _mesa_function_pool[1763]: TexCoord3f (offset 112) */
        "fff\0"
        "glTexCoord3f\0"
        "\0"
        /* _mesa_function_pool[1781]: TexCoord3fv (offset 113) */
        "p\0"
        "glTexCoord3fv\0"
        "\0"
        /* _mesa_function_pool[1798]: TexCoord3i (offset 114) */
        "iii\0"
        "glTexCoord3i\0"
        "\0"
        /* _mesa_function_pool[1816]: TexCoord3iv (offset 115) */
        "p\0"
        "glTexCoord3iv\0"
        "\0"
        /* _mesa_function_pool[1833]: TexCoord3s (offset 116) */
        "iii\0"
        "glTexCoord3s\0"
        "\0"
        /* _mesa_function_pool[1851]: TexCoord3sv (offset 117) */
        "p\0"
        "glTexCoord3sv\0"
        "\0"
        /* _mesa_function_pool[1868]: TexCoord4d (offset 118) */
        "dddd\0"
        "glTexCoord4d\0"
        "\0"
        /* _mesa_function_pool[1887]: TexCoord4dv (offset 119) */
        "p\0"
        "glTexCoord4dv\0"
        "\0"
        /* _mesa_function_pool[1904]: TexCoord4f (offset 120) */
        "ffff\0"
        "glTexCoord4f\0"
        "\0"
        /* _mesa_function_pool[1923]: TexCoord4fv (offset 121) */
        "p\0"
        "glTexCoord4fv\0"
        "\0"
        /* _mesa_function_pool[1940]: TexCoord4i (offset 122) */
        "iiii\0"
        "glTexCoord4i\0"
        "\0"
        /* _mesa_function_pool[1959]: TexCoord4iv (offset 123) */
        "p\0"
        "glTexCoord4iv\0"
        "\0"
        /* _mesa_function_pool[1976]: TexCoord4s (offset 124) */
        "iiii\0"
        "glTexCoord4s\0"
        "\0"
        /* _mesa_function_pool[1995]: TexCoord4sv (offset 125) */
        "p\0"
        "glTexCoord4sv\0"
        "\0"
        /* _mesa_function_pool[2012]: Vertex2d (offset 126) */
        "dd\0"
        "glVertex2d\0"
        "\0"
        /* _mesa_function_pool[2027]: Vertex2dv (offset 127) */
        "p\0"
        "glVertex2dv\0"
        "\0"
        /* _mesa_function_pool[2042]: Vertex2f (offset 128) */
        "ff\0"
        "glVertex2f\0"
        "\0"
        /* _mesa_function_pool[2057]: Vertex2fv (offset 129) */
        "p\0"
        "glVertex2fv\0"
        "\0"
        /* _mesa_function_pool[2072]: Vertex2i (offset 130) */
        "ii\0"
        "glVertex2i\0"
        "\0"
        /* _mesa_function_pool[2087]: Vertex2iv (offset 131) */
        "p\0"
        "glVertex2iv\0"
        "\0"
        /* _mesa_function_pool[2102]: Vertex2s (offset 132) */
        "ii\0"
        "glVertex2s\0"
        "\0"
        /* _mesa_function_pool[2117]: Vertex2sv (offset 133) */
        "p\0"
        "glVertex2sv\0"
        "\0"
        /* _mesa_function_pool[2132]: Vertex3d (offset 134) */
        "ddd\0"
        "glVertex3d\0"
        "\0"
        /* _mesa_function_pool[2148]: Vertex3dv (offset 135) */
        "p\0"
        "glVertex3dv\0"
        "\0"
        /* _mesa_function_pool[2163]: Vertex3f (offset 136) */
        "fff\0"
        "glVertex3f\0"
        "\0"
        /* _mesa_function_pool[2179]: Vertex3fv (offset 137) */
        "p\0"
        "glVertex3fv\0"
        "\0"
        /* _mesa_function_pool[2194]: Vertex3i (offset 138) */
        "iii\0"
        "glVertex3i\0"
        "\0"
        /* _mesa_function_pool[2210]: Vertex3iv (offset 139) */
        "p\0"
        "glVertex3iv\0"
        "\0"
        /* _mesa_function_pool[2225]: Vertex3s (offset 140) */
        "iii\0"
        "glVertex3s\0"
        "\0"
        /* _mesa_function_pool[2241]: Vertex3sv (offset 141) */
        "p\0"
        "glVertex3sv\0"
        "\0"
        /* _mesa_function_pool[2256]: Vertex4d (offset 142) */
        "dddd\0"
        "glVertex4d\0"
        "\0"
        /* _mesa_function_pool[2273]: Vertex4dv (offset 143) */
        "p\0"
        "glVertex4dv\0"
        "\0"
        /* _mesa_function_pool[2288]: Vertex4f (offset 144) */
        "ffff\0"
        "glVertex4f\0"
        "\0"
        /* _mesa_function_pool[2305]: Vertex4fv (offset 145) */
        "p\0"
        "glVertex4fv\0"
        "\0"
        /* _mesa_function_pool[2320]: Vertex4i (offset 146) */
        "iiii\0"
        "glVertex4i\0"
        "\0"
        /* _mesa_function_pool[2337]: Vertex4iv (offset 147) */
        "p\0"
        "glVertex4iv\0"
        "\0"
        /* _mesa_function_pool[2352]: Vertex4s (offset 148) */
        "iiii\0"
        "glVertex4s\0"
        "\0"
        /* _mesa_function_pool[2369]: Vertex4sv (offset 149) */
        "p\0"
        "glVertex4sv\0"
        "\0"
        /* _mesa_function_pool[2384]: ClipPlane (offset 150) */
        "ip\0"
        "glClipPlane\0"
        "\0"
        /* _mesa_function_pool[2400]: ColorMaterial (offset 151) */
        "ii\0"
        "glColorMaterial\0"
        "\0"
        /* _mesa_function_pool[2420]: CullFace (offset 152) */
        "i\0"
        "glCullFace\0"
        "\0"
        /* _mesa_function_pool[2434]: Fogf (offset 153) */
        "if\0"
        "glFogf\0"
        "\0"
        /* _mesa_function_pool[2445]: Fogfv (offset 154) */
        "ip\0"
        "glFogfv\0"
        "\0"
        /* _mesa_function_pool[2457]: Fogi (offset 155) */
        "ii\0"
        "glFogi\0"
        "\0"
        /* _mesa_function_pool[2468]: Fogiv (offset 156) */
        "ip\0"
        "glFogiv\0"
        "\0"
        /* _mesa_function_pool[2480]: FrontFace (offset 157) */
        "i\0"
        "glFrontFace\0"
        "\0"
        /* _mesa_function_pool[2495]: Hint (offset 158) */
        "ii\0"
        "glHint\0"
        "\0"
        /* _mesa_function_pool[2506]: Lightf (offset 159) */
        "iif\0"
        "glLightf\0"
        "\0"
        /* _mesa_function_pool[2520]: Lightfv (offset 160) */
        "iip\0"
        "glLightfv\0"
        "\0"
        /* _mesa_function_pool[2535]: Lighti (offset 161) */
        "iii\0"
        "glLighti\0"
        "\0"
        /* _mesa_function_pool[2549]: Lightiv (offset 162) */
        "iip\0"
        "glLightiv\0"
        "\0"
        /* _mesa_function_pool[2564]: LightModelf (offset 163) */
        "if\0"
        "glLightModelf\0"
        "\0"
        /* _mesa_function_pool[2582]: LightModelfv (offset 164) */
        "ip\0"
        "glLightModelfv\0"
        "\0"
        /* _mesa_function_pool[2601]: LightModeli (offset 165) */
        "ii\0"
        "glLightModeli\0"
        "\0"
        /* _mesa_function_pool[2619]: LightModeliv (offset 166) */
        "ip\0"
        "glLightModeliv\0"
        "\0"
        /* _mesa_function_pool[2638]: LineStipple (offset 167) */
        "ii\0"
        "glLineStipple\0"
        "\0"
        /* _mesa_function_pool[2656]: LineWidth (offset 168) */
        "f\0"
        "glLineWidth\0"
        "\0"
        /* _mesa_function_pool[2671]: Materialf (offset 169) */
        "iif\0"
        "glMaterialf\0"
        "\0"
        /* _mesa_function_pool[2688]: Materialfv (offset 170) */
        "iip\0"
        "glMaterialfv\0"
        "\0"
        /* _mesa_function_pool[2706]: Materiali (offset 171) */
        "iii\0"
        "glMateriali\0"
        "\0"
        /* _mesa_function_pool[2723]: Materialiv (offset 172) */
        "iip\0"
        "glMaterialiv\0"
        "\0"
        /* _mesa_function_pool[2741]: PointSize (offset 173) */
        "f\0"
        "glPointSize\0"
        "\0"
        /* _mesa_function_pool[2756]: PolygonMode (offset 174) */
        "ii\0"
        "glPolygonMode\0"
        "\0"
        /* _mesa_function_pool[2774]: PolygonStipple (offset 175) */
        "p\0"
        "glPolygonStipple\0"
        "\0"
        /* _mesa_function_pool[2794]: Scissor (offset 176) */
        "iiii\0"
        "glScissor\0"
        "\0"
        /* _mesa_function_pool[2810]: ShadeModel (offset 177) */
        "i\0"
        "glShadeModel\0"
        "\0"
        /* _mesa_function_pool[2826]: TexParameterf (offset 178) */
        "iif\0"
        "glTexParameterf\0"
        "\0"
        /* _mesa_function_pool[2847]: TexParameterfv (offset 179) */
        "iip\0"
        "glTexParameterfv\0"
        "\0"
        /* _mesa_function_pool[2869]: TexParameteri (offset 180) */
        "iii\0"
        "glTexParameteri\0"
        "\0"
        /* _mesa_function_pool[2890]: TexParameteriv (offset 181) */
        "iip\0"
        "glTexParameteriv\0"
        "\0"
        /* _mesa_function_pool[2912]: TexImage1D (offset 182) */
        "iiiiiiip\0"
        "glTexImage1D\0"
        "\0"
        /* _mesa_function_pool[2935]: TexImage2D (offset 183) */
        "iiiiiiiip\0"
        "glTexImage2D\0"
        "\0"
        /* _mesa_function_pool[2959]: TexEnvf (offset 184) */
        "iif\0"
        "glTexEnvf\0"
        "\0"
        /* _mesa_function_pool[2974]: TexEnvfv (offset 185) */
        "iip\0"
        "glTexEnvfv\0"
        "\0"
        /* _mesa_function_pool[2990]: TexEnvi (offset 186) */
        "iii\0"
        "glTexEnvi\0"
        "\0"
        /* _mesa_function_pool[3005]: TexEnviv (offset 187) */
        "iip\0"
        "glTexEnviv\0"
        "\0"
        /* _mesa_function_pool[3021]: TexGend (offset 188) */
        "iid\0"
        "glTexGend\0"
        "\0"
        /* _mesa_function_pool[3036]: TexGendv (offset 189) */
        "iip\0"
        "glTexGendv\0"
        "\0"
        /* _mesa_function_pool[3052]: TexGenf (offset 190) */
        "iif\0"
        "glTexGenf\0"
        "\0"
        /* _mesa_function_pool[3067]: TexGenfv (offset 191) */
        "iip\0"
        "glTexGenfv\0"
        "\0"
        /* _mesa_function_pool[3083]: TexGeni (offset 192) */
        "iii\0"
        "glTexGeni\0"
        "\0"
        /* _mesa_function_pool[3098]: TexGeniv (offset 193) */
        "iip\0"
        "glTexGeniv\0"
        "\0"
        /* _mesa_function_pool[3114]: FeedbackBuffer (offset 194) */
        "iip\0"
        "glFeedbackBuffer\0"
        "\0"
        /* _mesa_function_pool[3136]: SelectBuffer (offset 195) */
        "ip\0"
        "glSelectBuffer\0"
        "\0"
        /* _mesa_function_pool[3155]: RenderMode (offset 196) */
        "i\0"
        "glRenderMode\0"
        "\0"
        /* _mesa_function_pool[3171]: InitNames (offset 197) */
        "\0"
        "glInitNames\0"
        "\0"
        /* _mesa_function_pool[3185]: LoadName (offset 198) */
        "i\0"
        "glLoadName\0"
        "\0"
        /* _mesa_function_pool[3199]: PassThrough (offset 199) */
        "f\0"
        "glPassThrough\0"
        "\0"
        /* _mesa_function_pool[3216]: PopName (offset 200) */
        "\0"
        "glPopName\0"
        "\0"
        /* _mesa_function_pool[3228]: PushName (offset 201) */
        "i\0"
        "glPushName\0"
        "\0"
        /* _mesa_function_pool[3242]: DrawBuffer (offset 202) */
        "i\0"
        "glDrawBuffer\0"
        "\0"
        /* _mesa_function_pool[3258]: Clear (offset 203) */
        "i\0"
        "glClear\0"
        "\0"
        /* _mesa_function_pool[3269]: ClearAccum (offset 204) */
        "ffff\0"
        "glClearAccum\0"
        "\0"
        /* _mesa_function_pool[3288]: ClearIndex (offset 205) */
        "f\0"
        "glClearIndex\0"
        "\0"
        /* _mesa_function_pool[3304]: ClearColor (offset 206) */
        "ffff\0"
        "glClearColor\0"
        "\0"
        /* _mesa_function_pool[3323]: ClearStencil (offset 207) */
        "i\0"
        "glClearStencil\0"
        "\0"
        /* _mesa_function_pool[3341]: ClearDepth (offset 208) */
        "d\0"
        "glClearDepth\0"
        "\0"
        /* _mesa_function_pool[3357]: StencilMask (offset 209) */
        "i\0"
        "glStencilMask\0"
        "\0"
        /* _mesa_function_pool[3374]: ColorMask (offset 210) */
        "iiii\0"
        "glColorMask\0"
        "\0"
        /* _mesa_function_pool[3392]: DepthMask (offset 211) */
        "i\0"
        "glDepthMask\0"
        "\0"
        /* _mesa_function_pool[3407]: IndexMask (offset 212) */
        "i\0"
        "glIndexMask\0"
        "\0"
        /* _mesa_function_pool[3422]: Accum (offset 213) */
        "if\0"
        "glAccum\0"
        "\0"
        /* _mesa_function_pool[3434]: Disable (offset 214) */
        "i\0"
        "glDisable\0"
        "\0"
        /* _mesa_function_pool[3447]: Enable (offset 215) */
        "i\0"
        "glEnable\0"
        "\0"
        /* _mesa_function_pool[3459]: Finish (offset 216) */
        "\0"
        "glFinish\0"
        "\0"
        /* _mesa_function_pool[3470]: Flush (offset 217) */
        "\0"
        "glFlush\0"
        "\0"
        /* _mesa_function_pool[3480]: PopAttrib (offset 218) */
        "\0"
        "glPopAttrib\0"
        "\0"
        /* _mesa_function_pool[3494]: PushAttrib (offset 219) */
        "i\0"
        "glPushAttrib\0"
        "\0"
        /* _mesa_function_pool[3510]: Map1d (offset 220) */
        "iddiip\0"
        "glMap1d\0"
        "\0"
        /* _mesa_function_pool[3526]: Map1f (offset 221) */
        "iffiip\0"
        "glMap1f\0"
        "\0"
        /* _mesa_function_pool[3542]: Map2d (offset 222) */
        "iddiiddiip\0"
        "glMap2d\0"
        "\0"
        /* _mesa_function_pool[3562]: Map2f (offset 223) */
        "iffiiffiip\0"
        "glMap2f\0"
        "\0"
        /* _mesa_function_pool[3582]: MapGrid1d (offset 224) */
        "idd\0"
        "glMapGrid1d\0"
        "\0"
        /* _mesa_function_pool[3599]: MapGrid1f (offset 225) */
        "iff\0"
        "glMapGrid1f\0"
        "\0"
        /* _mesa_function_pool[3616]: MapGrid2d (offset 226) */
        "iddidd\0"
        "glMapGrid2d\0"
        "\0"
        /* _mesa_function_pool[3636]: MapGrid2f (offset 227) */
        "iffiff\0"
        "glMapGrid2f\0"
        "\0"
        /* _mesa_function_pool[3656]: EvalCoord1d (offset 228) */
        "d\0"
        "glEvalCoord1d\0"
        "\0"
        /* _mesa_function_pool[3673]: EvalCoord1dv (offset 229) */
        "p\0"
        "glEvalCoord1dv\0"
        "\0"
        /* _mesa_function_pool[3691]: EvalCoord1f (offset 230) */
        "f\0"
        "glEvalCoord1f\0"
        "\0"
        /* _mesa_function_pool[3708]: EvalCoord1fv (offset 231) */
        "p\0"
        "glEvalCoord1fv\0"
        "\0"
        /* _mesa_function_pool[3726]: EvalCoord2d (offset 232) */
        "dd\0"
        "glEvalCoord2d\0"
        "\0"
        /* _mesa_function_pool[3744]: EvalCoord2dv (offset 233) */
        "p\0"
        "glEvalCoord2dv\0"
        "\0"
        /* _mesa_function_pool[3762]: EvalCoord2f (offset 234) */
        "ff\0"
        "glEvalCoord2f\0"
        "\0"
        /* _mesa_function_pool[3780]: EvalCoord2fv (offset 235) */
        "p\0"
        "glEvalCoord2fv\0"
        "\0"
        /* _mesa_function_pool[3798]: EvalMesh1 (offset 236) */
        "iii\0"
        "glEvalMesh1\0"
        "\0"
        /* _mesa_function_pool[3815]: EvalPoint1 (offset 237) */
        "i\0"
        "glEvalPoint1\0"
        "\0"
        /* _mesa_function_pool[3831]: EvalMesh2 (offset 238) */
        "iiiii\0"
        "glEvalMesh2\0"
        "\0"
        /* _mesa_function_pool[3850]: EvalPoint2 (offset 239) */
        "ii\0"
        "glEvalPoint2\0"
        "\0"
        /* _mesa_function_pool[3867]: AlphaFunc (offset 240) */
        "if\0"
        "glAlphaFunc\0"
        "\0"
        /* _mesa_function_pool[3883]: BlendFunc (offset 241) */
        "ii\0"
        "glBlendFunc\0"
        "\0"
        /* _mesa_function_pool[3899]: LogicOp (offset 242) */
        "i\0"
        "glLogicOp\0"
        "\0"
        /* _mesa_function_pool[3912]: StencilFunc (offset 243) */
        "iii\0"
        "glStencilFunc\0"
        "\0"
        /* _mesa_function_pool[3931]: StencilOp (offset 244) */
        "iii\0"
        "glStencilOp\0"
        "\0"
        /* _mesa_function_pool[3948]: DepthFunc (offset 245) */
        "i\0"
        "glDepthFunc\0"
        "\0"
        /* _mesa_function_pool[3963]: PixelZoom (offset 246) */
        "ff\0"
        "glPixelZoom\0"
        "\0"
        /* _mesa_function_pool[3979]: PixelTransferf (offset 247) */
        "if\0"
        "glPixelTransferf\0"
        "\0"
        /* _mesa_function_pool[4000]: PixelTransferi (offset 248) */
        "ii\0"
        "glPixelTransferi\0"
        "\0"
        /* _mesa_function_pool[4021]: PixelStoref (offset 249) */
        "if\0"
        "glPixelStoref\0"
        "\0"
        /* _mesa_function_pool[4039]: PixelStorei (offset 250) */
        "ii\0"
        "glPixelStorei\0"
        "\0"
        /* _mesa_function_pool[4057]: PixelMapfv (offset 251) */
        "iip\0"
        "glPixelMapfv\0"
        "\0"
        /* _mesa_function_pool[4075]: PixelMapuiv (offset 252) */
        "iip\0"
        "glPixelMapuiv\0"
        "\0"
        /* _mesa_function_pool[4094]: PixelMapusv (offset 253) */
        "iip\0"
        "glPixelMapusv\0"
        "\0"
        /* _mesa_function_pool[4113]: ReadBuffer (offset 254) */
        "i\0"
        "glReadBuffer\0"
        "\0"
        /* _mesa_function_pool[4129]: CopyPixels (offset 255) */
        "iiiii\0"
        "glCopyPixels\0"
        "\0"
        /* _mesa_function_pool[4149]: ReadPixels (offset 256) */
        "iiiiiip\0"
        "glReadPixels\0"
        "\0"
        /* _mesa_function_pool[4171]: DrawPixels (offset 257) */
        "iiiip\0"
        "glDrawPixels\0"
        "\0"
        /* _mesa_function_pool[4191]: GetBooleanv (offset 258) */
        "ip\0"
        "glGetBooleanv\0"
        "\0"
        /* _mesa_function_pool[4209]: GetClipPlane (offset 259) */
        "ip\0"
        "glGetClipPlane\0"
        "\0"
        /* _mesa_function_pool[4228]: GetDoublev (offset 260) */
        "ip\0"
        "glGetDoublev\0"
        "\0"
        /* _mesa_function_pool[4245]: GetError (offset 261) */
        "\0"
        "glGetError\0"
        "\0"
        /* _mesa_function_pool[4258]: GetFloatv (offset 262) */
        "ip\0"
        "glGetFloatv\0"
        "\0"
        /* _mesa_function_pool[4274]: GetIntegerv (offset 263) */
        "ip\0"
        "glGetIntegerv\0"
        "\0"
        /* _mesa_function_pool[4292]: GetLightfv (offset 264) */
        "iip\0"
        "glGetLightfv\0"
        "\0"
        /* _mesa_function_pool[4310]: GetLightiv (offset 265) */
        "iip\0"
        "glGetLightiv\0"
        "\0"
        /* _mesa_function_pool[4328]: GetMapdv (offset 266) */
        "iip\0"
        "glGetMapdv\0"
        "\0"
        /* _mesa_function_pool[4344]: GetMapfv (offset 267) */
        "iip\0"
        "glGetMapfv\0"
        "\0"
        /* _mesa_function_pool[4360]: GetMapiv (offset 268) */
        "iip\0"
        "glGetMapiv\0"
        "\0"
        /* _mesa_function_pool[4376]: GetMaterialfv (offset 269) */
        "iip\0"
        "glGetMaterialfv\0"
        "\0"
        /* _mesa_function_pool[4397]: GetMaterialiv (offset 270) */
        "iip\0"
        "glGetMaterialiv\0"
        "\0"
        /* _mesa_function_pool[4418]: GetPixelMapfv (offset 271) */
        "ip\0"
        "glGetPixelMapfv\0"
        "\0"
        /* _mesa_function_pool[4438]: GetPixelMapuiv (offset 272) */
        "ip\0"
        "glGetPixelMapuiv\0"
        "\0"
        /* _mesa_function_pool[4459]: GetPixelMapusv (offset 273) */
        "ip\0"
        "glGetPixelMapusv\0"
        "\0"
        /* _mesa_function_pool[4480]: GetPolygonStipple (offset 274) */
        "p\0"
        "glGetPolygonStipple\0"
        "\0"
        /* _mesa_function_pool[4503]: GetString (offset 275) */
        "i\0"
        "glGetString\0"
        "\0"
        /* _mesa_function_pool[4518]: GetTexEnvfv (offset 276) */
        "iip\0"
        "glGetTexEnvfv\0"
        "\0"
        /* _mesa_function_pool[4537]: GetTexEnviv (offset 277) */
        "iip\0"
        "glGetTexEnviv\0"
        "\0"
        /* _mesa_function_pool[4556]: GetTexGendv (offset 278) */
        "iip\0"
        "glGetTexGendv\0"
        "\0"
        /* _mesa_function_pool[4575]: GetTexGenfv (offset 279) */
        "iip\0"
        "glGetTexGenfv\0"
        "\0"
        /* _mesa_function_pool[4594]: GetTexGeniv (offset 280) */
        "iip\0"
        "glGetTexGeniv\0"
        "\0"
        /* _mesa_function_pool[4613]: GetTexImage (offset 281) */
        "iiiip\0"
        "glGetTexImage\0"
        "\0"
        /* _mesa_function_pool[4634]: GetTexParameterfv (offset 282) */
        "iip\0"
        "glGetTexParameterfv\0"
        "\0"
        /* _mesa_function_pool[4659]: GetTexParameteriv (offset 283) */
        "iip\0"
        "glGetTexParameteriv\0"
        "\0"
        /* _mesa_function_pool[4684]: GetTexLevelParameterfv (offset 284) */
        "iiip\0"
        "glGetTexLevelParameterfv\0"
        "\0"
        /* _mesa_function_pool[4715]: GetTexLevelParameteriv (offset 285) */
        "iiip\0"
        "glGetTexLevelParameteriv\0"
        "\0"
        /* _mesa_function_pool[4746]: IsEnabled (offset 286) */
        "i\0"
        "glIsEnabled\0"
        "\0"
        /* _mesa_function_pool[4761]: IsList (offset 287) */
        "i\0"
        "glIsList\0"
        "\0"
        /* _mesa_function_pool[4773]: DepthRange (offset 288) */
        "dd\0"
        "glDepthRange\0"
        "\0"
        /* _mesa_function_pool[4790]: Frustum (offset 289) */
        "dddddd\0"
        "glFrustum\0"
        "\0"
        /* _mesa_function_pool[4808]: LoadIdentity (offset 290) */
        "\0"
        "glLoadIdentity\0"
        "\0"
        /* _mesa_function_pool[4825]: LoadMatrixf (offset 291) */
        "p\0"
        "glLoadMatrixf\0"
        "\0"
        /* _mesa_function_pool[4842]: LoadMatrixd (offset 292) */
        "p\0"
        "glLoadMatrixd\0"
        "\0"
        /* _mesa_function_pool[4859]: MatrixMode (offset 293) */
        "i\0"
        "glMatrixMode\0"
        "\0"
        /* _mesa_function_pool[4875]: MultMatrixf (offset 294) */
        "p\0"
        "glMultMatrixf\0"
        "\0"
        /* _mesa_function_pool[4892]: MultMatrixd (offset 295) */
        "p\0"
        "glMultMatrixd\0"
        "\0"
        /* _mesa_function_pool[4909]: Ortho (offset 296) */
        "dddddd\0"
        "glOrtho\0"
        "\0"
        /* _mesa_function_pool[4925]: PopMatrix (offset 297) */
        "\0"
        "glPopMatrix\0"
        "\0"
        /* _mesa_function_pool[4939]: PushMatrix (offset 298) */
        "\0"
        "glPushMatrix\0"
        "\0"
        /* _mesa_function_pool[4954]: Rotated (offset 299) */
        "dddd\0"
        "glRotated\0"
        "\0"
        /* _mesa_function_pool[4970]: Rotatef (offset 300) */
        "ffff\0"
        "glRotatef\0"
        "\0"
        /* _mesa_function_pool[4986]: Scaled (offset 301) */
        "ddd\0"
        "glScaled\0"
        "\0"
        /* _mesa_function_pool[5000]: Scalef (offset 302) */
        "fff\0"
        "glScalef\0"
        "\0"
        /* _mesa_function_pool[5014]: Translated (offset 303) */
        "ddd\0"
        "glTranslated\0"
        "\0"
        /* _mesa_function_pool[5032]: Translatef (offset 304) */
        "fff\0"
        "glTranslatef\0"
        "\0"
        /* _mesa_function_pool[5050]: Viewport (offset 305) */
        "iiii\0"
        "glViewport\0"
        "\0"
        /* _mesa_function_pool[5067]: ArrayElement (offset 306) */
        "i\0"
        "glArrayElement\0"
        "glArrayElementEXT\0"
        "\0"
        /* _mesa_function_pool[5103]: ColorPointer (offset 308) */
        "iiip\0"
        "glColorPointer\0"
        "\0"
        /* _mesa_function_pool[5124]: DisableClientState (offset 309) */
        "i\0"
        "glDisableClientState\0"
        "\0"
        /* _mesa_function_pool[5148]: DrawArrays (offset 310) */
        "iii\0"
        "glDrawArrays\0"
        "glDrawArraysEXT\0"
        "\0"
        /* _mesa_function_pool[5182]: DrawElements (offset 311) */
        "iiip\0"
        "glDrawElements\0"
        "\0"
        /* _mesa_function_pool[5203]: EdgeFlagPointer (offset 312) */
        "ip\0"
        "glEdgeFlagPointer\0"
        "\0"
        /* _mesa_function_pool[5225]: EnableClientState (offset 313) */
        "i\0"
        "glEnableClientState\0"
        "\0"
        /* _mesa_function_pool[5248]: GetPointerv (offset 329) */
        "ip\0"
        "glGetPointerv\0"
        "glGetPointervKHR\0"
        "glGetPointervEXT\0"
        "\0"
        /* _mesa_function_pool[5300]: IndexPointer (offset 314) */
        "iip\0"
        "glIndexPointer\0"
        "\0"
        /* _mesa_function_pool[5320]: InterleavedArrays (offset 317) */
        "iip\0"
        "glInterleavedArrays\0"
        "\0"
        /* _mesa_function_pool[5345]: NormalPointer (offset 318) */
        "iip\0"
        "glNormalPointer\0"
        "\0"
        /* _mesa_function_pool[5366]: TexCoordPointer (offset 320) */
        "iiip\0"
        "glTexCoordPointer\0"
        "\0"
        /* _mesa_function_pool[5390]: VertexPointer (offset 321) */
        "iiip\0"
        "glVertexPointer\0"
        "\0"
        /* _mesa_function_pool[5412]: PolygonOffset (offset 319) */
        "ff\0"
        "glPolygonOffset\0"
        "\0"
        /* _mesa_function_pool[5432]: CopyTexImage1D (offset 323) */
        "iiiiiii\0"
        "glCopyTexImage1D\0"
        "glCopyTexImage1DEXT\0"
        "\0"
        /* _mesa_function_pool[5478]: CopyTexImage2D (offset 324) */
        "iiiiiiii\0"
        "glCopyTexImage2D\0"
        "glCopyTexImage2DEXT\0"
        "\0"
        /* _mesa_function_pool[5525]: CopyTexSubImage1D (offset 325) */
        "iiiiii\0"
        "glCopyTexSubImage1D\0"
        "glCopyTexSubImage1DEXT\0"
        "\0"
        /* _mesa_function_pool[5576]: CopyTexSubImage2D (offset 326) */
        "iiiiiiii\0"
        "glCopyTexSubImage2D\0"
        "glCopyTexSubImage2DEXT\0"
        "\0"
        /* _mesa_function_pool[5629]: TexSubImage1D (offset 332) */
        "iiiiiip\0"
        "glTexSubImage1D\0"
        "glTexSubImage1DEXT\0"
        "\0"
        /* _mesa_function_pool[5673]: TexSubImage2D (offset 333) */
        "iiiiiiiip\0"
        "glTexSubImage2D\0"
        "glTexSubImage2DEXT\0"
        "\0"
        /* _mesa_function_pool[5719]: AreTexturesResident (offset 322) */
        "ipp\0"
        "glAreTexturesResident\0"
        "glAreTexturesResidentEXT\0"
        "\0"
        /* _mesa_function_pool[5771]: BindTexture (offset 307) */
        "ii\0"
        "glBindTexture\0"
        "glBindTextureEXT\0"
        "\0"
        /* _mesa_function_pool[5806]: DeleteTextures (offset 327) */
        "ip\0"
        "glDeleteTextures\0"
        "glDeleteTexturesEXT\0"
        "\0"
        /* _mesa_function_pool[5847]: GenTextures (offset 328) */
        "ip\0"
        "glGenTextures\0"
        "glGenTexturesEXT\0"
        "\0"
        /* _mesa_function_pool[5882]: IsTexture (offset 330) */
        "i\0"
        "glIsTexture\0"
        "glIsTextureEXT\0"
        "\0"
        /* _mesa_function_pool[5912]: PrioritizeTextures (offset 331) */
        "ipp\0"
        "glPrioritizeTextures\0"
        "glPrioritizeTexturesEXT\0"
        "\0"
        /* _mesa_function_pool[5962]: Indexub (offset 315) */
        "i\0"
        "glIndexub\0"
        "\0"
        /* _mesa_function_pool[5975]: Indexubv (offset 316) */
        "p\0"
        "glIndexubv\0"
        "\0"
        /* _mesa_function_pool[5989]: PopClientAttrib (offset 334) */
        "\0"
        "glPopClientAttrib\0"
        "\0"
        /* _mesa_function_pool[6009]: PushClientAttrib (offset 335) */
        "i\0"
        "glPushClientAttrib\0"
        "\0"
        /* _mesa_function_pool[6031]: BlendColor (offset 336) */
        "ffff\0"
        "glBlendColor\0"
        "glBlendColorEXT\0"
        "\0"
        /* _mesa_function_pool[6066]: BlendEquation (offset 337) */
        "i\0"
        "glBlendEquation\0"
        "glBlendEquationEXT\0"
        "\0"
        /* _mesa_function_pool[6104]: DrawRangeElements (offset 338) */
        "iiiiip\0"
        "glDrawRangeElements\0"
        "glDrawRangeElementsEXT\0"
        "\0"
        /* _mesa_function_pool[6155]: ColorTable (offset 339) */
        "iiiiip\0"
        "glColorTable\0"
        "glColorTableSGI\0"
        "glColorTableEXT\0"
        "\0"
        /* _mesa_function_pool[6208]: ColorTableParameterfv (offset 340) */
        "iip\0"
        "glColorTableParameterfv\0"
        "glColorTableParameterfvSGI\0"
        "\0"
        /* _mesa_function_pool[6264]: ColorTableParameteriv (offset 341) */
        "iip\0"
        "glColorTableParameteriv\0"
        "glColorTableParameterivSGI\0"
        "\0"
        /* _mesa_function_pool[6320]: CopyColorTable (offset 342) */
        "iiiii\0"
        "glCopyColorTable\0"
        "glCopyColorTableSGI\0"
        "\0"
        /* _mesa_function_pool[6364]: GetColorTable (offset 343) */
        "iiip\0"
        "glGetColorTable\0"
        "glGetColorTableSGI\0"
        "glGetColorTableEXT\0"
        "\0"
        /* _mesa_function_pool[6424]: GetColorTableParameterfv (offset 344) */
        "iip\0"
        "glGetColorTableParameterfv\0"
        "glGetColorTableParameterfvSGI\0"
        "glGetColorTableParameterfvEXT\0"
        "\0"
        /* _mesa_function_pool[6516]: GetColorTableParameteriv (offset 345) */
        "iip\0"
        "glGetColorTableParameteriv\0"
        "glGetColorTableParameterivSGI\0"
        "glGetColorTableParameterivEXT\0"
        "\0"
        /* _mesa_function_pool[6608]: ColorSubTable (offset 346) */
        "iiiiip\0"
        "glColorSubTable\0"
        "glColorSubTableEXT\0"
        "\0"
        /* _mesa_function_pool[6651]: CopyColorSubTable (offset 347) */
        "iiiii\0"
        "glCopyColorSubTable\0"
        "glCopyColorSubTableEXT\0"
        "\0"
        /* _mesa_function_pool[6701]: ConvolutionFilter1D (offset 348) */
        "iiiiip\0"
        "glConvolutionFilter1D\0"
        "glConvolutionFilter1DEXT\0"
        "\0"
        /* _mesa_function_pool[6756]: ConvolutionFilter2D (offset 349) */
        "iiiiiip\0"
        "glConvolutionFilter2D\0"
        "glConvolutionFilter2DEXT\0"
        "\0"
        /* _mesa_function_pool[6812]: ConvolutionParameterf (offset 350) */
        "iif\0"
        "glConvolutionParameterf\0"
        "glConvolutionParameterfEXT\0"
        "\0"
        /* _mesa_function_pool[6868]: ConvolutionParameterfv (offset 351) */
        "iip\0"
        "glConvolutionParameterfv\0"
        "glConvolutionParameterfvEXT\0"
        "\0"
        /* _mesa_function_pool[6926]: ConvolutionParameteri (offset 352) */
        "iii\0"
        "glConvolutionParameteri\0"
        "glConvolutionParameteriEXT\0"
        "\0"
        /* _mesa_function_pool[6982]: ConvolutionParameteriv (offset 353) */
        "iip\0"
        "glConvolutionParameteriv\0"
        "glConvolutionParameterivEXT\0"
        "\0"
        /* _mesa_function_pool[7040]: CopyConvolutionFilter1D (offset 354) */
        "iiiii\0"
        "glCopyConvolutionFilter1D\0"
        "glCopyConvolutionFilter1DEXT\0"
        "\0"
        /* _mesa_function_pool[7102]: CopyConvolutionFilter2D (offset 355) */
        "iiiiii\0"
        "glCopyConvolutionFilter2D\0"
        "glCopyConvolutionFilter2DEXT\0"
        "\0"
        /* _mesa_function_pool[7165]: GetConvolutionFilter (offset 356) */
        "iiip\0"
        "glGetConvolutionFilter\0"
        "glGetConvolutionFilterEXT\0"
        "\0"
        /* _mesa_function_pool[7220]: GetConvolutionParameterfv (offset 357) */
        "iip\0"
        "glGetConvolutionParameterfv\0"
        "glGetConvolutionParameterfvEXT\0"
        "\0"
        /* _mesa_function_pool[7284]: GetConvolutionParameteriv (offset 358) */
        "iip\0"
        "glGetConvolutionParameteriv\0"
        "glGetConvolutionParameterivEXT\0"
        "\0"
        /* _mesa_function_pool[7348]: GetSeparableFilter (offset 359) */
        "iiippp\0"
        "glGetSeparableFilter\0"
        "glGetSeparableFilterEXT\0"
        "\0"
        /* _mesa_function_pool[7401]: SeparableFilter2D (offset 360) */
        "iiiiiipp\0"
        "glSeparableFilter2D\0"
        "glSeparableFilter2DEXT\0"
        "\0"
        /* _mesa_function_pool[7454]: GetHistogram (offset 361) */
        "iiiip\0"
        "glGetHistogram\0"
        "glGetHistogramEXT\0"
        "\0"
        /* _mesa_function_pool[7494]: GetHistogramParameterfv (offset 362) */
        "iip\0"
        "glGetHistogramParameterfv\0"
        "glGetHistogramParameterfvEXT\0"
        "\0"
        /* _mesa_function_pool[7554]: GetHistogramParameteriv (offset 363) */
        "iip\0"
        "glGetHistogramParameteriv\0"
        "glGetHistogramParameterivEXT\0"
        "\0"
        /* _mesa_function_pool[7614]: GetMinmax (offset 364) */
        "iiiip\0"
        "glGetMinmax\0"
        "glGetMinmaxEXT\0"
        "\0"
        /* _mesa_function_pool[7648]: GetMinmaxParameterfv (offset 365) */
        "iip\0"
        "glGetMinmaxParameterfv\0"
        "glGetMinmaxParameterfvEXT\0"
        "\0"
        /* _mesa_function_pool[7702]: GetMinmaxParameteriv (offset 366) */
        "iip\0"
        "glGetMinmaxParameteriv\0"
        "glGetMinmaxParameterivEXT\0"
        "\0"
        /* _mesa_function_pool[7756]: Histogram (offset 367) */
        "iiii\0"
        "glHistogram\0"
        "glHistogramEXT\0"
        "\0"
        /* _mesa_function_pool[7789]: Minmax (offset 368) */
        "iii\0"
        "glMinmax\0"
        "glMinmaxEXT\0"
        "\0"
        /* _mesa_function_pool[7815]: ResetHistogram (offset 369) */
        "i\0"
        "glResetHistogram\0"
        "glResetHistogramEXT\0"
        "\0"
        /* _mesa_function_pool[7855]: ResetMinmax (offset 370) */
        "i\0"
        "glResetMinmax\0"
        "glResetMinmaxEXT\0"
        "\0"
        /* _mesa_function_pool[7889]: TexImage3D (offset 371) */
        "iiiiiiiiip\0"
        "glTexImage3D\0"
        "glTexImage3DEXT\0"
        "\0"
        /* _mesa_function_pool[7930]: TexSubImage3D (offset 372) */
        "iiiiiiiiiip\0"
        "glTexSubImage3D\0"
        "glTexSubImage3DEXT\0"
        "\0"
        /* _mesa_function_pool[7978]: CopyTexSubImage3D (offset 373) */
        "iiiiiiiii\0"
        "glCopyTexSubImage3D\0"
        "glCopyTexSubImage3DEXT\0"
        "\0"
        /* _mesa_function_pool[8032]: ActiveTexture (offset 374) */
        "i\0"
        "glActiveTexture\0"
        "glActiveTextureARB\0"
        "\0"
        /* _mesa_function_pool[8070]: ClientActiveTexture (offset 375) */
        "i\0"
        "glClientActiveTexture\0"
        "glClientActiveTextureARB\0"
        "\0"
        /* _mesa_function_pool[8120]: MultiTexCoord1d (offset 376) */
        "id\0"
        "glMultiTexCoord1d\0"
        "glMultiTexCoord1dARB\0"
        "\0"
        /* _mesa_function_pool[8163]: MultiTexCoord1dv (offset 377) */
        "ip\0"
        "glMultiTexCoord1dv\0"
        "glMultiTexCoord1dvARB\0"
        "\0"
        /* _mesa_function_pool[8208]: MultiTexCoord1fARB (offset 378) */
        "if\0"
        "glMultiTexCoord1f\0"
        "glMultiTexCoord1fARB\0"
        "\0"
        /* _mesa_function_pool[8251]: MultiTexCoord1fvARB (offset 379) */
        "ip\0"
        "glMultiTexCoord1fv\0"
        "glMultiTexCoord1fvARB\0"
        "\0"
        /* _mesa_function_pool[8296]: MultiTexCoord1i (offset 380) */
        "ii\0"
        "glMultiTexCoord1i\0"
        "glMultiTexCoord1iARB\0"
        "\0"
        /* _mesa_function_pool[8339]: MultiTexCoord1iv (offset 381) */
        "ip\0"
        "glMultiTexCoord1iv\0"
        "glMultiTexCoord1ivARB\0"
        "\0"
        /* _mesa_function_pool[8384]: MultiTexCoord1s (offset 382) */
        "ii\0"
        "glMultiTexCoord1s\0"
        "glMultiTexCoord1sARB\0"
        "\0"
        /* _mesa_function_pool[8427]: MultiTexCoord1sv (offset 383) */
        "ip\0"
        "glMultiTexCoord1sv\0"
        "glMultiTexCoord1svARB\0"
        "\0"
        /* _mesa_function_pool[8472]: MultiTexCoord2d (offset 384) */
        "idd\0"
        "glMultiTexCoord2d\0"
        "glMultiTexCoord2dARB\0"
        "\0"
        /* _mesa_function_pool[8516]: MultiTexCoord2dv (offset 385) */
        "ip\0"
        "glMultiTexCoord2dv\0"
        "glMultiTexCoord2dvARB\0"
        "\0"
        /* _mesa_function_pool[8561]: MultiTexCoord2fARB (offset 386) */
        "iff\0"
        "glMultiTexCoord2f\0"
        "glMultiTexCoord2fARB\0"
        "\0"
        /* _mesa_function_pool[8605]: MultiTexCoord2fvARB (offset 387) */
        "ip\0"
        "glMultiTexCoord2fv\0"
        "glMultiTexCoord2fvARB\0"
        "\0"
        /* _mesa_function_pool[8650]: MultiTexCoord2i (offset 388) */
        "iii\0"
        "glMultiTexCoord2i\0"
        "glMultiTexCoord2iARB\0"
        "\0"
        /* _mesa_function_pool[8694]: MultiTexCoord2iv (offset 389) */
        "ip\0"
        "glMultiTexCoord2iv\0"
        "glMultiTexCoord2ivARB\0"
        "\0"
        /* _mesa_function_pool[8739]: MultiTexCoord2s (offset 390) */
        "iii\0"
        "glMultiTexCoord2s\0"
        "glMultiTexCoord2sARB\0"
        "\0"
        /* _mesa_function_pool[8783]: MultiTexCoord2sv (offset 391) */
        "ip\0"
        "glMultiTexCoord2sv\0"
        "glMultiTexCoord2svARB\0"
        "\0"
        /* _mesa_function_pool[8828]: MultiTexCoord3d (offset 392) */
        "iddd\0"
        "glMultiTexCoord3d\0"
        "glMultiTexCoord3dARB\0"
        "\0"
        /* _mesa_function_pool[8873]: MultiTexCoord3dv (offset 393) */
        "ip\0"
        "glMultiTexCoord3dv\0"
        "glMultiTexCoord3dvARB\0"
        "\0"
        /* _mesa_function_pool[8918]: MultiTexCoord3fARB (offset 394) */
        "ifff\0"
        "glMultiTexCoord3f\0"
        "glMultiTexCoord3fARB\0"
        "\0"
        /* _mesa_function_pool[8963]: MultiTexCoord3fvARB (offset 395) */
        "ip\0"
        "glMultiTexCoord3fv\0"
        "glMultiTexCoord3fvARB\0"
        "\0"
        /* _mesa_function_pool[9008]: MultiTexCoord3i (offset 396) */
        "iiii\0"
        "glMultiTexCoord3i\0"
        "glMultiTexCoord3iARB\0"
        "\0"
        /* _mesa_function_pool[9053]: MultiTexCoord3iv (offset 397) */
        "ip\0"
        "glMultiTexCoord3iv\0"
        "glMultiTexCoord3ivARB\0"
        "\0"
        /* _mesa_function_pool[9098]: MultiTexCoord3s (offset 398) */
        "iiii\0"
        "glMultiTexCoord3s\0"
        "glMultiTexCoord3sARB\0"
        "\0"
        /* _mesa_function_pool[9143]: MultiTexCoord3sv (offset 399) */
        "ip\0"
        "glMultiTexCoord3sv\0"
        "glMultiTexCoord3svARB\0"
        "\0"
        /* _mesa_function_pool[9188]: MultiTexCoord4d (offset 400) */
        "idddd\0"
        "glMultiTexCoord4d\0"
        "glMultiTexCoord4dARB\0"
        "\0"
        /* _mesa_function_pool[9234]: MultiTexCoord4dv (offset 401) */
        "ip\0"
        "glMultiTexCoord4dv\0"
        "glMultiTexCoord4dvARB\0"
        "\0"
        /* _mesa_function_pool[9279]: MultiTexCoord4fARB (offset 402) */
        "iffff\0"
        "glMultiTexCoord4f\0"
        "glMultiTexCoord4fARB\0"
        "\0"
        /* _mesa_function_pool[9325]: MultiTexCoord4fvARB (offset 403) */
        "ip\0"
        "glMultiTexCoord4fv\0"
        "glMultiTexCoord4fvARB\0"
        "\0"
        /* _mesa_function_pool[9370]: MultiTexCoord4i (offset 404) */
        "iiiii\0"
        "glMultiTexCoord4i\0"
        "glMultiTexCoord4iARB\0"
        "\0"
        /* _mesa_function_pool[9416]: MultiTexCoord4iv (offset 405) */
        "ip\0"
        "glMultiTexCoord4iv\0"
        "glMultiTexCoord4ivARB\0"
        "\0"
        /* _mesa_function_pool[9461]: MultiTexCoord4s (offset 406) */
        "iiiii\0"
        "glMultiTexCoord4s\0"
        "glMultiTexCoord4sARB\0"
        "\0"
        /* _mesa_function_pool[9507]: MultiTexCoord4sv (offset 407) */
        "ip\0"
        "glMultiTexCoord4sv\0"
        "glMultiTexCoord4svARB\0"
        "\0"
        /* _mesa_function_pool[9552]: LoadTransposeMatrixf (will be remapped) */
        "p\0"
        "glLoadTransposeMatrixf\0"
        "glLoadTransposeMatrixfARB\0"
        "\0"
        /* _mesa_function_pool[9604]: LoadTransposeMatrixd (will be remapped) */
        "p\0"
        "glLoadTransposeMatrixd\0"
        "glLoadTransposeMatrixdARB\0"
        "\0"
        /* _mesa_function_pool[9656]: MultTransposeMatrixf (will be remapped) */
        "p\0"
        "glMultTransposeMatrixf\0"
        "glMultTransposeMatrixfARB\0"
        "\0"
        /* _mesa_function_pool[9708]: MultTransposeMatrixd (will be remapped) */
        "p\0"
        "glMultTransposeMatrixd\0"
        "glMultTransposeMatrixdARB\0"
        "\0"
        /* _mesa_function_pool[9760]: SampleCoverage (will be remapped) */
        "fi\0"
        "glSampleCoverage\0"
        "glSampleCoverageARB\0"
        "\0"
        /* _mesa_function_pool[9801]: CompressedTexImage3D (will be remapped) */
        "iiiiiiiip\0"
        "glCompressedTexImage3D\0"
        "glCompressedTexImage3DARB\0"
        "\0"
        /* _mesa_function_pool[9861]: CompressedTexImage2D (will be remapped) */
        "iiiiiiip\0"
        "glCompressedTexImage2D\0"
        "glCompressedTexImage2DARB\0"
        "\0"
        /* _mesa_function_pool[9920]: CompressedTexImage1D (will be remapped) */
        "iiiiiip\0"
        "glCompressedTexImage1D\0"
        "glCompressedTexImage1DARB\0"
        "\0"
        /* _mesa_function_pool[9978]: CompressedTexSubImage3D (will be remapped) */
        "iiiiiiiiiip\0"
        "glCompressedTexSubImage3D\0"
        "glCompressedTexSubImage3DARB\0"
        "\0"
        /* _mesa_function_pool[10046]: CompressedTexSubImage2D (will be remapped) */
        "iiiiiiiip\0"
        "glCompressedTexSubImage2D\0"
        "glCompressedTexSubImage2DARB\0"
        "\0"
        /* _mesa_function_pool[10112]: CompressedTexSubImage1D (will be remapped) */
        "iiiiiip\0"
        "glCompressedTexSubImage1D\0"
        "glCompressedTexSubImage1DARB\0"
        "\0"
        /* _mesa_function_pool[10176]: GetCompressedTexImage (will be remapped) */
        "iip\0"
        "glGetCompressedTexImage\0"
        "glGetCompressedTexImageARB\0"
        "\0"
        /* _mesa_function_pool[10232]: BlendFuncSeparate (will be remapped) */
        "iiii\0"
        "glBlendFuncSeparate\0"
        "glBlendFuncSeparateEXT\0"
        "glBlendFuncSeparateINGR\0"
        "\0"
        /* _mesa_function_pool[10305]: FogCoordfEXT (will be remapped) */
        "f\0"
        "glFogCoordf\0"
        "glFogCoordfEXT\0"
        "\0"
        /* _mesa_function_pool[10335]: FogCoordfvEXT (will be remapped) */
        "p\0"
        "glFogCoordfv\0"
        "glFogCoordfvEXT\0"
        "\0"
        /* _mesa_function_pool[10367]: FogCoordd (will be remapped) */
        "d\0"
        "glFogCoordd\0"
        "glFogCoorddEXT\0"
        "\0"
        /* _mesa_function_pool[10397]: FogCoorddv (will be remapped) */
        "p\0"
        "glFogCoorddv\0"
        "glFogCoorddvEXT\0"
        "\0"
        /* _mesa_function_pool[10429]: FogCoordPointer (will be remapped) */
        "iip\0"
        "glFogCoordPointer\0"
        "glFogCoordPointerEXT\0"
        "\0"
        /* _mesa_function_pool[10473]: MultiDrawArrays (will be remapped) */
        "ippi\0"
        "glMultiDrawArrays\0"
        "glMultiDrawArraysEXT\0"
        "\0"
        /* _mesa_function_pool[10518]: MultiDrawElementsEXT (will be remapped) */
        "ipipi\0"
        "glMultiDrawElements\0"
        "glMultiDrawElementsEXT\0"
        "\0"
        /* _mesa_function_pool[10568]: PointParameterf (will be remapped) */
        "if\0"
        "glPointParameterf\0"
        "glPointParameterfARB\0"
        "glPointParameterfEXT\0"
        "glPointParameterfSGIS\0"
        "\0"
        /* _mesa_function_pool[10654]: PointParameterfv (will be remapped) */
        "ip\0"
        "glPointParameterfv\0"
        "glPointParameterfvARB\0"
        "glPointParameterfvEXT\0"
        "glPointParameterfvSGIS\0"
        "\0"
        /* _mesa_function_pool[10744]: PointParameteri (will be remapped) */
        "ii\0"
        "glPointParameteri\0"
        "glPointParameteriNV\0"
        "\0"
        /* _mesa_function_pool[10786]: PointParameteriv (will be remapped) */
        "ip\0"
        "glPointParameteriv\0"
        "glPointParameterivNV\0"
        "\0"
        /* _mesa_function_pool[10830]: SecondaryColor3b (will be remapped) */
        "iii\0"
        "glSecondaryColor3b\0"
        "glSecondaryColor3bEXT\0"
        "\0"
        /* _mesa_function_pool[10876]: SecondaryColor3bv (will be remapped) */
        "p\0"
        "glSecondaryColor3bv\0"
        "glSecondaryColor3bvEXT\0"
        "\0"
        /* _mesa_function_pool[10922]: SecondaryColor3d (will be remapped) */
        "ddd\0"
        "glSecondaryColor3d\0"
        "glSecondaryColor3dEXT\0"
        "\0"
        /* _mesa_function_pool[10968]: SecondaryColor3dv (will be remapped) */
        "p\0"
        "glSecondaryColor3dv\0"
        "glSecondaryColor3dvEXT\0"
        "\0"
        /* _mesa_function_pool[11014]: SecondaryColor3fEXT (will be remapped) */
        "fff\0"
        "glSecondaryColor3f\0"
        "glSecondaryColor3fEXT\0"
        "\0"
        /* _mesa_function_pool[11060]: SecondaryColor3fvEXT (will be remapped) */
        "p\0"
        "glSecondaryColor3fv\0"
        "glSecondaryColor3fvEXT\0"
        "\0"
        /* _mesa_function_pool[11106]: SecondaryColor3i (will be remapped) */
        "iii\0"
        "glSecondaryColor3i\0"
        "glSecondaryColor3iEXT\0"
        "\0"
        /* _mesa_function_pool[11152]: SecondaryColor3iv (will be remapped) */
        "p\0"
        "glSecondaryColor3iv\0"
        "glSecondaryColor3ivEXT\0"
        "\0"
        /* _mesa_function_pool[11198]: SecondaryColor3s (will be remapped) */
        "iii\0"
        "glSecondaryColor3s\0"
        "glSecondaryColor3sEXT\0"
        "\0"
        /* _mesa_function_pool[11244]: SecondaryColor3sv (will be remapped) */
        "p\0"
        "glSecondaryColor3sv\0"
        "glSecondaryColor3svEXT\0"
        "\0"
        /* _mesa_function_pool[11290]: SecondaryColor3ub (will be remapped) */
        "iii\0"
        "glSecondaryColor3ub\0"
        "glSecondaryColor3ubEXT\0"
        "\0"
        /* _mesa_function_pool[11338]: SecondaryColor3ubv (will be remapped) */
        "p\0"
        "glSecondaryColor3ubv\0"
        "glSecondaryColor3ubvEXT\0"
        "\0"
        /* _mesa_function_pool[11386]: SecondaryColor3ui (will be remapped) */
        "iii\0"
        "glSecondaryColor3ui\0"
        "glSecondaryColor3uiEXT\0"
        "\0"
        /* _mesa_function_pool[11434]: SecondaryColor3uiv (will be remapped) */
        "p\0"
        "glSecondaryColor3uiv\0"
        "glSecondaryColor3uivEXT\0"
        "\0"
        /* _mesa_function_pool[11482]: SecondaryColor3us (will be remapped) */
        "iii\0"
        "glSecondaryColor3us\0"
        "glSecondaryColor3usEXT\0"
        "\0"
        /* _mesa_function_pool[11530]: SecondaryColor3usv (will be remapped) */
        "p\0"
        "glSecondaryColor3usv\0"
        "glSecondaryColor3usvEXT\0"
        "\0"
        /* _mesa_function_pool[11578]: SecondaryColorPointer (will be remapped) */
        "iiip\0"
        "glSecondaryColorPointer\0"
        "glSecondaryColorPointerEXT\0"
        "\0"
        /* _mesa_function_pool[11635]: WindowPos2d (will be remapped) */
        "dd\0"
        "glWindowPos2d\0"
        "glWindowPos2dARB\0"
        "glWindowPos2dMESA\0"
        "\0"
        /* _mesa_function_pool[11688]: WindowPos2dv (will be remapped) */
        "p\0"
        "glWindowPos2dv\0"
        "glWindowPos2dvARB\0"
        "glWindowPos2dvMESA\0"
        "\0"
        /* _mesa_function_pool[11743]: WindowPos2f (will be remapped) */
        "ff\0"
        "glWindowPos2f\0"
        "glWindowPos2fARB\0"
        "glWindowPos2fMESA\0"
        "\0"
        /* _mesa_function_pool[11796]: WindowPos2fv (will be remapped) */
        "p\0"
        "glWindowPos2fv\0"
        "glWindowPos2fvARB\0"
        "glWindowPos2fvMESA\0"
        "\0"
        /* _mesa_function_pool[11851]: WindowPos2i (will be remapped) */
        "ii\0"
        "glWindowPos2i\0"
        "glWindowPos2iARB\0"
        "glWindowPos2iMESA\0"
        "\0"
        /* _mesa_function_pool[11904]: WindowPos2iv (will be remapped) */
        "p\0"
        "glWindowPos2iv\0"
        "glWindowPos2ivARB\0"
        "glWindowPos2ivMESA\0"
        "\0"
        /* _mesa_function_pool[11959]: WindowPos2s (will be remapped) */
        "ii\0"
        "glWindowPos2s\0"
        "glWindowPos2sARB\0"
        "glWindowPos2sMESA\0"
        "\0"
        /* _mesa_function_pool[12012]: WindowPos2sv (will be remapped) */
        "p\0"
        "glWindowPos2sv\0"
        "glWindowPos2svARB\0"
        "glWindowPos2svMESA\0"
        "\0"
        /* _mesa_function_pool[12067]: WindowPos3d (will be remapped) */
        "ddd\0"
        "glWindowPos3d\0"
        "glWindowPos3dARB\0"
        "glWindowPos3dMESA\0"
        "\0"
        /* _mesa_function_pool[12121]: WindowPos3dv (will be remapped) */
        "p\0"
        "glWindowPos3dv\0"
        "glWindowPos3dvARB\0"
        "glWindowPos3dvMESA\0"
        "\0"
        /* _mesa_function_pool[12176]: WindowPos3f (will be remapped) */
        "fff\0"
        "glWindowPos3f\0"
        "glWindowPos3fARB\0"
        "glWindowPos3fMESA\0"
        "\0"
        /* _mesa_function_pool[12230]: WindowPos3fv (will be remapped) */
        "p\0"
        "glWindowPos3fv\0"
        "glWindowPos3fvARB\0"
        "glWindowPos3fvMESA\0"
        "\0"
        /* _mesa_function_pool[12285]: WindowPos3i (will be remapped) */
        "iii\0"
        "glWindowPos3i\0"
        "glWindowPos3iARB\0"
        "glWindowPos3iMESA\0"
        "\0"
        /* _mesa_function_pool[12339]: WindowPos3iv (will be remapped) */
        "p\0"
        "glWindowPos3iv\0"
        "glWindowPos3ivARB\0"
        "glWindowPos3ivMESA\0"
        "\0"
        /* _mesa_function_pool[12394]: WindowPos3s (will be remapped) */
        "iii\0"
        "glWindowPos3s\0"
        "glWindowPos3sARB\0"
        "glWindowPos3sMESA\0"
        "\0"
        /* _mesa_function_pool[12448]: WindowPos3sv (will be remapped) */
        "p\0"
        "glWindowPos3sv\0"
        "glWindowPos3svARB\0"
        "glWindowPos3svMESA\0"
        "\0"
        /* _mesa_function_pool[12503]: BindBuffer (will be remapped) */
        "ii\0"
        "glBindBuffer\0"
        "glBindBufferARB\0"
        "\0"
        /* _mesa_function_pool[12536]: BufferData (will be remapped) */
        "iipi\0"
        "glBufferData\0"
        "glBufferDataARB\0"
        "\0"
        /* _mesa_function_pool[12571]: BufferSubData (will be remapped) */
        "iiip\0"
        "glBufferSubData\0"
        "glBufferSubDataARB\0"
        "\0"
        /* _mesa_function_pool[12612]: DeleteBuffers (will be remapped) */
        "ip\0"
        "glDeleteBuffers\0"
        "glDeleteBuffersARB\0"
        "\0"
        /* _mesa_function_pool[12651]: GenBuffers (will be remapped) */
        "ip\0"
        "glGenBuffers\0"
        "glGenBuffersARB\0"
        "\0"
        /* _mesa_function_pool[12684]: GetBufferParameteriv (will be remapped) */
        "iip\0"
        "glGetBufferParameteriv\0"
        "glGetBufferParameterivARB\0"
        "\0"
        /* _mesa_function_pool[12738]: GetBufferPointerv (will be remapped) */
        "iip\0"
        "glGetBufferPointerv\0"
        "glGetBufferPointervARB\0"
        "\0"
        /* _mesa_function_pool[12786]: GetBufferSubData (will be remapped) */
        "iiip\0"
        "glGetBufferSubData\0"
        "glGetBufferSubDataARB\0"
        "\0"
        /* _mesa_function_pool[12833]: IsBuffer (will be remapped) */
        "i\0"
        "glIsBuffer\0"
        "glIsBufferARB\0"
        "\0"
        /* _mesa_function_pool[12861]: MapBuffer (will be remapped) */
        "ii\0"
        "glMapBuffer\0"
        "glMapBufferARB\0"
        "\0"
        /* _mesa_function_pool[12892]: UnmapBuffer (will be remapped) */
        "i\0"
        "glUnmapBuffer\0"
        "glUnmapBufferARB\0"
        "\0"
        /* _mesa_function_pool[12926]: GenQueries (will be remapped) */
        "ip\0"
        "glGenQueries\0"
        "glGenQueriesARB\0"
        "\0"
        /* _mesa_function_pool[12959]: DeleteQueries (will be remapped) */
        "ip\0"
        "glDeleteQueries\0"
        "glDeleteQueriesARB\0"
        "\0"
        /* _mesa_function_pool[12998]: IsQuery (will be remapped) */
        "i\0"
        "glIsQuery\0"
        "glIsQueryARB\0"
        "\0"
        /* _mesa_function_pool[13024]: BeginQuery (will be remapped) */
        "ii\0"
        "glBeginQuery\0"
        "glBeginQueryARB\0"
        "\0"
        /* _mesa_function_pool[13057]: EndQuery (will be remapped) */
        "i\0"
        "glEndQuery\0"
        "glEndQueryARB\0"
        "\0"
        /* _mesa_function_pool[13085]: GetQueryiv (will be remapped) */
        "iip\0"
        "glGetQueryiv\0"
        "glGetQueryivARB\0"
        "\0"
        /* _mesa_function_pool[13119]: GetQueryObjectiv (will be remapped) */
        "iip\0"
        "glGetQueryObjectiv\0"
        "glGetQueryObjectivARB\0"
        "\0"
        /* _mesa_function_pool[13165]: GetQueryObjectuiv (will be remapped) */
        "iip\0"
        "glGetQueryObjectuiv\0"
        "glGetQueryObjectuivARB\0"
        "\0"
        /* _mesa_function_pool[13213]: BlendEquationSeparate (will be remapped) */
        "ii\0"
        "glBlendEquationSeparate\0"
        "glBlendEquationSeparateEXT\0"
        "glBlendEquationSeparateATI\0"
        "\0"
        /* _mesa_function_pool[13295]: DrawBuffers (will be remapped) */
        "ip\0"
        "glDrawBuffers\0"
        "glDrawBuffersARB\0"
        "glDrawBuffersATI\0"
        "\0"
        /* _mesa_function_pool[13347]: StencilFuncSeparate (will be remapped) */
        "iiii\0"
        "glStencilFuncSeparate\0"
        "\0"
        /* _mesa_function_pool[13375]: StencilOpSeparate (will be remapped) */
        "iiii\0"
        "glStencilOpSeparate\0"
        "glStencilOpSeparateATI\0"
        "\0"
        /* _mesa_function_pool[13424]: StencilMaskSeparate (will be remapped) */
        "ii\0"
        "glStencilMaskSeparate\0"
        "\0"
        /* _mesa_function_pool[13450]: AttachShader (will be remapped) */
        "ii\0"
        "glAttachShader\0"
        "\0"
        /* _mesa_function_pool[13469]: BindAttribLocation (will be remapped) */
        "iip\0"
        "glBindAttribLocation\0"
        "glBindAttribLocationARB\0"
        "\0"
        /* _mesa_function_pool[13519]: CompileShader (will be remapped) */
        "i\0"
        "glCompileShader\0"
        "glCompileShaderARB\0"
        "\0"
        /* _mesa_function_pool[13557]: CreateProgram (will be remapped) */
        "\0"
        "glCreateProgram\0"
        "\0"
        /* _mesa_function_pool[13575]: CreateShader (will be remapped) */
        "i\0"
        "glCreateShader\0"
        "\0"
        /* _mesa_function_pool[13593]: DeleteProgram (will be remapped) */
        "i\0"
        "glDeleteProgram\0"
        "\0"
        /* _mesa_function_pool[13612]: DeleteShader (will be remapped) */
        "i\0"
        "glDeleteShader\0"
        "\0"
        /* _mesa_function_pool[13630]: DetachShader (will be remapped) */
        "ii\0"
        "glDetachShader\0"
        "\0"
        /* _mesa_function_pool[13649]: DisableVertexAttribArray (will be remapped) */
        "i\0"
        "glDisableVertexAttribArray\0"
        "glDisableVertexAttribArrayARB\0"
        "\0"
        /* _mesa_function_pool[13709]: EnableVertexAttribArray (will be remapped) */
        "i\0"
        "glEnableVertexAttribArray\0"
        "glEnableVertexAttribArrayARB\0"
        "\0"
        /* _mesa_function_pool[13767]: GetActiveAttrib (will be remapped) */
        "iiipppp\0"
        "glGetActiveAttrib\0"
        "glGetActiveAttribARB\0"
        "\0"
        /* _mesa_function_pool[13815]: GetActiveUniform (will be remapped) */
        "iiipppp\0"
        "glGetActiveUniform\0"
        "glGetActiveUniformARB\0"
        "\0"
        /* _mesa_function_pool[13865]: GetAttachedShaders (will be remapped) */
        "iipp\0"
        "glGetAttachedShaders\0"
        "\0"
        /* _mesa_function_pool[13892]: GetAttribLocation (will be remapped) */
        "ip\0"
        "glGetAttribLocation\0"
        "glGetAttribLocationARB\0"
        "\0"
        /* _mesa_function_pool[13939]: GetProgramiv (will be remapped) */
        "iip\0"
        "glGetProgramiv\0"
        "\0"
        /* _mesa_function_pool[13959]: GetProgramInfoLog (will be remapped) */
        "iipp\0"
        "glGetProgramInfoLog\0"
        "\0"
        /* _mesa_function_pool[13985]: GetShaderiv (will be remapped) */
        "iip\0"
        "glGetShaderiv\0"
        "\0"
        /* _mesa_function_pool[14004]: GetShaderInfoLog (will be remapped) */
        "iipp\0"
        "glGetShaderInfoLog\0"
        "\0"
        /* _mesa_function_pool[14029]: GetShaderSource (will be remapped) */
        "iipp\0"
        "glGetShaderSource\0"
        "glGetShaderSourceARB\0"
        "\0"
        /* _mesa_function_pool[14074]: GetUniformLocation (will be remapped) */
        "ip\0"
        "glGetUniformLocation\0"
        "glGetUniformLocationARB\0"
        "\0"
        /* _mesa_function_pool[14123]: GetUniformfv (will be remapped) */
        "iip\0"
        "glGetUniformfv\0"
        "glGetUniformfvARB\0"
        "\0"
        /* _mesa_function_pool[14161]: GetUniformiv (will be remapped) */
        "iip\0"
        "glGetUniformiv\0"
        "glGetUniformivARB\0"
        "\0"
        /* _mesa_function_pool[14199]: GetVertexAttribdv (will be remapped) */
        "iip\0"
        "glGetVertexAttribdv\0"
        "glGetVertexAttribdvARB\0"
        "\0"
        /* _mesa_function_pool[14247]: GetVertexAttribfv (will be remapped) */
        "iip\0"
        "glGetVertexAttribfv\0"
        "glGetVertexAttribfvARB\0"
        "\0"
        /* _mesa_function_pool[14295]: GetVertexAttribiv (will be remapped) */
        "iip\0"
        "glGetVertexAttribiv\0"
        "glGetVertexAttribivARB\0"
        "\0"
        /* _mesa_function_pool[14343]: GetVertexAttribPointerv (will be remapped) */
        "iip\0"
        "glGetVertexAttribPointerv\0"
        "glGetVertexAttribPointervARB\0"
        "glGetVertexAttribPointervNV\0"
        "\0"
        /* _mesa_function_pool[14431]: IsProgram (will be remapped) */
        "i\0"
        "glIsProgram\0"
        "\0"
        /* _mesa_function_pool[14446]: IsShader (will be remapped) */
        "i\0"
        "glIsShader\0"
        "\0"
        /* _mesa_function_pool[14460]: LinkProgram (will be remapped) */
        "i\0"
        "glLinkProgram\0"
        "glLinkProgramARB\0"
        "\0"
        /* _mesa_function_pool[14494]: ShaderSource (will be remapped) */
        "iipp\0"
        "glShaderSource\0"
        "glShaderSourceARB\0"
        "\0"
        /* _mesa_function_pool[14533]: UseProgram (will be remapped) */
        "i\0"
        "glUseProgram\0"
        "glUseProgramObjectARB\0"
        "\0"
        /* _mesa_function_pool[14571]: Uniform1f (will be remapped) */
        "if\0"
        "glUniform1f\0"
        "glUniform1fARB\0"
        "\0"
        /* _mesa_function_pool[14602]: Uniform2f (will be remapped) */
        "iff\0"
        "glUniform2f\0"
        "glUniform2fARB\0"
        "\0"
        /* _mesa_function_pool[14634]: Uniform3f (will be remapped) */
        "ifff\0"
        "glUniform3f\0"
        "glUniform3fARB\0"
        "\0"
        /* _mesa_function_pool[14667]: Uniform4f (will be remapped) */
        "iffff\0"
        "glUniform4f\0"
        "glUniform4fARB\0"
        "\0"
        /* _mesa_function_pool[14701]: Uniform1i (will be remapped) */
        "ii\0"
        "glUniform1i\0"
        "glUniform1iARB\0"
        "\0"
        /* _mesa_function_pool[14732]: Uniform2i (will be remapped) */
        "iii\0"
        "glUniform2i\0"
        "glUniform2iARB\0"
        "\0"
        /* _mesa_function_pool[14764]: Uniform3i (will be remapped) */
        "iiii\0"
        "glUniform3i\0"
        "glUniform3iARB\0"
        "\0"
        /* _mesa_function_pool[14797]: Uniform4i (will be remapped) */
        "iiiii\0"
        "glUniform4i\0"
        "glUniform4iARB\0"
        "\0"
        /* _mesa_function_pool[14831]: Uniform1fv (will be remapped) */
        "iip\0"
        "glUniform1fv\0"
        "glUniform1fvARB\0"
        "\0"
        /* _mesa_function_pool[14865]: Uniform2fv (will be remapped) */
        "iip\0"
        "glUniform2fv\0"
        "glUniform2fvARB\0"
        "\0"
        /* _mesa_function_pool[14899]: Uniform3fv (will be remapped) */
        "iip\0"
        "glUniform3fv\0"
        "glUniform3fvARB\0"
        "\0"
        /* _mesa_function_pool[14933]: Uniform4fv (will be remapped) */
        "iip\0"
        "glUniform4fv\0"
        "glUniform4fvARB\0"
        "\0"
        /* _mesa_function_pool[14967]: Uniform1iv (will be remapped) */
        "iip\0"
        "glUniform1iv\0"
        "glUniform1ivARB\0"
        "\0"
        /* _mesa_function_pool[15001]: Uniform2iv (will be remapped) */
        "iip\0"
        "glUniform2iv\0"
        "glUniform2ivARB\0"
        "\0"
        /* _mesa_function_pool[15035]: Uniform3iv (will be remapped) */
        "iip\0"
        "glUniform3iv\0"
        "glUniform3ivARB\0"
        "\0"
        /* _mesa_function_pool[15069]: Uniform4iv (will be remapped) */
        "iip\0"
        "glUniform4iv\0"
        "glUniform4ivARB\0"
        "\0"
        /* _mesa_function_pool[15103]: UniformMatrix2fv (will be remapped) */
        "iiip\0"
        "glUniformMatrix2fv\0"
        "glUniformMatrix2fvARB\0"
        "\0"
        /* _mesa_function_pool[15150]: UniformMatrix3fv (will be remapped) */
        "iiip\0"
        "glUniformMatrix3fv\0"
        "glUniformMatrix3fvARB\0"
        "\0"
        /* _mesa_function_pool[15197]: UniformMatrix4fv (will be remapped) */
        "iiip\0"
        "glUniformMatrix4fv\0"
        "glUniformMatrix4fvARB\0"
        "\0"
        /* _mesa_function_pool[15244]: ValidateProgram (will be remapped) */
        "i\0"
        "glValidateProgram\0"
        "glValidateProgramARB\0"
        "\0"
        /* _mesa_function_pool[15286]: VertexAttrib1d (will be remapped) */
        "id\0"
        "glVertexAttrib1d\0"
        "glVertexAttrib1dARB\0"
        "\0"
        /* _mesa_function_pool[15327]: VertexAttrib1dv (will be remapped) */
        "ip\0"
        "glVertexAttrib1dv\0"
        "glVertexAttrib1dvARB\0"
        "\0"
        /* _mesa_function_pool[15370]: VertexAttrib1fARB (will be remapped) */
        "if\0"
        "glVertexAttrib1f\0"
        "glVertexAttrib1fARB\0"
        "\0"
        /* _mesa_function_pool[15411]: VertexAttrib1fvARB (will be remapped) */
        "ip\0"
        "glVertexAttrib1fv\0"
        "glVertexAttrib1fvARB\0"
        "\0"
        /* _mesa_function_pool[15454]: VertexAttrib1s (will be remapped) */
        "ii\0"
        "glVertexAttrib1s\0"
        "glVertexAttrib1sARB\0"
        "\0"
        /* _mesa_function_pool[15495]: VertexAttrib1sv (will be remapped) */
        "ip\0"
        "glVertexAttrib1sv\0"
        "glVertexAttrib1svARB\0"
        "\0"
        /* _mesa_function_pool[15538]: VertexAttrib2d (will be remapped) */
        "idd\0"
        "glVertexAttrib2d\0"
        "glVertexAttrib2dARB\0"
        "\0"
        /* _mesa_function_pool[15580]: VertexAttrib2dv (will be remapped) */
        "ip\0"
        "glVertexAttrib2dv\0"
        "glVertexAttrib2dvARB\0"
        "\0"
        /* _mesa_function_pool[15623]: VertexAttrib2fARB (will be remapped) */
        "iff\0"
        "glVertexAttrib2f\0"
        "glVertexAttrib2fARB\0"
        "\0"
        /* _mesa_function_pool[15665]: VertexAttrib2fvARB (will be remapped) */
        "ip\0"
        "glVertexAttrib2fv\0"
        "glVertexAttrib2fvARB\0"
        "\0"
        /* _mesa_function_pool[15708]: VertexAttrib2s (will be remapped) */
        "iii\0"
        "glVertexAttrib2s\0"
        "glVertexAttrib2sARB\0"
        "\0"
        /* _mesa_function_pool[15750]: VertexAttrib2sv (will be remapped) */
        "ip\0"
        "glVertexAttrib2sv\0"
        "glVertexAttrib2svARB\0"
        "\0"
        /* _mesa_function_pool[15793]: VertexAttrib3d (will be remapped) */
        "iddd\0"
        "glVertexAttrib3d\0"
        "glVertexAttrib3dARB\0"
        "\0"
        /* _mesa_function_pool[15836]: VertexAttrib3dv (will be remapped) */
        "ip\0"
        "glVertexAttrib3dv\0"
        "glVertexAttrib3dvARB\0"
        "\0"
        /* _mesa_function_pool[15879]: VertexAttrib3fARB (will be remapped) */
        "ifff\0"
        "glVertexAttrib3f\0"
        "glVertexAttrib3fARB\0"
        "\0"
        /* _mesa_function_pool[15922]: VertexAttrib3fvARB (will be remapped) */
        "ip\0"
        "glVertexAttrib3fv\0"
        "glVertexAttrib3fvARB\0"
        "\0"
        /* _mesa_function_pool[15965]: VertexAttrib3s (will be remapped) */
        "iiii\0"
        "glVertexAttrib3s\0"
        "glVertexAttrib3sARB\0"
        "\0"
        /* _mesa_function_pool[16008]: VertexAttrib3sv (will be remapped) */
        "ip\0"
        "glVertexAttrib3sv\0"
        "glVertexAttrib3svARB\0"
        "\0"
        /* _mesa_function_pool[16051]: VertexAttrib4Nbv (will be remapped) */
        "ip\0"
        "glVertexAttrib4Nbv\0"
        "glVertexAttrib4NbvARB\0"
        "\0"
        /* _mesa_function_pool[16096]: VertexAttrib4Niv (will be remapped) */
        "ip\0"
        "glVertexAttrib4Niv\0"
        "glVertexAttrib4NivARB\0"
        "\0"
        /* _mesa_function_pool[16141]: VertexAttrib4Nsv (will be remapped) */
        "ip\0"
        "glVertexAttrib4Nsv\0"
        "glVertexAttrib4NsvARB\0"
        "\0"
        /* _mesa_function_pool[16186]: VertexAttrib4Nub (will be remapped) */
        "iiiii\0"
        "glVertexAttrib4Nub\0"
        "glVertexAttrib4NubARB\0"
        "\0"
        /* _mesa_function_pool[16234]: VertexAttrib4Nubv (will be remapped) */
        "ip\0"
        "glVertexAttrib4Nubv\0"
        "glVertexAttrib4NubvARB\0"
        "\0"
        /* _mesa_function_pool[16281]: VertexAttrib4Nuiv (will be remapped) */
        "ip\0"
        "glVertexAttrib4Nuiv\0"
        "glVertexAttrib4NuivARB\0"
        "\0"
        /* _mesa_function_pool[16328]: VertexAttrib4Nusv (will be remapped) */
        "ip\0"
        "glVertexAttrib4Nusv\0"
        "glVertexAttrib4NusvARB\0"
        "\0"
        /* _mesa_function_pool[16375]: VertexAttrib4bv (will be remapped) */
        "ip\0"
        "glVertexAttrib4bv\0"
        "glVertexAttrib4bvARB\0"
        "\0"
        /* _mesa_function_pool[16418]: VertexAttrib4d (will be remapped) */
        "idddd\0"
        "glVertexAttrib4d\0"
        "glVertexAttrib4dARB\0"
        "\0"
        /* _mesa_function_pool[16462]: VertexAttrib4dv (will be remapped) */
        "ip\0"
        "glVertexAttrib4dv\0"
        "glVertexAttrib4dvARB\0"
        "\0"
        /* _mesa_function_pool[16505]: VertexAttrib4fARB (will be remapped) */
        "iffff\0"
        "glVertexAttrib4f\0"
        "glVertexAttrib4fARB\0"
        "\0"
        /* _mesa_function_pool[16549]: VertexAttrib4fvARB (will be remapped) */
        "ip\0"
        "glVertexAttrib4fv\0"
        "glVertexAttrib4fvARB\0"
        "\0"
        /* _mesa_function_pool[16592]: VertexAttrib4iv (will be remapped) */
        "ip\0"
        "glVertexAttrib4iv\0"
        "glVertexAttrib4ivARB\0"
        "\0"
        /* _mesa_function_pool[16635]: VertexAttrib4s (will be remapped) */
        "iiiii\0"
        "glVertexAttrib4s\0"
        "glVertexAttrib4sARB\0"
        "\0"
        /* _mesa_function_pool[16679]: VertexAttrib4sv (will be remapped) */
        "ip\0"
        "glVertexAttrib4sv\0"
        "glVertexAttrib4svARB\0"
        "\0"
        /* _mesa_function_pool[16722]: VertexAttrib4ubv (will be remapped) */
        "ip\0"
        "glVertexAttrib4ubv\0"
        "glVertexAttrib4ubvARB\0"
        "\0"
        /* _mesa_function_pool[16767]: VertexAttrib4uiv (will be remapped) */
        "ip\0"
        "glVertexAttrib4uiv\0"
        "glVertexAttrib4uivARB\0"
        "\0"
        /* _mesa_function_pool[16812]: VertexAttrib4usv (will be remapped) */
        "ip\0"
        "glVertexAttrib4usv\0"
        "glVertexAttrib4usvARB\0"
        "\0"
        /* _mesa_function_pool[16857]: VertexAttribPointer (will be remapped) */
        "iiiiip\0"
        "glVertexAttribPointer\0"
        "glVertexAttribPointerARB\0"
        "\0"
        /* _mesa_function_pool[16912]: UniformMatrix2x3fv (will be remapped) */
        "iiip\0"
        "glUniformMatrix2x3fv\0"
        "\0"
        /* _mesa_function_pool[16939]: UniformMatrix3x2fv (will be remapped) */
        "iiip\0"
        "glUniformMatrix3x2fv\0"
        "\0"
        /* _mesa_function_pool[16966]: UniformMatrix2x4fv (will be remapped) */
        "iiip\0"
        "glUniformMatrix2x4fv\0"
        "\0"
        /* _mesa_function_pool[16993]: UniformMatrix4x2fv (will be remapped) */
        "iiip\0"
        "glUniformMatrix4x2fv\0"
        "\0"
        /* _mesa_function_pool[17020]: UniformMatrix3x4fv (will be remapped) */
        "iiip\0"
        "glUniformMatrix3x4fv\0"
        "\0"
        /* _mesa_function_pool[17047]: UniformMatrix4x3fv (will be remapped) */
        "iiip\0"
        "glUniformMatrix4x3fv\0"
        "\0"
        /* _mesa_function_pool[17074]: WeightbvARB (dynamic) */
        "ip\0"
        "glWeightbvARB\0"
        "\0"
        /* _mesa_function_pool[17092]: WeightsvARB (dynamic) */
        "ip\0"
        "glWeightsvARB\0"
        "\0"
        /* _mesa_function_pool[17110]: WeightivARB (dynamic) */
        "ip\0"
        "glWeightivARB\0"
        "\0"
        /* _mesa_function_pool[17128]: WeightfvARB (dynamic) */
        "ip\0"
        "glWeightfvARB\0"
        "\0"
        /* _mesa_function_pool[17146]: WeightdvARB (dynamic) */
        "ip\0"
        "glWeightdvARB\0"
        "\0"
        /* _mesa_function_pool[17164]: WeightubvARB (dynamic) */
        "ip\0"
        "glWeightubvARB\0"
        "\0"
        /* _mesa_function_pool[17183]: WeightusvARB (dynamic) */
        "ip\0"
        "glWeightusvARB\0"
        "\0"
        /* _mesa_function_pool[17202]: WeightuivARB (dynamic) */
        "ip\0"
        "glWeightuivARB\0"
        "\0"
        /* _mesa_function_pool[17221]: WeightPointerARB (dynamic) */
        "iiip\0"
        "glWeightPointerARB\0"
        "\0"
        /* _mesa_function_pool[17246]: VertexBlendARB (dynamic) */
        "i\0"
        "glVertexBlendARB\0"
        "\0"
        /* _mesa_function_pool[17266]: CurrentPaletteMatrixARB (dynamic) */
        "i\0"
        "glCurrentPaletteMatrixARB\0"
        "\0"
        /* _mesa_function_pool[17295]: MatrixIndexubvARB (dynamic) */
        "ip\0"
        "glMatrixIndexubvARB\0"
        "\0"
        /* _mesa_function_pool[17319]: MatrixIndexusvARB (dynamic) */
        "ip\0"
        "glMatrixIndexusvARB\0"
        "\0"
        /* _mesa_function_pool[17343]: MatrixIndexuivARB (dynamic) */
        "ip\0"
        "glMatrixIndexuivARB\0"
        "\0"
        /* _mesa_function_pool[17367]: MatrixIndexPointerARB (dynamic) */
        "iiip\0"
        "glMatrixIndexPointerARB\0"
        "\0"
        /* _mesa_function_pool[17397]: ProgramStringARB (will be remapped) */
        "iiip\0"
        "glProgramStringARB\0"
        "\0"
        /* _mesa_function_pool[17422]: BindProgramARB (will be remapped) */
        "ii\0"
        "glBindProgramARB\0"
        "glBindProgramNV\0"
        "\0"
        /* _mesa_function_pool[17459]: DeleteProgramsARB (will be remapped) */
        "ip\0"
        "glDeleteProgramsARB\0"
        "glDeleteProgramsNV\0"
        "\0"
        /* _mesa_function_pool[17502]: GenProgramsARB (will be remapped) */
        "ip\0"
        "glGenProgramsARB\0"
        "glGenProgramsNV\0"
        "\0"
        /* _mesa_function_pool[17539]: IsProgramARB (will be remapped) */
        "i\0"
        "glIsProgramARB\0"
        "glIsProgramNV\0"
        "\0"
        /* _mesa_function_pool[17571]: ProgramEnvParameter4dARB (will be remapped) */
        "iidddd\0"
        "glProgramEnvParameter4dARB\0"
        "glProgramParameter4dNV\0"
        "\0"
        /* _mesa_function_pool[17629]: ProgramEnvParameter4dvARB (will be remapped) */
        "iip\0"
        "glProgramEnvParameter4dvARB\0"
        "glProgramParameter4dvNV\0"
        "\0"
        /* _mesa_function_pool[17686]: ProgramEnvParameter4fARB (will be remapped) */
        "iiffff\0"
        "glProgramEnvParameter4fARB\0"
        "glProgramParameter4fNV\0"
        "\0"
        /* _mesa_function_pool[17744]: ProgramEnvParameter4fvARB (will be remapped) */
        "iip\0"
        "glProgramEnvParameter4fvARB\0"
        "glProgramParameter4fvNV\0"
        "\0"
        /* _mesa_function_pool[17801]: ProgramLocalParameter4dARB (will be remapped) */
        "iidddd\0"
        "glProgramLocalParameter4dARB\0"
        "\0"
        /* _mesa_function_pool[17838]: ProgramLocalParameter4dvARB (will be remapped) */
        "iip\0"
        "glProgramLocalParameter4dvARB\0"
        "\0"
        /* _mesa_function_pool[17873]: ProgramLocalParameter4fARB (will be remapped) */
        "iiffff\0"
        "glProgramLocalParameter4fARB\0"
        "\0"
        /* _mesa_function_pool[17910]: ProgramLocalParameter4fvARB (will be remapped) */
        "iip\0"
        "glProgramLocalParameter4fvARB\0"
        "\0"
        /* _mesa_function_pool[17945]: GetProgramEnvParameterdvARB (will be remapped) */
        "iip\0"
        "glGetProgramEnvParameterdvARB\0"
        "\0"
        /* _mesa_function_pool[17980]: GetProgramEnvParameterfvARB (will be remapped) */
        "iip\0"
        "glGetProgramEnvParameterfvARB\0"
        "\0"
        /* _mesa_function_pool[18015]: GetProgramLocalParameterdvARB (will be remapped) */
        "iip\0"
        "glGetProgramLocalParameterdvARB\0"
        "\0"
        /* _mesa_function_pool[18052]: GetProgramLocalParameterfvARB (will be remapped) */
        "iip\0"
        "glGetProgramLocalParameterfvARB\0"
        "\0"
        /* _mesa_function_pool[18089]: GetProgramivARB (will be remapped) */
        "iip\0"
        "glGetProgramivARB\0"
        "\0"
        /* _mesa_function_pool[18112]: GetProgramStringARB (will be remapped) */
        "iip\0"
        "glGetProgramStringARB\0"
        "\0"
        /* _mesa_function_pool[18139]: DeleteObjectARB (will be remapped) */
        "i\0"
        "glDeleteObjectARB\0"
        "\0"
        /* _mesa_function_pool[18160]: GetHandleARB (will be remapped) */
        "i\0"
        "glGetHandleARB\0"
        "\0"
        /* _mesa_function_pool[18178]: DetachObjectARB (will be remapped) */
        "ii\0"
        "glDetachObjectARB\0"
        "\0"
        /* _mesa_function_pool[18200]: CreateShaderObjectARB (will be remapped) */
        "i\0"
        "glCreateShaderObjectARB\0"
        "\0"
        /* _mesa_function_pool[18227]: CreateProgramObjectARB (will be remapped) */
        "\0"
        "glCreateProgramObjectARB\0"
        "\0"
        /* _mesa_function_pool[18254]: AttachObjectARB (will be remapped) */
        "ii\0"
        "glAttachObjectARB\0"
        "\0"
        /* _mesa_function_pool[18276]: GetObjectParameterfvARB (will be remapped) */
        "iip\0"
        "glGetObjectParameterfvARB\0"
        "\0"
        /* _mesa_function_pool[18307]: GetObjectParameterivARB (will be remapped) */
        "iip\0"
        "glGetObjectParameterivARB\0"
        "\0"
        /* _mesa_function_pool[18338]: GetInfoLogARB (will be remapped) */
        "iipp\0"
        "glGetInfoLogARB\0"
        "\0"
        /* _mesa_function_pool[18360]: GetAttachedObjectsARB (will be remapped) */
        "iipp\0"
        "glGetAttachedObjectsARB\0"
        "\0"
        /* _mesa_function_pool[18390]: ClampColor (will be remapped) */
        "ii\0"
        "glClampColorARB\0"
        "glClampColor\0"
        "\0"
        /* _mesa_function_pool[18423]: DrawArraysInstancedARB (will be remapped) */
        "iiii\0"
        "glDrawArraysInstancedARB\0"
        "glDrawArraysInstancedEXT\0"
        "glDrawArraysInstanced\0"
        "\0"
        /* _mesa_function_pool[18501]: DrawElementsInstancedARB (will be remapped) */
        "iiipi\0"
        "glDrawElementsInstancedARB\0"
        "glDrawElementsInstancedEXT\0"
        "glDrawElementsInstanced\0"
        "\0"
        /* _mesa_function_pool[18586]: IsRenderbuffer (will be remapped) */
        "i\0"
        "glIsRenderbuffer\0"
        "glIsRenderbufferEXT\0"
        "\0"
        /* _mesa_function_pool[18626]: BindRenderbuffer (will be remapped) */
        "ii\0"
        "glBindRenderbuffer\0"
        "\0"
        /* _mesa_function_pool[18649]: DeleteRenderbuffers (will be remapped) */
        "ip\0"
        "glDeleteRenderbuffers\0"
        "glDeleteRenderbuffersEXT\0"
        "\0"
        /* _mesa_function_pool[18700]: GenRenderbuffers (will be remapped) */
        "ip\0"
        "glGenRenderbuffers\0"
        "glGenRenderbuffersEXT\0"
        "\0"
        /* _mesa_function_pool[18745]: RenderbufferStorage (will be remapped) */
        "iiii\0"
        "glRenderbufferStorage\0"
        "glRenderbufferStorageEXT\0"
        "\0"
        /* _mesa_function_pool[18798]: RenderbufferStorageMultisample (will be remapped) */
        "iiiii\0"
        "glRenderbufferStorageMultisample\0"
        "glRenderbufferStorageMultisampleEXT\0"
        "\0"
        /* _mesa_function_pool[18874]: GetRenderbufferParameteriv (will be remapped) */
        "iip\0"
        "glGetRenderbufferParameteriv\0"
        "glGetRenderbufferParameterivEXT\0"
        "\0"
        /* _mesa_function_pool[18940]: IsFramebuffer (will be remapped) */
        "i\0"
        "glIsFramebuffer\0"
        "glIsFramebufferEXT\0"
        "\0"
        /* _mesa_function_pool[18978]: BindFramebuffer (will be remapped) */
        "ii\0"
        "glBindFramebuffer\0"
        "\0"
        /* _mesa_function_pool[19000]: DeleteFramebuffers (will be remapped) */
        "ip\0"
        "glDeleteFramebuffers\0"
        "glDeleteFramebuffersEXT\0"
        "\0"
        /* _mesa_function_pool[19049]: GenFramebuffers (will be remapped) */
        "ip\0"
        "glGenFramebuffers\0"
        "glGenFramebuffersEXT\0"
        "\0"
        /* _mesa_function_pool[19092]: CheckFramebufferStatus (will be remapped) */
        "i\0"
        "glCheckFramebufferStatus\0"
        "glCheckFramebufferStatusEXT\0"
        "\0"
        /* _mesa_function_pool[19148]: FramebufferTexture1D (will be remapped) */
        "iiiii\0"
        "glFramebufferTexture1D\0"
        "glFramebufferTexture1DEXT\0"
        "\0"
        /* _mesa_function_pool[19204]: FramebufferTexture2D (will be remapped) */
        "iiiii\0"
        "glFramebufferTexture2D\0"
        "glFramebufferTexture2DEXT\0"
        "\0"
        /* _mesa_function_pool[19260]: FramebufferTexture3D (will be remapped) */
        "iiiiii\0"
        "glFramebufferTexture3D\0"
        "glFramebufferTexture3DEXT\0"
        "\0"
        /* _mesa_function_pool[19317]: FramebufferTextureLayer (will be remapped) */
        "iiiii\0"
        "glFramebufferTextureLayer\0"
        "glFramebufferTextureLayerEXT\0"
        "\0"
        /* _mesa_function_pool[19379]: FramebufferRenderbuffer (will be remapped) */
        "iiii\0"
        "glFramebufferRenderbuffer\0"
        "glFramebufferRenderbufferEXT\0"
        "\0"
        /* _mesa_function_pool[19440]: GetFramebufferAttachmentParameteriv (will be remapped) */
        "iiip\0"
        "glGetFramebufferAttachmentParameteriv\0"
        "glGetFramebufferAttachmentParameterivEXT\0"
        "\0"
        /* _mesa_function_pool[19525]: BlitFramebuffer (will be remapped) */
        "iiiiiiiiii\0"
        "glBlitFramebuffer\0"
        "glBlitFramebufferEXT\0"
        "\0"
        /* _mesa_function_pool[19576]: GenerateMipmap (will be remapped) */
        "i\0"
        "glGenerateMipmap\0"
        "glGenerateMipmapEXT\0"
        "\0"
        /* _mesa_function_pool[19616]: VertexAttribDivisor (will be remapped) */
        "ii\0"
        "glVertexAttribDivisorARB\0"
        "glVertexAttribDivisor\0"
        "\0"
        /* _mesa_function_pool[19667]: VertexArrayVertexAttribDivisorEXT (will be remapped) */
        "iii\0"
        "glVertexArrayVertexAttribDivisorEXT\0"
        "\0"
        /* _mesa_function_pool[19708]: MapBufferRange (will be remapped) */
        "iiii\0"
        "glMapBufferRange\0"
        "\0"
        /* _mesa_function_pool[19731]: FlushMappedBufferRange (will be remapped) */
        "iii\0"
        "glFlushMappedBufferRange\0"
        "\0"
        /* _mesa_function_pool[19761]: TexBuffer (will be remapped) */
        "iii\0"
        "glTexBufferARB\0"
        "glTexBuffer\0"
        "\0"
        /* _mesa_function_pool[19793]: BindVertexArray (will be remapped) */
        "i\0"
        "glBindVertexArray\0"
        "\0"
        /* _mesa_function_pool[19814]: DeleteVertexArrays (will be remapped) */
        "ip\0"
        "glDeleteVertexArrays\0"
        "\0"
        /* _mesa_function_pool[19839]: GenVertexArrays (will be remapped) */
        "ip\0"
        "glGenVertexArrays\0"
        "\0"
        /* _mesa_function_pool[19861]: IsVertexArray (will be remapped) */
        "i\0"
        "glIsVertexArray\0"
        "\0"
        /* _mesa_function_pool[19880]: GetUniformIndices (will be remapped) */
        "iipp\0"
        "glGetUniformIndices\0"
        "\0"
        /* _mesa_function_pool[19906]: GetActiveUniformsiv (will be remapped) */
        "iipip\0"
        "glGetActiveUniformsiv\0"
        "\0"
        /* _mesa_function_pool[19935]: GetActiveUniformName (will be remapped) */
        "iiipp\0"
        "glGetActiveUniformName\0"
        "\0"
        /* _mesa_function_pool[19965]: GetUniformBlockIndex (will be remapped) */
        "ip\0"
        "glGetUniformBlockIndex\0"
        "\0"
        /* _mesa_function_pool[19992]: GetActiveUniformBlockiv (will be remapped) */
        "iiip\0"
        "glGetActiveUniformBlockiv\0"
        "\0"
        /* _mesa_function_pool[20024]: GetActiveUniformBlockName (will be remapped) */
        "iiipp\0"
        "glGetActiveUniformBlockName\0"
        "\0"
        /* _mesa_function_pool[20059]: UniformBlockBinding (will be remapped) */
        "iii\0"
        "glUniformBlockBinding\0"
        "\0"
        /* _mesa_function_pool[20086]: CopyBufferSubData (will be remapped) */
        "iiiii\0"
        "glCopyBufferSubData\0"
        "\0"
        /* _mesa_function_pool[20113]: DrawElementsBaseVertex (will be remapped) */
        "iiipi\0"
        "glDrawElementsBaseVertex\0"
        "\0"
        /* _mesa_function_pool[20145]: DrawRangeElementsBaseVertex (will be remapped) */
        "iiiiipi\0"
        "glDrawRangeElementsBaseVertex\0"
        "\0"
        /* _mesa_function_pool[20184]: MultiDrawElementsBaseVertex (will be remapped) */
        "ipipip\0"
        "glMultiDrawElementsBaseVertex\0"
        "\0"
        /* _mesa_function_pool[20222]: DrawElementsInstancedBaseVertex (will be remapped) */
        "iiipii\0"
        "glDrawElementsInstancedBaseVertex\0"
        "\0"
        /* _mesa_function_pool[20264]: FenceSync (will be remapped) */
        "ii\0"
        "glFenceSync\0"
        "\0"
        /* _mesa_function_pool[20280]: IsSync (will be remapped) */
        "i\0"
        "glIsSync\0"
        "\0"
        /* _mesa_function_pool[20292]: DeleteSync (will be remapped) */
        "i\0"
        "glDeleteSync\0"
        "\0"
        /* _mesa_function_pool[20308]: ClientWaitSync (will be remapped) */
        "iii\0"
        "glClientWaitSync\0"
        "\0"
        /* _mesa_function_pool[20330]: WaitSync (will be remapped) */
        "iii\0"
        "glWaitSync\0"
        "\0"
        /* _mesa_function_pool[20346]: GetInteger64v (will be remapped) */
        "ip\0"
        "glGetInteger64v\0"
        "\0"
        /* _mesa_function_pool[20366]: GetSynciv (will be remapped) */
        "iiipp\0"
        "glGetSynciv\0"
        "\0"
        /* _mesa_function_pool[20385]: TexImage2DMultisample (will be remapped) */
        "iiiiii\0"
        "glTexImage2DMultisample\0"
        "\0"
        /* _mesa_function_pool[20417]: TexImage3DMultisample (will be remapped) */
        "iiiiiii\0"
        "glTexImage3DMultisample\0"
        "\0"
        /* _mesa_function_pool[20450]: GetMultisamplefv (will be remapped) */
        "iip\0"
        "glGetMultisamplefv\0"
        "\0"
        /* _mesa_function_pool[20474]: SampleMaski (will be remapped) */
        "ii\0"
        "glSampleMaski\0"
        "\0"
        /* _mesa_function_pool[20492]: BlendEquationiARB (will be remapped) */
        "ii\0"
        "glBlendEquationiARB\0"
        "glBlendEquationIndexedAMD\0"
        "glBlendEquationi\0"
        "\0"
        /* _mesa_function_pool[20559]: BlendEquationSeparateiARB (will be remapped) */
        "iii\0"
        "glBlendEquationSeparateiARB\0"
        "glBlendEquationSeparateIndexedAMD\0"
        "glBlendEquationSeparatei\0"
        "\0"
        /* _mesa_function_pool[20651]: BlendFunciARB (will be remapped) */
        "iii\0"
        "glBlendFunciARB\0"
        "glBlendFuncIndexedAMD\0"
        "glBlendFunci\0"
        "\0"
        /* _mesa_function_pool[20707]: BlendFuncSeparateiARB (will be remapped) */
        "iiiii\0"
        "glBlendFuncSeparateiARB\0"
        "glBlendFuncSeparateIndexedAMD\0"
        "glBlendFuncSeparatei\0"
        "\0"
        /* _mesa_function_pool[20789]: MinSampleShading (will be remapped) */
        "f\0"
        "glMinSampleShadingARB\0"
        "glMinSampleShading\0"
        "\0"
        /* _mesa_function_pool[20833]: NamedStringARB (will be remapped) */
        "iipip\0"
        "glNamedStringARB\0"
        "\0"
        /* _mesa_function_pool[20857]: DeleteNamedStringARB (will be remapped) */
        "ip\0"
        "glDeleteNamedStringARB\0"
        "\0"
        /* _mesa_function_pool[20884]: CompileShaderIncludeARB (will be remapped) */
        "iipp\0"
        "glCompileShaderIncludeARB\0"
        "\0"
        /* _mesa_function_pool[20916]: IsNamedStringARB (will be remapped) */
        "ip\0"
        "glIsNamedStringARB\0"
        "\0"
        /* _mesa_function_pool[20939]: GetNamedStringARB (will be remapped) */
        "ipipp\0"
        "glGetNamedStringARB\0"
        "\0"
        /* _mesa_function_pool[20966]: GetNamedStringivARB (will be remapped) */
        "ipip\0"
        "glGetNamedStringivARB\0"
        "\0"
        /* _mesa_function_pool[20994]: BindFragDataLocationIndexed (will be remapped) */
        "iiip\0"
        "glBindFragDataLocationIndexed\0"
        "\0"
        /* _mesa_function_pool[21030]: GetFragDataIndex (will be remapped) */
        "ip\0"
        "glGetFragDataIndex\0"
        "\0"
        /* _mesa_function_pool[21053]: GenSamplers (will be remapped) */
        "ip\0"
        "glGenSamplers\0"
        "\0"
        /* _mesa_function_pool[21071]: DeleteSamplers (will be remapped) */
        "ip\0"
        "glDeleteSamplers\0"
        "\0"
        /* _mesa_function_pool[21092]: IsSampler (will be remapped) */
        "i\0"
        "glIsSampler\0"
        "\0"
        /* _mesa_function_pool[21107]: BindSampler (will be remapped) */
        "ii\0"
        "glBindSampler\0"
        "\0"
        /* _mesa_function_pool[21125]: SamplerParameteri (will be remapped) */
        "iii\0"
        "glSamplerParameteri\0"
        "\0"
        /* _mesa_function_pool[21150]: SamplerParameterf (will be remapped) */
        "iif\0"
        "glSamplerParameterf\0"
        "\0"
        /* _mesa_function_pool[21175]: SamplerParameteriv (will be remapped) */
        "iip\0"
        "glSamplerParameteriv\0"
        "\0"
        /* _mesa_function_pool[21201]: SamplerParameterfv (will be remapped) */
        "iip\0"
        "glSamplerParameterfv\0"
        "\0"
        /* _mesa_function_pool[21227]: SamplerParameterIiv (will be remapped) */
        "iip\0"
        "glSamplerParameterIiv\0"
        "\0"
        /* _mesa_function_pool[21254]: SamplerParameterIuiv (will be remapped) */
        "iip\0"
        "glSamplerParameterIuiv\0"
        "\0"
        /* _mesa_function_pool[21282]: GetSamplerParameteriv (will be remapped) */
        "iip\0"
        "glGetSamplerParameteriv\0"
        "\0"
        /* _mesa_function_pool[21311]: GetSamplerParameterfv (will be remapped) */
        "iip\0"
        "glGetSamplerParameterfv\0"
        "\0"
        /* _mesa_function_pool[21340]: GetSamplerParameterIiv (will be remapped) */
        "iip\0"
        "glGetSamplerParameterIiv\0"
        "\0"
        /* _mesa_function_pool[21370]: GetSamplerParameterIuiv (will be remapped) */
        "iip\0"
        "glGetSamplerParameterIuiv\0"
        "\0"
        /* _mesa_function_pool[21401]: GetQueryObjecti64v (will be remapped) */
        "iip\0"
        "glGetQueryObjecti64v\0"
        "glGetQueryObjecti64vEXT\0"
        "\0"
        /* _mesa_function_pool[21451]: GetQueryObjectui64v (will be remapped) */
        "iip\0"
        "glGetQueryObjectui64v\0"
        "glGetQueryObjectui64vEXT\0"
        "\0"
        /* _mesa_function_pool[21503]: QueryCounter (will be remapped) */
        "ii\0"
        "glQueryCounter\0"
        "\0"
        /* _mesa_function_pool[21522]: VertexP2ui (will be remapped) */
        "ii\0"
        "glVertexP2ui\0"
        "\0"
        /* _mesa_function_pool[21539]: VertexP3ui (will be remapped) */
        "ii\0"
        "glVertexP3ui\0"
        "\0"
        /* _mesa_function_pool[21556]: VertexP4ui (will be remapped) */
        "ii\0"
        "glVertexP4ui\0"
        "\0"
        /* _mesa_function_pool[21573]: VertexP2uiv (will be remapped) */
        "ip\0"
        "glVertexP2uiv\0"
        "\0"
        /* _mesa_function_pool[21591]: VertexP3uiv (will be remapped) */
        "ip\0"
        "glVertexP3uiv\0"
        "\0"
        /* _mesa_function_pool[21609]: VertexP4uiv (will be remapped) */
        "ip\0"
        "glVertexP4uiv\0"
        "\0"
        /* _mesa_function_pool[21627]: TexCoordP1ui (will be remapped) */
        "ii\0"
        "glTexCoordP1ui\0"
        "\0"
        /* _mesa_function_pool[21646]: TexCoordP2ui (will be remapped) */
        "ii\0"
        "glTexCoordP2ui\0"
        "\0"
        /* _mesa_function_pool[21665]: TexCoordP3ui (will be remapped) */
        "ii\0"
        "glTexCoordP3ui\0"
        "\0"
        /* _mesa_function_pool[21684]: TexCoordP4ui (will be remapped) */
        "ii\0"
        "glTexCoordP4ui\0"
        "\0"
        /* _mesa_function_pool[21703]: TexCoordP1uiv (will be remapped) */
        "ip\0"
        "glTexCoordP1uiv\0"
        "\0"
        /* _mesa_function_pool[21723]: TexCoordP2uiv (will be remapped) */
        "ip\0"
        "glTexCoordP2uiv\0"
        "\0"
        /* _mesa_function_pool[21743]: TexCoordP3uiv (will be remapped) */
        "ip\0"
        "glTexCoordP3uiv\0"
        "\0"
        /* _mesa_function_pool[21763]: TexCoordP4uiv (will be remapped) */
        "ip\0"
        "glTexCoordP4uiv\0"
        "\0"
        /* _mesa_function_pool[21783]: MultiTexCoordP1ui (will be remapped) */
        "iii\0"
        "glMultiTexCoordP1ui\0"
        "\0"
        /* _mesa_function_pool[21808]: MultiTexCoordP2ui (will be remapped) */
        "iii\0"
        "glMultiTexCoordP2ui\0"
        "\0"
        /* _mesa_function_pool[21833]: MultiTexCoordP3ui (will be remapped) */
        "iii\0"
        "glMultiTexCoordP3ui\0"
        "\0"
        /* _mesa_function_pool[21858]: MultiTexCoordP4ui (will be remapped) */
        "iii\0"
        "glMultiTexCoordP4ui\0"
        "\0"
        /* _mesa_function_pool[21883]: MultiTexCoordP1uiv (will be remapped) */
        "iip\0"
        "glMultiTexCoordP1uiv\0"
        "\0"
        /* _mesa_function_pool[21909]: MultiTexCoordP2uiv (will be remapped) */
        "iip\0"
        "glMultiTexCoordP2uiv\0"
        "\0"
        /* _mesa_function_pool[21935]: MultiTexCoordP3uiv (will be remapped) */
        "iip\0"
        "glMultiTexCoordP3uiv\0"
        "\0"
        /* _mesa_function_pool[21961]: MultiTexCoordP4uiv (will be remapped) */
        "iip\0"
        "glMultiTexCoordP4uiv\0"
        "\0"
        /* _mesa_function_pool[21987]: NormalP3ui (will be remapped) */
        "ii\0"
        "glNormalP3ui\0"
        "\0"
        /* _mesa_function_pool[22004]: NormalP3uiv (will be remapped) */
        "ip\0"
        "glNormalP3uiv\0"
        "\0"
        /* _mesa_function_pool[22022]: ColorP3ui (will be remapped) */
        "ii\0"
        "glColorP3ui\0"
        "\0"
        /* _mesa_function_pool[22038]: ColorP4ui (will be remapped) */
        "ii\0"
        "glColorP4ui\0"
        "\0"
        /* _mesa_function_pool[22054]: ColorP3uiv (will be remapped) */
        "ip\0"
        "glColorP3uiv\0"
        "\0"
        /* _mesa_function_pool[22071]: ColorP4uiv (will be remapped) */
        "ip\0"
        "glColorP4uiv\0"
        "\0"
        /* _mesa_function_pool[22088]: SecondaryColorP3ui (will be remapped) */
        "ii\0"
        "glSecondaryColorP3ui\0"
        "\0"
        /* _mesa_function_pool[22113]: SecondaryColorP3uiv (will be remapped) */
        "ip\0"
        "glSecondaryColorP3uiv\0"
        "\0"
        /* _mesa_function_pool[22139]: VertexAttribP1ui (will be remapped) */
        "iiii\0"
        "glVertexAttribP1ui\0"
        "\0"
        /* _mesa_function_pool[22164]: VertexAttribP2ui (will be remapped) */
        "iiii\0"
        "glVertexAttribP2ui\0"
        "\0"
        /* _mesa_function_pool[22189]: VertexAttribP3ui (will be remapped) */
        "iiii\0"
        "glVertexAttribP3ui\0"
        "\0"
        /* _mesa_function_pool[22214]: VertexAttribP4ui (will be remapped) */
        "iiii\0"
        "glVertexAttribP4ui\0"
        "\0"
        /* _mesa_function_pool[22239]: VertexAttribP1uiv (will be remapped) */
        "iiip\0"
        "glVertexAttribP1uiv\0"
        "\0"
        /* _mesa_function_pool[22265]: VertexAttribP2uiv (will be remapped) */
        "iiip\0"
        "glVertexAttribP2uiv\0"
        "\0"
        /* _mesa_function_pool[22291]: VertexAttribP3uiv (will be remapped) */
        "iiip\0"
        "glVertexAttribP3uiv\0"
        "\0"
        /* _mesa_function_pool[22317]: VertexAttribP4uiv (will be remapped) */
        "iiip\0"
        "glVertexAttribP4uiv\0"
        "\0"
        /* _mesa_function_pool[22343]: GetSubroutineUniformLocation (will be remapped) */
        "iip\0"
        "glGetSubroutineUniformLocation\0"
        "\0"
        /* _mesa_function_pool[22379]: GetSubroutineIndex (will be remapped) */
        "iip\0"
        "glGetSubroutineIndex\0"
        "\0"
        /* _mesa_function_pool[22405]: GetActiveSubroutineUniformiv (will be remapped) */
        "iiiip\0"
        "glGetActiveSubroutineUniformiv\0"
        "\0"
        /* _mesa_function_pool[22443]: GetActiveSubroutineUniformName (will be remapped) */
        "iiiipp\0"
        "glGetActiveSubroutineUniformName\0"
        "\0"
        /* _mesa_function_pool[22484]: GetActiveSubroutineName (will be remapped) */
        "iiiipp\0"
        "glGetActiveSubroutineName\0"
        "\0"
        /* _mesa_function_pool[22518]: UniformSubroutinesuiv (will be remapped) */
        "iip\0"
        "glUniformSubroutinesuiv\0"
        "\0"
        /* _mesa_function_pool[22547]: GetUniformSubroutineuiv (will be remapped) */
        "iip\0"
        "glGetUniformSubroutineuiv\0"
        "\0"
        /* _mesa_function_pool[22578]: GetProgramStageiv (will be remapped) */
        "iiip\0"
        "glGetProgramStageiv\0"
        "\0"
        /* _mesa_function_pool[22604]: PatchParameteri (will be remapped) */
        "ii\0"
        "glPatchParameteri\0"
        "glPatchParameteriEXT\0"
        "glPatchParameteriOES\0"
        "\0"
        /* _mesa_function_pool[22668]: PatchParameterfv (will be remapped) */
        "ip\0"
        "glPatchParameterfv\0"
        "\0"
        /* _mesa_function_pool[22691]: DrawArraysIndirect (will be remapped) */
        "ip\0"
        "glDrawArraysIndirect\0"
        "\0"
        /* _mesa_function_pool[22716]: DrawElementsIndirect (will be remapped) */
        "iip\0"
        "glDrawElementsIndirect\0"
        "\0"
        /* _mesa_function_pool[22744]: MultiDrawArraysIndirect (will be remapped) */
        "ipii\0"
        "glMultiDrawArraysIndirect\0"
        "glMultiDrawArraysIndirectAMD\0"
        "\0"
        /* _mesa_function_pool[22805]: MultiDrawElementsIndirect (will be remapped) */
        "iipii\0"
        "glMultiDrawElementsIndirect\0"
        "glMultiDrawElementsIndirectAMD\0"
        "\0"
        /* _mesa_function_pool[22871]: Uniform1d (will be remapped) */
        "id\0"
        "glUniform1d\0"
        "\0"
        /* _mesa_function_pool[22887]: Uniform2d (will be remapped) */
        "idd\0"
        "glUniform2d\0"
        "\0"
        /* _mesa_function_pool[22904]: Uniform3d (will be remapped) */
        "iddd\0"
        "glUniform3d\0"
        "\0"
        /* _mesa_function_pool[22922]: Uniform4d (will be remapped) */
        "idddd\0"
        "glUniform4d\0"
        "\0"
        /* _mesa_function_pool[22941]: Uniform1dv (will be remapped) */
        "iip\0"
        "glUniform1dv\0"
        "\0"
        /* _mesa_function_pool[22959]: Uniform2dv (will be remapped) */
        "iip\0"
        "glUniform2dv\0"
        "\0"
        /* _mesa_function_pool[22977]: Uniform3dv (will be remapped) */
        "iip\0"
        "glUniform3dv\0"
        "\0"
        /* _mesa_function_pool[22995]: Uniform4dv (will be remapped) */
        "iip\0"
        "glUniform4dv\0"
        "\0"
        /* _mesa_function_pool[23013]: UniformMatrix2dv (will be remapped) */
        "iiip\0"
        "glUniformMatrix2dv\0"
        "\0"
        /* _mesa_function_pool[23038]: UniformMatrix3dv (will be remapped) */
        "iiip\0"
        "glUniformMatrix3dv\0"
        "\0"
        /* _mesa_function_pool[23063]: UniformMatrix4dv (will be remapped) */
        "iiip\0"
        "glUniformMatrix4dv\0"
        "\0"
        /* _mesa_function_pool[23088]: UniformMatrix2x3dv (will be remapped) */
        "iiip\0"
        "glUniformMatrix2x3dv\0"
        "\0"
        /* _mesa_function_pool[23115]: UniformMatrix2x4dv (will be remapped) */
        "iiip\0"
        "glUniformMatrix2x4dv\0"
        "\0"
        /* _mesa_function_pool[23142]: UniformMatrix3x2dv (will be remapped) */
        "iiip\0"
        "glUniformMatrix3x2dv\0"
        "\0"
        /* _mesa_function_pool[23169]: UniformMatrix3x4dv (will be remapped) */
        "iiip\0"
        "glUniformMatrix3x4dv\0"
        "\0"
        /* _mesa_function_pool[23196]: UniformMatrix4x2dv (will be remapped) */
        "iiip\0"
        "glUniformMatrix4x2dv\0"
        "\0"
        /* _mesa_function_pool[23223]: UniformMatrix4x3dv (will be remapped) */
        "iiip\0"
        "glUniformMatrix4x3dv\0"
        "\0"
        /* _mesa_function_pool[23250]: GetUniformdv (will be remapped) */
        "iip\0"
        "glGetUniformdv\0"
        "\0"
        /* _mesa_function_pool[23270]: ProgramUniform1d (will be remapped) */
        "iid\0"
        "glProgramUniform1dEXT\0"
        "glProgramUniform1d\0"
        "\0"
        /* _mesa_function_pool[23316]: ProgramUniform2d (will be remapped) */
        "iidd\0"
        "glProgramUniform2dEXT\0"
        "glProgramUniform2d\0"
        "\0"
        /* _mesa_function_pool[23363]: ProgramUniform3d (will be remapped) */
        "iiddd\0"
        "glProgramUniform3dEXT\0"
        "glProgramUniform3d\0"
        "\0"
        /* _mesa_function_pool[23411]: ProgramUniform4d (will be remapped) */
        "iidddd\0"
        "glProgramUniform4dEXT\0"
        "glProgramUniform4d\0"
        "\0"
        /* _mesa_function_pool[23460]: ProgramUniform1dv (will be remapped) */
        "iiip\0"
        "glProgramUniform1dvEXT\0"
        "glProgramUniform1dv\0"
        "\0"
        /* _mesa_function_pool[23509]: ProgramUniform2dv (will be remapped) */
        "iiip\0"
        "glProgramUniform2dvEXT\0"
        "glProgramUniform2dv\0"
        "\0"
        /* _mesa_function_pool[23558]: ProgramUniform3dv (will be remapped) */
        "iiip\0"
        "glProgramUniform3dvEXT\0"
        "glProgramUniform3dv\0"
        "\0"
        /* _mesa_function_pool[23607]: ProgramUniform4dv (will be remapped) */
        "iiip\0"
        "glProgramUniform4dvEXT\0"
        "glProgramUniform4dv\0"
        "\0"
        /* _mesa_function_pool[23656]: ProgramUniformMatrix2dv (will be remapped) */
        "iiiip\0"
        "glProgramUniformMatrix2dvEXT\0"
        "glProgramUniformMatrix2dv\0"
        "\0"
        /* _mesa_function_pool[23718]: ProgramUniformMatrix3dv (will be remapped) */
        "iiiip\0"
        "glProgramUniformMatrix3dvEXT\0"
        "glProgramUniformMatrix3dv\0"
        "\0"
        /* _mesa_function_pool[23780]: ProgramUniformMatrix4dv (will be remapped) */
        "iiiip\0"
        "glProgramUniformMatrix4dvEXT\0"
        "glProgramUniformMatrix4dv\0"
        "\0"
        /* _mesa_function_pool[23842]: ProgramUniformMatrix2x3dv (will be remapped) */
        "iiiip\0"
        "glProgramUniformMatrix2x3dvEXT\0"
        "glProgramUniformMatrix2x3dv\0"
        "\0"
        /* _mesa_function_pool[23908]: ProgramUniformMatrix2x4dv (will be remapped) */
        "iiiip\0"
        "glProgramUniformMatrix2x4dvEXT\0"
        "glProgramUniformMatrix2x4dv\0"
        "\0"
        /* _mesa_function_pool[23974]: ProgramUniformMatrix3x2dv (will be remapped) */
        "iiiip\0"
        "glProgramUniformMatrix3x2dvEXT\0"
        "glProgramUniformMatrix3x2dv\0"
        "\0"
        /* _mesa_function_pool[24040]: ProgramUniformMatrix3x4dv (will be remapped) */
        "iiiip\0"
        "glProgramUniformMatrix3x4dvEXT\0"
        "glProgramUniformMatrix3x4dv\0"
        "\0"
        /* _mesa_function_pool[24106]: ProgramUniformMatrix4x2dv (will be remapped) */
        "iiiip\0"
        "glProgramUniformMatrix4x2dvEXT\0"
        "glProgramUniformMatrix4x2dv\0"
        "\0"
        /* _mesa_function_pool[24172]: ProgramUniformMatrix4x3dv (will be remapped) */
        "iiiip\0"
        "glProgramUniformMatrix4x3dvEXT\0"
        "glProgramUniformMatrix4x3dv\0"
        "\0"
        /* _mesa_function_pool[24238]: DrawTransformFeedbackStream (will be remapped) */
        "iii\0"
        "glDrawTransformFeedbackStream\0"
        "\0"
        /* _mesa_function_pool[24273]: BeginQueryIndexed (will be remapped) */
        "iii\0"
        "glBeginQueryIndexed\0"
        "\0"
        /* _mesa_function_pool[24298]: EndQueryIndexed (will be remapped) */
        "ii\0"
        "glEndQueryIndexed\0"
        "\0"
        /* _mesa_function_pool[24320]: GetQueryIndexediv (will be remapped) */
        "iiip\0"
        "glGetQueryIndexediv\0"
        "\0"
        /* _mesa_function_pool[24346]: UseProgramStages (will be remapped) */
        "iii\0"
        "glUseProgramStages\0"
        "glUseProgramStagesEXT\0"
        "\0"
        /* _mesa_function_pool[24392]: ActiveShaderProgram (will be remapped) */
        "ii\0"
        "glActiveShaderProgram\0"
        "glActiveShaderProgramEXT\0"
        "\0"
        /* _mesa_function_pool[24443]: CreateShaderProgramv (will be remapped) */
        "iip\0"
        "glCreateShaderProgramv\0"
        "glCreateShaderProgramvEXT\0"
        "\0"
        /* _mesa_function_pool[24497]: BindProgramPipeline (will be remapped) */
        "i\0"
        "glBindProgramPipeline\0"
        "glBindProgramPipelineEXT\0"
        "\0"
        /* _mesa_function_pool[24547]: DeleteProgramPipelines (will be remapped) */
        "ip\0"
        "glDeleteProgramPipelines\0"
        "glDeleteProgramPipelinesEXT\0"
        "\0"
        /* _mesa_function_pool[24604]: GenProgramPipelines (will be remapped) */
        "ip\0"
        "glGenProgramPipelines\0"
        "glGenProgramPipelinesEXT\0"
        "\0"
        /* _mesa_function_pool[24655]: IsProgramPipeline (will be remapped) */
        "i\0"
        "glIsProgramPipeline\0"
        "glIsProgramPipelineEXT\0"
        "\0"
        /* _mesa_function_pool[24701]: GetProgramPipelineiv (will be remapped) */
        "iip\0"
        "glGetProgramPipelineiv\0"
        "glGetProgramPipelineivEXT\0"
        "\0"
        /* _mesa_function_pool[24755]: ProgramUniform1i (will be remapped) */
        "iii\0"
        "glProgramUniform1i\0"
        "glProgramUniform1iEXT\0"
        "\0"
        /* _mesa_function_pool[24801]: ProgramUniform2i (will be remapped) */
        "iiii\0"
        "glProgramUniform2i\0"
        "glProgramUniform2iEXT\0"
        "\0"
        /* _mesa_function_pool[24848]: ProgramUniform3i (will be remapped) */
        "iiiii\0"
        "glProgramUniform3i\0"
        "glProgramUniform3iEXT\0"
        "\0"
        /* _mesa_function_pool[24896]: ProgramUniform4i (will be remapped) */
        "iiiiii\0"
        "glProgramUniform4i\0"
        "glProgramUniform4iEXT\0"
        "\0"
        /* _mesa_function_pool[24945]: ProgramUniform1ui (will be remapped) */
        "iii\0"
        "glProgramUniform1ui\0"
        "glProgramUniform1uiEXT\0"
        "\0"
        /* _mesa_function_pool[24993]: ProgramUniform2ui (will be remapped) */
        "iiii\0"
        "glProgramUniform2ui\0"
        "glProgramUniform2uiEXT\0"
        "\0"
        /* _mesa_function_pool[25042]: ProgramUniform3ui (will be remapped) */
        "iiiii\0"
        "glProgramUniform3ui\0"
        "glProgramUniform3uiEXT\0"
        "\0"
        /* _mesa_function_pool[25092]: ProgramUniform4ui (will be remapped) */
        "iiiiii\0"
        "glProgramUniform4ui\0"
        "glProgramUniform4uiEXT\0"
        "\0"
        /* _mesa_function_pool[25143]: ProgramUniform1f (will be remapped) */
        "iif\0"
        "glProgramUniform1f\0"
        "glProgramUniform1fEXT\0"
        "\0"
        /* _mesa_function_pool[25189]: ProgramUniform2f (will be remapped) */
        "iiff\0"
        "glProgramUniform2f\0"
        "glProgramUniform2fEXT\0"
        "\0"
        /* _mesa_function_pool[25236]: ProgramUniform3f (will be remapped) */
        "iifff\0"
        "glProgramUniform3f\0"
        "glProgramUniform3fEXT\0"
        "\0"
        /* _mesa_function_pool[25284]: ProgramUniform4f (will be remapped) */
        "iiffff\0"
        "glProgramUniform4f\0"
        "glProgramUniform4fEXT\0"
        "\0"
        /* _mesa_function_pool[25333]: ProgramUniform1iv (will be remapped) */
        "iiip\0"
        "glProgramUniform1iv\0"
        "glProgramUniform1ivEXT\0"
        "\0"
        /* _mesa_function_pool[25382]: ProgramUniform2iv (will be remapped) */
        "iiip\0"
        "glProgramUniform2iv\0"
        "glProgramUniform2ivEXT\0"
        "\0"
        /* _mesa_function_pool[25431]: ProgramUniform3iv (will be remapped) */
        "iiip\0"
        "glProgramUniform3iv\0"
        "glProgramUniform3ivEXT\0"
        "\0"
        /* _mesa_function_pool[25480]: ProgramUniform4iv (will be remapped) */
        "iiip\0"
        "glProgramUniform4iv\0"
        "glProgramUniform4ivEXT\0"
        "\0"
        /* _mesa_function_pool[25529]: ProgramUniform1uiv (will be remapped) */
        "iiip\0"
        "glProgramUniform1uiv\0"
        "glProgramUniform1uivEXT\0"
        "\0"
        /* _mesa_function_pool[25580]: ProgramUniform2uiv (will be remapped) */
        "iiip\0"
        "glProgramUniform2uiv\0"
        "glProgramUniform2uivEXT\0"
        "\0"
        /* _mesa_function_pool[25631]: ProgramUniform3uiv (will be remapped) */
        "iiip\0"
        "glProgramUniform3uiv\0"
        "glProgramUniform3uivEXT\0"
        "\0"
        /* _mesa_function_pool[25682]: ProgramUniform4uiv (will be remapped) */
        "iiip\0"
        "glProgramUniform4uiv\0"
        "glProgramUniform4uivEXT\0"
        "\0"
        /* _mesa_function_pool[25733]: ProgramUniform1fv (will be remapped) */
        "iiip\0"
        "glProgramUniform1fv\0"
        "glProgramUniform1fvEXT\0"
        "\0"
        /* _mesa_function_pool[25782]: ProgramUniform2fv (will be remapped) */
        "iiip\0"
        "glProgramUniform2fv\0"
        "glProgramUniform2fvEXT\0"
        "\0"
        /* _mesa_function_pool[25831]: ProgramUniform3fv (will be remapped) */
        "iiip\0"
        "glProgramUniform3fv\0"
        "glProgramUniform3fvEXT\0"
        "\0"
        /* _mesa_function_pool[25880]: ProgramUniform4fv (will be remapped) */
        "iiip\0"
        "glProgramUniform4fv\0"
        "glProgramUniform4fvEXT\0"
        "\0"
        /* _mesa_function_pool[25929]: ProgramUniformMatrix2fv (will be remapped) */
        "iiiip\0"
        "glProgramUniformMatrix2fv\0"
        "glProgramUniformMatrix2fvEXT\0"
        "\0"
        /* _mesa_function_pool[25991]: ProgramUniformMatrix3fv (will be remapped) */
        "iiiip\0"
        "glProgramUniformMatrix3fv\0"
        "glProgramUniformMatrix3fvEXT\0"
        "\0"
        /* _mesa_function_pool[26053]: ProgramUniformMatrix4fv (will be remapped) */
        "iiiip\0"
        "glProgramUniformMatrix4fv\0"
        "glProgramUniformMatrix4fvEXT\0"
        "\0"
        /* _mesa_function_pool[26115]: ProgramUniformMatrix2x3fv (will be remapped) */
        "iiiip\0"
        "glProgramUniformMatrix2x3fv\0"
        "glProgramUniformMatrix2x3fvEXT\0"
        "\0"
        /* _mesa_function_pool[26181]: ProgramUniformMatrix3x2fv (will be remapped) */
        "iiiip\0"
        "glProgramUniformMatrix3x2fv\0"
        "glProgramUniformMatrix3x2fvEXT\0"
        "\0"
        /* _mesa_function_pool[26247]: ProgramUniformMatrix2x4fv (will be remapped) */
        "iiiip\0"
        "glProgramUniformMatrix2x4fv\0"
        "glProgramUniformMatrix2x4fvEXT\0"
        "\0"
        /* _mesa_function_pool[26313]: ProgramUniformMatrix4x2fv (will be remapped) */
        "iiiip\0"
        "glProgramUniformMatrix4x2fv\0"
        "glProgramUniformMatrix4x2fvEXT\0"
        "\0"
        /* _mesa_function_pool[26379]: ProgramUniformMatrix3x4fv (will be remapped) */
        "iiiip\0"
        "glProgramUniformMatrix3x4fv\0"
        "glProgramUniformMatrix3x4fvEXT\0"
        "\0"
        /* _mesa_function_pool[26445]: ProgramUniformMatrix4x3fv (will be remapped) */
        "iiiip\0"
        "glProgramUniformMatrix4x3fv\0"
        "glProgramUniformMatrix4x3fvEXT\0"
        "\0"
        /* _mesa_function_pool[26511]: ValidateProgramPipeline (will be remapped) */
        "i\0"
        "glValidateProgramPipeline\0"
        "glValidateProgramPipelineEXT\0"
        "\0"
        /* _mesa_function_pool[26569]: GetProgramPipelineInfoLog (will be remapped) */
        "iipp\0"
        "glGetProgramPipelineInfoLog\0"
        "glGetProgramPipelineInfoLogEXT\0"
        "\0"
        /* _mesa_function_pool[26634]: VertexAttribL1d (will be remapped) */
        "id\0"
        "glVertexAttribL1d\0"
        "glVertexAttribL1dEXT\0"
        "\0"
        /* _mesa_function_pool[26677]: VertexAttribL2d (will be remapped) */
        "idd\0"
        "glVertexAttribL2d\0"
        "glVertexAttribL2dEXT\0"
        "\0"
        /* _mesa_function_pool[26721]: VertexAttribL3d (will be remapped) */
        "iddd\0"
        "glVertexAttribL3d\0"
        "glVertexAttribL3dEXT\0"
        "\0"
        /* _mesa_function_pool[26766]: VertexAttribL4d (will be remapped) */
        "idddd\0"
        "glVertexAttribL4d\0"
        "glVertexAttribL4dEXT\0"
        "\0"
        /* _mesa_function_pool[26812]: VertexAttribL1dv (will be remapped) */
        "ip\0"
        "glVertexAttribL1dv\0"
        "glVertexAttribL1dvEXT\0"
        "\0"
        /* _mesa_function_pool[26857]: VertexAttribL2dv (will be remapped) */
        "ip\0"
        "glVertexAttribL2dv\0"
        "glVertexAttribL2dvEXT\0"
        "\0"
        /* _mesa_function_pool[26902]: VertexAttribL3dv (will be remapped) */
        "ip\0"
        "glVertexAttribL3dv\0"
        "glVertexAttribL3dvEXT\0"
        "\0"
        /* _mesa_function_pool[26947]: VertexAttribL4dv (will be remapped) */
        "ip\0"
        "glVertexAttribL4dv\0"
        "glVertexAttribL4dvEXT\0"
        "\0"
        /* _mesa_function_pool[26992]: VertexAttribLPointer (will be remapped) */
        "iiiip\0"
        "glVertexAttribLPointer\0"
        "glVertexAttribLPointerEXT\0"
        "\0"
        /* _mesa_function_pool[27048]: GetVertexAttribLdv (will be remapped) */
        "iip\0"
        "glGetVertexAttribLdv\0"
        "glGetVertexAttribLdvEXT\0"
        "\0"
        /* _mesa_function_pool[27098]: VertexArrayVertexAttribLOffsetEXT (will be remapped) */
        "iiiiiii\0"
        "glVertexArrayVertexAttribLOffsetEXT\0"
        "\0"
        /* _mesa_function_pool[27143]: GetShaderPrecisionFormat (will be remapped) */
        "iipp\0"
        "glGetShaderPrecisionFormat\0"
        "\0"
        /* _mesa_function_pool[27176]: ReleaseShaderCompiler (will be remapped) */
        "\0"
        "glReleaseShaderCompiler\0"
        "\0"
        /* _mesa_function_pool[27202]: ShaderBinary (will be remapped) */
        "ipipi\0"
        "glShaderBinary\0"
        "\0"
        /* _mesa_function_pool[27224]: ClearDepthf (will be remapped) */
        "f\0"
        "glClearDepthf\0"
        "\0"
        /* _mesa_function_pool[27241]: DepthRangef (will be remapped) */
        "ff\0"
        "glDepthRangef\0"
        "\0"
        /* _mesa_function_pool[27259]: GetProgramBinary (will be remapped) */
        "iippp\0"
        "glGetProgramBinary\0"
        "\0"
        /* _mesa_function_pool[27285]: ProgramBinary (will be remapped) */
        "iipi\0"
        "glProgramBinary\0"
        "\0"
        /* _mesa_function_pool[27307]: ProgramParameteri (will be remapped) */
        "iii\0"
        "glProgramParameteri\0"
        "glProgramParameteriEXT\0"
        "\0"
        /* _mesa_function_pool[27355]: DebugMessageControl (will be remapped) */
        "iiiipi\0"
        "glDebugMessageControlARB\0"
        "glDebugMessageControl\0"
        "glDebugMessageControlKHR\0"
        "\0"
        /* _mesa_function_pool[27435]: DebugMessageInsert (will be remapped) */
        "iiiiip\0"
        "glDebugMessageInsertARB\0"
        "glDebugMessageInsert\0"
        "glDebugMessageInsertKHR\0"
        "\0"
        /* _mesa_function_pool[27512]: DebugMessageCallback (will be remapped) */
        "pp\0"
        "glDebugMessageCallbackARB\0"
        "glDebugMessageCallback\0"
        "glDebugMessageCallbackKHR\0"
        "\0"
        /* _mesa_function_pool[27591]: GetDebugMessageLog (will be remapped) */
        "iipppppp\0"
        "glGetDebugMessageLogARB\0"
        "glGetDebugMessageLog\0"
        "glGetDebugMessageLogKHR\0"
        "\0"
        /* _mesa_function_pool[27670]: GetGraphicsResetStatusARB (will be remapped) */
        "\0"
        "glGetGraphicsResetStatusARB\0"
        "glGetGraphicsResetStatus\0"
        "\0"
        /* _mesa_function_pool[27725]: GetnMapdvARB (will be remapped) */
        "iiip\0"
        "glGetnMapdvARB\0"
        "\0"
        /* _mesa_function_pool[27746]: GetnMapfvARB (will be remapped) */
        "iiip\0"
        "glGetnMapfvARB\0"
        "\0"
        /* _mesa_function_pool[27767]: GetnMapivARB (will be remapped) */
        "iiip\0"
        "glGetnMapivARB\0"
        "\0"
        /* _mesa_function_pool[27788]: GetnPixelMapfvARB (will be remapped) */
        "iip\0"
        "glGetnPixelMapfvARB\0"
        "\0"
        /* _mesa_function_pool[27813]: GetnPixelMapuivARB (will be remapped) */
        "iip\0"
        "glGetnPixelMapuivARB\0"
        "\0"
        /* _mesa_function_pool[27839]: GetnPixelMapusvARB (will be remapped) */
        "iip\0"
        "glGetnPixelMapusvARB\0"
        "\0"
        /* _mesa_function_pool[27865]: GetnPolygonStippleARB (will be remapped) */
        "ip\0"
        "glGetnPolygonStippleARB\0"
        "\0"
        /* _mesa_function_pool[27893]: GetnTexImageARB (will be remapped) */
        "iiiiip\0"
        "glGetnTexImageARB\0"
        "\0"
        /* _mesa_function_pool[27919]: ReadnPixelsARB (will be remapped) */
        "iiiiiiip\0"
        "glReadnPixelsARB\0"
        "glReadnPixels\0"
        "\0"
        /* _mesa_function_pool[27960]: GetnColorTableARB (will be remapped) */
        "iiiip\0"
        "glGetnColorTableARB\0"
        "\0"
        /* _mesa_function_pool[27987]: GetnConvolutionFilterARB (will be remapped) */
        "iiiip\0"
        "glGetnConvolutionFilterARB\0"
        "\0"
        /* _mesa_function_pool[28021]: GetnSeparableFilterARB (will be remapped) */
        "iiiipipp\0"
        "glGetnSeparableFilterARB\0"
        "\0"
        /* _mesa_function_pool[28056]: GetnHistogramARB (will be remapped) */
        "iiiiip\0"
        "glGetnHistogramARB\0"
        "\0"
        /* _mesa_function_pool[28083]: GetnMinmaxARB (will be remapped) */
        "iiiiip\0"
        "glGetnMinmaxARB\0"
        "\0"
        /* _mesa_function_pool[28107]: GetnCompressedTexImageARB (will be remapped) */
        "iiip\0"
        "glGetnCompressedTexImageARB\0"
        "\0"
        /* _mesa_function_pool[28141]: GetnUniformfvARB (will be remapped) */
        "iiip\0"
        "glGetnUniformfvARB\0"
        "glGetnUniformfv\0"
        "\0"
        /* _mesa_function_pool[28182]: GetnUniformivARB (will be remapped) */
        "iiip\0"
        "glGetnUniformivARB\0"
        "glGetnUniformiv\0"
        "\0"
        /* _mesa_function_pool[28223]: GetnUniformuivARB (will be remapped) */
        "iiip\0"
        "glGetnUniformuivARB\0"
        "glGetnUniformuiv\0"
        "\0"
        /* _mesa_function_pool[28266]: GetnUniformdvARB (will be remapped) */
        "iiip\0"
        "glGetnUniformdvARB\0"
        "\0"
        /* _mesa_function_pool[28291]: DrawArraysInstancedBaseInstance (will be remapped) */
        "iiiii\0"
        "glDrawArraysInstancedBaseInstance\0"
        "\0"
        /* _mesa_function_pool[28332]: DrawElementsInstancedBaseInstance (will be remapped) */
        "iiipii\0"
        "glDrawElementsInstancedBaseInstance\0"
        "\0"
        /* _mesa_function_pool[28376]: DrawElementsInstancedBaseVertexBaseInstance (will be remapped) */
        "iiipiii\0"
        "glDrawElementsInstancedBaseVertexBaseInstance\0"
        "\0"
        /* _mesa_function_pool[28431]: DrawTransformFeedbackInstanced (will be remapped) */
        "iii\0"
        "glDrawTransformFeedbackInstanced\0"
        "\0"
        /* _mesa_function_pool[28469]: DrawTransformFeedbackStreamInstanced (will be remapped) */
        "iiii\0"
        "glDrawTransformFeedbackStreamInstanced\0"
        "\0"
        /* _mesa_function_pool[28514]: GetInternalformativ (will be remapped) */
        "iiiip\0"
        "glGetInternalformativ\0"
        "\0"
        /* _mesa_function_pool[28543]: GetActiveAtomicCounterBufferiv (will be remapped) */
        "iiip\0"
        "glGetActiveAtomicCounterBufferiv\0"
        "\0"
        /* _mesa_function_pool[28582]: BindImageTexture (will be remapped) */
        "iiiiiii\0"
        "glBindImageTexture\0"
        "\0"
        /* _mesa_function_pool[28610]: MemoryBarrier (will be remapped) */
        "i\0"
        "glMemoryBarrier\0"
        "glMemoryBarrierEXT\0"
        "\0"
        /* _mesa_function_pool[28648]: TexStorage1D (will be remapped) */
        "iiii\0"
        "glTexStorage1D\0"
        "\0"
        /* _mesa_function_pool[28669]: TexStorage2D (will be remapped) */
        "iiiii\0"
        "glTexStorage2D\0"
        "\0"
        /* _mesa_function_pool[28691]: TexStorage3D (will be remapped) */
        "iiiiii\0"
        "glTexStorage3D\0"
        "\0"
        /* _mesa_function_pool[28714]: TextureStorage1DEXT (will be remapped) */
        "iiiii\0"
        "glTextureStorage1DEXT\0"
        "\0"
        /* _mesa_function_pool[28743]: TextureStorage2DEXT (will be remapped) */
        "iiiiii\0"
        "glTextureStorage2DEXT\0"
        "\0"
        /* _mesa_function_pool[28773]: TextureStorage3DEXT (will be remapped) */
        "iiiiiii\0"
        "glTextureStorage3DEXT\0"
        "\0"
        /* _mesa_function_pool[28804]: PushDebugGroup (will be remapped) */
        "iiip\0"
        "glPushDebugGroup\0"
        "glPushDebugGroupKHR\0"
        "\0"
        /* _mesa_function_pool[28847]: PopDebugGroup (will be remapped) */
        "\0"
        "glPopDebugGroup\0"
        "glPopDebugGroupKHR\0"
        "\0"
        /* _mesa_function_pool[28884]: ObjectLabel (will be remapped) */
        "iiip\0"
        "glObjectLabel\0"
        "glObjectLabelKHR\0"
        "\0"
        /* _mesa_function_pool[28921]: GetObjectLabel (will be remapped) */
        "iiipp\0"
        "glGetObjectLabel\0"
        "glGetObjectLabelKHR\0"
        "\0"
        /* _mesa_function_pool[28965]: ObjectPtrLabel (will be remapped) */
        "pip\0"
        "glObjectPtrLabel\0"
        "glObjectPtrLabelKHR\0"
        "\0"
        /* _mesa_function_pool[29007]: GetObjectPtrLabel (will be remapped) */
        "pipp\0"
        "glGetObjectPtrLabel\0"
        "glGetObjectPtrLabelKHR\0"
        "\0"
        /* _mesa_function_pool[29056]: ClearBufferData (will be remapped) */
        "iiiip\0"
        "glClearBufferData\0"
        "\0"
        /* _mesa_function_pool[29081]: ClearBufferSubData (will be remapped) */
        "iiiiiip\0"
        "glClearBufferSubData\0"
        "\0"
        /* _mesa_function_pool[29111]: ClearNamedBufferDataEXT (will be remapped) */
        "iiiip\0"
        "glClearNamedBufferDataEXT\0"
        "\0"
        /* _mesa_function_pool[29144]: ClearNamedBufferSubDataEXT (will be remapped) */
        "iiiiiip\0"
        "glClearNamedBufferSubDataEXT\0"
        "\0"
        /* _mesa_function_pool[29182]: DispatchCompute (will be remapped) */
        "iii\0"
        "glDispatchCompute\0"
        "\0"
        /* _mesa_function_pool[29205]: DispatchComputeIndirect (will be remapped) */
        "i\0"
        "glDispatchComputeIndirect\0"
        "\0"
        /* _mesa_function_pool[29234]: CopyImageSubData (will be remapped) */
        "iiiiiiiiiiiiiii\0"
        "glCopyImageSubData\0"
        "\0"
        /* _mesa_function_pool[29270]: TextureView (will be remapped) */
        "iiiiiiii\0"
        "glTextureView\0"
        "\0"
        /* _mesa_function_pool[29294]: BindVertexBuffer (will be remapped) */
        "iiii\0"
        "glBindVertexBuffer\0"
        "\0"
        /* _mesa_function_pool[29319]: VertexAttribFormat (will be remapped) */
        "iiiii\0"
        "glVertexAttribFormat\0"
        "\0"
        /* _mesa_function_pool[29347]: VertexAttribIFormat (will be remapped) */
        "iiii\0"
        "glVertexAttribIFormat\0"
        "\0"
        /* _mesa_function_pool[29375]: VertexAttribLFormat (will be remapped) */
        "iiii\0"
        "glVertexAttribLFormat\0"
        "\0"
        /* _mesa_function_pool[29403]: VertexAttribBinding (will be remapped) */
        "ii\0"
        "glVertexAttribBinding\0"
        "\0"
        /* _mesa_function_pool[29429]: VertexBindingDivisor (will be remapped) */
        "ii\0"
        "glVertexBindingDivisor\0"
        "\0"
        /* _mesa_function_pool[29456]: VertexArrayBindVertexBufferEXT (will be remapped) */
        "iiiii\0"
        "glVertexArrayBindVertexBufferEXT\0"
        "\0"
        /* _mesa_function_pool[29496]: VertexArrayVertexAttribFormatEXT (will be remapped) */
        "iiiiii\0"
        "glVertexArrayVertexAttribFormatEXT\0"
        "\0"
        /* _mesa_function_pool[29539]: VertexArrayVertexAttribIFormatEXT (will be remapped) */
        "iiiii\0"
        "glVertexArrayVertexAttribIFormatEXT\0"
        "\0"
        /* _mesa_function_pool[29582]: VertexArrayVertexAttribLFormatEXT (will be remapped) */
        "iiiii\0"
        "glVertexArrayVertexAttribLFormatEXT\0"
        "\0"
        /* _mesa_function_pool[29625]: VertexArrayVertexAttribBindingEXT (will be remapped) */
        "iii\0"
        "glVertexArrayVertexAttribBindingEXT\0"
        "\0"
        /* _mesa_function_pool[29666]: VertexArrayVertexBindingDivisorEXT (will be remapped) */
        "iii\0"
        "glVertexArrayVertexBindingDivisorEXT\0"
        "\0"
        /* _mesa_function_pool[29708]: FramebufferParameteri (will be remapped) */
        "iii\0"
        "glFramebufferParameteri\0"
        "\0"
        /* _mesa_function_pool[29737]: GetFramebufferParameteriv (will be remapped) */
        "iip\0"
        "glGetFramebufferParameteriv\0"
        "\0"
        /* _mesa_function_pool[29770]: NamedFramebufferParameteriEXT (will be remapped) */
        "iii\0"
        "glNamedFramebufferParameteriEXT\0"
        "\0"
        /* _mesa_function_pool[29807]: GetNamedFramebufferParameterivEXT (will be remapped) */
        "iip\0"
        "glGetNamedFramebufferParameterivEXT\0"
        "\0"
        /* _mesa_function_pool[29848]: GetInternalformati64v (will be remapped) */
        "iiiip\0"
        "glGetInternalformati64v\0"
        "\0"
        /* _mesa_function_pool[29879]: InvalidateTexSubImage (will be remapped) */
        "iiiiiiii\0"
        "glInvalidateTexSubImage\0"
        "\0"
        /* _mesa_function_pool[29913]: InvalidateTexImage (will be remapped) */
        "ii\0"
        "glInvalidateTexImage\0"
        "\0"
        /* _mesa_function_pool[29938]: InvalidateBufferSubData (will be remapped) */
        "iii\0"
        "glInvalidateBufferSubData\0"
        "\0"
        /* _mesa_function_pool[29969]: InvalidateBufferData (will be remapped) */
        "i\0"
        "glInvalidateBufferData\0"
        "\0"
        /* _mesa_function_pool[29995]: InvalidateSubFramebuffer (will be remapped) */
        "iipiiii\0"
        "glInvalidateSubFramebuffer\0"
        "\0"
        /* _mesa_function_pool[30031]: InvalidateFramebuffer (will be remapped) */
        "iip\0"
        "glInvalidateFramebuffer\0"
        "\0"
        /* _mesa_function_pool[30060]: GetProgramInterfaceiv (will be remapped) */
        "iiip\0"
        "glGetProgramInterfaceiv\0"
        "\0"
        /* _mesa_function_pool[30090]: GetProgramResourceIndex (will be remapped) */
        "iip\0"
        "glGetProgramResourceIndex\0"
        "\0"
        /* _mesa_function_pool[30121]: GetProgramResourceName (will be remapped) */
        "iiiipp\0"
        "glGetProgramResourceName\0"
        "\0"
        /* _mesa_function_pool[30154]: GetProgramResourceiv (will be remapped) */
        "iiiipipp\0"
        "glGetProgramResourceiv\0"
        "\0"
        /* _mesa_function_pool[30187]: GetProgramResourceLocation (will be remapped) */
        "iip\0"
        "glGetProgramResourceLocation\0"
        "\0"
        /* _mesa_function_pool[30221]: GetProgramResourceLocationIndex (will be remapped) */
        "iip\0"
        "glGetProgramResourceLocationIndex\0"
        "\0"
        /* _mesa_function_pool[30260]: ShaderStorageBlockBinding (will be remapped) */
        "iii\0"
        "glShaderStorageBlockBinding\0"
        "\0"
        /* _mesa_function_pool[30293]: TexBufferRange (will be remapped) */
        "iiiii\0"
        "glTexBufferRange\0"
        "\0"
        /* _mesa_function_pool[30317]: TextureBufferRangeEXT (will be remapped) */
        "iiiiii\0"
        "glTextureBufferRangeEXT\0"
        "\0"
        /* _mesa_function_pool[30349]: TexStorage2DMultisample (will be remapped) */
        "iiiiii\0"
        "glTexStorage2DMultisample\0"
        "\0"
        /* _mesa_function_pool[30383]: TexStorage3DMultisample (will be remapped) */
        "iiiiiii\0"
        "glTexStorage3DMultisample\0"
        "\0"
        /* _mesa_function_pool[30418]: TextureStorage2DMultisampleEXT (will be remapped) */
        "iiiiiii\0"
        "glTextureStorage2DMultisampleEXT\0"
        "\0"
        /* _mesa_function_pool[30460]: TextureStorage3DMultisampleEXT (will be remapped) */
        "iiiiiiii\0"
        "glTextureStorage3DMultisampleEXT\0"
        "\0"
        /* _mesa_function_pool[30503]: BufferStorage (will be remapped) */
        "iipi\0"
        "glBufferStorage\0"
        "\0"
        /* _mesa_function_pool[30525]: NamedBufferStorageEXT (will be remapped) */
        "iipi\0"
        "glNamedBufferStorageEXT\0"
        "\0"
        /* _mesa_function_pool[30555]: ClearTexImage (will be remapped) */
        "iiiip\0"
        "glClearTexImage\0"
        "\0"
        /* _mesa_function_pool[30578]: ClearTexSubImage (will be remapped) */
        "iiiiiiiiiip\0"
        "glClearTexSubImage\0"
        "\0"
        /* _mesa_function_pool[30610]: BindBuffersBase (will be remapped) */
        "iiip\0"
        "glBindBuffersBase\0"
        "\0"
        /* _mesa_function_pool[30634]: BindBuffersRange (will be remapped) */
        "iiippp\0"
        "glBindBuffersRange\0"
        "\0"
        /* _mesa_function_pool[30661]: BindTextures (will be remapped) */
        "iip\0"
        "glBindTextures\0"
        "\0"
        /* _mesa_function_pool[30681]: BindSamplers (will be remapped) */
        "iip\0"
        "glBindSamplers\0"
        "\0"
        /* _mesa_function_pool[30701]: BindImageTextures (will be remapped) */
        "iip\0"
        "glBindImageTextures\0"
        "\0"
        /* _mesa_function_pool[30726]: BindVertexBuffers (will be remapped) */
        "iippp\0"
        "glBindVertexBuffers\0"
        "\0"
        /* _mesa_function_pool[30753]: GetTextureHandleARB (will be remapped) */
        "i\0"
        "glGetTextureHandleARB\0"
        "\0"
        /* _mesa_function_pool[30778]: GetTextureSamplerHandleARB (will be remapped) */
        "ii\0"
        "glGetTextureSamplerHandleARB\0"
        "\0"
        /* _mesa_function_pool[30811]: MakeTextureHandleResidentARB (will be remapped) */
        "i\0"
        "glMakeTextureHandleResidentARB\0"
        "\0"
        /* _mesa_function_pool[30845]: MakeTextureHandleNonResidentARB (will be remapped) */
        "i\0"
        "glMakeTextureHandleNonResidentARB\0"
        "\0"
        /* _mesa_function_pool[30882]: GetImageHandleARB (will be remapped) */
        "iiiii\0"
        "glGetImageHandleARB\0"
        "\0"
        /* _mesa_function_pool[30909]: MakeImageHandleResidentARB (will be remapped) */
        "ii\0"
        "glMakeImageHandleResidentARB\0"
        "\0"
        /* _mesa_function_pool[30942]: MakeImageHandleNonResidentARB (will be remapped) */
        "i\0"
        "glMakeImageHandleNonResidentARB\0"
        "\0"
        /* _mesa_function_pool[30977]: UniformHandleui64ARB (will be remapped) */
        "ii\0"
        "glUniformHandleui64ARB\0"
        "\0"
        /* _mesa_function_pool[31004]: UniformHandleui64vARB (will be remapped) */
        "iip\0"
        "glUniformHandleui64vARB\0"
        "\0"
        /* _mesa_function_pool[31033]: ProgramUniformHandleui64ARB (will be remapped) */
        "iii\0"
        "glProgramUniformHandleui64ARB\0"
        "\0"
        /* _mesa_function_pool[31068]: ProgramUniformHandleui64vARB (will be remapped) */
        "iiip\0"
        "glProgramUniformHandleui64vARB\0"
        "\0"
        /* _mesa_function_pool[31105]: IsTextureHandleResidentARB (will be remapped) */
        "i\0"
        "glIsTextureHandleResidentARB\0"
        "\0"
        /* _mesa_function_pool[31137]: IsImageHandleResidentARB (will be remapped) */
        "i\0"
        "glIsImageHandleResidentARB\0"
        "\0"
        /* _mesa_function_pool[31167]: VertexAttribL1ui64ARB (will be remapped) */
        "ii\0"
        "glVertexAttribL1ui64ARB\0"
        "\0"
        /* _mesa_function_pool[31195]: VertexAttribL1ui64vARB (will be remapped) */
        "ip\0"
        "glVertexAttribL1ui64vARB\0"
        "\0"
        /* _mesa_function_pool[31224]: GetVertexAttribLui64vARB (will be remapped) */
        "iip\0"
        "glGetVertexAttribLui64vARB\0"
        "\0"
        /* _mesa_function_pool[31256]: DispatchComputeGroupSizeARB (will be remapped) */
        "iiiiii\0"
        "glDispatchComputeGroupSizeARB\0"
        "\0"
        /* _mesa_function_pool[31294]: MultiDrawArraysIndirectCountARB (will be remapped) */
        "iiiii\0"
        "glMultiDrawArraysIndirectCountARB\0"
        "glMultiDrawArraysIndirectCount\0"
        "\0"
        /* _mesa_function_pool[31366]: MultiDrawElementsIndirectCountARB (will be remapped) */
        "iiiiii\0"
        "glMultiDrawElementsIndirectCountARB\0"
        "glMultiDrawElementsIndirectCount\0"
        "\0"
        /* _mesa_function_pool[31443]: ClipControl (will be remapped) */
        "ii\0"
        "glClipControl\0"
        "\0"
        /* _mesa_function_pool[31461]: CreateTransformFeedbacks (will be remapped) */
        "ip\0"
        "glCreateTransformFeedbacks\0"
        "\0"
        /* _mesa_function_pool[31492]: TransformFeedbackBufferBase (will be remapped) */
        "iii\0"
        "glTransformFeedbackBufferBase\0"
        "\0"
        /* _mesa_function_pool[31527]: TransformFeedbackBufferRange (will be remapped) */
        "iiiii\0"
        "glTransformFeedbackBufferRange\0"
        "\0"
        /* _mesa_function_pool[31565]: GetTransformFeedbackiv (will be remapped) */
        "iip\0"
        "glGetTransformFeedbackiv\0"
        "\0"
        /* _mesa_function_pool[31595]: GetTransformFeedbacki_v (will be remapped) */
        "iiip\0"
        "glGetTransformFeedbacki_v\0"
        "\0"
        /* _mesa_function_pool[31627]: GetTransformFeedbacki64_v (will be remapped) */
        "iiip\0"
        "glGetTransformFeedbacki64_v\0"
        "\0"
        /* _mesa_function_pool[31661]: CreateBuffers (will be remapped) */
        "ip\0"
        "glCreateBuffers\0"
        "\0"
        /* _mesa_function_pool[31681]: NamedBufferStorage (will be remapped) */
        "iipi\0"
        "glNamedBufferStorage\0"
        "\0"
        /* _mesa_function_pool[31708]: NamedBufferData (will be remapped) */
        "iipi\0"
        "glNamedBufferData\0"
        "\0"
        /* _mesa_function_pool[31732]: NamedBufferSubData (will be remapped) */
        "iiip\0"
        "glNamedBufferSubData\0"
        "\0"
        /* _mesa_function_pool[31759]: CopyNamedBufferSubData (will be remapped) */
        "iiiii\0"
        "glCopyNamedBufferSubData\0"
        "\0"
        /* _mesa_function_pool[31791]: ClearNamedBufferData (will be remapped) */
        "iiiip\0"
        "glClearNamedBufferData\0"
        "\0"
        /* _mesa_function_pool[31821]: ClearNamedBufferSubData (will be remapped) */
        "iiiiiip\0"
        "glClearNamedBufferSubData\0"
        "\0"
        /* _mesa_function_pool[31856]: MapNamedBuffer (will be remapped) */
        "ii\0"
        "glMapNamedBuffer\0"
        "\0"
        /* _mesa_function_pool[31877]: MapNamedBufferRange (will be remapped) */
        "iiii\0"
        "glMapNamedBufferRange\0"
        "\0"
        /* _mesa_function_pool[31905]: UnmapNamedBufferEXT (will be remapped) */
        "i\0"
        "glUnmapNamedBuffer\0"
        "glUnmapNamedBufferEXT\0"
        "\0"
        /* _mesa_function_pool[31949]: FlushMappedNamedBufferRange (will be remapped) */
        "iii\0"
        "glFlushMappedNamedBufferRange\0"
        "\0"
        /* _mesa_function_pool[31984]: GetNamedBufferParameteriv (will be remapped) */
        "iip\0"
        "glGetNamedBufferParameteriv\0"
        "\0"
        /* _mesa_function_pool[32017]: GetNamedBufferParameteri64v (will be remapped) */
        "iip\0"
        "glGetNamedBufferParameteri64v\0"
        "\0"
        /* _mesa_function_pool[32052]: GetNamedBufferPointerv (will be remapped) */
        "iip\0"
        "glGetNamedBufferPointerv\0"
        "\0"
        /* _mesa_function_pool[32082]: GetNamedBufferSubData (will be remapped) */
        "iiip\0"
        "glGetNamedBufferSubData\0"
        "\0"
        /* _mesa_function_pool[32112]: CreateFramebuffers (will be remapped) */
        "ip\0"
        "glCreateFramebuffers\0"
        "\0"
        /* _mesa_function_pool[32137]: NamedFramebufferRenderbuffer (will be remapped) */
        "iiii\0"
        "glNamedFramebufferRenderbuffer\0"
        "\0"
        /* _mesa_function_pool[32174]: NamedFramebufferParameteri (will be remapped) */
        "iii\0"
        "glNamedFramebufferParameteri\0"
        "\0"
        /* _mesa_function_pool[32208]: NamedFramebufferTexture (will be remapped) */
        "iiii\0"
        "glNamedFramebufferTexture\0"
        "\0"
        /* _mesa_function_pool[32240]: NamedFramebufferTextureLayer (will be remapped) */
        "iiiii\0"
        "glNamedFramebufferTextureLayer\0"
        "\0"
        /* _mesa_function_pool[32278]: NamedFramebufferDrawBuffer (will be remapped) */
        "ii\0"
        "glNamedFramebufferDrawBuffer\0"
        "\0"
        /* _mesa_function_pool[32311]: NamedFramebufferDrawBuffers (will be remapped) */
        "iip\0"
        "glNamedFramebufferDrawBuffers\0"
        "\0"
        /* _mesa_function_pool[32346]: NamedFramebufferReadBuffer (will be remapped) */
        "ii\0"
        "glNamedFramebufferReadBuffer\0"
        "\0"
        /* _mesa_function_pool[32379]: InvalidateNamedFramebufferData (will be remapped) */
        "iip\0"
        "glInvalidateNamedFramebufferData\0"
        "\0"
        /* _mesa_function_pool[32417]: InvalidateNamedFramebufferSubData (will be remapped) */
        "iipiiii\0"
        "glInvalidateNamedFramebufferSubData\0"
        "\0"
        /* _mesa_function_pool[32462]: ClearNamedFramebufferiv (will be remapped) */
        "iiip\0"
        "glClearNamedFramebufferiv\0"
        "\0"
        /* _mesa_function_pool[32494]: ClearNamedFramebufferuiv (will be remapped) */
        "iiip\0"
        "glClearNamedFramebufferuiv\0"
        "\0"
        /* _mesa_function_pool[32527]: ClearNamedFramebufferfv (will be remapped) */
        "iiip\0"
        "glClearNamedFramebufferfv\0"
        "\0"
        /* _mesa_function_pool[32559]: ClearNamedFramebufferfi (will be remapped) */
        "iiifi\0"
        "glClearNamedFramebufferfi\0"
        "\0"
        /* _mesa_function_pool[32592]: BlitNamedFramebuffer (will be remapped) */
        "iiiiiiiiiiii\0"
        "glBlitNamedFramebuffer\0"
        "\0"
        /* _mesa_function_pool[32629]: CheckNamedFramebufferStatus (will be remapped) */
        "ii\0"
        "glCheckNamedFramebufferStatus\0"
        "\0"
        /* _mesa_function_pool[32663]: GetNamedFramebufferParameteriv (will be remapped) */
        "iip\0"
        "glGetNamedFramebufferParameteriv\0"
        "\0"
        /* _mesa_function_pool[32701]: GetNamedFramebufferAttachmentParameteriv (will be remapped) */
        "iiip\0"
        "glGetNamedFramebufferAttachmentParameteriv\0"
        "\0"
        /* _mesa_function_pool[32750]: CreateRenderbuffers (will be remapped) */
        "ip\0"
        "glCreateRenderbuffers\0"
        "\0"
        /* _mesa_function_pool[32776]: NamedRenderbufferStorage (will be remapped) */
        "iiii\0"
        "glNamedRenderbufferStorage\0"
        "\0"
        /* _mesa_function_pool[32809]: NamedRenderbufferStorageMultisample (will be remapped) */
        "iiiii\0"
        "glNamedRenderbufferStorageMultisample\0"
        "\0"
        /* _mesa_function_pool[32854]: GetNamedRenderbufferParameteriv (will be remapped) */
        "iip\0"
        "glGetNamedRenderbufferParameteriv\0"
        "\0"
        /* _mesa_function_pool[32893]: CreateTextures (will be remapped) */
        "iip\0"
        "glCreateTextures\0"
        "\0"
        /* _mesa_function_pool[32915]: TextureBuffer (will be remapped) */
        "iii\0"
        "glTextureBuffer\0"
        "\0"
        /* _mesa_function_pool[32936]: TextureBufferRange (will be remapped) */
        "iiiii\0"
        "glTextureBufferRange\0"
        "\0"
        /* _mesa_function_pool[32964]: TextureStorage1D (will be remapped) */
        "iiii\0"
        "glTextureStorage1D\0"
        "\0"
        /* _mesa_function_pool[32989]: TextureStorage2D (will be remapped) */
        "iiiii\0"
        "glTextureStorage2D\0"
        "\0"
        /* _mesa_function_pool[33015]: TextureStorage3D (will be remapped) */
        "iiiiii\0"
        "glTextureStorage3D\0"
        "\0"
        /* _mesa_function_pool[33042]: TextureStorage2DMultisample (will be remapped) */
        "iiiiii\0"
        "glTextureStorage2DMultisample\0"
        "\0"
        /* _mesa_function_pool[33080]: TextureStorage3DMultisample (will be remapped) */
        "iiiiiii\0"
        "glTextureStorage3DMultisample\0"
        "\0"
        /* _mesa_function_pool[33119]: TextureSubImage1D (will be remapped) */
        "iiiiiip\0"
        "glTextureSubImage1D\0"
        "\0"
        /* _mesa_function_pool[33148]: TextureSubImage2D (will be remapped) */
        "iiiiiiiip\0"
        "glTextureSubImage2D\0"
        "\0"
        /* _mesa_function_pool[33179]: TextureSubImage3D (will be remapped) */
        "iiiiiiiiiip\0"
        "glTextureSubImage3D\0"
        "\0"
        /* _mesa_function_pool[33212]: CompressedTextureSubImage1D (will be remapped) */
        "iiiiiip\0"
        "glCompressedTextureSubImage1D\0"
        "\0"
        /* _mesa_function_pool[33251]: CompressedTextureSubImage2D (will be remapped) */
        "iiiiiiiip\0"
        "glCompressedTextureSubImage2D\0"
        "\0"
        /* _mesa_function_pool[33292]: CompressedTextureSubImage3D (will be remapped) */
        "iiiiiiiiiip\0"
        "glCompressedTextureSubImage3D\0"
        "\0"
        /* _mesa_function_pool[33335]: CopyTextureSubImage1D (will be remapped) */
        "iiiiii\0"
        "glCopyTextureSubImage1D\0"
        "\0"
        /* _mesa_function_pool[33367]: CopyTextureSubImage2D (will be remapped) */
        "iiiiiiii\0"
        "glCopyTextureSubImage2D\0"
        "\0"
        /* _mesa_function_pool[33401]: CopyTextureSubImage3D (will be remapped) */
        "iiiiiiiii\0"
        "glCopyTextureSubImage3D\0"
        "\0"
        /* _mesa_function_pool[33436]: TextureParameterf (will be remapped) */
        "iif\0"
        "glTextureParameterf\0"
        "\0"
        /* _mesa_function_pool[33461]: TextureParameterfv (will be remapped) */
        "iip\0"
        "glTextureParameterfv\0"
        "\0"
        /* _mesa_function_pool[33487]: TextureParameteri (will be remapped) */
        "iii\0"
        "glTextureParameteri\0"
        "\0"
        /* _mesa_function_pool[33512]: TextureParameterIiv (will be remapped) */
        "iip\0"
        "glTextureParameterIiv\0"
        "\0"
        /* _mesa_function_pool[33539]: TextureParameterIuiv (will be remapped) */
        "iip\0"
        "glTextureParameterIuiv\0"
        "\0"
        /* _mesa_function_pool[33567]: TextureParameteriv (will be remapped) */
        "iip\0"
        "glTextureParameteriv\0"
        "\0"
        /* _mesa_function_pool[33593]: GenerateTextureMipmap (will be remapped) */
        "i\0"
        "glGenerateTextureMipmap\0"
        "\0"
        /* _mesa_function_pool[33620]: BindTextureUnit (will be remapped) */
        "ii\0"
        "glBindTextureUnit\0"
        "\0"
        /* _mesa_function_pool[33642]: GetTextureImage (will be remapped) */
        "iiiiip\0"
        "glGetTextureImage\0"
        "\0"
        /* _mesa_function_pool[33668]: GetCompressedTextureImage (will be remapped) */
        "iiip\0"
        "glGetCompressedTextureImage\0"
        "\0"
        /* _mesa_function_pool[33702]: GetTextureLevelParameterfv (will be remapped) */
        "iiip\0"
        "glGetTextureLevelParameterfv\0"
        "\0"
        /* _mesa_function_pool[33737]: GetTextureLevelParameteriv (will be remapped) */
        "iiip\0"
        "glGetTextureLevelParameteriv\0"
        "\0"
        /* _mesa_function_pool[33772]: GetTextureParameterfv (will be remapped) */
        "iip\0"
        "glGetTextureParameterfv\0"
        "\0"
        /* _mesa_function_pool[33801]: GetTextureParameterIiv (will be remapped) */
        "iip\0"
        "glGetTextureParameterIiv\0"
        "\0"
        /* _mesa_function_pool[33831]: GetTextureParameterIuiv (will be remapped) */
        "iip\0"
        "glGetTextureParameterIuiv\0"
        "\0"
        /* _mesa_function_pool[33862]: GetTextureParameteriv (will be remapped) */
        "iip\0"
        "glGetTextureParameteriv\0"
        "\0"
        /* _mesa_function_pool[33891]: CreateVertexArrays (will be remapped) */
        "ip\0"
        "glCreateVertexArrays\0"
        "\0"
        /* _mesa_function_pool[33916]: DisableVertexArrayAttrib (will be remapped) */
        "ii\0"
        "glDisableVertexArrayAttrib\0"
        "\0"
        /* _mesa_function_pool[33947]: EnableVertexArrayAttrib (will be remapped) */
        "ii\0"
        "glEnableVertexArrayAttrib\0"
        "\0"
        /* _mesa_function_pool[33977]: VertexArrayElementBuffer (will be remapped) */
        "ii\0"
        "glVertexArrayElementBuffer\0"
        "\0"
        /* _mesa_function_pool[34008]: VertexArrayVertexBuffer (will be remapped) */
        "iiiii\0"
        "glVertexArrayVertexBuffer\0"
        "\0"
        /* _mesa_function_pool[34041]: VertexArrayVertexBuffers (will be remapped) */
        "iiippp\0"
        "glVertexArrayVertexBuffers\0"
        "\0"
        /* _mesa_function_pool[34076]: VertexArrayAttribFormat (will be remapped) */
        "iiiiii\0"
        "glVertexArrayAttribFormat\0"
        "\0"
        /* _mesa_function_pool[34110]: VertexArrayAttribIFormat (will be remapped) */
        "iiiii\0"
        "glVertexArrayAttribIFormat\0"
        "\0"
        /* _mesa_function_pool[34144]: VertexArrayAttribLFormat (will be remapped) */
        "iiiii\0"
        "glVertexArrayAttribLFormat\0"
        "\0"
        /* _mesa_function_pool[34178]: VertexArrayAttribBinding (will be remapped) */
        "iii\0"
        "glVertexArrayAttribBinding\0"
        "\0"
        /* _mesa_function_pool[34210]: VertexArrayBindingDivisor (will be remapped) */
        "iii\0"
        "glVertexArrayBindingDivisor\0"
        "\0"
        /* _mesa_function_pool[34243]: GetVertexArrayiv (will be remapped) */
        "iip\0"
        "glGetVertexArrayiv\0"
        "\0"
        /* _mesa_function_pool[34267]: GetVertexArrayIndexediv (will be remapped) */
        "iiip\0"
        "glGetVertexArrayIndexediv\0"
        "\0"
        /* _mesa_function_pool[34299]: GetVertexArrayIndexed64iv (will be remapped) */
        "iiip\0"
        "glGetVertexArrayIndexed64iv\0"
        "\0"
        /* _mesa_function_pool[34333]: CreateSamplers (will be remapped) */
        "ip\0"
        "glCreateSamplers\0"
        "\0"
        /* _mesa_function_pool[34354]: CreateProgramPipelines (will be remapped) */
        "ip\0"
        "glCreateProgramPipelines\0"
        "\0"
        /* _mesa_function_pool[34383]: CreateQueries (will be remapped) */
        "iip\0"
        "glCreateQueries\0"
        "\0"
        /* _mesa_function_pool[34404]: GetQueryBufferObjectiv (will be remapped) */
        "iiii\0"
        "glGetQueryBufferObjectiv\0"
        "\0"
        /* _mesa_function_pool[34435]: GetQueryBufferObjectuiv (will be remapped) */
        "iiii\0"
        "glGetQueryBufferObjectuiv\0"
        "\0"
        /* _mesa_function_pool[34467]: GetQueryBufferObjecti64v (will be remapped) */
        "iiii\0"
        "glGetQueryBufferObjecti64v\0"
        "\0"
        /* _mesa_function_pool[34500]: GetQueryBufferObjectui64v (will be remapped) */
        "iiii\0"
        "glGetQueryBufferObjectui64v\0"
        "\0"
        /* _mesa_function_pool[34534]: GetTextureSubImage (will be remapped) */
        "iiiiiiiiiiip\0"
        "glGetTextureSubImage\0"
        "\0"
        /* _mesa_function_pool[34569]: GetCompressedTextureSubImage (will be remapped) */
        "iiiiiiiiip\0"
        "glGetCompressedTextureSubImage\0"
        "\0"
        /* _mesa_function_pool[34612]: TextureBarrierNV (will be remapped) */
        "\0"
        "glTextureBarrier\0"
        "glTextureBarrierNV\0"
        "\0"
        /* _mesa_function_pool[34650]: BufferPageCommitmentARB (will be remapped) */
        "iiii\0"
        "glBufferPageCommitmentARB\0"
        "\0"
        /* _mesa_function_pool[34682]: NamedBufferPageCommitmentEXT (will be remapped) */
        "iiii\0"
        "glNamedBufferPageCommitmentEXT\0"
        "\0"
        /* _mesa_function_pool[34719]: NamedBufferPageCommitmentARB (will be remapped) */
        "iiii\0"
        "glNamedBufferPageCommitmentARB\0"
        "\0"
        /* _mesa_function_pool[34756]: PrimitiveBoundingBox (will be remapped) */
        "ffffffff\0"
        "glPrimitiveBoundingBox\0"
        "glPrimitiveBoundingBoxARB\0"
        "\0"
        /* _mesa_function_pool[34815]: BlendBarrier (will be remapped) */
        "\0"
        "glBlendBarrier\0"
        "glBlendBarrierKHR\0"
        "\0"
        /* _mesa_function_pool[34850]: Uniform1i64ARB (will be remapped) */
        "ii\0"
        "glUniform1i64ARB\0"
        "glUniform1i64NV\0"
        "\0"
        /* _mesa_function_pool[34887]: Uniform2i64ARB (will be remapped) */
        "iii\0"
        "glUniform2i64ARB\0"
        "glUniform2i64NV\0"
        "\0"
        /* _mesa_function_pool[34925]: Uniform3i64ARB (will be remapped) */
        "iiii\0"
        "glUniform3i64ARB\0"
        "glUniform3i64NV\0"
        "\0"
        /* _mesa_function_pool[34964]: Uniform4i64ARB (will be remapped) */
        "iiiii\0"
        "glUniform4i64ARB\0"
        "glUniform4i64NV\0"
        "\0"
        /* _mesa_function_pool[35004]: Uniform1i64vARB (will be remapped) */
        "iip\0"
        "glUniform1i64vARB\0"
        "glUniform1i64vNV\0"
        "\0"
        /* _mesa_function_pool[35044]: Uniform2i64vARB (will be remapped) */
        "iip\0"
        "glUniform2i64vARB\0"
        "glUniform2i64vNV\0"
        "\0"
        /* _mesa_function_pool[35084]: Uniform3i64vARB (will be remapped) */
        "iip\0"
        "glUniform3i64vARB\0"
        "glUniform3i64vNV\0"
        "\0"
        /* _mesa_function_pool[35124]: Uniform4i64vARB (will be remapped) */
        "iip\0"
        "glUniform4i64vARB\0"
        "glUniform4i64vNV\0"
        "\0"
        /* _mesa_function_pool[35164]: Uniform1ui64ARB (will be remapped) */
        "ii\0"
        "glUniform1ui64ARB\0"
        "glUniform1ui64NV\0"
        "\0"
        /* _mesa_function_pool[35203]: Uniform2ui64ARB (will be remapped) */
        "iii\0"
        "glUniform2ui64ARB\0"
        "glUniform2ui64NV\0"
        "\0"
        /* _mesa_function_pool[35243]: Uniform3ui64ARB (will be remapped) */
        "iiii\0"
        "glUniform3ui64ARB\0"
        "glUniform3ui64NV\0"
        "\0"
        /* _mesa_function_pool[35284]: Uniform4ui64ARB (will be remapped) */
        "iiiii\0"
        "glUniform4ui64ARB\0"
        "glUniform4ui64NV\0"
        "\0"
        /* _mesa_function_pool[35326]: Uniform1ui64vARB (will be remapped) */
        "iip\0"
        "glUniform1ui64vARB\0"
        "glUniform1ui64vNV\0"
        "\0"
        /* _mesa_function_pool[35368]: Uniform2ui64vARB (will be remapped) */
        "iip\0"
        "glUniform2ui64vARB\0"
        "glUniform2ui64vNV\0"
        "\0"
        /* _mesa_function_pool[35410]: Uniform3ui64vARB (will be remapped) */
        "iip\0"
        "glUniform3ui64vARB\0"
        "glUniform3ui64vNV\0"
        "\0"
        /* _mesa_function_pool[35452]: Uniform4ui64vARB (will be remapped) */
        "iip\0"
        "glUniform4ui64vARB\0"
        "glUniform4ui64vNV\0"
        "\0"
        /* _mesa_function_pool[35494]: GetUniformi64vARB (will be remapped) */
        "iip\0"
        "glGetUniformi64vARB\0"
        "glGetUniformi64vNV\0"
        "\0"
        /* _mesa_function_pool[35538]: GetUniformui64vARB (will be remapped) */
        "iip\0"
        "glGetUniformui64vARB\0"
        "glGetUniformui64vNV\0"
        "\0"
        /* _mesa_function_pool[35584]: GetnUniformi64vARB (will be remapped) */
        "iiip\0"
        "glGetnUniformi64vARB\0"
        "\0"
        /* _mesa_function_pool[35611]: GetnUniformui64vARB (will be remapped) */
        "iiip\0"
        "glGetnUniformui64vARB\0"
        "\0"
        /* _mesa_function_pool[35639]: ProgramUniform1i64ARB (will be remapped) */
        "iii\0"
        "glProgramUniform1i64ARB\0"
        "glProgramUniform1i64NV\0"
        "\0"
        /* _mesa_function_pool[35691]: ProgramUniform2i64ARB (will be remapped) */
        "iiii\0"
        "glProgramUniform2i64ARB\0"
        "glProgramUniform2i64NV\0"
        "\0"
        /* _mesa_function_pool[35744]: ProgramUniform3i64ARB (will be remapped) */
        "iiiii\0"
        "glProgramUniform3i64ARB\0"
        "glProgramUniform3i64NV\0"
        "\0"
        /* _mesa_function_pool[35798]: ProgramUniform4i64ARB (will be remapped) */
        "iiiiii\0"
        "glProgramUniform4i64ARB\0"
        "glProgramUniform4i64NV\0"
        "\0"
        /* _mesa_function_pool[35853]: ProgramUniform1i64vARB (will be remapped) */
        "iiip\0"
        "glProgramUniform1i64vARB\0"
        "glProgramUniform1i64vNV\0"
        "\0"
        /* _mesa_function_pool[35908]: ProgramUniform2i64vARB (will be remapped) */
        "iiip\0"
        "glProgramUniform2i64vARB\0"
        "glProgramUniform2i64vNV\0"
        "\0"
        /* _mesa_function_pool[35963]: ProgramUniform3i64vARB (will be remapped) */
        "iiip\0"
        "glProgramUniform3i64vARB\0"
        "glProgramUniform3i64vNV\0"
        "\0"
        /* _mesa_function_pool[36018]: ProgramUniform4i64vARB (will be remapped) */
        "iiip\0"
        "glProgramUniform4i64vARB\0"
        "glProgramUniform4i64vNV\0"
        "\0"
        /* _mesa_function_pool[36073]: ProgramUniform1ui64ARB (will be remapped) */
        "iii\0"
        "glProgramUniform1ui64ARB\0"
        "glProgramUniform1ui64NV\0"
        "\0"
        /* _mesa_function_pool[36127]: ProgramUniform2ui64ARB (will be remapped) */
        "iiii\0"
        "glProgramUniform2ui64ARB\0"
        "glProgramUniform2ui64NV\0"
        "\0"
        /* _mesa_function_pool[36182]: ProgramUniform3ui64ARB (will be remapped) */
        "iiiii\0"
        "glProgramUniform3ui64ARB\0"
        "glProgramUniform3ui64NV\0"
        "\0"
        /* _mesa_function_pool[36238]: ProgramUniform4ui64ARB (will be remapped) */
        "iiiiii\0"
        "glProgramUniform4ui64ARB\0"
        "glProgramUniform4ui64NV\0"
        "\0"
        /* _mesa_function_pool[36295]: ProgramUniform1ui64vARB (will be remapped) */
        "iiip\0"
        "glProgramUniform1ui64vARB\0"
        "glProgramUniform1ui64vNV\0"
        "\0"
        /* _mesa_function_pool[36352]: ProgramUniform2ui64vARB (will be remapped) */
        "iiip\0"
        "glProgramUniform2ui64vARB\0"
        "glProgramUniform2ui64vNV\0"
        "\0"
        /* _mesa_function_pool[36409]: ProgramUniform3ui64vARB (will be remapped) */
        "iiip\0"
        "glProgramUniform3ui64vARB\0"
        "glProgramUniform3ui64vNV\0"
        "\0"
        /* _mesa_function_pool[36466]: ProgramUniform4ui64vARB (will be remapped) */
        "iiip\0"
        "glProgramUniform4ui64vARB\0"
        "glProgramUniform4ui64vNV\0"
        "\0"
        /* _mesa_function_pool[36523]: MaxShaderCompilerThreadsKHR (will be remapped) */
        "i\0"
        "glMaxShaderCompilerThreadsKHR\0"
        "glMaxShaderCompilerThreadsARB\0"
        "\0"
        /* _mesa_function_pool[36586]: SpecializeShaderARB (will be remapped) */
        "ipipp\0"
        "glSpecializeShaderARB\0"
        "glSpecializeShader\0"
        "\0"
        /* _mesa_function_pool[36634]: GetTexFilterFuncSGIS (dynamic) */
        "iip\0"
        "glGetTexFilterFuncSGIS\0"
        "\0"
        /* _mesa_function_pool[36662]: TexFilterFuncSGIS (dynamic) */
        "iiip\0"
        "glTexFilterFuncSGIS\0"
        "\0"
        /* _mesa_function_pool[36688]: PixelTexGenParameteriSGIS (dynamic) */
        "ii\0"
        "glPixelTexGenParameteriSGIS\0"
        "\0"
        /* _mesa_function_pool[36720]: PixelTexGenParameterivSGIS (dynamic) */
        "ip\0"
        "glPixelTexGenParameterivSGIS\0"
        "\0"
        /* _mesa_function_pool[36753]: PixelTexGenParameterfSGIS (dynamic) */
        "if\0"
        "glPixelTexGenParameterfSGIS\0"
        "\0"
        /* _mesa_function_pool[36785]: PixelTexGenParameterfvSGIS (dynamic) */
        "ip\0"
        "glPixelTexGenParameterfvSGIS\0"
        "\0"
        /* _mesa_function_pool[36818]: GetPixelTexGenParameterivSGIS (dynamic) */
        "ip\0"
        "glGetPixelTexGenParameterivSGIS\0"
        "\0"
        /* _mesa_function_pool[36854]: GetPixelTexGenParameterfvSGIS (dynamic) */
        "ip\0"
        "glGetPixelTexGenParameterfvSGIS\0"
        "\0"
        /* _mesa_function_pool[36890]: TexImage4DSGIS (dynamic) */
        "iiiiiiiiiip\0"
        "glTexImage4DSGIS\0"
        "\0"
        /* _mesa_function_pool[36920]: TexSubImage4DSGIS (dynamic) */
        "iiiiiiiiiiiip\0"
        "glTexSubImage4DSGIS\0"
        "\0"
        /* _mesa_function_pool[36955]: DetailTexFuncSGIS (dynamic) */
        "iip\0"
        "glDetailTexFuncSGIS\0"
        "\0"
        /* _mesa_function_pool[36980]: GetDetailTexFuncSGIS (dynamic) */
        "ip\0"
        "glGetDetailTexFuncSGIS\0"
        "\0"
        /* _mesa_function_pool[37007]: SharpenTexFuncSGIS (dynamic) */
        "iip\0"
        "glSharpenTexFuncSGIS\0"
        "\0"
        /* _mesa_function_pool[37033]: GetSharpenTexFuncSGIS (dynamic) */
        "ip\0"
        "glGetSharpenTexFuncSGIS\0"
        "\0"
        /* _mesa_function_pool[37061]: SampleMaskSGIS (will be remapped) */
        "fi\0"
        "glSampleMaskSGIS\0"
        "glSampleMaskEXT\0"
        "\0"
        /* _mesa_function_pool[37098]: SamplePatternSGIS (will be remapped) */
        "i\0"
        "glSamplePatternSGIS\0"
        "glSamplePatternEXT\0"
        "\0"
        /* _mesa_function_pool[37140]: ColorPointerEXT (will be remapped) */
        "iiiip\0"
        "glColorPointerEXT\0"
        "\0"
        /* _mesa_function_pool[37165]: EdgeFlagPointerEXT (will be remapped) */
        "iip\0"
        "glEdgeFlagPointerEXT\0"
        "\0"
        /* _mesa_function_pool[37191]: IndexPointerEXT (will be remapped) */
        "iiip\0"
        "glIndexPointerEXT\0"
        "\0"
        /* _mesa_function_pool[37215]: NormalPointerEXT (will be remapped) */
        "iiip\0"
        "glNormalPointerEXT\0"
        "\0"
        /* _mesa_function_pool[37240]: TexCoordPointerEXT (will be remapped) */
        "iiiip\0"
        "glTexCoordPointerEXT\0"
        "\0"
        /* _mesa_function_pool[37268]: VertexPointerEXT (will be remapped) */
        "iiiip\0"
        "glVertexPointerEXT\0"
        "\0"
        /* _mesa_function_pool[37294]: SpriteParameterfSGIX (dynamic) */
        "if\0"
        "glSpriteParameterfSGIX\0"
        "\0"
        /* _mesa_function_pool[37321]: SpriteParameterfvSGIX (dynamic) */
        "ip\0"
        "glSpriteParameterfvSGIX\0"
        "\0"
        /* _mesa_function_pool[37349]: SpriteParameteriSGIX (dynamic) */
        "ii\0"
        "glSpriteParameteriSGIX\0"
        "\0"
        /* _mesa_function_pool[37376]: SpriteParameterivSGIX (dynamic) */
        "ip\0"
        "glSpriteParameterivSGIX\0"
        "\0"
        /* _mesa_function_pool[37404]: GetInstrumentsSGIX (dynamic) */
        "\0"
        "glGetInstrumentsSGIX\0"
        "\0"
        /* _mesa_function_pool[37427]: InstrumentsBufferSGIX (dynamic) */
        "ip\0"
        "glInstrumentsBufferSGIX\0"
        "\0"
        /* _mesa_function_pool[37455]: PollInstrumentsSGIX (dynamic) */
        "p\0"
        "glPollInstrumentsSGIX\0"
        "\0"
        /* _mesa_function_pool[37480]: ReadInstrumentsSGIX (dynamic) */
        "i\0"
        "glReadInstrumentsSGIX\0"
        "\0"
        /* _mesa_function_pool[37505]: StartInstrumentsSGIX (dynamic) */
        "\0"
        "glStartInstrumentsSGIX\0"
        "\0"
        /* _mesa_function_pool[37530]: StopInstrumentsSGIX (dynamic) */
        "i\0"
        "glStopInstrumentsSGIX\0"
        "\0"
        /* _mesa_function_pool[37555]: FrameZoomSGIX (dynamic) */
        "i\0"
        "glFrameZoomSGIX\0"
        "\0"
        /* _mesa_function_pool[37574]: TagSampleBufferSGIX (dynamic) */
        "\0"
        "glTagSampleBufferSGIX\0"
        "\0"
        /* _mesa_function_pool[37598]: ReferencePlaneSGIX (dynamic) */
        "p\0"
        "glReferencePlaneSGIX\0"
        "\0"
        /* _mesa_function_pool[37622]: FlushRasterSGIX (dynamic) */
        "\0"
        "glFlushRasterSGIX\0"
        "\0"
        /* _mesa_function_pool[37642]: FogFuncSGIS (dynamic) */
        "ip\0"
        "glFogFuncSGIS\0"
        "\0"
        /* _mesa_function_pool[37660]: GetFogFuncSGIS (dynamic) */
        "p\0"
        "glGetFogFuncSGIS\0"
        "\0"
        /* _mesa_function_pool[37680]: ImageTransformParameteriHP (dynamic) */
        "iii\0"
        "glImageTransformParameteriHP\0"
        "\0"
        /* _mesa_function_pool[37714]: ImageTransformParameterfHP (dynamic) */
        "iif\0"
        "glImageTransformParameterfHP\0"
        "\0"
        /* _mesa_function_pool[37748]: ImageTransformParameterivHP (dynamic) */
        "iip\0"
        "glImageTransformParameterivHP\0"
        "\0"
        /* _mesa_function_pool[37783]: ImageTransformParameterfvHP (dynamic) */
        "iip\0"
        "glImageTransformParameterfvHP\0"
        "\0"
        /* _mesa_function_pool[37818]: GetImageTransformParameterivHP (dynamic) */
        "iip\0"
        "glGetImageTransformParameterivHP\0"
        "\0"
        /* _mesa_function_pool[37856]: GetImageTransformParameterfvHP (dynamic) */
        "iip\0"
        "glGetImageTransformParameterfvHP\0"
        "\0"
        /* _mesa_function_pool[37894]: HintPGI (dynamic) */
        "ii\0"
        "glHintPGI\0"
        "\0"
        /* _mesa_function_pool[37908]: GetListParameterfvSGIX (dynamic) */
        "iip\0"
        "glGetListParameterfvSGIX\0"
        "\0"
        /* _mesa_function_pool[37938]: GetListParameterivSGIX (dynamic) */
        "iip\0"
        "glGetListParameterivSGIX\0"
        "\0"
        /* _mesa_function_pool[37968]: ListParameterfSGIX (dynamic) */
        "iif\0"
        "glListParameterfSGIX\0"
        "\0"
        /* _mesa_function_pool[37994]: ListParameterfvSGIX (dynamic) */
        "iip\0"
        "glListParameterfvSGIX\0"
        "\0"
        /* _mesa_function_pool[38021]: ListParameteriSGIX (dynamic) */
        "iii\0"
        "glListParameteriSGIX\0"
        "\0"
        /* _mesa_function_pool[38047]: ListParameterivSGIX (dynamic) */
        "iip\0"
        "glListParameterivSGIX\0"
        "\0"
        /* _mesa_function_pool[38074]: IndexMaterialEXT (dynamic) */
        "ii\0"
        "glIndexMaterialEXT\0"
        "\0"
        /* _mesa_function_pool[38097]: IndexFuncEXT (dynamic) */
        "if\0"
        "glIndexFuncEXT\0"
        "\0"
        /* _mesa_function_pool[38116]: LockArraysEXT (will be remapped) */
        "ii\0"
        "glLockArraysEXT\0"
        "\0"
        /* _mesa_function_pool[38136]: UnlockArraysEXT (will be remapped) */
        "\0"
        "glUnlockArraysEXT\0"
        "\0"
        /* _mesa_function_pool[38156]: CullParameterdvEXT (dynamic) */
        "ip\0"
        "glCullParameterdvEXT\0"
        "\0"
        /* _mesa_function_pool[38181]: CullParameterfvEXT (dynamic) */
        "ip\0"
        "glCullParameterfvEXT\0"
        "\0"
        /* _mesa_function_pool[38206]: ViewportArrayv (will be remapped) */
        "iip\0"
        "glViewportArrayv\0"
        "\0"
        /* _mesa_function_pool[38228]: ViewportIndexedf (will be remapped) */
        "iffff\0"
        "glViewportIndexedf\0"
        "\0"
        /* _mesa_function_pool[38254]: ViewportIndexedfv (will be remapped) */
        "ip\0"
        "glViewportIndexedfv\0"
        "\0"
        /* _mesa_function_pool[38278]: ScissorArrayv (will be remapped) */
        "iip\0"
        "glScissorArrayv\0"
        "\0"
        /* _mesa_function_pool[38299]: ScissorIndexed (will be remapped) */
        "iiiii\0"
        "glScissorIndexed\0"
        "\0"
        /* _mesa_function_pool[38323]: ScissorIndexedv (will be remapped) */
        "ip\0"
        "glScissorIndexedv\0"
        "\0"
        /* _mesa_function_pool[38345]: DepthRangeArrayv (will be remapped) */
        "iip\0"
        "glDepthRangeArrayv\0"
        "\0"
        /* _mesa_function_pool[38369]: DepthRangeIndexed (will be remapped) */
        "idd\0"
        "glDepthRangeIndexed\0"
        "\0"
        /* _mesa_function_pool[38394]: GetFloati_v (will be remapped) */
        "iip\0"
        "glGetFloati_v\0"
        "glGetFloatIndexedvEXT\0"
        "glGetFloati_vEXT\0"
        "\0"
        /* _mesa_function_pool[38452]: GetDoublei_v (will be remapped) */
        "iip\0"
        "glGetDoublei_v\0"
        "glGetDoubleIndexedvEXT\0"
        "glGetDoublei_vEXT\0"
        "\0"
        /* _mesa_function_pool[38513]: FragmentColorMaterialSGIX (dynamic) */
        "ii\0"
        "glFragmentColorMaterialSGIX\0"
        "\0"
        /* _mesa_function_pool[38545]: FragmentLightfSGIX (dynamic) */
        "iif\0"
        "glFragmentLightfSGIX\0"
        "\0"
        /* _mesa_function_pool[38571]: FragmentLightfvSGIX (dynamic) */
        "iip\0"
        "glFragmentLightfvSGIX\0"
        "\0"
        /* _mesa_function_pool[38598]: FragmentLightiSGIX (dynamic) */
        "iii\0"
        "glFragmentLightiSGIX\0"
        "\0"
        /* _mesa_function_pool[38624]: FragmentLightivSGIX (dynamic) */
        "iip\0"
        "glFragmentLightivSGIX\0"
        "\0"
        /* _mesa_function_pool[38651]: FragmentLightModelfSGIX (dynamic) */
        "if\0"
        "glFragmentLightModelfSGIX\0"
        "\0"
        /* _mesa_function_pool[38681]: FragmentLightModelfvSGIX (dynamic) */
        "ip\0"
        "glFragmentLightModelfvSGIX\0"
        "\0"
        /* _mesa_function_pool[38712]: FragmentLightModeliSGIX (dynamic) */
        "ii\0"
        "glFragmentLightModeliSGIX\0"
        "\0"
        /* _mesa_function_pool[38742]: FragmentLightModelivSGIX (dynamic) */
        "ip\0"
        "glFragmentLightModelivSGIX\0"
        "\0"
        /* _mesa_function_pool[38773]: FragmentMaterialfSGIX (dynamic) */
        "iif\0"
        "glFragmentMaterialfSGIX\0"
        "\0"
        /* _mesa_function_pool[38802]: FragmentMaterialfvSGIX (dynamic) */
        "iip\0"
        "glFragmentMaterialfvSGIX\0"
        "\0"
        /* _mesa_function_pool[38832]: FragmentMaterialiSGIX (dynamic) */
        "iii\0"
        "glFragmentMaterialiSGIX\0"
        "\0"
        /* _mesa_function_pool[38861]: FragmentMaterialivSGIX (dynamic) */
        "iip\0"
        "glFragmentMaterialivSGIX\0"
        "\0"
        /* _mesa_function_pool[38891]: GetFragmentLightfvSGIX (dynamic) */
        "iip\0"
        "glGetFragmentLightfvSGIX\0"
        "\0"
        /* _mesa_function_pool[38921]: GetFragmentLightivSGIX (dynamic) */
        "iip\0"
        "glGetFragmentLightivSGIX\0"
        "\0"
        /* _mesa_function_pool[38951]: GetFragmentMaterialfvSGIX (dynamic) */
        "iip\0"
        "glGetFragmentMaterialfvSGIX\0"
        "\0"
        /* _mesa_function_pool[38984]: GetFragmentMaterialivSGIX (dynamic) */
        "iip\0"
        "glGetFragmentMaterialivSGIX\0"
        "\0"
        /* _mesa_function_pool[39017]: LightEnviSGIX (dynamic) */
        "ii\0"
        "glLightEnviSGIX\0"
        "\0"
        /* _mesa_function_pool[39037]: ApplyTextureEXT (dynamic) */
        "i\0"
        "glApplyTextureEXT\0"
        "\0"
        /* _mesa_function_pool[39058]: TextureLightEXT (dynamic) */
        "i\0"
        "glTextureLightEXT\0"
        "\0"
        /* _mesa_function_pool[39079]: TextureMaterialEXT (dynamic) */
        "ii\0"
        "glTextureMaterialEXT\0"
        "\0"
        /* _mesa_function_pool[39104]: AsyncMarkerSGIX (dynamic) */
        "i\0"
        "glAsyncMarkerSGIX\0"
        "\0"
        /* _mesa_function_pool[39125]: FinishAsyncSGIX (dynamic) */
        "p\0"
        "glFinishAsyncSGIX\0"
        "\0"
        /* _mesa_function_pool[39146]: PollAsyncSGIX (dynamic) */
        "p\0"
        "glPollAsyncSGIX\0"
        "\0"
        /* _mesa_function_pool[39165]: GenAsyncMarkersSGIX (dynamic) */
        "i\0"
        "glGenAsyncMarkersSGIX\0"
        "\0"
        /* _mesa_function_pool[39190]: DeleteAsyncMarkersSGIX (dynamic) */
        "ii\0"
        "glDeleteAsyncMarkersSGIX\0"
        "\0"
        /* _mesa_function_pool[39219]: IsAsyncMarkerSGIX (dynamic) */
        "i\0"
        "glIsAsyncMarkerSGIX\0"
        "\0"
        /* _mesa_function_pool[39242]: VertexPointervINTEL (dynamic) */
        "iip\0"
        "glVertexPointervINTEL\0"
        "\0"
        /* _mesa_function_pool[39269]: NormalPointervINTEL (dynamic) */
        "ip\0"
        "glNormalPointervINTEL\0"
        "\0"
        /* _mesa_function_pool[39295]: ColorPointervINTEL (dynamic) */
        "iip\0"
        "glColorPointervINTEL\0"
        "\0"
        /* _mesa_function_pool[39321]: TexCoordPointervINTEL (dynamic) */
        "iip\0"
        "glTexCoordPointervINTEL\0"
        "\0"
        /* _mesa_function_pool[39350]: PixelTransformParameteriEXT (dynamic) */
        "iii\0"
        "glPixelTransformParameteriEXT\0"
        "\0"
        /* _mesa_function_pool[39385]: PixelTransformParameterfEXT (dynamic) */
        "iif\0"
        "glPixelTransformParameterfEXT\0"
        "\0"
        /* _mesa_function_pool[39420]: PixelTransformParameterivEXT (dynamic) */
        "iip\0"
        "glPixelTransformParameterivEXT\0"
        "\0"
        /* _mesa_function_pool[39456]: PixelTransformParameterfvEXT (dynamic) */
        "iip\0"
        "glPixelTransformParameterfvEXT\0"
        "\0"
        /* _mesa_function_pool[39492]: TextureNormalEXT (dynamic) */
        "i\0"
        "glTextureNormalEXT\0"
        "\0"
        /* _mesa_function_pool[39514]: Tangent3bEXT (dynamic) */
        "iii\0"
        "glTangent3bEXT\0"
        "\0"
        /* _mesa_function_pool[39534]: Tangent3bvEXT (dynamic) */
        "p\0"
        "glTangent3bvEXT\0"
        "\0"
        /* _mesa_function_pool[39553]: Tangent3dEXT (dynamic) */
        "ddd\0"
        "glTangent3dEXT\0"
        "\0"
        /* _mesa_function_pool[39573]: Tangent3dvEXT (dynamic) */
        "p\0"
        "glTangent3dvEXT\0"
        "\0"
        /* _mesa_function_pool[39592]: Tangent3fEXT (dynamic) */
        "fff\0"
        "glTangent3fEXT\0"
        "\0"
        /* _mesa_function_pool[39612]: Tangent3fvEXT (dynamic) */
        "p\0"
        "glTangent3fvEXT\0"
        "\0"
        /* _mesa_function_pool[39631]: Tangent3iEXT (dynamic) */
        "iii\0"
        "glTangent3iEXT\0"
        "\0"
        /* _mesa_function_pool[39651]: Tangent3ivEXT (dynamic) */
        "p\0"
        "glTangent3ivEXT\0"
        "\0"
        /* _mesa_function_pool[39670]: Tangent3sEXT (dynamic) */
        "iii\0"
        "glTangent3sEXT\0"
        "\0"
        /* _mesa_function_pool[39690]: Tangent3svEXT (dynamic) */
        "p\0"
        "glTangent3svEXT\0"
        "\0"
        /* _mesa_function_pool[39709]: Binormal3bEXT (dynamic) */
        "iii\0"
        "glBinormal3bEXT\0"
        "\0"
        /* _mesa_function_pool[39730]: Binormal3bvEXT (dynamic) */
        "p\0"
        "glBinormal3bvEXT\0"
        "\0"
        /* _mesa_function_pool[39750]: Binormal3dEXT (dynamic) */
        "ddd\0"
        "glBinormal3dEXT\0"
        "\0"
        /* _mesa_function_pool[39771]: Binormal3dvEXT (dynamic) */
        "p\0"
        "glBinormal3dvEXT\0"
        "\0"
        /* _mesa_function_pool[39791]: Binormal3fEXT (dynamic) */
        "fff\0"
        "glBinormal3fEXT\0"
        "\0"
        /* _mesa_function_pool[39812]: Binormal3fvEXT (dynamic) */
        "p\0"
        "glBinormal3fvEXT\0"
        "\0"
        /* _mesa_function_pool[39832]: Binormal3iEXT (dynamic) */
        "iii\0"
        "glBinormal3iEXT\0"
        "\0"
        /* _mesa_function_pool[39853]: Binormal3ivEXT (dynamic) */
        "p\0"
        "glBinormal3ivEXT\0"
        "\0"
        /* _mesa_function_pool[39873]: Binormal3sEXT (dynamic) */
        "iii\0"
        "glBinormal3sEXT\0"
        "\0"
        /* _mesa_function_pool[39894]: Binormal3svEXT (dynamic) */
        "p\0"
        "glBinormal3svEXT\0"
        "\0"
        /* _mesa_function_pool[39914]: TangentPointerEXT (dynamic) */
        "iip\0"
        "glTangentPointerEXT\0"
        "\0"
        /* _mesa_function_pool[39939]: BinormalPointerEXT (dynamic) */
        "iip\0"
        "glBinormalPointerEXT\0"
        "\0"
        /* _mesa_function_pool[39965]: PixelTexGenSGIX (dynamic) */
        "i\0"
        "glPixelTexGenSGIX\0"
        "\0"
        /* _mesa_function_pool[39986]: FinishTextureSUNX (dynamic) */
        "\0"
        "glFinishTextureSUNX\0"
        "\0"
        /* _mesa_function_pool[40008]: GlobalAlphaFactorbSUN (dynamic) */
        "i\0"
        "glGlobalAlphaFactorbSUN\0"
        "\0"
        /* _mesa_function_pool[40035]: GlobalAlphaFactorsSUN (dynamic) */
        "i\0"
        "glGlobalAlphaFactorsSUN\0"
        "\0"
        /* _mesa_function_pool[40062]: GlobalAlphaFactoriSUN (dynamic) */
        "i\0"
        "glGlobalAlphaFactoriSUN\0"
        "\0"
        /* _mesa_function_pool[40089]: GlobalAlphaFactorfSUN (dynamic) */
        "f\0"
        "glGlobalAlphaFactorfSUN\0"
        "\0"
        /* _mesa_function_pool[40116]: GlobalAlphaFactordSUN (dynamic) */
        "d\0"
        "glGlobalAlphaFactordSUN\0"
        "\0"
        /* _mesa_function_pool[40143]: GlobalAlphaFactorubSUN (dynamic) */
        "i\0"
        "glGlobalAlphaFactorubSUN\0"
        "\0"
        /* _mesa_function_pool[40171]: GlobalAlphaFactorusSUN (dynamic) */
        "i\0"
        "glGlobalAlphaFactorusSUN\0"
        "\0"
        /* _mesa_function_pool[40199]: GlobalAlphaFactoruiSUN (dynamic) */
        "i\0"
        "glGlobalAlphaFactoruiSUN\0"
        "\0"
        /* _mesa_function_pool[40227]: ReplacementCodeuiSUN (dynamic) */
        "i\0"
        "glReplacementCodeuiSUN\0"
        "\0"
        /* _mesa_function_pool[40253]: ReplacementCodeusSUN (dynamic) */
        "i\0"
        "glReplacementCodeusSUN\0"
        "\0"
        /* _mesa_function_pool[40279]: ReplacementCodeubSUN (dynamic) */
        "i\0"
        "glReplacementCodeubSUN\0"
        "\0"
        /* _mesa_function_pool[40305]: ReplacementCodeuivSUN (dynamic) */
        "p\0"
        "glReplacementCodeuivSUN\0"
        "\0"
        /* _mesa_function_pool[40332]: ReplacementCodeusvSUN (dynamic) */
        "p\0"
        "glReplacementCodeusvSUN\0"
        "\0"
        /* _mesa_function_pool[40359]: ReplacementCodeubvSUN (dynamic) */
        "p\0"
        "glReplacementCodeubvSUN\0"
        "\0"
        /* _mesa_function_pool[40386]: ReplacementCodePointerSUN (dynamic) */
        "iip\0"
        "glReplacementCodePointerSUN\0"
        "\0"
        /* _mesa_function_pool[40419]: Color4ubVertex2fSUN (dynamic) */
        "iiiiff\0"
        "glColor4ubVertex2fSUN\0"
        "\0"
        /* _mesa_function_pool[40449]: Color4ubVertex2fvSUN (dynamic) */
        "pp\0"
        "glColor4ubVertex2fvSUN\0"
        "\0"
        /* _mesa_function_pool[40476]: Color4ubVertex3fSUN (dynamic) */
        "iiiifff\0"
        "glColor4ubVertex3fSUN\0"
        "\0"
        /* _mesa_function_pool[40507]: Color4ubVertex3fvSUN (dynamic) */
        "pp\0"
        "glColor4ubVertex3fvSUN\0"
        "\0"
        /* _mesa_function_pool[40534]: Color3fVertex3fSUN (dynamic) */
        "ffffff\0"
        "glColor3fVertex3fSUN\0"
        "\0"
        /* _mesa_function_pool[40563]: Color3fVertex3fvSUN (dynamic) */
        "pp\0"
        "glColor3fVertex3fvSUN\0"
        "\0"
        /* _mesa_function_pool[40589]: Normal3fVertex3fSUN (dynamic) */
        "ffffff\0"
        "glNormal3fVertex3fSUN\0"
        "\0"
        /* _mesa_function_pool[40619]: Normal3fVertex3fvSUN (dynamic) */
        "pp\0"
        "glNormal3fVertex3fvSUN\0"
        "\0"
        /* _mesa_function_pool[40646]: Color4fNormal3fVertex3fSUN (dynamic) */
        "ffffffffff\0"
        "glColor4fNormal3fVertex3fSUN\0"
        "\0"
        /* _mesa_function_pool[40687]: Color4fNormal3fVertex3fvSUN (dynamic) */
        "ppp\0"
        "glColor4fNormal3fVertex3fvSUN\0"
        "\0"
        /* _mesa_function_pool[40722]: TexCoord2fVertex3fSUN (dynamic) */
        "fffff\0"
        "glTexCoord2fVertex3fSUN\0"
        "\0"
        /* _mesa_function_pool[40753]: TexCoord2fVertex3fvSUN (dynamic) */
        "pp\0"
        "glTexCoord2fVertex3fvSUN\0"
        "\0"
        /* _mesa_function_pool[40782]: TexCoord4fVertex4fSUN (dynamic) */
        "ffffffff\0"
        "glTexCoord4fVertex4fSUN\0"
        "\0"
        /* _mesa_function_pool[40816]: TexCoord4fVertex4fvSUN (dynamic) */
        "pp\0"
        "glTexCoord4fVertex4fvSUN\0"
        "\0"
        /* _mesa_function_pool[40845]: TexCoord2fColor4ubVertex3fSUN (dynamic) */
        "ffiiiifff\0"
        "glTexCoord2fColor4ubVertex3fSUN\0"
        "\0"
        /* _mesa_function_pool[40888]: TexCoord2fColor4ubVertex3fvSUN (dynamic) */
        "ppp\0"
        "glTexCoord2fColor4ubVertex3fvSUN\0"
        "\0"
        /* _mesa_function_pool[40926]: TexCoord2fColor3fVertex3fSUN (dynamic) */
        "ffffffff\0"
        "glTexCoord2fColor3fVertex3fSUN\0"
        "\0"
        /* _mesa_function_pool[40967]: TexCoord2fColor3fVertex3fvSUN (dynamic) */
        "ppp\0"
        "glTexCoord2fColor3fVertex3fvSUN\0"
        "\0"
        /* _mesa_function_pool[41004]: TexCoord2fNormal3fVertex3fSUN (dynamic) */
        "ffffffff\0"
        "glTexCoord2fNormal3fVertex3fSUN\0"
        "\0"
        /* _mesa_function_pool[41046]: TexCoord2fNormal3fVertex3fvSUN (dynamic) */
        "ppp\0"
        "glTexCoord2fNormal3fVertex3fvSUN\0"
        "\0"
        /* _mesa_function_pool[41084]: TexCoord2fColor4fNormal3fVertex3fSUN (dynamic) */
        "ffffffffffff\0"
        "glTexCoord2fColor4fNormal3fVertex3fSUN\0"
        "\0"
        /* _mesa_function_pool[41137]: TexCoord2fColor4fNormal3fVertex3fvSUN (dynamic) */
        "pppp\0"
        "glTexCoord2fColor4fNormal3fVertex3fvSUN\0"
        "\0"
        /* _mesa_function_pool[41183]: TexCoord4fColor4fNormal3fVertex4fSUN (dynamic) */
        "fffffffffffffff\0"
        "glTexCoord4fColor4fNormal3fVertex4fSUN\0"
        "\0"
        /* _mesa_function_pool[41239]: TexCoord4fColor4fNormal3fVertex4fvSUN (dynamic) */
        "pppp\0"
        "glTexCoord4fColor4fNormal3fVertex4fvSUN\0"
        "\0"
        /* _mesa_function_pool[41285]: ReplacementCodeuiVertex3fSUN (dynamic) */
        "ifff\0"
        "glReplacementCodeuiVertex3fSUN\0"
        "\0"
        /* _mesa_function_pool[41322]: ReplacementCodeuiVertex3fvSUN (dynamic) */
        "pp\0"
        "glReplacementCodeuiVertex3fvSUN\0"
        "\0"
        /* _mesa_function_pool[41358]: ReplacementCodeuiColor4ubVertex3fSUN (dynamic) */
        "iiiiifff\0"
        "glReplacementCodeuiColor4ubVertex3fSUN\0"
        "\0"
        /* _mesa_function_pool[41407]: ReplacementCodeuiColor4ubVertex3fvSUN (dynamic) */
        "ppp\0"
        "glReplacementCodeuiColor4ubVertex3fvSUN\0"
        "\0"
        /* _mesa_function_pool[41452]: ReplacementCodeuiColor3fVertex3fSUN (dynamic) */
        "iffffff\0"
        "glReplacementCodeuiColor3fVertex3fSUN\0"
        "\0"
        /* _mesa_function_pool[41499]: ReplacementCodeuiColor3fVertex3fvSUN (dynamic) */
        "ppp\0"
        "glReplacementCodeuiColor3fVertex3fvSUN\0"
        "\0"
        /* _mesa_function_pool[41543]: ReplacementCodeuiNormal3fVertex3fSUN (dynamic) */
        "iffffff\0"
        "glReplacementCodeuiNormal3fVertex3fSUN\0"
        "\0"
        /* _mesa_function_pool[41591]: ReplacementCodeuiNormal3fVertex3fvSUN (dynamic) */
        "ppp\0"
        "glReplacementCodeuiNormal3fVertex3fvSUN\0"
        "\0"
        /* _mesa_function_pool[41636]: ReplacementCodeuiColor4fNormal3fVertex3fSUN (dynamic) */
        "iffffffffff\0"
        "glReplacementCodeuiColor4fNormal3fVertex3fSUN\0"
        "\0"
        /* _mesa_function_pool[41695]: ReplacementCodeuiColor4fNormal3fVertex3fvSUN (dynamic) */
        "pppp\0"
        "glReplacementCodeuiColor4fNormal3fVertex3fvSUN\0"
        "\0"
        /* _mesa_function_pool[41748]: ReplacementCodeuiTexCoord2fVertex3fSUN (dynamic) */
        "ifffff\0"
        "glReplacementCodeuiTexCoord2fVertex3fSUN\0"
        "\0"
        /* _mesa_function_pool[41797]: ReplacementCodeuiTexCoord2fVertex3fvSUN (dynamic) */
        "ppp\0"
        "glReplacementCodeuiTexCoord2fVertex3fvSUN\0"
        "\0"
        /* _mesa_function_pool[41844]: ReplacementCodeuiTexCoord2fNormal3fVertex3fSUN (dynamic) */
        "iffffffff\0"
        "glReplacementCodeuiTexCoord2fNormal3fVertex3fSUN\0"
        "\0"
        /* _mesa_function_pool[41904]: ReplacementCodeuiTexCoord2fNormal3fVertex3fvSUN (dynamic) */
        "pppp\0"
        "glReplacementCodeuiTexCoord2fNormal3fVertex3fvSUN\0"
        "\0"
        /* _mesa_function_pool[41960]: ReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fSUN (dynamic) */
        "iffffffffffff\0"
        "glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fSUN\0"
        "\0"
        /* _mesa_function_pool[42031]: ReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fvSUN (dynamic) */
        "ppppp\0"
        "glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fvSUN\0"
        "\0"
        /* _mesa_function_pool[42095]: FramebufferSampleLocationsfvARB (will be remapped) */
        "iiip\0"
        "glFramebufferSampleLocationsfvARB\0"
        "glFramebufferSampleLocationsfvNV\0"
        "\0"
        /* _mesa_function_pool[42168]: NamedFramebufferSampleLocationsfvARB (will be remapped) */
        "iiip\0"
        "glNamedFramebufferSampleLocationsfvARB\0"
        "glNamedFramebufferSampleLocationsfvNV\0"
        "\0"
        /* _mesa_function_pool[42251]: EvaluateDepthValuesARB (will be remapped) */
        "\0"
        "glEvaluateDepthValuesARB\0"
        "glResolveDepthValuesNV\0"
        "\0"
        /* _mesa_function_pool[42301]: VertexWeightfEXT (dynamic) */
        "f\0"
        "glVertexWeightfEXT\0"
        "\0"
        /* _mesa_function_pool[42323]: VertexWeightfvEXT (dynamic) */
        "p\0"
        "glVertexWeightfvEXT\0"
        "\0"
        /* _mesa_function_pool[42346]: VertexWeightPointerEXT (dynamic) */
        "iiip\0"
        "glVertexWeightPointerEXT\0"
        "\0"
        /* _mesa_function_pool[42377]: FlushVertexArrayRangeNV (dynamic) */
        "\0"
        "glFlushVertexArrayRangeNV\0"
        "\0"
        /* _mesa_function_pool[42405]: VertexArrayRangeNV (dynamic) */
        "ip\0"
        "glVertexArrayRangeNV\0"
        "\0"
        /* _mesa_function_pool[42430]: CombinerParameterfvNV (dynamic) */
        "ip\0"
        "glCombinerParameterfvNV\0"
        "\0"
        /* _mesa_function_pool[42458]: CombinerParameterfNV (dynamic) */
        "if\0"
        "glCombinerParameterfNV\0"
        "\0"
        /* _mesa_function_pool[42485]: CombinerParameterivNV (dynamic) */
        "ip\0"
        "glCombinerParameterivNV\0"
        "\0"
        /* _mesa_function_pool[42513]: CombinerParameteriNV (dynamic) */
        "ii\0"
        "glCombinerParameteriNV\0"
        "\0"
        /* _mesa_function_pool[42540]: CombinerInputNV (dynamic) */
        "iiiiii\0"
        "glCombinerInputNV\0"
        "\0"
        /* _mesa_function_pool[42566]: CombinerOutputNV (dynamic) */
        "iiiiiiiiii\0"
        "glCombinerOutputNV\0"
        "\0"
        /* _mesa_function_pool[42597]: FinalCombinerInputNV (dynamic) */
        "iiii\0"
        "glFinalCombinerInputNV\0"
        "\0"
        /* _mesa_function_pool[42626]: GetCombinerInputParameterfvNV (dynamic) */
        "iiiip\0"
        "glGetCombinerInputParameterfvNV\0"
        "\0"
        /* _mesa_function_pool[42665]: GetCombinerInputParameterivNV (dynamic) */
        "iiiip\0"
        "glGetCombinerInputParameterivNV\0"
        "\0"
        /* _mesa_function_pool[42704]: GetCombinerOutputParameterfvNV (dynamic) */
        "iiip\0"
        "glGetCombinerOutputParameterfvNV\0"
        "\0"
        /* _mesa_function_pool[42743]: GetCombinerOutputParameterivNV (dynamic) */
        "iiip\0"
        "glGetCombinerOutputParameterivNV\0"
        "\0"
        /* _mesa_function_pool[42782]: GetFinalCombinerInputParameterfvNV (dynamic) */
        "iip\0"
        "glGetFinalCombinerInputParameterfvNV\0"
        "\0"
        /* _mesa_function_pool[42824]: GetFinalCombinerInputParameterivNV (dynamic) */
        "iip\0"
        "glGetFinalCombinerInputParameterivNV\0"
        "\0"
        /* _mesa_function_pool[42866]: ResizeBuffersMESA (will be remapped) */
        "\0"
        "glResizeBuffersMESA\0"
        "\0"
        /* _mesa_function_pool[42888]: WindowPos4dMESA (will be remapped) */
        "dddd\0"
        "glWindowPos4dMESA\0"
        "\0"
        /* _mesa_function_pool[42912]: WindowPos4dvMESA (will be remapped) */
        "p\0"
        "glWindowPos4dvMESA\0"
        "\0"
        /* _mesa_function_pool[42934]: WindowPos4fMESA (will be remapped) */
        "ffff\0"
        "glWindowPos4fMESA\0"
        "\0"
        /* _mesa_function_pool[42958]: WindowPos4fvMESA (will be remapped) */
        "p\0"
        "glWindowPos4fvMESA\0"
        "\0"
        /* _mesa_function_pool[42980]: WindowPos4iMESA (will be remapped) */
        "iiii\0"
        "glWindowPos4iMESA\0"
        "\0"
        /* _mesa_function_pool[43004]: WindowPos4ivMESA (will be remapped) */
        "p\0"
        "glWindowPos4ivMESA\0"
        "\0"
        /* _mesa_function_pool[43026]: WindowPos4sMESA (will be remapped) */
        "iiii\0"
        "glWindowPos4sMESA\0"
        "\0"
        /* _mesa_function_pool[43050]: WindowPos4svMESA (will be remapped) */
        "p\0"
        "glWindowPos4svMESA\0"
        "\0"
        /* _mesa_function_pool[43072]: MultiModeDrawArraysIBM (will be remapped) */
        "pppii\0"
        "glMultiModeDrawArraysIBM\0"
        "\0"
        /* _mesa_function_pool[43104]: MultiModeDrawElementsIBM (will be remapped) */
        "ppipii\0"
        "glMultiModeDrawElementsIBM\0"
        "\0"
        /* _mesa_function_pool[43139]: ColorPointerListIBM (dynamic) */
        "iiipi\0"
        "glColorPointerListIBM\0"
        "\0"
        /* _mesa_function_pool[43168]: SecondaryColorPointerListIBM (dynamic) */
        "iiipi\0"
        "glSecondaryColorPointerListIBM\0"
        "\0"
        /* _mesa_function_pool[43206]: EdgeFlagPointerListIBM (dynamic) */
        "ipi\0"
        "glEdgeFlagPointerListIBM\0"
        "\0"
        /* _mesa_function_pool[43236]: FogCoordPointerListIBM (dynamic) */
        "iipi\0"
        "glFogCoordPointerListIBM\0"
        "\0"
        /* _mesa_function_pool[43267]: IndexPointerListIBM (dynamic) */
        "iipi\0"
        "glIndexPointerListIBM\0"
        "\0"
        /* _mesa_function_pool[43295]: NormalPointerListIBM (dynamic) */
        "iipi\0"
        "glNormalPointerListIBM\0"
        "\0"
        /* _mesa_function_pool[43324]: TexCoordPointerListIBM (dynamic) */
        "iiipi\0"
        "glTexCoordPointerListIBM\0"
        "\0"
        /* _mesa_function_pool[43356]: VertexPointerListIBM (dynamic) */
        "iiipi\0"
        "glVertexPointerListIBM\0"
        "\0"
        /* _mesa_function_pool[43386]: TbufferMask3DFX (dynamic) */
        "i\0"
        "glTbufferMask3DFX\0"
        "\0"
        /* _mesa_function_pool[43407]: TextureColorMaskSGIS (dynamic) */
        "iiii\0"
        "glTextureColorMaskSGIS\0"
        "\0"
        /* _mesa_function_pool[43436]: DeleteFencesNV (dynamic) */
        "ip\0"
        "glDeleteFencesNV\0"
        "\0"
        /* _mesa_function_pool[43457]: GenFencesNV (dynamic) */
        "ip\0"
        "glGenFencesNV\0"
        "\0"
        /* _mesa_function_pool[43475]: IsFenceNV (dynamic) */
        "i\0"
        "glIsFenceNV\0"
        "\0"
        /* _mesa_function_pool[43490]: TestFenceNV (dynamic) */
        "i\0"
        "glTestFenceNV\0"
        "\0"
        /* _mesa_function_pool[43507]: GetFenceivNV (dynamic) */
        "iip\0"
        "glGetFenceivNV\0"
        "\0"
        /* _mesa_function_pool[43527]: FinishFenceNV (dynamic) */
        "i\0"
        "glFinishFenceNV\0"
        "\0"
        /* _mesa_function_pool[43546]: SetFenceNV (dynamic) */
        "ii\0"
        "glSetFenceNV\0"
        "\0"
        /* _mesa_function_pool[43563]: MapControlPointsNV (dynamic) */
        "iiiiiiiip\0"
        "glMapControlPointsNV\0"
        "\0"
        /* _mesa_function_pool[43595]: MapParameterivNV (dynamic) */
        "iip\0"
        "glMapParameterivNV\0"
        "\0"
        /* _mesa_function_pool[43619]: MapParameterfvNV (dynamic) */
        "iip\0"
        "glMapParameterfvNV\0"
        "\0"
        /* _mesa_function_pool[43643]: GetMapControlPointsNV (dynamic) */
        "iiiiiip\0"
        "glGetMapControlPointsNV\0"
        "\0"
        /* _mesa_function_pool[43676]: GetMapParameterivNV (dynamic) */
        "iip\0"
        "glGetMapParameterivNV\0"
        "\0"
        /* _mesa_function_pool[43703]: GetMapParameterfvNV (dynamic) */
        "iip\0"
        "glGetMapParameterfvNV\0"
        "\0"
        /* _mesa_function_pool[43730]: GetMapAttribParameterivNV (dynamic) */
        "iiip\0"
        "glGetMapAttribParameterivNV\0"
        "\0"
        /* _mesa_function_pool[43764]: GetMapAttribParameterfvNV (dynamic) */
        "iiip\0"
        "glGetMapAttribParameterfvNV\0"
        "\0"
        /* _mesa_function_pool[43798]: EvalMapsNV (dynamic) */
        "ii\0"
        "glEvalMapsNV\0"
        "\0"
        /* _mesa_function_pool[43815]: CombinerStageParameterfvNV (dynamic) */
        "iip\0"
        "glCombinerStageParameterfvNV\0"
        "\0"
        /* _mesa_function_pool[43849]: GetCombinerStageParameterfvNV (dynamic) */
        "iip\0"
        "glGetCombinerStageParameterfvNV\0"
        "\0"
        /* _mesa_function_pool[43886]: AreProgramsResidentNV (will be remapped) */
        "ipp\0"
        "glAreProgramsResidentNV\0"
        "\0"
        /* _mesa_function_pool[43915]: ExecuteProgramNV (will be remapped) */
        "iip\0"
        "glExecuteProgramNV\0"
        "\0"
        /* _mesa_function_pool[43939]: GetProgramParameterdvNV (will be remapped) */
        "iiip\0"
        "glGetProgramParameterdvNV\0"
        "\0"
        /* _mesa_function_pool[43971]: GetProgramParameterfvNV (will be remapped) */
        "iiip\0"
        "glGetProgramParameterfvNV\0"
        "\0"
        /* _mesa_function_pool[44003]: GetProgramivNV (will be remapped) */
        "iip\0"
        "glGetProgramivNV\0"
        "\0"
        /* _mesa_function_pool[44025]: GetProgramStringNV (will be remapped) */
        "iip\0"
        "glGetProgramStringNV\0"
        "\0"
        /* _mesa_function_pool[44051]: GetTrackMatrixivNV (will be remapped) */
        "iiip\0"
        "glGetTrackMatrixivNV\0"
        "\0"
        /* _mesa_function_pool[44078]: GetVertexAttribdvNV (will be remapped) */
        "iip\0"
        "glGetVertexAttribdvNV\0"
        "\0"
        /* _mesa_function_pool[44105]: GetVertexAttribfvNV (will be remapped) */
        "iip\0"
        "glGetVertexAttribfvNV\0"
        "\0"
        /* _mesa_function_pool[44132]: GetVertexAttribivNV (will be remapped) */
        "iip\0"
        "glGetVertexAttribivNV\0"
        "\0"
        /* _mesa_function_pool[44159]: LoadProgramNV (will be remapped) */
        "iiip\0"
        "glLoadProgramNV\0"
        "\0"
        /* _mesa_function_pool[44181]: ProgramParameters4dvNV (will be remapped) */
        "iiip\0"
        "glProgramParameters4dvNV\0"
        "\0"
        /* _mesa_function_pool[44212]: ProgramParameters4fvNV (will be remapped) */
        "iiip\0"
        "glProgramParameters4fvNV\0"
        "\0"
        /* _mesa_function_pool[44243]: RequestResidentProgramsNV (will be remapped) */
        "ip\0"
        "glRequestResidentProgramsNV\0"
        "\0"
        /* _mesa_function_pool[44275]: TrackMatrixNV (will be remapped) */
        "iiii\0"
        "glTrackMatrixNV\0"
        "\0"
        /* _mesa_function_pool[44297]: VertexAttribPointerNV (will be remapped) */
        "iiiip\0"
        "glVertexAttribPointerNV\0"
        "\0"
        /* _mesa_function_pool[44328]: VertexAttrib1sNV (will be remapped) */
        "ii\0"
        "glVertexAttrib1sNV\0"
        "\0"
        /* _mesa_function_pool[44351]: VertexAttrib1svNV (will be remapped) */
        "ip\0"
        "glVertexAttrib1svNV\0"
        "\0"
        /* _mesa_function_pool[44375]: VertexAttrib2sNV (will be remapped) */
        "iii\0"
        "glVertexAttrib2sNV\0"
        "\0"
        /* _mesa_function_pool[44399]: VertexAttrib2svNV (will be remapped) */
        "ip\0"
        "glVertexAttrib2svNV\0"
        "\0"
        /* _mesa_function_pool[44423]: VertexAttrib3sNV (will be remapped) */
        "iiii\0"
        "glVertexAttrib3sNV\0"
        "\0"
        /* _mesa_function_pool[44448]: VertexAttrib3svNV (will be remapped) */
        "ip\0"
        "glVertexAttrib3svNV\0"
        "\0"
        /* _mesa_function_pool[44472]: VertexAttrib4sNV (will be remapped) */
        "iiiii\0"
        "glVertexAttrib4sNV\0"
        "\0"
        /* _mesa_function_pool[44498]: VertexAttrib4svNV (will be remapped) */
        "ip\0"
        "glVertexAttrib4svNV\0"
        "\0"
        /* _mesa_function_pool[44522]: VertexAttrib1fNV (will be remapped) */
        "if\0"
        "glVertexAttrib1fNV\0"
        "\0"
        /* _mesa_function_pool[44545]: VertexAttrib1fvNV (will be remapped) */
        "ip\0"
        "glVertexAttrib1fvNV\0"
        "\0"
        /* _mesa_function_pool[44569]: VertexAttrib2fNV (will be remapped) */
        "iff\0"
        "glVertexAttrib2fNV\0"
        "\0"
        /* _mesa_function_pool[44593]: VertexAttrib2fvNV (will be remapped) */
        "ip\0"
        "glVertexAttrib2fvNV\0"
        "\0"
        /* _mesa_function_pool[44617]: VertexAttrib3fNV (will be remapped) */
        "ifff\0"
        "glVertexAttrib3fNV\0"
        "\0"
        /* _mesa_function_pool[44642]: VertexAttrib3fvNV (will be remapped) */
        "ip\0"
        "glVertexAttrib3fvNV\0"
        "\0"
        /* _mesa_function_pool[44666]: VertexAttrib4fNV (will be remapped) */
        "iffff\0"
        "glVertexAttrib4fNV\0"
        "\0"
        /* _mesa_function_pool[44692]: VertexAttrib4fvNV (will be remapped) */
        "ip\0"
        "glVertexAttrib4fvNV\0"
        "\0"
        /* _mesa_function_pool[44716]: VertexAttrib1dNV (will be remapped) */
        "id\0"
        "glVertexAttrib1dNV\0"
        "\0"
        /* _mesa_function_pool[44739]: VertexAttrib1dvNV (will be remapped) */
        "ip\0"
        "glVertexAttrib1dvNV\0"
        "\0"
        /* _mesa_function_pool[44763]: VertexAttrib2dNV (will be remapped) */
        "idd\0"
        "glVertexAttrib2dNV\0"
        "\0"
        /* _mesa_function_pool[44787]: VertexAttrib2dvNV (will be remapped) */
        "ip\0"
        "glVertexAttrib2dvNV\0"
        "\0"
        /* _mesa_function_pool[44811]: VertexAttrib3dNV (will be remapped) */
        "iddd\0"
        "glVertexAttrib3dNV\0"
        "\0"
        /* _mesa_function_pool[44836]: VertexAttrib3dvNV (will be remapped) */
        "ip\0"
        "glVertexAttrib3dvNV\0"
        "\0"
        /* _mesa_function_pool[44860]: VertexAttrib4dNV (will be remapped) */
        "idddd\0"
        "glVertexAttrib4dNV\0"
        "\0"
        /* _mesa_function_pool[44886]: VertexAttrib4dvNV (will be remapped) */
        "ip\0"
        "glVertexAttrib4dvNV\0"
        "\0"
        /* _mesa_function_pool[44910]: VertexAttrib4ubNV (will be remapped) */
        "iiiii\0"
        "glVertexAttrib4ubNV\0"
        "\0"
        /* _mesa_function_pool[44937]: VertexAttrib4ubvNV (will be remapped) */
        "ip\0"
        "glVertexAttrib4ubvNV\0"
        "\0"
        /* _mesa_function_pool[44962]: VertexAttribs1svNV (will be remapped) */
        "iip\0"
        "glVertexAttribs1svNV\0"
        "\0"
        /* _mesa_function_pool[44988]: VertexAttribs2svNV (will be remapped) */
        "iip\0"
        "glVertexAttribs2svNV\0"
        "\0"
        /* _mesa_function_pool[45014]: VertexAttribs3svNV (will be remapped) */
        "iip\0"
        "glVertexAttribs3svNV\0"
        "\0"
        /* _mesa_function_pool[45040]: VertexAttribs4svNV (will be remapped) */
        "iip\0"
        "glVertexAttribs4svNV\0"
        "\0"
        /* _mesa_function_pool[45066]: VertexAttribs1fvNV (will be remapped) */
        "iip\0"
        "glVertexAttribs1fvNV\0"
        "\0"
        /* _mesa_function_pool[45092]: VertexAttribs2fvNV (will be remapped) */
        "iip\0"
        "glVertexAttribs2fvNV\0"
        "\0"
        /* _mesa_function_pool[45118]: VertexAttribs3fvNV (will be remapped) */
        "iip\0"
        "glVertexAttribs3fvNV\0"
        "\0"
        /* _mesa_function_pool[45144]: VertexAttribs4fvNV (will be remapped) */
        "iip\0"
        "glVertexAttribs4fvNV\0"
        "\0"
        /* _mesa_function_pool[45170]: VertexAttribs1dvNV (will be remapped) */
        "iip\0"
        "glVertexAttribs1dvNV\0"
        "\0"
        /* _mesa_function_pool[45196]: VertexAttribs2dvNV (will be remapped) */
        "iip\0"
        "glVertexAttribs2dvNV\0"
        "\0"
        /* _mesa_function_pool[45222]: VertexAttribs3dvNV (will be remapped) */
        "iip\0"
        "glVertexAttribs3dvNV\0"
        "\0"
        /* _mesa_function_pool[45248]: VertexAttribs4dvNV (will be remapped) */
        "iip\0"
        "glVertexAttribs4dvNV\0"
        "\0"
        /* _mesa_function_pool[45274]: VertexAttribs4ubvNV (will be remapped) */
        "iip\0"
        "glVertexAttribs4ubvNV\0"
        "\0"
        /* _mesa_function_pool[45301]: TexBumpParameterfvATI (will be remapped) */
        "ip\0"
        "glTexBumpParameterfvATI\0"
        "\0"
        /* _mesa_function_pool[45329]: TexBumpParameterivATI (will be remapped) */
        "ip\0"
        "glTexBumpParameterivATI\0"
        "\0"
        /* _mesa_function_pool[45357]: GetTexBumpParameterfvATI (will be remapped) */
        "ip\0"
        "glGetTexBumpParameterfvATI\0"
        "\0"
        /* _mesa_function_pool[45388]: GetTexBumpParameterivATI (will be remapped) */
        "ip\0"
        "glGetTexBumpParameterivATI\0"
        "\0"
        /* _mesa_function_pool[45419]: GenFragmentShadersATI (will be remapped) */
        "i\0"
        "glGenFragmentShadersATI\0"
        "\0"
        /* _mesa_function_pool[45446]: BindFragmentShaderATI (will be remapped) */
        "i\0"
        "glBindFragmentShaderATI\0"
        "\0"
        /* _mesa_function_pool[45473]: DeleteFragmentShaderATI (will be remapped) */
        "i\0"
        "glDeleteFragmentShaderATI\0"
        "\0"
        /* _mesa_function_pool[45502]: BeginFragmentShaderATI (will be remapped) */
        "\0"
        "glBeginFragmentShaderATI\0"
        "\0"
        /* _mesa_function_pool[45529]: EndFragmentShaderATI (will be remapped) */
        "\0"
        "glEndFragmentShaderATI\0"
        "\0"
        /* _mesa_function_pool[45554]: PassTexCoordATI (will be remapped) */
        "iii\0"
        "glPassTexCoordATI\0"
        "\0"
        /* _mesa_function_pool[45577]: SampleMapATI (will be remapped) */
        "iii\0"
        "glSampleMapATI\0"
        "\0"
        /* _mesa_function_pool[45597]: ColorFragmentOp1ATI (will be remapped) */
        "iiiiiii\0"
        "glColorFragmentOp1ATI\0"
        "\0"
        /* _mesa_function_pool[45628]: ColorFragmentOp2ATI (will be remapped) */
        "iiiiiiiiii\0"
        "glColorFragmentOp2ATI\0"
        "\0"
        /* _mesa_function_pool[45662]: ColorFragmentOp3ATI (will be remapped) */
        "iiiiiiiiiiiii\0"
        "glColorFragmentOp3ATI\0"
        "\0"
        /* _mesa_function_pool[45699]: AlphaFragmentOp1ATI (will be remapped) */
        "iiiiii\0"
        "glAlphaFragmentOp1ATI\0"
        "\0"
        /* _mesa_function_pool[45729]: AlphaFragmentOp2ATI (will be remapped) */
        "iiiiiiiii\0"
        "glAlphaFragmentOp2ATI\0"
        "\0"
        /* _mesa_function_pool[45762]: AlphaFragmentOp3ATI (will be remapped) */
        "iiiiiiiiiiii\0"
        "glAlphaFragmentOp3ATI\0"
        "\0"
        /* _mesa_function_pool[45798]: SetFragmentShaderConstantATI (will be remapped) */
        "ip\0"
        "glSetFragmentShaderConstantATI\0"
        "\0"
        /* _mesa_function_pool[45833]: DrawMeshArraysSUN (dynamic) */
        "iiii\0"
        "glDrawMeshArraysSUN\0"
        "\0"
        /* _mesa_function_pool[45859]: ActiveStencilFaceEXT (will be remapped) */
        "i\0"
        "glActiveStencilFaceEXT\0"
        "\0"
        /* _mesa_function_pool[45885]: ObjectPurgeableAPPLE (will be remapped) */
        "iii\0"
        "glObjectPurgeableAPPLE\0"
        "\0"
        /* _mesa_function_pool[45913]: ObjectUnpurgeableAPPLE (will be remapped) */
        "iii\0"
        "glObjectUnpurgeableAPPLE\0"
        "\0"
        /* _mesa_function_pool[45943]: GetObjectParameterivAPPLE (will be remapped) */
        "iiip\0"
        "glGetObjectParameterivAPPLE\0"
        "\0"
        /* _mesa_function_pool[45977]: BindVertexArrayAPPLE (dynamic) */
        "i\0"
        "glBindVertexArrayAPPLE\0"
        "\0"
        /* _mesa_function_pool[46003]: DeleteVertexArraysAPPLE (dynamic) */
        "ip\0"
        "glDeleteVertexArraysAPPLE\0"
        "\0"
        /* _mesa_function_pool[46033]: GenVertexArraysAPPLE (dynamic) */
        "ip\0"
        "glGenVertexArraysAPPLE\0"
        "\0"
        /* _mesa_function_pool[46060]: IsVertexArrayAPPLE (dynamic) */
        "i\0"
        "glIsVertexArrayAPPLE\0"
        "\0"
        /* _mesa_function_pool[46084]: ProgramNamedParameter4fNV (will be remapped) */
        "iipffff\0"
        "glProgramNamedParameter4fNV\0"
        "\0"
        /* _mesa_function_pool[46121]: ProgramNamedParameter4dNV (will be remapped) */
        "iipdddd\0"
        "glProgramNamedParameter4dNV\0"
        "\0"
        /* _mesa_function_pool[46158]: ProgramNamedParameter4fvNV (will be remapped) */
        "iipp\0"
        "glProgramNamedParameter4fvNV\0"
        "\0"
        /* _mesa_function_pool[46193]: ProgramNamedParameter4dvNV (will be remapped) */
        "iipp\0"
        "glProgramNamedParameter4dvNV\0"
        "\0"
        /* _mesa_function_pool[46228]: GetProgramNamedParameterfvNV (will be remapped) */
        "iipp\0"
        "glGetProgramNamedParameterfvNV\0"
        "\0"
        /* _mesa_function_pool[46265]: GetProgramNamedParameterdvNV (will be remapped) */
        "iipp\0"
        "glGetProgramNamedParameterdvNV\0"
        "\0"
        /* _mesa_function_pool[46302]: DepthBoundsEXT (will be remapped) */
        "dd\0"
        "glDepthBoundsEXT\0"
        "\0"
        /* _mesa_function_pool[46323]: BindRenderbufferEXT (will be remapped) */
        "ii\0"
        "glBindRenderbufferEXT\0"
        "\0"
        /* _mesa_function_pool[46349]: BindFramebufferEXT (will be remapped) */
        "ii\0"
        "glBindFramebufferEXT\0"
        "\0"
        /* _mesa_function_pool[46374]: StringMarkerGREMEDY (will be remapped) */
        "ip\0"
        "glStringMarkerGREMEDY\0"
        "\0"
        /* _mesa_function_pool[46400]: ProvokingVertex (will be remapped) */
        "i\0"
        "glProvokingVertexEXT\0"
        "glProvokingVertex\0"
        "\0"
        /* _mesa_function_pool[46442]: ColorMaski (will be remapped) */
        "iiiii\0"
        "glColorMaskIndexedEXT\0"
        "glColorMaski\0"
        "\0"
        /* _mesa_function_pool[46484]: GetBooleani_v (will be remapped) */
        "iip\0"
        "glGetBooleanIndexedvEXT\0"
        "glGetBooleani_v\0"
        "\0"
        /* _mesa_function_pool[46529]: GetIntegeri_v (will be remapped) */
        "iip\0"
        "glGetIntegerIndexedvEXT\0"
        "glGetIntegeri_v\0"
        "\0"
        /* _mesa_function_pool[46574]: Enablei (will be remapped) */
        "ii\0"
        "glEnableIndexedEXT\0"
        "glEnablei\0"
        "\0"
        /* _mesa_function_pool[46607]: Disablei (will be remapped) */
        "ii\0"
        "glDisableIndexedEXT\0"
        "glDisablei\0"
        "\0"
        /* _mesa_function_pool[46642]: IsEnabledi (will be remapped) */
        "ii\0"
        "glIsEnabledIndexedEXT\0"
        "glIsEnabledi\0"
        "\0"
        /* _mesa_function_pool[46681]: BufferParameteriAPPLE (will be remapped) */
        "iii\0"
        "glBufferParameteriAPPLE\0"
        "\0"
        /* _mesa_function_pool[46710]: FlushMappedBufferRangeAPPLE (will be remapped) */
        "iii\0"
        "glFlushMappedBufferRangeAPPLE\0"
        "\0"
        /* _mesa_function_pool[46745]: GetPerfMonitorGroupsAMD (will be remapped) */
        "pip\0"
        "glGetPerfMonitorGroupsAMD\0"
        "\0"
        /* _mesa_function_pool[46776]: GetPerfMonitorCountersAMD (will be remapped) */
        "ippip\0"
        "glGetPerfMonitorCountersAMD\0"
        "\0"
        /* _mesa_function_pool[46811]: GetPerfMonitorGroupStringAMD (will be remapped) */
        "iipp\0"
        "glGetPerfMonitorGroupStringAMD\0"
        "\0"
        /* _mesa_function_pool[46848]: GetPerfMonitorCounterStringAMD (will be remapped) */
        "iiipp\0"
        "glGetPerfMonitorCounterStringAMD\0"
        "\0"
        /* _mesa_function_pool[46888]: GetPerfMonitorCounterInfoAMD (will be remapped) */
        "iiip\0"
        "glGetPerfMonitorCounterInfoAMD\0"
        "\0"
        /* _mesa_function_pool[46925]: GenPerfMonitorsAMD (will be remapped) */
        "ip\0"
        "glGenPerfMonitorsAMD\0"
        "\0"
        /* _mesa_function_pool[46950]: DeletePerfMonitorsAMD (will be remapped) */
        "ip\0"
        "glDeletePerfMonitorsAMD\0"
        "\0"
        /* _mesa_function_pool[46978]: SelectPerfMonitorCountersAMD (will be remapped) */
        "iiiip\0"
        "glSelectPerfMonitorCountersAMD\0"
        "\0"
        /* _mesa_function_pool[47016]: BeginPerfMonitorAMD (will be remapped) */
        "i\0"
        "glBeginPerfMonitorAMD\0"
        "\0"
        /* _mesa_function_pool[47041]: EndPerfMonitorAMD (will be remapped) */
        "i\0"
        "glEndPerfMonitorAMD\0"
        "\0"
        /* _mesa_function_pool[47064]: GetPerfMonitorCounterDataAMD (will be remapped) */
        "iiipp\0"
        "glGetPerfMonitorCounterDataAMD\0"
        "\0"
        /* _mesa_function_pool[47102]: TextureRangeAPPLE (dynamic) */
        "iip\0"
        "glTextureRangeAPPLE\0"
        "\0"
        /* _mesa_function_pool[47127]: GetTexParameterPointervAPPLE (dynamic) */
        "iip\0"
        "glGetTexParameterPointervAPPLE\0"
        "\0"
        /* _mesa_function_pool[47163]: UseShaderProgramEXT (will be remapped) */
        "ii\0"
        "glUseShaderProgramEXT\0"
        "\0"
        /* _mesa_function_pool[47189]: ActiveProgramEXT (will be remapped) */
        "i\0"
        "glActiveProgramEXT\0"
        "\0"
        /* _mesa_function_pool[47211]: CreateShaderProgramEXT (will be remapped) */
        "ip\0"
        "glCreateShaderProgramEXT\0"
        "\0"
        /* _mesa_function_pool[47240]: CopyImageSubDataNV (will be remapped) */
        "iiiiiiiiiiiiiii\0"
        "glCopyImageSubDataNV\0"
        "\0"
        /* _mesa_function_pool[47278]: MatrixLoadfEXT (will be remapped) */
        "ip\0"
        "glMatrixLoadfEXT\0"
        "\0"
        /* _mesa_function_pool[47299]: MatrixLoaddEXT (will be remapped) */
        "ip\0"
        "glMatrixLoaddEXT\0"
        "\0"
        /* _mesa_function_pool[47320]: MatrixMultfEXT (will be remapped) */
        "ip\0"
        "glMatrixMultfEXT\0"
        "\0"
        /* _mesa_function_pool[47341]: MatrixMultdEXT (will be remapped) */
        "ip\0"
        "glMatrixMultdEXT\0"
        "\0"
        /* _mesa_function_pool[47362]: MatrixLoadIdentityEXT (will be remapped) */
        "i\0"
        "glMatrixLoadIdentityEXT\0"
        "\0"
        /* _mesa_function_pool[47389]: MatrixRotatefEXT (will be remapped) */
        "iffff\0"
        "glMatrixRotatefEXT\0"
        "\0"
        /* _mesa_function_pool[47415]: MatrixRotatedEXT (will be remapped) */
        "idddd\0"
        "glMatrixRotatedEXT\0"
        "\0"
        /* _mesa_function_pool[47441]: MatrixScalefEXT (will be remapped) */
        "ifff\0"
        "glMatrixScalefEXT\0"
        "\0"
        /* _mesa_function_pool[47465]: MatrixScaledEXT (will be remapped) */
        "iddd\0"
        "glMatrixScaledEXT\0"
        "\0"
        /* _mesa_function_pool[47489]: MatrixTranslatefEXT (will be remapped) */
        "ifff\0"
        "glMatrixTranslatefEXT\0"
        "\0"
        /* _mesa_function_pool[47517]: MatrixTranslatedEXT (will be remapped) */
        "iddd\0"
        "glMatrixTranslatedEXT\0"
        "\0"
        /* _mesa_function_pool[47545]: MatrixOrthoEXT (will be remapped) */
        "idddddd\0"
        "glMatrixOrthoEXT\0"
        "\0"
        /* _mesa_function_pool[47571]: MatrixFrustumEXT (will be remapped) */
        "idddddd\0"
        "glMatrixFrustumEXT\0"
        "\0"
        /* _mesa_function_pool[47599]: MatrixPushEXT (will be remapped) */
        "i\0"
        "glMatrixPushEXT\0"
        "\0"
        /* _mesa_function_pool[47618]: MatrixPopEXT (will be remapped) */
        "i\0"
        "glMatrixPopEXT\0"
        "\0"
        /* _mesa_function_pool[47636]: ClientAttribDefaultEXT (will be remapped) */
        "i\0"
        "glClientAttribDefaultEXT\0"
        "\0"
        /* _mesa_function_pool[47664]: PushClientAttribDefaultEXT (will be remapped) */
        "i\0"
        "glPushClientAttribDefaultEXT\0"
        "\0"
        /* _mesa_function_pool[47696]: GetTextureParameterivEXT (will be remapped) */
        "iiip\0"
        "glGetTextureParameterivEXT\0"
        "\0"
        /* _mesa_function_pool[47729]: GetTextureParameterfvEXT (will be remapped) */
        "iiip\0"
        "glGetTextureParameterfvEXT\0"
        "\0"
        /* _mesa_function_pool[47762]: GetTextureLevelParameterivEXT (will be remapped) */
        "iiiip\0"
        "glGetTextureLevelParameterivEXT\0"
        "\0"
        /* _mesa_function_pool[47801]: GetTextureLevelParameterfvEXT (will be remapped) */
        "iiiip\0"
        "glGetTextureLevelParameterfvEXT\0"
        "\0"
        /* _mesa_function_pool[47840]: TextureParameteriEXT (will be remapped) */
        "iiii\0"
        "glTextureParameteriEXT\0"
        "\0"
        /* _mesa_function_pool[47869]: TextureParameterivEXT (will be remapped) */
        "iiip\0"
        "glTextureParameterivEXT\0"
        "\0"
        /* _mesa_function_pool[47899]: TextureParameterfEXT (will be remapped) */
        "iiif\0"
        "glTextureParameterfEXT\0"
        "\0"
        /* _mesa_function_pool[47928]: TextureParameterfvEXT (will be remapped) */
        "iiip\0"
        "glTextureParameterfvEXT\0"
        "\0"
        /* _mesa_function_pool[47958]: TextureImage1DEXT (will be remapped) */
        "iiiiiiiip\0"
        "glTextureImage1DEXT\0"
        "\0"
        /* _mesa_function_pool[47989]: TextureImage2DEXT (will be remapped) */
        "iiiiiiiiip\0"
        "glTextureImage2DEXT\0"
        "\0"
        /* _mesa_function_pool[48021]: TextureImage3DEXT (will be remapped) */
        "iiiiiiiiiip\0"
        "glTextureImage3DEXT\0"
        "\0"
        /* _mesa_function_pool[48054]: TextureSubImage1DEXT (will be remapped) */
        "iiiiiiip\0"
        "glTextureSubImage1DEXT\0"
        "\0"
        /* _mesa_function_pool[48087]: TextureSubImage2DEXT (will be remapped) */
        "iiiiiiiiip\0"
        "glTextureSubImage2DEXT\0"
        "\0"
        /* _mesa_function_pool[48122]: TextureSubImage3DEXT (will be remapped) */
        "iiiiiiiiiiip\0"
        "glTextureSubImage3DEXT\0"
        "\0"
        /* _mesa_function_pool[48159]: CopyTextureImage1DEXT (will be remapped) */
        "iiiiiiii\0"
        "glCopyTextureImage1DEXT\0"
        "\0"
        /* _mesa_function_pool[48193]: CopyTextureImage2DEXT (will be remapped) */
        "iiiiiiiii\0"
        "glCopyTextureImage2DEXT\0"
        "\0"
        /* _mesa_function_pool[48228]: CopyTextureSubImage1DEXT (will be remapped) */
        "iiiiiii\0"
        "glCopyTextureSubImage1DEXT\0"
        "\0"
        /* _mesa_function_pool[48264]: CopyTextureSubImage2DEXT (will be remapped) */
        "iiiiiiiii\0"
        "glCopyTextureSubImage2DEXT\0"
        "\0"
        /* _mesa_function_pool[48302]: CopyTextureSubImage3DEXT (will be remapped) */
        "iiiiiiiiii\0"
        "glCopyTextureSubImage3DEXT\0"
        "\0"
        /* _mesa_function_pool[48341]: GetTextureImageEXT (will be remapped) */
        "iiiiip\0"
        "glGetTextureImageEXT\0"
        "\0"
        /* _mesa_function_pool[48370]: BindMultiTextureEXT (will be remapped) */
        "iii\0"
        "glBindMultiTextureEXT\0"
        "\0"
        /* _mesa_function_pool[48397]: EnableClientStateiEXT (will be remapped) */
        "ii\0"
        "glEnableClientStateIndexedEXT\0"
        "glEnableClientStateiEXT\0"
        "\0"
        /* _mesa_function_pool[48455]: DisableClientStateiEXT (will be remapped) */
        "ii\0"
        "glDisableClientStateIndexedEXT\0"
        "glDisableClientStateiEXT\0"
        "\0"
        /* _mesa_function_pool[48515]: GetPointerIndexedvEXT (will be remapped) */
        "iip\0"
        "glGetPointerIndexedvEXT\0"
        "glGetPointeri_vEXT\0"
        "\0"
        /* _mesa_function_pool[48563]: MultiTexEnviEXT (will be remapped) */
        "iiii\0"
        "glMultiTexEnviEXT\0"
        "\0"
        /* _mesa_function_pool[48587]: MultiTexEnvivEXT (will be remapped) */
        "iiip\0"
        "glMultiTexEnvivEXT\0"
        "\0"
        /* _mesa_function_pool[48612]: MultiTexEnvfEXT (will be remapped) */
        "iiif\0"
        "glMultiTexEnvfEXT\0"
        "\0"
        /* _mesa_function_pool[48636]: MultiTexEnvfvEXT (will be remapped) */
        "iiip\0"
        "glMultiTexEnvfvEXT\0"
        "\0"
        /* _mesa_function_pool[48661]: GetMultiTexEnvivEXT (will be remapped) */
        "iiip\0"
        "glGetMultiTexEnvivEXT\0"
        "\0"
        /* _mesa_function_pool[48689]: GetMultiTexEnvfvEXT (will be remapped) */
        "iiip\0"
        "glGetMultiTexEnvfvEXT\0"
        "\0"
        /* _mesa_function_pool[48717]: MultiTexParameteriEXT (will be remapped) */
        "iiii\0"
        "glMultiTexParameteriEXT\0"
        "\0"
        /* _mesa_function_pool[48747]: MultiTexParameterivEXT (will be remapped) */
        "iiip\0"
        "glMultiTexParameterivEXT\0"
        "\0"
        /* _mesa_function_pool[48778]: MultiTexParameterfEXT (will be remapped) */
        "iiif\0"
        "glMultiTexParameterfEXT\0"
        "\0"
        /* _mesa_function_pool[48808]: MultiTexParameterfvEXT (will be remapped) */
        "iiip\0"
        "glMultiTexParameterfvEXT\0"
        "\0"
        /* _mesa_function_pool[48839]: GetMultiTexParameterivEXT (will be remapped) */
        "iiip\0"
        "glGetMultiTexParameterivEXT\0"
        "\0"
        /* _mesa_function_pool[48873]: GetMultiTexParameterfvEXT (will be remapped) */
        "iiip\0"
        "glGetMultiTexParameterfvEXT\0"
        "\0"
        /* _mesa_function_pool[48907]: GetMultiTexImageEXT (will be remapped) */
        "iiiiip\0"
        "glGetMultiTexImageEXT\0"
        "\0"
        /* _mesa_function_pool[48937]: GetMultiTexLevelParameterivEXT (will be remapped) */
        "iiiip\0"
        "glGetMultiTexLevelParameterivEXT\0"
        "\0"
        /* _mesa_function_pool[48977]: GetMultiTexLevelParameterfvEXT (will be remapped) */
        "iiiip\0"
        "glGetMultiTexLevelParameterfvEXT\0"
        "\0"
        /* _mesa_function_pool[49017]: MultiTexImage1DEXT (will be remapped) */
        "iiiiiiiip\0"
        "glMultiTexImage1DEXT\0"
        "\0"
        /* _mesa_function_pool[49049]: MultiTexImage2DEXT (will be remapped) */
        "iiiiiiiiip\0"
        "glMultiTexImage2DEXT\0"
        "\0"
        /* _mesa_function_pool[49082]: MultiTexImage3DEXT (will be remapped) */
        "iiiiiiiiiip\0"
        "glMultiTexImage3DEXT\0"
        "\0"
        /* _mesa_function_pool[49116]: MultiTexSubImage1DEXT (will be remapped) */
        "iiiiiiip\0"
        "glMultiTexSubImage1DEXT\0"
        "\0"
        /* _mesa_function_pool[49150]: MultiTexSubImage2DEXT (will be remapped) */
        "iiiiiiiiip\0"
        "glMultiTexSubImage2DEXT\0"
        "\0"
        /* _mesa_function_pool[49186]: MultiTexSubImage3DEXT (will be remapped) */
        "iiiiiiiiiiip\0"
        "glMultiTexSubImage3DEXT\0"
        "\0"
        /* _mesa_function_pool[49224]: CopyMultiTexImage1DEXT (will be remapped) */
        "iiiiiiii\0"
        "glCopyMultiTexImage1DEXT\0"
        "\0"
        /* _mesa_function_pool[49259]: CopyMultiTexImage2DEXT (will be remapped) */
        "iiiiiiiii\0"
        "glCopyMultiTexImage2DEXT\0"
        "\0"
        /* _mesa_function_pool[49295]: CopyMultiTexSubImage1DEXT (will be remapped) */
        "iiiiiii\0"
        "glCopyMultiTexSubImage1DEXT\0"
        "\0"
        /* _mesa_function_pool[49332]: CopyMultiTexSubImage2DEXT (will be remapped) */
        "iiiiiiiii\0"
        "glCopyMultiTexSubImage2DEXT\0"
        "\0"
        /* _mesa_function_pool[49371]: CopyMultiTexSubImage3DEXT (will be remapped) */
        "iiiiiiiiii\0"
        "glCopyMultiTexSubImage3DEXT\0"
        "\0"
        /* _mesa_function_pool[49411]: MultiTexGendEXT (will be remapped) */
        "iiid\0"
        "glMultiTexGendEXT\0"
        "\0"
        /* _mesa_function_pool[49435]: MultiTexGendvEXT (will be remapped) */
        "iiip\0"
        "glMultiTexGendvEXT\0"
        "\0"
        /* _mesa_function_pool[49460]: MultiTexGenfEXT (will be remapped) */
        "iiif\0"
        "glMultiTexGenfEXT\0"
        "\0"
        /* _mesa_function_pool[49484]: MultiTexGenfvEXT (will be remapped) */
        "iiip\0"
        "glMultiTexGenfvEXT\0"
        "\0"
        /* _mesa_function_pool[49509]: MultiTexGeniEXT (will be remapped) */
        "iiii\0"
        "glMultiTexGeniEXT\0"
        "\0"
        /* _mesa_function_pool[49533]: MultiTexGenivEXT (will be remapped) */
        "iiip\0"
        "glMultiTexGenivEXT\0"
        "\0"
        /* _mesa_function_pool[49558]: GetMultiTexGendvEXT (will be remapped) */
        "iiip\0"
        "glGetMultiTexGendvEXT\0"
        "\0"
        /* _mesa_function_pool[49586]: GetMultiTexGenfvEXT (will be remapped) */
        "iiip\0"
        "glGetMultiTexGenfvEXT\0"
        "\0"
        /* _mesa_function_pool[49614]: GetMultiTexGenivEXT (will be remapped) */
        "iiip\0"
        "glGetMultiTexGenivEXT\0"
        "\0"
        /* _mesa_function_pool[49642]: MultiTexCoordPointerEXT (will be remapped) */
        "iiiip\0"
        "glMultiTexCoordPointerEXT\0"
        "\0"
        /* _mesa_function_pool[49675]: MatrixLoadTransposefEXT (will be remapped) */
        "ip\0"
        "glMatrixLoadTransposefEXT\0"
        "\0"
        /* _mesa_function_pool[49705]: MatrixLoadTransposedEXT (will be remapped) */
        "ip\0"
        "glMatrixLoadTransposedEXT\0"
        "\0"
        /* _mesa_function_pool[49735]: MatrixMultTransposefEXT (will be remapped) */
        "ip\0"
        "glMatrixMultTransposefEXT\0"
        "\0"
        /* _mesa_function_pool[49765]: MatrixMultTransposedEXT (will be remapped) */
        "ip\0"
        "glMatrixMultTransposedEXT\0"
        "\0"
        /* _mesa_function_pool[49795]: CompressedTextureImage1DEXT (will be remapped) */
        "iiiiiiip\0"
        "glCompressedTextureImage1DEXT\0"
        "\0"
        /* _mesa_function_pool[49835]: CompressedTextureImage2DEXT (will be remapped) */
        "iiiiiiiip\0"
        "glCompressedTextureImage2DEXT\0"
        "\0"
        /* _mesa_function_pool[49876]: CompressedTextureImage3DEXT (will be remapped) */
        "iiiiiiiiip\0"
        "glCompressedTextureImage3DEXT\0"
        "\0"
        /* _mesa_function_pool[49918]: CompressedTextureSubImage1DEXT (will be remapped) */
        "iiiiiiip\0"
        "glCompressedTextureSubImage1DEXT\0"
        "\0"
        /* _mesa_function_pool[49961]: CompressedTextureSubImage2DEXT (will be remapped) */
        "iiiiiiiiip\0"
        "glCompressedTextureSubImage2DEXT\0"
        "\0"
        /* _mesa_function_pool[50006]: CompressedTextureSubImage3DEXT (will be remapped) */
        "iiiiiiiiiiip\0"
        "glCompressedTextureSubImage3DEXT\0"
        "\0"
        /* _mesa_function_pool[50053]: GetCompressedTextureImageEXT (will be remapped) */
        "iiip\0"
        "glGetCompressedTextureImageEXT\0"
        "\0"
        /* _mesa_function_pool[50090]: CompressedMultiTexImage1DEXT (will be remapped) */
        "iiiiiiip\0"
        "glCompressedMultiTexImage1DEXT\0"
        "\0"
        /* _mesa_function_pool[50131]: CompressedMultiTexImage2DEXT (will be remapped) */
        "iiiiiiiip\0"
        "glCompressedMultiTexImage2DEXT\0"
        "\0"
        /* _mesa_function_pool[50173]: CompressedMultiTexImage3DEXT (will be remapped) */
        "iiiiiiiiip\0"
        "glCompressedMultiTexImage3DEXT\0"
        "\0"
        /* _mesa_function_pool[50216]: CompressedMultiTexSubImage1DEXT (will be remapped) */
        "iiiiiiip\0"
        "glCompressedMultiTexSubImage1DEXT\0"
        "\0"
        /* _mesa_function_pool[50260]: CompressedMultiTexSubImage2DEXT (will be remapped) */
        "iiiiiiiiip\0"
        "glCompressedMultiTexSubImage2DEXT\0"
        "\0"
        /* _mesa_function_pool[50306]: CompressedMultiTexSubImage3DEXT (will be remapped) */
        "iiiiiiiiiiip\0"
        "glCompressedMultiTexSubImage3DEXT\0"
        "\0"
        /* _mesa_function_pool[50354]: GetCompressedMultiTexImageEXT (will be remapped) */
        "iiip\0"
        "glGetCompressedMultiTexImageEXT\0"
        "\0"
        /* _mesa_function_pool[50392]: NamedBufferDataEXT (will be remapped) */
        "iipi\0"
        "glNamedBufferDataEXT\0"
        "\0"
        /* _mesa_function_pool[50419]: NamedBufferSubDataEXT (will be remapped) */
        "iiip\0"
        "glNamedBufferSubDataEXT\0"
        "\0"
        /* _mesa_function_pool[50449]: MapNamedBufferEXT (will be remapped) */
        "ii\0"
        "glMapNamedBufferEXT\0"
        "\0"
        /* _mesa_function_pool[50473]: GetNamedBufferSubDataEXT (will be remapped) */
        "iiip\0"
        "glGetNamedBufferSubDataEXT\0"
        "\0"
        /* _mesa_function_pool[50506]: GetNamedBufferPointervEXT (will be remapped) */
        "iip\0"
        "glGetNamedBufferPointervEXT\0"
        "\0"
        /* _mesa_function_pool[50539]: GetNamedBufferParameterivEXT (will be remapped) */
        "iip\0"
        "glGetNamedBufferParameterivEXT\0"
        "\0"
        /* _mesa_function_pool[50575]: FlushMappedNamedBufferRangeEXT (will be remapped) */
        "iii\0"
        "glFlushMappedNamedBufferRangeEXT\0"
        "\0"
        /* _mesa_function_pool[50613]: MapNamedBufferRangeEXT (will be remapped) */
        "iiii\0"
        "glMapNamedBufferRangeEXT\0"
        "\0"
        /* _mesa_function_pool[50644]: FramebufferDrawBufferEXT (will be remapped) */
        "ii\0"
        "glFramebufferDrawBufferEXT\0"
        "\0"
        /* _mesa_function_pool[50675]: FramebufferDrawBuffersEXT (will be remapped) */
        "iip\0"
        "glFramebufferDrawBuffersEXT\0"
        "\0"
        /* _mesa_function_pool[50708]: FramebufferReadBufferEXT (will be remapped) */
        "ii\0"
        "glFramebufferReadBufferEXT\0"
        "\0"
        /* _mesa_function_pool[50739]: GetFramebufferParameterivEXT (will be remapped) */
        "iip\0"
        "glGetFramebufferParameterivEXT\0"
        "\0"
        /* _mesa_function_pool[50775]: CheckNamedFramebufferStatusEXT (will be remapped) */
        "ii\0"
        "glCheckNamedFramebufferStatusEXT\0"
        "\0"
        /* _mesa_function_pool[50812]: NamedFramebufferTexture1DEXT (will be remapped) */
        "iiiii\0"
        "glNamedFramebufferTexture1DEXT\0"
        "\0"
        /* _mesa_function_pool[50850]: NamedFramebufferTexture2DEXT (will be remapped) */
        "iiiii\0"
        "glNamedFramebufferTexture2DEXT\0"
        "\0"
        /* _mesa_function_pool[50888]: NamedFramebufferTexture3DEXT (will be remapped) */
        "iiiiii\0"
        "glNamedFramebufferTexture3DEXT\0"
        "\0"
        /* _mesa_function_pool[50927]: NamedFramebufferRenderbufferEXT (will be remapped) */
        "iiii\0"
        "glNamedFramebufferRenderbufferEXT\0"
        "\0"
        /* _mesa_function_pool[50967]: GetNamedFramebufferAttachmentParameterivEXT (will be remapped) */
        "iiip\0"
        "glGetNamedFramebufferAttachmentParameterivEXT\0"
        "\0"
        /* _mesa_function_pool[51019]: NamedRenderbufferStorageEXT (will be remapped) */
        "iiii\0"
        "glNamedRenderbufferStorageEXT\0"
        "\0"
        /* _mesa_function_pool[51055]: GetNamedRenderbufferParameterivEXT (will be remapped) */
        "iip\0"
        "glGetNamedRenderbufferParameterivEXT\0"
        "\0"
        /* _mesa_function_pool[51097]: GenerateTextureMipmapEXT (will be remapped) */
        "ii\0"
        "glGenerateTextureMipmapEXT\0"
        "\0"
        /* _mesa_function_pool[51128]: GenerateMultiTexMipmapEXT (will be remapped) */
        "ii\0"
        "glGenerateMultiTexMipmapEXT\0"
        "\0"
        /* _mesa_function_pool[51160]: NamedRenderbufferStorageMultisampleEXT (will be remapped) */
        "iiiii\0"
        "glNamedRenderbufferStorageMultisampleEXT\0"
        "\0"
        /* _mesa_function_pool[51208]: NamedCopyBufferSubDataEXT (will be remapped) */
        "iiiii\0"
        "glNamedCopyBufferSubDataEXT\0"
        "\0"
        /* _mesa_function_pool[51243]: VertexArrayVertexOffsetEXT (will be remapped) */
        "iiiiii\0"
        "glVertexArrayVertexOffsetEXT\0"
        "\0"
        /* _mesa_function_pool[51280]: VertexArrayColorOffsetEXT (will be remapped) */
        "iiiiii\0"
        "glVertexArrayColorOffsetEXT\0"
        "\0"
        /* _mesa_function_pool[51316]: VertexArrayEdgeFlagOffsetEXT (will be remapped) */
        "iiii\0"
        "glVertexArrayEdgeFlagOffsetEXT\0"
        "\0"
        /* _mesa_function_pool[51353]: VertexArrayIndexOffsetEXT (will be remapped) */
        "iiiii\0"
        "glVertexArrayIndexOffsetEXT\0"
        "\0"
        /* _mesa_function_pool[51388]: VertexArrayNormalOffsetEXT (will be remapped) */
        "iiiii\0"
        "glVertexArrayNormalOffsetEXT\0"
        "\0"
        /* _mesa_function_pool[51424]: VertexArrayTexCoordOffsetEXT (will be remapped) */
        "iiiiii\0"
        "glVertexArrayTexCoordOffsetEXT\0"
        "\0"
        /* _mesa_function_pool[51463]: VertexArrayMultiTexCoordOffsetEXT (will be remapped) */
        "iiiiiii\0"
        "glVertexArrayMultiTexCoordOffsetEXT\0"
        "\0"
        /* _mesa_function_pool[51508]: VertexArrayFogCoordOffsetEXT (will be remapped) */
        "iiiii\0"
        "glVertexArrayFogCoordOffsetEXT\0"
        "\0"
        /* _mesa_function_pool[51546]: VertexArraySecondaryColorOffsetEXT (will be remapped) */
        "iiiiii\0"
        "glVertexArraySecondaryColorOffsetEXT\0"
        "\0"
        /* _mesa_function_pool[51591]: VertexArrayVertexAttribOffsetEXT (will be remapped) */
        "iiiiiiii\0"
        "glVertexArrayVertexAttribOffsetEXT\0"
        "\0"
        /* _mesa_function_pool[51636]: VertexArrayVertexAttribIOffsetEXT (will be remapped) */
        "iiiiiii\0"
        "glVertexArrayVertexAttribIOffsetEXT\0"
        "\0"
        /* _mesa_function_pool[51681]: EnableVertexArrayEXT (will be remapped) */
        "ii\0"
        "glEnableVertexArrayEXT\0"
        "\0"
        /* _mesa_function_pool[51708]: DisableVertexArrayEXT (will be remapped) */
        "ii\0"
        "glDisableVertexArrayEXT\0"
        "\0"
        /* _mesa_function_pool[51736]: EnableVertexArrayAttribEXT (will be remapped) */
        "ii\0"
        "glEnableVertexArrayAttribEXT\0"
        "\0"
        /* _mesa_function_pool[51769]: DisableVertexArrayAttribEXT (will be remapped) */
        "ii\0"
        "glDisableVertexArrayAttribEXT\0"
        "\0"
        /* _mesa_function_pool[51803]: GetVertexArrayIntegervEXT (will be remapped) */
        "iip\0"
        "glGetVertexArrayIntegervEXT\0"
        "\0"
        /* _mesa_function_pool[51836]: GetVertexArrayPointervEXT (will be remapped) */
        "iip\0"
        "glGetVertexArrayPointervEXT\0"
        "\0"
        /* _mesa_function_pool[51869]: GetVertexArrayIntegeri_vEXT (will be remapped) */
        "iiip\0"
        "glGetVertexArrayIntegeri_vEXT\0"
        "\0"
        /* _mesa_function_pool[51905]: GetVertexArrayPointeri_vEXT (will be remapped) */
        "iiip\0"
        "glGetVertexArrayPointeri_vEXT\0"
        "\0"
        /* _mesa_function_pool[51941]: NamedProgramStringEXT (will be remapped) */
        "iiiip\0"
        "glNamedProgramStringEXT\0"
        "\0"
        /* _mesa_function_pool[51972]: GetNamedProgramStringEXT (will be remapped) */
        "iiip\0"
        "glGetNamedProgramStringEXT\0"
        "\0"
        /* _mesa_function_pool[52005]: NamedProgramLocalParameter4fEXT (will be remapped) */
        "iiiffff\0"
        "glNamedProgramLocalParameter4fEXT\0"
        "\0"
        /* _mesa_function_pool[52048]: NamedProgramLocalParameter4fvEXT (will be remapped) */
        "iiip\0"
        "glNamedProgramLocalParameter4fvEXT\0"
        "\0"
        /* _mesa_function_pool[52089]: GetNamedProgramLocalParameterfvEXT (will be remapped) */
        "iiip\0"
        "glGetNamedProgramLocalParameterfvEXT\0"
        "\0"
        /* _mesa_function_pool[52132]: NamedProgramLocalParameter4dEXT (will be remapped) */
        "iiidddd\0"
        "glNamedProgramLocalParameter4dEXT\0"
        "\0"
        /* _mesa_function_pool[52175]: NamedProgramLocalParameter4dvEXT (will be remapped) */
        "iiip\0"
        "glNamedProgramLocalParameter4dvEXT\0"
        "\0"
        /* _mesa_function_pool[52216]: GetNamedProgramLocalParameterdvEXT (will be remapped) */
        "iiip\0"
        "glGetNamedProgramLocalParameterdvEXT\0"
        "\0"
        /* _mesa_function_pool[52259]: GetNamedProgramivEXT (will be remapped) */
        "iiip\0"
        "glGetNamedProgramivEXT\0"
        "\0"
        /* _mesa_function_pool[52288]: TextureBufferEXT (will be remapped) */
        "iiii\0"
        "glTextureBufferEXT\0"
        "\0"
        /* _mesa_function_pool[52313]: MultiTexBufferEXT (will be remapped) */
        "iiii\0"
        "glMultiTexBufferEXT\0"
        "\0"
        /* _mesa_function_pool[52339]: TextureParameterIivEXT (will be remapped) */
        "iiip\0"
        "glTextureParameterIivEXT\0"
        "\0"
        /* _mesa_function_pool[52370]: TextureParameterIuivEXT (will be remapped) */
        "iiip\0"
        "glTextureParameterIuivEXT\0"
        "\0"
        /* _mesa_function_pool[52402]: GetTextureParameterIivEXT (will be remapped) */
        "iiip\0"
        "glGetTextureParameterIivEXT\0"
        "\0"
        /* _mesa_function_pool[52436]: GetTextureParameterIuivEXT (will be remapped) */
        "iiip\0"
        "glGetTextureParameterIuivEXT\0"
        "\0"
        /* _mesa_function_pool[52471]: MultiTexParameterIivEXT (will be remapped) */
        "iiip\0"
        "glMultiTexParameterIivEXT\0"
        "\0"
        /* _mesa_function_pool[52503]: MultiTexParameterIuivEXT (will be remapped) */
        "iiip\0"
        "glMultiTexParameterIuivEXT\0"
        "\0"
        /* _mesa_function_pool[52536]: GetMultiTexParameterIivEXT (will be remapped) */
        "iiip\0"
        "glGetMultiTexParameterIivEXT\0"
        "\0"
        /* _mesa_function_pool[52571]: GetMultiTexParameterIuivEXT (will be remapped) */
        "iiip\0"
        "glGetMultiTexParameterIuivEXT\0"
        "\0"
        /* _mesa_function_pool[52607]: NamedProgramLocalParameters4fvEXT (will be remapped) */
        "iiiip\0"
        "glNamedProgramLocalParameters4fvEXT\0"
        "\0"
        /* _mesa_function_pool[52650]: BindImageTextureEXT (will be remapped) */
        "iiiiiii\0"
        "glBindImageTextureEXT\0"
        "\0"
        /* _mesa_function_pool[52681]: SubpixelPrecisionBiasNV (will be remapped) */
        "ii\0"
        "glSubpixelPrecisionBiasNV\0"
        "\0"
        /* _mesa_function_pool[52711]: ConservativeRasterParameterfNV (will be remapped) */
        "if\0"
        "glConservativeRasterParameterfNV\0"
        "\0"
        /* _mesa_function_pool[52748]: ConservativeRasterParameteriNV (will be remapped) */
        "ii\0"
        "glConservativeRasterParameteriNV\0"
        "\0"
        /* _mesa_function_pool[52785]: GetFirstPerfQueryIdINTEL (will be remapped) */
        "p\0"
        "glGetFirstPerfQueryIdINTEL\0"
        "\0"
        /* _mesa_function_pool[52815]: GetNextPerfQueryIdINTEL (will be remapped) */
        "ip\0"
        "glGetNextPerfQueryIdINTEL\0"
        "\0"
        /* _mesa_function_pool[52845]: GetPerfQueryIdByNameINTEL (will be remapped) */
        "pp\0"
        "glGetPerfQueryIdByNameINTEL\0"
        "\0"
        /* _mesa_function_pool[52877]: GetPerfQueryInfoINTEL (will be remapped) */
        "iippppp\0"
        "glGetPerfQueryInfoINTEL\0"
        "\0"
        /* _mesa_function_pool[52910]: GetPerfCounterInfoINTEL (will be remapped) */
        "iiipipppppp\0"
        "glGetPerfCounterInfoINTEL\0"
        "\0"
        /* _mesa_function_pool[52949]: CreatePerfQueryINTEL (will be remapped) */
        "ip\0"
        "glCreatePerfQueryINTEL\0"
        "\0"
        /* _mesa_function_pool[52976]: DeletePerfQueryINTEL (will be remapped) */
        "i\0"
        "glDeletePerfQueryINTEL\0"
        "\0"
        /* _mesa_function_pool[53002]: BeginPerfQueryINTEL (will be remapped) */
        "i\0"
        "glBeginPerfQueryINTEL\0"
        "\0"
        /* _mesa_function_pool[53027]: EndPerfQueryINTEL (will be remapped) */
        "i\0"
        "glEndPerfQueryINTEL\0"
        "\0"
        /* _mesa_function_pool[53050]: GetPerfQueryDataINTEL (will be remapped) */
        "iiipp\0"
        "glGetPerfQueryDataINTEL\0"
        "\0"
        /* _mesa_function_pool[53081]: AlphaToCoverageDitherControlNV (will be remapped) */
        "i\0"
        "glAlphaToCoverageDitherControlNV\0"
        "\0"
        /* _mesa_function_pool[53117]: PolygonOffsetClampEXT (will be remapped) */
        "fff\0"
        "glPolygonOffsetClampEXT\0"
        "glPolygonOffsetClamp\0"
        "\0"
        /* _mesa_function_pool[53167]: WindowRectanglesEXT (will be remapped) */
        "iip\0"
        "glWindowRectanglesEXT\0"
        "\0"
        /* _mesa_function_pool[53194]: FramebufferFetchBarrierEXT (will be remapped) */
        "\0"
        "glFramebufferFetchBarrierEXT\0"
        "\0"
        /* _mesa_function_pool[53225]: RenderbufferStorageMultisampleAdvancedAMD (will be remapped) */
        "iiiiii\0"
        "glRenderbufferStorageMultisampleAdvancedAMD\0"
        "\0"
        /* _mesa_function_pool[53277]: NamedRenderbufferStorageMultisampleAdvancedAMD (will be remapped) */
        "iiiiii\0"
        "glNamedRenderbufferStorageMultisampleAdvancedAMD\0"
        "\0"
        /* _mesa_function_pool[53334]: StencilFuncSeparateATI (will be remapped) */
        "iiii\0"
        "glStencilFuncSeparateATI\0"
        "\0"
        /* _mesa_function_pool[53365]: ProgramEnvParameters4fvEXT (will be remapped) */
        "iiip\0"
        "glProgramEnvParameters4fvEXT\0"
        "\0"
        /* _mesa_function_pool[53400]: ProgramLocalParameters4fvEXT (will be remapped) */
        "iiip\0"
        "glProgramLocalParameters4fvEXT\0"
        "\0"
        /* _mesa_function_pool[53437]: IglooInterfaceSGIX (dynamic) */
        "ip\0"
        "glIglooInterfaceSGIX\0"
        "\0"
        /* _mesa_function_pool[53462]: DeformationMap3dSGIX (dynamic) */
        "iddiiddiiddiip\0"
        "glDeformationMap3dSGIX\0"
        "\0"
        /* _mesa_function_pool[53501]: DeformationMap3fSGIX (dynamic) */
        "iffiiffiiffiip\0"
        "glDeformationMap3fSGIX\0"
        "\0"
        /* _mesa_function_pool[53540]: DeformSGIX (dynamic) */
        "i\0"
        "glDeformSGIX\0"
        "\0"
        /* _mesa_function_pool[53556]: LoadIdentityDeformationMapSGIX (dynamic) */
        "i\0"
        "glLoadIdentityDeformationMapSGIX\0"
        "\0"
        /* _mesa_function_pool[53592]: InternalBufferSubDataCopyMESA (will be remapped) */
        "iiiiiii\0"
        "glInternalBufferSubDataCopyMESA\0"
        "\0"
        /* _mesa_function_pool[53633]: InternalSetError (will be remapped) */
        "i\0"
        "glInternalSetError\0"
        "\0"
        /* _mesa_function_pool[53655]: EGLImageTargetTexture2DOES (will be remapped) */
        "ip\0"
        "glEGLImageTargetTexture2DOES\0"
        "\0"
        /* _mesa_function_pool[53688]: EGLImageTargetRenderbufferStorageOES (will be remapped) */
        "ip\0"
        "glEGLImageTargetRenderbufferStorageOES\0"
        "\0"
        /* _mesa_function_pool[53731]: EGLImageTargetTexStorageEXT (will be remapped) */
        "ipp\0"
        "glEGLImageTargetTexStorageEXT\0"
        "\0"
        /* _mesa_function_pool[53766]: EGLImageTargetTextureStorageEXT (will be remapped) */
        "ipp\0"
        "glEGLImageTargetTextureStorageEXT\0"
        "\0"
        /* _mesa_function_pool[53805]: ClearColorIiEXT (will be remapped) */
        "iiii\0"
        "glClearColorIiEXT\0"
        "\0"
        /* _mesa_function_pool[53829]: ClearColorIuiEXT (will be remapped) */
        "iiii\0"
        "glClearColorIuiEXT\0"
        "\0"
        /* _mesa_function_pool[53854]: TexParameterIiv (will be remapped) */
        "iip\0"
        "glTexParameterIivEXT\0"
        "glTexParameterIiv\0"
        "\0"
        /* _mesa_function_pool[53898]: TexParameterIuiv (will be remapped) */
        "iip\0"
        "glTexParameterIuivEXT\0"
        "glTexParameterIuiv\0"
        "\0"
        /* _mesa_function_pool[53944]: GetTexParameterIiv (will be remapped) */
        "iip\0"
        "glGetTexParameterIivEXT\0"
        "glGetTexParameterIiv\0"
        "\0"
        /* _mesa_function_pool[53994]: GetTexParameterIuiv (will be remapped) */
        "iip\0"
        "glGetTexParameterIuivEXT\0"
        "glGetTexParameterIuiv\0"
        "\0"
        /* _mesa_function_pool[54046]: VertexAttribI1iEXT (will be remapped) */
        "ii\0"
        "glVertexAttribI1iEXT\0"
        "glVertexAttribI1i\0"
        "\0"
        /* _mesa_function_pool[54089]: VertexAttribI2iEXT (will be remapped) */
        "iii\0"
        "glVertexAttribI2iEXT\0"
        "glVertexAttribI2i\0"
        "\0"
        /* _mesa_function_pool[54133]: VertexAttribI3iEXT (will be remapped) */
        "iiii\0"
        "glVertexAttribI3iEXT\0"
        "glVertexAttribI3i\0"
        "\0"
        /* _mesa_function_pool[54178]: VertexAttribI4iEXT (will be remapped) */
        "iiiii\0"
        "glVertexAttribI4iEXT\0"
        "glVertexAttribI4i\0"
        "\0"
        /* _mesa_function_pool[54224]: VertexAttribI1uiEXT (will be remapped) */
        "ii\0"
        "glVertexAttribI1uiEXT\0"
        "glVertexAttribI1ui\0"
        "\0"
        /* _mesa_function_pool[54269]: VertexAttribI2uiEXT (will be remapped) */
        "iii\0"
        "glVertexAttribI2uiEXT\0"
        "glVertexAttribI2ui\0"
        "\0"
        /* _mesa_function_pool[54315]: VertexAttribI3uiEXT (will be remapped) */
        "iiii\0"
        "glVertexAttribI3uiEXT\0"
        "glVertexAttribI3ui\0"
        "\0"
        /* _mesa_function_pool[54362]: VertexAttribI4uiEXT (will be remapped) */
        "iiiii\0"
        "glVertexAttribI4uiEXT\0"
        "glVertexAttribI4ui\0"
        "\0"
        /* _mesa_function_pool[54410]: VertexAttribI1iv (will be remapped) */
        "ip\0"
        "glVertexAttribI1ivEXT\0"
        "glVertexAttribI1iv\0"
        "\0"
        /* _mesa_function_pool[54455]: VertexAttribI2ivEXT (will be remapped) */
        "ip\0"
        "glVertexAttribI2ivEXT\0"
        "glVertexAttribI2iv\0"
        "\0"
        /* _mesa_function_pool[54500]: VertexAttribI3ivEXT (will be remapped) */
        "ip\0"
        "glVertexAttribI3ivEXT\0"
        "glVertexAttribI3iv\0"
        "\0"
        /* _mesa_function_pool[54545]: VertexAttribI4ivEXT (will be remapped) */
        "ip\0"
        "glVertexAttribI4ivEXT\0"
        "glVertexAttribI4iv\0"
        "\0"
        /* _mesa_function_pool[54590]: VertexAttribI1uiv (will be remapped) */
        "ip\0"
        "glVertexAttribI1uivEXT\0"
        "glVertexAttribI1uiv\0"
        "\0"
        /* _mesa_function_pool[54637]: VertexAttribI2uivEXT (will be remapped) */
        "ip\0"
        "glVertexAttribI2uivEXT\0"
        "glVertexAttribI2uiv\0"
        "\0"
        /* _mesa_function_pool[54684]: VertexAttribI3uivEXT (will be remapped) */
        "ip\0"
        "glVertexAttribI3uivEXT\0"
        "glVertexAttribI3uiv\0"
        "\0"
        /* _mesa_function_pool[54731]: VertexAttribI4uivEXT (will be remapped) */
        "ip\0"
        "glVertexAttribI4uivEXT\0"
        "glVertexAttribI4uiv\0"
        "\0"
        /* _mesa_function_pool[54778]: VertexAttribI4bv (will be remapped) */
        "ip\0"
        "glVertexAttribI4bvEXT\0"
        "glVertexAttribI4bv\0"
        "\0"
        /* _mesa_function_pool[54823]: VertexAttribI4sv (will be remapped) */
        "ip\0"
        "glVertexAttribI4svEXT\0"
        "glVertexAttribI4sv\0"
        "\0"
        /* _mesa_function_pool[54868]: VertexAttribI4ubv (will be remapped) */
        "ip\0"
        "glVertexAttribI4ubvEXT\0"
        "glVertexAttribI4ubv\0"
        "\0"
        /* _mesa_function_pool[54915]: VertexAttribI4usv (will be remapped) */
        "ip\0"
        "glVertexAttribI4usvEXT\0"
        "glVertexAttribI4usv\0"
        "\0"
        /* _mesa_function_pool[54962]: VertexAttribIPointer (will be remapped) */
        "iiiip\0"
        "glVertexAttribIPointerEXT\0"
        "glVertexAttribIPointer\0"
        "\0"
        /* _mesa_function_pool[55018]: GetVertexAttribIiv (will be remapped) */
        "iip\0"
        "glGetVertexAttribIivEXT\0"
        "glGetVertexAttribIiv\0"
        "\0"
        /* _mesa_function_pool[55068]: GetVertexAttribIuiv (will be remapped) */
        "iip\0"
        "glGetVertexAttribIuivEXT\0"
        "glGetVertexAttribIuiv\0"
        "\0"
        /* _mesa_function_pool[55120]: Uniform1ui (will be remapped) */
        "ii\0"
        "glUniform1uiEXT\0"
        "glUniform1ui\0"
        "\0"
        /* _mesa_function_pool[55153]: Uniform2ui (will be remapped) */
        "iii\0"
        "glUniform2uiEXT\0"
        "glUniform2ui\0"
        "\0"
        /* _mesa_function_pool[55187]: Uniform3ui (will be remapped) */
        "iiii\0"
        "glUniform3uiEXT\0"
        "glUniform3ui\0"
        "\0"
        /* _mesa_function_pool[55222]: Uniform4ui (will be remapped) */
        "iiiii\0"
        "glUniform4uiEXT\0"
        "glUniform4ui\0"
        "\0"
        /* _mesa_function_pool[55258]: Uniform1uiv (will be remapped) */
        "iip\0"
        "glUniform1uivEXT\0"
        "glUniform1uiv\0"
        "\0"
        /* _mesa_function_pool[55294]: Uniform2uiv (will be remapped) */
        "iip\0"
        "glUniform2uivEXT\0"
        "glUniform2uiv\0"
        "\0"
        /* _mesa_function_pool[55330]: Uniform3uiv (will be remapped) */
        "iip\0"
        "glUniform3uivEXT\0"
        "glUniform3uiv\0"
        "\0"
        /* _mesa_function_pool[55366]: Uniform4uiv (will be remapped) */
        "iip\0"
        "glUniform4uivEXT\0"
        "glUniform4uiv\0"
        "\0"
        /* _mesa_function_pool[55402]: GetUniformuiv (will be remapped) */
        "iip\0"
        "glGetUniformuivEXT\0"
        "glGetUniformuiv\0"
        "\0"
        /* _mesa_function_pool[55442]: BindFragDataLocation (will be remapped) */
        "iip\0"
        "glBindFragDataLocationEXT\0"
        "glBindFragDataLocation\0"
        "\0"
        /* _mesa_function_pool[55496]: GetFragDataLocation (will be remapped) */
        "ip\0"
        "glGetFragDataLocationEXT\0"
        "glGetFragDataLocation\0"
        "\0"
        /* _mesa_function_pool[55547]: ClearBufferiv (will be remapped) */
        "iip\0"
        "glClearBufferiv\0"
        "\0"
        /* _mesa_function_pool[55568]: ClearBufferuiv (will be remapped) */
        "iip\0"
        "glClearBufferuiv\0"
        "\0"
        /* _mesa_function_pool[55590]: ClearBufferfv (will be remapped) */
        "iip\0"
        "glClearBufferfv\0"
        "\0"
        /* _mesa_function_pool[55611]: ClearBufferfi (will be remapped) */
        "iifi\0"
        "glClearBufferfi\0"
        "\0"
        /* _mesa_function_pool[55633]: GetStringi (will be remapped) */
        "ii\0"
        "glGetStringi\0"
        "\0"
        /* _mesa_function_pool[55650]: BeginTransformFeedback (will be remapped) */
        "i\0"
        "glBeginTransformFeedback\0"
        "glBeginTransformFeedbackEXT\0"
        "\0"
        /* _mesa_function_pool[55706]: EndTransformFeedback (will be remapped) */
        "\0"
        "glEndTransformFeedback\0"
        "glEndTransformFeedbackEXT\0"
        "\0"
        /* _mesa_function_pool[55757]: BindBufferRange (will be remapped) */
        "iiiii\0"
        "glBindBufferRange\0"
        "glBindBufferRangeEXT\0"
        "\0"
        /* _mesa_function_pool[55803]: BindBufferBase (will be remapped) */
        "iii\0"
        "glBindBufferBase\0"
        "glBindBufferBaseEXT\0"
        "\0"
        /* _mesa_function_pool[55845]: TransformFeedbackVaryings (will be remapped) */
        "iipi\0"
        "glTransformFeedbackVaryings\0"
        "glTransformFeedbackVaryingsEXT\0"
        "\0"
        /* _mesa_function_pool[55910]: GetTransformFeedbackVarying (will be remapped) */
        "iiipppp\0"
        "glGetTransformFeedbackVarying\0"
        "glGetTransformFeedbackVaryingEXT\0"
        "\0"
        /* _mesa_function_pool[55982]: BeginConditionalRender (will be remapped) */
        "ii\0"
        "glBeginConditionalRender\0"
        "glBeginConditionalRenderNV\0"
        "\0"
        /* _mesa_function_pool[56038]: EndConditionalRender (will be remapped) */
        "\0"
        "glEndConditionalRender\0"
        "glEndConditionalRenderNV\0"
        "\0"
        /* _mesa_function_pool[56088]: PrimitiveRestartIndex (will be remapped) */
        "i\0"
        "glPrimitiveRestartIndex\0"
        "glPrimitiveRestartIndexNV\0"
        "\0"
        /* _mesa_function_pool[56141]: GetInteger64i_v (will be remapped) */
        "iip\0"
        "glGetInteger64i_v\0"
        "\0"
        /* _mesa_function_pool[56164]: GetBufferParameteri64v (will be remapped) */
        "iip\0"
        "glGetBufferParameteri64v\0"
        "\0"
        /* _mesa_function_pool[56194]: FramebufferTexture (will be remapped) */
        "iiii\0"
        "glFramebufferTexture\0"
        "\0"
        /* _mesa_function_pool[56221]: PrimitiveRestartNV (will be remapped) */
        "\0"
        "glPrimitiveRestartNV\0"
        "\0"
        /* _mesa_function_pool[56244]: BindBufferOffsetEXT (will be remapped) */
        "iiii\0"
        "glBindBufferOffsetEXT\0"
        "\0"
        /* _mesa_function_pool[56272]: BindTransformFeedback (will be remapped) */
        "ii\0"
        "glBindTransformFeedback\0"
        "\0"
        /* _mesa_function_pool[56300]: DeleteTransformFeedbacks (will be remapped) */
        "ip\0"
        "glDeleteTransformFeedbacks\0"
        "\0"
        /* _mesa_function_pool[56331]: GenTransformFeedbacks (will be remapped) */
        "ip\0"
        "glGenTransformFeedbacks\0"
        "\0"
        /* _mesa_function_pool[56359]: IsTransformFeedback (will be remapped) */
        "i\0"
        "glIsTransformFeedback\0"
        "\0"
        /* _mesa_function_pool[56384]: PauseTransformFeedback (will be remapped) */
        "\0"
        "glPauseTransformFeedback\0"
        "\0"
        /* _mesa_function_pool[56411]: ResumeTransformFeedback (will be remapped) */
        "\0"
        "glResumeTransformFeedback\0"
        "\0"
        /* _mesa_function_pool[56439]: DrawTransformFeedback (will be remapped) */
        "ii\0"
        "glDrawTransformFeedback\0"
        "\0"
        /* _mesa_function_pool[56467]: VDPAUInitNV (will be remapped) */
        "pp\0"
        "glVDPAUInitNV\0"
        "\0"
        /* _mesa_function_pool[56485]: VDPAUFiniNV (will be remapped) */
        "\0"
        "glVDPAUFiniNV\0"
        "\0"
        /* _mesa_function_pool[56501]: VDPAURegisterVideoSurfaceNV (will be remapped) */
        "piip\0"
        "glVDPAURegisterVideoSurfaceNV\0"
        "\0"
        /* _mesa_function_pool[56537]: VDPAURegisterOutputSurfaceNV (will be remapped) */
        "piip\0"
        "glVDPAURegisterOutputSurfaceNV\0"
        "\0"
        /* _mesa_function_pool[56574]: VDPAUIsSurfaceNV (will be remapped) */
        "i\0"
        "glVDPAUIsSurfaceNV\0"
        "\0"
        /* _mesa_function_pool[56596]: VDPAUUnregisterSurfaceNV (will be remapped) */
        "i\0"
        "glVDPAUUnregisterSurfaceNV\0"
        "\0"
        /* _mesa_function_pool[56626]: VDPAUGetSurfaceivNV (will be remapped) */
        "iiipp\0"
        "glVDPAUGetSurfaceivNV\0"
        "\0"
        /* _mesa_function_pool[56655]: VDPAUSurfaceAccessNV (will be remapped) */
        "ii\0"
        "glVDPAUSurfaceAccessNV\0"
        "\0"
        /* _mesa_function_pool[56682]: VDPAUMapSurfacesNV (will be remapped) */
        "ip\0"
        "glVDPAUMapSurfacesNV\0"
        "\0"
        /* _mesa_function_pool[56707]: VDPAUUnmapSurfacesNV (will be remapped) */
        "ip\0"
        "glVDPAUUnmapSurfacesNV\0"
        "\0"
        /* _mesa_function_pool[56734]: GetUnsignedBytevEXT (will be remapped) */
        "ip\0"
        "glGetUnsignedBytevEXT\0"
        "\0"
        /* _mesa_function_pool[56760]: GetUnsignedBytei_vEXT (will be remapped) */
        "iip\0"
        "glGetUnsignedBytei_vEXT\0"
        "\0"
        /* _mesa_function_pool[56789]: DeleteMemoryObjectsEXT (will be remapped) */
        "ip\0"
        "glDeleteMemoryObjectsEXT\0"
        "\0"
        /* _mesa_function_pool[56818]: IsMemoryObjectEXT (will be remapped) */
        "i\0"
        "glIsMemoryObjectEXT\0"
        "\0"
        /* _mesa_function_pool[56841]: CreateMemoryObjectsEXT (will be remapped) */
        "ip\0"
        "glCreateMemoryObjectsEXT\0"
        "\0"
        /* _mesa_function_pool[56870]: MemoryObjectParameterivEXT (will be remapped) */
        "iip\0"
        "glMemoryObjectParameterivEXT\0"
        "\0"
        /* _mesa_function_pool[56904]: GetMemoryObjectParameterivEXT (will be remapped) */
        "iip\0"
        "glGetMemoryObjectParameterivEXT\0"
        "\0"
        /* _mesa_function_pool[56941]: TexStorageMem2DEXT (will be remapped) */
        "iiiiiii\0"
        "glTexStorageMem2DEXT\0"
        "\0"
        /* _mesa_function_pool[56971]: TexStorageMem2DMultisampleEXT (will be remapped) */
        "iiiiiiii\0"
        "glTexStorageMem2DMultisampleEXT\0"
        "\0"
        /* _mesa_function_pool[57013]: TexStorageMem3DEXT (will be remapped) */
        "iiiiiiii\0"
        "glTexStorageMem3DEXT\0"
        "\0"
        /* _mesa_function_pool[57044]: TexStorageMem3DMultisampleEXT (will be remapped) */
        "iiiiiiiii\0"
        "glTexStorageMem3DMultisampleEXT\0"
        "\0"
        /* _mesa_function_pool[57087]: BufferStorageMemEXT (will be remapped) */
        "iiii\0"
        "glBufferStorageMemEXT\0"
        "\0"
        /* _mesa_function_pool[57115]: TextureStorageMem2DEXT (will be remapped) */
        "iiiiiii\0"
        "glTextureStorageMem2DEXT\0"
        "\0"
        /* _mesa_function_pool[57149]: TextureStorageMem2DMultisampleEXT (will be remapped) */
        "iiiiiiii\0"
        "glTextureStorageMem2DMultisampleEXT\0"
        "\0"
        /* _mesa_function_pool[57195]: TextureStorageMem3DEXT (will be remapped) */
        "iiiiiiii\0"
        "glTextureStorageMem3DEXT\0"
        "\0"
        /* _mesa_function_pool[57230]: TextureStorageMem3DMultisampleEXT (will be remapped) */
        "iiiiiiiii\0"
        "glTextureStorageMem3DMultisampleEXT\0"
        "\0"
        /* _mesa_function_pool[57277]: NamedBufferStorageMemEXT (will be remapped) */
        "iiii\0"
        "glNamedBufferStorageMemEXT\0"
        "\0"
        /* _mesa_function_pool[57310]: TexStorageMem1DEXT (will be remapped) */
        "iiiiii\0"
        "glTexStorageMem1DEXT\0"
        "\0"
        /* _mesa_function_pool[57339]: TextureStorageMem1DEXT (will be remapped) */
        "iiiiii\0"
        "glTextureStorageMem1DEXT\0"
        "\0"
        /* _mesa_function_pool[57372]: GenSemaphoresEXT (will be remapped) */
        "ip\0"
        "glGenSemaphoresEXT\0"
        "\0"
        /* _mesa_function_pool[57395]: DeleteSemaphoresEXT (will be remapped) */
        "ip\0"
        "glDeleteSemaphoresEXT\0"
        "\0"
        /* _mesa_function_pool[57421]: IsSemaphoreEXT (will be remapped) */
        "i\0"
        "glIsSemaphoreEXT\0"
        "\0"
        /* _mesa_function_pool[57441]: SemaphoreParameterui64vEXT (will be remapped) */
        "iip\0"
        "glSemaphoreParameterui64vEXT\0"
        "\0"
        /* _mesa_function_pool[57475]: GetSemaphoreParameterui64vEXT (will be remapped) */
        "iip\0"
        "glGetSemaphoreParameterui64vEXT\0"
        "\0"
        /* _mesa_function_pool[57512]: WaitSemaphoreEXT (will be remapped) */
        "iipipp\0"
        "glWaitSemaphoreEXT\0"
        "\0"
        /* _mesa_function_pool[57539]: SignalSemaphoreEXT (will be remapped) */
        "iipipp\0"
        "glSignalSemaphoreEXT\0"
        "\0"
        /* _mesa_function_pool[57568]: ImportMemoryFdEXT (will be remapped) */
        "iiii\0"
        "glImportMemoryFdEXT\0"
        "\0"
        /* _mesa_function_pool[57594]: ImportSemaphoreFdEXT (will be remapped) */
        "iii\0"
        "glImportSemaphoreFdEXT\0"
        "\0"
        /* _mesa_function_pool[57622]: ViewportSwizzleNV (will be remapped) */
        "iiiii\0"
        "glViewportSwizzleNV\0"
        "\0"
        /* _mesa_function_pool[57649]: Vertex2hNV (will be remapped) */
        "dd\0"
        "glVertex2hNV\0"
        "\0"
        /* _mesa_function_pool[57666]: Vertex2hvNV (will be remapped) */
        "p\0"
        "glVertex2hvNV\0"
        "\0"
        /* _mesa_function_pool[57683]: Vertex3hNV (will be remapped) */
        "ddd\0"
        "glVertex3hNV\0"
        "\0"
        /* _mesa_function_pool[57701]: Vertex3hvNV (will be remapped) */
        "p\0"
        "glVertex3hvNV\0"
        "\0"
        /* _mesa_function_pool[57718]: Vertex4hNV (will be remapped) */
        "dddd\0"
        "glVertex4hNV\0"
        "\0"
        /* _mesa_function_pool[57737]: Vertex4hvNV (will be remapped) */
        "p\0"
        "glVertex4hvNV\0"
        "\0"
        /* _mesa_function_pool[57754]: Normal3hNV (will be remapped) */
        "ddd\0"
        "glNormal3hNV\0"
        "\0"
        /* _mesa_function_pool[57772]: Normal3hvNV (will be remapped) */
        "p\0"
        "glNormal3hvNV\0"
        "\0"
        /* _mesa_function_pool[57789]: Color3hNV (will be remapped) */
        "ddd\0"
        "glColor3hNV\0"
        "\0"
        /* _mesa_function_pool[57806]: Color3hvNV (will be remapped) */
        "p\0"
        "glColor3hvNV\0"
        "\0"
        /* _mesa_function_pool[57822]: Color4hNV (will be remapped) */
        "dddd\0"
        "glColor4hNV\0"
        "\0"
        /* _mesa_function_pool[57840]: Color4hvNV (will be remapped) */
        "p\0"
        "glColor4hvNV\0"
        "\0"
        /* _mesa_function_pool[57856]: TexCoord1hNV (will be remapped) */
        "d\0"
        "glTexCoord1hNV\0"
        "\0"
        /* _mesa_function_pool[57874]: TexCoord1hvNV (will be remapped) */
        "p\0"
        "glTexCoord1hvNV\0"
        "\0"
        /* _mesa_function_pool[57893]: TexCoord2hNV (will be remapped) */
        "dd\0"
        "glTexCoord2hNV\0"
        "\0"
        /* _mesa_function_pool[57912]: TexCoord2hvNV (will be remapped) */
        "p\0"
        "glTexCoord2hvNV\0"
        "\0"
        /* _mesa_function_pool[57931]: TexCoord3hNV (will be remapped) */
        "ddd\0"
        "glTexCoord3hNV\0"
        "\0"
        /* _mesa_function_pool[57951]: TexCoord3hvNV (will be remapped) */
        "p\0"
        "glTexCoord3hvNV\0"
        "\0"
        /* _mesa_function_pool[57970]: TexCoord4hNV (will be remapped) */
        "dddd\0"
        "glTexCoord4hNV\0"
        "\0"
        /* _mesa_function_pool[57991]: TexCoord4hvNV (will be remapped) */
        "p\0"
        "glTexCoord4hvNV\0"
        "\0"
        /* _mesa_function_pool[58010]: MultiTexCoord1hNV (will be remapped) */
        "id\0"
        "glMultiTexCoord1hNV\0"
        "\0"
        /* _mesa_function_pool[58034]: MultiTexCoord1hvNV (will be remapped) */
        "ip\0"
        "glMultiTexCoord1hvNV\0"
        "\0"
        /* _mesa_function_pool[58059]: MultiTexCoord2hNV (will be remapped) */
        "idd\0"
        "glMultiTexCoord2hNV\0"
        "\0"
        /* _mesa_function_pool[58084]: MultiTexCoord2hvNV (will be remapped) */
        "ip\0"
        "glMultiTexCoord2hvNV\0"
        "\0"
        /* _mesa_function_pool[58109]: MultiTexCoord3hNV (will be remapped) */
        "iddd\0"
        "glMultiTexCoord3hNV\0"
        "\0"
        /* _mesa_function_pool[58135]: MultiTexCoord3hvNV (will be remapped) */
        "ip\0"
        "glMultiTexCoord3hvNV\0"
        "\0"
        /* _mesa_function_pool[58160]: MultiTexCoord4hNV (will be remapped) */
        "idddd\0"
        "glMultiTexCoord4hNV\0"
        "\0"
        /* _mesa_function_pool[58187]: MultiTexCoord4hvNV (will be remapped) */
        "ip\0"
        "glMultiTexCoord4hvNV\0"
        "\0"
        /* _mesa_function_pool[58212]: VertexAttrib1hNV (will be remapped) */
        "id\0"
        "glVertexAttrib1hNV\0"
        "\0"
        /* _mesa_function_pool[58235]: VertexAttrib1hvNV (will be remapped) */
        "ip\0"
        "glVertexAttrib1hvNV\0"
        "\0"
        /* _mesa_function_pool[58259]: VertexAttrib2hNV (will be remapped) */
        "idd\0"
        "glVertexAttrib2hNV\0"
        "\0"
        /* _mesa_function_pool[58283]: VertexAttrib2hvNV (will be remapped) */
        "ip\0"
        "glVertexAttrib2hvNV\0"
        "\0"
        /* _mesa_function_pool[58307]: VertexAttrib3hNV (will be remapped) */
        "iddd\0"
        "glVertexAttrib3hNV\0"
        "\0"
        /* _mesa_function_pool[58332]: VertexAttrib3hvNV (will be remapped) */
        "ip\0"
        "glVertexAttrib3hvNV\0"
        "\0"
        /* _mesa_function_pool[58356]: VertexAttrib4hNV (will be remapped) */
        "idddd\0"
        "glVertexAttrib4hNV\0"
        "\0"
        /* _mesa_function_pool[58382]: VertexAttrib4hvNV (will be remapped) */
        "ip\0"
        "glVertexAttrib4hvNV\0"
        "\0"
        /* _mesa_function_pool[58406]: VertexAttribs1hvNV (will be remapped) */
        "iip\0"
        "glVertexAttribs1hvNV\0"
        "\0"
        /* _mesa_function_pool[58432]: VertexAttribs2hvNV (will be remapped) */
        "iip\0"
        "glVertexAttribs2hvNV\0"
        "\0"
        /* _mesa_function_pool[58458]: VertexAttribs3hvNV (will be remapped) */
        "iip\0"
        "glVertexAttribs3hvNV\0"
        "\0"
        /* _mesa_function_pool[58484]: VertexAttribs4hvNV (will be remapped) */
        "iip\0"
        "glVertexAttribs4hvNV\0"
        "\0"
        /* _mesa_function_pool[58510]: FogCoordhNV (will be remapped) */
        "d\0"
        "glFogCoordhNV\0"
        "\0"
        /* _mesa_function_pool[58527]: FogCoordhvNV (will be remapped) */
        "p\0"
        "glFogCoordhvNV\0"
        "\0"
        /* _mesa_function_pool[58545]: SecondaryColor3hNV (will be remapped) */
        "ddd\0"
        "glSecondaryColor3hNV\0"
        "\0"
        /* _mesa_function_pool[58571]: SecondaryColor3hvNV (will be remapped) */
        "p\0"
        "glSecondaryColor3hvNV\0"
        "\0"
        /* _mesa_function_pool[58596]: MemoryBarrierByRegion (will be remapped) */
        "i\0"
        "glMemoryBarrierByRegion\0"
        "\0"
;

/* these functions need to be remapped */
static const struct gl_function_pool_remap MESA_remap_table_functions[] = {
        {  9920, CompressedTexImage1D_remap_index },
        {  9861, CompressedTexImage2D_remap_index },
        {  9801, CompressedTexImage3D_remap_index },
        { 10112, CompressedTexSubImage1D_remap_index },
        { 10046, CompressedTexSubImage2D_remap_index },
        {  9978, CompressedTexSubImage3D_remap_index },
        { 10176, GetCompressedTexImage_remap_index },
        {  9604, LoadTransposeMatrixd_remap_index },
        {  9552, LoadTransposeMatrixf_remap_index },
        {  9708, MultTransposeMatrixd_remap_index },
        {  9656, MultTransposeMatrixf_remap_index },
        {  9760, SampleCoverage_remap_index },
        { 10232, BlendFuncSeparate_remap_index },
        { 10429, FogCoordPointer_remap_index },
        { 10367, FogCoordd_remap_index },
        { 10397, FogCoorddv_remap_index },
        { 10473, MultiDrawArrays_remap_index },
        { 10568, PointParameterf_remap_index },
        { 10654, PointParameterfv_remap_index },
        { 10744, PointParameteri_remap_index },
        { 10786, PointParameteriv_remap_index },
        { 10830, SecondaryColor3b_remap_index },
        { 10876, SecondaryColor3bv_remap_index },
        { 10922, SecondaryColor3d_remap_index },
        { 10968, SecondaryColor3dv_remap_index },
        { 11106, SecondaryColor3i_remap_index },
        { 11152, SecondaryColor3iv_remap_index },
        { 11198, SecondaryColor3s_remap_index },
        { 11244, SecondaryColor3sv_remap_index },
        { 11290, SecondaryColor3ub_remap_index },
        { 11338, SecondaryColor3ubv_remap_index },
        { 11386, SecondaryColor3ui_remap_index },
        { 11434, SecondaryColor3uiv_remap_index },
        { 11482, SecondaryColor3us_remap_index },
        { 11530, SecondaryColor3usv_remap_index },
        { 11578, SecondaryColorPointer_remap_index },
        { 11635, WindowPos2d_remap_index },
        { 11688, WindowPos2dv_remap_index },
        { 11743, WindowPos2f_remap_index },
        { 11796, WindowPos2fv_remap_index },
        { 11851, WindowPos2i_remap_index },
        { 11904, WindowPos2iv_remap_index },
        { 11959, WindowPos2s_remap_index },
        { 12012, WindowPos2sv_remap_index },
        { 12067, WindowPos3d_remap_index },
        { 12121, WindowPos3dv_remap_index },
        { 12176, WindowPos3f_remap_index },
        { 12230, WindowPos3fv_remap_index },
        { 12285, WindowPos3i_remap_index },
        { 12339, WindowPos3iv_remap_index },
        { 12394, WindowPos3s_remap_index },
        { 12448, WindowPos3sv_remap_index },
        { 13024, BeginQuery_remap_index },
        { 12503, BindBuffer_remap_index },
        { 12536, BufferData_remap_index },
        { 12571, BufferSubData_remap_index },
        { 12612, DeleteBuffers_remap_index },
        { 12959, DeleteQueries_remap_index },
        { 13057, EndQuery_remap_index },
        { 12651, GenBuffers_remap_index },
        { 12926, GenQueries_remap_index },
        { 12684, GetBufferParameteriv_remap_index },
        { 12738, GetBufferPointerv_remap_index },
        { 12786, GetBufferSubData_remap_index },
        { 13119, GetQueryObjectiv_remap_index },
        { 13165, GetQueryObjectuiv_remap_index },
        { 13085, GetQueryiv_remap_index },
        { 12833, IsBuffer_remap_index },
        { 12998, IsQuery_remap_index },
        { 12861, MapBuffer_remap_index },
        { 12892, UnmapBuffer_remap_index },
        { 13450, AttachShader_remap_index },
        { 13469, BindAttribLocation_remap_index },
        { 13213, BlendEquationSeparate_remap_index },
        { 13519, CompileShader_remap_index },
        { 13557, CreateProgram_remap_index },
        { 13575, CreateShader_remap_index },
        { 13593, DeleteProgram_remap_index },
        { 13612, DeleteShader_remap_index },
        { 13630, DetachShader_remap_index },
        { 13649, DisableVertexAttribArray_remap_index },
        { 13295, DrawBuffers_remap_index },
        { 13709, EnableVertexAttribArray_remap_index },
        { 13767, GetActiveAttrib_remap_index },
        { 13815, GetActiveUniform_remap_index },
        { 13865, GetAttachedShaders_remap_index },
        { 13892, GetAttribLocation_remap_index },
        { 13959, GetProgramInfoLog_remap_index },
        { 13939, GetProgramiv_remap_index },
        { 14004, GetShaderInfoLog_remap_index },
        { 14029, GetShaderSource_remap_index },
        { 13985, GetShaderiv_remap_index },
        { 14074, GetUniformLocation_remap_index },
        { 14123, GetUniformfv_remap_index },
        { 14161, GetUniformiv_remap_index },
        { 14343, GetVertexAttribPointerv_remap_index },
        { 14199, GetVertexAttribdv_remap_index },
        { 14247, GetVertexAttribfv_remap_index },
        { 14295, GetVertexAttribiv_remap_index },
        { 14431, IsProgram_remap_index },
        { 14446, IsShader_remap_index },
        { 14460, LinkProgram_remap_index },
        { 14494, ShaderSource_remap_index },
        { 13347, StencilFuncSeparate_remap_index },
        { 13424, StencilMaskSeparate_remap_index },
        { 13375, StencilOpSeparate_remap_index },
        { 14571, Uniform1f_remap_index },
        { 14831, Uniform1fv_remap_index },
        { 14701, Uniform1i_remap_index },
        { 14967, Uniform1iv_remap_index },
        { 14602, Uniform2f_remap_index },
        { 14865, Uniform2fv_remap_index },
        { 14732, Uniform2i_remap_index },
        { 15001, Uniform2iv_remap_index },
        { 14634, Uniform3f_remap_index },
        { 14899, Uniform3fv_remap_index },
        { 14764, Uniform3i_remap_index },
        { 15035, Uniform3iv_remap_index },
        { 14667, Uniform4f_remap_index },
        { 14933, Uniform4fv_remap_index },
        { 14797, Uniform4i_remap_index },
        { 15069, Uniform4iv_remap_index },
        { 15103, UniformMatrix2fv_remap_index },
        { 15150, UniformMatrix3fv_remap_index },
        { 15197, UniformMatrix4fv_remap_index },
        { 14533, UseProgram_remap_index },
        { 15244, ValidateProgram_remap_index },
        { 15286, VertexAttrib1d_remap_index },
        { 15327, VertexAttrib1dv_remap_index },
        { 15454, VertexAttrib1s_remap_index },
        { 15495, VertexAttrib1sv_remap_index },
        { 15538, VertexAttrib2d_remap_index },
        { 15580, VertexAttrib2dv_remap_index },
        { 15708, VertexAttrib2s_remap_index },
        { 15750, VertexAttrib2sv_remap_index },
        { 15793, VertexAttrib3d_remap_index },
        { 15836, VertexAttrib3dv_remap_index },
        { 15965, VertexAttrib3s_remap_index },
        { 16008, VertexAttrib3sv_remap_index },
        { 16051, VertexAttrib4Nbv_remap_index },
        { 16096, VertexAttrib4Niv_remap_index },
        { 16141, VertexAttrib4Nsv_remap_index },
        { 16186, VertexAttrib4Nub_remap_index },
        { 16234, VertexAttrib4Nubv_remap_index },
        { 16281, VertexAttrib4Nuiv_remap_index },
        { 16328, VertexAttrib4Nusv_remap_index },
        { 16375, VertexAttrib4bv_remap_index },
        { 16418, VertexAttrib4d_remap_index },
        { 16462, VertexAttrib4dv_remap_index },
        { 16592, VertexAttrib4iv_remap_index },
        { 16635, VertexAttrib4s_remap_index },
        { 16679, VertexAttrib4sv_remap_index },
        { 16722, VertexAttrib4ubv_remap_index },
        { 16767, VertexAttrib4uiv_remap_index },
        { 16812, VertexAttrib4usv_remap_index },
        { 16857, VertexAttribPointer_remap_index },
        { 16912, UniformMatrix2x3fv_remap_index },
        { 16966, UniformMatrix2x4fv_remap_index },
        { 16939, UniformMatrix3x2fv_remap_index },
        { 17020, UniformMatrix3x4fv_remap_index },
        { 16993, UniformMatrix4x2fv_remap_index },
        { 17047, UniformMatrix4x3fv_remap_index },
        { 55982, BeginConditionalRender_remap_index },
        { 55650, BeginTransformFeedback_remap_index },
        { 55803, BindBufferBase_remap_index },
        { 55757, BindBufferRange_remap_index },
        { 55442, BindFragDataLocation_remap_index },
        { 18390, ClampColor_remap_index },
        { 55611, ClearBufferfi_remap_index },
        { 55590, ClearBufferfv_remap_index },
        { 55547, ClearBufferiv_remap_index },
        { 55568, ClearBufferuiv_remap_index },
        { 46442, ColorMaski_remap_index },
        { 46607, Disablei_remap_index },
        { 46574, Enablei_remap_index },
        { 56038, EndConditionalRender_remap_index },
        { 55706, EndTransformFeedback_remap_index },
        { 46484, GetBooleani_v_remap_index },
        { 55496, GetFragDataLocation_remap_index },
        { 46529, GetIntegeri_v_remap_index },
        { 55633, GetStringi_remap_index },
        { 53944, GetTexParameterIiv_remap_index },
        { 53994, GetTexParameterIuiv_remap_index },
        { 55910, GetTransformFeedbackVarying_remap_index },
        { 55402, GetUniformuiv_remap_index },
        { 55018, GetVertexAttribIiv_remap_index },
        { 55068, GetVertexAttribIuiv_remap_index },
        { 46642, IsEnabledi_remap_index },
        { 53854, TexParameterIiv_remap_index },
        { 53898, TexParameterIuiv_remap_index },
        { 55845, TransformFeedbackVaryings_remap_index },
        { 55120, Uniform1ui_remap_index },
        { 55258, Uniform1uiv_remap_index },
        { 55153, Uniform2ui_remap_index },
        { 55294, Uniform2uiv_remap_index },
        { 55187, Uniform3ui_remap_index },
        { 55330, Uniform3uiv_remap_index },
        { 55222, Uniform4ui_remap_index },
        { 55366, Uniform4uiv_remap_index },
        { 54410, VertexAttribI1iv_remap_index },
        { 54590, VertexAttribI1uiv_remap_index },
        { 54778, VertexAttribI4bv_remap_index },
        { 54823, VertexAttribI4sv_remap_index },
        { 54868, VertexAttribI4ubv_remap_index },
        { 54915, VertexAttribI4usv_remap_index },
        { 54962, VertexAttribIPointer_remap_index },
        { 56088, PrimitiveRestartIndex_remap_index },
        { 19761, TexBuffer_remap_index },
        { 56194, FramebufferTexture_remap_index },
        { 56164, GetBufferParameteri64v_remap_index },
        { 56141, GetInteger64i_v_remap_index },
        { 19616, VertexAttribDivisor_remap_index },
        { 20789, MinSampleShading_remap_index },
        { 58596, MemoryBarrierByRegion_remap_index },
        { 17422, BindProgramARB_remap_index },
        { 17459, DeleteProgramsARB_remap_index },
        { 17502, GenProgramsARB_remap_index },
        { 17945, GetProgramEnvParameterdvARB_remap_index },
        { 17980, GetProgramEnvParameterfvARB_remap_index },
        { 18015, GetProgramLocalParameterdvARB_remap_index },
        { 18052, GetProgramLocalParameterfvARB_remap_index },
        { 18112, GetProgramStringARB_remap_index },
        { 18089, GetProgramivARB_remap_index },
        { 17539, IsProgramARB_remap_index },
        { 17571, ProgramEnvParameter4dARB_remap_index },
        { 17629, ProgramEnvParameter4dvARB_remap_index },
        { 17686, ProgramEnvParameter4fARB_remap_index },
        { 17744, ProgramEnvParameter4fvARB_remap_index },
        { 17801, ProgramLocalParameter4dARB_remap_index },
        { 17838, ProgramLocalParameter4dvARB_remap_index },
        { 17873, ProgramLocalParameter4fARB_remap_index },
        { 17910, ProgramLocalParameter4fvARB_remap_index },
        { 17397, ProgramStringARB_remap_index },
        { 15370, VertexAttrib1fARB_remap_index },
        { 15411, VertexAttrib1fvARB_remap_index },
        { 15623, VertexAttrib2fARB_remap_index },
        { 15665, VertexAttrib2fvARB_remap_index },
        { 15879, VertexAttrib3fARB_remap_index },
        { 15922, VertexAttrib3fvARB_remap_index },
        { 16505, VertexAttrib4fARB_remap_index },
        { 16549, VertexAttrib4fvARB_remap_index },
        { 18254, AttachObjectARB_remap_index },
        { 18227, CreateProgramObjectARB_remap_index },
        { 18200, CreateShaderObjectARB_remap_index },
        { 18139, DeleteObjectARB_remap_index },
        { 18178, DetachObjectARB_remap_index },
        { 18360, GetAttachedObjectsARB_remap_index },
        { 18160, GetHandleARB_remap_index },
        { 18338, GetInfoLogARB_remap_index },
        { 18276, GetObjectParameterfvARB_remap_index },
        { 18307, GetObjectParameterivARB_remap_index },
        { 18423, DrawArraysInstancedARB_remap_index },
        { 18501, DrawElementsInstancedARB_remap_index },
        { 18978, BindFramebuffer_remap_index },
        { 18626, BindRenderbuffer_remap_index },
        { 19525, BlitFramebuffer_remap_index },
        { 19092, CheckFramebufferStatus_remap_index },
        { 19000, DeleteFramebuffers_remap_index },
        { 18649, DeleteRenderbuffers_remap_index },
        { 19379, FramebufferRenderbuffer_remap_index },
        { 19148, FramebufferTexture1D_remap_index },
        { 19204, FramebufferTexture2D_remap_index },
        { 19260, FramebufferTexture3D_remap_index },
        { 19317, FramebufferTextureLayer_remap_index },
        { 19049, GenFramebuffers_remap_index },
        { 18700, GenRenderbuffers_remap_index },
        { 19576, GenerateMipmap_remap_index },
        { 19440, GetFramebufferAttachmentParameteriv_remap_index },
        { 18874, GetRenderbufferParameteriv_remap_index },
        { 18940, IsFramebuffer_remap_index },
        { 18586, IsRenderbuffer_remap_index },
        { 18745, RenderbufferStorage_remap_index },
        { 18798, RenderbufferStorageMultisample_remap_index },
        { 19731, FlushMappedBufferRange_remap_index },
        { 19708, MapBufferRange_remap_index },
        { 19793, BindVertexArray_remap_index },
        { 19814, DeleteVertexArrays_remap_index },
        { 19839, GenVertexArrays_remap_index },
        { 19861, IsVertexArray_remap_index },
        { 20024, GetActiveUniformBlockName_remap_index },
        { 19992, GetActiveUniformBlockiv_remap_index },
        { 19935, GetActiveUniformName_remap_index },
        { 19906, GetActiveUniformsiv_remap_index },
        { 19965, GetUniformBlockIndex_remap_index },
        { 19880, GetUniformIndices_remap_index },
        { 20059, UniformBlockBinding_remap_index },
        { 20086, CopyBufferSubData_remap_index },
        { 20308, ClientWaitSync_remap_index },
        { 20292, DeleteSync_remap_index },
        { 20264, FenceSync_remap_index },
        { 20346, GetInteger64v_remap_index },
        { 20366, GetSynciv_remap_index },
        { 20280, IsSync_remap_index },
        { 20330, WaitSync_remap_index },
        { 20113, DrawElementsBaseVertex_remap_index },
        { 20222, DrawElementsInstancedBaseVertex_remap_index },
        { 20145, DrawRangeElementsBaseVertex_remap_index },
        { 20184, MultiDrawElementsBaseVertex_remap_index },
        { 46400, ProvokingVertex_remap_index },
        { 20450, GetMultisamplefv_remap_index },
        { 20474, SampleMaski_remap_index },
        { 20385, TexImage2DMultisample_remap_index },
        { 20417, TexImage3DMultisample_remap_index },
        { 20559, BlendEquationSeparateiARB_remap_index },
        { 20492, BlendEquationiARB_remap_index },
        { 20707, BlendFuncSeparateiARB_remap_index },
        { 20651, BlendFunciARB_remap_index },
        { 20994, BindFragDataLocationIndexed_remap_index },
        { 21030, GetFragDataIndex_remap_index },
        { 21107, BindSampler_remap_index },
        { 21071, DeleteSamplers_remap_index },
        { 21053, GenSamplers_remap_index },
        { 21340, GetSamplerParameterIiv_remap_index },
        { 21370, GetSamplerParameterIuiv_remap_index },
        { 21311, GetSamplerParameterfv_remap_index },
        { 21282, GetSamplerParameteriv_remap_index },
        { 21092, IsSampler_remap_index },
        { 21227, SamplerParameterIiv_remap_index },
        { 21254, SamplerParameterIuiv_remap_index },
        { 21150, SamplerParameterf_remap_index },
        { 21201, SamplerParameterfv_remap_index },
        { 21125, SamplerParameteri_remap_index },
        { 21175, SamplerParameteriv_remap_index },
        { 21401, GetQueryObjecti64v_remap_index },
        { 21451, GetQueryObjectui64v_remap_index },
        { 21503, QueryCounter_remap_index },
        { 22022, ColorP3ui_remap_index },
        { 22054, ColorP3uiv_remap_index },
        { 22038, ColorP4ui_remap_index },
        { 22071, ColorP4uiv_remap_index },
        { 21783, MultiTexCoordP1ui_remap_index },
        { 21883, MultiTexCoordP1uiv_remap_index },
        { 21808, MultiTexCoordP2ui_remap_index },
        { 21909, MultiTexCoordP2uiv_remap_index },
        { 21833, MultiTexCoordP3ui_remap_index },
        { 21935, MultiTexCoordP3uiv_remap_index },
        { 21858, MultiTexCoordP4ui_remap_index },
        { 21961, MultiTexCoordP4uiv_remap_index },
        { 21987, NormalP3ui_remap_index },
        { 22004, NormalP3uiv_remap_index },
        { 22088, SecondaryColorP3ui_remap_index },
        { 22113, SecondaryColorP3uiv_remap_index },
        { 21627, TexCoordP1ui_remap_index },
        { 21703, TexCoordP1uiv_remap_index },
        { 21646, TexCoordP2ui_remap_index },
        { 21723, TexCoordP2uiv_remap_index },
        { 21665, TexCoordP3ui_remap_index },
        { 21743, TexCoordP3uiv_remap_index },
        { 21684, TexCoordP4ui_remap_index },
        { 21763, TexCoordP4uiv_remap_index },
        { 22139, VertexAttribP1ui_remap_index },
        { 22239, VertexAttribP1uiv_remap_index },
        { 22164, VertexAttribP2ui_remap_index },
        { 22265, VertexAttribP2uiv_remap_index },
        { 22189, VertexAttribP3ui_remap_index },
        { 22291, VertexAttribP3uiv_remap_index },
        { 22214, VertexAttribP4ui_remap_index },
        { 22317, VertexAttribP4uiv_remap_index },
        { 21522, VertexP2ui_remap_index },
        { 21573, VertexP2uiv_remap_index },
        { 21539, VertexP3ui_remap_index },
        { 21591, VertexP3uiv_remap_index },
        { 21556, VertexP4ui_remap_index },
        { 21609, VertexP4uiv_remap_index },
        { 22691, DrawArraysIndirect_remap_index },
        { 22716, DrawElementsIndirect_remap_index },
        { 23250, GetUniformdv_remap_index },
        { 22871, Uniform1d_remap_index },
        { 22941, Uniform1dv_remap_index },
        { 22887, Uniform2d_remap_index },
        { 22959, Uniform2dv_remap_index },
        { 22904, Uniform3d_remap_index },
        { 22977, Uniform3dv_remap_index },
        { 22922, Uniform4d_remap_index },
        { 22995, Uniform4dv_remap_index },
        { 23013, UniformMatrix2dv_remap_index },
        { 23088, UniformMatrix2x3dv_remap_index },
        { 23115, UniformMatrix2x4dv_remap_index },
        { 23038, UniformMatrix3dv_remap_index },
        { 23142, UniformMatrix3x2dv_remap_index },
        { 23169, UniformMatrix3x4dv_remap_index },
        { 23063, UniformMatrix4dv_remap_index },
        { 23196, UniformMatrix4x2dv_remap_index },
        { 23223, UniformMatrix4x3dv_remap_index },
        { 22484, GetActiveSubroutineName_remap_index },
        { 22443, GetActiveSubroutineUniformName_remap_index },
        { 22405, GetActiveSubroutineUniformiv_remap_index },
        { 22578, GetProgramStageiv_remap_index },
        { 22379, GetSubroutineIndex_remap_index },
        { 22343, GetSubroutineUniformLocation_remap_index },
        { 22547, GetUniformSubroutineuiv_remap_index },
        { 22518, UniformSubroutinesuiv_remap_index },
        { 22668, PatchParameterfv_remap_index },
        { 22604, PatchParameteri_remap_index },
        { 56272, BindTransformFeedback_remap_index },
        { 56300, DeleteTransformFeedbacks_remap_index },
        { 56439, DrawTransformFeedback_remap_index },
        { 56331, GenTransformFeedbacks_remap_index },
        { 56359, IsTransformFeedback_remap_index },
        { 56384, PauseTransformFeedback_remap_index },
        { 56411, ResumeTransformFeedback_remap_index },
        { 24273, BeginQueryIndexed_remap_index },
        { 24238, DrawTransformFeedbackStream_remap_index },
        { 24298, EndQueryIndexed_remap_index },
        { 24320, GetQueryIndexediv_remap_index },
        { 27224, ClearDepthf_remap_index },
        { 27241, DepthRangef_remap_index },
        { 27143, GetShaderPrecisionFormat_remap_index },
        { 27176, ReleaseShaderCompiler_remap_index },
        { 27202, ShaderBinary_remap_index },
        { 27259, GetProgramBinary_remap_index },
        { 27285, ProgramBinary_remap_index },
        { 27307, ProgramParameteri_remap_index },
        { 27048, GetVertexAttribLdv_remap_index },
        { 26634, VertexAttribL1d_remap_index },
        { 26812, VertexAttribL1dv_remap_index },
        { 26677, VertexAttribL2d_remap_index },
        { 26857, VertexAttribL2dv_remap_index },
        { 26721, VertexAttribL3d_remap_index },
        { 26902, VertexAttribL3dv_remap_index },
        { 26766, VertexAttribL4d_remap_index },
        { 26947, VertexAttribL4dv_remap_index },
        { 26992, VertexAttribLPointer_remap_index },
        { 38345, DepthRangeArrayv_remap_index },
        { 38369, DepthRangeIndexed_remap_index },
        { 38452, GetDoublei_v_remap_index },
        { 38394, GetFloati_v_remap_index },
        { 38278, ScissorArrayv_remap_index },
        { 38299, ScissorIndexed_remap_index },
        { 38323, ScissorIndexedv_remap_index },
        { 38206, ViewportArrayv_remap_index },
        { 38228, ViewportIndexedf_remap_index },
        { 38254, ViewportIndexedfv_remap_index },
        { 27670, GetGraphicsResetStatusARB_remap_index },
        { 27960, GetnColorTableARB_remap_index },
        { 28107, GetnCompressedTexImageARB_remap_index },
        { 27987, GetnConvolutionFilterARB_remap_index },
        { 28056, GetnHistogramARB_remap_index },
        { 27725, GetnMapdvARB_remap_index },
        { 27746, GetnMapfvARB_remap_index },
        { 27767, GetnMapivARB_remap_index },
        { 28083, GetnMinmaxARB_remap_index },
        { 27788, GetnPixelMapfvARB_remap_index },
        { 27813, GetnPixelMapuivARB_remap_index },
        { 27839, GetnPixelMapusvARB_remap_index },
        { 27865, GetnPolygonStippleARB_remap_index },
        { 28021, GetnSeparableFilterARB_remap_index },
        { 27893, GetnTexImageARB_remap_index },
        { 28266, GetnUniformdvARB_remap_index },
        { 28141, GetnUniformfvARB_remap_index },
        { 28182, GetnUniformivARB_remap_index },
        { 28223, GetnUniformuivARB_remap_index },
        { 27919, ReadnPixelsARB_remap_index },
        { 28291, DrawArraysInstancedBaseInstance_remap_index },
        { 28332, DrawElementsInstancedBaseInstance_remap_index },
        { 28376, DrawElementsInstancedBaseVertexBaseInstance_remap_index },
        { 28431, DrawTransformFeedbackInstanced_remap_index },
        { 28469, DrawTransformFeedbackStreamInstanced_remap_index },
        { 28514, GetInternalformativ_remap_index },
        { 28543, GetActiveAtomicCounterBufferiv_remap_index },
        { 28582, BindImageTexture_remap_index },
        { 28610, MemoryBarrier_remap_index },
        { 28648, TexStorage1D_remap_index },
        { 28669, TexStorage2D_remap_index },
        { 28691, TexStorage3D_remap_index },
        { 28714, TextureStorage1DEXT_remap_index },
        { 28743, TextureStorage2DEXT_remap_index },
        { 28773, TextureStorage3DEXT_remap_index },
        { 29056, ClearBufferData_remap_index },
        { 29081, ClearBufferSubData_remap_index },
        { 29182, DispatchCompute_remap_index },
        { 29205, DispatchComputeIndirect_remap_index },
        { 29234, CopyImageSubData_remap_index },
        { 29270, TextureView_remap_index },
        { 29294, BindVertexBuffer_remap_index },
        { 29403, VertexAttribBinding_remap_index },
        { 29319, VertexAttribFormat_remap_index },
        { 29347, VertexAttribIFormat_remap_index },
        { 29375, VertexAttribLFormat_remap_index },
        { 29429, VertexBindingDivisor_remap_index },
        { 29708, FramebufferParameteri_remap_index },
        { 29737, GetFramebufferParameteriv_remap_index },
        { 29848, GetInternalformati64v_remap_index },
        { 22744, MultiDrawArraysIndirect_remap_index },
        { 22805, MultiDrawElementsIndirect_remap_index },
        { 30060, GetProgramInterfaceiv_remap_index },
        { 30090, GetProgramResourceIndex_remap_index },
        { 30187, GetProgramResourceLocation_remap_index },
        { 30221, GetProgramResourceLocationIndex_remap_index },
        { 30121, GetProgramResourceName_remap_index },
        { 30154, GetProgramResourceiv_remap_index },
        { 30260, ShaderStorageBlockBinding_remap_index },
        { 30293, TexBufferRange_remap_index },
        { 30349, TexStorage2DMultisample_remap_index },
        { 30383, TexStorage3DMultisample_remap_index },
        { 30503, BufferStorage_remap_index },
        { 30555, ClearTexImage_remap_index },
        { 30578, ClearTexSubImage_remap_index },
        { 30610, BindBuffersBase_remap_index },
        { 30634, BindBuffersRange_remap_index },
        { 30701, BindImageTextures_remap_index },
        { 30681, BindSamplers_remap_index },
        { 30661, BindTextures_remap_index },
        { 30726, BindVertexBuffers_remap_index },
        { 30882, GetImageHandleARB_remap_index },
        { 30753, GetTextureHandleARB_remap_index },
        { 30778, GetTextureSamplerHandleARB_remap_index },
        { 31224, GetVertexAttribLui64vARB_remap_index },
        { 31137, IsImageHandleResidentARB_remap_index },
        { 31105, IsTextureHandleResidentARB_remap_index },
        { 30942, MakeImageHandleNonResidentARB_remap_index },
        { 30909, MakeImageHandleResidentARB_remap_index },
        { 30845, MakeTextureHandleNonResidentARB_remap_index },
        { 30811, MakeTextureHandleResidentARB_remap_index },
        { 31033, ProgramUniformHandleui64ARB_remap_index },
        { 31068, ProgramUniformHandleui64vARB_remap_index },
        { 30977, UniformHandleui64ARB_remap_index },
        { 31004, UniformHandleui64vARB_remap_index },
        { 31167, VertexAttribL1ui64ARB_remap_index },
        { 31195, VertexAttribL1ui64vARB_remap_index },
        { 31256, DispatchComputeGroupSizeARB_remap_index },
        { 31294, MultiDrawArraysIndirectCountARB_remap_index },
        { 31366, MultiDrawElementsIndirectCountARB_remap_index },
        { 31443, ClipControl_remap_index },
        { 33620, BindTextureUnit_remap_index },
        { 32592, BlitNamedFramebuffer_remap_index },
        { 32629, CheckNamedFramebufferStatus_remap_index },
        { 31791, ClearNamedBufferData_remap_index },
        { 31821, ClearNamedBufferSubData_remap_index },
        { 32559, ClearNamedFramebufferfi_remap_index },
        { 32527, ClearNamedFramebufferfv_remap_index },
        { 32462, ClearNamedFramebufferiv_remap_index },
        { 32494, ClearNamedFramebufferuiv_remap_index },
        { 33212, CompressedTextureSubImage1D_remap_index },
        { 33251, CompressedTextureSubImage2D_remap_index },
        { 33292, CompressedTextureSubImage3D_remap_index },
        { 31759, CopyNamedBufferSubData_remap_index },
        { 33335, CopyTextureSubImage1D_remap_index },
        { 33367, CopyTextureSubImage2D_remap_index },
        { 33401, CopyTextureSubImage3D_remap_index },
        { 31661, CreateBuffers_remap_index },
        { 32112, CreateFramebuffers_remap_index },
        { 34354, CreateProgramPipelines_remap_index },
        { 34383, CreateQueries_remap_index },
        { 32750, CreateRenderbuffers_remap_index },
        { 34333, CreateSamplers_remap_index },
        { 32893, CreateTextures_remap_index },
        { 31461, CreateTransformFeedbacks_remap_index },
        { 33891, CreateVertexArrays_remap_index },
        { 33916, DisableVertexArrayAttrib_remap_index },
        { 33947, EnableVertexArrayAttrib_remap_index },
        { 31949, FlushMappedNamedBufferRange_remap_index },
        { 33593, GenerateTextureMipmap_remap_index },
        { 33668, GetCompressedTextureImage_remap_index },
        { 32017, GetNamedBufferParameteri64v_remap_index },
        { 31984, GetNamedBufferParameteriv_remap_index },
        { 32052, GetNamedBufferPointerv_remap_index },
        { 32082, GetNamedBufferSubData_remap_index },
        { 32701, GetNamedFramebufferAttachmentParameteriv_remap_index },
        { 32663, GetNamedFramebufferParameteriv_remap_index },
        { 32854, GetNamedRenderbufferParameteriv_remap_index },
        { 34467, GetQueryBufferObjecti64v_remap_index },
        { 34404, GetQueryBufferObjectiv_remap_index },
        { 34500, GetQueryBufferObjectui64v_remap_index },
        { 34435, GetQueryBufferObjectuiv_remap_index },
        { 33642, GetTextureImage_remap_index },
        { 33702, GetTextureLevelParameterfv_remap_index },
        { 33737, GetTextureLevelParameteriv_remap_index },
        { 33801, GetTextureParameterIiv_remap_index },
        { 33831, GetTextureParameterIuiv_remap_index },
        { 33772, GetTextureParameterfv_remap_index },
        { 33862, GetTextureParameteriv_remap_index },
        { 31627, GetTransformFeedbacki64_v_remap_index },
        { 31595, GetTransformFeedbacki_v_remap_index },
        { 31565, GetTransformFeedbackiv_remap_index },
        { 34299, GetVertexArrayIndexed64iv_remap_index },
        { 34267, GetVertexArrayIndexediv_remap_index },
        { 34243, GetVertexArrayiv_remap_index },
        { 32379, InvalidateNamedFramebufferData_remap_index },
        { 32417, InvalidateNamedFramebufferSubData_remap_index },
        { 31856, MapNamedBuffer_remap_index },
        { 31877, MapNamedBufferRange_remap_index },
        { 31708, NamedBufferData_remap_index },
        { 31681, NamedBufferStorage_remap_index },
        { 31732, NamedBufferSubData_remap_index },
        { 32278, NamedFramebufferDrawBuffer_remap_index },
        { 32311, NamedFramebufferDrawBuffers_remap_index },
        { 32174, NamedFramebufferParameteri_remap_index },
        { 32346, NamedFramebufferReadBuffer_remap_index },
        { 32137, NamedFramebufferRenderbuffer_remap_index },
        { 32208, NamedFramebufferTexture_remap_index },
        { 32240, NamedFramebufferTextureLayer_remap_index },
        { 32776, NamedRenderbufferStorage_remap_index },
        { 32809, NamedRenderbufferStorageMultisample_remap_index },
        { 32915, TextureBuffer_remap_index },
        { 32936, TextureBufferRange_remap_index },
        { 33512, TextureParameterIiv_remap_index },
        { 33539, TextureParameterIuiv_remap_index },
        { 33436, TextureParameterf_remap_index },
        { 33461, TextureParameterfv_remap_index },
        { 33487, TextureParameteri_remap_index },
        { 33567, TextureParameteriv_remap_index },
        { 32964, TextureStorage1D_remap_index },
        { 32989, TextureStorage2D_remap_index },
        { 33042, TextureStorage2DMultisample_remap_index },
        { 33015, TextureStorage3D_remap_index },
        { 33080, TextureStorage3DMultisample_remap_index },
        { 33119, TextureSubImage1D_remap_index },
        { 33148, TextureSubImage2D_remap_index },
        { 33179, TextureSubImage3D_remap_index },
        { 31492, TransformFeedbackBufferBase_remap_index },
        { 31527, TransformFeedbackBufferRange_remap_index },
        { 31905, UnmapNamedBufferEXT_remap_index },
        { 34178, VertexArrayAttribBinding_remap_index },
        { 34076, VertexArrayAttribFormat_remap_index },
        { 34110, VertexArrayAttribIFormat_remap_index },
        { 34144, VertexArrayAttribLFormat_remap_index },
        { 34210, VertexArrayBindingDivisor_remap_index },
        { 33977, VertexArrayElementBuffer_remap_index },
        { 34008, VertexArrayVertexBuffer_remap_index },
        { 34041, VertexArrayVertexBuffers_remap_index },
        { 34569, GetCompressedTextureSubImage_remap_index },
        { 34534, GetTextureSubImage_remap_index },
        { 34650, BufferPageCommitmentARB_remap_index },
        { 34719, NamedBufferPageCommitmentARB_remap_index },
        { 35494, GetUniformi64vARB_remap_index },
        { 35538, GetUniformui64vARB_remap_index },
        { 35584, GetnUniformi64vARB_remap_index },
        { 35611, GetnUniformui64vARB_remap_index },
        { 35639, ProgramUniform1i64ARB_remap_index },
        { 35853, ProgramUniform1i64vARB_remap_index },
        { 36073, ProgramUniform1ui64ARB_remap_index },
        { 36295, ProgramUniform1ui64vARB_remap_index },
        { 35691, ProgramUniform2i64ARB_remap_index },
        { 35908, ProgramUniform2i64vARB_remap_index },
        { 36127, ProgramUniform2ui64ARB_remap_index },
        { 36352, ProgramUniform2ui64vARB_remap_index },
        { 35744, ProgramUniform3i64ARB_remap_index },
        { 35963, ProgramUniform3i64vARB_remap_index },
        { 36182, ProgramUniform3ui64ARB_remap_index },
        { 36409, ProgramUniform3ui64vARB_remap_index },
        { 35798, ProgramUniform4i64ARB_remap_index },
        { 36018, ProgramUniform4i64vARB_remap_index },
        { 36238, ProgramUniform4ui64ARB_remap_index },
        { 36466, ProgramUniform4ui64vARB_remap_index },
        { 34850, Uniform1i64ARB_remap_index },
        { 35004, Uniform1i64vARB_remap_index },
        { 35164, Uniform1ui64ARB_remap_index },
        { 35326, Uniform1ui64vARB_remap_index },
        { 34887, Uniform2i64ARB_remap_index },
        { 35044, Uniform2i64vARB_remap_index },
        { 35203, Uniform2ui64ARB_remap_index },
        { 35368, Uniform2ui64vARB_remap_index },
        { 34925, Uniform3i64ARB_remap_index },
        { 35084, Uniform3i64vARB_remap_index },
        { 35243, Uniform3ui64ARB_remap_index },
        { 35410, Uniform3ui64vARB_remap_index },
        { 34964, Uniform4i64ARB_remap_index },
        { 35124, Uniform4i64vARB_remap_index },
        { 35284, Uniform4ui64ARB_remap_index },
        { 35452, Uniform4ui64vARB_remap_index },
        { 42251, EvaluateDepthValuesARB_remap_index },
        { 42095, FramebufferSampleLocationsfvARB_remap_index },
        { 42168, NamedFramebufferSampleLocationsfvARB_remap_index },
        { 36586, SpecializeShaderARB_remap_index },
        { 29969, InvalidateBufferData_remap_index },
        { 29938, InvalidateBufferSubData_remap_index },
        { 30031, InvalidateFramebuffer_remap_index },
        { 29995, InvalidateSubFramebuffer_remap_index },
        { 29913, InvalidateTexImage_remap_index },
        { 29879, InvalidateTexSubImage_remap_index },
        { 37061, SampleMaskSGIS_remap_index },
        { 37098, SamplePatternSGIS_remap_index },
        { 37140, ColorPointerEXT_remap_index },
        { 37165, EdgeFlagPointerEXT_remap_index },
        { 37191, IndexPointerEXT_remap_index },
        { 37215, NormalPointerEXT_remap_index },
        { 37240, TexCoordPointerEXT_remap_index },
        { 37268, VertexPointerEXT_remap_index },
        { 24392, ActiveShaderProgram_remap_index },
        { 24497, BindProgramPipeline_remap_index },
        { 24443, CreateShaderProgramv_remap_index },
        { 24547, DeleteProgramPipelines_remap_index },
        { 24604, GenProgramPipelines_remap_index },
        { 26569, GetProgramPipelineInfoLog_remap_index },
        { 24701, GetProgramPipelineiv_remap_index },
        { 24655, IsProgramPipeline_remap_index },
        { 38116, LockArraysEXT_remap_index },
        { 23270, ProgramUniform1d_remap_index },
        { 23460, ProgramUniform1dv_remap_index },
        { 25143, ProgramUniform1f_remap_index },
        { 25733, ProgramUniform1fv_remap_index },
        { 24755, ProgramUniform1i_remap_index },
        { 25333, ProgramUniform1iv_remap_index },
        { 24945, ProgramUniform1ui_remap_index },
        { 25529, ProgramUniform1uiv_remap_index },
        { 23316, ProgramUniform2d_remap_index },
        { 23509, ProgramUniform2dv_remap_index },
        { 25189, ProgramUniform2f_remap_index },
        { 25782, ProgramUniform2fv_remap_index },
        { 24801, ProgramUniform2i_remap_index },
        { 25382, ProgramUniform2iv_remap_index },
        { 24993, ProgramUniform2ui_remap_index },
        { 25580, ProgramUniform2uiv_remap_index },
        { 23363, ProgramUniform3d_remap_index },
        { 23558, ProgramUniform3dv_remap_index },
        { 25236, ProgramUniform3f_remap_index },
        { 25831, ProgramUniform3fv_remap_index },
        { 24848, ProgramUniform3i_remap_index },
        { 25431, ProgramUniform3iv_remap_index },
        { 25042, ProgramUniform3ui_remap_index },
        { 25631, ProgramUniform3uiv_remap_index },
        { 23411, ProgramUniform4d_remap_index },
        { 23607, ProgramUniform4dv_remap_index },
        { 25284, ProgramUniform4f_remap_index },
        { 25880, ProgramUniform4fv_remap_index },
        { 24896, ProgramUniform4i_remap_index },
        { 25480, ProgramUniform4iv_remap_index },
        { 25092, ProgramUniform4ui_remap_index },
        { 25682, ProgramUniform4uiv_remap_index },
        { 23656, ProgramUniformMatrix2dv_remap_index },
        { 25929, ProgramUniformMatrix2fv_remap_index },
        { 23842, ProgramUniformMatrix2x3dv_remap_index },
        { 26115, ProgramUniformMatrix2x3fv_remap_index },
        { 23908, ProgramUniformMatrix2x4dv_remap_index },
        { 26247, ProgramUniformMatrix2x4fv_remap_index },
        { 23718, ProgramUniformMatrix3dv_remap_index },
        { 25991, ProgramUniformMatrix3fv_remap_index },
        { 23974, ProgramUniformMatrix3x2dv_remap_index },
        { 26181, ProgramUniformMatrix3x2fv_remap_index },
        { 24040, ProgramUniformMatrix3x4dv_remap_index },
        { 26379, ProgramUniformMatrix3x4fv_remap_index },
        { 23780, ProgramUniformMatrix4dv_remap_index },
        { 26053, ProgramUniformMatrix4fv_remap_index },
        { 24106, ProgramUniformMatrix4x2dv_remap_index },
        { 26313, ProgramUniformMatrix4x2fv_remap_index },
        { 24172, ProgramUniformMatrix4x3dv_remap_index },
        { 26445, ProgramUniformMatrix4x3fv_remap_index },
        { 38136, UnlockArraysEXT_remap_index },
        { 24346, UseProgramStages_remap_index },
        { 26511, ValidateProgramPipeline_remap_index },
        { 27512, DebugMessageCallback_remap_index },
        { 27355, DebugMessageControl_remap_index },
        { 27435, DebugMessageInsert_remap_index },
        { 27591, GetDebugMessageLog_remap_index },
        { 28921, GetObjectLabel_remap_index },
        { 29007, GetObjectPtrLabel_remap_index },
        { 28884, ObjectLabel_remap_index },
        { 28965, ObjectPtrLabel_remap_index },
        { 28847, PopDebugGroup_remap_index },
        { 28804, PushDebugGroup_remap_index },
        { 11014, SecondaryColor3fEXT_remap_index },
        { 11060, SecondaryColor3fvEXT_remap_index },
        { 10518, MultiDrawElementsEXT_remap_index },
        { 10305, FogCoordfEXT_remap_index },
        { 10335, FogCoordfvEXT_remap_index },
        { 42866, ResizeBuffersMESA_remap_index },
        { 42888, WindowPos4dMESA_remap_index },
        { 42912, WindowPos4dvMESA_remap_index },
        { 42934, WindowPos4fMESA_remap_index },
        { 42958, WindowPos4fvMESA_remap_index },
        { 42980, WindowPos4iMESA_remap_index },
        { 43004, WindowPos4ivMESA_remap_index },
        { 43026, WindowPos4sMESA_remap_index },
        { 43050, WindowPos4svMESA_remap_index },
        { 43072, MultiModeDrawArraysIBM_remap_index },
        { 43104, MultiModeDrawElementsIBM_remap_index },
        { 43886, AreProgramsResidentNV_remap_index },
        { 43915, ExecuteProgramNV_remap_index },
        { 43939, GetProgramParameterdvNV_remap_index },
        { 43971, GetProgramParameterfvNV_remap_index },
        { 44025, GetProgramStringNV_remap_index },
        { 44003, GetProgramivNV_remap_index },
        { 44051, GetTrackMatrixivNV_remap_index },
        { 44078, GetVertexAttribdvNV_remap_index },
        { 44105, GetVertexAttribfvNV_remap_index },
        { 44132, GetVertexAttribivNV_remap_index },
        { 44159, LoadProgramNV_remap_index },
        { 44181, ProgramParameters4dvNV_remap_index },
        { 44212, ProgramParameters4fvNV_remap_index },
        { 44243, RequestResidentProgramsNV_remap_index },
        { 44275, TrackMatrixNV_remap_index },
        { 44716, VertexAttrib1dNV_remap_index },
        { 44739, VertexAttrib1dvNV_remap_index },
        { 44522, VertexAttrib1fNV_remap_index },
        { 44545, VertexAttrib1fvNV_remap_index },
        { 44328, VertexAttrib1sNV_remap_index },
        { 44351, VertexAttrib1svNV_remap_index },
        { 44763, VertexAttrib2dNV_remap_index },
        { 44787, VertexAttrib2dvNV_remap_index },
        { 44569, VertexAttrib2fNV_remap_index },
        { 44593, VertexAttrib2fvNV_remap_index },
        { 44375, VertexAttrib2sNV_remap_index },
        { 44399, VertexAttrib2svNV_remap_index },
        { 44811, VertexAttrib3dNV_remap_index },
        { 44836, VertexAttrib3dvNV_remap_index },
        { 44617, VertexAttrib3fNV_remap_index },
        { 44642, VertexAttrib3fvNV_remap_index },
        { 44423, VertexAttrib3sNV_remap_index },
        { 44448, VertexAttrib3svNV_remap_index },
        { 44860, VertexAttrib4dNV_remap_index },
        { 44886, VertexAttrib4dvNV_remap_index },
        { 44666, VertexAttrib4fNV_remap_index },
        { 44692, VertexAttrib4fvNV_remap_index },
        { 44472, VertexAttrib4sNV_remap_index },
        { 44498, VertexAttrib4svNV_remap_index },
        { 44910, VertexAttrib4ubNV_remap_index },
        { 44937, VertexAttrib4ubvNV_remap_index },
        { 44297, VertexAttribPointerNV_remap_index },
        { 45170, VertexAttribs1dvNV_remap_index },
        { 45066, VertexAttribs1fvNV_remap_index },
        { 44962, VertexAttribs1svNV_remap_index },
        { 45196, VertexAttribs2dvNV_remap_index },
        { 45092, VertexAttribs2fvNV_remap_index },
        { 44988, VertexAttribs2svNV_remap_index },
        { 45222, VertexAttribs3dvNV_remap_index },
        { 45118, VertexAttribs3fvNV_remap_index },
        { 45014, VertexAttribs3svNV_remap_index },
        { 45248, VertexAttribs4dvNV_remap_index },
        { 45144, VertexAttribs4fvNV_remap_index },
        { 45040, VertexAttribs4svNV_remap_index },
        { 45274, VertexAttribs4ubvNV_remap_index },
        { 45357, GetTexBumpParameterfvATI_remap_index },
        { 45388, GetTexBumpParameterivATI_remap_index },
        { 45301, TexBumpParameterfvATI_remap_index },
        { 45329, TexBumpParameterivATI_remap_index },
        { 45699, AlphaFragmentOp1ATI_remap_index },
        { 45729, AlphaFragmentOp2ATI_remap_index },
        { 45762, AlphaFragmentOp3ATI_remap_index },
        { 45502, BeginFragmentShaderATI_remap_index },
        { 45446, BindFragmentShaderATI_remap_index },
        { 45597, ColorFragmentOp1ATI_remap_index },
        { 45628, ColorFragmentOp2ATI_remap_index },
        { 45662, ColorFragmentOp3ATI_remap_index },
        { 45473, DeleteFragmentShaderATI_remap_index },
        { 45529, EndFragmentShaderATI_remap_index },
        { 45419, GenFragmentShadersATI_remap_index },
        { 45554, PassTexCoordATI_remap_index },
        { 45577, SampleMapATI_remap_index },
        { 45798, SetFragmentShaderConstantATI_remap_index },
        { 45859, ActiveStencilFaceEXT_remap_index },
        { 46265, GetProgramNamedParameterdvNV_remap_index },
        { 46228, GetProgramNamedParameterfvNV_remap_index },
        { 46121, ProgramNamedParameter4dNV_remap_index },
        { 46193, ProgramNamedParameter4dvNV_remap_index },
        { 46084, ProgramNamedParameter4fNV_remap_index },
        { 46158, ProgramNamedParameter4fvNV_remap_index },
        { 56221, PrimitiveRestartNV_remap_index },
        { 46302, DepthBoundsEXT_remap_index },
        { 46349, BindFramebufferEXT_remap_index },
        { 46323, BindRenderbufferEXT_remap_index },
        { 46374, StringMarkerGREMEDY_remap_index },
        { 46681, BufferParameteriAPPLE_remap_index },
        { 46710, FlushMappedBufferRangeAPPLE_remap_index },
        { 54046, VertexAttribI1iEXT_remap_index },
        { 54224, VertexAttribI1uiEXT_remap_index },
        { 54089, VertexAttribI2iEXT_remap_index },
        { 54455, VertexAttribI2ivEXT_remap_index },
        { 54269, VertexAttribI2uiEXT_remap_index },
        { 54637, VertexAttribI2uivEXT_remap_index },
        { 54133, VertexAttribI3iEXT_remap_index },
        { 54500, VertexAttribI3ivEXT_remap_index },
        { 54315, VertexAttribI3uiEXT_remap_index },
        { 54684, VertexAttribI3uivEXT_remap_index },
        { 54178, VertexAttribI4iEXT_remap_index },
        { 54545, VertexAttribI4ivEXT_remap_index },
        { 54362, VertexAttribI4uiEXT_remap_index },
        { 54731, VertexAttribI4uivEXT_remap_index },
        { 53805, ClearColorIiEXT_remap_index },
        { 53829, ClearColorIuiEXT_remap_index },
        { 56244, BindBufferOffsetEXT_remap_index },
        { 47016, BeginPerfMonitorAMD_remap_index },
        { 46950, DeletePerfMonitorsAMD_remap_index },
        { 47041, EndPerfMonitorAMD_remap_index },
        { 46925, GenPerfMonitorsAMD_remap_index },
        { 47064, GetPerfMonitorCounterDataAMD_remap_index },
        { 46888, GetPerfMonitorCounterInfoAMD_remap_index },
        { 46848, GetPerfMonitorCounterStringAMD_remap_index },
        { 46776, GetPerfMonitorCountersAMD_remap_index },
        { 46811, GetPerfMonitorGroupStringAMD_remap_index },
        { 46745, GetPerfMonitorGroupsAMD_remap_index },
        { 46978, SelectPerfMonitorCountersAMD_remap_index },
        { 45943, GetObjectParameterivAPPLE_remap_index },
        { 45885, ObjectPurgeableAPPLE_remap_index },
        { 45913, ObjectUnpurgeableAPPLE_remap_index },
        { 47189, ActiveProgramEXT_remap_index },
        { 47211, CreateShaderProgramEXT_remap_index },
        { 47163, UseShaderProgramEXT_remap_index },
        { 34612, TextureBarrierNV_remap_index },
        { 56485, VDPAUFiniNV_remap_index },
        { 56626, VDPAUGetSurfaceivNV_remap_index },
        { 56467, VDPAUInitNV_remap_index },
        { 56574, VDPAUIsSurfaceNV_remap_index },
        { 56682, VDPAUMapSurfacesNV_remap_index },
        { 56537, VDPAURegisterOutputSurfaceNV_remap_index },
        { 56501, VDPAURegisterVideoSurfaceNV_remap_index },
        { 56655, VDPAUSurfaceAccessNV_remap_index },
        { 56707, VDPAUUnmapSurfacesNV_remap_index },
        { 56596, VDPAUUnregisterSurfaceNV_remap_index },
        { 53002, BeginPerfQueryINTEL_remap_index },
        { 52949, CreatePerfQueryINTEL_remap_index },
        { 52976, DeletePerfQueryINTEL_remap_index },
        { 53027, EndPerfQueryINTEL_remap_index },
        { 52785, GetFirstPerfQueryIdINTEL_remap_index },
        { 52815, GetNextPerfQueryIdINTEL_remap_index },
        { 52910, GetPerfCounterInfoINTEL_remap_index },
        { 53050, GetPerfQueryDataINTEL_remap_index },
        { 52845, GetPerfQueryIdByNameINTEL_remap_index },
        { 52877, GetPerfQueryInfoINTEL_remap_index },
        { 53117, PolygonOffsetClampEXT_remap_index },
        { 52681, SubpixelPrecisionBiasNV_remap_index },
        { 52711, ConservativeRasterParameterfNV_remap_index },
        { 52748, ConservativeRasterParameteriNV_remap_index },
        { 53167, WindowRectanglesEXT_remap_index },
        { 57087, BufferStorageMemEXT_remap_index },
        { 56841, CreateMemoryObjectsEXT_remap_index },
        { 56789, DeleteMemoryObjectsEXT_remap_index },
        { 57395, DeleteSemaphoresEXT_remap_index },
        { 57372, GenSemaphoresEXT_remap_index },
        { 56904, GetMemoryObjectParameterivEXT_remap_index },
        { 57475, GetSemaphoreParameterui64vEXT_remap_index },
        { 56760, GetUnsignedBytei_vEXT_remap_index },
        { 56734, GetUnsignedBytevEXT_remap_index },
        { 56818, IsMemoryObjectEXT_remap_index },
        { 57421, IsSemaphoreEXT_remap_index },
        { 56870, MemoryObjectParameterivEXT_remap_index },
        { 57277, NamedBufferStorageMemEXT_remap_index },
        { 57441, SemaphoreParameterui64vEXT_remap_index },
        { 57539, SignalSemaphoreEXT_remap_index },
        { 57310, TexStorageMem1DEXT_remap_index },
        { 56941, TexStorageMem2DEXT_remap_index },
        { 56971, TexStorageMem2DMultisampleEXT_remap_index },
        { 57013, TexStorageMem3DEXT_remap_index },
        { 57044, TexStorageMem3DMultisampleEXT_remap_index },
        { 57339, TextureStorageMem1DEXT_remap_index },
        { 57115, TextureStorageMem2DEXT_remap_index },
        { 57149, TextureStorageMem2DMultisampleEXT_remap_index },
        { 57195, TextureStorageMem3DEXT_remap_index },
        { 57230, TextureStorageMem3DMultisampleEXT_remap_index },
        { 57512, WaitSemaphoreEXT_remap_index },
        { 57568, ImportMemoryFdEXT_remap_index },
        { 57594, ImportSemaphoreFdEXT_remap_index },
        { 53194, FramebufferFetchBarrierEXT_remap_index },
        { 53277, NamedRenderbufferStorageMultisampleAdvancedAMD_remap_index },
        { 53225, RenderbufferStorageMultisampleAdvancedAMD_remap_index },
        { 53334, StencilFuncSeparateATI_remap_index },
        { 53365, ProgramEnvParameters4fvEXT_remap_index },
        { 53400, ProgramLocalParameters4fvEXT_remap_index },
        { 53688, EGLImageTargetRenderbufferStorageOES_remap_index },
        { 53655, EGLImageTargetTexture2DOES_remap_index },
        { 34815, BlendBarrier_remap_index },
        { 34756, PrimitiveBoundingBox_remap_index },
        { 36523, MaxShaderCompilerThreadsKHR_remap_index },
        { 47278, MatrixLoadfEXT_remap_index },
        { 47299, MatrixLoaddEXT_remap_index },
        { 47320, MatrixMultfEXT_remap_index },
        { 47341, MatrixMultdEXT_remap_index },
        { 47362, MatrixLoadIdentityEXT_remap_index },
        { 47389, MatrixRotatefEXT_remap_index },
        { 47415, MatrixRotatedEXT_remap_index },
        { 47441, MatrixScalefEXT_remap_index },
        { 47465, MatrixScaledEXT_remap_index },
        { 47489, MatrixTranslatefEXT_remap_index },
        { 47517, MatrixTranslatedEXT_remap_index },
        { 47545, MatrixOrthoEXT_remap_index },
        { 47571, MatrixFrustumEXT_remap_index },
        { 47599, MatrixPushEXT_remap_index },
        { 47618, MatrixPopEXT_remap_index },
        { 49675, MatrixLoadTransposefEXT_remap_index },
        { 49705, MatrixLoadTransposedEXT_remap_index },
        { 49735, MatrixMultTransposefEXT_remap_index },
        { 49765, MatrixMultTransposedEXT_remap_index },
        { 48370, BindMultiTextureEXT_remap_index },
        { 50392, NamedBufferDataEXT_remap_index },
        { 50419, NamedBufferSubDataEXT_remap_index },
        { 30525, NamedBufferStorageEXT_remap_index },
        { 50613, MapNamedBufferRangeEXT_remap_index },
        { 47958, TextureImage1DEXT_remap_index },
        { 47989, TextureImage2DEXT_remap_index },
        { 48021, TextureImage3DEXT_remap_index },
        { 48054, TextureSubImage1DEXT_remap_index },
        { 48087, TextureSubImage2DEXT_remap_index },
        { 48122, TextureSubImage3DEXT_remap_index },
        { 48159, CopyTextureImage1DEXT_remap_index },
        { 48193, CopyTextureImage2DEXT_remap_index },
        { 48228, CopyTextureSubImage1DEXT_remap_index },
        { 48264, CopyTextureSubImage2DEXT_remap_index },
        { 48302, CopyTextureSubImage3DEXT_remap_index },
        { 50449, MapNamedBufferEXT_remap_index },
        { 47696, GetTextureParameterivEXT_remap_index },
        { 47729, GetTextureParameterfvEXT_remap_index },
        { 47840, TextureParameteriEXT_remap_index },
        { 47869, TextureParameterivEXT_remap_index },
        { 47899, TextureParameterfEXT_remap_index },
        { 47928, TextureParameterfvEXT_remap_index },
        { 48341, GetTextureImageEXT_remap_index },
        { 47762, GetTextureLevelParameterivEXT_remap_index },
        { 47801, GetTextureLevelParameterfvEXT_remap_index },
        { 50473, GetNamedBufferSubDataEXT_remap_index },
        { 50506, GetNamedBufferPointervEXT_remap_index },
        { 50539, GetNamedBufferParameterivEXT_remap_index },
        { 50575, FlushMappedNamedBufferRangeEXT_remap_index },
        { 50644, FramebufferDrawBufferEXT_remap_index },
        { 50675, FramebufferDrawBuffersEXT_remap_index },
        { 50708, FramebufferReadBufferEXT_remap_index },
        { 50739, GetFramebufferParameterivEXT_remap_index },
        { 50775, CheckNamedFramebufferStatusEXT_remap_index },
        { 50812, NamedFramebufferTexture1DEXT_remap_index },
        { 50850, NamedFramebufferTexture2DEXT_remap_index },
        { 50888, NamedFramebufferTexture3DEXT_remap_index },
        { 50927, NamedFramebufferRenderbufferEXT_remap_index },
        { 50967, GetNamedFramebufferAttachmentParameterivEXT_remap_index },
        { 48397, EnableClientStateiEXT_remap_index },
        { 48455, DisableClientStateiEXT_remap_index },
        { 48515, GetPointerIndexedvEXT_remap_index },
        { 48563, MultiTexEnviEXT_remap_index },
        { 48587, MultiTexEnvivEXT_remap_index },
        { 48612, MultiTexEnvfEXT_remap_index },
        { 48636, MultiTexEnvfvEXT_remap_index },
        { 48661, GetMultiTexEnvivEXT_remap_index },
        { 48689, GetMultiTexEnvfvEXT_remap_index },
        { 48717, MultiTexParameteriEXT_remap_index },
        { 48747, MultiTexParameterivEXT_remap_index },
        { 48778, MultiTexParameterfEXT_remap_index },
        { 48808, MultiTexParameterfvEXT_remap_index },
        { 48907, GetMultiTexImageEXT_remap_index },
        { 49017, MultiTexImage1DEXT_remap_index },
        { 49049, MultiTexImage2DEXT_remap_index },
        { 49082, MultiTexImage3DEXT_remap_index },
        { 49116, MultiTexSubImage1DEXT_remap_index },
        { 49150, MultiTexSubImage2DEXT_remap_index },
        { 49186, MultiTexSubImage3DEXT_remap_index },
        { 48839, GetMultiTexParameterivEXT_remap_index },
        { 48873, GetMultiTexParameterfvEXT_remap_index },
        { 49224, CopyMultiTexImage1DEXT_remap_index },
        { 49259, CopyMultiTexImage2DEXT_remap_index },
        { 49295, CopyMultiTexSubImage1DEXT_remap_index },
        { 49332, CopyMultiTexSubImage2DEXT_remap_index },
        { 49371, CopyMultiTexSubImage3DEXT_remap_index },
        { 49411, MultiTexGendEXT_remap_index },
        { 49435, MultiTexGendvEXT_remap_index },
        { 49460, MultiTexGenfEXT_remap_index },
        { 49484, MultiTexGenfvEXT_remap_index },
        { 49509, MultiTexGeniEXT_remap_index },
        { 49533, MultiTexGenivEXT_remap_index },
        { 49558, GetMultiTexGendvEXT_remap_index },
        { 49586, GetMultiTexGenfvEXT_remap_index },
        { 49614, GetMultiTexGenivEXT_remap_index },
        { 49642, MultiTexCoordPointerEXT_remap_index },
        { 52650, BindImageTextureEXT_remap_index },
        { 49795, CompressedTextureImage1DEXT_remap_index },
        { 49835, CompressedTextureImage2DEXT_remap_index },
        { 49876, CompressedTextureImage3DEXT_remap_index },
        { 49918, CompressedTextureSubImage1DEXT_remap_index },
        { 49961, CompressedTextureSubImage2DEXT_remap_index },
        { 50006, CompressedTextureSubImage3DEXT_remap_index },
        { 50053, GetCompressedTextureImageEXT_remap_index },
        { 50090, CompressedMultiTexImage1DEXT_remap_index },
        { 50131, CompressedMultiTexImage2DEXT_remap_index },
        { 50173, CompressedMultiTexImage3DEXT_remap_index },
        { 50216, CompressedMultiTexSubImage1DEXT_remap_index },
        { 50260, CompressedMultiTexSubImage2DEXT_remap_index },
        { 50306, CompressedMultiTexSubImage3DEXT_remap_index },
        { 50354, GetCompressedMultiTexImageEXT_remap_index },
        { 48937, GetMultiTexLevelParameterivEXT_remap_index },
        { 48977, GetMultiTexLevelParameterfvEXT_remap_index },
        { 51019, NamedRenderbufferStorageEXT_remap_index },
        { 51055, GetNamedRenderbufferParameterivEXT_remap_index },
        { 47636, ClientAttribDefaultEXT_remap_index },
        { 47664, PushClientAttribDefaultEXT_remap_index },
        { 51941, NamedProgramStringEXT_remap_index },
        { 51972, GetNamedProgramStringEXT_remap_index },
        { 52005, NamedProgramLocalParameter4fEXT_remap_index },
        { 52048, NamedProgramLocalParameter4fvEXT_remap_index },
        { 52089, GetNamedProgramLocalParameterfvEXT_remap_index },
        { 52132, NamedProgramLocalParameter4dEXT_remap_index },
        { 52175, NamedProgramLocalParameter4dvEXT_remap_index },
        { 52216, GetNamedProgramLocalParameterdvEXT_remap_index },
        { 52259, GetNamedProgramivEXT_remap_index },
        { 52288, TextureBufferEXT_remap_index },
        { 52313, MultiTexBufferEXT_remap_index },
        { 52339, TextureParameterIivEXT_remap_index },
        { 52370, TextureParameterIuivEXT_remap_index },
        { 52402, GetTextureParameterIivEXT_remap_index },
        { 52436, GetTextureParameterIuivEXT_remap_index },
        { 52471, MultiTexParameterIivEXT_remap_index },
        { 52503, MultiTexParameterIuivEXT_remap_index },
        { 52536, GetMultiTexParameterIivEXT_remap_index },
        { 52571, GetMultiTexParameterIuivEXT_remap_index },
        { 52607, NamedProgramLocalParameters4fvEXT_remap_index },
        { 51097, GenerateTextureMipmapEXT_remap_index },
        { 51128, GenerateMultiTexMipmapEXT_remap_index },
        { 51160, NamedRenderbufferStorageMultisampleEXT_remap_index },
        { 51208, NamedCopyBufferSubDataEXT_remap_index },
        { 51243, VertexArrayVertexOffsetEXT_remap_index },
        { 51280, VertexArrayColorOffsetEXT_remap_index },
        { 51316, VertexArrayEdgeFlagOffsetEXT_remap_index },
        { 51353, VertexArrayIndexOffsetEXT_remap_index },
        { 51388, VertexArrayNormalOffsetEXT_remap_index },
        { 51424, VertexArrayTexCoordOffsetEXT_remap_index },
        { 51463, VertexArrayMultiTexCoordOffsetEXT_remap_index },
        { 51508, VertexArrayFogCoordOffsetEXT_remap_index },
        { 51546, VertexArraySecondaryColorOffsetEXT_remap_index },
        { 51591, VertexArrayVertexAttribOffsetEXT_remap_index },
        { 51636, VertexArrayVertexAttribIOffsetEXT_remap_index },
        { 51681, EnableVertexArrayEXT_remap_index },
        { 51708, DisableVertexArrayEXT_remap_index },
        { 51736, EnableVertexArrayAttribEXT_remap_index },
        { 51769, DisableVertexArrayAttribEXT_remap_index },
        { 51803, GetVertexArrayIntegervEXT_remap_index },
        { 51836, GetVertexArrayPointervEXT_remap_index },
        { 51869, GetVertexArrayIntegeri_vEXT_remap_index },
        { 51905, GetVertexArrayPointeri_vEXT_remap_index },
        { 29111, ClearNamedBufferDataEXT_remap_index },
        { 29144, ClearNamedBufferSubDataEXT_remap_index },
        { 29770, NamedFramebufferParameteriEXT_remap_index },
        { 29807, GetNamedFramebufferParameterivEXT_remap_index },
        { 27098, VertexArrayVertexAttribLOffsetEXT_remap_index },
        { 19667, VertexArrayVertexAttribDivisorEXT_remap_index },
        { 30317, TextureBufferRangeEXT_remap_index },
        { 30418, TextureStorage2DMultisampleEXT_remap_index },
        { 30460, TextureStorage3DMultisampleEXT_remap_index },
        { 29456, VertexArrayBindVertexBufferEXT_remap_index },
        { 29496, VertexArrayVertexAttribFormatEXT_remap_index },
        { 29539, VertexArrayVertexAttribIFormatEXT_remap_index },
        { 29582, VertexArrayVertexAttribLFormatEXT_remap_index },
        { 29625, VertexArrayVertexAttribBindingEXT_remap_index },
        { 29666, VertexArrayVertexBindingDivisorEXT_remap_index },
        { 34682, NamedBufferPageCommitmentEXT_remap_index },
        { 20833, NamedStringARB_remap_index },
        { 20857, DeleteNamedStringARB_remap_index },
        { 20884, CompileShaderIncludeARB_remap_index },
        { 20916, IsNamedStringARB_remap_index },
        { 20939, GetNamedStringARB_remap_index },
        { 20966, GetNamedStringivARB_remap_index },
        { 53731, EGLImageTargetTexStorageEXT_remap_index },
        { 53766, EGLImageTargetTextureStorageEXT_remap_index },
        { 47240, CopyImageSubDataNV_remap_index },
        { 57622, ViewportSwizzleNV_remap_index },
        { 53081, AlphaToCoverageDitherControlNV_remap_index },
        { 53592, InternalBufferSubDataCopyMESA_remap_index },
        { 57649, Vertex2hNV_remap_index },
        { 57666, Vertex2hvNV_remap_index },
        { 57683, Vertex3hNV_remap_index },
        { 57701, Vertex3hvNV_remap_index },
        { 57718, Vertex4hNV_remap_index },
        { 57737, Vertex4hvNV_remap_index },
        { 57754, Normal3hNV_remap_index },
        { 57772, Normal3hvNV_remap_index },
        { 57789, Color3hNV_remap_index },
        { 57806, Color3hvNV_remap_index },
        { 57822, Color4hNV_remap_index },
        { 57840, Color4hvNV_remap_index },
        { 57856, TexCoord1hNV_remap_index },
        { 57874, TexCoord1hvNV_remap_index },
        { 57893, TexCoord2hNV_remap_index },
        { 57912, TexCoord2hvNV_remap_index },
        { 57931, TexCoord3hNV_remap_index },
        { 57951, TexCoord3hvNV_remap_index },
        { 57970, TexCoord4hNV_remap_index },
        { 57991, TexCoord4hvNV_remap_index },
        { 58010, MultiTexCoord1hNV_remap_index },
        { 58034, MultiTexCoord1hvNV_remap_index },
        { 58059, MultiTexCoord2hNV_remap_index },
        { 58084, MultiTexCoord2hvNV_remap_index },
        { 58109, MultiTexCoord3hNV_remap_index },
        { 58135, MultiTexCoord3hvNV_remap_index },
        { 58160, MultiTexCoord4hNV_remap_index },
        { 58187, MultiTexCoord4hvNV_remap_index },
        { 58510, FogCoordhNV_remap_index },
        { 58527, FogCoordhvNV_remap_index },
        { 58545, SecondaryColor3hNV_remap_index },
        { 58571, SecondaryColor3hvNV_remap_index },
        { 53633, InternalSetError_remap_index },
        { 58212, VertexAttrib1hNV_remap_index },
        { 58235, VertexAttrib1hvNV_remap_index },
        { 58259, VertexAttrib2hNV_remap_index },
        { 58283, VertexAttrib2hvNV_remap_index },
        { 58307, VertexAttrib3hNV_remap_index },
        { 58332, VertexAttrib3hvNV_remap_index },
        { 58356, VertexAttrib4hNV_remap_index },
        { 58382, VertexAttrib4hvNV_remap_index },
        { 58406, VertexAttribs1hvNV_remap_index },
        { 58432, VertexAttribs2hvNV_remap_index },
        { 58458, VertexAttribs3hvNV_remap_index },
        { 58484, VertexAttribs4hvNV_remap_index },
        {    -1, -1 }
};
