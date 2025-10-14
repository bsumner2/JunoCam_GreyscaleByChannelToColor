/* Copyright (C) Burt O Sumner October 13th, 2025
 * All Rights Reserved */

#include "SDL_render.h"
#include "SDL_video.h"
#include "error_io.h"
#include "image_proc.h"
#include <SDL.h>
#include <assert.h>
#define MIN_EXT_LEN 4
#define MAX_EXT_LEN 5
typedef enum e_supported_fmt_spellings {
  BMP_LOWER=0, BMP_UPPER, PNG_LOWER, PNG_UPPER, JPG_LOWER, JPG_UPPER, JPEG_LOWER,
  JPEG_UPPER, SUPPORTED_FMT_SPELLING_COUNT
} SupportedFmtSpelling_e;



static const char *SUPPORTED_FMT_SPELLINGS[SUPPORTED_FMT_SPELLING_COUNT] = {
  ".bmp", ".BMP", ".png", ".PNG", ".jpg", ".JPG", ".jpeg", ".JPEG"
};

static void PrintUsage(const char *__restrict argv0) {
  fprintf(stderr, "\x1b[1;34mUsage:\x1b[0m\n"
      "\t\x1b[1;36m%s \x1b[31m<red channel grayscale img path> "
      "\x1b[32m<green channel grayscale img path> "
      "\x1b[34m<blue channel grayscale img path> "
      "\x1b[39m<output image path>\x1b[22m\n"
      "Supported file formats: \x1b[1;35mBMP\x1b[0m, "
      "\x1b[1;35mJPG\x1b[0m, "
      "\x1b[1;35mPNG\x1b[0m.\n", 
      argv0);
}

int main(int argc, char *argv[]) {
  SupportedFmt_e formats[4]={
    SUPPORTED_FMT_COUNT,SUPPORTED_FMT_COUNT,
    SUPPORTED_FMT_COUNT,SUPPORTED_FMT_COUNT
  };
  
  if (5 != argc) {
    PERR("Invalid arg count. See usage below.\n");
    PrintUsage(argv[0]);
    return -1;
  }
  for (int valid, len, i = 1; 5>i; ++i) {
    len = strlen(argv[i]);
    valid = (bool)(MIN_EXT_LEN<len);
    if (valid) {
      valid = 0;
      for (int fmt, extlen, j = 0; SUPPORTED_FMT_SPELLING_COUNT>j;++j) {
        extlen = strlen(SUPPORTED_FMT_SPELLINGS[j]);
        if (0!=strcmp(&argv[i][len-extlen], SUPPORTED_FMT_SPELLINGS[j]))
          continue;
        if (JPG_LOWER<=j) {
          fmt = JPG;
        } else {
          fmt = j/2;
        }
        assert(BMP<=fmt && SUPPORTED_FMT_COUNT>fmt);
        formats[i-1]=fmt;
        valid = 1;
        break;
      }
    }
    if (!valid) {
      PERRF("Invalid argv[\x1b[1m%d\x1b[0m]. \x1b[1;33m\"%s\"\x1b[0m is not "
          "a valid file format. See usage below.\n", i, argv[i]);
      PrintUsage(argv[0]);
      return -1;
    }
  }
  RawImage_t out_img = {
    .initialized=false,
    .buf = NULL,
    .width = 0,
    .height = 0,
  };
  SDL_Window *win;
  SDL_Renderer *ren;
  if (0>SDL_Init(SDL_INIT_EVERYTHING^SDL_INIT_AUDIO)) {
    PERRF("Failed to initialize SDL.\n\x1b[1;34mDetails:\n\x1b[0m%s\n",
        SDL_GetError());
    return -1;
  }

  for (RawImage_Channel_Idx_t channel=RED; ALPHA>channel; ++channel) {
    if (!Image_IncorporateImageAsChannel(&out_img, argv[1+channel], 
                                         channel, formats[channel])) {
      if (out_img.initialized) {
        SDL_Quit();
        assert(NULL!=out_img.buf);
        free(out_img.buf);
        return -1;
      }
      SDL_Quit();
      return -1;
    }
  }
  const int H=out_img.height, W=out_img.width;
  if (H<=0 || W<=0) {
    printf("Invalid window dims %dx%d", W, H);
    free(out_img.buf);
    SDL_Quit();
    return -1;
  }
  if (!(win = SDL_CreateWindow("JunoCam Image Processor", SDL_WINDOWPOS_CENTERED, 
          SDL_WINDOWPOS_CENTERED, W, H, SDL_WINDOW_SHOWN))) {
    PERRF("SDL couldn't open window. Details: %s\n",
        SDL_GetError());
    free(out_img.buf);
    SDL_Quit();
    return -1;
  }


  if (!(ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_SOFTWARE))) {
    PERRF("SDL couldn't open renderer. Details: %s\n",
        SDL_GetError());
    free(out_img.buf);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return -1;
  }
  RawImage_Pixel_t curpixel;
  for (int x,y=0; H>y;++y) {
    for (x=0; W>x; ++x) {
      curpixel = out_img.buf[y*W+x];
      SDL_SetRenderDrawColor(ren, curpixel.channels.r, curpixel.channels.g, curpixel.channels.b, 0xFF);
      SDL_RenderDrawPoint(ren, x,y);
    }
  }
  SDL_RenderPresent(ren);

  Image_Save(argv[4], formats[3], &out_img, ren);
  free(out_img.buf);

  
  SDL_Event ev;
  for (bool run = true; run; ) {
    for (;SDL_PollEvent(&ev);) {
      if (SDL_QUIT==ev.type) {
        run=false;
        break;
      }
    }
  }
  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);
  SDL_Quit();
  return 0;
}
