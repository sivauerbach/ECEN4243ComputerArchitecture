// riscvsingle.sv

// RISC-V single-cycle processor
// From Section 7.6 of Digital Design & Computer Architecture
// 27 April 2020
// David_Harris@hmc.edu 
// Sarah.Harris@unlv.edu

// run 210
// Expect simulator to print "Simulation succeeded"
// when the value 25 (0x19) is written to address 100 (0x64)

//   Instruction  opcode    funct3    funct7
//   add          0110011   000       0000000
//   sub          0110011   000       0100000
//   and          0110011   111       0000000
//   or           0110011   110       0000000
//   slt          0110011   010       0000000
//   addi         0010011   000       immediate
//   andi         0010011   111       immediate
//   ori          0010011   110       immediate
//   slti         0010011   010       immediate
//   beq          1100011   000       immediate
//   lw	          0000011   010       immediate
//   sw           0100011   010       immediate
//   jal          1101111   immediate immediate

module testbench();

  logic        clk;
  logic        reset;

  logic [31:0] WriteData;
  logic [31:0] DataAdr;
  logic        MemWrite;

  // instantiate device to be tested
  top dut(clk, reset, WriteData, DataAdr, MemWrite);

  initial
    begin
      string memfilename;
      
      /*Lab 1 test files: */
      memfilename = {"../riscvtest/lab1_tests/sb.memfile"};
      
      /* Main test file */
      //memfilename = {"../riscvtest/mytest.memfile"};
      
      $readmemh(memfilename, dut.imem.RAM);
    end

  
  // initialize test
  initial begin
    reset <= 1; # 22; reset <= 0;
  end

  // generate clock to sequence tests
  always begin
      clk <= 1; # 5; clk <= 0; # 5;
  end

  // // check results
  // always @(negedge clk)
  //   begin
  //     if(MemWrite) begin
  //         if(DataAdr === 100 & WriteData === 25) begin
  //           $display("Simulation succeeded");
  //           $stop;
  //           end 
  //         else if (DataAdr !== 96) begin
  //           $display("Simulation failed");
  //           $stop;
  //           end
  //     end
  //   end
endmodule // testbench

module riscvsingle (input  logic        clk, reset,
      output logic [31:0] PC,
      input  logic [31:0] Instr,
      output logic 	MemWrite,
      output logic [31:0] ALUResult, WriteData,
      input  logic [31:0] ReadData);
  
  logic 				ALUSrc, RegWrite, Zero, BranchYN;
  logic [1:0] 				ResultSrc, JumpType, PCSrc; 
  logic [2:0]         ImmSrc;
  logic [3:0] 				ALUControl;
  
  controller c (Instr[6:0], Instr[14:12], Instr[30], Zero, BranchYN,
    ResultSrc, MemWrite, PCSrc,
    ALUSrc, RegWrite, JumpType,
    ImmSrc, ALUControl);
  datapath dp (clk, reset, ResultSrc, PCSrc,
  ALUSrc, RegWrite,
  ImmSrc, ALUControl,
  Instr[14:12], Zero, BranchYN, PC, Instr,
  ALUResult, WriteData, ReadData);
  
endmodule // riscvsingle

module controller (input  logic [6:0] op,
      input  logic [2:0] funct3,
      input  logic       funct7b5,
      input  logic       Zero,
      input  logic       BranchYN,
      output logic [1:0] ResultSrc,
      output logic       MemWrite,
      output logic [1:0] PCSrc,
      output logic       ALUSrc,

      output logic       RegWrite, 
      output logic [1:0] JumpType,
      output logic [2:0] ImmSrc,
      output logic [3:0] ALUControl);
  
  logic [1:0] 			      ALUOp;
  logic 			      BranchType;
  
  maindec md (op, ResultSrc, MemWrite, BranchType,
        ALUSrc, RegWrite, JumpType, ImmSrc, ALUOp);
  aludec ad (op, funct3, funct7b5, ALUOp, ALUControl);
  
  //assigns PCSrc: 
  //JAL : PCSrc = 10
  //JALR : PCSrc = 11
  // B type: PCSrc = 01
  // else: PCSrc = 00
  
  // assign PCSrc = 

  
  always_comb
    case(JumpType)
      2'b00: PCSrc = (BranchType & (BranchYN)) ? 2'b01 : 2'b00; //else/branch
      2'b01: PCSrc = 2'b01;                                     // JAL
      2'b10: PCSrc = 2'b10;                                     // JALR
      default: PCSrc =2'b00;
    endcase

  
endmodule // controller

module maindec (input  logic [6:0] op,
  output logic [1:0] ResultSrc,
  output logic 	   MemWrite,
  output logic 	   BranchType, ALUSrc,
  output logic 	   RegWrite,
  output logic [1:0] JumpType,
  output logic [2:0] ImmSrc,
  output logic [1:0] ALUOp);
  
  logic [12:0] 		   controls;
  
  assign {RegWrite, ImmSrc, ALUSrc, MemWrite,
    ResultSrc, BranchType, ALUOp, JumpType} = controls;
  
  always_comb
    case(op)
      // RegWrite_ImmSrc_ALUSrc_MemWrite_ResultSrc_Branch_ALUOp_JumpType_MemStrobe ****
      7'b0000011: controls = 13'b1_000_1_0_01_0_00_00; // lw, lh, lb ****
      7'b0110111: controls = 13'b1_100_1_0_00_0_10_00; // lui in ALU
      7'b0100011: controls = 13'b0_001_1_1_00_0_00_00; // sw, sb, sh
      7'b0110011: controls = 13'b1_xxx_0_0_00_0_10_00; // R–type
      7'b1100011: controls = 13'b0_010_0_0_00_1_01_00; // B-Type
      7'b0010011: controls = 13'b1_000_1_0_00_0_10_00; // I–type ALU
      7'b1101111: controls = 13'b1_011_0_0_10_0_00_01; // jal
      7'b0010111: controls = 13'b1_100_1_0_00_0_10_00; // auipc
      7'b1100111: controls = 13'b1_011_0_0_10_0_00_10; // jalr
      
      default: controls = 13'bx_xxx_x_x_xx_x_xx_x; // ???

      // Define ALUSrc: 0=R type    1=I type (Source for SrcB)
      //
      //

    endcase // case (op)
  
endmodule // maindec

// Computes ALUControl
module aludec (
        input  logic [6:0] op,
        input  logic [2:0] funct3,
        input  logic 	  funct7b5,
        input  logic [1:0] ALUOp,
        output logic [3:0] ALUControl);
  
  logic 			  SubSra;
  
  // TRUE for SUB, SRA, SRAI
  assign SubSra = (((op == 7'b0010011) & (funct3 == 3'b101) & funct7b5) // case for SRL/SRA 
                  | (op == 7'b0110011) & funct7b5);                    // case for Add/Sub
  always_comb
  case(op)
    7'b0110111: ALUControl = 4'b1110; //LUI
    7'b0010111: ALUControl = 4'b1001; //AUIPC
    7'b1100111: ALUControl = 4'b0000; // JALR
    default: case(ALUOp)
      2'b00: ALUControl = 4'b0000; // addition
      2'b01: ALUControl = 4'b0001; // subtraction
      default: case(funct3) // R–type or I–type ALU
                3'b000: if (SubSra)
                          ALUControl = 4'b0001; // sub
                        else
                          ALUControl = 4'b0000; // add, addi
                3'b001: ALUControl = 4'b0110; //sll, slli
                3'b010: ALUControl = 4'b0101; // slt, slti
                3'b011: ALUControl = 4'b1101;  //sltu, sltiu
                3'b100: ALUControl = 4'b0100; // xor, xori
                3'b101: if (SubSra)
                          ALUControl = 4'b1111; //sra, srai
                        else
                          ALUControl = 4'b0111; //srl srli
                3'b110: ALUControl = 4'b0011; // or, ori
                3'b111: ALUControl = 4'b0010; // and, andi  
                default: ALUControl = 4'bxxxx; // ???
              endcase // case (funct3)       
    endcase // case (ALUOp)
  endcase // case(op)
  
endmodule // aludec

module datapath (input  logic        clk, reset,
    input  logic [1:0]  ResultSrc,
    input  logic [1:0]  PCSrc,
    input  logic 	      ALUSrc,
    input  logic 	     RegWrite,
    input  logic [2:0]  ImmSrc,
    input  logic [3:0]  ALUControl,
    input  logic [2:0]  funct3, //new
    output logic 	      Zero,
    output logic 	      BranchYN,
    output logic [31:0] PC,
    input  logic [31:0] Instr,
    output logic [31:0] ALUResult, WriteData,
    input  logic [31:0] ReadData/*Full chunk of data from DataMem*/);
  
  logic [31:0] 		     PCNext, PCPlus4, PCTarget;
  logic [31:0] 		     ImmExt;
  logic [31:0] 		     SrcA, SrcB;
  logic [31:0] 		     Result;
  logic [31:0]         ModReadData, RegWriteData;
  
  // next PC logic
  flopr #(32) pcreg (clk, reset, PCNext, PC); // parameter WIDTH=32 
  adder  pcadd4 (PC, 32'd4, PCPlus4);
  adder  pcaddbranch (PC, ImmExt, PCTarget);
  mux3 #(32)  pcmux (PCPlus4, PCTarget, ALUResult, PCSrc, PCNext); 
  // register file logic
  regfile  rf (clk, RegWrite, Instr[19:15], Instr[24:20],
        Instr[11:7], Result, SrcA, RegWriteData);
  extend  ext (Instr[31:7], ImmSrc, ImmExt);
  //Load-Type inst Data
  LoadDec loaddec (ReadData, funct3, ModReadData);
  // Store-Type
  StoreDec storedec (RegWriteData, funct3, WriteData);

  // ALU logic
  mux2 #(32)  srcbmux (WriteData, ImmExt, ALUSrc, SrcB);
  alu  alu (SrcA, SrcB, ALUControl, funct3, PC, ALUResult, Zero, BranchYN);
  mux3 #(32) resultmux (ALUResult, ModReadData, PCPlus4, ResultSrc, Result);

endmodule // datapath

module adder (input  logic [31:0] a, b,
      output logic [31:0] y);
  
  assign y = a + b;
  
endmodule

module extend (input  logic [31:7] instr,
                input  logic [2:0]  immsrc,
                output logic [31:0] immext);
  
  always_comb
    case(immsrc)
      // I−type
      3'b000:  immext = {{20{instr[31]}}, instr[31:20]};
      // S−type (stores)
      3'b001:  immext = {{20{instr[31]}}, instr[31:25], instr[11:7]};
      // B−type (branches)
      3'b010:  immext = {{20{instr[31]}}, instr[7], instr[30:25], instr[11:8], 1'b0};       
      // J−type (jal)
      3'b011:  immext = {{12{instr[31]}}, instr[19:12], instr[20], instr[30:21], 1'b0};
      // LUI **
      3'b100: immext = {{20{instr[31]}}, instr[31:12]};
      default: immext = 32'bx; // undefined (in colroller set immSrc= 3'b100) **
    endcase // case (immsrc)
  
endmodule // extend

module flopr #(parameter WIDTH = 8)
  (input  logic             clk, reset,
  input logic [WIDTH-1:0]  d,
  output logic [WIDTH-1:0] q);
  
  always_ff @(posedge clk, posedge reset)
    if (reset) q <= 0;
    else  q <= d;
  
  endmodule // flopr

module flopenr #(parameter WIDTH = 8)
  (input  logic             clk, reset, en,
  input logic [WIDTH-1:0]  d,
  output logic [WIDTH-1:0] q);
  
  always_ff @(posedge clk, posedge reset)
    if (reset)  q <= 0;
    else if (en) q <= d;
  
  endmodule // flopenr

module mux2 #(parameter WIDTH = 8)
  (input  logic [WIDTH-1:0] d0, d1,
  input logic 	     s,
  output logic [WIDTH-1:0] y);
  
  assign y = s ? d1 : d0;
  
  endmodule // mux2

module mux3 #(parameter WIDTH = 8)
  (input  logic [WIDTH-1:0] d0, d1, d2,
  input logic [1:0] 	     s,
  output logic [WIDTH-1:0] y);
  
  // for ResultSrc = 0: output = d0 = ALUResult
  // for ResultSrc = 1: output = d1 = ReadData
  // for ResultSrc = 2: output = d2 = PCPlus4
  // for ResultSrc = 3: output = d3 = PCPlus4
  assign y = s[1] ? d2 : (s[0] ? d1 : d0);
  
  endmodule // mux3

module top (input  logic        clk, reset,
    output logic [31:0] WriteData, DataAdr,
    output logic 	MemWrite);
  
  logic [31:0] 		PC, Instr, ReadData;
  
  // instantiate processor and memories
  riscvsingle rv32single (clk, reset, PC, Instr, MemWrite, DataAdr,/*=ALUResult*/
        WriteData, ReadData);
  imem imem (PC, Instr);
  dmem dmem (clk, MemWrite, DataAdr, WriteData, ReadData);
  
  endmodule // top

module imem (input  logic [31:0] a,
      output logic [31:0] rd);
  
  logic [31:0] 		 RAM[63:0];
  
  assign rd = RAM[a[31:2]]; // word aligned
  
  endmodule // imem

module dmem (input  logic        clk, we,
      input  logic [31:0] a, wd,
      output logic [31:0] rd);
  
  logic [31:0] 		 RAM[1047:0];
  
  assign rd = RAM[a[31:2]]; // word aligned
  always_ff @(posedge clk)
    if (we) RAM[a[31:2]] <= wd;

/*
Option:

logic [31:0] correct_wd;

  always_comb
    case(funct3)
      3'b000:  correct_wd = {rd[]}
      // S−type (stores)
      3'b001:  immext = {{20{instr[31]}}, instr[31:25], instr[11:7]};
      // B−type (branches)
      3'b010:  immext = {{20{instr[31]}}, instr[7], instr[30:25], instr[11:8], 1'b0};       
      // J−type (jal)
      3'b011:  immext = {{12{instr[31]}}, instr[19:12], instr[20], instr[30:21], 1'b0};
      // LUI **
      3'b100: immext = {{20{instr[31]}}, instr[31:12]};
      default: immext = 32'bx; // undefined (in colroller set immSrc= 3'b100) **
    endcase // case (immsrc)

    // case (func3) 

  //    xxxx: if (we) RAM[a[31:2]] <= wd[7:0];
  //    xxxx: if (we) RAM[a[31:2]] <= wd[15:0];
  //    xxxx: if (we) RAM[a[31:2]] <= wd[31:0];
*/


  endmodule // dmem

module alu (input  logic [31:0] a, b,
            input  logic [3:0] 	alucontrol,
            input  logic [2:0]  funct3,
            input logic [31:0] PC,
            output logic [31:0] ALUResult,
            output logic 	zero,
            output logic BranchYN);

  logic [31:0] 	       condinvb, sum;
  logic 		       v, c;              // overflow. Q: why do we need this?
  logic 		       isAddSub;       // true when is add or subtract operation


  branchComp bc(a, b, funct3, BranchYN);
  assign condinvb = alucontrol[0] ? ~b : b;
  assign {c,sum} = a + condinvb + alucontrol[0];
  assign isAddSub = ~alucontrol[2] & ~alucontrol[1] |
                    ~alucontrol[1] & alucontrol[0];   

  always_comb
    case (alucontrol)
      4'b0000:  ALUResult = sum;         // add, addi
      4'b0001:  ALUResult = sum;         // subtract
      4'b0010:  ALUResult = a & b;       // and, andi
      4'b0011:  ALUResult = a | b;       // or, ori
      4'b0100:  ALUResult = a ^ b;       // xor, xori
      4'b0101:  ALUResult = sum[31] ^ v; // slt, slti
//      4'b1101:  ALUResult = ($unsigned(a) < $unsigned(b)) ? 32'b1 : 0;     // sltu, sltiu  
      4'b1101:  ALUResult = { {30{1'b0}}, ~c };     // sltu, sltiu  
      4'b0110:  ALUResult = a << b[4:0];      //sll, slli
      4'b0111:  ALUResult = a >> b[4:0];      //srl
      4'b1110:  ALUResult = b << 12;          //LUI
      4'b1111:  ALUResult = a >>> b[4:0];     // sra, srai
      4'b1001: ALUResult = PC + (b << 12);            //AUIPC
      default: ALUResult = 32'bx;
    endcase

  assign zero = (ALUResult == 32'b0);
  assign v = ~(alucontrol[0] ^ a[31] ^ b[31]) & (a[31] ^ sum[31]) & isAddSub;
  
endmodule // alu

module regfile (input  logic        clk, 
		input  logic 	    we3, 
		input  logic [4:0]  a1, a2, a3, // a1=Rs1, a2=Rs2, a3=Rd
		input  logic [31:0] wd3,        // wd3 = Result
		output logic [31:0] rd1, rd2);  // rd1 = SrcA, rd2 = WriteData

  logic [31:0] 		    rf[31:0];

   // three ported register file
   // read two ports combinationally (A1/RD1, A2/RD2)
   // write third port on rising edge of clock (A3/WD3/WE3)
   // register 0 hardwired to 0

  always_ff @(posedge clk)
    if (we3) rf[a3] <= wd3;	
    assign rd1 = (a1 != 0) ? rf[a1] : 0;
    assign rd2 = (a2 != 0) ? rf[a2] : 0;
  
endmodule // regfile

module branchComp (input [31:0] a,
                  input [31:0] b,
                  input [2:0] funct3,
                  output logic BranchYN);

  always_comb
    case (funct3)
      3'b000:  BranchYN = (a == b);                      // beq
      3'b001:  BranchYN = ~(a == b);                     // bne
      3'b100:  BranchYN = ($signed(a) < $signed(b));     // blt
      3'b101:  BranchYN = ($signed(a) >= $signed(b));    // bge  
      3'b110:  BranchYN = (a < b);                      // bltu
      3'b111:  BranchYN = (a >= b);                     // bgeu
      default: BranchYN = 1'bx;
    endcase

endmodule

module LoadDec( input logic [31:0] ReadData,
                input logic [2:0]   funct3,
                output logic [31:0] ModReadData);

  always_comb
    case (funct3)
      3'b000:  ModReadData = {{24{ReadData[7]}}, ReadData[7:0]};    // lb
      3'b001:  ModReadData = {{16{ReadData[15]}}, ReadData[15:0]};  // lh
      3'b010:  ModReadData = ReadData;                               // lw
      3'b100:  ModReadData = {{24{1'b0}}, ReadData[7:0]};           // lbu
      3'b101:  ModReadData = {{16{1'b0}}, ReadData[15:0]};          // lhu
      default: ModReadData = 32'bx;
    endcase

endmodule

module StoreDec ( input logic [31:0]  RegWriteData,
                  input logic [2:0]   funct3,
                  output logic [31:0] WriteData);

  always_comb
    case (funct3)
      3'b000:  WriteData = {{24{RegWriteData[7]}}, RegWriteData[7:0]};    // sb
      3'b001:  WriteData = {{16{RegWriteData[15]}}, RegWriteData[15:0]};  // sh
      3'b010:  WriteData = RegWriteData;                               // sw
      default: WriteData = RegWriteData;
    endcase

endmodule