//===- LabelBB.cpp - Example code from "Writing an LLVM Pass" ---------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements two versions of the LLVM "LabelBB World" pass described
// in docs/WritingAnLLVMPass.html
//
//===----------------------------------------------------------------------===//

#include "llvm/ADT/Statistic.h"
#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
using namespace llvm;

namespace {
  struct LabelBB : public FunctionPass {
    static char ID; // Pass identification, replacement for typeid
    LabelBB() : FunctionPass(ID) {}

    bool runOnFunction(Function &F) override {
	for(Function::iterator i=F.begin(), e=F.end(); i != e; ++i)
	{
		BasicBlock *blk = i;
		blk->setName("BB");
	}
      return true;
    }

    bool doFinalization(Module &M) override {
	return true;
    } 

  };
}

char LabelBB::ID = 0;
static RegisterPass<LabelBB> X("LabelBB", "LabelBB World Pass");

