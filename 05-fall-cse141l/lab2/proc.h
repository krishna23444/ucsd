#ifndef PROC_H__
#define PROC_H__

#include <cstdio>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <vector>
using namespace std;


class Proc {
public:
  Proc();
  ~Proc();

  void Init();

  int Lookup(const string& reg_name);

  void loadIMem(FILE *fin);
  void loadDMem(FILE *fin);

  void dumpIMem(FILE *fout=stdout, int start=0, int end=-1);
  void dumpDMem(FILE *fout=stdout, int start=0, int end=-1);
  void dumpRegister(FILE *fout=stdout, int num=-1);

  void reset();

  void Fetch();
  void Decode();
  void Execute();
  void WriteResult();
  void SetNextPC();
  void GenerateInstTrace(FILE *fout, int inst_num);
  void GenerateInstTrace(FILE *fout=stdout);

  void Debug();

  void runLoop(int num_inst=-1, bool debug=false);

  void PrintInstCount(FILE *fout=stdout);

private:
  // Representation of an instruction
  struct Instruction {
    unsigned int machine_code;
    string cmd_name;

    bool is_branch;

    unsigned int num_ops;

    unsigned int op1_mask;
    unsigned int op1_shift;
    bool op1_reads_from_reg;
    bool op1_reads_from_mem;

    unsigned int op2_mask;
    unsigned int op2_shift;
    bool op2_reads_from_reg;
    bool op2_reads_from_mem;

    bool writes_to_reg;
    bool writes_to_mem;

    unsigned int op1() { return (machine_code & op1_mask) >> op1_shift; }
    unsigned int op2() { return (machine_code & op2_mask) >> op2_shift; }
  };

  // mappings
  static const string REG_NAMES[];
  static map<string, int> reg_lookup;

  // ISA instruction table
  static vector<Instruction> CMDS;

  // runLoop variables
  unsigned int inst;
  string cmd;
  unsigned int op1, op2, result;
  bool alu_bit_flag;
  bool halted;

  // registers
  unsigned char regs[8];

  // Instruction Memory
  vector<unsigned int> imem;

  // Data Memory
  vector<unsigned int> dmem;

  // Program Counter
  unsigned char pc;

  // Dynamic instruction count
  unsigned int dyn_inst_count;
};

#endif
