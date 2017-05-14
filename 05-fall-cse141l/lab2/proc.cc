#include <cstdio>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "proc.h"
using namespace std;


// register number to name map
const string Proc::REG_NAMES[] = {
  "$r0", "$r1", "$r2", "$r3", "$zero", "$one", "$three", "$res"
};

// register name -> number map
map<string, int> Proc::reg_lookup;

// ISA lookup table
vector<Proc::Instruction> Proc::CMDS(256);

// constructor, call init to initialize everything
Proc::Proc() {
  Init();
}

// destructor
Proc::~Proc() {
}

// Initializes processor's ISA instruction lookup table.
void Proc::Init() {
  // initialize lookup tables for register locations
  reg_lookup["r0"] = 0;
  reg_lookup["r1"] = 1;
  reg_lookup["r2"] = 2;
  reg_lookup["r3"] = 3;
  reg_lookup["zero"] = 4;
  reg_lookup["one"] = 5;
  reg_lookup["three"] = 6;
  reg_lookup["res"] = 7;

  // set $zero, $one, and $three values
  regs[4] = 0;
  regs[5] = 1;
  regs[6] = 3;

  // clear all the commands
  for (unsigned int i = 0; i < CMDS.size(); ++i) {
    CMDS[i].num_ops = 0;
    CMDS[i].op1_mask = 0;
    CMDS[i].op1_shift = 0;
    CMDS[i].op1_reads_from_reg = false;
    CMDS[i].op1_reads_from_mem = false;
    CMDS[i].op2_mask = 0;
    CMDS[i].op2_shift = 0;
    CMDS[i].op2_reads_from_reg = 0;
    CMDS[i].op2_reads_from_mem = false;
    CMDS[i].writes_to_reg = false;
    CMDS[i].writes_to_mem = false;
    CMDS[i].is_branch = false;
    CMDS[i].cmd_name = "";
    CMDS[i].machine_code = i;
  }

  // branch less than
  for (int i = 0x00; i < 0x20; ++i) {
    CMDS[i].is_branch = true;
    CMDS[i].num_ops = 1;
    CMDS[i].op1_mask = 0x1F;
    CMDS[i].cmd_name = "blt";
  }

  // add, subtract
  for (int i = 0x20; i < 0x60; ++i) {
    CMDS[i].num_ops = 2;
    CMDS[i].op1_mask = 0x1C;
    CMDS[i].op1_shift = 2;
    CMDS[i].op1_reads_from_reg = true;
    CMDS[i].op2_mask = 0x03;
    CMDS[i].op2_reads_from_reg = true;
    CMDS[i].writes_to_reg = true;
    CMDS[i].cmd_name = i < 0x40 ? "add" : "sub";
  }

  // shift right logical 4, shift right logical, shift left logical 4
  for (int i = 0x60; i < 0x78; ++i) {
    CMDS[i].num_ops = 1;
    CMDS[i].op1_mask = 0x07;
    CMDS[i].op1_reads_from_reg = true;
    CMDS[i].writes_to_reg = true;

    if (i < 0x68)
      CMDS[i].cmd_name = "srl4";
    else if (i < 0x70)
      CMDS[i].cmd_name = "srl";
    else
      CMDS[i].cmd_name = "sll4";
  }

  // shift right logical 3
  for (int i = 0x78; i < 0x7C; ++i) {
    CMDS[i].num_ops = 1;
    CMDS[i].op1_mask = 0x03;
    CMDS[i].op1_reads_from_reg = true;
    CMDS[i].writes_to_reg = true;
    CMDS[i].cmd_name = "srl3";
  }

  // increment with flag bit register
  CMDS[0x7C].cmd_name = "inch";
  CMDS[0x7C].writes_to_reg = true;

  // halt
  CMDS[0x7D].cmd_name = "hlt";

  // clear $res register
  CMDS[0x7E].cmd_name = "clrr";
  CMDS[0x7E].writes_to_reg = true;

  // 0x7F is unused currently

  // branch greater than or equal to
  for (int i = 0x80; i < 0x90; ++i) {
    CMDS[i].is_branch = true;
    CMDS[i].num_ops = 1;
    CMDS[i].op1_mask = 0x0F;
    CMDS[i].cmd_name = "bge";
  }

  // bitwise and with $one
  for (int i = 0x90; i < 0x98; ++i) {
    CMDS[i].num_ops = 1;
    CMDS[i].op1_mask = 0x07;
    CMDS[i].op1_reads_from_reg = true;
    CMDS[i].writes_to_reg = true;
    CMDS[i].cmd_name = "and1";
  }

  // 0x98 - 0x9F are unused

  // move
  for (int i = 0xA0; i < 0xC0; ++i) {
    CMDS[i].num_ops = 2;
    CMDS[i].op1_mask = 0x18;
    CMDS[i].op1_shift = 3;
    CMDS[i].op2_mask = 0x07;
    CMDS[i].op2_reads_from_reg = true;
    CMDS[i].writes_to_reg = true;
    CMDS[i].cmd_name = "mov";
  }

  // load, load next
  for (int i = 0xC0; i < 0xE0; ++i) {
    CMDS[i].num_ops = 2;
    CMDS[i].op1_mask = 0x0C;
    CMDS[i].op1_shift = 2;
    CMDS[i].op2_mask = 0x03;
    CMDS[i].op2_reads_from_reg = true;
    CMDS[i].op2_reads_from_mem = true;
    CMDS[i].writes_to_reg = true;
    if (i < 0xD0)
      CMDS[i].cmd_name = "ld";
    else if (i < 0xE0)
      CMDS[i].cmd_name = "ldn";
  }

  // store, store next
  for (int i = 0xE0; i < 0xFF; ++i) {
    CMDS[i].num_ops = 2;
    CMDS[i].op1_mask = 0x0C;
    CMDS[i].op1_shift = 2;
    CMDS[i].op1_reads_from_reg = true;
    CMDS[i].op2_mask = 0x03;
    CMDS[i].op2_reads_from_reg = true;
    CMDS[i].writes_to_mem = true;
    if (i < 0xF0)
      CMDS[i].cmd_name = "st";
    else
      CMDS[i].cmd_name = "stn";
  }
}

// Get a register's index by name
int Proc::Lookup(const string& reg_name) {
  if (reg_lookup.find(reg_name) != reg_lookup.end())
    return reg_lookup[reg_name];

  return -1;
}

// Reads machine code instructions from a file into imem.
void Proc::loadIMem(FILE *fin) {
  unsigned int tmp;

  while (!feof(fin)) {
    fscanf(fin, " %x ", &tmp);
    imem.push_back(tmp);
  }
}

// Loads data memory into processor
void Proc::loadDMem(FILE *fin) {
  unsigned int tmp;

  while (!feof(fin)) {
    fscanf(fin, " %x ", &tmp);
    dmem.push_back(tmp);
  }
}

// Dumps instructions memory (imem) in range start-end
void Proc::dumpIMem(FILE *fout, int start, int end) {
  if (end == -1 || end > (int)imem.size())
    end = imem.size();

  for (int i = start; i < end; ++i)
    fprintf(fout, "%d: 0x%02X\n", i, imem[i]);

  fflush(fout);
}

// Dumps data memory (dmem) in range start-end
void Proc::dumpDMem(FILE *fout, int start, int end) {
  if (end == -1 || end > (int)dmem.size())
    end = dmem.size();

  for (int i = start; i < end; ++i)
    fprintf(fout, "%d: 0x%02X\n", i, dmem[i]);

  fflush(fout);
}

// Dumps a given register, or all if num is outside the register range
void Proc::dumpRegister(FILE *fout, int num) {
  if (num >= 0 && num <= 7)
    fprintf(fout, "%-7s: %d\n", REG_NAMES[num].c_str(), regs[num]);

  else
    for (int i = 0; i <= 7; ++i)
      dumpRegister(fout, i);
}

// Resets program counter and dynamic instruction count
void Proc::reset() {
  pc = 0;
  dyn_inst_count = 0;
}

// Prints the dynamic instruction count
void Proc::PrintInstCount(FILE *fout) {
  fprintf(fout, "Dynamic Instruction Count: %u\n", dyn_inst_count);
  fflush(fout);
}

// Fetch the next instruciton (pointed to by pc)
void Proc::Fetch() {
  inst =  imem[pc];
  cmd = CMDS[inst].cmd_name;
}

// Decodes the instruction and fetches operands. Reads from registers if needed
void Proc::Decode() {
  switch (CMDS[inst].num_ops) {
    case 2:
      // get operand 2 location
      op2 = (inst & CMDS[inst].op2_mask) >> CMDS[inst].op2_shift;

      // read operand 2 from registers, if necessary
      if (CMDS[inst].op2_reads_from_reg)
        op2 = regs[op2];
    case 1:
      // get operand 1 location
      op1 = (inst & CMDS[inst].op1_mask) >> CMDS[inst].op1_shift;

      // read operand 1 from registers, if necessary
      if (CMDS[inst].op1_reads_from_reg)
        op1 = regs[op1];
      break;
    case 0:
    default:
      op2 = 0;
      op1 = 0;
      break;
  }
}

// Executes instruction. Does the ALU task and writes to 'result' and
// 'alu_flag_bit'
void Proc::Execute() {
  // halt
  if (cmd == "hlt") {
    halted = true;
  }
  // add
  else if (cmd == "add") {
    result = (op1 + op2) & 0xFF;
    alu_bit_flag = (op1 + op2) > 0xFF;
  }
  // subtract
  else if (cmd == "sub") {
    result = op1 - op2;
    alu_bit_flag = op1 < op2;
  }
  // shift right logical by 4
  else if (cmd == "srl4") {
    result = op1 >> 4;
  }
  // shift right logical by 1
  else if (cmd == "srl") {
    result = op1 >> 1;
  }
  // shift left logical by 4
  else if (cmd == "sll4") {
    result = op1 << 4;
  }
  // shift right logical by 3
  else if (cmd == "srl3") {
    result = op1 >> 3;
  }
  // increment with alu flag
  else if (cmd == "inch") {
    result = regs[Lookup("res")];
    if (alu_bit_flag)
      result++;
  }
  // clear res register
  else if (cmd == "clrr") {
    result = 0;
  }
  // branch less than, branch greater than or equal to, branch less than to 17
  else if (cmd == "blt" || cmd == "bge") {
    result = alu_bit_flag;
  }
  // bitwise and
  else if (cmd == "and1") {
    result = op1 & regs[Lookup("one")];
  }
  // move
  else if (cmd == "mov") {
    result = op2;
  }
  // load
  else if (cmd == "ld") {
    result = dmem[op2];
  }
  // load next
  else if (cmd == "ldn") {
    result = dmem[op2 + 1];
  }
  // store, store next
  else if (cmd == "st" || cmd == "stn") {
    result = op2;
  }
}

// In general commands write to either op1 or $res, but there are many
// exceptions to the rule.
void Proc::WriteResult() {
  // most commands write to op1, so we initialize to that register/address
  int dst = CMDS[inst].op1();

  // writing to a register
  if (CMDS[inst].writes_to_reg) {
    // addition is special, writes to op1 only if its not $zero, $one or $three,
    // otherwise writes to op2
    if (cmd == "add") {
      if (dst == Lookup("zero") || dst == Lookup("one") ||
            dst == Lookup("three"))
        dst = CMDS[inst].op2();
    }

    // commands that write to $res register
    else if (cmd == "sub" || cmd == "inch" || cmd == "clrr")
      dst = Lookup("res");

    regs[dst] = result;
  }

  // writing to memory
  else if (CMDS[inst].writes_to_mem) {
    dst = regs[dst];

    // store next memory location, increment dst
    if (cmd == "stn")
      dst++;

    printf("cmd writes to memory -- dst = %d, res = %d\n", dst, result);
    dmem[dst] = result;
  }
}

// Sets next program counter (pc). Checks for 
void Proc::SetNextPC() {
  // branch less than, check alu flag
  if (cmd == "blt")
    pc = alu_bit_flag ? pc - op1 : pc + 1;
  // branch greater than or equal to, check opposite of alu flag
  else if (cmd == "bge")
    pc = !alu_bit_flag ? pc + op1 : pc + 1;
  else
    pc = pc + 1;
}

// Prints the instruction just executed
void Proc::GenerateInstTrace(FILE *fout, int inst_num) {
  fprintf(fout, "0x%02X: \"%s", inst_num, CMDS[inst_num].cmd_name.c_str());

  switch (CMDS[inst_num].num_ops) {
    case 2:
      fprintf(fout, "  %s, %s", REG_NAMES[CMDS[inst_num].op1()].c_str(),
              REG_NAMES[CMDS[inst_num].op2()].c_str());
      break;
    case 1:
      if (CMDS[inst_num].is_branch)
        fprintf(fout, "  %d", CMDS[inst_num].op1());
      else
        fprintf(fout, "  %s", REG_NAMES[CMDS[inst_num].op1()].c_str());
      break;
    case 0:
    default:
      break;
  }

  fprintf(fout, "\"  ");
  fprintf(fout, "[nextpc = %d", pc);

  if (cmd == "add" || cmd == "sub")
    fprintf(fout, ", alu_flag = %d", alu_bit_flag);

  fprintf(fout, "]");
  fprintf(fout, "\n");
}

// Prints the instruction just executed
void Proc::GenerateInstTrace(FILE *fout) {
  GenerateInstTrace(fout, inst);
}

//
void Proc::Debug() {
  GenerateInstTrace();
  dumpRegister();
}

// Main loop program. Uncomment the two function calls below to enable
// instruction by instruction debugging
void Proc::runLoop(int num_inst, bool debug) {
  if (num_inst < 0)
    num_inst = INT_MAX;

  halted = false;

  for (int i = 0; i < num_inst && !halted; ++i) {
    Fetch();

    Decode();

    Execute();

    WriteResult();

    SetNextPC();

    if (debug)
      Debug();

    dyn_inst_count++;
  }
}
