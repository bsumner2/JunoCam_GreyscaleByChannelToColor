/** (C) 14 of October, 2025 Burt Sumner */
/** Free to use, but this copyright message must remain here */

#ifndef _IMAGE_PROC_
#define _IMAGE_PROC_

#include <SDL_render.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif  /* C++ Name mangler guard */
typedef struct s_raw_img RawImage_t;
typedef union u_raw_img_pixel RawImage_Pixel_t;
union u_raw_img_pixel {
  struct s_rgb_channels {
    uint8_t r,g,b,a;
  } channels;
  uint8_t channel_array[4];
  uint32_t raw;
} __attribute__ (( aligned(4) ));
struct s_raw_img {
 RawImage_Pixel_t *buf;
  uint32_t width, height;
  bool initialized;
} __attribute__ (( aligned(8) ));
typedef enum {
  RED=0,GREEN,BLUE,ALPHA
} RawImage_Channel_Idx_t;

typedef enum e_supported_fmt {
  BMP, PNG, JPG, SUPPORTED_FMT_COUNT
} SupportedFmt_e;

bool Image_IncorporateImageAsChannel(RawImage_t *img_out, 
                                   const char *__restrict file_path,
                                   RawImage_Channel_Idx_t channel,
                                   SupportedFmt_e in_file_fmt);


bool Image_Save(const char *__restrict file_path,
                SupportedFmt_e format,
                const RawImage_t *img_data,
                SDL_Renderer *ren);


#ifdef __cplusplus
}
#endif  /* C++ Name mangler guard */

#endif  /* _IMAGE_PROC_ */
