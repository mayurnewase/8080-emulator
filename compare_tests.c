#include <stdio.h>
#include "i8080.h"
#include "8080_emulator_new.h"

// NOTE: to run -> clang i8080.c 8080_emulator_new.c compare_tests.c && ./a.out

#define ROM_NAME "test_programs/mov_A_M.bin"
uint16_t memory_offset_to_load_rom = 0x0;


// #define ROM_NAME "cpu_tests/TST8080.COM"
// uint16_t memory_offset_to_load_rom = 0x100;

uint8_t* memory = NULL;

uint8_t rb(void* userdata, int addr) {
  //   printf("\nchip reading byte at address %x\n", addr);
  return memory[addr];
}

int compare_registers(i8080* chip, cpu* cpu) {
  if (chip->a != cpu->a) {
    printf("a register is not equal\n");
    return 0;
  }
  if (chip->b != cpu->b) {
    printf("b register is not equal\n");
    return 0;
  }
  if (chip->c != cpu->c) {
    printf("c register is not equal\n");
    return 0;
  } 
  if (chip->d != cpu->d) {
    printf("d register is not equal\n");
    return 0;
  }
  if (chip->e != cpu->e) {
    printf("e register is not equal\n");
    return 0;
  }
  if (chip->h != cpu->h) {
    printf("h register is not equal\n");
    return 0;
  }
  if (chip->l != cpu->l) {
    printf("l register is not equal\n");
    return 0;
  }
  if (chip->pc != cpu->pc) {
    printf("pc register is not equal\n");
    return 0;
  }
  if (chip->sp != cpu->sp) {
    printf("sp register is not equal\n");
    return 0;
  }
  if (chip->sf != cpu->sf) {
    printf("sf register is not equal\n");
    return 0;
  }
  if (chip->zf != cpu->zf) {
    printf("zf register is not equal\n");
    return 0;
  }
  if (chip->pf != cpu->pf) {
    printf("pf register is not equal\n");
    return 0;
  }
  if (chip->cf != cpu->cf) {
    printf("cf register is not equal\n");
    return 0;
  }
  if (chip->hf != cpu->acf) {
    printf("acf register is not equal\n");
    return 0;
  }
  if (chip->interrupt_delay != cpu->interrupt_delay) {
    printf("interrupt_delay register is not equal\n");
    return 0;
  }
  if (chip->halted != cpu->halted) {
    printf("halted register is not equal\n");
    return 0;
  }
  return 1;
}

int main() {
  memory = malloc(0x10000);

  i8080* chip = malloc(sizeof(i8080));
  cpu* cpu = malloc(50);

  i8080_init(chip);
  printf("chip initialized\n");
  cpu_init(cpu, ROM_NAME, memory_offset_to_load_rom);
  printf("cpu initialized\n");

  chip->userdata = chip;
  chip->read_byte = rb;

  FILE* f = fopen(ROM_NAME, "rb");
  fread(memory, sizeof(uint8_t), ftell(f), f);
  fclose(f);

  FILE* op_fh = fopen(OUPUT_ASSEMBLY_NAME, "w");
  printf("memory %x", memory[0]);

  // custom op for tests
  // chip->pc = 0x100;
  // cpu->pc = 0x100;

  for (int i = 0; i < 4; i++) {
    i8080_step(chip);
    step(cpu, op_fh);

    i8080_debug_output(chip, true);
    debug_cpu(cpu);

    if (!compare_registers(chip, cpu)) {
      printf("Registers are not equal\n");
      return 1;
    }
    printf("\n-------------------step finish-----------------------\n");
  }

  printf("\nRegisters are equal\n");
  return 0;
}
