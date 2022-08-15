#include <cassert>
#include <cstdio>
#include <cstring>
#include <stack>
#include <vector>

int main(int, char **) {

#include "program.h"

  std::stack<size_t> stack;
  std::vector<int> cells(static_cast<size_t>(30000), 0);
  size_t ptr{0};
  int ignore = 0;

  for (size_t i = 0; i < std::strlen(prg); i++) {
    if (ignore && (prg[i] != ']' && prg[i] != '[')) {
      continue;
    }

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
      stack.push(i - 1);
      if (cells[ptr] == 0) {
        ignore++;
      }
      break;
    case ']':
      if (cells[ptr] != 0) {
        assert(stack.size() > 0);
        i = stack.top();
      }
      if (ignore) {
        ignore--;
      }
      stack.pop();
      break;
    }
  }

  return 0;
}
