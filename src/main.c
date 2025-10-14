/* Copyright (C) Burt O Sumner October 13th, 2025
 * All Rights Reserved */

#include "SDL_render.h"
#include "SDL_video.h"
#include "error_io.h"
#include "image_proc.h"
#include <SDL.h>
#include <assert.h>
int main(int argc, char *argv[]) {
  if (5 != argc) {
    PERRF("Invalid arg count. \x1b[1;34mUsage:\x1b[0m\n"
        "\t\x1b[1;36m%s \x1b[31m<red channel grayscale img path> "
        "\x1b[32m<green channel grayscale img path> "
        "\x1b[34m<blue channel grayscale img path> "
        "\x1b[39m<output image folder and basename*>\x1b[22m\n"
        "\t\t*: Omit file extension!!!!! Program will decide output format.\n"
        "\t\t(in reality, I just want to do bitmap, and don't wanna have to "
        "validate whether\n\t\tor not output path given has extension \x1b[1m.bmp\x1b[0m)\n"
        "\t\tJust use gimp or something if you don't like it.\n"
        "\t\tI apologize for the inconvenience, it's just bitmaps are the least"
        "pain in the a55 image format to deal with.\n\t\tMaybe it's even "
        "better this way, tho, as bmp's aren't ever compressed lossily.\n", 
        argv[0]);
    return -1;
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
  if (!PNG_IncorporateImageAsChannel(&out_img, argv[1], RED)) {
    if (out_img.initialized) {
      assert(NULL!=out_img.buf);
      free(out_img.buf);
      return -1;
    }
    SDL_Quit();
    return -1;
  }
  if (!PNG_IncorporateImageAsChannel(&out_img, argv[2], GREEN)) {
    free(out_img.buf);
    SDL_Quit();
    return -1;
  }

  if (!PNG_IncorporateImageAsChannel(&out_img, argv[3], BLUE)) {
    free(out_img.buf);
    SDL_Quit();
    return -1;
  }

  const int H=out_img.height, W=out_img.width;
  if (H<=0 || W<=0) {
    printf("Invalid window dims %dx%d", W, H);
    free(out_img.buf);
    SDL_Quit();
    return -1;
  }
  if (!(win = SDL_CreateWindow("Tilemap", SDL_WINDOWPOS_CENTERED, 
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
