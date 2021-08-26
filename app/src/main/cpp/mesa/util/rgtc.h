#ifndef _RGTC_H
#define _RGTC_H

void util_format_unsigned_fetch_texel_rgtc(unsigned srcRowStride, const unsigned char *pixdata,
                                           unsigned i, unsigned j, unsigned char *value, unsigned comps);

void util_format_signed_fetch_texel_rgtc(unsigned srcRowStride, const signed char *pixdata,
                                         unsigned i, unsigned j, signed char *value, unsigned comps);

void util_format_unsigned_encode_rgtc_ubyte(unsigned char *blkaddr, unsigned char srccolors[4][4],
                                            int numxpixels, int numypixels);

void util_format_signed_encode_rgtc_ubyte(signed char *blkaddr, signed char srccolors[4][4],
                                          int numxpixels, int numypixels);
#endif /* _RGTC_H */
