#include <stdio.h>
#include "i8080.h"
#include "8080_emulator_new.h"

// #include "8080_emulator_new.h"

uint8_t* memory = NULL;

uint8_t rb(void* userdata, int addr) {
    printf("reading byte at address %x\n", addr);
    return memory[addr];
}

void main(){
    memory = malloc(0x10000);
    memory[0xa14] = 30;

    i8080* chip = malloc(sizeof(i8080));
    cpu* cpu;

    i8080_init(chip);
    cpu_init(cpu);

    chip->userdata = chip;
    chip->read_byte = rb;

    FILE *f = fopen("test_programs/mov_A_M.bin", "rb");
    fread(memory, 1, 5, f);

    printf("%x\n", memory[0]);

    fclose(f);

    for(int i = 0; i < 5; i++){
    i8080_step(chip);

    }

    
}


