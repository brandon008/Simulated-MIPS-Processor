#include <stdio.h>
#include <string.h>
#include "j_instructions.h"
#include "cpu.h"

#define OPCODE(x) ((x>>26) & (0x3F))

//0-25
#define ADDRESS(x) ((x) & (0x3FFFFFF))

void j(unsigned int instruction){
  unsigned int address = ADDRESS(instruction);
  npc = ((pc+4) & 0xf0000000)|(address<<2);
}
void jal(unsigned int instruction){
  unsigned int address = ADDRESS(instruction);

  registers[31] = pc + 4;
  npc = ((pc + 4) & 0xf0000000)|(address<<2);
}
