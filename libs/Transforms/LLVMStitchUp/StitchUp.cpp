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
#include "ControlFlowAnalysis.h"
#include "BranchErrorImpactAnalysis.h"
using namespace llvm;

namespace {
  struct StitchUp : public FunctionPass {
    static char ID; // Pass identification, replacement for typeid
    StitchUp() : FunctionPass(ID) {}

    bool runOnFunction(Function &F) override {
		
	for(Function::iterator i=F.begin(), e=F.end(); i != e; ++i){
		BasicBlock *blk = i;
		TerminatorInst *TInst = blk->getTerminator();
		if(BranchInst *BInst = dyn_cast<BranchInst>(TInst)){
			if(BInst->isConditional()){
				StchUp::BranchSlice currBranch(&F,BInst);
				currBranch.testPrint();
			}	
		}
	}
	
	StchUp::ControlFlowAnalysis CFGbones(&F);	
	CFGbones.createControlShadow();

      return true;
    }

    bool doFinalization(Module &M) override {
	return true;
    } 

  };
}

char StitchUp::ID = 0;
static RegisterPass<StitchUp> X("StitchUp", "StitchUp LLVM frontend Pass");

