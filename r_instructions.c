#include <stdio.h>
#include <string.h>
#include "r_instructions.h"
//#include "test_programs.c"
#include "cpu.h"




// first 6 bits (bit 31 - 26 )
#define OPCODE(x) ((x>>26) & (0x3F))

// rs first 5 after Opcode (bit 25 - 21 )
#define RS(x) ((x>>21) & (0x1F))

// rt first 5 after rs (bit 20 - 16)
#define RT(x) ((x>>16) & (0x1F))

// rd first 5 after ft (bit 15 - 11)
#define RD(x) ((x>>11) & (0x1F))

// shamt first 5 after rd (10 - 6)
#define SHAMT(x) ((x>>6) & (0x1F))

// funt first 6 after shamt (5 - 0)
#define FUNCT(x) ((x) & (0x3F))


void add(unsigned int instruction){

   int rs = RS(instruction);
   int rt = RT(instruction);
   int rd = RD(instruction);
  //printf("rs: %x \nrt: %x \nrd: %x \ninstruction: %d \n", rs, rt, rd, instruction);

  registers[rd] = (unsigned int)((int)registers[rs] + (int)registers[rt]);
}

void addu(unsigned int instruction){
  unsigned int rs = RS(instruction);
  unsigned int rt = RT(instruction);
  unsigned int rd = RD(instruction);
  registers[rd] = registers[rs] + registers[rt];
}

void sub(unsigned int instruction){
   int rs = RS(instruction);
   int rt = RT(instruction);
   int rd = RD(instruction);

   registers[rd] = (unsigned int)((int)registers[rs] - (int)registers[rt]);
}

void subu(unsigned int instruction){
  unsigned int rs = RS(instruction);
  unsigned int rt = RT(instruction);
  unsigned int rd = RD(instruction);

  registers[rd] = registers[rs] - registers[rt];

}

void mult(unsigned int instruction){
  long int rs = RS(instruction);
  long int rt = RT(instruction);

  long x =(long)(registers[rs]) * (long)(registers[rt]);
  LO = (unsigned int)x;
  HI = (unsigned int)(x>>32);
}

void multu(unsigned int instruction){
  unsigned long int rs = RS(instruction);
  unsigned long int rt = RT(instruction);

  unsigned long x =(unsigned long)(registers[rs]) * (unsigned long)(registers[rt]);
  LO = (unsigned int) x;
  HI = (unsigned int)(x>>32);

}

void div(unsigned int instruction){
  int rs = RS(instruction);
  int rt = RT(instruction);

  int x = (int)(registers[rs]) / (int)(registers[rt]);
  int y = (int)(registers[rs]) % (int)(registers[rt]);
  LO = (unsigned int)x;
  HI = (unsigned int)y;
}

void divu(unsigned int instruction){
  unsigned int rs = RS(instruction);
  unsigned int rt = RT(instruction);

  unsigned int x = (unsigned int)(registers[rs]) / (unsigned int)(registers[rt]);
  unsigned int y = (registers[rs]) % (registers[rt]);
  LO = x;
  HI = y;
}

void and(unsigned int instruction){
  unsigned int rs = RS(instruction);
  unsigned int rt = RT(instruction);
  unsigned int rd = RD(instruction);
  registers[rd] = registers[rt] & registers[rs];
}

void nor(unsigned int instruction){
  unsigned int rs = RS(instruction);
  unsigned int rt = RT(instruction);
  unsigned int rd = RD(instruction);
  registers[rd] = ~(registers[rs] | registers[rt]);
}

void or(unsigned int instruction){
  unsigned int rs = RS(instruction);
  unsigned int rt = RT(instruction);
  unsigned int rd = RD(instruction);

  registers[rd] = registers[rs] | registers[rt];
}

void xor(unsigned int instruction){
  unsigned int rs = RS(instruction);
  unsigned int rt = RT(instruction);
  unsigned int rd = RD(instruction);

  registers[rd] = registers[rs] ^ registers[rt];

}

void sll(unsigned int instruction){
  unsigned int rt = RT(instruction);
  unsigned int rd = RD(instruction);

  registers[rd] = registers[rt]<<(SHAMT(instruction));

}

void sllv(unsigned int instruction){
  unsigned int rs = RS(instruction);
  unsigned int rt = RT(instruction);
  unsigned int rd = RD(instruction);

  registers[rd] = registers[rs]<<(registers[rt]);
}

void srl(unsigned int instruction){
  unsigned int rt = RT(instruction);
  unsigned int rd = RD(instruction);

  registers[rd] = registers[rt]>>(SHAMT(instruction));
}

void sra(unsigned int instruction){
  unsigned int rs = RS(instruction);
  unsigned int rt = RT(instruction);
  unsigned int rd = RD(instruction);

  unsigned int shamt = SHAMT(instruction);
  unsigned int benmask = ((1 << shamt) - 1) << (32 - shamt);

  if ((registers[rt] >> 31) & 0x1){
    registers[rd] = registers[rt] >> shamt;
    registers[rd] = registers[rd] | benmask;

  }else{
    registers[rd]=registers[rt] >> shamt;
  }

}

void srlv(unsigned int instruction){
  unsigned int rs = RS(instruction);
  unsigned int rt = RT(instruction);
  unsigned int rd = RD(instruction);

  registers[rd] = registers[rs]>>(registers[rt]);
}

void srav(unsigned int instruction){
  unsigned int rs = RS(instruction);
  unsigned int rt = RT(instruction);
  unsigned int rd = RD(instruction);
  unsigned int benmask = ((1 << registers[rt]) - 1) << (32 - registers[rt]);
  if ((registers[rs] >> 31) & 0x1){
    registers[rd] = registers[rs]>>(registers[rt]);
    registers[rd] = registers[rd] | benmask;

  } else {
    registers[rd] = registers[rt]>>(registers[rt]);
  }


}

void slt(unsigned int instruction){
   int rs = (int)RS(instruction);
   int rt = (int)RT(instruction);
   int rd = (int)RD(instruction);

  if((int)registers[rs] < (int)registers[rt]){
    registers[rd] = 1;
  } else {
    registers[rd] = 0;
  }
}

void sltu(unsigned int instruction){
  unsigned int rs = RS(instruction);
  unsigned int rt = RT(instruction);
  unsigned int rd = RD(instruction);

  if(registers[rs] < registers[rt]){
    registers[rd] = 1;
  } else {
    registers[rd] = 0;
  }
}

void jr(unsigned int instruction){
  unsigned int rs = RS(instruction);
  npc = registers[rs];
}

void jalr(unsigned int instruction){
  unsigned int rs = RS(instruction);
  registers[31] = pc + 4;
  npc = registers[rs];

}

void mfhi(unsigned int instruction){
  unsigned int rd = RD(instruction);

  registers[rd] = HI;

}

void mflo(unsigned int instruction){
  unsigned int rd = RD(instruction);

  registers[rd] = LO;

}
// int main(){
//   add(438969253);
//   printf("\n");
// }
