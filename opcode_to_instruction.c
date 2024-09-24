#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/*
Program to convert instructions from 8080 data sheet pdf to 8080 instructions
reference I will provide instuction, instruction size, description, its binary
opcode, size, number of machine cycles, flags affected, should output
    hex_opcode, instruction_size, flags_affected, description

*/

// int A = 0b111;
// int B = 0b000;
// int C = 0b001;
// int D = 0b010;
// int E = 0b011;
// int H = 0b100;
// int L = 0b101;

// int BC = 0b00;
// int DE = 0b01;
// int HL = 0b10;
// int SP = 0b11;

typedef struct {
  char* name;
  char* value;
} register_;

struct instruction {
  char* name;
  char* description;
  char* opcode;
  register_ registers_allowed[8];
  // char* register_pairs_allowed_string;
  int number_of_registers_allowed;
  int size;
  int cycles;
  int states;
  char* flags;
};

void fill_register_array(register_ registers[]) {
  register_ A = {.name = "A", .value = "111"};

  register_ B = {.name = "B", .value = "000"};

  register_ C = {.name = "C", .value = "001"};

  register_ D = {.name = "D", .value = "010"};

  register_ E = {.name = "E", .value = "011"};

  register_ H = {.name = "H", .value = "100"};

  register_ L = {.name = "L", .value = "101"};

  registers[0] = A;
  registers[1] = B;
  registers[2] = C;
  registers[3] = D;
  registers[4] = E;
  registers[5] = H;
  registers[6] = L;
}

void fill_register_pair(register_ register_pairs[]) {
  register_ BC = {.name = "BC", .value = "00"};

  register_ DE = {.name = "DE", .value = "01"};

  register_ HL = {.name = "HL", .value = "10"};

  register_ SP = {.name = "SP", .value = "11"};

  register_pairs[0] = BC;
  register_pairs[1] = DE;
  register_pairs[2] = HL;
  register_pairs[3] = SP;
}

void fill_conditions(register_ conditions[]) {
  register_ NZ = {.name = "NZ", .value = "000"};

  register_ Z = {.name = "Z", .value = "001"};

  register_ NC = {.name = "NC", .value = "010"};

  register_ C = {.name = "C", .value = "011"};

  register_ PO = {.name = "PO", .value = "100"};

  register_ PE = {.name = "PE", .value = "101"};

  register_ P = {.name = "P", .value = "110"};

  register_ M = {.name = "M", .value = "111"};

  conditions[0] = NZ;
  conditions[1] = Z;
  conditions[2] = NC;
  conditions[3] = C;
  conditions[4] = PO;
  conditions[5] = PE;
  conditions[6] = P;
  conditions[7] = M;
}

int TOTAL_INSTRUCTIONS_TO_GENERATE = 72;
void fill_instructions(struct instruction instructions[],
    register_ register_pairs[], register_ conditions[]) {

  // struct instruction  = {.name = "",
  //     .description = "",

  //     .opcode = "",
  //     .size = 1,
  //     .cycles = ,
  //     .states = ,
  //     .flags = "Z,S,P,AC"};

  struct instruction mov_r1_r2 = {.name = "MOV r1 r2",
      .description = "Move data from r2 to r1",
      .opcode = "01DDDSSS",
      .size = 1,
      .cycles = 1,
      .states = 5,
      .flags = "none"};

  struct instruction mov_r_m = {.name = "MOV r M",
      .description = "Move data from memory location whose address is in H,L "
                     "to register r",
      .opcode = "01DDD110",
      .size = 1,
      .cycles = 2,
      .states = 7,
      .flags = "none"};

  struct instruction mov_m_r = {.name = "MOV M r",
      .description = "Move data from register r to memory location whose "
                     "address is in registers H, L",
      .opcode = "01110SSS",
      .size = 1,
      .cycles = 2,
      .states = 7,
      .flags = "none"};

  struct instruction mvi_r_data = {.name = "MVI r, data",
      .description =
          "Content from byte 2 of instruction is moved to register r",
      .opcode = "00DDD110",
      .size = 1,
      .cycles = 2,
      .states = 7,
      .flags = "none"};

  struct instruction mvi_m_data = {.name = "MVI M, data",
      .description = "Content from byte 2 of instruction is moved to memory "
                     "location whose address is in register H,L",
      .opcode = "00110110",
      .size = 3,
      .cycles = 3,
      .states = 10,
      .flags = "none"};

  struct instruction lxi_rp_data_16 = {.name = "LXI rp, data 16",
      .description =
          "byte 3 of instruction moved into high order register rh of pair rp, "
          "byte 2of instruction moved into low order register rl of pair rp",
      .opcode = "00RP0001",
      .size = 3,
      .cycles = 3,
      .states = 10,
      .flags = "none"};

  struct instruction lda_addr = {.name = "LDA addr",
      .description = "content of memory location whose address is in byte 3 "
                     "and byte 2 of instruction is moved to register A",
      .opcode = "00111010",
      .size = 3,
      .cycles = 4,
      .states = 13,
      .flags = "none"};

  struct instruction sta_addr = {.name = "STA addr",
      .description = "content of accumulator is moved to memory location whose "
                     "address is in byte 3 and byte 2 of instruction",
      .opcode = "00110010",
      .size = 3,
      .cycles = 4,
      .states = 13,
      .flags = "none"};

  struct instruction lhld_addr = {.name = "LHLD addr",
      .description = "content of memory location whose address is in byte 3 "
                     "and byte 2 of instruction is moved to register L"
                     "content of memoey localtion at succeddging address is "
                     "moved to register H",
      .opcode = "00101010",
      .size = 3,
      .cycles = 5,
      .states = 16,
      .flags = "none"};

  struct instruction shld_addr = {.name = "SHLD addr",
      .description = "conent of register L moved to memory location whose "
                     "address is in byte 3 and byte 2 of instruction"
                     "content of register H moved to memory location at "
                     "succeeding address",
      .opcode = "00100010",
      .size = 3,
      .cycles = 5,
      .states = 16,
      .flags = "none"};

  struct instruction ldax_rp = {.name = "LDAX rp",
      .description = "content of memory location whose address is in register "
                     "pair rp is moved to register A",
      .opcode = "00RP1010",
      // alloweed register pairs are BC and DE
      .registers_allowed = {register_pairs[0], register_pairs[1]},
      .number_of_registers_allowed = 2,
      .size = 3,
      .cycles = 2,
      .states = 7,
      .flags = "none"};

  struct instruction stax_rp = {.name = "STAX rp",
      .description = "content of register A is moved to memory location whose "
                     "address is in register pair rp",
      .opcode = "00RP0010",
      // alloweed register pairs are BC and DE
      .registers_allowed = {register_pairs[0], register_pairs[1]},
      .number_of_registers_allowed = 2,
      .size = 3,
      .cycles = 2,
      .states = 7,
      .flags = "none"};

  struct instruction xchg = {.name = "XCHG",
      .description = "contents of register H and L are exchanged with contens "
                     "of register D and E",
      .opcode = "11101011",
      .size = 3,
      .cycles = 1,
      .states = 4,
      .flags = "none"};

  struct instruction add_r = {.name = "ADD r",
      .description = "A <- (A) + (r) content of register r is added to content "
                     "of register A",
      .opcode = "10000SSS",
      .size = 1,
      .cycles = 1,
      .states = 4,
      .flags = "Z,S,P,CY,AC"};

  struct instruction add_M = {.name = "ADD M",
      .description = "(A) <- (A) + ((H) (L)) content of memory location whose "
                     "address is contained in "
                     "H and L registers is added to the content of register A"
                     "result is placed in register A",

      .opcode = "10000110",
      .size = 1,
      .cycles = 2,
      .states = 7,
      .flags = "Z,S,P,CY,AC"};

  struct instruction adi_data = {.name = "ADI data",
      .description = "content of second byte of instruction is added to "
                     "content of register A"
                     "result is places in A",
      .opcode = "11000110",
      .size = 1,
      .cycles = 2,
      .states = 7,
      .flags = "Z,S,P,CY,AC"};

  struct instruction adc_r = {.name = "ADC r",
      .description = "(A) <- (A) + (r) + (CY)  content of register r and "
                     "content of carry buit are added to content of register A"
                     "result is placed in register A",

      .opcode = "10001SSS",
      .size = 1,
      .cycles = 1,
      .states = 4,
      .flags = "Z,S,P,CY,AC"};

  struct instruction adc_M = {.name = "ADC M",
      .description =
          "content  of memory location whose address is contained in "
          "H and L registers and content of carry bit are added to "
          "content of register A"
          "result is placed in register A",

      .opcode = "10001110",
      .size = 1,
      .cycles = 2,
      .states = 7,
      .flags = "Z,S,P,CY,AC"};

  struct instruction aci_data = {.name = "ACI data",
      .description = "(A) <- (A) + ((byte 2) + (CY)) ",

      .opcode = "11001110",
      .size = 1,
      .cycles = 2,
      .states = 7,
      .flags = "Z,S,P,CY,AC"};

  struct instruction sub_r = {.name = "SUB r",
      .description = "(A) <- (A) - (r) content of register r is subtracted "
                     "from content of register A"
                     "result is placed in register A",

      .opcode = "10010SSS",
      .size = 1,
      .cycles = 1,
      .states = 4,
      .flags = "Z,S,P,CY,AC"};

  struct instruction sub_M = {.name = "SUB M",
      .description = "(A) <- (A) - ((H) (L)) content of memory location whose "
                     "address is contained in "
                     "H and L registers is subtracted from the content of "
                     "register A"
                     "result is placed in register A",

      .opcode = "10010110",
      .size = 1,
      .cycles = 2,
      .states = 7,
      .flags = "Z,S,P,CY,AC"};

  struct instruction sui_data = {.name = "SUI data",
      .description = "(A) <- (A) - (byte 2) content of second byte of "
                     "instruction is subtracted from content of register A"
                     "result is placed in register A",

      .opcode = "11010110",
      .size = 1,
      .cycles = 2,
      .states = 7,
      .flags = "Z,S,P,CY,AC"};

  struct instruction sbb_r = {.name = "SBB r",
      .description = "(A) <- (A) - (r) - (CY) content of register r and "
                     "content of carry bit are subtracted from content of "
                     "register A"
                     "result is placed in register A",

      .opcode = "10011SSS",
      .size = 1,
      .cycles = 1,
      .states = 4,
      .flags = "Z,S,P,CY,AC"};

  struct instruction sbb_M = {.name = "SBB M",
      .description = "(A) <- (A) - ((H) (L)) - (CY) content of memory "
                     "location whose address is contained in "
                     "H and L registers is subtracted from the content of "
                     "register A"
                     "result is placed in register A",

      .opcode = "10011110",
      .size = 1,
      .cycles = 2,
      .states = 7,
      .flags = "Z,S,P,CY,AC"};

  struct instruction sbi_data = {.name = "SBI data",
      .description =
          "(A) <- (A) - ((byte 2) - (CY)) content of second byte and content "
          "of carry bit are both subtracted from register A"
          "result is placed in register A",

      .opcode = "11011110",
      .size = 1,
      .cycles = 2,
      .states = 7,
      .flags = "Z,S,P,CY,AC"};

  struct instruction inr_r = {.name = "INR r",
      .description = "content of register r is incremented by 1",

      .opcode = "00DDD100",
      .size = 1,
      .cycles = 1,
      .states = 5,
      .flags = "Z,S,P,AC"};

  struct instruction inr_m = {.name = "INR M",
      .description = "((H) (L)) <- ((H)(L)) + 1"
                     "content of memory location whose addres is contained in "
                     "H and L is incremented by 1",

      .opcode = "00110100",
      .size = 1,
      .cycles = 3,
      .states = 10,
      .flags = "Z,S,P,AC"};

  struct instruction dcr_r = {.name = "DCR r",
      .description = "(r) <- (r) - 1 content of register r is decremented by 1",

      .opcode = "00DDD101",
      .size = 1,
      .cycles = 1,
      .states = 5,
      .flags = "Z,S,P,AC"};

  struct instruction dcr_M = {.name = "DCR M",
      .description = "((H) (L)) <- ((H)(L)) - 1"
                     "content of memory location whose addres is contained in "
                     "H and L is decremented by 1",

      .opcode = "00110101",
      .size = 1,
      .cycles = 3,
      .states = 10,
      .flags = "Z,S,P,AC"};

  struct instruction inx_rp = {.name = "INX rp",
      .description = "(rh) (rl) <- (rh) (rl) + 1"
                     "content of register pair rp is incremented by 1",

      .opcode = "00RP0011",
      .size = 1,
      .cycles = 1,
      .states = 5,
      .flags = "none"};

  struct instruction dcx_rp = {.name = "DCX rp",
      .description = "(rh) (rl) <- (rh) (rl) - 1"
                     "content of register pair rp is decremented by 1",

      .opcode = "00RP1011",
      .size = 1,
      .cycles = 3,
      .states = 10,
      .flags = "Z,S,P,AC"};

  struct instruction dad_rp = {.name = "DAD rp",
      .description =
          "(H) (L) <- (H) (L) + (rh) (rl)"
          "content of register pair rp is added to content of register pair H "
          "andL"
          "result is placed in register pair H and L. carry flag is set if "
          "there is carry out of double precision add, oterwise its reset",

      .opcode = "00RP1001",
      .size = 1,
      .cycles = 3,
      .states = 10,
      .flags = "CY"};

  struct instruction daa = {.name = "DAA",
      .description =
          "eight but number in register A is adjusted to form two 4 but binary "
          "coded decimal digits by following process"
          "1. if value of least significant 4 bits of register A is > thatn 9 "
          "or if AC flag is set, 6 is added to register A"
          "2. if value of most significant 4 buts of register A > 9or if CY "
          "flag is set, 6 is added to most significant 4 bits of register A",

      .opcode = "00100111",
      .size = 1,
      .cycles = 1,
      .states = 4,
      .flags = "Z,S,P,AC",
      "CY"};

  struct instruction ana_r = {.name = "ANA r",
      .description = "(A) <- (A) & (r) content of register r is logically "
                     "ANDed with content of register A"
                     "result is placed in register A and CY flag is cleared",

      .opcode = "10100SSS",
      .size = 1,
      .cycles = 1,
      .states = 4,
      .flags = "Z,S,P,AC"};

  struct instruction ana_M = {.name = "ANA M",
      .description = "(A) <- (A) & ((H) (L)) content of memory location whose "
                     "address is contained in "
                     "H and L registers is logically ANDed with content of "
                     "register A"
                     "result is placed in register A and CY flag is cleared",
      .opcode = "10100110",
      .size = 1,
      .cycles = 2,
      .states = 7,
      .flags = "Z,S,P,AC"};

  struct instruction ani_data = {.name = "ANI data",
      .description = "(A) <- (A) & (byte 2) content of second byte of "
                     "instruction is logically ANDed with content of register A"
                     "result is placed in register A and CY flag is cleared",

      .opcode = "11100110",
      .size = 1,
      .cycles = 2,
      .states = 7,
      .flags = "Z,S,P,AC"};

  struct instruction xra_r = {.name = "XRA r",
      .description =
          "(A) <- (A) XOR (r) content of register r is logically "
          "XORed with content of register A"
          "result is placed in register A and CY and AC flag is cleared",

      .opcode = "10101SSS",
      .size = 1,
      .cycles = 1,
      .states = 4,
      .flags = "Z,S,P,AC"};

  struct instruction xra_m = {.name = "XRA M",
      .description =
          "(A) <- (A) XOR ((H) (L)) content of memory location "
          "whose address is contained in "
          "H and L registers is logically XORed with content of "
          "register A"
          "result is placed in register A and CY and AC flag is cleared",
      .opcode = "10101110",
      .size = 1,
      .cycles = 2,
      .states = 7,
      .flags = "Z,S,P,AC"};

  struct instruction xri_data = {.name = "XRI data",
      .description =
          "(A) <- (A) exclusive-or (byte2)"
          "content of second byte of instructoion is exclude-ORd with content "
          "of register A"
          "result is placed on resgiter A, CY and AC flags are cleared",
      .opcode = "11101110",
      .size = 1,
      .cycles = 2,
      .states = 7,
      .flags = "Z,S,P,AC"};

  struct instruction ora_r = {.name = "ORA r",
      .description =
          "(A) <- (A) inclusive-ord (r)"
          "content of register r is includesive ored with content of regsiter A"
          "result is placed in register A, CY and AC flags are cleared",
      .opcode = "10110SSS",
      .size = 1,
      .cycles = 1,
      .states = 4,
      .flags = "Z,S,P,CY,AC"};

  struct instruction ora_m = {.name = "ORA M",
      .description = "(A) <- (A) inclusive-ord ((H) (L))"
                     "content of memory location whose address is contained in "
                     "H and L registers is inclusive ored with content of "
                     "register A"
                     "result is placed in register A, CY and AC flags are "
                     "cleared",
      .opcode = "10110110",
      .size = 1,
      .cycles = 2,
      .states = 7,
      .flags = "Z,S,P,AC,CY"};

  struct instruction ori_data = {.name = "ORI data",
      .description = "(A) <- (A) inclusive ored(byte2) "
                     "content of second byte of instruction is inclusive ored "
                     "with content of register A"
                     "result is placed in register A, CY and AC flags are "
                     "cleared",
      .opcode = "11110110",
      .size = 1,
      .cycles = 2,
      .states = 7,
      .flags = "Z,S,P,AC,CY"};

  struct instruction cmp_r = {.name = "CMP r",
      .description =
          "(A) - (r) content of register r is subtracted from "
          "content of register A"
          "result is not stored, only flags are affected like below"
          "Z flag is set to 1 if (A) = (r), CY flag is set to 1 if (A) < (r)",

      .opcode = "10111SSS",
      .size = 1,
      .cycles = 1,
      .states = 4,
      .flags = "Z,S,P,AC,CY"};

  struct instruction cmp_m = {.name = "CMP M",
      .description =
          "(A) - ((H)(L)) content of memory location whose address "
          "is contained in H and L registers is subtracted from "
          "content of register A"
          "result is not stored, only flags are affected like below"
          "Z flag is set to 1 if (A) = ((H)(L)), CY flag is set to 1 "
          "if (A) < ((H)(L))",

      .opcode = "10111110",
      .size = 1,
      .cycles = 2,
      .states = 7,
      .flags = "Z,S,P,AC,CY"};

  struct instruction cpi_data = {.name = "CPI data",
      .description = "(A) - (byte 2) "
                     "content of second byte of instruction is subtracted from "
                     "content of register A"
                     "condition flags are set"
                     "",

      .opcode = "11111110",
      .size = 1,
      .cycles = 2,
      .states = 7,
      .flags = "Z,S,P,AC,CY"};

  struct instruction rlc = {.name = "RLC",
      .description = "rotate left"
                     "(An+1) <- (An) ; (A0) <- (A7)"
                     "(CY) <- (A7)"
                     "contents of register A are rotated left one bit position"
                     "lower order bit and CY flag are both set to value "
                     "shifted out of high order bit position",
      .opcode = "00000111",
      .size = 1,
      .cycles = 1,
      .states = 4,
      .flags = "CY"};

  struct instruction rrc = {.name = "RRC",
      .description = "rotate right"
                     "(An) <- (AA-1) ; (A7) <- (A0)"
                     "(CY) <- (A0)"
                     "contents of register A are rotated right one bit position"
                     "high order bit and CY flag are both set to value "
                     "shifted out of low order bit position",

      .opcode = "00001111",
      .size = 1,
      .cycles = 1,
      .states = 4,
      .flags = "CY"};

  struct instruction ral = {.name = "RAL",
      .description = "(An+1) <- (An); (A0) <- (CY)"
                     "(CY) <- (A7)"
                     "rotate left through carry"
                     "contents of register A are rotated left one bit position"
                     "through carry flag"
                     "lower bit is set equal to CY flag and CY flag is set to "
                     "value shifted out of high order bit",

      .opcode = "00010111",
      .size = 1,
      .cycles = 1,
      .states = 4,
      .flags = "CY"};

  struct instruction rar = {.name = "RAR",
      .description = "rotate right through carry"
                     "(An) <- (An+1); (CY) <- (A0)"
                     "(A7) <- (CY)",

      .opcode = "00011111",
      .size = 1,
      .cycles = 1,
      .states = 4,
      .flags = "CY"};

  struct instruction cma = {.name = "CMA",
      .description = "(A) <- (A) complement"
                     "contents of register A are complemented"
                     "0 bits become 1, 1 become 0",

      .opcode = "00101111",
      .size = 1,
      .cycles = 1,
      .states = 4,
      .flags = "none"};

  struct instruction cmc = {.name = "CMC",
      .description = "complement carry"
                     "(CY) <- (CY) complement"
                     "CY flag is complemented",

      .opcode = "00111111",
      .size = 1,
      .cycles = 1,
      .states = 4,
      .flags = "CY"};

  struct instruction stc = {.name = "STC",
      .description = "(CY) <- 1"
                     "CY flag is set to 1",

      .opcode = "00110111",
      .size = 1,
      .cycles = 1,
      .states = 4,
      .flags = "CY"};

  struct instruction jmp_addr = {.name = "JMP addr",
      .description = "(PC) <- (byte 3)(byte 2)"
                     "program control is transferred to the memory location "
                     "whose address is specified in the instruction",

      .opcode = "11000011",
      .size = 1,
      .cycles = 3,
      .states = 10,
      .flags = "none"};

  struct instruction jcondition_addr = {.name = "Jcondition addr",
      .description =
          "if (CCC) then (PC) <- (byte 3)(byte 2)"
          "program control is transferred to the memory location "
          "whose address is specified in the byte 3 and 2 of this instruction"
          "if condition is satisfied else control continues normally",
      .number_of_registers_allowed = 8,
      .registers_allowed = {conditions[0], conditions[1], conditions[2],
          conditions[3], conditions[4], conditions[5], conditions[6],
          conditions[7]},
      .opcode = "11CCC010",
      .size = 1,
      .cycles = 3,
      .states = 10,
      .flags = "none"};

  struct instruction call_addr = {.name = "CALL addr",
      .description =
          "((SP) - 1) <- (PCH)"
          "((SP) - 2) <- (PCL)"
          "(SP) <- (SP) - 2"
          "(PC) <- (byte 3) (byte 2)"
          "higher order eight bits of next instruciton address are moved to "
          "memoery location whose address is one less than content of register "
          "SP.low order eight bits of next instruction address are moved to "
          "memory location whose address is 2 less than content of register "
          "SP.content of register SP is decrement ed by 2.control is "
          "transferred to instruction whose address is specified in byte 3 and "
          "bute 2 of current instruction",

      .opcode = "11001101",
      .size = 3,
      .cycles = 5,
      .states = 17,
      .flags = "none"};

  struct instruction ccondition_addr = {.name = "Ccondition addr",
      .description = "if (CCC)"
                     "((SP) - 1) <- (PCH)"
                     "((SP) - 2) <- (PCL)"
                     "(SP) <- (SP) - 2"
                     "(PC) <- (byte 3) (byte 2)"
                     "if specified condition is true then actions specified in "
                     "call instruction are performed",
      .number_of_registers_allowed = 8,
      .registers_allowed = {conditions[0], conditions[1], conditions[2],
          conditions[3], conditions[4], conditions[5], conditions[6],
          conditions[7]},
      .opcode = "11CCC100",
      .size = 3,
      .cycles = 3 / 5,
      .states = 11 / 17,
      .flags = "none"};

  struct instruction ret = {.name = "RET",
      .description =
          "(PCL) <- ((SP))"
          "(PCH) <- ((SP) + 1)"
          "(SP) <- (SP) + 2"
          "content of memory location whose address is specificed in register "
          "SP is moved to low order 8 bits of register PC"
          "content of memory location whose address is 1 more than content of "
          "register SP is moved to high order 8 bits of register PC."
          "content of register SP is incremented by 2",
      .opcode = "11001001",
      .size = 1,
      .cycles = 3,
      .states = 10,
      .flags = "none"};

  struct instruction rcondition = {.name = "Rcondition",
      .description = "conditional return"
                     "if (CCC)"
                     "(PCL) <- ((SP))"
                     "(PCH) <- ((SP) + 1)"
                     "(SP) <- (SP) + 2"
                     "if specified condition is true then actions specified in "
                     "return instruction are performed",
      .number_of_registers_allowed = 8,
      .registers_allowed = {conditions[0], conditions[1], conditions[2],
          conditions[3], conditions[4], conditions[5], conditions[6],
          conditions[7]},
      .opcode = "11CCC000",
      .size = 1,
      .cycles = 1 / 3,
      .states = 5 / 11,
      .flags = "nonw"};

  struct instruction rst_n = {
      .name = "RST n", // ignore this instruction for now, needs to handle NNN
      .description = "restart (figure out NNN in opcode later)"
                     "((SP) - 1) <- (PCH)"
                     "((SP) - 2) <- (PCL)"
                     "(SP) <- (SP) - 2"
                     "(PC) <- 8 * (NNN)",

      .opcode = "11NNN111",
      .size = 1,
      .cycles = 3,
      .states = 11,
      .flags = "none"};

  struct instruction pchl = {.name = "PCHL",
      .description = "jump H and L indirect -> move H and L to PC"
                     "(PCH) <- (H)"
                     "(PCL) <- (L)"
                     "",

      .opcode = "11101001",
      "contens fo register H ais modev to hight order 8 bits of register PC.",
      .size = 1,
      .cycles = 1,
      .states = 5,
      .flags = "none"};

  struct instruction push_rp = {.name = "PUSH rp",
      .description =
          "((SP) - 1) <- (rh)"
          "((SP) - 2) <- (rl)"
          "(SP) <- (SP) - 2"
          "content of high order register pair rp is moved to memory location "
          "whose address is one less than content of register SP"
          "content of low order register pair rp is moved to memory location "
          "whose address is 2 less than content of register SP"
          "content of register SP is decremented by 2"
          "register pair RP is now allowed",
      .registers_allowed = {register_pairs[0], register_pairs[1],
          register_pairs[2]},
      .number_of_registers_allowed = 3,
      .opcode = "11RP0101",
      .size = 1,
      .cycles = 3,
      .states = 11,
      .flags = "none"};

  struct instruction push_psw = {.name = "PUSH PSW",
      .description = "push processor status word"
                     "((SP) - 1) <- (A)"
                     "((SP) - 2)0 <- (CY), ((SP) - 2)1 <- 1"
                     "((SP) - 2)2 <- (P), ((SP) - 2)3 <- 0"
                     "((SP) - 2)4 <- (AC), ((SP) - 2)5 <- 0"
                     "((SP) - 2)6 <- (Z), ((SP) - 2)7 <- (S)"
                     "(SP) <- (SP) - 2",

      .opcode = "11110101",
      .size = 1,
      .cycles = 3,
      .states = 11,
      .flags = "none"};

  struct instruction pop_rp = {.name = "POP rp",
      .description =
          "(rl) <- ((SP))"
          "(rh) <- ((SP) + 1)"
          "(SP) <- (SP) + 2"
          "content of memory location whose address is specified by content of "
          "regsiter SP is moved to low order regsister of register pir "
          "rp.content  of memory location whose address is 1 more than  "
          "content of register pair SP is moved to high order regisster of "
          "register pair rp.content of register SP is incremented by 2. rp = "
          "SP is not allowed",

      .opcode = "11RP0001",
      .registers_allowed = {register_pairs[0], register_pairs[1],
          register_pairs[2]},
      .number_of_registers_allowed = 3,
      .size = 1,
      .cycles = 3,
      .states = 10,
      .flags = "none"};

  struct instruction pop_psw = {.name = "POP PSW",
      .description =
          "pop processor status word"
          "(CY) <- ((SP))0"
          "(P) <- ((SP))2"
          "(AC) <- ((SP))4"
          "(Z) <- ((SP))6"
          "(S) <- ((SP))7"
          "(S) <- ((SP) + 1)"
          "(SP) <- (SP) + 2"
          "content of memory location whose address is specified by content of "
          "register SP is used to restore condition flags."
          "content of memory location whose address is 1 more than content of "
          "regsiter SP is moved to register A"
          "content of register SP is incremented by 2",

      .opcode = "11110001",
      .size = 1,
      .cycles = 3,
      .states = 10,
      .flags = "Z,S,P,CY,AC"};

  struct instruction xthl = {.name = "XTHL",
      .description =
          "exchange stack top with H and L"
          "(L) <-> ((SP))"
          "(H) <-> ((SP) + 1)"
          "content of register L is exchanged with content of memory location "
          "whose address is specified by content of register SP"
          "content of register H is exchanged with content of memory location "
          "whose address is 1 more than content of register SP",
      .opcode = "11100011",
      .size = 1,
      .cycles = 5,
      .states = 18,
      .flags = "none"};

  struct instruction sphl = {.name = "SPHL",
      .description = "move HL to SP"
                     "(SP) <- (H) (L)"
                     "content of register pair HL is moved to register SP",

      .opcode = "11111001",
      .size = 1,
      .cycles = 1,
      .states = 5,
      .flags = "none"};

  struct instruction in_port = {.name = "IN port",
      .description = "(A) <- data"
                     "The data placed on data bys by specified port (byte 2) "
                     "is moved to register A",

      .opcode = "11011011",
      .size = 2,
      .cycles = 3,
      .states = 10,
      .flags = "none"};

  struct instruction out_port = {.name = "OUT port",
      .description = "(data) <- (A)"
                     "content of regsiter A is placed on 8 bit data bus for "
                     "transmission to specified port",

      .opcode = "11010011",
      .size = 1,
      .cycles = 3,
      .states = 10,
      .flags = "none"};

  struct instruction ei = {.name = "EI",
      .description = "enable interrupts -> interrupt system is enabled "
                     "following the execution of next instruction",

      .opcode = "11111011",
      .size = 1,
      .cycles = 1,
      .states = 4,
      .flags = "none"};

  struct instruction di = {.name = "DI",
      .description = "disable interrupts -> interrupt system is disabled "
                     "following the execution of next instruction",

      .opcode = "11110011",
      .size = 1,
      .cycles = 1,
      .states = 4,
      .flags = "none"};

  struct instruction hlt = {.name = "HLT",
      .description = "prcessor is stopped, registers and flags are unaffected",

      .opcode = "01110110",
      .size = 1,
      .cycles = 1,
      .states = 7,
      .flags = "none"};

  struct instruction nop = {.name = "no op",
      .description =
          "no operation is performed, regsiters and flags are unaffected",

      .opcode = "00000000",
      .size = 1,
      .cycles = 1,
      .states = 4,
      .flags = "none"};

  instructions[0] = mov_r1_r2;
  instructions[1] = mov_r_m;
  instructions[2] = mov_m_r;
  instructions[3] = mvi_r_data;
  instructions[4] = mvi_m_data;
  instructions[5] = lxi_rp_data_16;
  instructions[6] = lda_addr;
  instructions[7] = sta_addr;
  instructions[8] = lhld_addr;
  instructions[9] = shld_addr;
  instructions[10] = ldax_rp;
  instructions[11] = stax_rp;
  instructions[12] = xchg;
  instructions[13] = add_r;
  instructions[14] = add_M;
  instructions[15] = adi_data;
  instructions[16] = adc_r;
  instructions[17] = adc_M;
  instructions[18] = aci_data;
  instructions[19] = sub_r;
  instructions[20] = sub_M;
  instructions[21] = sui_data;
  instructions[22] = sbb_r;
  instructions[23] = sbb_M;
  instructions[24] = sbi_data;
  instructions[25] = inr_r;
  instructions[26] = inr_m;
  instructions[27] = dcr_r;
  instructions[28] = dcr_M;
  instructions[29] = inx_rp;
  instructions[30] = dcx_rp;
  instructions[31] = dad_rp;
  instructions[32] = daa;
  instructions[33] = ana_r;
  instructions[34] = ana_M;
  instructions[35] = ani_data;
  instructions[36] = xra_r;
  instructions[37] = xra_m;
  instructions[38] = xri_data;
  instructions[39] = ora_r;
  instructions[40] = ora_m;
  instructions[41] = ori_data;
  instructions[42] = cmp_r;
  instructions[43] = cmp_m;
  instructions[44] = cpi_data;
  instructions[45] = rlc;
  instructions[46] = rrc;
  instructions[47] = ral;
  instructions[48] = rar;
  instructions[49] = cma;
  instructions[50] = cmc;
  instructions[51] = stc;
  instructions[52] = jmp_addr;
  instructions[53] = jcondition_addr;
  instructions[54] = call_addr;
  instructions[55] = ccondition_addr;
  instructions[56] = ret;
  instructions[57] = rcondition;
  instructions[58] = rst_n;
  instructions[59] = pchl;
  instructions[60] = push_rp;
  instructions[61] = push_psw;
  instructions[62] = pop_rp;
  instructions[63] = pop_psw;
  instructions[64] = xthl;
  instructions[65] = sphl;
  instructions[66] = in_port;
  instructions[67] = out_port;
  instructions[68] = ei;
  instructions[69] = di;
  instructions[70] = hlt;
  instructions[71] = nop;
}

void trimSpaces(char* str) {
  char* end;
  // Trim leading spaces
  while (isspace((unsigned char) *str))
    str++;

  if (*str == 0) { // All spaces?
    *str = '\0';
    return;
  }

  // Trim trailing spaces
  end = str + strlen(str) - 1;
  while (end > str && isspace((unsigned char) *end))
    end--;

  // Write new null terminator
  *(end + 1) = '\0';
}

void replace_string_with_len(
    char* str, char* substr, char* replacement, int len) {
  trimSpaces(substr);
  char* pos = strstr(str, substr);
  if (pos != NULL) {
    memmove(pos, replacement, len);
  }
}

void replace_string(char* str, char* substr, char* replacement) {
  trimSpaces(substr);

  char* pos = strstr(str, substr);
  if (pos != NULL) {
    memmove(pos, replacement, strlen(replacement));
  }
}

void replace_r1_r2_registers(struct instruction instruction,
    register_ registers[], char* src_reg_name, char* dst_reg_name,
    char* substr1, char* substr2, FILE* f) {
  char opcode[9];
  strcpy(opcode, instruction.opcode);
  char* dst_reg = strstr(opcode, substr1);
  char* src_reg = strstr(opcode, substr2);

  for (int j = 0; j < 7; j++) {
    for (int k = 0; k < 7; k++) {
      char name[20];

      strcpy(name, instruction.name);

      char* src_reg_name_position = strstr(name, src_reg_name);

      if (src_reg != NULL) {
        memmove(src_reg, registers[j].value, 3);
        // replace_string_with_len(opcode, substr2, registers[j].value, 3);

        // if (src_reg_name_position != NULL) {
        // memmove(src_reg_name_position, registers[j].name, 2);
        // printf("size of %d\n", strlen(registers[j].name));
        replace_string_with_len(name, src_reg_name, registers[j].name, 2);
        // }
      }

      char* dst_reg_name_position = strstr(name, dst_reg_name);

      if (dst_reg != NULL) {
        memmove(dst_reg, registers[k].value, 3);
        // replace_string(opcode, substr1, registers[k].value);
        if (dst_reg_name_position != NULL) {
          memmove(dst_reg_name_position, registers[k].name, 1);
          memmove(dst_reg_name_position + 1, dst_reg_name_position + 2, 10);
        }
      }
      int instruction_opcde = strtol(opcode, NULL, 2);

      fprintf(f, "%s, 0x%x, %s, %d, %d, %d, %s, %s\n", name, instruction_opcde,
          opcode, instruction.size, instruction.cycles, instruction.states,
          instruction.description, instruction.flags);
    }
  }
}

void replace_r_M_registers(struct instruction instruction,
    register_ registers[], char* src_reg_name, char* dst_reg_name,
    char* substr1, char* substr2, char* single_register_name, char* memory_name,
    FILE* f) {
  // replace r and M

  for (int i = 0; i < 7; i++) {
    char opcode[9];
    char name[strlen(instruction.name)];

    strcpy(opcode, instruction.opcode);
    strcpy(name, instruction.name);

    replace_string(opcode, substr1, registers[i].value);
    replace_string(opcode, substr2, registers[i].value);
    replace_string(name, single_register_name, registers[i].name);

    int instruction_opcode = strtol(opcode, NULL, 2);
    fprintf(f, "%s, 0x%x, %s, %d, %d, %d, %s, %s\n", name, instruction_opcode,
        opcode, instruction.size, instruction.cycles, instruction.states,
        instruction.description, instruction.flags);
  }
}

void replace_r_registers(struct instruction instruction, register_ registers[],
    char* src_reg_name, char* dst_reg_name, char* substr1, char* substr2,
    char* single_register_name, char* data_name, FILE* f) {
  // replace r and data

  for (int i = 0; i < 7; i++) {
    char opcode[9];
    char name[strlen(instruction.name)];

    strcpy(opcode, instruction.opcode);
    strcpy(name, instruction.name);

    replace_string(opcode, substr1, registers[i].value);
    replace_string(opcode, substr2, registers[i].value);
    replace_string(name, single_register_name, registers[i].name);

    int instruction_opcode = strtol(opcode, NULL, 2);
    fprintf(f, "%s, 0x%x, %s, %d, %d, %d, %s, %s\n", name, instruction_opcode,
        opcode, instruction.size, instruction.cycles, instruction.states,
        instruction.description, instruction.flags);
  }
}

void replace_rp_registers(struct instruction instruction,
    register_ register_pairs[], char* src_reg_name, char* dst_reg_name,
    char* op_rp, char* register_pair_name, char* data_name, FILE* f) {
  // replace rp

  register_* pairs_to_use;
  int number_of_pairs_to_use;

  if (instruction.registers_allowed[0].name != NULL) {
    pairs_to_use = instruction.registers_allowed;
    number_of_pairs_to_use = instruction.number_of_registers_allowed;
  } else {
    pairs_to_use = register_pairs;
    number_of_pairs_to_use = 4;
  }

  for (int i = 0; i < number_of_pairs_to_use; i++) {
    char opcode[9];
    char name[strlen(instruction.name)];

    strcpy(opcode, instruction.opcode);
    strcpy(name, instruction.name);
    printf("name %s\n", name); 

    replace_string(opcode, op_rp, pairs_to_use[i].value);
    // replace_string_with_len(name, register_pair_name, pairs_to_use[i].name, 3);
    replace_string(name, register_pair_name, pairs_to_use[i].name);
    printf("name %s\n", name); 


    int instruction_opcode = strtol(opcode, NULL, 2);
    fprintf(f, "%s, 0x%x, %s, %d, %d, %d, %s, %s\n", name, instruction_opcode,
        opcode, instruction.size, instruction.cycles, instruction.states,
        instruction.description, instruction.flags);
  }
}

void replace_condition_registers(struct instruction instruction,
    char* op_condiion, register_ conditions[], FILE* f) {
  // replace CCC in opcode, no change in the name

  register_* pairs_to_use;
  int number_of_pairs_to_use;

  pairs_to_use = instruction.registers_allowed;
  number_of_pairs_to_use = instruction.number_of_registers_allowed;

  for (int i = 0; i < number_of_pairs_to_use; i++) {
    char opcode[9];
    char name[strlen(instruction.name)];

    strcpy(name, instruction.name);
    strcpy(opcode, instruction.opcode);

    replace_string(opcode, op_condiion, pairs_to_use[i].value);

    int instruction_opcode = strtol(opcode, NULL, 2);
    fprintf(f, "%s, 0x%x, %s, %d, %d, %d, %s, %s\n", name, instruction_opcode,
        opcode, instruction.size, instruction.cycles, instruction.states,
        instruction.description, instruction.flags);
  }
}

void replace_restart_opcode(struct instruction instruction, char* restart_name, FILE* f)
{
    char opcode[9];
    char name[strlen(instruction.name)];
    
    for(int i=0; i<8; i++){
        char to_replace[4] = {
        to_replace[0] = (i & 4) ? '1' : '0',
        to_replace[1] = (i & 2)  ? '1' : '0',
        to_replace[2] = (i & 1)  ? '1' : '0',
        '\0'
        };

        // printf("rst replacing with %s\n", to_replace);
        strcpy(name, instruction.name);
        strcpy(opcode, instruction.opcode);

        replace_string(opcode, restart_name, to_replace);

        int instruction_opcode = strtol(opcode, NULL, 2);
        fprintf(f, "%s, 0x%x, %s, %d, %d, %d, %s, %s\n", name, instruction_opcode,
            opcode, instruction.size, instruction.cycles, instruction.states,
            instruction.description, instruction.flags);
    }
}

int main() {
  struct instruction instructions[100];
  register_ registers[8];
  register_ register_pairs[4];
  register_ conditions[200];

  fill_register_array(registers);
  fill_register_pair(register_pairs);
  fill_conditions(conditions);
  fill_instructions(instructions, register_pairs, conditions);

  FILE* f = fopen("./opcode_to_instruction.txt", "w");
  char substr1[4] = "DDD";
  char substr2[4] = "SSS";
  char op_rp[4] = "RP";

  char src_reg_name[3] = "r2";
  char dst_reg_name[3] = "r1";

  char single_register_name[3] = "r";
  char memory_name[3] = " M";
  char data_name[6] = "data";
  char register_pair_name[4] = "rp";
  char address_name[5] = "addr";
  char port_name[5] = "port";
  char condition_name[4] = "CCC";
  char restart_name[4] = "NNN";

  for (int i = 0; i < TOTAL_INSTRUCTIONS_TO_GENERATE; i++) {
    fprintf(f, "\n");
    printf("processing instruction %s\n", instructions[i].name);

    struct instruction k = instructions[i];

    char* src_reg_position = strstr(k.name, src_reg_name);
    char* dst_reg_position = strstr(k.name, dst_reg_name);
    char* single_register_position = strstr(k.name, single_register_name);
    char* memory_position = strstr(k.name, memory_name);
    char* data_position = strstr(k.name, data_name);
    char* register_pair_position = strstr(k.name, register_pair_name);
    char* address_position = strstr(k.name, address_name);
    char* port_position = strstr(k.name, port_name);
    char* condition_position_in_opcode = strstr(k.opcode, condition_name);
    char* restart_position_in_opcode = strstr(k.opcode, restart_name);

    if (src_reg_position != NULL || dst_reg_position != NULL) {
      printf("r1 r2 proc \n");
      replace_r1_r2_registers(
          k, registers, src_reg_name, dst_reg_name, substr2, substr1, f);
    } else if (single_register_position != NULL && memory_position != NULL) {
      printf("r M proc \n");
      replace_r_M_registers(k, registers, src_reg_name, dst_reg_name, substr1,
          substr2, single_register_name, memory_name, f);
    } else if (single_register_position != NULL && address_position == NULL &&
               register_pair_position == NULL && port_position == NULL) {
      printf("r proc \n");
      replace_r_registers(k, registers, src_reg_name, dst_reg_name, substr1,
          substr2, single_register_name, data_name, f);
    }

    else if (register_pair_position != NULL) {
      printf("rp proc \n");
      replace_rp_registers(k, register_pairs, src_reg_name, dst_reg_name, op_rp,
          register_pair_name, data_name, f);
    }

    else if (condition_position_in_opcode != NULL) {
      printf("condition proc \n");
      replace_condition_registers(k, condition_name, conditions, f);
    }

    else if (restart_position_in_opcode != NULL )
    {
        printf("restart proc \n");
        replace_restart_opcode(k, restart_name, f);
    }
    
    else {
      int instruction_opcde = strtol(instructions[i].opcode, NULL, 2);

      fprintf(f, "%s, 0x%x, %s, %d, %d, %d, %s, %s\n", instructions[i].name,
          instruction_opcde, instructions[i].opcode, instructions[i].size,
          instructions[i].cycles, instructions[i].states,
          instructions[i].description, instructions[i].flags);
    }
  }

  fclose(f);
}
