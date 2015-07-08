//===-- ControlFlowAnalysis.h - ControlFlowAnalysis class definition -------*- C++ -*-===//
//
//                    StitchUp: Fault Tolerant High Level Synthesis 
//
// This file is distributed under the StitchUp 
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// This file contains the declaration of the ControlFlowAnalysis class, which is
/// used to determine the set of instructions that have an effect on control for
/// a particular function 
///
//===----------------------------------------------------------------------===//

#include <vector>
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"

using namespace llvm;

namespace StchUp {
	class ControlFlowAnalysis {
	public:
		ControlFlowAnalysis(Function *iF); //Default constructor	
		void findExitBlocks(void); //populates the exits vector

	private:
		std::vector<Value *> *CDS = new std::vector<Value *>; //CDS = Control Dependancy Set, the set of instructions which influence control 
		std::vector<BasicBlock *> *exits = new std::vector<BasicBlock *>;//Determine all the exit blocks for the input function 	
		Function *F; //function that is being analysed

	}; //class ControlFlowAnalysis 

	//---------------------------------------------------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------------------------------------------------
	ControlFlowAnalysis::ControlFlowAnalysis(Function *iF)	
	{
		F = iF;
	} //ControlFlowAnalysis

	//---------------------------------------------------------------------------------------------------------------------
	//Adds every BasicBlock that has a return instruction as it TerminatorInst
	//to the exits vector.
	//---------------------------------------------------------------------------------------------------------------------
	void ControlFlowAnalysis::findExitBlocks(void)
	{
		for(Function::iterator fs=F->begin(), fe=F->end(); fs != fe; ++fs)  
		{
		    BasicBlock *blk = fs;
		    TerminatorInst *TInst = blk->getTerminator();
		    if(isa<llvm::ReturnInst>(*TInst))
		    {
		        errs() << "exit@BB" << blk->getName() << "\n";
		        for(std::vector<BasicBlock *>::iterator it = exits->begin(); it != exits->end(); ++it){
		            BasicBlock *curr = *it;
		            if(curr->getName() == blk->getName()) { return; } //This exit block is already in the list
		        }
		        exits->push_back(blk);
		    }
		}
		return;		
	} //findExitBlocks


} //namespace StitchUp
