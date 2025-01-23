# 8080

## to run space invader

`cd games/space_invaders`
`clang main.c ../../8080_emulator_new.c -lSDL2 -o main && ./main`

## to generate list of all possible opcodes in a single file
`clang opcode_to_instructions.c`

## Resources used

- cpu instruction from "8080 Data Sheet" included in this repo.
- Space Invader hardware info from [https://computerarcheology.com/Arcade/SpaceInvaders/Hardware.html]
- reference emulator [https://github.com/superzazu/8080]
