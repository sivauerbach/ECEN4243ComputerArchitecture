/***************************************************************/
/*                                                             */
/*   RISC-V RV32 Instruction Level Simulator                   */
/*                                                             */
/*   ECEN 4243                                                 */
/*   Oklahoma State University                                 */
/*                                                             */
/***************************************************************/

#ifndef _SIM_ISA_H_
#define _SIM_ISA_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"


// MACRO: Check sign bit (sb) of (v) to see if negative
//   if no, just give number
//   if yes, sign extend (e.g., 0x80_0000 -> 0xFF80_0000)
#define SIGNEXT(v, sb) ((v) | ( (0 < ((v) & (1 << (sb-1)))) ? ~((1 << (sb-1))-1) : 0))


// I Instructions

/* PASSED ALL TESTS */
int ADDI (int Rd, int Rs1, int Imm) {
  int cur = 0;
  cur = CURRENT_STATE.REGS[Rs1] + SIGNEXT(Imm,12);
  NEXT_STATE.REGS[Rd] = cur;
  return 0;
}
int LB (int Rs1, int Rs2, int Imm){}
int LH (int Rs1, int Rs2, int Imm){}
int LW (int Rs1, int Rs2, int Imm){}
int LBU (int Rs1, int Rs2, int Imm){}
int LHU (int Rs1, int Rs2, int Imm){}
int SLLI (int Rs1, int Rs2, int Imm){}
int SLTI (int Rs1, int Rs2, int Imm){}
int SLTIU (int Rs1, int Rs2, int Imm){}
int XORI (int Rs1, int Rs2, int Imm){}
int SRLI (int Rs1, int Rs2, int Imm){}
int SRAI (int Rs1, int Rs2, int Imm){}
int ORI (int Rs1, int Rs2, int Imm){}
int ANDI (int Rs1, int Rs2, int Imm){}
int JALR (int Rs1, int Rs2, int Imm){}

// S Instruction

int SB (int Rs1, int Rs2, int Imm){}
int SH (int Rs1, int Rs2, int Imm){}
// signed/unsigned?
int SW (int Rs1, int Rs2, int Imm){
  uint32_t value = ( (CURRENT_STATE.REGS[Rs2] << 16) >>16);
  uint32_t address = Rs1 + Imm*4;
  mem_write_32(address, value);
  printf("DEBUG: Rs1=%d, Rs2= %d, Imm=%d, new val=%d", Rs1, Rs2, Imm, value);
  return 0;
}

// U Instruction

/* PASSED ALL TESTS */
int AUIPC (int Rd, int UpImm) {
  int cur = 0;
  UpImm = UpImm << 12;
  cur = UpImm + CURRENT_STATE.PC;
  NEXT_STATE.REGS[Rd] = cur;
  printf("DEBUG: Rd=%d, UpImm=%d, new val=%d", Rd, UpImm, cur);
  return 0;
}

/* NOT PASSED ALL TESTS, need to impl SRAI first*/
int LUI (int Rd, int UpImm) {
  UpImm = UpImm << 12;
  NEXT_STATE.REGS[Rd] = UpImm;

  printf("DEBUG: Rd=%d, UpImm=%d\n\n", Rd, UpImm);
  return 0;
}

// R instructions - COMPLETED

/* PASSED ALL TESTS */
int ADD (int Rd, int Rs1, int Rs2, int Funct3) {
  int cur = 0;
  cur = CURRENT_STATE.REGS[Rs1] + CURRENT_STATE.REGS[Rs2];
  NEXT_STATE.REGS[Rd] = cur;
  return 0;
}

/* PASSED ALL TESTS */
int SUB (int Rd, int Rs1, int Rs2) {
  int cur = 0;
  cur = CURRENT_STATE.REGS[Rs1] - CURRENT_STATE.REGS[Rs2];
  NEXT_STATE.REGS[Rd] = cur;
  return 0;
}
/* PASSES ALL TESTS */
int SLT (int Rd, int Rs1, int Rs2) {
  int Rs1_value = CURRENT_STATE.REGS[Rs1];
  int Rs2_value = CURRENT_STATE.REGS[Rs2];
  int cur = (Rs1_value < Rs2_value);
  NEXT_STATE.REGS[Rd] = cur;
  return 0;
}
/* PASSES ALL TESTS */
int SLTU (int Rd, int Rs1, int Rs2) {  
  int cur = (CURRENT_STATE.REGS[Rs2] > CURRENT_STATE.REGS[Rs1]);
  NEXT_STATE.REGS[Rd] = cur;
  return 0;
}

/* PASSES ALL TESTS */
int XOR (int Rd, int Rs1, int Rs2) {
  int cur = (CURRENT_STATE.REGS[Rs2] ^ CURRENT_STATE.REGS[Rs1]);
  NEXT_STATE.REGS[Rd] = cur;
  return 0;
}
/* PASSES ALL TESTS */
int OR (int Rd, int Rs1, int Rs2) {
  int cur = (CURRENT_STATE.REGS[Rs2] | CURRENT_STATE.REGS[Rs1]);
  NEXT_STATE.REGS[Rd] = cur;
  return 0;
}
/* PASSES ALL TESTS */
int AND (int Rd, int Rs1, int Rs2) {
  int cur = (CURRENT_STATE.REGS[Rs2] & CURRENT_STATE.REGS[Rs1]);
  NEXT_STATE.REGS[Rd] = cur;
  return 0;
}

/* PASSES ALL TESTS */
int SRL (int Rd, int Rs1, int Rs2) {
  //  CURRENT_STATE.REGS hold unsigned int_32, >> performs logic shift 
  int cur = CURRENT_STATE.REGS[Rs1] >> CURRENT_STATE.REGS[Rs2]; 
  NEXT_STATE.REGS[Rd] =   cur;
  return 0;
}

/* PASSES ALL TESTS */
int SRA (int Rd, int Rs1, int Rs2) {
  int rs1 = CURRENT_STATE.REGS[Rs1];
  int rs2 = CURRENT_STATE.REGS[Rs2];
  // performs signed (arithmetic) shift on int types
  NEXT_STATE.REGS[Rd] = rs1 >> rs2;
  return 0;
}

/* PASSES ALL TESTS */
int SLL (int Rd, int Rs1, int Rs2) {
  int cur = CURRENT_STATE.REGS[Rs1] << CURRENT_STATE.REGS[Rs2]; 
  NEXT_STATE.REGS[Rd] =   cur;
  return 0;}


// B instructions - COMPLETED

/* PASSED ALL TESTS */
int BNE (int Rs1, int Rs2, int Imm) {
  int cur = 0;
  Imm = Imm << 1;
  if (CURRENT_STATE.REGS[Rs1] != CURRENT_STATE.REGS[Rs2])
    NEXT_STATE.PC = (CURRENT_STATE.PC + 4) + (SIGNEXT(Imm,13));
  return 0;
}

/* PASSES ALL TESTS */
int BEQ (int Rs1, int Rs2, int Imm) {
  int cur = 0;
  Imm = Imm << 1;
  if (CURRENT_STATE.REGS[Rs1] == CURRENT_STATE.REGS[Rs2])
      NEXT_STATE.PC = (CURRENT_STATE.PC + 4) + (SIGNEXT(Imm,13));
  return 0;
}

/* PASSES ALL TESTS */
int BLT (int Rs1, int Rs2, int Imm){
  int cur = 0;
  Imm = Imm << 1;
  if (((int)CURRENT_STATE.REGS[Rs1]) < ((int)CURRENT_STATE.REGS[Rs2]))
      NEXT_STATE.PC = (CURRENT_STATE.PC + 4) + (SIGNEXT(Imm,13));
  return 0;
}

/* PASSES ALL TESTS */
int BGE (int Rs1, int Rs2, int Imm){
  int cur = 0;
  Imm = Imm << 1;
  if (((int)CURRENT_STATE.REGS[Rs1]) >= ((int)CURRENT_STATE.REGS[Rs2]))
      NEXT_STATE.PC = (CURRENT_STATE.PC + 4) + (SIGNEXT(Imm,13));
  return 0;
}


/* PASSES ALL TESTS */
int BLTU (int Rs1, int Rs2, int Imm){
  int cur = 0;
  Imm = Imm << 1;
  if (CURRENT_STATE.REGS[Rs1] < CURRENT_STATE.REGS[Rs2])
      NEXT_STATE.PC = (CURRENT_STATE.PC + 4) + (SIGNEXT(Imm,13));
  return 0;
}

/* PASSES ALL TESTS */
int BGEU (int Rs1, int Rs2, int Imm){
  int cur = 0;
  Imm = Imm << 1;
  if (CURRENT_STATE.REGS[Rs1] >= CURRENT_STATE.REGS[Rs2])
      NEXT_STATE.PC = (CURRENT_STATE.PC + 4) + (SIGNEXT(Imm,13));
  return 0;
}

// J instruction - COMPLETED
/* PASSES ALL TESTS */
int JAL (int Rd, int Imm){
  Imm = Imm << 1;
  NEXT_STATE.PC = (CURRENT_STATE.PC -4) + (SIGNEXT(Imm,21));
  NEXT_STATE.REGS[Rd] = CURRENT_STATE.PC + 4;
}

int ECALL (char* i_){return 0;}

#endif
