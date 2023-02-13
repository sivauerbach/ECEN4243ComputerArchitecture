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

//
// MACRO: Check sign bit (sb) of (v) to see if negative
//   if no, just give number
//   if yes, sign extend (e.g., 0x80_0000 -> 0xFF80_0000)
//
#define SIGNEXT(v, sb) ((v) | (((v) & (1 << (sb))) ? ~((1 << (sb))-1) : 0))

//Tested 
int ADD (int Rd, int Rs1, int Rs2, int Funct3) {

  int cur = 0;
  cur = CURRENT_STATE.REGS[Rs1] + CURRENT_STATE.REGS[Rs2];
  NEXT_STATE.REGS[Rd] = cur;
  return 0;

}
//Tested
int ADDI (int Rd, int Rs1, int Imm, int Funct3) {
  int cur = 0;
  cur = CURRENT_STATE.REGS[Rs1] + SIGNEXT(Imm,12);
  NEXT_STATE.REGS[Rd] = cur;
  return 0;
}

int BNE (int Rs1, int Rs2, int Imm, int Funct3) {

  int cur = 0;
  Imm = Imm << 1;
  if (CURRENT_STATE.REGS[Rs1] != CURRENT_STATE.REGS[Rs2])
    NEXT_STATE.PC = (CURRENT_STATE.PC + 4) + (SIGNEXT(Imm,13));
  return 0;

}

// I Instructions
int LB (char* i_) ;
int LH (char* i_);
int LW (char* i_);
int LBU (char* i_);
int LHU (char* i_);
//zimm? 
int SLLI (int Rd, int Rs1, int zimm, int Funct3){
  int cur = 0;
  cur = (CURRENT_STATE.REGS[Rs1] << zimm);
  NEXT_STATE.REGS[Rd] = cur;
  return 0;
}
int SLTI (int Rd, int Rs1, int Imm, int Funct3){
  int cur = 0;
  cur = (CURRENT_STATE.REGS[Rs1] < SIGNEXT(Imm, 12));
  NEXT_STATE.REGS[Rd] = cur;
  return 0;
}

//how should this be different then SLTI
int SLTIU (int Rd, int Rs1, int Imm, int Funct3){
  int cur = 0;
  cur = (CURRENT_STATE.REGS[Rs1] < SIGNEXT(Imm, 12));
  NEXT_STATE.REGS[Rd] = cur;
  return 0;
}

int XORI (int Rd, int Rs1, int Imm, int Funct3){
  int cur = 0;
  cur = (CURRENT_STATE.REGS[Rs1] ^ SIGNEXT(Imm,12));
  NEXT_STATE.REGS[Rd] = cur;
  return 0;
};

//zimm?
int SRLI (int Rd, int Rs1, int zimm, int Funct3){
  int cur = 0;
  cur = (CURRENT_STATE.REGS[Rs1] >> zimm);
  NEXT_STATE.REGS[Rd] = cur;
  return 0;
}

//zimm? also >>> vs >>? 
int SRAI (int Rd, int Rs1, int zimm, int Funct3){
  int cur = 0;
  cur = (CURRENT_STATE.REGS[Rs1] >> zimm);
  NEXT_STATE.REGS[Rd] = cur;
  return 0;
}

int ORI (int Rd, int Rs1, int Imm, int Funct3){
  int cur = 0;
  cur = (CURRENT_STATE.REGS[Rs1] | SIGNEXT(Imm, 12));
  NEXT_STATE.REGS[Rd] = cur;
  return 0;
}

int ANDI (char* i_){
  int cur = 0;
  cur = (CURRENT_STATE.REGS[Rs2] & SIGNEXT(Imm, 12));
  NEXT_STATE.REGS[Rd] = cur;
  return 0;
}

// U Instruction
int AUIPC (int Rd, int UpImm) {
  int cur = 0;
  UpImm = UpImm << 12;
  cur = UpImm + CURRENT_STATE.PC;
  NEXT_STATE.REGS[Rd] = cur;

  printf("DEBUG: Rd=%d, UpImm=%d, new val=%d", Rd, UpImm, cur);
  return 0;
}

int LUI (int Rd, int UpImm) {
  UpImm = UpImm << 12;
  NEXT_STATE.REGS[Rd] = UpImm;

  printf("DEBUG: Rd=%d, UpImm=%d\n\n", Rd, UpImm);
  return 0;
}

// S Instruction
int SB (char* i_);
int SH (char* i_);
int SW (char* i_);

// R instruction
int SUB (int Rd, int Rs1, int Rs2) {
  int cur = 0;
  cur = CURRENT_STATE.REGS[Rs1] - CURRENT_STATE.REGS[Rs2];
  NEXT_STATE.REGS[Rd] = cur;
  return 0;
}

int SLL (int Rd, int Rs1, int Rs2) {
///???
}

// How to impl. unsigned vs signed??
int SLT (int Rd, int Rs1, int Rs2) {
  int cur = (CURRENT_STATE.REGS[Rs2] > CURRENT_STATE.REGS[Rs1]);
  NEXT_STATE.REGS[Rd] = cur;
  return 0;
}

int SLTU (int Rd, int Rs1, int Rs2) {}

//Tested 
int XOR (int Rd, int Rs1, int Rs2) {
  int cur = (CURRENT_STATE.REGS[Rs2] ^ CURRENT_STATE.REGS[Rs1]);
  NEXT_STATE.REGS[Rd] = cur;
  return 0;
}
int SRL (int Rd, int Rs1, int Rs2) {}
int SRA (int Rd, int Rs1, int Rs2) {}

//Tested 
int OR (int Rd, int Rs1, int Rs2) {
  int cur = (CURRENT_STATE.REGS[Rs2] | CURRENT_STATE.REGS[Rs1]);
  NEXT_STATE.REGS[Rd] = cur;
  return 0;
}

//Tested 
int AND (int Rd, int Rs1, int Rs2) {
  int cur = (CURRENT_STATE.REGS[Rs2] & CURRENT_STATE.REGS[Rs1]);
  NEXT_STATE.REGS[Rd] = cur;
  return 0;
}

// B instructions
int BEQ (char* i_);
int BLT (char* i_);
int BGE (char* i_);
int BLTU (char* i_);
int BGEU (char* i_);

// I instruction
int JALR (char* i_);

// J instruction
int JAL (char* i_);

int ECALL (char* i_){return 0;}

#endif
