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
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/CFG.h"

using namespace llvm;

namespace StchUp {
	class ControlFlowAnalysis {
	public:
		ControlFlowAnalysis(Function *iF); //Default constructor	
		~ControlFlowAnalysis();
		void findExitBlocks(void); //populates the exits vector
		void BuildCDS(void);//builds the CDS, the set of instructions that effect control
		void BuildCDSHelper(BasicBlock * blk); //Main helper function for building the CDS 
		void BuildCDSHelperDependancyFillIn(BasicBlock *blk); //Helper function which adds updates the CDS deps with new deps
		void BuildCDSHelperBranchCondition(BasicBlock *blk);   
		bool BuildCDSHelperCheckIfExists(Value *item); //Checks if an Instruction is present in the CDS
		bool BuildCDSVisitedBB(BasicBlock *blk); //Returns if the BasicBlock has already been visited
		void printCDS(); //Temporary debug, prints the CDS
	private:
		std::vector<Value *> *CDS = new std::vector<Value *>; //CDS = Control Dependancy Set, the set of instructions which influence control 
		std::vector<BasicBlock *> *exits = new std::vector<BasicBlock *>;//Determine all the exit blocks for the input function 	
		std::vector<BasicBlock *> *visited = new std::vector<BasicBlock *>;
		Function *F; //function that is being analysed

	}; //class ControlFlowAnalysis 

	//---------------------------------------------------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------------------------------------------------
	ControlFlowAnalysis::ControlFlowAnalysis(Function *iF)	
	{
		F = iF;
		findExitBlocks();
		BuildCDS();
	} //ControlFlowAnalysis

	//---------------------------------------------------------------------------------------------------------------------
	//Destructor
	//---------------------------------------------------------------------------------------------------------------------
	ControlFlowAnalysis::~ControlFlowAnalysis()
	{
		delete CDS;
		delete exits;
		delete visited;
	}


	//---------------------------------------------------------------------------------------------------------------------
	//TODO: Remove, used for debugging purposes
	void ControlFlowAnalysis::printCDS()
	{
		errs() << "{";                                
		for(std::vector<Value *>::iterator s = CDS->begin(), e=CDS->end(); s != e; ++s)
		{
		    Value *t = *s;
		    errs() << *t << "  ";
		}
		errs() << "}\n";	
	}

	//---------------------------------------------------------------------------------------------------------------------
	// Checks to see if a BasicBlock has already been analysed and added to the visited list.
	// (TODO: Change the way that blocks are comapred, this relies on names again...)
	//---------------------------------------------------------------------------------------------------------------------
	bool ControlFlowAnalysis::BuildCDSVisitedBB(BasicBlock *blk)
	{ 
	    for(std::vector<BasicBlock *>::iterator it = visited->begin(); it != visited->end(); ++it){
	        BasicBlock *curr = *it;
	        if(blk == curr){return true;} //Yes, we have already seen this node and these branches are related to a loop
	    }
	    return false;
	}


	//---------------------------------------------------------------------------------------------------------------------
	// Helper function for building the CDS, checks to see if an item exists in the CDS
	// (TODO: This should be changed so that if does not check the name of the Instruction, but compares the objects directly)
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
	// Helper function for building the CDS
	// Tests to see if the exit from a basic block is a branch, if it is a conditional branch it determines the
	// instructions that imapct on the condition and adds them to the CDS. (This needs to be upgraded so that it
	// actually adds the branch instructions to the CDS as well)
	//---------------------------------------------------------------------------------------------------------------------
	void ControlFlowAnalysis::BuildCDSHelperBranchCondition(BasicBlock *blk)   
	{
	    TerminatorInst *TInst = blk->getTerminator();
	    if(BranchInst *BInst = dyn_cast<BranchInst>(TInst))
	    {
	        if(BInst->isConditional())
	        {
	            Value * cond = BInst->getCondition();
	            if(Instruction *ICond = dyn_cast<Instruction>(cond)) {
	            if(!isa<Constant>(ICond))
	            {
	                CDS->push_back(ICond);
	            }
	            for(unsigned i=0; i<ICond->getNumOperands(); i++)
	            {
	                Value *o = ICond->getOperand(i);
	                if(!(isa<Constant>(o))) 
	                {
	                    CDS->push_back(o);
	                }
	            }
	            }
	            else {
	                errs() << "Error: Condition casting to instruction failed!\n";
	            }   
	        }   
	    }   
	    return;
	} 

	//---------------------------------------------------------------------------------------------------------------------
	// Helper function which iterates through the CDS and adds any instructions that
	// is dependent on an instruction already present in the CDS
	//---------------------------------------------------------------------------------------------------------------------
	void ControlFlowAnalysis::BuildCDSHelperDependancyFillIn(BasicBlock *blk)
	{
	    for(BasicBlock::iterator bs=blk->begin(), be=blk->end(); bs != be; ++bs)
	    {
	        Instruction *c = bs;
	        bool fp=false;
	        while(!fp)
	        {
	            fp=true;
	            for(std::vector<Value *>::iterator cds_s=CDS->begin(), cds_e=CDS->end(); cds_s != cds_e; ++cds_s)
	            {
	                Value *d = *cds_s;
	                if(c == d)
	                {
	                    //we have a match, we need to add the arguments to the CDS (and check if they are already in there)
	                    for(unsigned i=0; i<c->getNumOperands(); i++)
	                    {
	                        Value *o = c->getOperand(i);
	                        if(!(isa<Constant>(o))) //Make sure it is not a constant!
	                        {
	                            if(!BuildCDSHelperCheckIfExists(o))
	                            {
	                                fp=false;
	                                CDS->push_back(o);
	                            }
	                        }
	                    }
	                }
	            }
	        }
	    }
	    return;
	}            

	//---------------------------------------------------------------------------------------------------------------------
	// Main helper function for building the CDS
	// It automatically adds the branch condition of the current basic block to the set, then it 
	// moves through all the instructions in the BB and finds any dependancies, adding them also to the set
	// then it calls over all predecessor basic blocks. 
	//---------------------------------------------------------------------------------------------------------------------
	void ControlFlowAnalysis::BuildCDSHelper(BasicBlock * blk) 
	{
	    visited->push_back(blk); //Mark that we have visited this location (infinite loop prevention)
	    BuildCDSHelperBranchCondition(blk);
	    BuildCDSHelperDependancyFillIn(blk);
	    for(pred_iterator PI=pred_begin(blk), E=pred_end(blk); PI != E; ++PI)
	    {
	        BasicBlock *Pred = *PI;
	        if(!BuildCDSVisitedBB(Pred))
	        {
	            BuildCDSHelper(Pred);
	        }
	    }
	    return;
	}

	//---------------------------------------------------------------------------------------------------------------------
	//Builds the control dependency sets (CDS)
	//---------------------------------------------------------------------------------------------------------------------
	void ControlFlowAnalysis::BuildCDS()                                                                                                                                                                  
	{
	    for(std::vector<BasicBlock *>::iterator it = exits->begin(); it != exits->end(); ++it){
	        BasicBlock *curr = *it;
	        BuildCDSHelper(curr);
	        visited->clear();
	    }
	}

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
