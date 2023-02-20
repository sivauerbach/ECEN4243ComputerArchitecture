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
int LB (int Rd, int Rs1, int Imm){ //mask 
  int32_t address = CURRENT_STATE.REGS[Rs1] + SIGNEXT(Imm, 12);
  int32_t value = mem_read_32(address);
  int8_t cur = value;
  NEXT_STATE.REGS[Rd] = cur;
  printf("DEBUG: Rd=%u, UpImm=%u, new val=%u, address=%u\n", Rd, Imm, value, address); 
}
int LH (int Rd, int Rs1, int Imm){
  int32_t address = CURRENT_STATE.REGS[Rs1] + SIGNEXT(Imm, 12);
  int32_t value = mem_read_32(address);
  int16_t cur = value;
  NEXT_STATE.REGS[Rd] = cur;
  printf("DEBUG: Rd=%u, UpImm=%u, new val=%u, address=%u\n", Rd, Imm, value, address); 
}
/*PASSED ALL TESTS*/
int LW (int Rd, int Rs1, int Imm){
  int32_t address = CURRENT_STATE.REGS[Rs1] + SIGNEXT(Imm, 12);
  int32_t value = mem_read_32(address);
  NEXT_STATE.REGS[Rd] = value;
  printf("DEBUG: Rd=%u, UpImm=%u, new val=%u, address=%u\n", Rd, Imm, value, address);
  return 0;
}


int LBU (int Rd, int Rs1, int Imm){
  int32_t address = CURRENT_STATE.REGS[Rs1] + SIGNEXT(Imm, 12);
  uint32_t value = mem_read_32(address);
  uint8_t cur = value;
  NEXT_STATE.REGS[Rd] = cur;
  printf("DEBUG: Rd=%u, UpImm=%u, new val=%u, address=%u\n", Rd, Imm, value, address); 
}
int LHU (int Rd, int Rs1, int Imm){
  int32_t address = CURRENT_STATE.REGS[Rs1] + SIGNEXT(Imm, 12);
  uint32_t value = mem_read_32(address);
  uint16_t cur = value;
  NEXT_STATE.REGS[Rd] = cur;
  printf("DEBUG: Rd=%u, UpImm=%u, new val=%u, address=%u\n", Rd, Imm, value, address); 
}

/* PASSED ALL TESTS */
int SLLI (int Rd, int Rs1, int Imm){
  uint32_t bottom_imm = ((Imm << 7) >> 7);
  int cur = CURRENT_STATE.REGS[Rs1] << bottom_imm; 
  NEXT_STATE.REGS[Rd] =   cur;
  printf("imm= %x, bottom imm= %x\n", Imm, bottom_imm);
  return 0;
}

/* PASSED ALL TESTS */
int SLTI (int Rd, int Rs1, int Imm){
  int Rs1_value = CURRENT_STATE.REGS[Rs1];
  int Imm_signed = SIGNEXT(Imm, 12);
  int cur = (Rs1_value < Imm_signed);
  NEXT_STATE.REGS[Rd] = cur;
  return 0;
}
/* PASSED ALL TESTS */
int SLTIU (int Rd, int Rs1, int Imm){
  uint32_t unsigned_imm = (uint32_t)SIGNEXT(Imm, 12);
  uint32_t Rs1_val = CURRENT_STATE.REGS[Rs1];
  int cur = ( Rs1_val< unsigned_imm);
  NEXT_STATE.REGS[Rd] = cur;
  return 0;
}
/* PASSED ALL TESTS */
int XORI (int Rd, int Rs1, int Imm){
  int cur = (CURRENT_STATE.REGS[Rs1] ^ SIGNEXT(Imm, 12));
  NEXT_STATE.REGS[Rd] = cur;
  return 0;
}

/* PASSED ALL TESTS */
int SRLI (int Rd, int Rs1, int Imm){
  //  CURRENT_STATE.REGS hold unsigned int_32, >> performs logic shift 
  uint32_t bottom_imm = ((Imm << 7) >> 7);
  int cur = CURRENT_STATE.REGS[Rs1] >> bottom_imm; 
  NEXT_STATE.REGS[Rd] =   cur;
  return 0;
}

/* PASSED ALL TESTS */
int SRAI (int Rd, int Rs1, int Imm){
  int rs1 = CURRENT_STATE.REGS[Rs1];
  uint32_t bottom_imm = ((Imm << 7) >> 7);
  // performs signed (arithmetic) shift on int types
  NEXT_STATE.REGS[Rd] = rs1 >> bottom_imm;
  return 0;
}

/* PASSED ALL TESTS */
int ORI (int Rd, int Rs1, int Imm){
  int cur = (CURRENT_STATE.REGS[Rs1] | SIGNEXT(Imm, 12));
  NEXT_STATE.REGS[Rd] = cur;
  return 0;
}
/* PASSED ALL TESTS */
int ANDI (int Rd, int Rs1, int Imm){
  int cur = (CURRENT_STATE.REGS[Rs1] & SIGNEXT(Imm, 12));
  NEXT_STATE.REGS[Rd] = cur;
  return 0;
}
int JALR (int Rd, int Rs1, int Imm){
  Imm = Imm << 1;
  NEXT_STATE.PC = (CURRENT_STATE.REGS[Rs1] -4) + (SIGNEXT(Imm,12));
  NEXT_STATE.REGS[Rd] = CURRENT_STATE.PC + 4;
}

// S Instruction

int SB (int Rs1, int Rs2, int Imm){
  int32_t value = CURRENT_STATE.REGS[Rs2]; //<< 16) >>16);
  int32_t address = CURRENT_STATE.REGS[Rs1] + SIGNEXT(Imm, 12);
  mem_write_32(address, value);
  printf("DEBUG: Rs1=%d, Rs2= %d, Imm=%d, new val=%d", Rs1, Rs2, Imm, value);
}
int SH (int Rs1, int Rs2, int Imm){
  int32_t value = CURRENT_STATE.REGS[Rs2]; //<< 16) >>16);
  int32_t address = CURRENT_STATE.REGS[Rs1] + SIGNEXT(Imm, 12);
  mem_write_32(address, value);
  printf("DEBUG: Rs1=%d, Rs2= %d, Imm=%d, new val=%d", Rs1, Rs2, Imm, value);
}
// signed/unsigned?
int SW (int Rs1, int Rs2, int Imm){
  int32_t value = CURRENT_STATE.REGS[Rs2]; //<< 16) >>16);
  int32_t address = CURRENT_STATE.REGS[Rs1] + SIGNEXT(Imm, 12);
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
  return 0;
}


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
