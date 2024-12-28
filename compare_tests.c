#include <stdio.h>
#include "i8080.h"
#include "8080_emulator_new.h"

// NOTE: to run -> clang i8080.c 8080_emulator_new.c compare_tests.c && ./a.out

// #define ROM_NAME "test_programs/mov_A_M.bin"
// uint16_t memory_offset_to_load_rom = 0x0;

#define ROM_NAME "cpu_tests/TST8080.COM"
uint16_t memory_offset_to_load_rom = 0x100;

uint8_t* memory = NULL;

static bool test_finished = 0;

uint8_t chip_read_byte(void* userdata, int addr) {
  return memory[addr];
}

void chip_write_byte(void* userdata, uint16_t address, uint8_t data){
  memory[address] = data;
}

static void chip_port_out(void* userdata, uint8_t port, uint8_t value) {
  i8080* const c = (i8080*) userdata;

  if (port == 0) {
    test_finished = 1;
    exit(1);
  } else if (port == 1) {
    uint8_t operation = c->c;

    if (operation == 2) { // print a character stored in E
      printf("%c", c->e);
    } else if (operation == 9) { // print from memory at (DE) until '$' char
      uint16_t addr = (c->d << 8) | c->e;
      do {
        printf("%c", chip_read_byte(c, addr++));
      } while (chip_read_byte(c, addr) != '$');
    }
  }
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

  for(int i=0; i<MEMORY_SIZE; i++) {
    if (memory[i] != cpu->memory[i]) {
      printf("memory is not equal %d %x %x \n", i, memory[i], cpu->memory[i]);
      return 0;
    }
  }

  return 1;
}

int main() {
  memory = malloc(0x10000);

  i8080* chip = malloc(sizeof(i8080));
  cpu* cpu = malloc(50);

  // init chip 
  i8080_init(chip);  
  chip->userdata = chip;
  chip->read_byte = chip_read_byte;
  chip->write_byte = chip_write_byte;
  chip->port_out = chip_port_out;

  FILE* f = fopen(ROM_NAME, "rb");
  fseek(f, 0, SEEK_END);
  size_t file_size = ftell(f);
  rewind(f);
  fread(&memory[memory_offset_to_load_rom], sizeof(uint8_t), file_size, f);
  fclose(f);
  // memory[0xa14] = 20; // TODO: remove this
  printf("chip initialized\n");
  printf("100 %d", memory[0x100]);

  // init cpu
  cpu_init(cpu, ROM_NAME, memory_offset_to_load_rom);
  printf("cpu initialized\n");

  FILE* op_fh = fopen(OUPUT_ASSEMBLY_NAME, "w");
  
  // custom operations for tests
  chip->pc = 0x100;
  cpu->pc = 0x100;
  memory[0x5] = 0xD3;
  memory[0x6] = 0x1;
  memory[0x7] = 0xC9;
  cpu->memory[0x5] = 0xD3;
  cpu->memory[0x6] = 0x1;
  cpu->memory[0x7] = 0xC9;
  
  for (int i = 0; i < 652; i++) {
    if(test_finished){
      printf("\n-------------------test finished with port out 0-----------------------\n");
      return 0;
    }

    printf("\n---- step %d/%d ", i, 652);
    i8080_step(chip);
    step(cpu, op_fh);

    i8080_debug_output(chip, true);
    debug_cpu(cpu);

    printf("\n-------------------step finished-----------------------\n");

    if (!compare_registers(chip, cpu)) {
      printf("Registers are not equal\n");
      return 1;
    }
  }

  fclose(op_fh);
  printf("\nTest finished after limited steps\n");
  return 0;
}
