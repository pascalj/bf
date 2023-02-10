#include "llvm/IR/LLVMContext.h"
#include <filesystem>
#include <fstream>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <memory>
#include <vector>
#include "brainfuck.h"


std::vector<char> read_program(const std::string &path) {
  std::ifstream in("program.bf");
  std::vector<char> source;

  in.seekg(0, std::ios_base::end);
  std::streampos size = in.tellg();
  source.resize(size);
  in.seekg(0, std::ios_base::beg);
  in.read(source.data(), size);
  return source;
}


int main() {
  llvm::LLVMContext context;
  llvm::IRBuilder<> builder(context);
  std::unique_ptr<llvm::Module> module;
  // TODO: insert llvm::Function as parent for the BasicBlock
  auto block = llvm::BasicBlock::Create(context);
  builder.SetInsertPoint(block);

  auto stack_size = llvm::ConstantInt::get(builder.getInt8Ty(), 32 * 1000);
  auto mem_ptr = builder.CreateAlloca(builder.getPtrTy(), 0u, stack_size, "mem_ptr");
  builder.Insert(mem_ptr);

  auto source = read_program("program.bf");

  for(char token : source) {
   switch (token) {
    case '>':
      builder.CreateAdd(mem_ptr, builder.getInt8(1));
   }
  }

  module->print(llvm::errs(), nullptr);
}
