#include <cstring>
#include <vector>
#include <iostream>

int main(int, char **) {

#include "program.h"
  std::vector<int> cells(static_cast<size_t>(30000), 0);
  size_t ptr{0};
  int ignore{0};
  const auto prog_size = std::strlen(prg);

  for (size_t i = 0; i < prog_size; i++) {
    switch (prg[i]) {
    case '>':
      ptr++;
      break;
    case '<':
      ptr--;
      break;
    case '+':
      cells[ptr]++;
      break;
    case '-':
      cells[ptr]--;
      break;
    case '.':
      std::putchar(cells[ptr]);
      break;
    case ',':
      cells[ptr] = std::getchar();
      break;
    case '[':
      if (!cells[ptr]) {
        for (ignore = 1, i++; ignore; ++i) {
          if (prg[i] == '[') {
            ignore++;
          } else if (prg[i] == ']') {
            ignore--;
          }
        }
        i--;
      }
      break;
    case ']':
      if (cells[ptr]) {
        for (ignore = 1, i--; ignore; --i) {
          if (prg[i] == ']') {
            ignore++;
          } else if (prg[i] == '[') {
            ignore--;
          }
        }
      }
      break;
    }
  }
  return 0;
}
