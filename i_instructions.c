#include <stdio.h>
#include <string.h>
#include "r_instructions.h"
//#include "test_programs.c"
#include "cpu.h"

#define OPCODE(x) ((x>>26) & (0x3F))
// rs first 5 after Opcode (bit 25 - 21 )
#define RS(x) ((x>>21) & (0x1F))
// rt first 5 after rs (bit 20 - 16)
#define RT(x) ((x>>16) & (0x1F))

// IMM last 16 bits (16-0)
#define IMM(x) ((x) & (0xFFFF))

unsigned int sign_extendf(unsigned int param);

unsigned int sign_extendf(unsigned int param){
  if ((param >> 15) & 1){
    param |= ~(0xFFFF);
    return param;
  } else{
    return param;
  }
}


void addi(unsigned int instruction){
   int rs = RS(instruction);
   int rt = RT(instruction);
   int imm = IMM(instruction);
   int sign_extend = ~(0xFFFF);

  if((imm >> 15) & 1){
      imm |= sign_extend;
  }

  registers[rt] = (unsigned int)((int)registers[rs] + (int)imm);
}

void addiu(unsigned int instruction){
  unsigned int rs = RS(instruction);
  unsigned int rt = RT(instruction);
  unsigned int imm = IMM(instruction);
  unsigned int sign_extend = ~(0xFFFF);

  if((imm >> 15) & 1){
      imm |= sign_extend;
  }

  registers[rt] = registers[rs] + imm;

}

void andi(unsigned int instruction){
  unsigned int rs = RS(instruction);
  unsigned int rt = RT(instruction);
  unsigned int imm = IMM(instruction);

  registers[rt] = (registers[rs] & imm);
}

void ori(unsigned int instruction){
  unsigned int rs = RS(instruction);
  unsigned int rt = RT(instruction);
  unsigned int imm = IMM(instruction);

  registers[rt] = (registers[rs] | imm);
}

void xori(unsigned int instruction){
  unsigned int rs = RS(instruction);
  unsigned int rt = RT(instruction);
  unsigned int imm = IMM(instruction);

  registers[rt] = registers[rs]^imm;
}

void slti(unsigned int instruction){
   int rs = (int)RS(instruction);
   int imm = (int)IMM(instruction);
   int rt = (int)RT(instruction);

  if((int)registers[rs] < (int)sign_extendf(imm)){
    registers[rt] = 1;
  } else{
    registers[rt] = 0;
  }
}

void sltiu(unsigned int instruction){
  unsigned int rs = RS(instruction);
  unsigned int imm = IMM(instruction);
  unsigned int rt = RT(instruction);

 if(registers[rs] < sign_extendf(imm)){
   registers[rt] = 1;
 } else{
   registers[rt] = 0;
 }
}

void beq(unsigned int instruction){
  unsigned int rs = RS(instruction);
  unsigned int rt = RT(instruction);
  unsigned int imm = IMM(instruction);

  if(registers[rs] == registers[rt]){
    npc = pc+4+(sign_extendf(imm) << 2);
  }
}

void bne(unsigned int instruction){
  unsigned int rs = RS(instruction);
  unsigned int rt = RT(instruction);
  unsigned int imm = IMM(instruction);

  if (registers[rs] != registers[rt]){
    npc = pc + 4 + (sign_extendf(imm) << 2);
  }
}

void lb(unsigned int instruction){ //unsigned
   int rs = RS(instruction);
   int rt = RT(instruction);
   int imm = IMM(instruction);
   int mask8 = (1<<7);
   int sign_extend = ~(0xFFFF);
   int sign_extend2 = ~(0xFF);


  if((imm >> 15) & 1){
      imm |= sign_extend;
  }
  char *p = (char *)(&memory[(int)registers[rs] + (int)imm]);
  if(mask8 & *p){
    *p = *p | sign_extend2;
  }
  registers[rt] = *p;
}

void lbu(unsigned int instruction){
  unsigned int rs = RS(instruction);
  unsigned int rt = RT(instruction);
  unsigned int imm = IMM(instruction);
  unsigned char *p = (unsigned char *)&memory[registers[rs] + imm];
  registers[rt] = *p;
}

void lh(unsigned int instruction){ // supposed to be unsigned
   int rs = RS(instruction);
   int rt = RT(instruction);
   int imm = IMM(instruction);
   int mask8 = (1<<7);
   int sign_extend2 = ~(0xFF);
   int sign_extend = ~(0xFFFF);


  if((imm >> 15) & 1){
      imm |= sign_extend;
  }
  short *p = (short *)&memory[(int)registers[rs] + (int)imm];
  if(mask8 & *p){
    *p = *p | sign_extend2;
  }
  registers[rt] = *p;
}

void lhu(unsigned int instruction){
   unsigned int rs = RS(instruction);
   unsigned int rt = RT(instruction);
   unsigned int imm = IMM(instruction);
   unsigned short *p = (unsigned short *)&memory[registers[rs] + imm];
   registers[rt] = *p;
}

void lw(unsigned int instruction){
  unsigned int rs = RS(instruction);
  unsigned int rt = RT(instruction);
  unsigned int imm = IMM(instruction);
  unsigned int sign_extend = ~(0xFFFF);

  if((imm >> 15) & 1){
      imm |= sign_extend;
  }

  unsigned int *p = (unsigned int *)(&memory[registers[rs]+imm]);
  registers[rt] = *p;
}

void lui(unsigned int instruction){
  unsigned int rt = RT(instruction);
  unsigned int imm = IMM(instruction);

  registers[rt] = (imm << 16);
}

void sb(unsigned int instruction){
  unsigned int rs = RS(instruction);
  unsigned int rt = RT(instruction);
  unsigned int imm = IMM(instruction);
  unsigned int sign_extend = ~(0xFFFF);

  if((imm >> 15) & 1){
      imm |= sign_extend;
  }
  memory[registers[rs] + imm] = registers[rt];
}

void sh(unsigned int instruction){
  unsigned int rs = RS(instruction);
  unsigned int rt = RT(instruction);
  unsigned int imm = IMM(instruction);
  unsigned int sign_extend = ~(0xFFFF);

  if((imm >> 15) & 1){
      imm |= sign_extend;
  }
  unsigned short *p = (unsigned short *)(&memory[(int)registers[rs] + (int)imm]);
  *p = registers[rt];
}

void sw(unsigned int instruction){
  unsigned int rs = RS(instruction);
  unsigned int rt = RT(instruction);
  unsigned int imm = IMM(instruction);
  unsigned int sign_extend = ~(0xFFFF);
  if((imm >> 15) & 1){
      imm |= sign_extend;
  }
  unsigned int *p = (unsigned int *)(&memory[registers[rs]+imm]);
  *p = registers[rt];
}
