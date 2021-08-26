#include "format_srgb.h"

const float
        util_format_srgb_8unorm_to_linear_float_table[256] = {
        0.0000000e+00f, 3.0352698e-04f, 6.0705397e-04f, 9.1058095e-04f,
        1.2141079e-03f, 1.5176349e-03f, 1.8211619e-03f, 2.1246889e-03f,
        2.4282159e-03f, 2.7317429e-03f, 3.0352698e-03f, 3.3465358e-03f,
        3.6765073e-03f, 4.0247170e-03f, 4.3914420e-03f, 4.7769535e-03f,
        5.1815167e-03f, 5.6053916e-03f, 6.0488330e-03f, 6.5120908e-03f,
        6.9954102e-03f, 7.4990320e-03f, 8.0231930e-03f, 8.5681256e-03f,
        9.1340587e-03f, 9.7212173e-03f, 1.0329823e-02f, 1.0960094e-02f,
        1.1612245e-02f, 1.2286488e-02f, 1.2983032e-02f, 1.3702083e-02f,
        1.4443844e-02f, 1.5208514e-02f, 1.5996293e-02f, 1.6807376e-02f,
        1.7641954e-02f, 1.8500220e-02f, 1.9382361e-02f, 2.0288563e-02f,
        2.1219010e-02f, 2.2173885e-02f, 2.3153366e-02f, 2.4157632e-02f,
        2.5186860e-02f, 2.6241222e-02f, 2.7320892e-02f, 2.8426040e-02f,
        2.9556834e-02f, 3.0713444e-02f, 3.1896033e-02f, 3.3104767e-02f,
        3.4339807e-02f, 3.5601315e-02f, 3.6889450e-02f, 3.8204372e-02f,
        3.9546235e-02f, 4.0915197e-02f, 4.2311411e-02f, 4.3735029e-02f,
        4.5186204e-02f, 4.6665086e-02f, 4.8171824e-02f, 4.9706566e-02f,
        5.1269458e-02f, 5.2860647e-02f, 5.4480276e-02f, 5.6128490e-02f,
        5.7805430e-02f, 5.9511238e-02f, 6.1246054e-02f, 6.3010018e-02f,
        6.4803267e-02f, 6.6625939e-02f, 6.8478170e-02f, 7.0360096e-02f,
        7.2271851e-02f, 7.4213568e-02f, 7.6185381e-02f, 7.8187422e-02f,
        8.0219820e-02f, 8.2282707e-02f, 8.4376212e-02f, 8.6500462e-02f,
        8.8655586e-02f, 9.0841711e-02f, 9.3058963e-02f, 9.5307467e-02f,
        9.7587347e-02f, 9.9898728e-02f, 1.0224173e-01f, 1.0461648e-01f,
        1.0702310e-01f, 1.0946171e-01f, 1.1193243e-01f, 1.1443537e-01f,
        1.1697067e-01f, 1.1953843e-01f, 1.2213877e-01f, 1.2477182e-01f,
        1.2743768e-01f, 1.3013648e-01f, 1.3286832e-01f, 1.3563333e-01f,
        1.3843162e-01f, 1.4126329e-01f, 1.4412847e-01f, 1.4702727e-01f,
        1.4995979e-01f, 1.5292615e-01f, 1.5592646e-01f, 1.5896084e-01f,
        1.6202938e-01f, 1.6513219e-01f, 1.6826940e-01f, 1.7144110e-01f,
        1.7464740e-01f, 1.7788842e-01f, 1.8116424e-01f, 1.8447499e-01f,
        1.8782077e-01f, 1.9120168e-01f, 1.9461783e-01f, 1.9806932e-01f,
        2.0155625e-01f, 2.0507874e-01f, 2.0863687e-01f, 2.1223076e-01f,
        2.1586050e-01f, 2.1952620e-01f, 2.2322796e-01f, 2.2696587e-01f,
        2.3074005e-01f, 2.3455058e-01f, 2.3839757e-01f, 2.4228112e-01f,
        2.4620133e-01f, 2.5015828e-01f, 2.5415209e-01f, 2.5818285e-01f,
        2.6225066e-01f, 2.6635560e-01f, 2.7049779e-01f, 2.7467731e-01f,
        2.7889426e-01f, 2.8314874e-01f, 2.8744084e-01f, 2.9177065e-01f,
        2.9613827e-01f, 3.0054379e-01f, 3.0498731e-01f, 3.0946892e-01f,
        3.1398871e-01f, 3.1854678e-01f, 3.2314321e-01f, 3.2777810e-01f,
        3.3245154e-01f, 3.3716362e-01f, 3.4191442e-01f, 3.4670406e-01f,
        3.5153260e-01f, 3.5640014e-01f, 3.6130678e-01f, 3.6625260e-01f,
        3.7123768e-01f, 3.7626212e-01f, 3.8132601e-01f, 3.8642943e-01f,
        3.9157248e-01f, 3.9675523e-01f, 4.0197778e-01f, 4.0724021e-01f,
        4.1254261e-01f, 4.1788507e-01f, 4.2326767e-01f, 4.2869050e-01f,
        4.3415364e-01f, 4.3965717e-01f, 4.4520119e-01f, 4.5078578e-01f,
        4.5641102e-01f, 4.6207700e-01f, 4.6778380e-01f, 4.7353150e-01f,
        4.7932018e-01f, 4.8514994e-01f, 4.9102085e-01f, 4.9693300e-01f,
        5.0288646e-01f, 5.0888132e-01f, 5.1491767e-01f, 5.2099557e-01f,
        5.2711513e-01f, 5.3327640e-01f, 5.3947949e-01f, 5.4572446e-01f,
        5.5201140e-01f, 5.5834039e-01f, 5.6471151e-01f, 5.7112483e-01f,
        5.7758044e-01f, 5.8407842e-01f, 5.9061884e-01f, 5.9720179e-01f,
        6.0382734e-01f, 6.1049557e-01f, 6.1720656e-01f, 6.2396039e-01f,
        6.3075714e-01f, 6.3759687e-01f, 6.4447968e-01f, 6.5140564e-01f,
        6.5837482e-01f, 6.6538730e-01f, 6.7244316e-01f, 6.7954247e-01f,
        6.8668531e-01f, 6.9387176e-01f, 7.0110189e-01f, 7.0837578e-01f,
        7.1569350e-01f, 7.2305513e-01f, 7.3046074e-01f, 7.3791041e-01f,
        7.4540421e-01f, 7.5294222e-01f, 7.6052450e-01f, 7.6815115e-01f,
        7.7582222e-01f, 7.8353779e-01f, 7.9129794e-01f, 7.9910274e-01f,
        8.0695226e-01f, 8.1484657e-01f, 8.2278575e-01f, 8.3076988e-01f,
        8.3879901e-01f, 8.4687323e-01f, 8.5499261e-01f, 8.6315721e-01f,
        8.7136712e-01f, 8.7962240e-01f, 8.8792312e-01f, 8.9626935e-01f,
        9.0466117e-01f, 9.1309865e-01f, 9.2158186e-01f, 9.3011086e-01f,
        9.3868573e-01f, 9.4730654e-01f, 9.5597335e-01f, 9.6468625e-01f,
        9.7344529e-01f, 9.8225055e-01f, 9.9110210e-01f, 1.0000000e+00f,
};

const uint8_t
        util_format_srgb_to_linear_8unorm_table[256] = {
        0,   0,   0,   0,   0,   0,   0,   1,   1,   1,   1,   1,   1,   1,   1,   1,
        1,   1,   2,   2,   2,   2,   2,   2,   2,   2,   3,   3,   3,   3,   3,   3,
        4,   4,   4,   4,   4,   5,   5,   5,   5,   6,   6,   6,   6,   7,   7,   7,
        8,   8,   8,   8,   9,   9,   9,  10,  10,  10,  11,  11,  12,  12,  12,  13,
        13,  13,  14,  14,  15,  15,  16,  16,  17,  17,  17,  18,  18,  19,  19,  20,
        20,  21,  22,  22,  23,  23,  24,  24,  25,  25,  26,  27,  27,  28,  29,  29,
        30,  30,  31,  32,  32,  33,  34,  35,  35,  36,  37,  37,  38,  39,  40,  41,
        41,  42,  43,  44,  45,  45,  46,  47,  48,  49,  50,  51,  51,  52,  53,  54,
        55,  56,  57,  58,  59,  60,  61,  62,  63,  64,  65,  66,  67,  68,  69,  70,
        71,  72,  73,  74,  76,  77,  78,  79,  80,  81,  82,  84,  85,  86,  87,  88,
        90,  91,  92,  93,  95,  96,  97,  99, 100, 101, 103, 104, 105, 107, 108, 109,
        111, 112, 114, 115, 116, 118, 119, 121, 122, 124, 125, 127, 128, 130, 131, 133,
        134, 136, 138, 139, 141, 142, 144, 146, 147, 149, 151, 152, 154, 156, 157, 159,
        161, 163, 164, 166, 168, 170, 171, 173, 175, 177, 179, 181, 183, 184, 186, 188,
        190, 192, 194, 196, 198, 200, 202, 204, 206, 208, 210, 212, 214, 216, 218, 220,
        222, 224, 226, 229, 231, 233, 235, 237, 239, 242, 244, 246, 248, 250, 253, 255,
};

const uint8_t
        util_format_linear_to_srgb_8unorm_table[256] = {
        0,  13,  22,  28,  34,  38,  42,  46,  50,  53,  56,  59,  61,  64,  66,  69,
        71,  73,  75,  77,  79,  81,  83,  85,  86,  88,  90,  92,  93,  95,  96,  98,
        99, 101, 102, 104, 105, 106, 108, 109, 110, 112, 113, 114, 115, 117, 118, 119,
        120, 121, 122, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136,
        137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 148, 149, 150, 151,
        152, 153, 154, 155, 155, 156, 157, 158, 159, 159, 160, 161, 162, 163, 163, 164,
        165, 166, 167, 167, 168, 169, 170, 170, 171, 172, 173, 173, 174, 175, 175, 176,
        177, 178, 178, 179, 180, 180, 181, 182, 182, 183, 184, 185, 185, 186, 187, 187,
        188, 189, 189, 190, 190, 191, 192, 192, 193, 194, 194, 195, 196, 196, 197, 197,
        198, 199, 199, 200, 200, 201, 202, 202, 203, 203, 204, 205, 205, 206, 206, 207,
        208, 208, 209, 209, 210, 210, 211, 212, 212, 213, 213, 214, 214, 215, 215, 216,
        216, 217, 218, 218, 219, 219, 220, 220, 221, 221, 222, 222, 223, 223, 224, 224,
        225, 226, 226, 227, 227, 228, 228, 229, 229, 230, 230, 231, 231, 232, 232, 233,
        233, 234, 234, 235, 235, 236, 236, 237, 237, 238, 238, 238, 239, 239, 240, 240,
        241, 241, 242, 242, 243, 243, 244, 244, 245, 245, 246, 246, 246, 247, 247, 248,
        248, 249, 249, 250, 250, 251, 251, 251, 252, 252, 253, 253, 254, 254, 255, 255,
};

const unsigned
        util_format_linear_to_srgb_helper_table[104] = {
        0x0073000d, 0x007a000d, 0x0080000d, 0x0087000d,
        0x008d000d, 0x0094000d, 0x009a000d, 0x00a1000d,
        0x00a7001a, 0x00b4001a, 0x00c1001a, 0x00ce001a,
        0x00da001a, 0x00e7001a, 0x00f4001a, 0x0101001a,
        0x010e0033, 0x01280033, 0x01410033, 0x015b0033,
        0x01750033, 0x018f0033, 0x01a80033, 0x01c20033,
        0x01dc0067, 0x020f0067, 0x02430067, 0x02760067,
        0x02aa0067, 0x02dd0067, 0x03110067, 0x03440067,
        0x037800ce, 0x03df00ce, 0x044600ce, 0x04ad00ce,
        0x051400ce, 0x057b00c5, 0x05dd00bc, 0x063b00b5,
        0x06970158, 0x07420142, 0x07e30130, 0x087b0120,
        0x090b0112, 0x09940106, 0x0a1700fc, 0x0a9500f2,
        0x0b0f01cb, 0x0bf401ae, 0x0ccb0195, 0x0d950180,
        0x0e56016e, 0x0f0d015e, 0x0fbc0150, 0x10630143,
        0x11070264, 0x1238023e, 0x1357021d, 0x14660201,
        0x156601e9, 0x165a01d3, 0x174401c0, 0x182401af,
        0x18fe0331, 0x1a9602fe, 0x1c1502d2, 0x1d7e02ad,
        0x1ed4028d, 0x201a0270, 0x21520256, 0x227d0240,
        0x239f0443, 0x25c003fe, 0x27bf03c4, 0x29a10392,
        0x2b6a0367, 0x2d1d0341, 0x2ebe031f, 0x304d0300,
        0x31d105b0, 0x34a80555, 0x37520507, 0x39d504c5,
        0x3c37048b, 0x3e7c0458, 0x40a8042a, 0x42bd0401,
        0x44c20798, 0x488e071e, 0x4c1c06b6, 0x4f76065d,
        0x52a50610, 0x55ac05cc, 0x5892058f, 0x5b590559,
        0x5e0c0a23, 0x631c0980, 0x67db08f6, 0x6c55087f,
        0x70940818, 0x74a007bd, 0x787d076c, 0x7c330723,
};

