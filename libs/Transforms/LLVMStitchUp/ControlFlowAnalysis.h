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
#include <sstream>
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/IR/CFG.h"

#include "ControlDependenceSet.h"

using namespace llvm;

namespace StchUp {
	class ControlFlowAnalysis {
	public:
		ControlFlowAnalysis(Function *iF); //Default constructor	
		~ControlFlowAnalysis();
		void findExitBlocks(void); //populates the exits vector
		void BuildCDS(void);//builds the CDS, the set of instructions that effect control
		void BuildCDSHelperAddOperands(bool &fixedpoint); //iterates over the operands of CDS instructions and adds them to the CDS
		void BuildCDSHelperSearchLoadStoreOperands(bool &fixedpoint); //iterates over all Load instructions and adds them to the CDS if their operands are in the CDS 
		void BuildCDSHelperProcessBranch(BasicBlock *blk); //adds branch conditions to the CDS via ProcessCondition   
		void BuildCDSHelperProcessCondition(Value *cond);
		void BuildCDSHelperAddProcessBranches(); //Adds all branch conditions to the CDS
		void createControlShadow(void); //Removes all non CDS instructions leaving just hte control shadow behind
		void finalSanityCheck(void); //Performs final sanity checks on the produced code, called in the destructor
		//Debug below
		void labelBasicBlocks(); //Label all the BasicBLocks with a name
		void testPrint();
	private:
		Function *F; //LLVMfunction that is being analysed
		ControlDependenceSet CDS; //CDS = Control Dependency Set, the set of instructions which influence control 

	}; //class ControlFlowAnalysis 

	void ControlFlowAnalysis::testPrint()
	{
		CDS.print();
	}

	//Constructor
	ControlFlowAnalysis::ControlFlowAnalysis(Function *iF)	
	{
		F = iF;
		BuildCDS();
	}

	//Destructor
	ControlFlowAnalysis::~ControlFlowAnalysis()
	{
		finalSanityCheck();
	}


//---------------------------------------------------------------------------------------------------------------------
//TODO: Remove, used for debugging purposes
	void ControlFlowAnalysis::labelBasicBlocks()
	{
		for(Function::iterator fs=F->begin(), fe=F->end(); fs != fe; ++fs)
		{
			BasicBlock *blk = fs;
			blk->setName("BB");
		}
	}

//---------------------------------------------------------------------------------------------------------------------
// Keeps iterating over the Basic Blocks in the program removing instructions that are 
// not present in the CDS, it is a fixed point algorithm due to the restriction that
// to remove instructions safely they must not be used anywhere. 
//---------------------------------------------------------------------------------------------------------------------
	void ControlFlowAnalysis::createControlShadow(){

		ControlDependenceSet nonCDS;

		for(Function::iterator fs=F->begin(), fe=F->end(); fs != fe; ++fs){
			BasicBlock *blk = fs;
			for(BasicBlock::iterator bs=blk->begin(), be=blk->end(); bs != be; ++bs) {
				Instruction *inst = bs;	
				if(!isa<TerminatorInst>(inst))
				{
					if(!CDS.checkIfExists(inst))
						nonCDS.add(inst);
				}
			}
		}
		//Now iterate through and delete the nonCDS instrustions, make sure we do it from the end of use_def chain
		for(cds_iterator item = nonCDS.begin(), end=nonCDS.end(); item != end; ++item)
		{
			Value *curr = *item;
			Value *blank = UndefValue::get(curr->getType());
			if(Instruction *inst = dyn_cast<Instruction>(curr))
			{
				inst->replaceAllUsesWith(blank);
				inst->eraseFromParent();
			}
		}
	}


//---------------------------------------------------------------------------------------------------------------------
//Helper function which process the conditional instruction from a
//branch or select instruction and adds it to the CDS
//---------------------------------------------------------------------------------------------------------------------
	void ControlFlowAnalysis::BuildCDSHelperProcessCondition(Value *cond)
	{
		if(Instruction *ICond = dyn_cast<Instruction>(cond)) {
			if(!isa<Constant>(ICond))
			{
			    	CDS.addIfNotPresent(ICond);
				for(unsigned i=0; i<ICond->getNumOperands(); i++)
				{
				    Value *o = ICond->getOperand(i);
				    if(!(isa<Constant>(o))) 
					CDS.addIfNotPresent(o);
				}
			}
		}
		else {
		    errs() << "Error: Condition casting to instruction failed!\n";
		}   
	}

//---------------------------------------------------------------------------------------------------------------------
// Helper function for building the CDS
// Tests to see if the exit from a basic block is a branch, if it is a conditional branch it determines the
//---------------------------------------------------------------------------------------------------------------------
	void ControlFlowAnalysis::BuildCDSHelperProcessBranch(BasicBlock *blk)   
	{
	    TerminatorInst *TInst = blk->getTerminator();
	    if(BranchInst *BInst = dyn_cast<BranchInst>(TInst))
	    {
	        if(BInst->isConditional())
	        {
	            Value * cond = BInst->getCondition();
		    BuildCDSHelperProcessCondition(cond);
	        }   
	    }   
	    else
	    {
		if(SwitchInst *SInst = dyn_cast<SwitchInst>(TInst))
		{
		   Value *cond = SInst->getCondition();
		   BuildCDSHelperProcessCondition(cond);
		}
	    }
	    return;
	} 

	
//---------------------------------------------------------------------------------------------------------------------
//Adds all SSA that branches are conditional on to the CDS
//this is typically the first step in the analysis.
//---------------------------------------------------------------------------------------------------------------------
	void ControlFlowAnalysis::BuildCDSHelperAddProcessBranches()
	{
		for(Function::iterator fs=F->begin(), fe=F->end(); fs != fe; ++fs)
		{
			BasicBlock *blk = fs; 	
			BuildCDSHelperProcessBranch(blk);
		} 
		return;
	}

//---------------------------------------------------------------------------------------------------------------------
//Builds the control dependency sets (CDS)
//---------------------------------------------------------------------------------------------------------------------
	void ControlFlowAnalysis::BuildCDS()
	{
		BuildCDSHelperAddProcessBranches();
		bool fixedpoint = false;
		while(!fixedpoint){
			fixedpoint=true;
			BuildCDSHelperAddOperands(fixedpoint); 
			BuildCDSHelperSearchLoadStoreOperands(fixedpoint);
		}
		return;
	}   

//---------------------------------------------------------------------------------------------------------------------
// iterates over all Load instructions, detects if their operands are present in the CDS and adds them if they are 
//---------------------------------------------------------------------------------------------------------------------
	void ControlFlowAnalysis::BuildCDSHelperSearchLoadStoreOperands(bool &fixedpoint)
	{
		for(Function::iterator fs=F->begin(), fe=F->end(); fs != fe; ++fs)
		{
			BasicBlock *blk = fs;
			for(BasicBlock::iterator bs = blk->begin(), be = blk->end(); bs != be; ++bs)
			{
				Instruction *inst = bs;
				if(!isa<TerminatorInst>(inst))
				{
					if(StoreInst * storei = dyn_cast<StoreInst>(inst)) //store value ptr, we need to add ptr if value \in CDS
					{
						Value *storeVal = storei->getValueOperand(); 
						Value *storePtr = storei->getPointerOperand(); 
						
						//if the memory location has been tagged then inst and op1 should be added 
						if(CDS.checkMemLocation(storePtr))
						{
							if(CDS.addIfNotPresent(storeVal)) { fixedpoint = false; }
							if(CDS.addIfNotPresent(storei)) { fixedpoint = false; }	
						}

						//Check to see if the whole array has been flagged.
						if(GetElementPtrInst *t = dyn_cast<GetElementPtrInst>(storePtr))
						{
							if(CDS.checkIfArrayExists(t))
							{
								if(CDS.addIfNotPresent(storeVal)) { fixedpoint = false; }
								if(CDS.addIfNotPresent(storei)) { fixedpoint = false; }	
							}	
						}
					}
					if(LoadInst * loadi= dyn_cast<LoadInst>(inst))
					{
						if(CDS.checkIfExists(loadi))
						 {
						       if(CDS.addIfNotPresent(loadi)) { fixedpoint = false; }
						       Value *loadPtr = loadi->getPointerOperand();
						       //We need to tag the corresponding memory location
						       if(CDS.addMemIfNotPresent(loadPtr))
							{
								if(GetElementPtrInst * t = dyn_cast<GetElementPtrInst>(loadPtr))
								{
									if(CDS.addArrayRefIfNotPresent(t)) { fixedpoint = false; }
								}
								if(CDS.addIfNotPresent(loadPtr)) { fixedpoint = false; }
							  	fixedpoint = false;
							}
						 }
					}
				}
			}
		}
	}

//---------------------------------------------------------------------------------------------------------------------
// iterates across the CDS and adds operands that depend on instructions in the CDS 
//---------------------------------------------------------------------------------------------------------------------
	void ControlFlowAnalysis::BuildCDSHelperAddOperands(bool &fixedpoint)
	{
	    for(cds_iterator s=CDS.begin(), e=CDS.end(); s != e; ++s)
	    {
	        Value * curr = *s;
		if(Instruction * inst = dyn_cast<Instruction>(curr))
		{
			for(unsigned i=0; i<inst->getNumOperands(); i++)
			{	
				Value *operand = inst->getOperand(i);
				if(CDS.addIfNotPresent(operand)) { fixedpoint = false; }
			}
		}	
	    }	
	}

//---------------------------------------------------------------------------------------------------------------------
// Checks for Undefs in the operands and other signifiers that something has gone wrong
//---------------------------------------------------------------------------------------------------------------------
void ControlFlowAnalysis::finalSanityCheck()
{
	for(Function::iterator fiter=F->begin(), fend=F->end(); fiter != fend; ++fiter)
	{
		BasicBlock *blk = fiter;
		for(BasicBlock::iterator bs=blk->begin(), be=blk->end(); bs != be; ++bs)
		{
			Instruction *inst = bs;
			if(!isa<TerminatorInst>(inst)){
				for(unsigned i=0; i<inst->getNumOperands(); i++)
				{
				    Value *o = inst->getOperand(i);
				    if(isa<UndefValue>(o))
				    {
					errs() << "SANITY CHECK FAILURE: instruction  " << *inst << "   has an undef Op\n";
				    	assert(!isa<UndefValue>(o));
				    }
				}
			}	
		}
	}
} 

} //namespace StitchUp
