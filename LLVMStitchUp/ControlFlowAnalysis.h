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
		void BuildCDSHelperDependencyFillIn(BasicBlock *blk, bool &fixedpoint); //Helper function which adds updates the CDS deps with new deps
		void BuildCDSHelperProcessBranch(BasicBlock *blk); //adds branch conditions to the CDS via ProcessCondition   
		void BuildCDSHelperProcessCondition(Value *cond);
		void BuildCDSHelperAddProcessBranches(); //Adds all branch conditions to the CDS
		bool BuildCDSHelperCheckIfExists(Value *item); //Checks if an Instruction is present in the CDS
		void createControlShadow(void); //Removes all non CDS instructions leaving just hte control shadow behind
		//Debug below
		void labelBasicBlocks(); //Label all the BasicBLocks with a name
		void testPrint();
	private:
		std::vector<Value *> *CDS; //CDS = Control Dependency Set, the set of instructions which influence control 
		Function *F; //LLVMfunction that is being analysed
		ControlDependenceSet test;

	}; //class ControlFlowAnalysis 

	void ControlFlowAnalysis::testPrint()
	{
		for(std::vector<Value *>::iterator cds_s = CDS->begin(), cds_e = CDS->end(); cds_s != cds_e; ++cds_s)
		{
			Value *item = *cds_s;
			test.add(item); 
		}
		test.print();
	}



	//Constructor
	ControlFlowAnalysis::ControlFlowAnalysis(Function *iF)	
	{
		CDS = new std::vector<Value *>;  
		F = iF;
		BuildCDS();
	}

	//Destructor
	ControlFlowAnalysis::~ControlFlowAnalysis()
	{
		delete CDS;
	}

	//---------------------------------------------------------------------------------------------------------------------
	// Keeps iterating over the Basic Blocks in the program removing instructions that are 
	// not present in the CDS, it is a fixed point algorithm due to the restriction that
	// to remove instructions safely they must not be used anywhere. 
	//---------------------------------------------------------------------------------------------------------------------
	void ControlFlowAnalysis::createControlShadow(){

		std::vector<Value *>* nonCDS = new std::vector<Value *>;

		for(Function::iterator fs=F->begin(), fe=F->end(); fs != fe; ++fs){
			BasicBlock *blk = fs;
			for(BasicBlock::iterator bs=blk->begin(), be=blk->end(); bs != be; ++bs) {
				Instruction *inst = bs;	
				if(!isa<TerminatorInst>(inst))
				{
					if(!BuildCDSHelperCheckIfExists(inst))
					{
						nonCDS->push_back(inst);
					}
				}
			}
		}
		
		//Now iterate through and delete the nonCDS instrustions, make use we do it from the end of the use_def chain
		for(std::vector<Value *>::iterator item = nonCDS->begin(), end=nonCDS->end();
			item != end; ++item)
		{
			Value *curr = *item;
			Value *blank = UndefValue::get(curr->getType());
			curr->replaceAllUsesWith(blank);
			if(Instruction *inst = dyn_cast<Instruction>(curr))
				inst->eraseFromParent();
		}
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
	// Helper function for building the CDS, checks to see if an item exists in the CDS
	//---------------------------------------------------------------------------------------------------------------------
	bool ControlFlowAnalysis::BuildCDSHelperCheckIfExists(Value *item)
	{
	    for(std::vector<Value *>::iterator s=CDS->begin(), e=CDS->end(); s != e; ++s)
	    {
	        Value * curr = *s;
	        if(curr == item){return true;}
	    }
	    return false;
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
		    if(!BuildCDSHelperCheckIfExists(ICond))
		    {	
		    	CDS->push_back(ICond);
		    }
		}
		for(unsigned i=0; i<ICond->getNumOperands(); i++)
		{
		    Value *o = ICond->getOperand(i);
		    if(!(isa<Constant>(o))) 
		    {
		       if(!BuildCDSHelperCheckIfExists(o))
		        {
		    	CDS->push_back(o);
		        }
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
	// Helper function which iterates through the CDS and adds any instructions that
	// is dependent on an instruction already present in the CDS
	//---------------------------------------------------------------------------------------------------------------------
	void ControlFlowAnalysis::BuildCDSHelperDependencyFillIn(BasicBlock *blk, bool &fixedpoint)
	{
	    for(BasicBlock::iterator bs=blk->begin(), be=blk->end(); bs != be; ++bs)
	    {
	        Instruction *c = bs;
	            for(std::vector<Value *>::iterator cds_s=CDS->begin(), cds_e=CDS->end(); cds_s != cds_e; ++cds_s)
	            {
	                Value *d = *cds_s;
	                if(c == d)
	                {
	                    //we have a match, we need to add the arguments to the CDS (and check if they are already in there)
	                    for(unsigned i=0; i<c->getNumOperands(); i++)
	                    {
	                        Value *o = c->getOperand(i);
	                        if(!(isa<Constant>(o))) 
	                        {
	                            if(!BuildCDSHelperCheckIfExists(o))
	                            {
	                                fixedpoint=false;
	                                CDS->push_back(o);
	                            }
	                        }
	                    }
	                }
			else
			{
				errs() << *d << " /=" << *c <<"\n";
			}
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
					if(isa<StoreInst>(inst)) //store value ptr, we need to add ptr if value \in CDS
					{
						Value *op1 = inst->getOperand(0); //Get the value to be stored in the ptr
						Value *op2 = inst->getOperand(1); 
						if(BuildCDSHelperCheckIfExists(op1))
						{
							if(!BuildCDSHelperCheckIfExists(inst))
							{
								CDS->push_back(op2); //add ptr
								fixedpoint = false;
							}
							if(!BuildCDSHelperCheckIfExists(inst))
							{
								CDS->push_back(inst);
								fixedpoint = false;
							}
						}
					}
					if(isa<LoadInst>(inst))
					{
					 Value *op1 = inst->getOperand(0); //Get the value to be stored in the ptr
					 if(BuildCDSHelperCheckIfExists(op1))
					  {
					   if(!BuildCDSHelperCheckIfExists(op1)){CDS->push_back(op1); fixedpoint=false;}
					   if(!BuildCDSHelperCheckIfExists(inst)){CDS->push_back(inst); fixedpoint=false;}
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
	    for(std::vector<Value *>::iterator s=CDS->begin(), e=CDS->end(); s != e; ++s)
	    {
	        Value * curr = *s;
		if(Instruction * inst = dyn_cast<Instruction>(curr))
		{
			for(User::op_iterator os = inst->op_begin(), oe = inst->op_end(); os != oe; ++os)
			{	
				Value *operand = *os;
				if(!BuildCDSHelperCheckIfExists(operand))
				{
					CDS->push_back(operand);
					fixedpoint=false;
				}
			}
		}	
	    }	
	}


} //namespace StitchUp
