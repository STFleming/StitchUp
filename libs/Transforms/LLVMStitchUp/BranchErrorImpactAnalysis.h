//===-- BranchSlice.h - BranchSlice class definition -------*- C++ -*-===//
//
//                    StitchUp: Fault Tolerant High Level Synthesis 
//
// This file is distributed under the StitchUp 
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// This file contains the declaration of the BranchSlice class, which is
/// used to determine the set of instructions that have an effect on control for
/// a particular function 
///
//===----------------------------------------------------------------------===//

#ifndef BRANCHERRORIMPACTANALYSIS_LIB
#define BRANCHERRORIMPACTANALYSIS_LIB

#include <vector>
#include <sstream>
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/IR/CFG.h"

#include "CondBranchSet.h"

using namespace llvm;

namespace StchUp {
	class BranchSlice {
	public:
		BranchSlice(Function *iF, BranchInst *iB); //Default constructor	
		~BranchSlice();
		//void getAllConditionalBranches(); //Obtains all conditional branches within the program
		void slice(); //Returns the partial control replicant for this slice
		void sliceHelperAddOperands(bool &fixedpoint); //Checks the current slice and adds operands to it
		void sliceHelperSearchLoadStoreOperands(bool &fixedpoint); //Checks the current slice and adds common memory locations to it 
		void changeBranch(BranchInst *b);
		//Debug below
		void testPrint();
	private:
		Function *F; //LLVMfunction that is being analysed
		BranchInst *cB; //Current Conditional Branch that we want to compute a slice of
		CondBranchSet Bslice; //BEI = Branch Error Impact Set, the set of conditional Branches and associated CSIS 

	}; //class BranchSlice 

	void BranchSlice::testPrint()
	{
		errs() << *cB << "\t" << Bslice.size() << "\n";
	//	Bslice.print();
	}

	//Constructor
	BranchSlice::BranchSlice(Function *iF, BranchInst *iB)	
	{
		F = iF;
		if(iB->isConditional()){
			cB = iB;
			Value *cond = iB->getCondition();
			Bslice.add(cond);
			Bslice.add(cB);
			slice();
		} else {
			errs() << "Trying to compute slice of non-conditional branch\n";
		}
	}


//---------------------------------------------------------------------------------------------------------------------
// Determines the slice for the current branch 
//---------------------------------------------------------------------------------------------------------------------
	void BranchSlice::slice()
	{
		bool fixedpoint = false; 
		while(!fixedpoint){
			fixedpoint=true;
			sliceHelperAddOperands(fixedpoint);
			sliceHelperSearchLoadStoreOperands(fixedpoint);
		}
	}

	
//---------------------------------------------------------------------------------------------------------------------
// iterates across the slice and adds operands that depend on any instructions within it 
//---------------------------------------------------------------------------------------------------------------------
        void BranchSlice::sliceHelperAddOperands(bool &fixedpoint)
        {
            for(iterator s=Bslice.begin(), e=Bslice.end(); s != e; ++s)
            {
                Value * curr = *s;
                if(Instruction * inst = dyn_cast<Instruction>(curr))
                {
                        for(unsigned i=0; i<inst->getNumOperands(); i++)
                        {
                                Value *operand = inst->getOperand(i);
                                if(Bslice.addIfNotPresent(operand)) { fixedpoint = false; }
                        }
                }
            }
        }


//---------------------------------------------------------------------------------------------------------------------
// iterates over all Load instructions, detects if their operands are present in the Bslice and adds them if they are 
//---------------------------------------------------------------------------------------------------------------------
	void BranchSlice::sliceHelperSearchLoadStoreOperands(bool &fixedpoint) 
        {
                for(Function::iterator fs=F->begin(), fe=F->end(); fs != fe; ++fs)
                {
                        BasicBlock *blk = fs;
                        for(BasicBlock::iterator bs = blk->begin(), be = blk->end(); bs != be; ++bs)
                        {
                                Instruction *inst = bs;
                                if(!isa<TerminatorInst>(inst))
                                {
                                        if(StoreInst * storei = dyn_cast<StoreInst>(inst)) //store value ptr, we need to add ptr if value \in Bslice
                                        {
                                                Value *storeVal = storei->getValueOperand();
                                                Value *storePtr = storei->getPointerOperand();

                                                //if the memory location has been tagged then inst and op1 should be added 
                                                if(Bslice.checkMemLocation(storePtr))
                                                {
                                                        if(Bslice.addIfNotPresent(storeVal)) { fixedpoint = false; }
                                                        if(Bslice.addIfNotPresent(storei)) { fixedpoint = false; }
                                                }

                                                //Check to see if the whole array has been flagged.
                                                if(GetElementPtrInst *t = dyn_cast<GetElementPtrInst>(storePtr))
                                                {
                                                        if(Bslice.checkIfArrayExists(t))
                                                        {
                                                                if(Bslice.addIfNotPresent(storeVal)) { fixedpoint = false; }
                                                                if(Bslice.addIfNotPresent(storei)) { fixedpoint = false; }
                                                        }
                                                }
                                        }
                                        if(LoadInst * loadi= dyn_cast<LoadInst>(inst))
                                        {
                                                if(Bslice.checkIfExists(loadi))
                                                 {
                                                       if(Bslice.addIfNotPresent(loadi)) { fixedpoint = false; }
                                                       Value *loadPtr = loadi->getPointerOperand();
                                                       //We need to tag the corresponding memory location
                                                       if(Bslice.addMemIfNotPresent(loadPtr))
                                                        {
                                                                if(GetElementPtrInst * t = dyn_cast<GetElementPtrInst>(loadPtr))
                                                                {
                                                                        if(Bslice.addArrayRefIfNotPresent(t)) { fixedpoint = false; }
                                                                }
                                                                if(Bslice.addIfNotPresent(loadPtr)) { fixedpoint = false; }
                                                                fixedpoint = false;
                                                        }
                                                 }
                                        }
                                }
                        }
                }
        }




	//Destructor
	BranchSlice::~BranchSlice(){ }

} //namespace StitchUp

#endif
