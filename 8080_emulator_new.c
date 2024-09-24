#include <stdio.h>
#include <string.h>
#include "8080_emulator_new.h"

void load_rom(cpu* cpu) {
  // load the ROM into memory
  printf("loading rom\n");
  fflush(stdout);

  FILE* f = fopen(ROM_NAME, "rb");
  printf("opened file\n");
  fflush(stdout);

  if (f == NULL) {
    fprintf(stderr, "error: can't open file '%s'.\n", ROM_NAME);
    exit(1);
  }
  printf("rom loaded");
  fflush(stdout);

  size_t bytes_read = fread(cpu->memory, 1, 5, f);

  printf("read byte in moemry %x %x %x \n", cpu->memory[0], cpu->memory[1],
      cpu->memory[2]);

  // fclose(f); // TODO: gives error, fix it
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

void cpu_init(cpu* cpu) {
  // load the ROM into memory
  for (int i = 0; i < MEMORY_SIZE; i++) {
    cpu->memory[i] = 0;
  }

  load_rom(cpu);

  init_registers(cpu);
  init_flags(cpu);
}

void set_zero_flag(cpu* cpu, uint8_t result) {
  cpu->zf = result & 0xff == 0 ? 1 : 0;
}
void set_carry_flag(cpu* cpu, uint16_t result) {
  cpu->cf = result > 0xff ? 1 : 0;
}

void step(cpu* cpu, FILE* op_fh) {
  // fetch the instruction using program counter
  int instruction = cpu->memory[cpu->pc];

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
    cpu->b = cpu->a;
    cpu->pc += 1;
    break;
  case 0x79:
    printf("> MOV A, C\n");
    fprintf(op_fh, "MOV A, C\n");
    cpu->c = cpu->a;
    cpu->pc += 1;
    break;
  case 0x7a:
    printf("> MOV A, D\n");
    fprintf(op_fh, "MOV A, D\n");
    cpu->d = cpu->a;
    cpu->pc += 1;
    break;
  case 0x7b:
    printf("> MOV A, E\n");
    fprintf(op_fh, "MOV A, E\n");
    cpu->e = cpu->a;
    cpu->pc += 1;
    break;
  case 0x7c:
    printf("> MOV A, H\n");
    fprintf(op_fh, "MOV A, H\n");
    cpu->h = cpu->a;
    cpu->pc += 1;
    break;
  case 0x7d:
    printf("> MOV A, L\n");
    fprintf(op_fh, "MOV A, L\n");
    cpu->l = cpu->a;
    cpu->pc += 1;
    break;
  case 0x47:
    printf("> MOV B, A\n");
    fprintf(op_fh, "MOV B, A\n");
    cpu->a = cpu->b;
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
    cpu->c = cpu->b;
    cpu->pc += 1;
    break;
  case 0x42:
    printf("> MOV B, D\n");
    fprintf(op_fh, "MOV B, D\n");
    cpu->d = cpu->b;
    cpu->pc += 1;
    break;
  case 0x43:
    printf("> MOV B, E\n");
    fprintf(op_fh, "MOV B, E\n");
    cpu->e = cpu->b;
    cpu->pc += 1;
    break;
  case 0x44:
    printf("> MOV B, H\n");
    fprintf(op_fh, "MOV B, H\n");
    cpu->h = cpu->b;
    cpu->pc += 1;
    break;
  case 0x45:
    printf("> MOV L, B\n");
    fprintf(op_fh, "MOV B, L\n");
    cpu->l = cpu->b;
    cpu->pc += 1;
    break;
  case 0x4f:
    printf("> MOV A, C\n");
    fprintf(op_fh, "MOV C, A\n");
    cpu->a = cpu->c;
    cpu->pc += 1;
    break;
  case 0x48:
    printf("> MOV B, C\n");
    fprintf(op_fh, "MOV C, B\n");
    cpu->b = cpu->c;
    cpu->pc += 1;
    break;
  case 0x49:
    printf("> MOV C, C\n");
    fprintf(op_fh, "MOV C, C\n");
    cpu->c = cpu->c;
    cpu->pc += 1;
    break;
  case 0x4a:
    printf("> MOV D, C");
    fprintf(op_fh, "MOV D C\n");
    cpu->d = cpu->c;
    cpu->pc += 1;
    break;
  case 0x4b:
    printf("> MOV E, C\n");
    fprintf(op_fh, "MOV E, C\n");
    cpu->e = cpu->c;
    cpu->pc += 1;
    break;
  case 0x4c:
    printf("> MOV H, C\n");
    fprintf(op_fh, "MOV H, C\n");
    cpu->h = cpu->c;
    cpu->pc += 1;
    break;
  case 0x4d:
    printf("> MOV L, C\n");
    fprintf(op_fh, "MOV L, C\n");
    cpu->l = cpu->c;
    cpu->pc += 1;
    break;
  case 0x57:
    printf("> MOV A D\n");
    fprintf(op_fh, "MOV A, D\n");
    cpu->a = cpu->d;
    cpu->pc += 1;
    break;
  case 0x50:
    printf("> MOV B, D\n");
    fprintf(op_fh, "MOV B, D\n");
    cpu->b = cpu->d;
    cpu->pc += 1;
    break;
  case 0x51:
    printf("> MOV C, D\n");
    fprintf(op_fh, "MOV C, D\n");
    cpu->c = cpu->d;
    cpu->pc += 1;
    break;
  case 0x52:
    printf("> MOV D, D\n");
    fprintf(op_fh, "MOV D, D\n");
    cpu->d = cpu->d;
    cpu->pc += 1;
    break;
  case 0x53:
    printf("> MOV E, D\n");
    fprintf(op_fh, "MOV E, D\n");
    cpu->e = cpu->d;
    cpu->pc += 1;
    break;
  case 0x54:
    printf("> MOV H, D\n");
    fprintf(op_fh, "MOV H, D\n");
    cpu->h = cpu->d;
    cpu->pc += 1;
    break;
  case 0x55:
    printf("> MOV L, D\n");
    fprintf(op_fh, "MOV L, D\n");
    cpu->l = cpu->d;
    cpu->pc += 1;
    break;
  case 0x5f:
    printf("> MOV A, E\n");
    fprintf(op_fh, "MOV A, E\n");
    cpu->a = cpu->e;
    cpu->pc += 1;
    break;
  case 0x58:
    printf("> MOV B, E\n");
    fprintf(op_fh, "MOV B, E\n");
    cpu->b = cpu->e;
    cpu->pc += 1;
    break;
  case 0x59:
    printf("> MOV C, E\n");
    fprintf(op_fh, "MOV C, E\n");
    cpu->c = cpu->e;
    cpu->pc += 1;
    break;
  case 0x5a:
    printf("> MOV D, E\n");
    fprintf(op_fh, "MOV D, E\n");
    cpu->d = cpu->e;
    cpu->pc += 1;
    break;
  case 0x5b:
    printf("> MOV E, E\n");
    fprintf(op_fh, "MOV E, E\n");
    cpu->e = cpu->e;
    cpu->pc += 1;
    break;
  case 0x5c:
    printf("> MOV H, E\n");
    fprintf(op_fh, "MOV H, E\n");
    cpu->h = cpu->e;
    cpu->pc += 1;
    break;
  case 0x5d:
    printf("> MOV L, E\n");
    fprintf(op_fh, "MOV L, E\n");
    cpu->l = cpu->e;
    cpu->pc += 1;
    break;
  case 0x67:
    printf("> MOV A, H\n");
    fprintf(op_fh, "MOV A, H\n");
    cpu->a = cpu->h;
    cpu->pc += 1;
    break;
  case 0x60:
    printf("> MOV B, H\n");
    fprintf(op_fh, "MOV B, H\n");
    cpu->b = cpu->h;
    cpu->pc += 1;
    break;
  case 0x61:
    printf("> MOV C, H\n");
    fprintf(op_fh, "MOV C, H\n");
    cpu->c = cpu->h;
    cpu->pc += 1;
    break;
  case 0x62:
    printf("> MOV D, H\n");
    fprintf(op_fh, "MOV D, H\n");
    cpu->d = cpu->h;
    cpu->pc += 1;
    break;
  case 0x63:
    printf("> MOV E, H\n");
    fprintf(op_fh, "MOV E, H\n");
    cpu->e = cpu->h;
    cpu->pc += 1;
    break;
  case 0x64:
    printf("> MOV H, H\n");
    fprintf(op_fh, "MOV H, H\n");
    cpu->h = cpu->h;
    cpu->pc += 1;
    break;
  case 0x65:
    printf("> MOV L, H\n");
    fprintf(op_fh, "MOV L, H\n");
    cpu->l = cpu->h;
    cpu->pc += 1;
    break;
  case 0x6f:
    printf("> MOV A, L\n");
    fprintf(op_fh, "MOV A, L\n");
    cpu->a = cpu->l;
    cpu->pc += 1;
    break;
  case 0x68:
    printf("> MOV B, L\n");
    fprintf(op_fh, "MOV B, L\n");
    cpu->b = cpu->l;
    cpu->pc += 1;
    break;
  case 0x69:
    printf("> MOV C, L\n");
    fprintf(op_fh, "MOV C, L\n");
    cpu->c = cpu->l;
    cpu->pc += 1;
    break;
  case 0x6a:
    printf("> MOV D, L\n");
    fprintf(op_fh, "MOV D, L\n");
    cpu->d = cpu->l;
    cpu->pc += 1;
    break;
  case 0x6b:
    printf("> MOV E, L\n");
    fprintf(op_fh, "MOV E, L\n");
    cpu->e = cpu->l;
    cpu->pc += 1;
    break;
  case 0x6c:
    printf("> MOV H, L\n");
    fprintf(op_fh, "MOV H, L\n");
    cpu->h = cpu->l;
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
    cpu->a =
        cpu->memory[cpu->h << 8 | cpu->l]; // TODO: test and compare with chip
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
  case 0x3e: // MVI A, data
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
    cpu->pc + 3;
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
    cpu->a += cpu->a;
    cpu->pc += 1;
    break;
  case 0x80:
    printf("> ADD B\n");
    fprintf(op_fh, "ADD B\n");
    cpu->a += cpu->b;
    cpu->pc += 1;
    break;
  case 0x81:
    printf("> ADD C\n");
    fprintf(op_fh, "ADD C\n");
    cpu->a += cpu->c;
    cpu->pc += 1;
    break;
  case 0x82:
    printf("> ADD D\n");
    fprintf(op_fh, "ADD D\n");
    cpu->a += cpu->d;
    cpu->pc += 1;
    break;
  case 0x83:
    printf("> ADD E\n");
    fprintf(op_fh, "ADD E\n");
    cpu->a += cpu->e;
    cpu->pc += 1;
    break;
  case 0x84:
    printf("> ADD H\n");
    fprintf(op_fh, "ADD H\n");
    cpu->a += cpu->h;
    cpu->pc += 1;
    break;
  case 0x85:
    printf("> ADD L\n");
    fprintf(op_fh, "ADD L\n");
    cpu->a += cpu->l;
    cpu->pc += 1;
    break;
  case 0x86:
    printf("> ADD M\n");
    fprintf(op_fh, "ADD M\n");
    cpu->a += cpu->memory[cpu->h << 8 | cpu->l];
    cpu->pc += 1;
    break;
  case 0xc6:
    printf("> ADI data\n");
    fprintf(op_fh, "ADI data\n");
    cpu->a += cpu->memory[cpu->pc + 1];
    cpu->pc += 2;
    break;
  case 0x8f:
    printf("> ADC A\n");
    fprintf(op_fh, "ADC A\n");
    cpu->a = cpu->a + cpu->a + cpu->cf;
    cpu->pc += 1;
    break;
  case 0x88:
    printf("> ADC B\n");
    fprintf(op_fh, "ADC B\n");
    cpu->a = cpu->a + cpu->b + cpu->cf;
    cpu->pc += 1;
    break;
  case 0x89:
    printf("> ADC C\n");
    fprintf(op_fh, "ADC C\n");
    cpu->a = cpu->a + cpu->c + cpu->cf;
    cpu->pc += 1;
    break;
  case 0x8a:
    printf("> ADC D\n");
    fprintf(op_fh, "ADC D\n");
    cpu->a = cpu->a + cpu->d + cpu->cf;
    cpu->pc += 1;
    break;
  case 0x8b:
    printf("> ADC E\n");
    fprintf(op_fh, "ADC E\n");
    cpu->a = cpu->a + cpu->e + cpu->cf;
    cpu->pc += 1;
    break;
  case 0x8c:
    printf("> ADC H\n");
    fprintf(op_fh, "ADC H\n");
    cpu->a = cpu->a + cpu->h + cpu->cf;
    cpu->pc += 1;
    break;
  case 0x8d:
    printf("> ADC L\n");
    fprintf(op_fh, "ADC L\n");
    cpu->a = cpu->a + cpu->l + cpu->cf;
    cpu->pc += 1;
    break;
  case 0x8e:
    printf("> ADC M\n");
    fprintf(op_fh, "ADC M\n");
    cpu->a = cpu->a + cpu->memory[cpu->h << 8 | cpu->l] + cpu->cf;
    cpu->pc += 1;
    break;
  case 0xce:
    printf("> ACI data\n");
    fprintf(op_fh, "ACI data\n");
    cpu->a = cpu->a + cpu->memory[cpu->pc + 1] + cpu->cf;
    cpu->pc += 2;
    break;
  case 0x97:
    printf("> SUB A\n");
    fprintf(op_fh, "SUB A\n");
    cpu->a -= cpu->a;
    cpu->pc += 1;
    break;
  case 0x90:
    printf("> SUB B\n");
    fprintf(op_fh, "SUB B\n");
    cpu->a -= cpu->b;
    cpu->pc += 1;
    break;
  case 0x91:
    printf("> SUB C\n");
    fprintf(op_fh, "SUB C\n");
    cpu->a -= cpu->c;
    cpu->pc += 1;
    break;
  case 0x92:
    printf("> SUB D\n");
    fprintf(op_fh, "SUB D\n");
    cpu->a -= cpu->d;
    cpu->pc += 1;
    break;
  case 0x93:
    printf("> SUB E\n");
    fprintf(op_fh, "SUB E\n");
    cpu->a -= cpu->e;
    cpu->pc += 1;
    break;
  case 0x94:
    printf("> SUB H\n");
    fprintf(op_fh, "SUB H\n");
    cpu->a -= cpu->h;
    cpu->pc += 1;
    break;
  case 0x95:
    printf("> SUB L\n");
    fprintf(op_fh, "SUB L\n");
    cpu->a -= cpu->l;
    cpu->pc += 1;
    break;
  case 0x96:
    printf("> SUB M\n");
    fprintf(op_fh, "SUB M\n");
    cpu->a -= cpu->memory[cpu->h << 8 | cpu->l];
    cpu->pc += 1;
    break;
  case 0xd6:
    printf("> SUI data\n");
    fprintf(op_fh, "SUI data\n");
    cpu->a -= cpu->memory[cpu->pc + 1];
    cpu->pc += 2;
    break;
  case 0x9f:
    printf("> SBB A\n");
    fprintf(op_fh, "SBB A\n");
    cpu->a = cpu->a - cpu->a - cpu->cf;
    cpu->pc += 1;
    break;
  case 0x98:
    printf("> SBB B\n");
    fprintf(op_fh, "SBB B\n");
    cpu->a = cpu->a - cpu->b - cpu->cf;
    cpu->pc += 1;
    break;
  case 0x99:
    printf("> SBB C\n");
    fprintf(op_fh, "SBB C\n");
    cpu->a = cpu->a - cpu->c - cpu->cf;
    cpu->pc += 1;
    break;
  case 0x9a:
    printf("> SBB D\n");
    fprintf(op_fh, "SBB D\n");
    cpu->a = cpu->a - cpu->d - cpu->cf;
    cpu->pc += 1;
    break;
  case 0x9b:
    printf("> SBB E\n");
    fprintf(op_fh, "SBB E\n");
    cpu->a = cpu->a - cpu->e - cpu->cf;
    cpu->pc += 1;
    break;
  case 0x9c:
    printf("> SBB H\n");
    fprintf(op_fh, "SBB H\n");
    cpu->a = cpu->a - cpu->h - cpu->cf;
    cpu->pc += 1;
    break;
  case 0x9d:
    printf("> SBB L\n");
    fprintf(op_fh, "SBB L\n");
    cpu->a = cpu->a - cpu->l - cpu->cf;
    cpu->pc += 1;
    break;
  case 0x9e:
    printf("> SBB M\n");
    fprintf(op_fh, "SBB M\n");
    cpu->a = cpu->a - cpu->memory[cpu->h << 8 | cpu->l] - cpu->cf;
    cpu->pc += 1;
    break;
  case 0xde:
    printf("> SBI data\n");
    fprintf(op_fh, "SBI data\n");
    cpu->a = cpu->a - cpu->memory[cpu->pc + 1] - cpu->cf;
    cpu->pc += 2;
    break;
  case 0x3c:
    printf("> INR A\n");
    fprintf(op_fh, "INR A\n");
    cpu->a += 1;
    cpu->pc += 1;
    break;
  case 0x4:
    printf("> INR B\n");
    fprintf(op_fh, "INR B\n");
    cpu->b += 1;
    cpu->pc += 1;
    break;
  case 0xc:
    printf("> INR C\n");
    fprintf(op_fh, "INR C\n");
    cpu->c += 1;
    cpu->pc += 1;
    break;
  case 0x14:
    printf("> INR D\n");
    fprintf(op_fh, "INR D\n");
    cpu->d += 1;
    cpu->pc += 1;
    break;
  case 0x1c:
    printf("> INR E\n");
    fprintf(op_fh, "INR E\n");
    cpu->e += 1;
    cpu->pc += 1;
    break;
  case 0x24:
    printf("> INR H\n");
    fprintf(op_fh, "INR H\n");
    cpu->h += 1;
    cpu->pc += 1;
    break;
  case 0x2c:
    printf("> INR L\n");
    fprintf(op_fh, "INR L\n");
    cpu->l += 1;
    cpu->pc += 1;
    break;
  case 0x34:
    printf("> INR M\n");
    fprintf(op_fh, "INR M\n");
    cpu->memory[cpu->h << 8 | cpu->l] += 1;
    cpu->pc += 1;
    break;
  case 0x3d:
    printf("> DCR A\n");
    fprintf(op_fh, "DCR A\n");
    cpu->a -= 1;
    cpu->pc += 1;
    break;
  case 0x5:
    printf("> DCR B\n");
    fprintf(op_fh, "DCR B\n");
    cpu->b -= 1;
    cpu->pc += 1;
    break;
  case 0xd:
    printf("> DCR C\n");
    fprintf(op_fh, "DCR C\n");
    cpu->c -= 1;
    cpu->pc += 1;
    break;
  case 0x15:
    printf("> DCR D\n");
    fprintf(op_fh, "DCR D\n");
    cpu->d -= 1;
    cpu->pc += 1;
    break;
  case 0x1d:
    printf("> DCR E\n");
    fprintf(op_fh, "DCR E\n");
    cpu->e -= 1;
    cpu->pc += 1;
    break;
  case 0x25:
    printf("> DCR H\n");
    fprintf(op_fh, "DCR H\n");
    cpu->h -= 1;
    cpu->pc += 1;
    break;
  case 0x2d:
    printf("> DCR L\n");
    fprintf(op_fh, "DCR L\n");
    cpu->l -= 1;
    cpu->pc += 1;
    break;
  case 0x35:
    printf("> DCR M\n");
    fprintf(op_fh, "DCR M\n");
    cpu->memory[cpu->h << 8 | cpu->l] -= 1;
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
    uint16_t sum = bc_3 + hl_3;
    cpu->h = sum >> 8;
    cpu->l = sum;
    cpu->cf = sum > 0xffff ? 1 : 0;
    cpu->pc += 1;
    break;
  case 0x19:
    printf("> DAD DE\n");
    fprintf(op_fh, "DAD DE\n");
    uint16_t de_3 = cpu->d << 8 | cpu->e;
    uint16_t hl_4 = cpu->h << 8 | cpu->l;
    uint16_t sum_2 = de_3 + hl_4;
    cpu->h = sum_2 >> 8;
    cpu->l = sum_2;
    cpu->cf = sum_2 > 0xffff ? 1 : 0;
    cpu->pc += 1;
    break;
  case 0x29:
    printf("> DAD HL\n");
    fprintf(op_fh, "DAD HL\n");
    uint16_t hl_5 = cpu->h << 8 | cpu->l;
    uint16_t sum_3 = hl_5 + hl_5;
    cpu->h = sum_3 >> 8;
    cpu->l = sum_3;
    cpu->cf = sum_3 > 0xffff ? 1 : 0;
    cpu->pc += 1;
    break;
  case 0x39:
    printf("> DAD SP\n");
    fprintf(op_fh, "DAD SP\n");
    uint16_t hl_6 = cpu->h << 8 | cpu->l;
    uint16_t sum_4 = hl_6 + cpu->sp;
    cpu->h = sum_4 >> 8;
    cpu->l = sum_4;
    cpu->cf = sum_4 > 0xffff ? 1 : 0;
    cpu->pc += 1;
    break;
  case 0x27:
    printf("> DAA \n");
    fprintf(op_fh, "DAA\n");
    uint8_t daa_higher_4_bits = cpu->a >> 4;
    uint8_t daa_lower_4_bits = cpu->a & 0xf;
    if (daa_lower_4_bits > 9 || cpu->acf == 1) {
      cpu->a += 6;
    }
    if (daa_higher_4_bits > 9 || cpu->cf == 1) {
      daa_higher_4_bits += 6;
      cpu->a = daa_higher_4_bits << 4 | cpu->a & 0xf;
    }

    // set cf,acf and zsp flags
    // to find if cf needs to set to 1 for additions
    // store actual addition result in int16_t variable
    // and check if its > 0xff
    // int16_t true_result =

    break;

  case 0xa7:
    printf("> ANA A\n");
    fprintf(op_fh, "ANA A\n");
    cpu->a = cpu->a & cpu->a;
    cpu->pc += 1;
    break;
  case 0xa0:
    printf("> ANA B\n");
    fprintf(op_fh, "ANA B\n");
    cpu->a = cpu->a & cpu->b;
    cpu->pc += 1;
    break;
  case 0xa1:
    printf("> ANA C\n");
    fprintf(op_fh, "ANA C\n");
    cpu->a = cpu->a & cpu->c;
    cpu->pc += 1;
    break;
  case 0xa2:
    printf("> ANA D\n");
    fprintf(op_fh, "ANA D\n");
    cpu->a = cpu->a & cpu->d;
    cpu->pc += 1;
    break;
  case 0xa3:
    printf("> ANA E\n");
    fprintf(op_fh, "ANA E\n");
    cpu->a = cpu->a & cpu->e;
    cpu->pc += 1;
    break;
  case 0xa4:
    printf("> ANA H\n");
    fprintf(op_fh, "ANA H\n");
    cpu->a = cpu->a & cpu->h;
    cpu->pc += 1;
    break;
  case 0xa5:
    printf("> ANA L\n");
    fprintf(op_fh, "ANA L\n");
    cpu->a = cpu->a & cpu->l;
    cpu->pc += 1;
    break;
  case 0xe6:
    printf("> ANI data\n");
    fprintf(op_fh, "ANI data\n");
    cpu->a = cpu->a & cpu->memory[cpu->pc + 1];
    cpu->pc += 2;
    break;
  case 0xaf:
    printf("> XRA A\n");
    fprintf(op_fh, "XRA A\n");
    cpu->a = cpu->a ^ cpu->a;
    cpu->pc += 1;
    cpu->cf = 0;
    cpu->acf = 0;
    break;
  case 0xa8:
    printf("> XRA B\n");
    fprintf(op_fh, "XRA B\n");
    cpu->a = cpu->a ^ cpu->b;
    cpu->pc += 1;
    cpu->cf = 0;
    cpu->acf = 0;
    break;
  case 0xa9:
    printf("> XRA C\n");
    fprintf(op_fh, "XRA C\n");
    cpu->a = cpu->a ^ cpu->c;
    cpu->pc += 1;
    cpu->cf = 0;
    cpu->acf = 0;
    break;
  case 0xaa:
    printf("> XRA D\n");
    fprintf(op_fh, "XRA D\n");
    cpu->a = cpu->a ^ cpu->d;
    cpu->pc += 1;
    cpu->cf = 0;
    cpu->acf = 0;
    break;
  case 0xab:
    printf("> XRA E\n");
    fprintf(op_fh, "XRA E\n");
    cpu->a = cpu->a ^ cpu->e;
    cpu->pc += 1;
    cpu->cf = 0;
    cpu->acf = 0;
    break;
  case 0xac:
    printf("> XRA H\n");
    fprintf(op_fh, "XRA H\n");
    cpu->a = cpu->a ^ cpu->h;
    cpu->pc += 1;
    cpu->cf = 0;
    cpu->acf = 0;
    break;
  case 0xad:
    printf("> XRA L\n");
    fprintf(op_fh, "XRA L\n");
    cpu->a = cpu->a ^ cpu->l;
    cpu->pc += 1;
    cpu->cf = 0;
    cpu->acf = 0;
    break;
  case 0xae:
    printf("> XRA M\n");
    fprintf(op_fh, "XRA M\n");
    cpu->a = cpu->a ^ cpu->memory[cpu->h << 8 | cpu->l];
    cpu->pc += 1;
    cpu->cf = 0;
    cpu->acf = 0;
    break;
  case 0xee:
    printf("> XRI data\n");
    fprintf(op_fh, "XRI data\n");
    cpu->a = cpu->a ^ cpu->memory[cpu->pc + 1];
    cpu->pc += 2;
    cpu->cf = 0;
    cpu->acf = 0;
    break;
  case 0xb7:
    printf("> ORA A\n");
    fprintf(op_fh, "ORA A\n");
    cpu->a = cpu->a | cpu->a;
    cpu->pc += 1;
    cpu->cf = 0;
    cpu->acf = 0;
    break;
  case 0xb0:
    printf("> ORA B\n");
    fprintf(op_fh, "ORA B\n");
    cpu->a = cpu->a | cpu->b;
    cpu->pc += 1;
    cpu->cf = 0;
    cpu->acf = 0;
    break;
  case 0xb1:
    printf("> ORA C\n");
    fprintf(op_fh, "ORA C\n");
    cpu->a = cpu->a | cpu->c;
    cpu->pc += 1;
    cpu->cf = 0;
    cpu->acf = 0;
    break;
  case 0xb2:
    printf("> ORA D\n");
    fprintf(op_fh, "ORA D\n");
    cpu->a = cpu->a | cpu->d;
    cpu->pc += 1;
    cpu->cf = 0;
    cpu->acf = 0;
    break;
  case 0xb3:
    printf("> ORA E\n");
    fprintf(op_fh, "ORA E\n");
    cpu->a = cpu->a | cpu->e;
    cpu->pc += 1;
    cpu->cf = 0;
    cpu->acf = 0;
    break;
  case 0xb4:
    printf("> ORA H\n");
    fprintf(op_fh, "ORA H\n");
    cpu->a = cpu->a | cpu->h;
    cpu->pc += 1;
    cpu->cf = 0;
    cpu->acf = 0;
    break;
  case 0xb5:
    printf("> ORA L\n");
    fprintf(op_fh, "ORA L\n");
    cpu->a = cpu->a | cpu->l;
    cpu->pc += 1;
    cpu->cf = 0;
    cpu->acf = 0;
    break;
  case 0xf6:
    printf("> ORI data\n");
    fprintf(op_fh, "ORI data\n");
    cpu->a = cpu->a | cpu->memory[cpu->pc + 1];
    cpu->pc += 2;
    cpu->cf = 0;
    cpu->acf = 0;
    break;
  case 0xbf:
    printf("> CMP A\n");
    fprintf(op_fh, "CMP A\n");
    uint16_t cmp_result = cpu->a - cpu->a;
    set_carry_flag(cpu, cmp_result);
    set_zero_flag(cpu, cmp_result);
    cpu->pc += 1;
    break;
  case 0xb8:
    printf("> CMP B\n");
    fprintf(op_fh, "CMP B\n");
    uint16_t cmp_result_2 = cpu->a - cpu->b;
    set_carry_flag(cpu, cmp_result_2);
    set_zero_flag(cpu, cmp_result_2);
    cpu->pc += 1;
    break;
  case 0xb9:
    printf("> CMP C\n");
    fprintf(op_fh, "CMP C\n");
    uint16_t cmp_result_3 = cpu->a - cpu->c;
    set_carry_flag(cpu, cmp_result_3);
    set_zero_flag(cpu, cmp_result_3);
    cpu->pc += 1;
    break;
  case 0xba:
    printf("> CMP D\n");
    fprintf(op_fh, "CMP D\n");
    uint16_t cmp_result_4 = cpu->a - cpu->d;
    set_carry_flag(cpu, cmp_result_4);
    set_zero_flag(cpu, cmp_result_4);
    cpu->pc += 1;
    break;
  case 0xbb:
    printf("> CMP E\n");
    fprintf(op_fh, "CMP E\n");
    uint16_t cmp_result_5 = cpu->a - cpu->e;
    set_carry_flag(cpu, cmp_result_5);
    set_zero_flag(cpu, cmp_result_5);
    cpu->pc += 1;
    break;
  case 0xbc:
    printf("> CMP H\n");
    fprintf(op_fh, "CMP H\n");
    uint16_t cmp_result_6 = cpu->a - cpu->h;
    set_carry_flag(cpu, cmp_result_6);
    set_zero_flag(cpu, cmp_result_6);
    cpu->pc += 1;
    break;
  case 0xbd:
    printf("> CMP L\n");
    fprintf(op_fh, "CMP L\n");
    uint16_t cmp_result_7 = cpu->a - cpu->l;
    set_carry_flag(cpu, cmp_result_7);
    set_zero_flag(cpu, cmp_result_7);
    cpu->pc += 1;
    break;
  case 0xbe:
    printf("> CMP M\n");
    fprintf(op_fh, "CMP M\n");
    uint16_t cmp_result_8 = cpu->a - cpu->memory[cpu->h << 8 | cpu->l];
    set_carry_flag(cpu, cmp_result_8);
    set_zero_flag(cpu, cmp_result_8);
    cpu->pc += 1;
    break; 
  case 0xfe:
    printf("> CPI data\n");
    fprintf(op_fh, "CPI data\n");
    uint16_t cmp_result_9 = cpu->a - cpu->memory[cpu->pc + 1];
    set_carry_flag(cpu, cmp_result_9);
    set_zero_flag(cpu, cmp_result_9);
    cpu->pc += 2;
    break;
  case 0x7:
    printf("> RLC\n");
    fprintf(op_fh, "RLC\n");
    cpu -> cf = (cpu -> a) >> 7 & 0b00000001;
    cpu -> a = (cpu -> a) << 1 | cpu -> cf;
    cpu -> pc +=1 ;
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
    cpu->a = -cpu->a;
    cpu->pc+=1;
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
    cpu->pc = cpu->memory[cpu->pc + 1] << 8 | cpu->memory[cpu->pc+1];
    break;
  case 0xc2:
    if(cpu->zf == 0){
      cpu->pc = cpu->memory[cpu->pc+2] << 8 | cpu->memory[cpu->pc+1];
    }
    else {
      cpu->pc += 3;
    }
    break;
  case 0xca:
    if(cpu->zf == 1){
      cpu->pc = cpu->memory[cpu->pc+2] << 8 | cpu->memory[cpu->pc+1];
    }
    else {
      cpu->pc += 3;
    }
    break;
  case 0xd2:
    if(cpu->cf == 0){
      cpu->pc = cpu->memory[cpu->pc+2] << 8 | cpu->memory[cpu->pc+1];
    }
    else {
      cpu->pc += 3;
    }
    break;
  case 0xda:
    if(cpu->cf == 1){
      cpu->pc = cpu->memory[cpu->pc+2] << 8 | cpu->memory[cpu->pc+1];
    }
    else {
      cpu->pc += 3;
    }
    break;
  case 0xe2:
    if(cpu->pf == 0){
      cpu->pc = cpu->memory[cpu->pc+2] << 8 | cpu->memory[cpu->pc+1];
    }
    else {
      cpu->pc += 3;
    }
    break;
  case 0xea:
    if(cpu->pf == 1){
      cpu->pc = cpu->memory[cpu->pc+2] << 8 | cpu->memory[cpu->pc+1];
    }
    else {
      cpu->pc += 3;
    }
    break;
  case 0xf2:
    if(cpu->sf == 0){
      cpu->pc = cpu->memory[cpu->pc+2] << 8 | cpu->memory[cpu->pc+1];
    }
    else {
      cpu->pc += 3;
    }
    break;
  case 0xfa:
    if(cpu->sf == 1){
      cpu->pc = cpu->memory[cpu->pc+2] << 8 | cpu->memory[cpu->pc+1];
    }
    else {
      cpu->pc += 3;
    }
    break;
  case 0xcd:
    cpu->memory[cpu->sp - 1] = cpu->pc >> 8;
    cpu->memory[cpu->sp - 2] = cpu->pc & 0xff;
    cpu->sp -= 2;
    cpu->pc = cpu->memory[cpu->pc+2] << 8 | cpu->memory[cpu->pc+1];
    break;
  case 0xc4:
    if(cpu->zf == 0){
      cpu->memory[cpu->sp - 1] = cpu->pc >> 8;
      cpu->memory[cpu->sp - 2] = cpu->pc & 0xff;
      cpu->sp -= 2;
      cpu->pc = cpu->memory[cpu->pc+2] << 8 | cpu->memory[cpu->pc+1];
    }
    else {
      cpu->pc += 3;
    }
    break;
  case 0xcc:
    if(cpu->zf == 1){
      cpu->memory[cpu->sp - 1] = cpu->pc >> 8;
      cpu->memory[cpu->sp - 2] = cpu->pc & 0xff;
      cpu->sp -= 2;
      cpu->pc = cpu->memory[cpu->pc+2] << 8 | cpu->memory[cpu->pc+1];
    }
    else {
      cpu->pc += 3;
    }
    break;
  case 0xd4:
    if(cpu->cf == 0){
      cpu->memory[cpu->sp - 1] = cpu->pc >> 8;
      cpu->memory[cpu->sp - 2] = cpu->pc & 0xff;
      cpu->sp -= 2;
      cpu->pc = cpu->memory[cpu->pc+2] << 8 | cpu->memory[cpu->pc+1];
    }
    else {
      cpu->pc += 3;
    }
    break;
  case 0xdc:
    if(cpu->cf == 1){
      cpu->memory[cpu->sp - 1] = cpu->pc >> 8;
      cpu->memory[cpu->sp - 2] = cpu->pc & 0xff;
      cpu->sp -= 2;
      cpu->pc = cpu->memory[cpu->pc+2] << 8 | cpu->memory[cpu->pc+1];
    }
    else {
      cpu->pc += 3;
    }
    break;
  case 0xe4:
    if(cpu->pf == 0){
      cpu->memory[cpu->sp - 1] = cpu->pc >> 8;
      cpu->memory[cpu->sp - 2] = cpu->pc & 0xff;
      cpu->sp -= 2;
      cpu->pc = cpu->memory[cpu->pc+2] << 8 | cpu->memory[cpu->pc+1];
    }
    else {
      cpu->pc += 3;
    }
    break;
  case 0xec:
    if(cpu->pf == 1){
      cpu->memory[cpu->sp - 1] = cpu->pc >> 8;
      cpu->memory[cpu->sp - 2] = cpu->pc & 0xff;
      cpu->sp -= 2;
      cpu->pc = cpu->memory[cpu->pc+2] << 8 | cpu->memory[cpu->pc+1];
    }
    else {
      cpu->pc += 3;
    }
    break;
  case 0xf4:
    if(cpu->sf == 0){
      cpu->memory[cpu->sp - 1] = cpu->pc >> 8;
      cpu->memory[cpu->sp - 2] = cpu->pc & 0xff;
      cpu->sp -= 2;
      cpu->pc = cpu->memory[cpu->pc+2] << 8 | cpu->memory[cpu->pc+1];
    }
    else {
      cpu->pc += 3;
    }
    break;
  case 0xfc:
    if(cpu->sf == 1){
      cpu->memory[cpu->sp - 1] = cpu->pc >> 8;
      cpu->memory[cpu->sp - 2] = cpu->pc & 0xff;
      cpu->sp -= 2;
      cpu->pc = cpu->memory[cpu->pc+2] << 8 | cpu->memory[cpu->pc+1];
    }
    else {
      cpu->pc += 3;
    }
    break;
  case 0xc9:
    cpu->pc = cpu->memory[cpu->sp] | cpu->memory[cpu->sp+1] << 8;
    cpu->sp += 2;
    break;
  case 0xc0:
    if(cpu->zf == 0) {
      cpu->pc = cpu->memory[cpu->sp] | cpu->memory[cpu->sp+1] << 8;
      cpu->sp += 2;
    }
    else {
      cpu->pc += 1;
    }
    break;
  case 0xc8:
    if(cpu->zf == 1) {
      cpu->pc = cpu->memory[cpu->sp] | cpu->memory[cpu->sp+1] << 8;
      cpu->sp += 2;
    }
    else {
      cpu->pc += 1;
    }
    break;
  case 0xd0:
    if(cpu->cf == 0) {
      cpu->pc = cpu->memory[cpu->sp] | cpu->memory[cpu->sp+1] << 8;
      cpu->sp += 2;
    }
    else {
      cpu->pc += 1;
    }
    break;
  case 0xd8:
    if(cpu->cf == 1) {
      cpu->pc = cpu->memory[cpu->sp] | cpu->memory[cpu->sp+1] << 8;
      cpu->sp += 2;
    }
    else {
      cpu->pc += 1;
    }
    break;
  case 0xe0:
    if(cpu->pf == 0) {
      cpu->pc = cpu->memory[cpu->sp] | cpu->memory[cpu->sp+1] << 8;
      cpu->sp += 2;
    }
    else {
      cpu->pc += 1;
    }
    break;
  case 0xe8:
    if(cpu->pf == 1) {
      cpu->pc = cpu->memory[cpu->sp] | cpu->memory[cpu->sp+1] << 8;
      cpu->sp += 2;
    }
    else {
      cpu->pc += 1;
    }
    break;
  case 0xf0:
    if(cpu->sf == 0) {
      cpu->pc = cpu->memory[cpu->sp] | cpu->memory[cpu->sp+1] << 8;
      cpu->sp += 2;
    }
    else {
      cpu->pc += 1;
    }
    break;
  case 0xf8:
    if(cpu->sf == 1) {
      cpu->pc = cpu->memory[cpu->sp] | cpu->memory[cpu->sp+1] << 8;
      cpu->sp += 2;
    }
    else {
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
    cpu->memory[cpu->sp-1] = cpu->b;
    cpu->memory[cpu->sp-2] = cpu->c;
    cpu->sp -= 2;
    cpu->pc += 1;
    break;
  case 0xd5:
    printf("> PUSH DE\n");
    fprintf(op_fh, "PUSH DE\n");
    cpu->memory[cpu->sp-1] = cpu->d;
    cpu->memory[cpu->sp-2] = cpu->e;
    cpu->sp -= 2;
    cpu->pc += 1;
    break;
  case 0xe5:
    printf("> PUSH HL\n");
    fprintf(op_fh, "PUSH HL\n");
    cpu->memory[cpu->sp-1] = cpu->h;
    cpu->memory[cpu->sp-2] = cpu->l;
    cpu->sp -= 2;
    cpu->pc += 1;
    break;
  
  case 0xf5:
    printf("> PUSH PSW\n");
    fprintf(op_fh, "PUSH PSW\n");
    cpu->memory[cpu->sp-1] = cpu->a;
    uint8_t psw = 0;
    psw = cpu->sf << 7 | cpu->zf << 6 | cpu->acf << 4 | cpu->pf << 2 | cpu-> cf;
    cpu->memory[cpu->sp-2] = psw;
    cpu->sp -= 2;
    cpu->pc += 1;
    break;
  
  case 0xc1:
    printf("> POP BC\n");
    fprintf(op_fh, "POP BC\n");
    cpu->c = cpu->memory[cpu->sp];
    cpu->b = cpu->memory[cpu->sp+1];
    cpu->sp += 2;
    cpu->pc += 1;
    break;
  case 0xd1:
    printf("> POP DE\n");
    fprintf(op_fh, "POP DE\n");
    cpu->e = cpu->memory[cpu->sp];
    cpu->d = cpu->memory[cpu->sp+1];
    cpu->sp += 2;
    cpu->pc += 1;
    break;
  case 0xe1:
    printf("> POP HL\n");
    fprintf(op_fh, "POP HL\n");
    cpu->l = cpu->memory[cpu->sp];
    cpu->h = cpu->memory[cpu->sp+1];
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
    cpu->a = cpu->memory[cpu->sp+1];
    cpu->sp += 2;
    cpu->pc += 1;
    break;
  
  case 0xe3:
    printf("> XTHL\n");
    fprintf(op_fh, "XTHL\n");
    uint8_t tmp = cpu->memory[cpu->sp];
    cpu->memory[cpu->sp] = cpu->l;
    cpu->l = tmp;
    tmp = cpu->memory[cpu->sp+1];
    cpu->memory[cpu->sp+1] = cpu->h;
    cpu->h = tmp;
    cpu->pc += 1;
    break;
  
  case 0xf9:
    printf("> SPHL\n");
    fprintf(op_fh, "SPHL\n");
    cpu->sp = cpu->h << 8 | cpu->l;
    cpu->pc += 1;
    break;

  case 0xdb: // NOTE: not implemented
    printf("> IN data\n");
    fprintf(op_fh, "IN data\n");
    break;

  case 0xd3: // NOTE: not implemented
    printf("> OUT data\n");
    fprintf(op_fh, "OUT data\n");
    break;
  
  case 0xfb:
    printf("> EI\n");
    fprintf(op_fh, "EI\n");
    cpu-> interrupt_enable = 1;
    cpu -> interrupt_delay = 1;
    cpu->pc += 1;
    break;
  
  case 0xf3:
    printf("> DI\n");
    fprintf(op_fh, "DI\n");
    cpu-> interrupt_enable = 0;
    cpu -> interrupt_delay = 0;
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
    break;

  default: printf("instruction not found: %x\n", instruction); break;
  }
}



void debug(cpu* cpu) {
  printf("---------------------------\n");
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
  printf("---------------------------\n");
}

int main(int argc, char** argv) {
  printf(
      "starting program %d %d %d \n ", sizeof(cpu), sizeof(int), sizeof(bool));
  cpu* cpu = malloc(50);

  cpu_init(cpu);
  int total_steps = 5;

  FILE* op_fh = fopen(OUPUT_ASSEMBLY_NAME, "w");

  for (int i = 0; i < total_steps; i++) {
    step(cpu, op_fh);
    debug(cpu);
  }

  free(cpu);
  printf("\n program finished\n");
  return 0;
}
