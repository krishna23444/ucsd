#include <cstdio>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "proc.h"
using namespace std;


// non-negative number to string mapping
const string Proc::NUMS[] = {
  "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13",
  "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25"
};

// register number to name map
const string Proc::REGS[] = {
  "$r0", "$r1", "$r2", "$r3", "$zero", "$one", "$three", "$res"
};

map<string, int> Proc::reg_lookup;
vector<Proc::Instruction> Proc::CMDS(256);

//
Proc::Proc() {
  Init();
}

//
Proc::~Proc() {
}

//
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

  // halt
  CMDS[0x0].cmd_name = "hlt";

  // branch less than, branch greater than
  for (int i = 0x01; i < 0x20; ++i) {
    CMDS[i].is_branch = true;
    CMDS[i].num_ops = 1;
    CMDS[i].op1_mask = 0x0F;
    CMDS[i].cmd_name = i < 0x10 ? "blt" : "bge";
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

  // branch less than to line 14
  CMDS[0x7D].cmd_name = "blt14";
  CMDS[0x7D].is_branch = true;

  // clear $res register
  CMDS[0x7E].cmd_name = "clrr";
  CMDS[0x7E].writes_to_reg = true;

  // 0x7F is unused currently

  // bitwise and
  for (int i = 0x80; i < 0xA0; ++i) {
    CMDS[i].num_ops = 2;
    CMDS[i].op1_mask = 0x18;
    CMDS[i].op1_shift = 3;
    CMDS[i].op1_reads_from_reg = true;
    CMDS[i].op2_mask = 0x07;
    CMDS[i].op2_reads_from_reg = true;
    CMDS[i].writes_to_reg = true;
    CMDS[i].cmd_name = "and";
  }

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

//
int Proc::Lookup(const string& reg_name) {
  if (reg_lookup.find(reg_name) != reg_lookup.end())
    return reg_lookup[reg_name];

  return -1;
}

//
void Proc::PrintInst(int inst_num) {
  /*
  cerr << "executing 0x" << setbase(16) << inst_num << ": " <<
    CMDS[inst_num].cmd_name << setbase(10);
  if (CMDS[inst_num].num_ops > 0)
    cerr << "  " << REGS[CMDS[inst_num].op1()];
  if (CMDS[inst_num].num_ops > 1)
    cerr << ", " << REGS[CMDS[inst_num].op2()];
  cerr << " " << CMDS[inst_num].op1() << ", " << CMDS[inst_num].op2();
  cerr << " num_opts: " << CMDS[inst_num].num_ops << setbase(16);
  cerr << ", op1: (0x" << CMDS[inst_num].op1_mask << ", " <<
      CMDS[inst_num].op1_shift << ") ";
  cerr << "op2: (0x" << CMDS[inst_num].op2_mask << ", " <<
      CMDS[inst_num].op2_shift << ") " << setbase(10);
  cerr << "\n";
      */
}

// Reads machine code instructions from a file into imem.
void Proc::loadIMem(FILE *fin) {
  unsigned int tmp;

  while (!feof(fin)) {
    fscanf(fin, " %x ", &tmp);
    imem.push_back(tmp);
  }
}

//
void Proc::loadDMem(FILE *fin) {
  unsigned int tmp;

  while (!feof(fin)) {
    fscanf(fin, " %x ", &tmp);
    dmem.push_back(tmp);
  }
}

//
void Proc::dumpIMem(FILE *fout, int start, int end) {
  if (end == -1 || end > (int)imem.size())
    end = imem.size();

  for (int i = start; i < end; ++i)
    fprintf(fout, "%d: 0x%02X\n", i, imem[i]);

  fflush(fout);
}

//
void Proc::dumpDMem(FILE *fout, int start, int end) {
  if (end == -1 || end > (int)dmem.size())
    end = dmem.size();

  for (int i = start; i < end; ++i)
    fprintf(fout, "%d: 0x%02X\n", i, dmem[i]);

  fflush(fout);
}

//
void Proc::dumpRegister(FILE *fout, int num) {
  if (num >= 0 && num <= 7)
    fprintf(fout, "%-7s: %d\n", REGS[num].c_str(), regs[num]);

  else
    for (int i = 0; i <= 7; ++i)
      dumpRegister(fout, i);
}

// Resets program counter and dynamic instruction count
void Proc::reset() {
  pc = 0;
  dyn_inst_count = 0;
}

//
void Proc::PrintInstCount(FILE *fout) {
  fprintf(fout, "Dynamic Instruction Count: %u\n", dyn_inst_count);
  fflush(fout);
}

//
void Proc::Fetch() {
  inst =  imem[pc];
  cmd = CMDS[inst].cmd_name;
}

//
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

//
void Proc::Execute() {
  // halt
  if (cmd == "hlt") {
    halted = true;
//    cerr << "hlt";
  }

  // add
  else if (cmd == "add") {
    result = (op1 + op2) & 0xFF;
    alu_bit_flag = (op1 + op2) > 0xFF;
//    cerr << "add";
  }

  // subtract
  else if (cmd == "sub") {
    result = op1 - op2;
    alu_bit_flag = op1 < op2;
//    cerr << "sub";
  }

  // shift right logical by 4
  else if (cmd == "srl4") {
    result = op1 >> 4;
//    cerr << "srl4";
  }

  // shift right logical by 1
  else if (cmd == "srl") {
    result = op1 >> 1;
//    cerr << "srl";
  }

  // shift left logical by 4
  else if (cmd == "sll4") {
    result = op1 << 4;
//    cerr << "sll4";
  }

  // shift right logical by 3
  else if (cmd == "srl3") {
    result = op1 >> 3;
//    cerr << "srl3";
  }

  // increment with alu flag
  else if (cmd == "inch") {
    result = regs[Lookup("res")];
    if (alu_bit_flag)
      result++;
//    cerr << "inch";
  }

  // clear res register
  else if (cmd == "clrr") {
    result = 0;
//    cerr << "clrr";
  }

  // branch less than, branch greater than or equal to, branch less than to 14
  else if (cmd == "blt" || cmd == "bge" || cmd == "blt14") {
    result = alu_bit_flag;
//    cerr << "B";
  }
  // bitwise and
  else if (cmd == "and") {
    result = op1 & op2;
//    cerr << "and";
  }
  // move
  else if (cmd == "mov") {
    result = op2;
//    cerr << "mov";
  }
  // load
  else if (cmd == "ld") {
    result = dmem[op2];
//    cerr << "ld";
  }
  // load next
  else if (cmd == "ldn") {
    result = dmem[op2 + 1];
//    cerr << "ldn";
  }
  // store, store next
  else if (cmd == "st" || cmd == "stn") {
    result = op2;
//    cerr << "st/stn";
  }
  
  /*
  cerr << ": op1 = " << op1 << ", op2 = " << op2 << ", result = " << result <<
      ", flag = " << alu_bit_flag << endl;
      */
}

// In general commands write to either op1 or $res, but there are many
// exceptions to the rule.
void Proc::WriteResult() {
  // most commands write to op1, so we initialize to that register/address
  int dst = CMDS[inst].op1();

  // writing to a register
  if (CMDS[inst].writes_to_reg) {
    // load from next memory, increment dst
    if (cmd == "ldn")
      dst++;

    // addition is special, writes to op1 only if its not $zero, $one or $three,
    // otherwise writes to op2
    else if (cmd == "add") {
      if (dst == Lookup("zero") || dst == Lookup("one") ||
            dst == Lookup("three"))
        dst = CMDS[inst].op2();
    }

    // commands that write to $res register
    else if (cmd == "sub" || cmd == "inch" || cmd == "clrr")
      dst = Lookup("res");

//    cerr << "writing " << result << " to reg " << dst << endl;
    regs[dst] = result;
  }

  // writing to memory
  else if (CMDS[inst].writes_to_mem) {
    dst = regs[dst];

    // store next memory location, increment dst
    if (cmd == "stn")
      dst++;

//    cerr << "writing " << result << " to mem addr " << dst << endl;
    dmem[dst] = result;
  }
}

//
void Proc::SetNextPC() {
  if (cmd == "blt") {
    pc = result > 0 ? pc - op1 : pc + 1;
    cerr << "blt\n";
  }
  else if (cmd == "bge") {
    pc = result > 0 ? pc + op1 : pc + 1;
    cerr << "bge\n";
  }
  else if (cmd == "blt14") {
    pc = result > 0 ? 14 : pc + 1;
    cerr << "blt14\n";
  }
 else
    pc = pc + 1;
}

//
void Proc::GenerateInstTrace() {
}

//
void Proc::runLoop(int num_inst) {
  if (num_inst > 256)
    num_inst = 256;

  halted = false;

  for (int i = 0; i < num_inst && !halted; ++i) {
    Fetch();

        PrintInst(inst);

    Decode();

    Execute();

    WriteResult();

    SetNextPC();

//        dumpRegister();

    GenerateInstTrace();

    dyn_inst_count++;
  }
}
