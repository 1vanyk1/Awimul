#ifndef ST_TGSI_LOWER_YUV_H
#define ST_TGSI_LOWER_YUV_H

struct tgsi_token;

extern const struct tgsi_token * st_tgsi_lower_yuv(const struct tgsi_token *tokens,
                                                   unsigned free_slots,
                                                   unsigned lower_nv12,
                                                   unsigned lower_iyuv);

#endif /* ST_TGSI_LOWER_YUV_H */
