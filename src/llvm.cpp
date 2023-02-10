#include "brainfuck.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Verifier.h"
#include <filesystem>
#include <fstream>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <memory>
#include <vector>

std::vector<char> read_program(const std::string &path) {
  std::ifstream     in("program.bf");
  std::vector<char> source;

  in.seekg(0, std::ios_base::end);
  std::streampos size = in.tellg();
  source.resize(size);
  in.seekg(0, std::ios_base::beg);
  in.read(source.data(), size);
  return source;
}

int main() {
  auto context = std::make_unique<llvm::LLVMContext>();
  auto builder = std::make_unique<llvm::IRBuilder<>>(*context);
  auto module  = std::make_unique<llvm::Module>("brainfuck", *context);

  std::vector<llvm::Type *> no_args{};
  auto FT    = llvm::FunctionType::get(builder->getInt8Ty(), no_args, false);
  auto F     = llvm::Function::Create(FT, llvm::Function::ExternalLinkage, "bf",
                                      module.get());
  auto block = llvm::BasicBlock::Create(*context, "entry", F);
  builder->SetInsertPoint(block);

  auto stack_size = builder->getInt32(32 * 1000);
  auto mem_ptr =
      builder->CreateAlloca(builder->getPtrTy(), 0u, stack_size, "mem_ptr");

  auto source = read_program("program.bf");

  for (char token : source) {
    switch (token) {
    case '>':
      builder->CreateAdd(mem_ptr, builder->getInt8(1));
      break;
    case '<':
      builder->CreateSub(mem_ptr, builder->getInt8(1));
      break;
    case '-':
      {
        auto sub_val = builder->CreateLoad(builder->getInt8Ty(), mem_ptr);
        builder->CreateSub(sub_val, builder->getInt8(1));
        builder->CreateStore(sub_val, mem_ptr);
      }
      break;
    case '+':
      {
        auto add_val = builder->CreateLoad(builder->getInt8Ty(), mem_ptr);
        builder->CreateAdd(add_val, builder->getInt8(1));
        builder->CreateStore(add_val, mem_ptr);
      }
      break;
    }
  }

  builder->CreateRet(builder->getInt8(1));

  llvm::verifyFunction(*F);

  module->print(llvm::outs(), nullptr);
}
