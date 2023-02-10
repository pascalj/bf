#include "brainfuck.h"
#include <functional>

Program parse(const std::vector<char> source) {
  Program program{};
  std::transform(source.begin(), source.end(),
                 std::back_inserter(program.instructions), [](char token) {
                  return Instruction{};
                 });
  return program;
}
