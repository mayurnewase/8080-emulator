LXI SP, data
LXI HL, data
PUSH DE
XCHG
MVI C, data
OUT data
POP DE
ANI data
ADI data
ADI data
ADI data
ADI data
CPI data
CPI data
CPI data
ACI data
ACI data
CPI data
SUI data
SUI data
CPI data
SBI data
SBI data
CPI data
ANI data
CPI data
ORI data
CPI data
XRI data
CPI data
ANI data
CPI data
SUI data
CPI data
ANI data
ADI data
ADI data
ADI data
ADI data
ADI data
ADI data
ADI data
ADI data
CPI data
ADI data
ADI data
SUI data
ADI data
ADI data
ADI data
ADI data
CPI data
MVI A, data
INR A
MOV B, A
INR B
MOV C, B
DCR C
MOV D, C
MOV E, D
MOV H, E
MOV L, H
MOV A, L
DCR A
MOV C, A
MOV E, C
MOV L, E
MOV B, L
MOV D, B
MOV H, D
MOV A, H
MOV D, A
INR D
MOV L, D
MOV C, L
INR C
MOV H, C
MOV B, H
DCR B
MOV E, B
MOV A, E
MOV E, A
INR E
MOV B, E
MOV H, B
INR H
MOV C, H
MOV L, C
MOV D, L
DCR D
MOV A, D
MOV H, A
DCR H
MOV D, H
MOV B, D
MOV L, B
INR L
MOV E, L
DCR E
MOV C, E
MOV A, C
MOV L, A
DCR L
MOV H, L
MOV E, H
MOV D, E
MOV C D
MOV B, C
MOV A, B
CPI data
XRA A
MVI B, data
MVI C, data
MVI D, data
MVI E, data
MVI H, data
MVI L, data
ADD B
ADD C
ADD D
ADD E
ADD H
ADD L
ADD A
CPI data
SUB B
SUB C
SUB D
SUB E
SUB H
SUB L
CPI data
SUB A
MVI A, data
ADD A
MVI B, data
MVI C, data
MVI D, data
MVI E, data
MVI H, data
MVI L, data
ADC B
MVI B, data
ADD B
ADD B
ADC C
ADD B
ADD B
ADC D
ADD B
ADD B
ADC E
ADD B
ADD B
ADC H
ADD B
ADD B
ADC L
ADD B
ADD B
ADC A
CPI data
MVI A, data
ADD A
MVI B, data
SBB B
MVI B, data
ADD B
SBB C
ADD B
SBB D
ADD B
SBB E
ADD B
SBB H
ADD B
SBB L
CPI data
MVI A, data
ADD A
SBB A
CPI data
MVI A, data
MVI B, data
MVI C, data
MVI D, data
MVI E, data
MVI H, data
MVI L, data
ANA B
ANA C
ANA D
ANA E
ANA H
ANA L
ANA A
CPI data
XRA A
MVI B, data
MVI C, data
MVI D, data
MVI E, data
MVI H, data
MVI L, data
ORA B
ORA C
ORA D
ORA E
ORA H
ORA L
ORA A
CPI data
MVI A, data
MVI H, data
MVI L, data
XRA B
XRA C
XRA D
XRA E
XRA H
XRA L
CPI data
XRA A
MVI B, data
MVI C, data
MVI D, data
MVI E, data
MVI H, data
MVI L, data
MOV M, B
MVI B, data
MOV B, M
MVI A, data
CMP B
MOV M, C
MVI C, data
MOV C, M
MVI A, data
CMP C
MOV M, D
MVI D, data
MOV D, M
MVI A, data
CMP D
MOV M, E
MVI E, data
MOV E, M
MVI A, data
CMP E
MOV M, H
MVI H, data
MVI L, data
MOV H, M
MVI A, data
CMP H
MOV M, L
MVI H, data
MVI L, data
MOV L, M
MVI A, data
CMP L
MVI H, data
MVI L, data
MVI A, data
MOV M, A
CMP M
ADD M
CPI data
XRA A
MOV A, M
CPI data
MVI H, data
MVI L, data
MOV A, M
SUB M
MVI A, data
ADD A
ADC M
CPI data
MVI A, data
ADD A
SBB M
CPI data
ANA M
MVI A, data
ORA M
CPI data
XRA M
CPI data
MVI M, data
INR M
DCR M
ADD M
CPI data
LXI BC, data
LXI DE, data
LXI HL, data
INX BC
INX DE
INX HL
MVI A, data
CMP B
CMP D
CMP H
MVI A, data
CMP C
CMP E
CMP L
DCX BC
DCX DE
DCX HL
MVI A, data
CMP B
CMP D
CMP H
MVI A, data
CMP C
CMP E
CMP L
STA addr
XRA A
LDA addr
CPI data
LHLD addr
SHLD addr
LDA addr
MOV B, A
LDA addr
CMP B
LDA addr
MOV B, A
LDA addr
CMP B
MVI A, data
STA addr
MOV B, H
MOV C, L
XRA A
LDAX BC
CPI data
INR A
STAX BC
LDA addr
CPI data
MVI A, data
STA addr
LHLD addr
LXI DE, data
XCHG
XRA A
LDAX DE
CPI data
XRA A
ADD H
ADD L
MVI A, data
STAX DE
LDA addr
CPI data
STAX DE
LDA addr
CPI data
LXI HL, data
DAD HL
MVI A, data
CMP H
CMP L
LXI HL, data
LXI BC, data
DAD BC
MVI A, data
CMP H
MVI A, data
CMP L
LXI HL, data
LXI DE, data
DAD DE
MVI A, data
CMP H
CMP L
STC
CMC
MVI A, data
CMA
CPI data
ORA A
DAA
CPI data
MVI A, data
ADD A
DAA
CPI data
XRA A
MVI A, data
DAA
CPI data
XRA A
MVI A, data
DAA
STC
MVI A, data
RLC
RLC
CPI data
RRC
RRC
CPI data
RAL
RAL
CPI data
RAR
RAR
CPI data
LXI BC, data
LXI DE, data
LXI HL, data
XRA A
PUSH BC
PUSH DE
PUSH HL
PUSH PSW 
LXI BC, data
LXI DE, data
LXI HL, data
MVI A, data
ADI data
POP PSW 
POP HL
POP DE
POP BC
MVI A, data
CMP B
MVI A, data
CMP C
MVI A, data
CMP D
CMP E
MVI A, data
CMP H
CMP L
LXI HL, data
DAD SP
SHLD addr
LXI SP, data
DCX SP
DCX SP
INX SP
DCX SP
MVI A, data
STA addr
CMA
STA addr
POP BC
CMP B
CMA
CMP C
LXI HL, data
SPHL
LXI HL, data
DCX SP
DCX SP
XTHL
LDA addr
CPI data
LDA addr
CPI data
MVI A, data
CMP L
CMA
CMP H
LHLD addr
SPHL
LXI HL, data
PCHL
LXI HL, data
PUSH DE
XCHG
MVI C, data
OUT data
POP DE
NOP
NOP
