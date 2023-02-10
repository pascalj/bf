#pragma once

#include <vector>

class Instruction {
};


struct Program {
  std::vector<Instruction> instructions;
};

Program parse(const std::vector<char> source);
