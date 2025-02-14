#include <stdio.h>
#include <string.h>
#include "8080_emulator_new.h"

void load_rom(cpu* cpu, char* rom_name, uint16_t memory_offset) {

  // for(int i=0; i<MEMORY_SIZE; i++){
  //   cpu->memory[i] = (uint8_t) 0;
  // }
  // printf("\n memory content %x %x \n", cpu->memory[0x7b7],
  // cpu->memory[0x7b8]);

  // load the ROM into memory
  printf("loading rom %s\n", rom_name);
  // fflush(stdout);

  FILE* f = fopen(rom_name, "rb");

  if (f == NULL) {
    fprintf(stderr, "error: can't open file '%s'.\n", rom_name);
    exit(1);
  }

  // printf("file opened\n");
  // fflush(stdout);

  // size_t bytes_read = fread(cpu->memory, 1, 5, f);

  // get the file size to load into memory correctly, or ftell gives 0 lol.
  fseek(f, 0, SEEK_END);

  size_t file_size = ftell(f);
  rewind(f);

  size_t bytes_read =
      fread(&cpu->memory[memory_offset], sizeof(uint8_t), file_size, f);

  printf("read byte in memory %x %x \n", cpu->memory[0x16c9 - memory_offset],
      cpu->memory[0x16c9 - memory_offset + 1]);

  // fclose(f); // TODO: gives error, fix it
  printf("rom loaded %d bytes\n", bytes_read);
}

void init_registers(cpu* cpu) {
  cpu->a = 0x0;
  cpu->b = 0x0;
  cpu->c = 0x0;
  cpu->d = 0x0;
  cpu->e = 0x0;
  cpu->h = 0x0;
  cpu->l = 0x0;

  cpu->pc = 0x0;
}

void init_flags(cpu* cpu) {
  cpu->zf = 0;
  cpu->cf = 0;
  cpu->sf = 0;
  cpu->pf = 0;
  cpu->acf = 0;
}

void reset_state_count(cpu* cpu) {
  cpu->state_count = 0;
}

void init_memory(cpu* cpu) {
  cpu->memory = malloc(MEMORY_SIZE);
  memset(cpu->memory, 0, MEMORY_SIZE);
}

void cpu_init(cpu* cpu, char* rom_name, uint16_t memory_offset_to_load_rom) {
  // load the ROM into memory
  // for (int i = 0; i < MEMORY_SIZE; i++) {
  //   cpu->memory[i] = 0;
  // }

  init_memory(cpu);

  load_rom(cpu, rom_name, memory_offset_to_load_rom);

  init_registers(cpu);
  init_flags(cpu);
  cpu->state_count = 0;
}

void set_zero_flag(cpu* cpu, uint8_t result) {
  cpu->zf = (result & 0xff) == 0;
}
void set_carry_flag(cpu* cpu, uint8_t a, uint8_t b, bool cy) {
  // cpu->cf = result > 0xff ? 1 : 0; // TODO: figure out why this doesn't work
  // as carry should be just if result is > 0xff then should be set to 1
  uint16_t result = a + b + cy;
  uint16_t carry = result ^ a ^ b;
  printf("\n cpu carry %d %d %d %d", a, b, cy, carry & (1 << 8));

  cpu->cf = carry & (1 << 8);
}
void set_auxiliary_carry_flag( // TODO: check this function on how does it finds
                               // out carry from 3rd bit to 4th bit.
    cpu* cpu, uint8_t a, uint8_t b, bool cy) {
  int16_t result = a + b + cy;
  int16_t carry = result ^ a ^ b;
  cpu->acf = carry & (1 << 4);
}
void set_parity_flag(cpu* cpu, uint8_t result) {
  int count_of_ones = 0;
  for (int i = 0; i < 8; i++) {
    if (result & 0x01) {
      count_of_ones++;
    }
    result = result >> 1;
  }
  cpu->pf = count_of_ones % 2 == 0;
}
void set_sign_flag(cpu* cpu, uint8_t result) {
  cpu->sf = result >> 7;
}

// cycle count of all instructions
// these are states count of instructions and not cycle count from the pdf

static const uint8_t OPCODES_STATES[256] = {
    //  0  1   2   3   4   5   6   7   8  9   A   B   C   D   E  F
    4, 10, 7, 5, 5, 5, 7, 4, 4, 10, 7, 5, 5, 5, 7, 4, // 0
    4, 10, 7, 5, 5, 5, 7, 4, 4, 10, 7, 5, 5, 5, 7, 4, // 1
    4, 10, 16, 5, 5, 5, 7, 4, 4, 10, 16, 5, 5, 5, 7, 4, // 2
    4, 10, 13, 5, 10, 10, 10, 4, 4, 10, 13, 5, 5, 5, 7, 4, // 3
    5, 5, 5, 5, 5, 5, 7, 5, 5, 5, 5, 5, 5, 5, 7, 5, // 4
    5, 5, 5, 5, 5, 5, 7, 5, 5, 5, 5, 5, 5, 5, 7, 5, // 5
    5, 5, 5, 5, 5, 5, 7, 5, 5, 5, 5, 5, 5, 5, 7, 5, // 6
    7, 7, 7, 7, 7, 7, 7, 7, 5, 5, 5, 5, 5, 5, 7, 5, // 7
    4, 4, 4, 4, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 7, 4, // 8
    4, 4, 4, 4, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 7, 4, // 9
    4, 4, 4, 4, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 7, 4, // A
    4, 4, 4, 4, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 7, 4, // B
    5, 10, 10, 10, 11, 11, 7, 11, 5, 10, 10, 10, 11, 17, 7, 11, // C
    5, 10, 10, 10, 11, 11, 7, 11, 5, 10, 10, 10, 11, 17, 7, 11, // D
    5, 10, 10, 18, 11, 11, 7, 11, 5, 5, 10, 4, 11, 17, 7, 11, // E
    5, 10, 10, 4, 11, 11, 7, 11, 5, 5, 10, 4, 11, 17, 7, 11 // F
};

void step(cpu* cpu, FILE* op_fh) {
  uint8_t instruction;
  if (cpu->interrupt_enable && cpu->interrupt_delay == 0) {
    cpu->interrupt_enable = 0;
    instruction = cpu->interrupt_opcode;
  } else {
    // fetch the instruction using program counter
    instruction = cpu->memory[cpu->pc];
  }
  // printf("\ncpu running instruction %x %d\n", instruction, cpu->state_count);

  cpu->state_count += OPCODES_STATES[instruction];

  switch (instruction) {

  case 0x7f:
    printf("> MOV A, A\n");
    fprintf(op_fh, "MOV A, A\n");
    cpu->a = cpu->a;
    cpu->pc += 1;
    break;
  case 0x78:
    printf("> MOV A, B\n");
    fprintf(op_fh, "MOV A, B\n");
    cpu->a = cpu->b;
    cpu->pc += 1;
    break;
  case 0x79:
    printf("> MOV A, C\n");
    fprintf(op_fh, "MOV A, C\n");
    cpu->a = cpu->c;
    cpu->pc += 1;
    break;
  case 0x7a:
    printf("> MOV A, D\n");
    fprintf(op_fh, "MOV A, D\n");
    cpu->a = cpu->d;
    cpu->pc += 1;
    break;
  case 0x7b:
    printf("> MOV A, E\n");
    fprintf(op_fh, "MOV A, E\n");
    cpu->a = cpu->e;
    cpu->pc += 1;
    break;
  case 0x7c:
    printf("> MOV A, H\n");
    fprintf(op_fh, "MOV A, H\n");
    cpu->a = cpu->h;
    cpu->pc += 1;
    break;
  case 0x7d:
    printf("> MOV A, L\n");
    fprintf(op_fh, "MOV A, L\n");
    cpu->a = cpu->l;
    cpu->pc += 1;
    break;
  case 0x47:
    printf("> MOV B, A\n");
    fprintf(op_fh, "MOV B, A\n");
    cpu->b = cpu->a;
    cpu->pc += 1;
    break;
  case 0x40:
    printf("> MOV B, B\n");
    fprintf(op_fh, "MOV B, B\n");
    cpu->b = cpu->b;
    cpu->pc += 1;
    break;
  case 0x41:
    printf("> MOV B, C\n");
    fprintf(op_fh, "MOV B, C\n");
    cpu->b = cpu->c;
    cpu->pc += 1;
    break;
  case 0x42:
    printf("> MOV B, D\n");
    fprintf(op_fh, "MOV B, D\n");
    cpu->b = cpu->d;
    cpu->pc += 1;
    break;
  case 0x43:
    printf("> MOV B, E\n");
    fprintf(op_fh, "MOV B, E\n");
    cpu->b = cpu->e;
    cpu->pc += 1;
    break;
  case 0x44:
    printf("> MOV B, H\n");
    fprintf(op_fh, "MOV B, H\n");
    cpu->b = cpu->h;
    cpu->pc += 1;
    break;
  case 0x45:
    printf("> MOV B, L\n");
    fprintf(op_fh, "MOV B, L\n");
    cpu->b = cpu->l;
    cpu->pc += 1;
    break;
  case 0x4f:
    printf("> MOV C, A\n");
    fprintf(op_fh, "MOV C, A\n");
    cpu->c = cpu->a;
    cpu->pc += 1;
    break;
  case 0x48:
    printf("> MOV C, B\n");
    fprintf(op_fh, "MOV C, B\n");
    cpu->c = cpu->b;
    cpu->pc += 1;
    break;
  case 0x49:
    printf("> MOV C, C\n");
    fprintf(op_fh, "MOV C, C\n");
    cpu->c = cpu->c;
    cpu->pc += 1;
    break;
  case 0x4a:
    printf("> MOV C, D");
    fprintf(op_fh, "MOV C D\n");
    cpu->c = cpu->d;
    cpu->pc += 1;
    break;
  case 0x4b:
    printf("> MOV C, E\n");
    fprintf(op_fh, "MOV C, E\n");
    cpu->c = cpu->e;
    cpu->pc += 1;
    break;
  case 0x4c:
    printf("> MOV C, H\n");
    fprintf(op_fh, "MOV C, H\n");
    cpu->c = cpu->h;
    cpu->pc += 1;
    break;
  case 0x4d:
    printf("> MOV C, L\n");
    fprintf(op_fh, "MOV C, L\n");
    cpu->c = cpu->l;
    cpu->pc += 1;
    break;
  case 0x57:
    printf("> MOV D A\n");
    fprintf(op_fh, "MOV D, A\n");
    cpu->d = cpu->a;
    cpu->pc += 1;
    break;
  case 0x50:
    printf("> MOV D, B\n");
    fprintf(op_fh, "MOV D, B\n");
    cpu->d = cpu->b;
    cpu->pc += 1;
    break;
  case 0x51:
    printf("> MOV D, C\n");
    fprintf(op_fh, "MOV D, C\n");
    cpu->d = cpu->c;
    cpu->pc += 1;
    break;
  case 0x52:
    printf("> MOV D, D\n");
    fprintf(op_fh, "MOV D, D\n");
    cpu->d = cpu->d;
    cpu->pc += 1;
    break;
  case 0x53:
    printf("> MOV D, E\n");
    fprintf(op_fh, "MOV D, E\n");
    cpu->d = cpu->e;
    cpu->pc += 1;
    break;
  case 0x54:
    printf("> MOV D, H\n");
    fprintf(op_fh, "MOV D, H\n");
    cpu->d = cpu->h;
    cpu->pc += 1;
    break;
  case 0x55:
    printf("> MOV D, L\n");
    fprintf(op_fh, "MOV D, L\n");
    cpu->d = cpu->l;
    cpu->pc += 1;
    break;
  case 0x5f:
    printf("> MOV E, A\n");
    fprintf(op_fh, "MOV E, A\n");
    cpu->e = cpu->a;
    cpu->pc += 1;
    break;
  case 0x58:
    printf("> MOV E, B\n");
    fprintf(op_fh, "MOV E, B\n");
    cpu->e = cpu->b;
    cpu->pc += 1;
    break;
  case 0x59:
    printf("> MOV E, C\n");
    fprintf(op_fh, "MOV E, C\n");
    cpu->e = cpu->c;
    cpu->pc += 1;
    break;
  case 0x5a:
    printf("> MOV E, D\n");
    fprintf(op_fh, "MOV E, D\n");
    cpu->e = cpu->d;
    cpu->pc += 1;
    break;
  case 0x5b:
    printf("> MOV E, E\n");
    fprintf(op_fh, "MOV E, E\n");
    cpu->e = cpu->e;
    cpu->pc += 1;
    break;
  case 0x5c:
    printf("> MOV E, H\n");
    fprintf(op_fh, "MOV E, H\n");
    cpu->e = cpu->h;
    cpu->pc += 1;
    break;
  case 0x5d:
    printf("> MOV E, L\n");
    fprintf(op_fh, "MOV E, L\n");
    cpu->e = cpu->l;
    cpu->pc += 1;
    break;
  case 0x67:
    printf("> MOV H, A\n");
    fprintf(op_fh, "MOV H, A\n");
    cpu->h = cpu->a;
    cpu->pc += 1;
    break;
  case 0x60:
    printf("> MOV H, B\n");
    fprintf(op_fh, "MOV H, B\n");
    cpu->h = cpu->b;
    cpu->pc += 1;
    break;
  case 0x61:
    printf("> MOV H, C\n");
    fprintf(op_fh, "MOV H, C\n");
    cpu->h = cpu->c;
    cpu->pc += 1;
    break;
  case 0x62:
    printf("> MOV H, D\n");
    fprintf(op_fh, "MOV H, D\n");
    cpu->h = cpu->d;
    cpu->pc += 1;
    break;
  case 0x63:
    printf("> MOV H, E\n");
    fprintf(op_fh, "MOV H, E\n");
    cpu->h = cpu->e;
    cpu->pc += 1;
    break;
  case 0x64:
    printf("> MOV H, H\n");
    fprintf(op_fh, "MOV H, H\n");
    cpu->h = cpu->h;
    cpu->pc += 1;
    break;
  case 0x65:
    printf("> MOV H, L\n");
    fprintf(op_fh, "MOV H, L\n");
    cpu->h = cpu->l;
    cpu->pc += 1;
    break;
  case 0x6f:
    printf("> MOV L, A\n");
    fprintf(op_fh, "MOV L, A\n");
    cpu->l = cpu->a;
    cpu->pc += 1;
    break;
  case 0x68:
    printf("> MOV L, B\n");
    fprintf(op_fh, "MOV L, B\n");
    cpu->l = cpu->b;
    cpu->pc += 1;
    break;
  case 0x69:
    printf("> MOV L, C\n");
    fprintf(op_fh, "MOV L, C\n");
    cpu->l = cpu->c;
    cpu->pc += 1;
    break;
  case 0x6a:
    printf("> MOV L, D\n");
    fprintf(op_fh, "MOV L, D\n");
    cpu->l = cpu->d;
    cpu->pc += 1;
    break;
  case 0x6b:
    printf("> MOV L, E\n");
    fprintf(op_fh, "MOV L, E\n");
    cpu->l = cpu->e;
    cpu->pc += 1;
    break;
  case 0x6c:
    printf("> MOV L, H\n");
    fprintf(op_fh, "MOV L, H\n");
    cpu->l = cpu->h;
    cpu->pc += 1;
    break;
  case 0x6d:
    printf("> MOV L, L\n");
    fprintf(op_fh, "MOV L, L\n");
    cpu->l = cpu->l;
    cpu->pc += 1;
    break;
  case 0x7e:
    printf("> MOV A, M\n");
    fprintf(op_fh, "MOV A, M\n");
    cpu->a = cpu->memory[cpu->h << 8 | cpu->l];
    cpu->pc += 1;
    break;
  case 0x46:
    printf("> MOV B, M\n");
    fprintf(op_fh, "MOV B, M\n");
    cpu->b = cpu->memory[cpu->h << 8 | cpu->l];
    cpu->pc += 1;
    break;
  case 0x4e:
    printf("> MOV C, M\n");
    fprintf(op_fh, "MOV C, M\n");
    cpu->c = cpu->memory[cpu->h << 8 | cpu->l];
    cpu->pc += 1;
    break;
  case 0x56:
    printf("> MOV D, M\n");
    fprintf(op_fh, "MOV D, M\n");
    cpu->d = cpu->memory[cpu->h << 8 | cpu->l];
    cpu->pc += 1;
    break;
  case 0x5e:
    printf("> MOV E, M\n");
    fprintf(op_fh, "MOV E, M\n");
    cpu->e = cpu->memory[cpu->h << 8 | cpu->l];
    cpu->pc += 1;
    break;
  case 0x66:
    printf("> MOV H, M\n");
    fprintf(op_fh, "MOV H, M\n");
    cpu->h = cpu->memory[cpu->h << 8 | cpu->l];
    cpu->pc += 1;
    break;
  case 0x6e:
    printf("> MOV L, M\n");
    fprintf(op_fh, "MOV L, M\n");
    cpu->l = cpu->memory[cpu->h << 8 | cpu->l];
    cpu->pc += 1;
    break;
  case 0x77:
    printf("> MOV M, A\n");
    fprintf(op_fh, "MOV M, A\n");
    cpu->memory[cpu->h << 8 | cpu->l] = cpu->a;
    cpu->pc += 1;
    break;
  case 0x70:
    printf("> MOV M, B\n");
    fprintf(op_fh, "MOV M, B\n");
    cpu->memory[cpu->h << 8 | cpu->l] = cpu->b;
    cpu->pc += 1;
    break;
  case 0x71:
    printf("> MOV M, C\n");
    fprintf(op_fh, "MOV M, C\n");
    cpu->memory[cpu->h << 8 | cpu->l] = cpu->c;
    cpu->pc += 1;
    break;
  case 0x72:
    printf("> MOV M, D\n");
    fprintf(op_fh, "MOV M, D\n");
    cpu->memory[cpu->h << 8 | cpu->l] = cpu->d;
    cpu->pc += 1;
    break;
  case 0x73:
    printf("> MOV M, E\n");
    fprintf(op_fh, "MOV M, E\n");
    cpu->memory[cpu->h << 8 | cpu->l] = cpu->e;
    cpu->pc += 1;
    break;
  case 0x74:
    printf("> MOV M, H\n");
    fprintf(op_fh, "MOV M, H\n");
    cpu->memory[cpu->h << 8 | cpu->l] = cpu->h;
    cpu->pc += 1;
    break;
  case 0x75:
    printf("> MOV M, L\n");
    fprintf(op_fh, "MOV M, L\n");
    cpu->memory[cpu->h << 8 | cpu->l] = cpu->l;
    cpu->pc += 1;
    break;

  case 0x3e:
    printf("> MVI A, data\n");
    fprintf(op_fh, "MVI A, data\n");
    cpu->a = cpu->memory[cpu->pc + 1];
    cpu->pc += 2;
    break;
  case 0x6:
    printf("> MVI B, data\n");
    fprintf(op_fh, "MVI B, data\n");
    cpu->b = cpu->memory[cpu->pc + 1];
    cpu->pc += 2;
    break;
  case 0xe:
    printf("> MVI C, data\n");
    fprintf(op_fh, "MVI C, data\n");
    cpu->c = cpu->memory[cpu->pc + 1];
    cpu->pc += 2;
    break;
  case 0x16:
    printf("> MVI D, data\n");
    fprintf(op_fh, "MVI D, data\n");
    cpu->d = cpu->memory[cpu->pc + 1];
    cpu->pc += 2;
    break;
  case 0x1e:
    printf("> MVI E, data\n");
    fprintf(op_fh, "MVI E, data\n");
    cpu->e = cpu->memory[cpu->pc + 1];
    cpu->pc += 2;
    break;
  case 0x26:
    printf("> MVI H, data\n");
    fprintf(op_fh, "MVI H, data\n");
    cpu->h = cpu->memory[cpu->pc + 1];
    cpu->pc += 2;
    break;
  case 0x2e:
    printf("> MVI L, data\n");
    fprintf(op_fh, "MVI L, data\n");
    cpu->l = cpu->memory[cpu->pc + 1];
    cpu->pc += 2;
    break;
  case 0x36:
    printf("> MVI M, data\n");
    fprintf(op_fh, "MVI M, data\n");
    cpu->memory[cpu->h << 8 | cpu->l] = cpu->memory[cpu->pc + 1];
    cpu->pc += 2;
    break;
  case 0x1:
    printf("> LXI BC, data\n");
    fprintf(op_fh, "LXI BC, data\n");
    cpu->b = cpu->memory[cpu->pc + 2];
    cpu->c = cpu->memory[cpu->pc + 1];
    cpu->pc += 3;
    break;
  case 0x11:
    printf("> LXI DE, data\n");
    fprintf(op_fh, "LXI DE, data\n");
    cpu->d = cpu->memory[cpu->pc + 2];
    cpu->e = cpu->memory[cpu->pc + 1];
    cpu->pc += 3;
    break;
  case 0x21:
    printf("> LXI HL, data\n");
    fprintf(op_fh, "LXI HL, data\n");
    cpu->h = cpu->memory[cpu->pc + 2];
    cpu->l = cpu->memory[cpu->pc + 1];
    cpu->pc += 3;
    break;
  case 0x31:
    printf("> LXI SP, data\n");
    fprintf(op_fh, "LXI SP, data\n");
    cpu->sp = cpu->memory[cpu->pc + 2] << 8 | cpu->memory[cpu->pc + 1];
    cpu->pc += 3;
    break;
  case 0x3a:
    printf("> LDA addr]n");
    fprintf(op_fh, "LDA addr\n");
    cpu->a =
        cpu->memory[cpu->memory[cpu->pc + 2] << 8 | cpu->memory[cpu->pc + 1]];
    cpu->pc += 3;
    break;
  case 0x32:
    printf("> STA addr\n");
    fprintf(op_fh, "STA addr\n");
    uint16_t destination_address =
        cpu->memory[cpu->pc + 2] << 8 | cpu->memory[cpu->pc + 1];
    cpu->memory[destination_address] = cpu->a;
    cpu->pc += 3;
    break;
  case 0x2a:
    printf("> LHLD addr\n");
    fprintf(op_fh, "LHLD addr\n");
    uint16_t source_address =
        cpu->memory[cpu->pc + 2] << 8 | cpu->memory[cpu->pc + 1];
    cpu->l = cpu->memory[source_address];
    cpu->h = cpu->memory[source_address + 1];
    cpu->pc += 3;
    break;
  case 0x22:
    printf("> SHLD addr\n");
    fprintf(op_fh, "SHLD addr\n");
    uint16_t destination_address_2 =
        cpu->memory[cpu->pc + 2] << 8 | cpu->memory[cpu->pc + 1];
    cpu->memory[destination_address_2] = cpu->l;
    cpu->memory[destination_address_2 + 1] = cpu->h;
    cpu->pc += 3;
    break;
  case 0xa:
    printf("> LDAX BC\n");
    fprintf(op_fh, "LDAX BC\n");
    cpu->a = cpu->memory[cpu->b << 8 | cpu->c];
    cpu->pc += 1;
    break;
  case 0x1a:
    printf("> LDAX DE\n");
    fprintf(op_fh, "LDAX DE\n");
    cpu->a = cpu->memory[cpu->d << 8 | cpu->e];
    cpu->pc += 1;
    break;
  case 0x2:
    printf("> STAX BC\n");
    fprintf(op_fh, "STAX BC\n");
    cpu->memory[cpu->b << 8 | cpu->c] = cpu->a;
    cpu->pc += 1;
    break;
  case 0x12:
    printf("> STAX DE\n");
    fprintf(op_fh, "STAX DE\n");
    cpu->memory[cpu->d << 8 | cpu->e] = cpu->a;
    cpu->pc += 1;
    break;
  case 0xeb:
    printf("> XCHG\n");
    fprintf(op_fh, "XCHG\n");
    uint8_t temp = cpu->d;
    cpu->d = cpu->h;
    cpu->h = temp;
    temp = cpu->e;
    cpu->e = cpu->l;
    cpu->l = temp;
    cpu->pc += 1;
    break;
  case 0x87:
    printf("> ADD A\n");
    fprintf(op_fh, "ADD A\n");
    uint8_t tmp_a1 = cpu->a;
    cpu->a += cpu->a;

    set_zero_flag(cpu, cpu->a);
    set_sign_flag(cpu, cpu->a);
    set_parity_flag(cpu, cpu->a);
    set_carry_flag(cpu, tmp_a1, tmp_a1, 0);
    set_auxiliary_carry_flag(cpu, tmp_a1, tmp_a1, 0);

    cpu->pc += 1;
    break;
  case 0x80:
    printf("> ADD B\n");
    fprintf(op_fh, "ADD B\n");
    uint8_t tmp_a2 = cpu->a;

    cpu->a += cpu->b;

    set_zero_flag(cpu, cpu->a);
    set_sign_flag(cpu, cpu->a);
    set_parity_flag(cpu, cpu->a);
    set_carry_flag(cpu, tmp_a2, cpu->b, 0);
    set_auxiliary_carry_flag(cpu, tmp_a2, cpu->b, 0);

    cpu->pc += 1;
    break;
  case 0x81:
    printf("> ADD C\n");
    fprintf(op_fh, "ADD C\n");
    uint8_t tmp_a3 = cpu->a;

    cpu->a += cpu->c;
    set_zero_flag(cpu, cpu->a);
    set_sign_flag(cpu, cpu->a);
    set_parity_flag(cpu, cpu->a);
    set_auxiliary_carry_flag(cpu, tmp_a3, cpu->c, 0);
    cpu->pc += 1;
    break;
  case 0x82:
    printf("> ADD D\n");
    fprintf(op_fh, "ADD D\n");
    uint8_t tmp_a10 = cpu->a;

    cpu->a += cpu->d;
    set_zero_flag(cpu, cpu->a);
    set_sign_flag(cpu, cpu->a);
    set_parity_flag(cpu, cpu->a);
    set_auxiliary_carry_flag(cpu, tmp_a10, cpu->d, 0);

    cpu->pc += 1;
    break;
  case 0x83:
    printf("> ADD E\n");
    fprintf(op_fh, "ADD E\n");
    uint8_t tmp_a5 = cpu->a;
    cpu->a += cpu->e;

    set_zero_flag(cpu, cpu->a);
    set_sign_flag(cpu, cpu->a);
    set_parity_flag(cpu, cpu->a);
    set_auxiliary_carry_flag(cpu, tmp_a5, cpu->e, 0);

    cpu->pc += 1;
    break;
  case 0x84:
    printf("> ADD H\n");
    fprintf(op_fh, "ADD H\n");
    uint8_t tmp_a6 = cpu->a;
    cpu->a += cpu->h;

    set_zero_flag(cpu, cpu->a);
    set_sign_flag(cpu, cpu->a);
    set_parity_flag(cpu, cpu->a);
    set_auxiliary_carry_flag(cpu, tmp_a6, cpu->h, 0);
    cpu->pc += 1;
    break;
  case 0x85:
    printf("> ADD L\n");
    fprintf(op_fh, "ADD L\n");
    uint8_t tmp_a7 = cpu->a;
    cpu->a += cpu->l;
    set_zero_flag(cpu, cpu->a);
    set_sign_flag(cpu, cpu->a);
    set_parity_flag(cpu, cpu->a);
    set_auxiliary_carry_flag(cpu, tmp_a7, cpu->l, 0);
    cpu->pc += 1;
    break;
  case 0x86:
    printf("> ADD M\n");
    fprintf(op_fh, "ADD M\n");
    uint8_t tmp_a8 = cpu->a;
    cpu->a += cpu->memory[cpu->h << 8 | cpu->l];
    set_zero_flag(cpu, cpu->a);
    set_sign_flag(cpu, cpu->a);
    set_parity_flag(cpu, cpu->a);
    set_auxiliary_carry_flag(cpu, tmp_a8, cpu->memory[cpu->h << 8 | cpu->l], 0);
    cpu->pc += 1;
    break;
  case 0xc6:
    printf("> ADI data\n");
    fprintf(op_fh, "ADI data\n");
    uint16_t tmp_a = cpu->a;
    cpu->a += cpu->memory[cpu->pc + 1];
    set_zero_flag(cpu, cpu->a);
    set_parity_flag(cpu, cpu->a);
    set_auxiliary_carry_flag(cpu, tmp_a, cpu->memory[cpu->pc + 1], 0);
    set_carry_flag(cpu, tmp_a, cpu->memory[cpu->pc + 1],
        0); // cf is not added in the computation of new carry flag value
    set_sign_flag(cpu, cpu->a);
    cpu->pc += 2;
    break;
  case 0x8f:
    printf("> ADC A\n");
    fprintf(op_fh, "ADC A\n");
    uint8_t tmp_a9 = cpu->a;

    cpu->a = cpu->a + cpu->a + cpu->cf;

    set_zero_flag(cpu, cpu->a);
    set_parity_flag(cpu, cpu->a);
    set_auxiliary_carry_flag(cpu, tmp_a9, cpu->a, cpu->cf);
    set_carry_flag(cpu, tmp_a9, tmp_a9, cpu->cf);
    set_sign_flag(cpu, cpu->a);

    cpu->pc += 1;
    break;
  case 0x88:
    printf("> ADC B\n");
    fprintf(op_fh, "ADC B\n");
    uint8_t tmp_a16 = cpu->a;

    cpu->a = cpu->a + cpu->b + cpu->cf;

    set_zero_flag(cpu, cpu->a);
    set_parity_flag(cpu, cpu->a);
    set_auxiliary_carry_flag(cpu, tmp_a16, cpu->b, cpu->cf);
    set_carry_flag(cpu, tmp_a16, cpu->b, cpu->cf);
    set_sign_flag(cpu, cpu->a);

    cpu->pc += 1;
    break;
  case 0x89:
    printf("> ADC C\n");
    fprintf(op_fh, "ADC C\n");
    uint8_t tmp_a11 = cpu->a;
    cpu->a = cpu->a + cpu->c + cpu->cf;

    set_zero_flag(cpu, cpu->a);
    set_parity_flag(cpu, cpu->a);
    set_auxiliary_carry_flag(cpu, tmp_a11, cpu->c, cpu->cf);
    set_carry_flag(cpu, tmp_a11, cpu->c, cpu->cf);
    set_sign_flag(cpu, cpu->a);

    cpu->pc += 1;
    break;
  case 0x8a:
    printf("> ADC D\n");
    fprintf(op_fh, "ADC D\n");
    uint8_t tmp_a12 = cpu->a;
    cpu->a = cpu->a + cpu->d + cpu->cf;

    set_zero_flag(cpu, cpu->a);
    set_parity_flag(cpu, cpu->a);
    set_auxiliary_carry_flag(cpu, tmp_a12, cpu->c, cpu->cf);
    set_carry_flag(cpu, tmp_a12, cpu->c, cpu->cf);
    set_sign_flag(cpu, cpu->a);

    cpu->pc += 1;
    break;
  case 0x8b:
    printf("> ADC E\n");
    fprintf(op_fh, "ADC E\n");
    uint8_t tmp_a13 = cpu->a;
    cpu->a = cpu->a + cpu->e + cpu->cf;

    set_zero_flag(cpu, cpu->a);
    set_parity_flag(cpu, cpu->a);
    set_auxiliary_carry_flag(cpu, tmp_a13, cpu->e, cpu->cf);
    set_carry_flag(cpu, tmp_a13, cpu->e, cpu->cf);
    set_sign_flag(cpu, cpu->a);

    cpu->pc += 1;
    break;
  case 0x8c:
    printf("> ADC H\n");
    fprintf(op_fh, "ADC H\n");
    uint8_t tmp_a14 = cpu->a;
    cpu->a = cpu->a + cpu->h + cpu->cf;

    set_zero_flag(cpu, cpu->a);
    set_parity_flag(cpu, cpu->a);
    set_auxiliary_carry_flag(cpu, tmp_a14, cpu->h, cpu->cf);
    set_carry_flag(cpu, tmp_a14, cpu->h, cpu->cf);
    set_sign_flag(cpu, cpu->a);

    cpu->pc += 1;
    break;
  case 0x8d:
    printf("> ADC L\n");
    fprintf(op_fh, "ADC L\n");
    uint8_t tmp_a15 = cpu->a;
    cpu->a = cpu->a + cpu->l + cpu->cf;

    set_zero_flag(cpu, cpu->a);
    set_parity_flag(cpu, cpu->a);
    set_auxiliary_carry_flag(cpu, tmp_a15, cpu->l, cpu->cf);
    set_carry_flag(cpu, tmp_a15, cpu->l, cpu->cf);
    set_sign_flag(cpu, cpu->a);

    cpu->pc += 1;
    break;
  case 0x8e:
    printf("> ADC M\n");
    fprintf(op_fh, "ADC M\n");
    uint8_t tmp_a17 = cpu->a;
    cpu->a = cpu->a + cpu->memory[cpu->h << 8 | cpu->l] + cpu->cf;

    set_zero_flag(cpu, cpu->a);
    set_parity_flag(cpu, cpu->a);
    set_auxiliary_carry_flag(
        cpu, tmp_a17, cpu->memory[cpu->h << 8 | cpu->l], cpu->cf);
    set_carry_flag(cpu, tmp_a17, cpu->memory[cpu->h << 8 | cpu->l], cpu->cf);
    set_sign_flag(cpu, cpu->a);

    cpu->pc += 1;
    break;
  case 0xce:
    printf("> ACI data\n");
    fprintf(op_fh, "ACI data\n");
    uint16_t tmp_a_2 = cpu->a;
    uint16_t tmp_result = cpu->a + cpu->memory[cpu->pc + 1] + cpu->cf;
    cpu->a = tmp_result;

    set_zero_flag(cpu, cpu->a);
    set_auxiliary_carry_flag(cpu, tmp_a_2, cpu->memory[cpu->pc + 1], cpu->cf);
    set_carry_flag(cpu, tmp_a_2, cpu->memory[cpu->pc + 1], cpu->cf);

    set_sign_flag(cpu, cpu->a);
    set_parity_flag(cpu, cpu->a);

    cpu->pc += 2;
    break;
  case 0x97:
    printf("> SUB A\n");
    fprintf(op_fh, "SUB A\n");
    uint8_t tmp_a_13 = cpu->a;
    cpu->a -= cpu->a;

    set_sign_flag(cpu, cpu->a);
    set_zero_flag(cpu, cpu->a);
    set_parity_flag(cpu, cpu->a);
    set_carry_flag(cpu, tmp_a_13, cpu->a, 1);
    set_auxiliary_carry_flag(cpu, tmp_a_13, ~(cpu->a), 1);

    cpu->pc += 1;
    break;
  case 0x90:
    printf("> SUB B\n");
    fprintf(op_fh, "SUB B\n");
    uint8_t tmp_a_11 = cpu->a;
    cpu->a -= cpu->b;

    set_sign_flag(cpu, cpu->a);
    set_zero_flag(cpu, cpu->a);
    set_parity_flag(cpu, cpu->a);
    set_carry_flag(cpu, tmp_a_11, cpu->b, 1);
    set_auxiliary_carry_flag(cpu, tmp_a_11, ~(cpu->b), 1);

    cpu->pc += 1;
    break;
  case 0x91:
    printf("> SUB C\n");
    fprintf(op_fh, "SUB C\n");
    uint8_t tmp_a_12 = cpu->a;
    cpu->a -= cpu->c;

    set_sign_flag(cpu, cpu->a);
    set_zero_flag(cpu, cpu->a);
    set_parity_flag(cpu, cpu->a);
    set_carry_flag(cpu, tmp_a_12, cpu->c, 1);
    set_auxiliary_carry_flag(cpu, tmp_a_12, ~(cpu->c), 1);

    cpu->pc += 1;
    break;
  case 0x92:
    printf("> SUB D\n");
    fprintf(op_fh, "SUB D\n");
    uint8_t tmp_a_14 = cpu->a;

    cpu->a -= cpu->d;

    set_sign_flag(cpu, cpu->a);
    set_zero_flag(cpu, cpu->a);
    set_parity_flag(cpu, cpu->a);
    set_carry_flag(cpu, tmp_a_14, cpu->d, 1);
    set_auxiliary_carry_flag(cpu, tmp_a_14, ~(cpu->d), 1);

    cpu->pc += 1;
    break;
  case 0x93:
    printf("> SUB E\n");
    fprintf(op_fh, "SUB E\n");
    uint8_t tmp_a_15 = cpu->a;
    cpu->a -= cpu->e;

    set_sign_flag(cpu, cpu->a);
    set_zero_flag(cpu, cpu->a);
    set_parity_flag(cpu, cpu->a);
    set_carry_flag(cpu, tmp_a_15, cpu->e, 1);
    set_auxiliary_carry_flag(cpu, tmp_a_15, ~(cpu->e), 1);

    cpu->pc += 1;
    break;
  case 0x94:
    printf("> SUB H\n");
    fprintf(op_fh, "SUB H\n");
    uint8_t tmp_a_16 = cpu->a;
    cpu->a -= cpu->h;

    set_sign_flag(cpu, cpu->a);
    set_zero_flag(cpu, cpu->a);
    set_parity_flag(cpu, cpu->a);
    set_carry_flag(cpu, tmp_a_16, cpu->h, 1);
    set_auxiliary_carry_flag(cpu, tmp_a_16, ~(cpu->h), 1);

    cpu->pc += 1;
    break;
  case 0x95:
    printf("> SUB L\n");
    fprintf(op_fh, "SUB L\n");
    uint8_t tmp_a_17 = cpu->a;
    cpu->a -= cpu->l;

    set_sign_flag(cpu, cpu->a);
    set_zero_flag(cpu, cpu->a);
    set_parity_flag(cpu, cpu->a);
    set_carry_flag(cpu, tmp_a_17, cpu->l, 1);
    set_auxiliary_carry_flag(cpu, tmp_a_17, ~(cpu->l), 1);

    cpu->pc += 1;
    break;
  case 0x96:
    printf("> SUB M\n");
    fprintf(op_fh, "SUB M\n");
    uint8_t tmp_a_18 = cpu->a;
    cpu->a -= cpu->memory[cpu->h << 8 | cpu->l];

    set_sign_flag(cpu, cpu->a);
    set_zero_flag(cpu, cpu->a);
    set_parity_flag(cpu, cpu->a);
    set_carry_flag(cpu, tmp_a_18, cpu->memory[cpu->h << 8 | cpu->l], 1);
    set_auxiliary_carry_flag(
        cpu, tmp_a_18, ~(cpu->memory[cpu->h << 8 | cpu->l]), 1);

    cpu->pc += 1;
    break;
  case 0xd6:
    printf("> SUI data\n");
    fprintf(op_fh, "SUI data\n");
    uint8_t tmp_a_3 = cpu->a;
    uint16_t sui_result = cpu->a - cpu->memory[cpu->pc + 1];
    cpu->a = sui_result;

    set_zero_flag(cpu, cpu->a);
    set_parity_flag(cpu, cpu->a);
    set_auxiliary_carry_flag(cpu, tmp_a_3, ~(cpu->memory[cpu->pc + 1]), 1);
    // TODO: check why 1 is always sent as carry flag to calculate

    cpu->cf = !cpu->cf;
    set_sign_flag(cpu, cpu->a);

    cpu->pc += 2;
    break;
  case 0x9f:
    printf("> SBB A\n");
    fprintf(op_fh, "SBB A\n");
    uint8_t tmp_a_19 = cpu->a;
    cpu->a = cpu->a - cpu->a - cpu->cf;

    set_sign_flag(cpu, cpu->a);
    set_zero_flag(cpu, cpu->a);
    set_parity_flag(cpu, cpu->a);
    set_carry_flag(cpu, tmp_a_19, ~(cpu->a), !cpu->cf);
    set_auxiliary_carry_flag(cpu, tmp_a_19, ~(cpu->a), !cpu->cf);
    cpu->cf = !cpu->cf;

    cpu->pc += 1;
    break;
  case 0x98:
    printf("> SBB B\n");
    fprintf(op_fh, "SBB B\n");
    uint8_t tmp_a_20 = cpu->a;

    cpu->a = cpu->a - cpu->b - cpu->cf;

    set_sign_flag(cpu, cpu->a);
    set_zero_flag(cpu, cpu->a);
    set_parity_flag(cpu, cpu->a);
    set_carry_flag(cpu, tmp_a_20, ~(cpu->b), !cpu->cf);
    set_auxiliary_carry_flag(cpu, tmp_a_20, ~(cpu->b), !cpu->cf);
    cpu->cf = !cpu->cf;

    cpu->pc += 1;
    break;
  case 0x99:
    printf("> SBB C\n");
    fprintf(op_fh, "SBB C\n");
    uint8_t tmp_a_21 = cpu->a;

    cpu->a = cpu->a - cpu->c - cpu->cf;

    set_sign_flag(cpu, cpu->a);
    set_zero_flag(cpu, cpu->a);
    set_parity_flag(cpu, cpu->a);
    set_carry_flag(cpu, tmp_a_21, ~(cpu->c), !cpu->cf);
    set_auxiliary_carry_flag(cpu, tmp_a_21, ~(cpu->c), !cpu->cf);
    cpu->cf = !cpu->cf;

    cpu->pc += 1;
    break;
  case 0x9a:
    printf("> SBB D\n");
    fprintf(op_fh, "SBB D\n");
    uint8_t tmp_a_22 = cpu->a;

    cpu->a = cpu->a - cpu->d - cpu->cf;

    set_sign_flag(cpu, cpu->a);
    set_zero_flag(cpu, cpu->a);
    set_parity_flag(cpu, cpu->a);
    set_carry_flag(cpu, tmp_a_22, ~(cpu->d), !cpu->cf);
    set_auxiliary_carry_flag(cpu, tmp_a_22, ~(cpu->d), !cpu->cf);
    cpu->cf = !cpu->cf;

    cpu->pc += 1;
    break;
  case 0x9b:
    printf("> SBB E\n");
    fprintf(op_fh, "SBB E\n");
    uint8_t tmp_a_23 = cpu->a;
    cpu->a = cpu->a - cpu->e - cpu->cf;

    set_sign_flag(cpu, cpu->a);
    set_zero_flag(cpu, cpu->a);
    set_parity_flag(cpu, cpu->a);
    set_carry_flag(cpu, tmp_a_23, ~(cpu->e), !cpu->cf);
    set_auxiliary_carry_flag(cpu, tmp_a_23, ~(cpu->e), !cpu->cf);
    cpu->cf = !cpu->cf;

    cpu->pc += 1;
    break;
  case 0x9c:
    printf("> SBB H\n");
    fprintf(op_fh, "SBB H\n");
    uint8_t tmp_a_24 = cpu->a;
    cpu->a = cpu->a - cpu->h - cpu->cf;

    set_sign_flag(cpu, cpu->a);
    set_zero_flag(cpu, cpu->a);
    set_parity_flag(cpu, cpu->a);
    set_carry_flag(cpu, tmp_a_24, ~(cpu->h), !cpu->cf);
    set_auxiliary_carry_flag(cpu, tmp_a_24, ~(cpu->h), !cpu->cf);
    cpu->cf = !cpu->cf;

    cpu->pc += 1;
    break;
  case 0x9d:
    printf("> SBB L\n");
    fprintf(op_fh, "SBB L\n");
    uint8_t tmp_a_25 = cpu->a;
    cpu->a = cpu->a - cpu->l - cpu->cf;

    set_sign_flag(cpu, cpu->a);
    set_zero_flag(cpu, cpu->a);
    set_parity_flag(cpu, cpu->a);
    set_carry_flag(cpu, tmp_a_25, ~(cpu->l), !cpu->cf);
    set_auxiliary_carry_flag(cpu, tmp_a_25, ~(cpu->l), !cpu->cf);
    cpu->cf = !cpu->cf;

    cpu->pc += 1;
    break;
  case 0x9e:
    printf("> SBB M\n");
    uint8_t tmp_a_26 = cpu->a;
    fprintf(op_fh, "SBB M\n");
    cpu->a = cpu->a - cpu->memory[cpu->h << 8 | cpu->l] - cpu->cf;

    set_sign_flag(cpu, cpu->a);
    set_zero_flag(cpu, cpu->a);
    set_parity_flag(cpu, cpu->a);
    set_carry_flag(cpu, tmp_a_26, ~(cpu->b), !cpu->cf);
    set_auxiliary_carry_flag(
        cpu, tmp_a_26, ~(cpu->memory[cpu->h << 8 | cpu->l]), !cpu->cf);
    cpu->cf = !cpu->cf;

    cpu->pc += 1;
    break;
  case 0xde:
    printf("> SBI data\n");
    fprintf(op_fh, "SBI data\n");
    uint16_t sbi_result = cpu->a - cpu->memory[cpu->pc + 1] - cpu->cf;
    cpu->a = sbi_result;

    set_zero_flag(cpu, cpu->a);
    set_parity_flag(cpu, cpu->a);
    // set_auxiliary_carry_flag(cpu, tmp_a_3, ~(cpu->memory[cpu->pc + 1]), 4,
    // !cpu->cf); // TODO: check why inverted carry flag is sent to calculate
    // half carry flag
    cpu->acf = ((cpu->a & 0x0F) - (cpu->memory[cpu->pc + 1] & 0x0F - cpu->cf)) <
               0; // TODO: check why this works and not above function
    // set_carry_flag(cpu, sbi_result);
    cpu->cf = !cpu->cf;
    set_sign_flag(cpu, cpu->a);

    cpu->pc += 2;
    break;
  case 0x3c:
    printf("> INR A\n");
    fprintf(op_fh, "INR A\n");
    uint8_t tmp_a4 = cpu->a;
    cpu->a += 1;

    set_zero_flag(cpu, cpu->a);
    set_sign_flag(cpu, cpu->a);
    set_parity_flag(cpu, cpu->a);
    // set_auxiliary_carry_flag(cpu, tmp_a4, 1, 0);
    cpu->acf = (cpu->a & 0x0f) == 0;

    cpu->pc += 1;
    break;
  case 0x4:
    printf("> INR B\n");
    fprintf(op_fh, "INR B\n");
    uint8_t tmp_b1 = cpu->b;

    cpu->b += 1;

    set_zero_flag(cpu, cpu->b);
    set_sign_flag(cpu, cpu->b);
    set_parity_flag(cpu, cpu->b);
    // set_auxiliary_carry_flag(cpu, tmp_b1, 1, 0);
    cpu->acf = (cpu->b & 0x0f) == 0;

    cpu->pc += 1;
    break;
  case 0xc:
    printf("> INR C\n");
    fprintf(op_fh, "INR C\n");
    uint8_t tmp_c1 = cpu->c;
    cpu->c += 1;

    set_zero_flag(cpu, cpu->c);
    set_sign_flag(cpu, cpu->c);
    set_parity_flag(cpu, cpu->c);
    // set_auxiliary_carry_flag(cpu, tmp_c1, 1, 0);
    cpu->acf = (cpu->c & 0x0f) == 0;
    cpu->pc += 1;
    break;
  case 0x14:
    printf("> INR D\n");
    fprintf(op_fh, "INR D\n");
    cpu->d += 1;

    set_zero_flag(cpu, cpu->d);
    set_sign_flag(cpu, cpu->d);
    set_parity_flag(cpu, cpu->d);
    // set_auxiliary_carry_flag(cpu, tmp_c1, 1, 0);
    cpu->acf = (cpu->d & 0x0f) == 0;

    cpu->pc += 1;
    break;
  case 0x1c:
    printf("> INR E\n");
    fprintf(op_fh, "INR E\n");
    cpu->e += 1;

    set_zero_flag(cpu, cpu->e);
    set_sign_flag(cpu, cpu->e);
    set_parity_flag(cpu, cpu->e);
    // set_auxiliary_carry_flag(cpu, tmp_c1, 1, 0);
    cpu->acf = (cpu->e & 0x0f) == 0;

    cpu->pc += 1;
    break;
  case 0x24:
    printf("> INR H\n");
    fprintf(op_fh, "INR H\n");
    cpu->h += 1;
    set_zero_flag(cpu, cpu->h);
    set_sign_flag(cpu, cpu->h);
    set_parity_flag(cpu, cpu->h);
    // set_auxiliary_carry_flag(cpu, tmp_c1, 1, 0);
    cpu->acf = (cpu->h & 0x0f) == 0;

    cpu->pc += 1;
    break;
  case 0x2c:
    printf("> INR L\n");
    fprintf(op_fh, "INR L\n");
    cpu->l += 1;

    set_zero_flag(cpu, cpu->l);
    set_sign_flag(cpu, cpu->l);
    set_parity_flag(cpu, cpu->e);
    // set_auxiliary_carry_flag(cpu, tmp_c1, 1, 0);
    cpu->acf = (cpu->l & 0x0f) == 0;

    cpu->pc += 1;
    break;
  case 0x34:
    printf("> INR M\n");
    fprintf(op_fh, "INR M\n");
    cpu->memory[cpu->h << 8 | cpu->l] += 1;
    set_zero_flag(cpu, cpu->memory[cpu->h << 8 | cpu->l]);
    set_sign_flag(cpu, cpu->memory[cpu->h << 8 | cpu->l]);
    set_parity_flag(cpu, cpu->memory[cpu->h << 8 | cpu->l]);
    // set_auxiliary_carry_flag(cpu, tmp_c1, 1, 0);
    cpu->acf = (cpu->memory[cpu->h << 8 | cpu->l] & 0x0f) == 0;
    cpu->pc += 1;
    break;
  case 0x3d:
    printf("> DCR A\n");
    fprintf(op_fh, "DCR A\n");
    cpu->a -= 1;
    set_zero_flag(cpu, cpu->a);
    set_sign_flag(cpu, cpu->a);
    set_parity_flag(cpu, cpu->a);
    cpu->acf = !((cpu->a & 0xf) == 0xf);

    cpu->pc += 1;
    break;
  case 0x5:
    printf("> DCR B\n");
    fprintf(op_fh, "DCR B\n");
    cpu->b -= 1;
    set_zero_flag(cpu, cpu->b);
    set_sign_flag(cpu, cpu->b);
    set_parity_flag(cpu, cpu->b);
    cpu->acf = !((cpu->b & 0xf) == 0xf);

    cpu->pc += 1;
    break;
  case 0xd:
    printf("> DCR C\n");
    fprintf(op_fh, "DCR C\n");
    cpu->c -= 1;
    set_zero_flag(cpu, cpu->c);
    set_sign_flag(cpu, cpu->c);
    set_parity_flag(cpu, cpu->c);
    cpu->acf = !((cpu->c & 0xf) == 0xf);

    cpu->pc += 1;
    break;
  case 0x15:
    printf("> DCR D\n");
    fprintf(op_fh, "DCR D\n");
    cpu->d -= 1;

    set_zero_flag(cpu, cpu->d);
    set_sign_flag(cpu, cpu->d);
    set_parity_flag(cpu, cpu->d);
    cpu->acf = !((cpu->d & 0xf) == 0xf);

    cpu->pc += 1;
    break;
  case 0x1d:
    printf("> DCR E\n");
    fprintf(op_fh, "DCR E\n");
    cpu->e -= 1;

    set_zero_flag(cpu, cpu->e);
    set_sign_flag(cpu, cpu->e);
    set_parity_flag(cpu, cpu->e);
    cpu->acf = !((cpu->e & 0xf) == 0xf);

    cpu->pc += 1;
    break;
  case 0x25:
    printf("> DCR H\n");
    fprintf(op_fh, "DCR H\n");
    cpu->h -= 1;
    set_zero_flag(cpu, cpu->h);
    set_sign_flag(cpu, cpu->h);
    set_parity_flag(cpu, cpu->h);
    cpu->acf = !((cpu->h & 0xf) == 0xf);

    cpu->pc += 1;
    break;
  case 0x2d:
    printf("> DCR L\n");
    fprintf(op_fh, "DCR L\n");
    cpu->l -= 1;
    set_zero_flag(cpu, cpu->l);
    set_sign_flag(cpu, cpu->l);
    set_parity_flag(cpu, cpu->l);
    cpu->acf = !((cpu->l & 0xf) == 0xf);
    cpu->pc += 1;
    break;
  case 0x35:
    printf("> DCR M\n");
    fprintf(op_fh, "DCR M\n");
    cpu->memory[cpu->h << 8 | cpu->l] -= 1;

    set_zero_flag(cpu, cpu->memory[cpu->h << 8 | cpu->l]);
    set_sign_flag(cpu, cpu->memory[cpu->h << 8 | cpu->l]);
    set_parity_flag(cpu, cpu->memory[cpu->h << 8 | cpu->l]);
    cpu->acf = !((cpu->memory[cpu->h << 8 | cpu->l] & 0xf) == 0xf);

    cpu->pc += 1;
    break;
  case 0x3:
    printf("> INX BC\n");
    fprintf(op_fh, "INX BC\n");
    uint16_t bc = cpu->b << 8 | cpu->c;
    bc += 1;
    cpu->b = bc >> 8;
    cpu->c = bc;
    cpu->pc += 1;
    break;
  case 0x13:
    printf("> INX DE\n");
    fprintf(op_fh, "INX DE\n");
    uint16_t de = cpu->d << 8 | cpu->e;
    de += 1;
    cpu->d = de >> 8;
    cpu->e = de;
    cpu->pc += 1;
    break;
  case 0x23:
    printf("> INX HL\n");
    fprintf(op_fh, "INX HL\n");
    uint16_t hl = cpu->h << 8 | cpu->l;
    hl += 1;
    cpu->h = hl >> 8;
    cpu->l = hl;
    cpu->pc += 1;
    break;
  case 0x33:
    printf("> INX SP\n");
    fprintf(op_fh, "INX SP\n");
    cpu->sp += 1;
    cpu->pc += 1;
    break;
  case 0xb:
    printf("> DCX BC\n");
    fprintf(op_fh, "DCX BC\n");
    uint16_t bc_2 = cpu->b << 8 | cpu->c;
    bc_2 -= 1;
    cpu->b = bc_2 >> 8;
    cpu->c = bc_2;
    cpu->pc += 1;
    break;
  case 0x1b:
    printf("> DCX DE\n");
    fprintf(op_fh, "DCX DE\n");
    uint16_t de_2 = cpu->d << 8 | cpu->e;
    de_2 -= 1;
    cpu->d = de_2 >> 8;
    cpu->e = de_2;
    cpu->pc += 1;
    break;
  case 0x2b:
    printf("> DCX HL\n");
    fprintf(op_fh, "DCX HL\n");
    uint16_t hl_2 = cpu->h << 8 | cpu->l;
    hl_2 -= 1;
    cpu->h = hl_2 >> 8;
    cpu->l = hl_2;
    cpu->pc += 1;
    break;
  case 0x3b:
    printf("> DCX SP\n");
    fprintf(op_fh, "DCX SP\n");
    cpu->sp -= 1;
    cpu->pc += 1;
    break;
  case 0x9:
    printf("> DAD BC\n");
    fprintf(op_fh, "DAD BC\n");
    uint16_t bc_3 = cpu->b << 8 | cpu->c;
    uint16_t hl_3 = cpu->h << 8 | cpu->l;
    uint32_t sum = bc_3 + hl_3;
    printf("sum: %x %x %x %x %x \n", cpu->h, cpu->l, cpu->b, cpu->c, sum);

    cpu->h = sum >> 8;
    cpu->l = sum;
    cpu->cf = sum >= 0xffff ? 1 : 0;
    cpu->pc += 1;
    break;
  case 0x19:
    printf("> DAD DE\n");
    fprintf(op_fh, "DAD DE\n");
    uint16_t de_3 = cpu->d << 8 | cpu->e;
    uint16_t hl_4 = cpu->h << 8 | cpu->l;
    uint32_t sum_2 = de_3 + hl_4;
    cpu->h = sum_2 >> 8;
    cpu->l = sum_2;
    cpu->cf = sum_2 > 0xffff ? 1 : 0;
    cpu->pc += 1;
    break;
  case 0x29:
    printf("> DAD HL\n");
    fprintf(op_fh, "DAD HL\n");
    uint16_t hl_5 = cpu->h << 8 | cpu->l;
    uint32_t sum_3 = hl_5 + hl_5;
    cpu->h = sum_3 >> 8;
    cpu->l = sum_3;
    cpu->cf = sum_3 > 0xffff ? 1 : 0;
    cpu->pc += 1;
    break;
  case 0x39:
    printf("> DAD SP\n");
    fprintf(op_fh, "DAD SP\n");
    uint16_t hl_6 = cpu->h << 8 | cpu->l;
    uint32_t sum_4 = hl_6 + cpu->sp;
    cpu->h = sum_4 >> 8;
    cpu->l = sum_4;
    cpu->cf = sum_4 > 0xffff ? 1 : 0;
    cpu->pc += 1;
    break;
  case 0x27:
    printf("> DAA \n");
    fprintf(op_fh, "DAA\n");
    uint8_t daa_tmp_a = cpu->a;
    uint8_t correction =
        0; // NOTE: ignore this variable, only used for acf calculation
    uint8_t daa_higher_4_bits = cpu->a >> 4;
    uint8_t daa_lower_4_bits = cpu->a & 0x0f;
    if (daa_lower_4_bits > 9 || cpu->acf == 1) {
      cpu->a += 6;
      correction += 0x06;
    }
    daa_higher_4_bits = cpu->a >> 4;

    if (daa_higher_4_bits > 9 || cpu->cf == 1) {
      daa_higher_4_bits += 6;
      cpu->a = daa_higher_4_bits << 4 | cpu->a & 0x0f;
      cpu->cf = 1;
      correction += 0x60;
    }

    // set cf,acf and zsp flags
    // to find if cf needs to set to 1 for additions
    // store actual addition result in int16_t variable
    // and check if its > 0xff
    // int16_t true_result =
    set_zero_flag(cpu, cpu->a);
    set_sign_flag(cpu, cpu->a);
    set_parity_flag(cpu, cpu->a);
    set_auxiliary_carry_flag(cpu, daa_tmp_a, correction, 0);

    cpu->pc += 1;
    break;

  case 0xa7:
    printf("> ANA A\n");
    fprintf(op_fh, "ANA A\n");
    uint8_t tmp_a_28 = cpu->a;
    cpu->a = cpu->a & cpu->a;

    set_zero_flag(cpu, cpu->a);
    set_parity_flag(cpu, cpu->a);
    set_sign_flag(cpu, cpu->a);
    cpu->cf = 0;
    cpu->acf = ((tmp_a_28 | tmp_a_28) & 0x08) != 0;

    cpu->pc += 1;
    break;
  case 0xa0:
    printf("> ANA B\n");
    fprintf(op_fh, "ANA B\n");
    uint8_t tmp_a_29 = cpu->a;
    cpu->a = cpu->a & cpu->b;

    set_zero_flag(cpu, cpu->a);
    set_parity_flag(cpu, cpu->a);
    set_sign_flag(cpu, cpu->a);
    cpu->cf = 0;
    cpu->acf = ((tmp_a_29 | cpu->b) & 0x08) != 0;

    cpu->pc += 1;
    break;
  case 0xa1:
    printf("> ANA C\n");
    fprintf(op_fh, "ANA C\n");
    uint8_t tmp_a_30 = cpu->a;
    cpu->a = cpu->a & cpu->c;

    set_zero_flag(cpu, cpu->a);
    set_parity_flag(cpu, cpu->a);
    set_sign_flag(cpu, cpu->a);
    cpu->cf = 0;
    cpu->acf = ((tmp_a_30 | cpu->c) & 0x08) != 0;

    cpu->pc += 1;
    break;
  case 0xa2:
    printf("> ANA D\n");
    fprintf(op_fh, "ANA D\n");
    uint8_t tmp_a_31 = cpu->a;
    cpu->a = cpu->a & cpu->d;

    set_zero_flag(cpu, cpu->a);
    set_parity_flag(cpu, cpu->a);
    set_sign_flag(cpu, cpu->a);
    cpu->cf = 0;
    cpu->acf = ((tmp_a_31 | cpu->d) & 0x08) != 0;

    cpu->pc += 1;
    break;
  case 0xa3:
    printf("> ANA E\n");
    fprintf(op_fh, "ANA E\n");
    uint8_t tmp_a_32 = cpu->a;
    cpu->a = cpu->a & cpu->e;

    set_zero_flag(cpu, cpu->a);
    set_parity_flag(cpu, cpu->a);
    set_sign_flag(cpu, cpu->a);
    cpu->cf = 0;
    cpu->acf = ((tmp_a_32 | cpu->e) & 0x08) != 0;

    cpu->pc += 1;
    break;
  case 0xa4:
    printf("> ANA H\n");
    fprintf(op_fh, "ANA H\n");
    uint8_t tmp_a_33 = cpu->a;
    cpu->a = cpu->a & cpu->h;

    set_zero_flag(cpu, cpu->a);
    set_parity_flag(cpu, cpu->a);
    set_sign_flag(cpu, cpu->a);
    cpu->cf = 0;
    cpu->acf = ((tmp_a_33 | cpu->h) & 0x08) != 0;

    cpu->pc += 1;
    break;
  case 0xa5:
    printf("> ANA L\n");
    fprintf(op_fh, "ANA L\n");
    uint8_t tmp_a_34 = cpu->a;
    cpu->a = cpu->a & cpu->l;

    set_zero_flag(cpu, cpu->a);
    set_parity_flag(cpu, cpu->a);
    set_sign_flag(cpu, cpu->a);
    cpu->cf = 0;
    cpu->acf = ((tmp_a_34 | cpu->l) & 0x08) != 0;

    cpu->pc += 1;
    break;
  case 0xa6:
    printf("> ANA M\n");
    fprintf(op_fh, "ANA M\n");
    uint8_t tmp_a_35 = cpu->a;
    cpu->a = cpu->a & cpu->memory[cpu->h << 8 | cpu->l];
    set_zero_flag(cpu, cpu->a);
    set_parity_flag(cpu, cpu->a);
    set_sign_flag(cpu, cpu->a);
    cpu->cf = 0;
    cpu->acf = ((tmp_a_35 | (cpu->memory[cpu->h << 8 | cpu->l])) & 0x08) != 0;

    cpu->pc += 1;
    break;

  case 0xe6:
    printf("> ANI data\n");
    fprintf(op_fh, "ANI data\n");
    cpu->a = cpu->a & cpu->memory[cpu->pc + 1];
    cpu->pc += 2;
    cpu->cf = 0;
    set_zero_flag(cpu, cpu->a);
    set_parity_flag(cpu, cpu->a);
    set_sign_flag(cpu, cpu->a);
    set_auxiliary_carry_flag(cpu, cpu->a, cpu->memory[cpu->pc + 1], cpu->cf);

    break;
  case 0xaf:
    printf("> XRA A\n");
    fprintf(op_fh, "XRA A\n");
    cpu->a = cpu->a ^ cpu->a;
    cpu->pc += 1;
    cpu->cf = 0;
    cpu->acf = 0;
    set_zero_flag(cpu, cpu->a);
    set_parity_flag(cpu, cpu->a);
    set_sign_flag(cpu, cpu->a);
    break;
  case 0xa8:
    printf("> XRA B\n");
    fprintf(op_fh, "XRA B\n");
    cpu->a = cpu->a ^ cpu->b;
    cpu->pc += 1;
    cpu->cf = 0;
    cpu->acf = 0;
    set_zero_flag(cpu, cpu->a);
    set_parity_flag(cpu, cpu->a);
    set_sign_flag(cpu, cpu->a);
    break;
  case 0xa9:
    printf("> XRA C\n");
    fprintf(op_fh, "XRA C\n");
    cpu->a = cpu->a ^ cpu->c;
    cpu->pc += 1;
    cpu->cf = 0;
    cpu->acf = 0;
    set_zero_flag(cpu, cpu->a);
    set_parity_flag(cpu, cpu->a);
    set_sign_flag(cpu, cpu->a);
    break;
  case 0xaa:
    printf("> XRA D\n");
    fprintf(op_fh, "XRA D\n");
    cpu->a = cpu->a ^ cpu->d;
    cpu->pc += 1;
    cpu->cf = 0;
    cpu->acf = 0;
    set_zero_flag(cpu, cpu->a);
    set_parity_flag(cpu, cpu->a);
    set_sign_flag(cpu, cpu->a);
    break;
  case 0xab:
    printf("> XRA E\n");
    fprintf(op_fh, "XRA E\n");
    cpu->a = cpu->a ^ cpu->e;
    cpu->pc += 1;
    cpu->cf = 0;
    cpu->acf = 0;
    set_zero_flag(cpu, cpu->a);
    set_parity_flag(cpu, cpu->a);
    set_sign_flag(cpu, cpu->a);
    break;
  case 0xac:
    printf("> XRA H\n");
    fprintf(op_fh, "XRA H\n");
    cpu->a = cpu->a ^ cpu->h;
    cpu->pc += 1;
    cpu->cf = 0;
    cpu->acf = 0;
    set_zero_flag(cpu, cpu->a);
    set_parity_flag(cpu, cpu->a);
    set_sign_flag(cpu, cpu->a);
    break;
  case 0xad:
    printf("> XRA L\n");
    fprintf(op_fh, "XRA L\n");
    cpu->a = cpu->a ^ cpu->l;
    cpu->pc += 1;
    cpu->cf = 0;
    cpu->acf = 0;
    set_zero_flag(cpu, cpu->a);
    set_parity_flag(cpu, cpu->a);
    set_sign_flag(cpu, cpu->a);
    break;
  case 0xae:
    printf("> XRA M\n");
    fprintf(op_fh, "XRA M\n");
    cpu->a = cpu->a ^ cpu->memory[cpu->h << 8 | cpu->l];
    cpu->pc += 1;
    cpu->cf = 0;
    cpu->acf = 0;
    set_zero_flag(cpu, cpu->a);
    set_parity_flag(cpu, cpu->a);
    set_sign_flag(cpu, cpu->a);
    break;
  case 0xee:
    printf("> XRI data\n");
    fprintf(op_fh, "XRI data\n");
    cpu->a = cpu->a ^ cpu->memory[cpu->pc + 1];
    cpu->cf = 0;
    cpu->acf = 0;
    set_parity_flag(cpu, cpu->a);
    set_zero_flag(cpu, cpu->a);
    set_sign_flag(cpu, cpu->a);

    cpu->pc += 2;
    break;
  case 0xb7:
    printf("> ORA A\n");
    fprintf(op_fh, "ORA A\n");
    cpu->a = cpu->a | cpu->a;
    cpu->cf = 0;
    cpu->acf = 0;
    set_zero_flag(cpu, cpu->a);
    set_sign_flag(cpu, cpu->a);
    set_parity_flag(cpu, cpu->a);
    cpu->pc += 1;
    break;
  case 0xb0:
    printf("> ORA B\n");
    fprintf(op_fh, "ORA B\n");
    cpu->a = cpu->a | cpu->b;
    cpu->cf = 0;
    cpu->acf = 0;
    set_zero_flag(cpu, cpu->a);
    set_sign_flag(cpu, cpu->a);
    set_parity_flag(cpu, cpu->a);
    cpu->pc += 1;
    break;
  case 0xb1:
    printf("> ORA C\n");
    fprintf(op_fh, "ORA C\n");
    cpu->a = cpu->a | cpu->c;
    cpu->cf = 0;
    cpu->acf = 0;
    set_zero_flag(cpu, cpu->a);
    set_sign_flag(cpu, cpu->a);
    set_parity_flag(cpu, cpu->a);
    cpu->pc += 1;
    break;
  case 0xb2:
    printf("> ORA D\n");
    fprintf(op_fh, "ORA D\n");
    cpu->a = cpu->a | cpu->d;
    cpu->cf = 0;
    cpu->acf = 0;
    set_zero_flag(cpu, cpu->a);
    set_sign_flag(cpu, cpu->a);
    set_parity_flag(cpu, cpu->a);
    cpu->pc += 1;
    break;
  case 0xb3:
    printf("> ORA E\n");
    fprintf(op_fh, "ORA E\n");
    cpu->a = cpu->a | cpu->e;
    cpu->cf = 0;
    cpu->acf = 0;
    set_zero_flag(cpu, cpu->a);
    set_sign_flag(cpu, cpu->a);
    set_parity_flag(cpu, cpu->a);
    cpu->pc += 1;
    break;
  case 0xb4:
    printf("> ORA H\n");
    fprintf(op_fh, "ORA H\n");
    cpu->a = cpu->a | cpu->h;
    cpu->cf = 0;
    cpu->acf = 0;
    set_zero_flag(cpu, cpu->a);
    set_sign_flag(cpu, cpu->a);
    set_parity_flag(cpu, cpu->a);
    cpu->pc += 1;
    break;
  case 0xb5:
    printf("> ORA L\n");
    fprintf(op_fh, "ORA L\n");
    cpu->a = cpu->a | cpu->l;
    cpu->cf = 0;
    cpu->acf = 0;
    set_zero_flag(cpu, cpu->a);
    set_sign_flag(cpu, cpu->a);
    set_parity_flag(cpu, cpu->a);
    cpu->pc += 1;
    break;
  case 0xb6:
    printf("> ORA M\n");
    fprintf(op_fh, "ORA M\n");
    cpu->a = cpu->a | cpu->memory[cpu->h << 8 | cpu->l];
    cpu->cf = 0;
    cpu->acf = 0;
    set_zero_flag(cpu, cpu->a);
    set_sign_flag(cpu, cpu->a);
    set_parity_flag(cpu, cpu->a);
    cpu->pc += 1;
    break;

  case 0xf6:
    printf("> ORI data\n");
    fprintf(op_fh, "ORI data\n");
    cpu->a = cpu->a | cpu->memory[cpu->pc + 1];

    set_zero_flag(cpu, cpu->a);
    set_parity_flag(cpu, cpu->a);
    set_sign_flag(cpu, cpu->a);
    cpu->cf = 0;
    cpu->acf = 0;

    cpu->pc += 2;
    break;
  case 0xbf:
    printf("> CMP A\n");
    fprintf(op_fh, "CMP A\n");
    uint16_t cmp_result = cpu->a - cpu->a;
    cpu->cf = cmp_result >> 8;
    set_zero_flag(cpu, cmp_result);
    set_sign_flag(cpu, (uint8_t) cmp_result);
    set_parity_flag(cpu, (uint8_t) cmp_result);
    cpu->acf = ~(cpu->a ^ cmp_result ^ cpu->a) & 0x10;
    cpu->pc += 1;
    break;
  case 0xb8:
    printf("> CMP B\n");
    fprintf(op_fh, "CMP B\n");
    uint16_t cmp_result_2 = cpu->a - cpu->b;
    cpu->cf = cmp_result_2 >> 8;
    set_zero_flag(cpu, cmp_result_2);
    set_sign_flag(cpu, (uint8_t) cmp_result_2);
    set_parity_flag(cpu, (uint8_t) cmp_result_2);
    cpu->acf = ~(cpu->a ^ cmp_result_2 ^ cpu->b) & 0x10;
    cpu->pc += 1;
    break;
  case 0xb9:
    printf("> CMP C\n");
    fprintf(op_fh, "CMP C\n");
    uint16_t cmp_result_3 = cpu->a - cpu->c;
    // set_carry_flag(cpu, cmp_result_3);
    cpu->cf = cmp_result_3 >> 8;

    set_zero_flag(cpu, cmp_result_3);
    set_sign_flag(cpu, (uint8_t) cmp_result_3);
    set_parity_flag(cpu, (uint8_t) cmp_result_3);
    cpu->acf = ~(cpu->a ^ cmp_result_3 ^ cpu->c) & 0x10;
    cpu->pc += 1;
    break;
  case 0xba:
    printf("> CMP D\n");
    fprintf(op_fh, "CMP D\n");
    uint16_t cmp_result_4 = cpu->a - cpu->d;
    // set_carry_flag(cpu, cmp_result_4);
    cpu->cf = cmp_result_4 >> 8;
    set_zero_flag(cpu, cmp_result_4);
    set_sign_flag(cpu, (uint8_t) cmp_result_4);
    set_parity_flag(cpu, (uint8_t) cmp_result_4);
    cpu->acf = ~(cpu->a ^ cmp_result_4 ^ cpu->d) & 0x10;
    cpu->pc += 1;
    break;
  case 0xbb:
    printf("> CMP E\n");
    fprintf(op_fh, "CMP E\n");
    uint16_t cmp_result_5 = cpu->a - cpu->e;
    // set_carry_flag(cpu, cmp_result_5);
    cpu->cf = cmp_result_5 >> 8;
    set_zero_flag(cpu, cmp_result_5);
    set_sign_flag(cpu, (uint8_t) cmp_result_5);
    set_parity_flag(cpu, (uint8_t) cmp_result_5);
    cpu->acf = ~(cpu->a ^ cmp_result_5 ^ cpu->e) & 0x10;
    cpu->pc += 1;
    break;
  case 0xbc:
    printf("> CMP H\n");
    fprintf(op_fh, "CMP H\n");
    uint16_t cmp_result_6 = cpu->a - cpu->h;
    // set_carry_flag(cpu, cmp_result_6);
    cpu->cf = cmp_result_6 >> 8;
    set_zero_flag(cpu, cmp_result_6);
    set_sign_flag(cpu, (uint8_t) cmp_result_6);
    set_parity_flag(cpu, (uint8_t) cmp_result_6);
    cpu->acf = ~(cpu->a ^ cmp_result_6 ^ cpu->h) & 0x10;
    cpu->pc += 1;
    break;
  case 0xbd:
    printf("> CMP L\n");
    fprintf(op_fh, "CMP L\n");
    uint16_t cmp_result_7 = cpu->a - cpu->l;
    // set_carry_flag(cpu, cmp_result_7);
    cpu->cf = cmp_result_7 >> 8;
    set_zero_flag(cpu, cmp_result_7);
    set_sign_flag(cpu, (uint8_t) cmp_result_7);
    set_parity_flag(cpu, (uint8_t) cmp_result_7);
    cpu->acf = ~(cpu->a ^ cmp_result_7 ^ cpu->l) & 0x10;
    cpu->pc += 1;
    break;
  case 0xbe:
    printf("> CMP M\n");
    fprintf(op_fh, "CMP M\n");
    uint16_t cmp_result_8 = cpu->a - cpu->memory[cpu->h << 8 | cpu->l];
    // set_carry_flag(cpu, cmp_result_8);
    cpu->cf = cmp_result_8 >> 8;
    set_zero_flag(cpu, cmp_result_8);
    set_sign_flag(cpu, (uint8_t) cmp_result_8);
    set_parity_flag(cpu, (uint8_t) cmp_result_8);
    cpu->acf =
        ~(cpu->a ^ cmp_result_8 ^ (cpu->memory[cpu->h << 8 | cpu->l])) & 0x10;
    cpu->pc += 1;
    break;
  case 0xfe:
    printf("> CPI data\n");
    fprintf(op_fh, "CPI data\n");
    uint16_t cmp_result_9 = cpu->a - cpu->memory[cpu->pc + 1];
    set_zero_flag(cpu, cmp_result_9);

    // set_carry_flag(cpu, tmp_a + cpu->memory[cpu->pc + 1]);
    // TODO: use generic function above, but this is different
    // set_carry_flag(cpu, cmp_result_9);
    cpu->cf = cpu->cf >> 8;
    if (cpu->a < cpu->memory[cpu->pc + 1]) {
      cpu->cf = 1;
    }

    // set_auxiliary_carry_flag(cpu, tmp_a, cpu->memory[cpu->pc + 1], 4, 0);
    // TODO: use generic function aboe, but this is different
    cpu->acf = ~(cpu->a ^ cmp_result_9 ^ cpu->memory[cpu->pc + 1]) & 0x10;

    set_sign_flag(cpu, cmp_result_9);
    set_parity_flag(cpu, (uint8_t) cmp_result_9);

    cpu->pc += 2;
    break;
  case 0x7:
    printf("> RLC\n");
    fprintf(op_fh, "RLC\n");
    cpu->cf = (cpu->a) >> 7 & 0b00000001;
    cpu->a = (cpu->a) << 1 | cpu->cf;
    cpu->pc += 1;
    break;
  case 0xf:
    printf("> RRC\n");
    fprintf(op_fh, "RRC\n");
    cpu->cf = cpu->a & 0b00000001;
    cpu->a = (cpu->a) >> 1 | cpu->cf << 7;
    cpu->pc += 1;
    break;
  case 0x17:
    printf("> RAL\n");
    fprintf(op_fh, "RAL\n");
    bool tmp_carry = cpu->cf;
    cpu->cf = (cpu->a) >> 7 & 0b00000001;
    cpu->a = (cpu->a) << 1 | tmp_carry;
    cpu->pc += 1;
    break;
  case 0x1f:
    printf("> RAR\n");
    fprintf(op_fh, "RAR\n");
    bool tmp_carry_2 = cpu->cf;
    cpu->cf = cpu->a & 0b00000001;
    cpu->a = (cpu->a) >> 1 | tmp_carry_2 << 7;
    cpu->pc += 1;
    break;
  case 0x2f:
    printf("> CMA\n");
    fprintf(op_fh, "CMA\n");
    cpu->a = ~cpu->a;
    cpu->pc += 1;
    break;
  case 0x3f:
    printf("> CMC\n");
    fprintf(op_fh, "CMC\n");
    cpu->cf = !cpu->cf;
    cpu->pc += 1;
    break;
  case 0x37:
    printf("> STC\n");
    fprintf(op_fh, "STC\n");
    cpu->cf = 1;
    cpu->pc += 1;
    break;
  case 0xc3:
    printf("JMP addr");
    fprintf(op_fh, "JMP addr");
    cpu->pc = cpu->memory[cpu->pc + 2] << 8 | cpu->memory[cpu->pc + 1];
    break;
  case 0xc2:
    printf("Jcondition addr\n");
    fprintf(op_fh, "Jcondition addr\n");
    if (cpu->zf == 0) {
      cpu->pc = cpu->memory[cpu->pc + 2] << 8 | cpu->memory[cpu->pc + 1];
    } else {
      cpu->pc += 3;
    }
    break;
  case 0xca:
    printf("Jcondition addr");
    fprintf(op_fh, "Jcondition addr");
    if (cpu->zf == 1) {
      cpu->pc = cpu->memory[cpu->pc + 2] << 8 | cpu->memory[cpu->pc + 1];
    } else {
      cpu->pc += 3;
    }
    break;
  case 0xd2:
    printf("Jcondition addr");
    fprintf(op_fh, "Jcondition addr");
    if (cpu->cf == 0) {
      cpu->pc = cpu->memory[cpu->pc + 2] << 8 | cpu->memory[cpu->pc + 1];
    } else {
      cpu->pc += 3;
    }
    break;
  case 0xda:
    printf("Jcondition addr");
    fprintf(op_fh, "Jcondition addr");
    if (cpu->cf == 1) {
      cpu->pc = cpu->memory[cpu->pc + 2] << 8 | cpu->memory[cpu->pc + 1];
    } else {
      cpu->pc += 3;
    }
    break;
  case 0xe2:
    printf("Jcondition addr");
    fprintf(op_fh, "Jcondition addr");
    if (cpu->pf == 0) {
      cpu->pc = cpu->memory[cpu->pc + 2] << 8 | cpu->memory[cpu->pc + 1];
    } else {
      cpu->pc += 3;
    }
    break;
  case 0xea:
    printf("Jcondition addr");
    fprintf(op_fh, "Jcondition addr");
    if (cpu->pf == 1) {
      cpu->pc = cpu->memory[cpu->pc + 2] << 8 | cpu->memory[cpu->pc + 1];
    } else {
      cpu->pc += 3;
    }
    break;
  case 0xf2:
    printf("Jcondition addr");
    fprintf(op_fh, "Jcondition addr");
    if (cpu->sf == 0) {
      cpu->pc = cpu->memory[cpu->pc + 2] << 8 | cpu->memory[cpu->pc + 1];
    } else {
      cpu->pc += 3;
    }
    break;
  case 0xfa:
    printf("Jcondition addr");
    fprintf(op_fh, "Jcondition addr");
    if (cpu->sf == 1) {
      cpu->pc = cpu->memory[cpu->pc + 2] << 8 | cpu->memory[cpu->pc + 1];
    } else {
      cpu->pc += 3;
    }
    break;
  case 0xcd:
    printf("Jcondition addr");
    fprintf(op_fh, "Jcondition addr");
    cpu->pc += 3;
    cpu->memory[cpu->sp - 1] = cpu->pc >> 8;
    cpu->memory[cpu->sp - 2] = cpu->pc & 0xFF;
    // printf("cpu writing memory %x %x= %x %x= %x", cpu->pc ,cpu->sp-1, cpu->pc
    // >> 8,
    //  cpu->sp-2, cpu->pc & 0xFF);

    cpu->sp -= 2;
    cpu->pc = cpu->memory[cpu->pc - 1] << 8 | cpu->memory[cpu->pc - 2];
    break;
  case 0xc4:
    printf("Ccondition addr");
    fprintf(op_fh, "Ccondition addr");
    if (cpu->zf == 0) {
      uint16_t next_two_bytes =
          cpu->memory[cpu->pc + 2] << 8 | cpu->memory[cpu->pc + 1];
      cpu->pc += 3;
      cpu->memory[cpu->sp - 1] = cpu->pc >> 8;
      cpu->memory[cpu->sp - 2] = cpu->pc & 0xff;
      cpu->sp -= 2;
      cpu->pc = next_two_bytes;
    } else {
      cpu->pc += 3;
    }
    break;
  case 0xcc:
    printf("Ccondition addr");
    fprintf(op_fh, "Ccondition addr");
    if (cpu->zf == 1) {
      uint16_t next_two_bytes =
          cpu->memory[cpu->pc + 2] << 8 | cpu->memory[cpu->pc + 1];
      cpu->pc += 3;
      cpu->memory[cpu->sp - 1] = cpu->pc >> 8;
      cpu->memory[cpu->sp - 2] = cpu->pc & 0xff;
      cpu->sp -= 2;
      cpu->pc = next_two_bytes;
    } else {
      cpu->pc += 3;
    }
    break;
  case 0xd4:
    printf("Ccondition addr");
    fprintf(op_fh, "Ccondition addr");
    if (cpu->cf == 0) {
      uint16_t next_two_bytes =
          cpu->memory[cpu->pc + 2] << 8 | cpu->memory[cpu->pc + 1];
      cpu->pc += 3;
      cpu->memory[cpu->sp - 1] = cpu->pc >> 8;
      cpu->memory[cpu->sp - 2] = cpu->pc & 0xff;
      cpu->sp -= 2;
      cpu->pc = next_two_bytes;
    } else {
      cpu->pc += 3;
    }
    break;
  case 0xdc:
    printf("Ccondition addr");
    fprintf(op_fh, "Ccondition addr");
    if (cpu->cf == 1) {
      uint16_t next_two_bytes =
          cpu->memory[cpu->pc + 2] << 8 | cpu->memory[cpu->pc + 1];
      cpu->pc += 3;
      cpu->memory[cpu->sp - 1] = cpu->pc >> 8;
      cpu->memory[cpu->sp - 2] = cpu->pc & 0xff;
      cpu->sp -= 2;
      cpu->pc = next_two_bytes;
    } else {
      cpu->pc += 3;
    }
    break;
  case 0xe4:
    printf("Ccondition addr");
    fprintf(op_fh, "Ccondition addr");
    // cpu->pc += 1;
    if (cpu->pf == 0) {
      uint16_t next_two_bytes =
          cpu->memory[cpu->pc + 2] << 8 | cpu->memory[cpu->pc + 1];
      cpu->pc += 3;
      cpu->memory[cpu->sp - 1] = cpu->pc >> 8;
      cpu->memory[cpu->sp - 2] = cpu->pc & 0xff;
      cpu->sp -= 2;
      cpu->pc = next_two_bytes;
    } else {
      cpu->pc += 3;
    }
    break;
  case 0xec:
    printf("Ccondition addr");
    fprintf(op_fh, "Ccondition addr");
    if (cpu->pf == 1) {
      uint16_t next_two_bytes =
          cpu->memory[cpu->pc + 2] << 8 | cpu->memory[cpu->pc + 1];
      cpu->pc += 3;
      cpu->memory[cpu->sp - 1] = cpu->pc >> 8;
      cpu->memory[cpu->sp - 2] = cpu->pc & 0xff;
      cpu->sp -= 2;
      cpu->pc = next_two_bytes;
    } else {
      cpu->pc += 3;
    }
    break;
  case 0xf4:
    printf("Ccondition addr");
    fprintf(op_fh, "Ccondition addr");
    if (cpu->sf == 0) {
      uint16_t next_two_bytes =
          cpu->memory[cpu->pc + 2] << 8 | cpu->memory[cpu->pc + 1];
      cpu->pc += 3;

      cpu->memory[cpu->sp - 1] = cpu->pc >> 8;
      cpu->memory[cpu->sp - 2] = cpu->pc & 0xff;
      cpu->sp -= 2;
      cpu->pc = next_two_bytes;
    } else {
      cpu->pc += 3;
    }
    break;
  case 0xfc:
    printf("Ccondition addr");
    fprintf(op_fh, "Ccondition addr");
    if (cpu->sf == 1) {
      uint16_t next_two_bytes =
          cpu->memory[cpu->pc + 2] << 8 | cpu->memory[cpu->pc + 1];
      cpu->pc += 3;
      cpu->memory[cpu->sp - 1] = cpu->pc >> 8;
      cpu->memory[cpu->sp - 2] = cpu->pc & 0xff;
      cpu->sp -= 2;
      cpu->pc = next_two_bytes;
    } else {
      cpu->pc += 3;
    }
    break;
  case 0xc9:
    printf("> RET\n");
    printf("memory at %x %x %x", cpu->memory[cpu->sp], cpu->memory[cpu->sp + 1],
        cpu->memory[0x7b7]);
    cpu->pc = (cpu->memory[cpu->sp + 1] << 8) | cpu->memory[cpu->sp];
    cpu->sp += 2;
    break;
  case 0xc0:
    printf("Rcondition addr");
    fprintf(op_fh, "Rcondition addr");

    if (cpu->zf == 0) {
      cpu->pc = cpu->memory[cpu->sp] | cpu->memory[cpu->sp + 1] << 8;
      cpu->sp += 2;
    } else {
      cpu->pc += 1;
    }
    break;
  case 0xc8:
    printf("Rcondition addr");
    fprintf(op_fh, "Rcondition addr");
    if (cpu->zf == 1) {
      cpu->pc = cpu->memory[cpu->sp] | cpu->memory[cpu->sp + 1] << 8;
      cpu->sp += 2;
    } else {
      cpu->pc += 1;
    }
    break;
  case 0xd0:
    printf("Rcondition addr");
    fprintf(op_fh, "Rcondition addr");
    if (cpu->cf == 0) {
      cpu->pc = cpu->memory[cpu->sp] | cpu->memory[cpu->sp + 1] << 8;
      cpu->sp += 2;
    } else {
      cpu->pc += 1;
    }
    break;
  case 0xd8:
    printf("Rcondition addr");
    fprintf(op_fh, "Rcondition addr");
    if (cpu->cf == 1) {
      cpu->pc = cpu->memory[cpu->sp] | cpu->memory[cpu->sp + 1] << 8;
      cpu->sp += 2;
    } else {
      cpu->pc += 1;
    }
    break;
  case 0xe0:
    printf("Rcondition addr");
    fprintf(op_fh, "Rcondition addr");
    if (cpu->pf == 0) {
      cpu->pc = cpu->memory[cpu->sp] | cpu->memory[cpu->sp + 1] << 8;
      cpu->sp += 2;
    } else {
      cpu->pc += 1;
    }
    break;
  case 0xe8:
    printf("Rcondition addr");
    fprintf(op_fh, "Rcondition addr");
    if (cpu->pf == 1) {
      cpu->pc = cpu->memory[cpu->sp] | cpu->memory[cpu->sp + 1] << 8;
      cpu->sp += 2;
    } else {
      cpu->pc += 1;
    }
    break;
  case 0xf0:
    printf("Rcondition addr");
    fprintf(op_fh, "Rcondition addr");
    if (cpu->sf == 0) {
      cpu->pc = cpu->memory[cpu->sp] | cpu->memory[cpu->sp + 1] << 8;
      cpu->sp += 2;
    } else {
      cpu->pc += 1;
    }
    break;
  case 0xf8:
    printf("Rcondition addr");
    fprintf(op_fh, "Rcondition addr");
    if (cpu->sf == 1) {
      cpu->pc = cpu->memory[cpu->sp] | cpu->memory[cpu->sp + 1] << 8;
      cpu->sp += 2;
    } else {
      cpu->pc += 1;
    }
    break;
  case 0xc7:
    printf("> RST 0\n");
    fprintf(op_fh, "RST 0\n");
    cpu->memory[cpu->sp - 1] = cpu->pc >> 8;
    cpu->memory[cpu->sp - 2] = cpu->pc & 0xff;
    cpu->sp -= 2;
    cpu->pc = 8 * 0;
    break;
  case 0xcf:
    printf("> RST 1\n");
    fprintf(op_fh, "RST 1\n");
    cpu->memory[cpu->sp - 1] = cpu->pc >> 8;
    cpu->memory[cpu->sp - 2] = cpu->pc & 0xff;
    cpu->sp -= 2;
    cpu->pc = 8 * 1;
    break;
  case 0xd7:
    printf("> RST 2\n");
    fprintf(op_fh, "RST 2\n");
    cpu->memory[cpu->sp - 1] = cpu->pc >> 8;
    cpu->memory[cpu->sp - 2] = cpu->pc & 0xff;
    cpu->sp -= 2;
    cpu->pc = 8 * 2;
    break;
  case 0xdf:
    printf("> RST 3\n");
    fprintf(op_fh, "RST 3\n");
    cpu->memory[cpu->sp - 1] = cpu->pc >> 8;
    cpu->memory[cpu->sp - 2] = cpu->pc & 0xff;
    cpu->sp -= 2;
    cpu->pc = 8 * 3;
    break;
  case 0xe7:
    printf("> RST 4\n");
    fprintf(op_fh, "RST 4\n");
    cpu->memory[cpu->sp - 1] = cpu->pc >> 8;
    cpu->memory[cpu->sp - 2] = cpu->pc & 0xff;
    cpu->sp -= 2;
    cpu->pc = 8 * 4;
    break;
  case 0xef:
    printf("> RST 5\n");
    fprintf(op_fh, "RST 5\n");
    cpu->memory[cpu->sp - 1] = cpu->pc >> 8;
    cpu->memory[cpu->sp - 2] = cpu->pc & 0xff;
    cpu->sp -= 2;
    cpu->pc = 8 * 5;
    break;
  case 0xf7:
    printf("> RST 6\n");
    fprintf(op_fh, "RST 6\n");
    cpu->memory[cpu->sp - 1] = cpu->pc >> 8;
    cpu->memory[cpu->sp - 2] = cpu->pc & 0xff;
    cpu->sp -= 2;
    cpu->pc = 8 * 6;
    break;
  case 0xff:
    printf("> RST 7\n");
    fprintf(op_fh, "RST 7\n");
    cpu->memory[cpu->sp - 1] = cpu->pc >> 8;
    cpu->memory[cpu->sp - 2] = cpu->pc & 0xff;
    cpu->sp -= 2;
    cpu->pc = 8 * 7;
    break;

  case 0xe9:
    printf("> PCHL\n");
    fprintf(op_fh, "PCHL\n");
    cpu->pc = cpu->h << 8 | cpu->l;
    break;

  case 0xc5:
    printf("> PUSH BC \n");
    fprintf(op_fh, "PUSH BC\n");
    cpu->memory[cpu->sp - 1] = cpu->b;
    cpu->memory[cpu->sp - 2] = cpu->c;
    cpu->sp -= 2;
    cpu->pc += 1;
    break;
  case 0xd5:
    printf("> PUSH DE\n");
    fprintf(op_fh, "PUSH DE\n");
    cpu->memory[cpu->sp - 1] = cpu->d;
    cpu->memory[cpu->sp - 2] = cpu->e;
    cpu->sp -= 2;
    cpu->pc += 1;
    break;
  case 0xe5:
    printf("> PUSH HL\n");
    fprintf(op_fh, "PUSH HL\n");
    cpu->memory[cpu->sp - 1] = cpu->h;
    cpu->memory[cpu->sp - 2] = cpu->l;
    cpu->sp -= 2;
    cpu->pc += 1;
    break;

  case 0xf5:
    printf("> PUSH PSW\n");
    fprintf(op_fh, "PUSH PSW \n");
    printf("sp %d ", cpu->sp);
    cpu->memory[cpu->sp - 1] = cpu->a;
    uint8_t psw = 0;
    psw = cpu->sf << 7 | cpu->zf << 6 | cpu->acf << 4 | cpu->pf << 2 | 0b10 |
          cpu->cf;
    cpu->memory[cpu->sp - 2] = psw;
    cpu->sp -= 2;
    cpu->pc += 1;
    break;

  case 0xc1:
    printf("> POP BC\n");
    fprintf(op_fh, "POP BC\n");
    cpu->c = cpu->memory[cpu->sp];
    cpu->b = cpu->memory[cpu->sp + 1];
    cpu->sp += 2;
    cpu->pc += 1;
    break;
  case 0xd1:
    printf("> POP DE\n");
    fprintf(op_fh, "POP DE\n");
    cpu->e = cpu->memory[cpu->sp];
    cpu->d = cpu->memory[cpu->sp + 1];
    cpu->sp += 2;
    cpu->pc += 1;
    break;
  case 0xe1:
    printf("> POP HL\n");
    fprintf(op_fh, "POP HL\n");
    cpu->l = cpu->memory[cpu->sp];
    cpu->h = cpu->memory[cpu->sp + 1];
    cpu->sp += 2;
    cpu->pc += 1;
    break;

  case 0xf1:
    printf("> POP PSW\n");
    fprintf(op_fh, "POP PSW \n");
    cpu->cf = cpu->memory[cpu->sp] & 1;
    cpu->pf = cpu->memory[cpu->sp] >> 2 & 1;
    cpu->acf = cpu->memory[cpu->sp] >> 4 & 1;
    cpu->zf = cpu->memory[cpu->sp] >> 6 & 1;
    cpu->sf = cpu->memory[cpu->sp] >> 7 & 1;
    cpu->a = cpu->memory[cpu->sp + 1];
    cpu->sp += 2;
    cpu->pc += 1;
    break;

  case 0xe3:
    printf("> XTHL\n");
    fprintf(op_fh, "XTHL\n");
    uint8_t tmp = cpu->memory[cpu->sp];
    cpu->memory[cpu->sp] = cpu->l;
    cpu->l = tmp;
    tmp = cpu->memory[cpu->sp + 1];
    cpu->memory[cpu->sp + 1] = cpu->h;
    cpu->h = tmp;
    cpu->pc += 1;
    break;

  case 0xf9:
    printf("> SPHL\n");
    fprintf(op_fh, "SPHL\n");
    cpu->sp = cpu->h << 8 | cpu->l;
    cpu->pc += 1;
    break;

  case 0xdb:
    printf("> IN data\n");
    fprintf(op_fh, "IN data\n");
    cpu->a = cpu->port_in(cpu->memory[cpu->pc + 1]);

    if (cpu->a != 0) {
      printf("\nport data %d\n", cpu->a);
      // sleep(1);
    }
    cpu->pc += 2;
    break;

  case 0xd3:
    printf("> OUT data\n");
    fprintf(op_fh, "OUT data\n");
    uint8_t port = cpu->memory[cpu->pc + 1];
    uint8_t data = cpu->a;
    cpu->port_out(port, data);
    cpu->pc += 2;
    break;

  case 0xfb:
    printf("> EI\n");
    fprintf(op_fh, "EI\n");
    cpu->interrupt_enable = 1;
    cpu->interrupt_delay = 1;
    cpu->pc += 1;
    break;

  case 0xf3:
    printf("> DI\n");
    fprintf(op_fh, "DI\n");
    cpu->interrupt_enable = 0;
    cpu->interrupt_delay = 0;
    cpu->pc += 1;
    break;

  case 0x76:
    printf("> HLT\n");
    fprintf(op_fh, "HLT\n");
    cpu->halted = 1;
    cpu->pc += 1;
    break;

  case 0x0:
    printf("> NOP\n");
    fprintf(op_fh, "NOP\n");
    cpu->pc += 1;
    break;

  case 0x08:
  case 0x10:
  case 0x18:
  case 0x20:
  case 0x28:
  case 0x30:
  case 0x38:
    printf("> undocumented instruction\n");
    fprintf(op_fh, "undocuemnted instruction\n");
    cpu->pc += 1;
    break;

  default: printf("instruction not found: %x\n", instruction); exit(1);
  }
}

void interrupt_cpu(cpu* cpu, uint8_t opcode) {
  cpu->interrupt_enable = 1;
  cpu->interrupt_delay = 0;
  cpu->interrupt_opcode = opcode;
}

void debug_cpu(cpu* cpu) {
  printf("\n------------cpu---------------\n");
  printf("A : %x\n", cpu->a);
  printf("B : %x\n", cpu->b);
  printf("C : %x\n", cpu->c);
  printf("D : %x\n", cpu->d);
  printf("E : %x\n", cpu->e);
  printf("H : %x\n", cpu->h);
  printf("L : %x\n", cpu->l);
  printf("PC: %x\n", cpu->pc);
  printf("ZF: %x\n", cpu->zf);
  printf("CF: %x\n", cpu->cf);
  printf("SF: %x\n", cpu->sf);
  printf("PF: %x\n", cpu->pf);
  printf("ACF: %x\n", cpu->acf);
  printf("SP: %x\n", cpu->sp);
  printf("---------------------------\n");
}

// int main(int argc, char** argv) {
//   printf(
//       "starting program %d %d %d \n ", sizeof(cpu), sizeof(int),
//       sizeof(bool));
//   cpu* cpu = malloc(50);

//   cpu_init(cpu, ROM_NAME);
//   int total_steps = 5;

//   FILE* op_fh = fopen(OUPUT_ASSEMBLY_NAME, "w");

//   for (int i = 0; i < total_steps; i++) {
//     step(cpu, op_fh);
//     debug_cpu(cpu);
//   }

//   free(cpu);
//   printf("\n program finished\n");
//   return 0;
// }
