// This file uses the 8080 emulator to run the test suite (roms in cpu_tests
// directory). It uses a simple array as memory.
// use hexdump -C cpu_tests/TST8080.COM  to check contents of test files
// to run this test check readme for make command or run below directly
  // gcc i8080_tests.c i8080.c && ./a.out

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "i8080.h"

// memory callbacks
#define MEMORY_SIZE 0x10000 // in bytes
static uint8_t* memory = NULL; // holds the test file content which is program to run
static bool test_finished = 0;

static uint8_t rb(void* userdata, uint16_t addr) {
  return memory[addr];
}

static void wb(void* userdata, uint16_t addr, uint8_t val) {
  memory[addr] = val;
}

static uint8_t port_in(void* userdata, uint8_t port) {
  return 0x00;
}

static void port_out(void* userdata, uint8_t port, uint8_t value) {
  i8080* const c = (i8080*) userdata;

  if (port == 0) {
    test_finished = 1;
  } else if (port == 1) {
    uint8_t operation = c->c;

    if (operation == 2) { // print a character stored in E
      printf("%c", c->e);
    } else if (operation == 9) { // print from memory at (DE) until '$' char
      uint16_t addr = (c->d << 8) | c->e;
      do {
        printf("%c", rb(c, addr++));
      } while (rb(c, addr) != '$');
    }
  }
}

static inline int load_file(const char* filename, uint16_t addr) {
  FILE* f = fopen(filename, "rb");
  if (f == NULL) {
    fprintf(stderr, "error: can't open file '%s'.\n", filename);
    return 1;
  }

  // file size check:
  fseek(f, 0, SEEK_END);
  size_t file_size = ftell(f);
  rewind(f);

  if (file_size + addr >= MEMORY_SIZE) {
    fprintf(stderr, "error: file %s can't fit in memory.\n", filename);
    return 1;
  }

  // copying the bytes in memory:
  size_t result = fread(&memory[addr], sizeof(uint8_t), file_size, f);
  if (result != file_size) {
    fprintf(stderr, "error: while reading file '%s'\n", filename);
    return 1;
  }

  fclose(f);
  return 0;
}

static inline void run_test(
    i8080* const c, const char* filename, unsigned long cyc_expected) {
  i8080_init(c);
  c->userdata = c;
  c->read_byte = rb;
  c->write_byte = wb;
  c->port_in = port_in;
  c->port_out = port_out;
  memset(memory, 0, MEMORY_SIZE);

  if (load_file(filename, 0x100) != 0) { // loads file data in memorry array from 256th position
    return;
  }
  printf("*** TEST: %s\n", filename);

  c->pc = 0x100;

  // inject "out 0,a" at 0x0000 (signal to stop the test)
  // printf("memory data %d %x ", memory[0]);
  // printf("memory data %d %x ", memory[0], memory[0]);

  memory[0x0000] = 0xD3;
  memory[0x0001] = 0x00;
  int index_test = 437;

  // printf("memory data %d %x %x\n", memory[0], memory[0], memory[0x100]);
  // printf("memory data %d %d \n", memory[434], memory[0x01b2]);
  // printf("memory data %d %x \n", memory[index_test], memory[index_test]);

  // for(int i = 0 ; i <= 435 ; i++){
  //   printf("%d %d %x | ", i, memory[i], memory[i]);
  // }

  // inject "out 1,a" at 0x0005 (signal to output some characters)
  memory[0x0005] = 0xD3; // D3 instruction opcode prints to cli
  memory[0x0006] = 0x01;
  memory[0x0007] = 0xC9;

  // printf("memory at 0x100 %x %x", memory[0x100], memory[0x101]);
  // exit(1);
  long nb_instructions = 0;

  test_finished = 0;
  int steps = 0;
  while (!test_finished && steps < 652) {
    nb_instructions += 1;

    // uncomment following line to have a debug output of machine state
    // warning: will output multiple GB of data for the whole test suite
    // i8080_debug_output(c, false);
    steps++;
    // i8080_debug_output(c, false);
    i8080_step(c);
    i8080_debug_output(c, false);
  }

  long long diff = cyc_expected - c->cyc;
  printf("\n*** %lu instructions executed on %lu cycles"
         " (expected=%lu, diff=%lld)\n\n",
      nb_instructions, c->cyc, cyc_expected, diff);
}

int main(void) {
  memory = malloc(MEMORY_SIZE);
  if (memory == NULL) {
    return 1;
  }

  i8080 cpu;
  run_test(&cpu, "cpu_tests/TST8080.COM", 4924LU);
  // run_test(&cpu, "cpu_tests/CPUTEST.COM", 255653383LU);
  // run_test(&cpu, "cpu_tests/8080PRE.COM", 7817LU);
  // run_test(&cpu, "cpu_tests/8080EXM.COM", 23803381171LU);
  // run_test(&cpu, "cpu_tests/cpudiag.bin", 4924LU);

  free(memory);

  return 0;
}
