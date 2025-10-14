


/* Copyright (C) Burt O Sumner October 13th, 2025
 * All Rights Reserved */
#include "image_proc.h"
#include "error_io.h"
#include <SDL_pixels.h>
#include <SDL_image.h>
#include <SDL_surface.h>
#include <stdint.h>
#include <stdlib.h>

#define ALPHA_STRENGTH_FACTOR(alpha)\
  ((long double)(alpha/255.0L))
  

bool PNG_IncorporateImageAsChannel(RawImage_t *img_out,
                                   const char *__restrict png_file_path,
                                   RawImage_Channel_Idx_t channel) {

  long double channel_strength;
  SDL_Surface *surface;
  const RawImage_Pixel_t *pixels;
  uint32_t width, height;
  if (ALPHA<=channel || RED > channel)
    return false;
  if (NULL==img_out || NULL==png_file_path)
    return false;
  if (IMG_INIT_PNG!=IMG_Init(IMG_INIT_PNG)) {
    PERRF("Failed to init image reading library. \x1b[1mDetails:\x1b[0m %s\n",
        IMG_GetError());
    return false;
  }
  surface = IMG_Load(png_file_path);
  if (!surface) {
    PERRF("Failed to load image. \x1b[1mDetails:\x1b[0m %s\n", IMG_GetError());
    return false;
  }
  if (!img_out->initialized) {
    width = img_out->width = surface->w;
    height = img_out->height = surface->h;
    img_out->buf = calloc(width*height, sizeof(RawImage_Pixel_t));
    img_out->initialized = true;
  } else {
    width = img_out->width;
    height = img_out->height;
  }
  if ((const int)width!=surface->w) {
    PERRF("Size diff in previously established img size and \x1b[1;34m%s"
        "\x1b[0m\nPre-established size is %dx%d, but this png file is %dx%d\n", 
        png_file_path, width, height, surface->w, surface->h);
    SDL_FreeSurface(surface);
    IMG_Quit();
    return false;
  }
  if ((const int)height!=surface->h) {
    PERRF("Size diff in previously established img size and \x1b[1;34m%s"
        "\x1b[0m\nPre-established size is %dx%d, but this png file is %dx%d\n", 
        png_file_path, width, height, surface->w, surface->h);
    SDL_FreeSurface(surface);
    IMG_Quit();
    return false;
  }
  surface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA8888, 0);
  if (NULL==surface) {
    PERRF("Failed to convert surface to a normalized format. \x1b[1;34mDetails:"
        "\x1b[0m\n%s\n",IMG_GetError()); 
    IMG_Quit();
    return false;
  }
  pixels = surface->pixels;
  for (uint32_t sum, x, y = 0; height > y; ++y) {
    for (x = 0; width > x; ++x, ++pixels) {
      sum = 0;
      for (uint32_t _channel = RED; ALPHA>_channel; ++_channel) {
        sum+=pixels->channel_array[_channel];
      }
      channel_strength = (long double)sum / 3.0L;
      channel_strength*=ALPHA_STRENGTH_FACTOR(pixels->channel_array[ALPHA]);
      sum = (uint32_t)channel_strength;
      if (sum!=(sum&0xFF)) {
        WARNF("Unexpected value for channel strength: %Lf casted to int value %u\n", channel_strength, sum);
        sum&=0xFF;
      }
      img_out->buf[y*width+x].channel_array[channel] = (uint8_t)sum;
    }
  }
  SDL_FreeSurface(surface);
  IMG_Quit();
  return true;

  
  
}
