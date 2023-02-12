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
  auto FT = llvm::FunctionType::get(builder->getInt8Ty(), no_args, false);
  auto F  = llvm::Function::Create(FT, llvm::Function::ExternalLinkage, "main",
                                   module.get());
  auto block = llvm::BasicBlock::Create(*context, "entry", F);
  builder->SetInsertPoint(block);

  static const int stack_size = 32000;

  std::vector<llvm::Type *> arg_types;
  arg_types.push_back(builder->getInt64Ty());
  arg_types.push_back(builder->getInt64Ty());

  auto calloc_type =
      llvm::FunctionType::get(builder->getPtrTy(), arg_types, false);
  auto calloc = module->getOrInsertFunction("calloc", calloc_type);

  auto stack_size_const = builder->getInt32(stack_size);
  auto char_size        = builder->getInt32(1);

  std::vector<llvm::Value *> args       = {stack_size_const, char_size};
  auto                       stack_base = builder->CreateCall(calloc, args);

  auto stack_ptr =
      builder->CreateAlloca(builder->getInt8PtrTy(), 0u, "stack_ptr");

  builder->CreateStore(stack_ptr, stack_base);

  auto source = read_program("program.bf");

  for (char token : source) {
    switch (token) {
    case '>': {
      auto inc_gep = builder->CreateGEP(builder->getInt8PtrTy(), stack_ptr,
                                        builder->getInt8(1));
      builder->CreateStore(inc_gep, stack_ptr);
    } break;
    case '<': {
      auto dec_gep = builder->CreateGEP(builder->getInt8PtrTy(), stack_ptr,
                                        builder->getInt8(-1));
      builder->CreateStore(dec_gep, stack_ptr);
    } break;
    case '-': {
      auto sub_val = builder->CreateLoad(builder->getInt8Ty(), stack_ptr);
      builder->CreateSub(sub_val, builder->getInt8(1));
      builder->CreateStore(sub_val, stack_ptr);
    } break;
    case '+': {
      auto add_val = builder->CreateLoad(builder->getInt8Ty(), stack_ptr);
      builder->CreateAdd(add_val, builder->getInt8(1));
      builder->CreateStore(add_val, stack_ptr);
    } break;
    case '.': {
      auto char_val = builder->CreateLoad(builder->getInt8Ty(), stack_ptr);
      auto char_int_val =
          builder->CreateIntCast(char_val, builder->getInt32Ty(), true);

      std::vector<llvm::Type *> putchar_atypes = {builder->getInt32Ty()};
      auto                      putchar_type =
          llvm::FunctionType::get(builder->getVoidTy(), putchar_atypes, false);
      std::vector<llvm::Value *> putchar_args = {char_int_val};
      auto                       putchar =
          module->getOrInsertFunction("putchar", putchar_type);
      builder->CreateCall(putchar, putchar_args);

    } break;
    }
  }

  builder->CreateRet(builder->getInt8(1));

  llvm::verifyFunction(*F);

  module->print(llvm::outs(), nullptr);
}
