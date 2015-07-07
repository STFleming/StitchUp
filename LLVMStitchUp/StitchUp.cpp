//===- StitchUp.cpp - Example code from "Writing an LLVM Pass" ---------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements two versions of the LLVM "StitchUp World" pass described
// in docs/WritingAnLLVMPass.html
//
//===----------------------------------------------------------------------===//

#include "llvm/ADT/Statistic.h"
#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
using namespace llvm;

#define DEBUG_TYPE "StitchUp"

STATISTIC(StitchUpCounter, "Counts number of functions greeted");

namespace {
  struct StitchUp : public FunctionPass {
    static char ID; // Pass identification, replacement for typeid
    StitchUp() : FunctionPass(ID) {}

    bool runOnFunction(Function &F) override {
      errs() << "I think we had better StitchUp this function: " << F.getName() << "\n";
      return false;
    }
  };
}

char StitchUp::ID = 0;
static RegisterPass<StitchUp> X("StitchUp", "StitchUp World Pass");

