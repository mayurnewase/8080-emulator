// #include <"SDL.h">
#include "../../8080_emulator_new.h"
#include <SDL2/SDL.h>

// to compile -> clang main.c 8080_emulator_new.c -lSDL2 -o main

struct cabinate_t {
  // the arcade cabinate

  cpu cpu;
  uint8_t port1, port2;
  uint8_t shift_msb, shift_lsb,
      shift_offset; // dedicated registers for shifting, as 8080 didn't have
                    // shift instructions
                    // ussed only for rendering aliens and bullets

  uint8_t cpu_interrupt_opcode;
} cabinate;

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Texture* texture;
int pitch;
bool should_quit = false;
int time_last_updated = 0;
int CPU_CLOCK_FREQUENCY =
    2000000; // = number of states / second, 8080 uses 2 MHz clock.
int FPS = 60;
#define STATES_PER_FRAME CPU_CLOCK_FREQUENCY / FPS
#define SCREEN_WIDTH 224 // after rotation
#define SCREEN_HEIGHT 256 // after rotation
int vram_address = 0x2400;

void update_screen() {
  Uint32 buffer[SCREEN_HEIGHT][SCREEN_WIDTH];

  for (int i = 0; i < 256 * 224 / 8; i++) {
    uint8_t pixel_byte = cabinate.cpu.memory[vram_address + i];
    int py = i * 8 / 256;

    for (uint8_t j = 0; j < 8; j++) {
      Uint32 pixel_value = (pixel_byte >> j) & 0x1 ? 0xffffffff : 0x00000000;
      int px = ((i * 8) % 256) + j;

      // rotate screen 90 degrees anticlockwise
      // NOTE: SDL's co-ordinate system starts from top left corner
      // x on horizontal axis and y on vertical axis
      // and buffer co-ordinates for x and y are reversed.

      int rotated_px = py;
      int rotated_py = 255 - px;
      buffer[rotated_py][rotated_px] = pixel_value;
    }
  }
  void* texture_pixels;

  SDL_LockTexture(texture, NULL, &texture_pixels, &pitch);
  memcpy(texture_pixels, &buffer, 256 * 224 * sizeof(Uint32));

  //   int* pixels = (int*) malloc(224 * 256 * sizeof(int));
  //   Uint32* pixel_byte = (Uint32*) malloc(224 * 256 * sizeof(Uint32));
  //   // memset(pixel_byte, 0x0000000f, 224 * 256 * sizeof(Uint32));
  //    for (int i = 0; i < 224*256; i++) {
  //        pixels[i] = 0xffffffff;
  //      }
  //  memcpy(texture_pixels, pixels, 224 * 256 * sizeof(Uint32));

  SDL_UnlockTexture(texture);
  SDL_RenderCopy(renderer, texture, NULL, NULL);
  SDL_RenderPresent(renderer);
}

uint8_t cabinate_port_in(uint8_t port_no) {
  // has 2 8 bit ports as below, which will be set by cabinate
  // to be read by cpu

  // PORT 1:
  // Bit Description
  // 0   Coin slot (1 = coin inserted)
  // 1   Two players button
  // 2   One player button (start)
  // 3   n/a
  // 4   Player one - Fire button
  // 5   Player one - Left button
  // 6   Player one - Right button
  // 7   n/a

  // PORT 2:
  // Bit Description
  // 0-1 DIP switch: number of ships (00 = 3 ships, 10 = 5 ships,
  //                                  01 = 4 ships, 11 = 6 ships)
  // 2   TILT
  // 3   n/a
  // 4   Player two - Fire button
  // 5   Player two - Left button
  // 6   Player two - Right button
  // 7   ???

  // sleep(1);
  uint8_t value = 0xff;

  switch (port_no) {
  case 1: value = cabinate.port1; break;

  case 2: value = cabinate.port2; break;

  case 3: {
    const uint16_t shift = (cabinate.shift_msb << 8) | cabinate.shift_lsb;
    value = (shift >> (8 - cabinate.shift_offset)) & 0xFF;
  } break;

  default: printf("\nport in invalid %d\n", port_no);
  }

  return value;
}

void cabinate_port_out(uint8_t port_no, uint8_t data) {
  /*
  The 8080 instruction set does not include opcodes for shifting. An 8-bit pixel
  image must be shifted into a 16-bit word for the desired bit-position on the
  screen. Space Invaders adds a hardware shift register to help with the math.


    16 bit shift register:

        f              0	bit
        xxxxxxxxyyyyyyyy

        Writing to port 4 shifts x into y, and the new value into x, eg.
        $0000,
        write $aa -> $aa00,
        write $ff -> $ffaa,
        write $12 -> $12ff, ..

        Writing to port 2 (bits 0,1,2) sets the offset for the 8 bit result, eg.
        offset 0:
        rrrrrrrr		result=xxxxxxxx
        xxxxxxxxyyyyyyyy

        offset 2:
          rrrrrrrr	result=xxxxxxyy
        xxxxxxxxyyyyyyyy

        offset 7:
               rrrrrrrr	result=xyyyyyyy
        xxxxxxxxyyyyyyyy

        Reading from port 3 returns said result.
  */
  switch (port_no) {
  case 2: cabinate.shift_offset = data & 0b111; break;

  case 4: cabinate.shift_lsb = cabinate.shift_msb; cabinate.shift_msb = data;
  }
}

void init() {
  // init sdl and cabinate and cpu

  SDL_Init(SDL_INIT_VIDEO);
  window = SDL_CreateWindow("Aliens ka hamla", SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2,
      SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);

  renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

  texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
      SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
  SDL_SetWindowMinimumSize(window, SCREEN_WIDTH, SCREEN_HEIGHT);

  // cpu
  init_memory(&cabinate.cpu);
  load_rom(&cabinate.cpu, "roms/invaders.h", 0x0000);
  load_rom(&cabinate.cpu, "roms/invaders.g", 0x0800);
  load_rom(&cabinate.cpu, "roms/invaders.f", 0x1000);
  load_rom(&cabinate.cpu, "roms/invaders.e", 0x1800);
  cabinate.cpu.port_in = cabinate_port_in;
  cabinate.cpu.port_out = cabinate_port_out;

  init_registers(&cabinate);
  init_flags(&cabinate.cpu);
  reset_state_count(&cabinate.cpu);

  cabinate.cpu_interrupt_opcode = 0xcf;
}

void update_invader(int ms, FILE* op_fh) {
  // just step the cpu for millisecond received
  int count = 0;
  while (count < ms * CPU_CLOCK_FREQUENCY / 1000) {
    int current_state_count = cabinate.cpu.state_count;
    step(&cabinate.cpu, op_fh);
    count += cabinate.cpu.state_count - current_state_count;

    if (cabinate.cpu.state_count >= STATES_PER_FRAME / 2) {
      cabinate.cpu.state_count -= STATES_PER_FRAME / 2;
      interrupt_cpu(&cabinate.cpu, cabinate.cpu_interrupt_opcode);

      update_screen();
      cabinate.cpu_interrupt_opcode =
          cabinate.cpu_interrupt_opcode == 0xcf ? 0xd7 : 0xcf;
    }
  }
}

void event_loop(FILE* op_fh) {
  // use SDL_pollevent to poll for input and update routes it to the machine
  // ports executes cpu steps for time passed since last frame update update the
  // frame by getting pixels from video ram of machine update screen by
  // SDL_locktexture and memcpy

  SDL_Event event;

  while (!should_quit) {
    int current_time = SDL_GetTicks();

    while (SDL_PollEvent(&event) != 0) {
      if (event.type == SDL_QUIT ||
          event.key.keysym.scancode == SDL_SCANCODE_Q) {
        should_quit = true;
        quit();
      }

      else if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.scancode == SDL_SCANCODE_C) {
          cabinate.port1 |= (0b1 << 0);
        } else if (event.key.keysym.scancode == SDL_SCANCODE_2) {
          cabinate.port1 |= (0b1 << 2);
        } else if (event.key.keysym.scancode == SDL_SCANCODE_LEFT) {
          cabinate.port1 |= (0b1 << 5);
        } else if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT) {
          cabinate.port1 |= (0b1 << 6);
        } else if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
          cabinate.port1 |= (0b1 << 4);
        }
      } else if (event.type == SDL_KEYUP) {
        // clear the bit when button is released,
        // this should probably done by the game rom,
        // but maybe due to port_out not used we have to do it ourselves
        if (event.key.keysym.scancode == SDL_SCANCODE_C) {
          cabinate.port1 &= 0b11111110;
        } else if (event.key.keysym.scancode == SDL_SCANCODE_2) {
          cabinate.port1 &= 0b11111011;
        } else if (event.key.keysym.scancode == SDL_SCANCODE_LEFT) {
          cabinate.port1 &= 0b11011111;
        } else if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT) {
          cabinate.port1 &= 0b10111111;
        } else if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
          cabinate.port1 &= 0b11101111;
        }
      }
    }

    update_invader(current_time - time_last_updated, op_fh);
    time_last_updated = current_time;
  }
}

int quit() {
  // cleanup
  printf("quitting");

  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}

int main() {
  FILE* op_fh = fopen("executed_assembly.txt", "w");

  init();
  event_loop(op_fh);

  return 0;
}
