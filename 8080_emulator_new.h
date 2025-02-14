#include <stdint.h>
#include <stdbool.h>

// int MEMORY_SIZE = 20;
// #define MEMORY_SIZE 20
#define MEMORY_SIZE 0x10000

// char ROM_NAME[] = "test_programs/mov_a_10_and_inr_a.a80.bin";
// char ROM_NAME[] = "test_programs/first.a80.bin";
// char ROM_NAME[] = "test_programs/mov_A_M.bin";
// char ROM_NAME[] = "test_programs/daa.asm80.a80.bin";
// extern ROM_NAME[];
#define ROM_NAME "test_programs/daa.asm80.a80.bin"

// char OUPUT_ASSEMBLY_NAME[] = "test_programs/output.asm";
#define OUPUT_ASSEMBLY_NAME "test_programs/output.asm"

// define processor object

typedef struct {
  // 8 bit registers
  uint8_t a, b, c, d, e, h, l;

  // 5 bit flag registes
  bool zf; // zero flag
  bool cf; // carry flag
  bool sf; // sign flag
  bool pf; // parity flag
  bool acf; // auxiliary carry flag

  // program counter
  uint16_t pc;

  // stack pointer
  uint16_t sp;

  // memory
  // uint8_t memory[MEMORY_SIZE];
  // dynamic memory
  uint8_t* memory;

  // interrupt enable or disable
  bool interrupt_enable;
  uint8_t interrupt_delay; // when to enable/disable the ineterrupt after
                           // executing EI / DI
  uint8_t interrupt_opcode;

  // processor halted
  bool halted;

  unsigned long state_count;

  uint8_t (*port_in)(uint8_t);
  void (*port_out)(uint8_t, uint8_t);

  // TODO: add data and address bus bits if interfacing with other devices is
  // also emulated

} cpu;

void cpu_init(cpu* cpu, char* rom_name, uint16_t memory_offset_to_load_rom);
void debug_cpu(cpu* cpu);

// for direct access
void load_rom(cpu* cpu, char* rom_name, uint16_t memory_offset);
void init_registers(cpu* cpu);
void init_flags(cpu* cpu);
void init_memory(cpu* cpu);
void interrupt_cpu(cpu* cpu, uint8_t opcode);
