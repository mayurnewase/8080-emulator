#include <SDL2/SDL.h>

// to run
// >> clang main.c -lSDL2 && ./a.out

void demo1() {
  SDL_Texture* texture;

  SDL_Init(SDL_INIT_VIDEO);

  // Create a window
  SDL_Window* window =
      SDL_CreateWindow("SDL Basics", 100, 100, 640, 480, SDL_WINDOW_SHOWN);

  // Create a renderer
  SDL_Renderer* renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  // Create a surface
  SDL_Surface* surface = SDL_CreateRGBSurface(0, 100, 100, 32, 0, 0, 0, 0);

  // Draw on the surface
  SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 255, 0, 0));

  // Create a texture from the surface
  texture = SDL_CreateTextureFromSurface(renderer, surface);

  // Draw the texture on the renderer
  SDL_RenderCopy(renderer, texture, NULL, NULL);

  // Update the renderer
  SDL_RenderPresent(renderer);

  sleep(1);

  SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0, 255, 0));
  texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_RenderCopy(renderer, texture, NULL, NULL);
  SDL_RenderPresent(renderer);

  // Clean up
  SDL_DestroyTexture(texture);
  SDL_FreeSurface(surface);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void demo2() {
  // Initialize SDL
  SDL_Init(SDL_INIT_VIDEO);

  // Create a window
  SDL_Window* window =
      SDL_CreateWindow("Texture Update", 100, 100, 640, 480, SDL_WINDOW_SHOWN);

  // Create a renderer
  SDL_Renderer* renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  // Create a texture
  SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
      SDL_TEXTUREACCESS_STREAMING, 640, 480);

  // Create an array to store the pixel data
  Uint32* pixels = (Uint32*) malloc(640 * 480 * sizeof(Uint32));

  // Update the pixel data based on some condition
  for (int i = 0; i < 640 * 480; i++) {
    // if (i % 2 == 0) {
    pixels[i] = 0xFFFFFFFF; // White color
    // }else {
    //     pixels[i] = 0x1100000;
    // }
  }

  // Lock the texture
  void* texturePixels;
  int pitch;
  SDL_LockTexture(texture, NULL, &texturePixels, &pitch);

  // Update the texture pixel data
  memcpy(texturePixels, pixels, 640 * 480 * sizeof(Uint32));

  // Unlock the texture
  SDL_UnlockTexture(texture);

  // Update the renderer
  SDL_RenderCopy(renderer, texture, NULL, NULL);
  SDL_RenderPresent(renderer);

  // Clean up
  // delete[] pixels;
  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void demo3() {
  // Initialize SDL
  int width = 200;
  int height = 100;

  SDL_Init(SDL_INIT_VIDEO);

  // Create a window
  SDL_Window* window =
      SDL_CreateWindow("Texture Update", SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
    width, height,  SDL_WINDOW_SHOWN |
          SDL_WINDOW_OPENGL);

  // Create a renderer
  SDL_Renderer* renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  // Create a texture
  SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
      SDL_TEXTUREACCESS_STREAMING, width, height);

  // Create an array to store the pixel data
//   Uint32* pixels = (Uint32*) malloc(640 * 480 * sizeof(Uint32));

  // Update the pixel data based on some condition
//   for (int i = 0; i < 640 * 480; i++) {
//     pixels[i] = 0xffffffff;
//   }

    
    int px = 199;
    int py = 99;

    // int rotated_py = width - px - 1;
    // int rotated_px = py;


    Uint32 pixels[height][width];
    memset(&pixels, 0, width * height * sizeof(Uint32));
    pixels[py][px] = 0xffffffff;

  // Lock the texture
  void* texturePixels;
  int pitch;
  SDL_LockTexture(texture, NULL, &texturePixels, &pitch);

  // Update the texture pixel data
  memcpy(texturePixels, pixels, width * height * sizeof(Uint32));

  // Unlock the texture
  SDL_UnlockTexture(texture);

  // Update the renderer
  SDL_RenderCopy(renderer, texture, NULL, NULL);
  SDL_RenderPresent(renderer);

  sleep(2);

  // Clean up
  // delete[] pixels;
  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

int main() {
  // Initialize SDL

  // demo1();
//   demo2();
  demo3();
  
  return 0;
}
