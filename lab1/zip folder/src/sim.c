/***************************************************************/
/*                                                             */
/*   RISC-V RV32 Instruction Level Simulator                   */
/*                                                             */
/*   ECEN 4243                                                 */
/*   Oklahoma State University                                 */
/*                                                             */
/***************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"
#include "isa.h"

char *byte_to_binary(int x) {

  static char b[9];
  b[0] = '\0';

  int z;
  for (z = 128; z > 0; z >>= 1) {
    strcat(b, ((x & z) == z) ? "1" : "0");
  }

  return b;
}

char *byte_to_binary32(int x) {

  static char b[33];
  b[0] = '\0';

  unsigned int z;
  for (z = 2147483648; z > 0; z >>= 1) {
    strcat(b, ((x & z) == z) ? "1" : "0");
  }

  return b;
}

int bchar_to_int(char* rsa) {

  int i = 0;
  int result = 0;
  int t = 0;
  while(rsa[i] != '\0')i++;
  while(i>0)
    {
      --i;
      // printf("%d\n", (rsa[i]-'0')<<t);
      result += (rsa[i] - '0')<<t;
      t++;
    }
  return result;
}

int r_process(char* i_) {

  char d_opcode[8];
  d_opcode[0] = i_[31-6]; 
  d_opcode[1] = i_[31-5]; 
  d_opcode[2] = i_[31-4]; 
  d_opcode[3] = i_[31-3];
  d_opcode[4] = i_[31-2]; 
  d_opcode[5] = i_[31-1]; 
  d_opcode[6] = i_[31-0];
  d_opcode[7] = '\0';
  char rs1[6]; rs1[5] = '\0';		   
  char rs2[6]; rs2[5] = '\0';
  char rd[6]; rd[5] = '\0';
  char funct3[4]; funct3[3] = '\0';
  //Declaring funct7
  char funct7[8]; funct7[7] = '\0';
  for(int i = 0; i < 7; i++) {
    funct7[i] = i_[31-31+i];
  }
  for(int i = 0; i < 5; i++) {
    rs1[i] = i_[31-19+i];
    rs2[i] = i_[31-24+i];            
    rd[i] = i_[31-11+i];
  }
  for(int i = 0; i < 3; i++) {
    funct3[i] = i_[31-14+i];
    //funct3[i] = i_[12+i];
  }
  int Rs1 = bchar_to_int(rs1);
  int Rs2 = bchar_to_int(rs2);		   
  int Rd = bchar_to_int(rd);
  int Funct3 = bchar_to_int(funct3);
  int Funct7 = bchar_to_int(funct7); 

  printf ("Opcode = %s\n Rs1 = %d\n Rs2 = %d\n Rd = %d\n Funct3 = %d\n Funct7 = %d\n",
	  d_opcode, Rs1, Rs2, Rd, Funct3, Funct7);
  printf("\n");

  /* Example - use and replicate */
  if(!strcmp(d_opcode,"0110011")) {
    printf("DEBUG: Funct3= %d, funct7= %s\n", Funct3, funct7);
    switch(Funct3) {
      case 0: 
            if (Funct7 == 0) {
              printf("--- This is an ADD instruction. \n\n");
              ADD(Rd, Rs1, Rs2, Funct3);
              return 0;
            }
            if (Funct7 == 32) {
              printf("--- This is an SUB instruction. \n\n");
              SUB(Rd, Rs1, Rs2);
              return 0;
            }
            break;
      case 1:
              printf("--- This is an SLL instruction. \n\n");
              SLL(Rd, Rs1, Rs2);
              return 0;
              break;
      case 2:
              printf("--- This is an SLT instruction. \n\n");
              SLT(Rd, Rs1, Rs2);
              return 0;
      case 3:
              printf("--- This is an SLTU instruction. \n\n");
              SLTU(Rd, Rs1, Rs2);
              return 0;
      case 4:
              printf("--- This is an XOR instruction. \n\n");
              XOR(Rd, Rs1, Rs2);
              return 0;
      case 5:
              if (Funct7 == 0) {
                printf("--- This is an SRL instruction. \n\n");
                SRL(Rd, Rs1, Rs2);
                return 0;
              }
              if (Funct7 == 32) {
                printf("--- This is an SRA instruction. \n\n");
                SRA(Rd, Rs1, Rs2);
                return 0;
              }
              break;
      case 6:
              printf("--- This is an OR instruction. \n\n");
              OR(Rd, Rs1, Rs2);
              return 0;
      case 7:
              printf("--- This is an AND instruction. \n\n");
              AND(Rd, Rs1, Rs2);
              return 0;
      default: 
              printf("--- ERROR: Invalid Funct3 Value. \n");
              return -1;
    }
    
    

  }

  /* Add other data instructions here */ 

  return 1;	
}

int i_process(char* i_) {

  char d_opcode[8];
  d_opcode[0] = i_[31-6]; 
  d_opcode[1] = i_[31-5]; 
  d_opcode[2] = i_[31-4]; 
  d_opcode[3] = i_[31-3];
  d_opcode[4] = i_[31-2]; 
  d_opcode[5] = i_[31-1]; 
  d_opcode[6] = i_[31-0]; 
  d_opcode[7] = '\0';
  char rs1[6]; rs1[5] = '\0';		   
  char rd[6]; rd[5] = '\0';
  char funct3[4]; funct3[3] = '\0';
  char imm[13]; imm[12] = '\0';
  for(int i = 0; i < 5; i++) {
    rs1[i] = i_[31-19+i];
    rd[i] = i_[31-11+i];
  }
  for(int i = 0; i < 12; i++) {
    imm[i] = i_[31-31+i];
  }
  for(int i = 0; i < 3; i++) {
    funct3[i] = i_[31-14+i];
  }
  //Declaring funct7
  char funct7[8]; funct7[7] = '\0';
  for(int i = 0; i < 7; i++) {
    funct7[i] = i_[31-31+i];
  }
  int Rs1 = bchar_to_int(rs1);
  int Rd = bchar_to_int(rd);
  int Funct3 = bchar_to_int(funct3);
  int Funct7 = bchar_to_int(funct7);
  int Imm = bchar_to_int(imm);
  printf ("Opcode = %s\n Rs1 = %d\n Imm = %d\n Rd = %d\n Funct3 = %d\n",
	  d_opcode, Rs1, Imm, Rd, Funct3);
  printf("\n");

  /* Add other imm instructions here */ 

  if(!strcmp(d_opcode,"0000011")) {
    switch(Funct3) {
      case 0: 
            printf("--- This is an LB instruction. \n\n");
            LB(Rd, Rs1, Imm);
            return 0;
      case 1:
            printf("--- This is an LH instruction. \n\n");
            LH(Rd, Rs1, Imm);
            return 0;
      case 2:
            printf("--- This is an LW instruction. \n\n");
            LW(Rd, Rs1, Imm);
            return 0;
      case 4:
            printf("--- This is an LBU instruction. \n\n");
            LBU(Rd, Rs1, Imm);
            return 0;
      case 5:
            printf("--- This is an LHU instruction. \n\n");
            LHU(Rd, Rs1, Imm);
            return 0;
      default: 
              printf("--- ERROR: Invalid Funct3 Value. \n");
              return -1;
    }
  }
  /* This is an Immediate Instruciton */
  if(!strcmp(d_opcode,"0010011")) {
    switch(Funct3) {
      case 0: 
            printf("--- This is an ADDI instruction. \n\n");
            ADDI(Rd, Rs1, Imm);
            return 0;
      case 1:
            printf("--- This is an SLLI instruction. \n\n");
            SLLI(Rd, Rs1, Imm);
            return 0;
      case 2:
            printf("--- This is an SLTI instruction. \n\n");
            SLTI(Rd, Rs1, Imm);
            return 0;
      case 3:
            printf("--- This is an SLTIU instruction. \n\n");
            SLTIU(Rd, Rs1, Imm);
            return 0;
      case 4:
            printf("--- This is an XORI instruction. \n\n");
            XORI(Rd, Rs1, Imm);
            return 0;
      case 5:
              if (Funct7 == 0) {
                printf("--- This is an SRLI instruction. \n\n");
                SRLI(Rd, Rs1, Imm);
                return 0;
              }
              if (Funct7 == 32) {
                printf("--- This is an SRAI instruction. \n\n");
                SRAI(Rd, Rs1, Imm);
                return 0;
              }
      case 6:
            printf("--- This is an ORI instruction. \n\n");
            ORI(Rd, Rs1, Imm);
            return 0;
      case 7:
              printf("--- This is an ANDI instruction. \n\n");
              ANDI(Rd, Rs1, Imm);
              return 0;
      default: 
              printf("--- ERROR: Invalid Funct3 Value. \n");
              return -1;
    }
  }	 
  if(!strcmp(d_opcode,"1100111")) {
    printf("--- This is an JALR instruction. \n\n");
    JALR(Rd, Rs1, Imm);
  }

  return 1;	
}

int b_process(char* i_) {
  
  /* This function execute branch instruction */

  char d_opcode[8];
  d_opcode[0] = i_[31-6]; 
  d_opcode[1] = i_[31-5]; 
  d_opcode[2] = i_[31-4]; 
  d_opcode[3] = i_[31-3];
  d_opcode[4] = i_[31-2]; 
  d_opcode[5] = i_[31-1]; 
  d_opcode[6] = i_[31-0]; 
  d_opcode[7] = '\0';
  char rs1[6]; rs1[5] = '\0';
  char rs2[6]; rs2[5] = '\0';
  char funct3[4]; funct3[3] = '\0';
  char imm[13]; 
  for(int i = 0; i < 5; i++) {
    rs1[i] = i_[31-19+i];
    rs2[i] = i_[31-24+i];                
  }
  // Old-fashioned method but works :)
  imm[0] = i_[31-31]; 
  imm[1] = i_[31-7]; 
  imm[2] = i_[31-30]; 
  imm[3] = i_[31-29];
  imm[4] = i_[31-28]; 
  imm[5] = i_[31-27]; 
  imm[6] = i_[31-26];
  imm[7] = i_[31-25];
  imm[8] = i_[31-11];
  imm[9] = i_[31-10];
  imm[10] = i_[31-9];
  imm[11] = i_[31-8];
  imm[12] = '\0';  

  for(int i = 0; i < 3; i++) {
    funct3[i] = i_[31-14+i];
  }
  int Rs1 = bchar_to_int(rs1);
  int Rs2 = bchar_to_int(rs2);  
  int Funct3 = bchar_to_int(funct3);
  int Imm = bchar_to_int(imm);
  printf ("Opcode = %s\n Rs1 = %d\n Rs2 = %d\n Imm = %d\n Funct3 = %d\n",
	  d_opcode, Rs1, Rs2, Imm, Funct3);
  printf("\n");    

  /* Add branch instructions here */

  /* This is an Add Immediate Instruciton */
  if(!strcmp(d_opcode,"1100011")) {

    switch(Funct3) {
      case 0:
              printf("--- This is an BEQ instruction. \n\n");
              BEQ(Rs1, Rs2, Imm);
              return 0;
              break;
      case 1:
              printf("--- This is an BNE instruction. \n\n");
              BNE(Rs1, Rs2, Imm);
              return 0;
      case 4:
              printf("--- This is an BLT instruction. \n\n");
              BLT(Rs1, Rs2, Imm);
              return 0;
      case 5:
              printf("--- This is an BGE instruction. \n\n");
              BGE(Rs1, Rs2, Imm);
              return 0;
      case 6:
              printf("--- This is an BLTU instruction. \n\n");
              BLTU(Rs1, Rs2, Imm);
              return 0;
      case 7:
              printf("--- This is an BGEU instruction. \n\n");
              BGEU(Rs1, Rs2, Imm);
              return 0;
      default:
              printf("--- ERROR: Invalid Funct3 Value. \n");
              return -1;
    }
  }	    

  return 1;

}

int s_process(char* i_) {

  /* This function execute S type instructions */

  char d_opcode[8];
  d_opcode[0] = i_[31-6]; 
  d_opcode[1] = i_[31-5]; 
  d_opcode[2] = i_[31-4]; 
  d_opcode[3] = i_[31-3];
  d_opcode[4] = i_[31-2]; 
  d_opcode[5] = i_[31-1]; 
  d_opcode[6] = i_[31-0]; 
  d_opcode[7] = '\0';
  char rs1[6]; rs1[5] = '\0';
  char rs2[6]; rs2[5] = '\0';		     
  char funct3[4]; funct3[3] = '\0';
  char imm[13]; 
  for(int i = 0; i < 5; i++) {
    rs1[i] = i_[31-19+i];
    rs2[i] = i_[31-24+i];                
  }
  // Old-fashioned method but works :)
  imm[0] = i_[31-31]; 
  imm[1] = i_[31-30]; 
  imm[2] = i_[31-29]; 
  imm[3] = i_[31-28];
  imm[4] = i_[31-27]; 
  imm[5] = i_[31-26]; 
  imm[6] = i_[31-25];
  imm[7] = i_[31-11];
  imm[8] = i_[31-10];
  imm[9] = i_[31-9];
  imm[10] = i_[31-8];
  imm[11] = i_[31-7];
  imm[12] = '\0';  

  for(int i = 0; i < 3; i++) {
    funct3[i] = i_[31-14+i];
  }
  int Rs1 = bchar_to_int(rs1);
  int Rs2 = bchar_to_int(rs2);  
  int Funct3 = bchar_to_int(funct3);
  int Imm = bchar_to_int(imm);
  printf ("Opcode = %s\n Rs1 = %d\n Rs2 = %d\n Imm = %d\n Funct3 = %d\n",
	  d_opcode, Rs1, Rs2, Imm, Funct3);
  printf("\n");    

  /* Add store instructions here */ 
  if(!strcmp(d_opcode,"0100011")) {
    switch(Funct3) {
      case 0:
              printf("--- This is an SB instruction. \n\n");
              SB(Rs1, Rs2, Imm);
              return 0;
              break;
      case 1:
              printf("--- This is an SH instruction. \n\n");
              SH(Rs1, Rs2, Imm);
              return 0;
      case 2:
              printf("--- This is an SW instruction. \n\n");
              SW(Rs1, Rs2, Imm);
              return 0;
      default:
              printf("--- ERROR: Invalid Funct3 Value. \n");
              return -1;
    }
  }
  return 1;

}

int j_process(char* i_) {

  /* This function execute Jump instructions */


  /* This function execute S type instructions */

  char d_opcode[8];
  d_opcode[0] = i_[31-6]; 
  d_opcode[1] = i_[31-5]; 
  d_opcode[2] = i_[31-4]; 
  d_opcode[3] = i_[31-3];
  d_opcode[4] = i_[31-2]; 
  d_opcode[5] = i_[31-1]; 
  d_opcode[6] = i_[31-0]; 
  d_opcode[7] = '\0';

  char rd[6]; rd[5] = '\0';
  for(int i = 0; i < 5; i++) {
    rd[i] = i_[31-11+i];
  }

  char imm[21];

  // Old-fashioned method but works :)
  imm[0] = i_[31-31]; //bit 20
  imm[1] = i_[31-19]; //bits 19:12
  imm[2] = i_[31-18]; 
  imm[3] = i_[31-17];
  imm[4] = i_[31-16]; 
  imm[5] = i_[31-15]; 
  imm[6] = i_[31-14];
  imm[7] = i_[31-13];
  imm[8] = i_[31-12];
  imm[9] = i_[31-20]; // bit 11
  imm[10] = i_[31-30]; //bit 10:1
  imm[11] = i_[31-29];
  imm[12] = i_[31-28];
  imm[13] = i_[31-27];
  imm[14] = i_[31-26];
  imm[15] = i_[31-25];
  imm[16] = i_[31-24];
  imm[17] = i_[31-23];
  imm[18] = i_[31-22];
  imm[19] = i_[31-21];
  imm[20] = '\0';  

  int Rd = bchar_to_int(rd);
  int Imm = bchar_to_int(imm);
  printf ("Opcode = %s\n Rd = %d\n Imm = %d\n",
	  d_opcode, Rd, Imm);
  printf("\n");    

  /* Add jump instructions here */ 
  if(!strcmp(d_opcode,"1101111")) {
    printf("--- This is an JAL instruction. \n\n");
    JAL(Rd, Imm);
  }

  //all jalr
  return 1;
}

int u_process(char* i_) {

  /* This function execute U type instructions */

  char u_opcode[8];
  u_opcode[0] = i_[31-6]; 
  u_opcode[1] = i_[31-5]; 
  u_opcode[2] = i_[31-4]; 
  u_opcode[3] = i_[31-3];
  u_opcode[4] = i_[31-2]; 
  u_opcode[5] = i_[31-1]; 
  u_opcode[6] = i_[31-0]; 
  u_opcode[7] = '\0';

  char rd[6]; rd[5] = '\0';
  for(int i = 0; i < 5; i++) {
    rd[i] = i_[31-11+i];
  }
  char imm[21]; imm[20] = '\0';

  imm[0] = i_[31-31]; 
  imm[1] = i_[31-30]; 
  imm[2] = i_[31-29];
  imm[3] = i_[31-28]; 
  imm[4] = i_[31-27]; 
  imm[5] = i_[31-26];
  imm[6] = i_[31-25];
  imm[7] = i_[31-24];
  imm[8] = i_[31-23];
  imm[9] = i_[31-22];
  imm[10] = i_[31-21];
  imm[11] = i_[31-20];
  imm[12] = i_[31-19];
  imm[13] = i_[31-18]; 
  imm[14] = i_[31-17]; 
  imm[15] = i_[31-16];
  imm[16] = i_[31-15];
  imm[17] = i_[31-14];
  imm[18] = i_[31-13];
  imm[19] = i_[31-12];
  imm[20] = '\0'; 


  int Rd = bchar_to_int(rd);
  int Imm = bchar_to_int(imm);
  printf ("Opcode = %s\n Rd = %d\n UpperImm = %d\n",
	  u_opcode, Rd, Imm);
  printf("\n"); 
  /* Add U instructions here */ 
  if(!strcmp(u_opcode,"0010111")) {
    printf("--- This is an AUIPC instruction. \n\n");
    AUIPC(Rd, Imm);
    return 0;
  }	

  if(!strcmp(u_opcode,"0110111")) {
    printf("--- This is an LUI instruction. \n\n");
    LUI(Rd, Imm);
    return 0;
  }	

  return 1;

}

int interruption_process(char* i_) {

  ECALL(i_);
  RUN_BIT = 0;
  return 0;

}

int decode_and_execute(char* i_) {

  /* 
     This function decode the instruction and update 
     CPU_State (NEXT_STATE)
  */

  if((i_[25] == '0') && (i_[26] == '0') &&
     (i_[27] == '1') && (i_[28] == '0') &&
     (i_[29] == '0') && (i_[30] == '1') && (i_[31] == '1')) {
    printf("- This is an Immediate Type Instruction. \n");
    i_process(i_);
  }
  if((i_[25] == '0') && (i_[26] == '0') &&
     (i_[27] == '0') && (i_[28] == '0') &&
     (i_[29] == '0') && (i_[30] == '1') && (i_[31] == '1')) {
    printf("- This is an Immediate Type Instruction. \n");
    i_process(i_);
  }

  if((i_[25] == '1') && (i_[26] == '1') &&
     (i_[27] == '0') && (i_[28] == '0') &&
     (i_[29] == '1') && (i_[30] == '1') && (i_[31] == '1')) {
    printf("- This is an Immediate Type Instruction. \n");
    i_process(i_);
  }
  if((i_[25] == '0') && (i_[26] == '1') &&
     (i_[27] == '1') && (i_[28] == '0') &&
     (i_[29] == '0') && (i_[30] == '1') && (i_[31] == '1')) {
    printf("- This is an R Type Instruction. \n");
    r_process(i_);
  }    
  if((i_[25] == '1') && (i_[26] == '1') &&
     (i_[27] == '0') && (i_[28] == '0') &&
     (i_[29] == '0') && (i_[30] == '1') && (i_[31] == '1')) {
    printf("- This is a B Type Instruction. \n");
    b_process(i_);
  }
  if((i_[25] == '0') && (i_[26] == '1') &&
     (i_[27] == '0') && (i_[28] == '0') &&
     (i_[29] == '0') && (i_[30] == '1') && (i_[31] == '1')) {
    printf("- This is a S Type Instruction. \n");
    s_process(i_);
  }  
  if((i_[25] == '1') && (i_[26] == '1') &&
     (i_[27] == '0') && (i_[28] == '1') &&
     (i_[29] == '1') && (i_[30] == '1') && (i_[31] == '1')) {
    printf("- This is a J Type Instruction. \n");
    j_process(i_);
  }
  if((i_[25] == '0') && (i_[26] == '0') &&
     (i_[27] == '1') && (i_[28] == '0') &&
     (i_[29] == '1') && (i_[30] == '1') && (i_[31] == '1')) {
    printf("- This is a U Type Instruction. \n");
    u_process(i_);
  }

  /* Added: Opcode 0110111 (51) for LUI U Type Instruction*/
  if((i_[25] == '0') && (i_[26] == '1') &&
     (i_[27] == '1') && (i_[28] == '0') &&
     (i_[29] == '1') && (i_[30] == '1') && (i_[31] == '1')) {
    printf("- This is a U Type Instruction. \n");
    u_process(i_);
  }


  if((i_[25] == '1') && (i_[26] == '1') &&
     (i_[27] == '1') && (i_[28] == '0') &&
     (i_[29] == '0') && (i_[30] == '1') && (i_[31] == '1')) {
    printf("- This is a Software Interruption Instruction. \n");
    interruption_process(i_);
  }

  return 0;

}

unsigned int OPCODE (unsigned int i_word) {

  return ((i_word<<27)>>27);

}

void process_instruction() {

  /* 
     execute one instruction here. You should use CURRENT_STATE and modify
     values in NEXT_STATE. You can call mem_read_32() and mem_write_32() to
     access memory. 
  */   

  unsigned int inst_word = mem_read_32(CURRENT_STATE.PC);
  printf("The instruction is: %x \n", inst_word);
  printf("33222222222211111111110000000000\n");
  printf("10987654321098765432109876543210\n");
  printf("--------------------------------\n");
  printf("%s \n", byte_to_binary32(inst_word));
  printf("\n");
  decode_and_execute(byte_to_binary32(inst_word));

  NEXT_STATE.PC += 4;

}
