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
		void BuildCDSHelper(BasicBlock * blk, unsigned &namecount); //Main helper function for building the CDS 
		void BuildCDSHelperDependancyFillIn(BasicBlock *blk, unsigned &namecount); //Helper function which adds updates the CDS deps with new deps
		void BuildCDSHelperBranchCondition(BasicBlock *blk, unsigned &namecount);   
		bool BuildCDSHelperCheckIfExists(Value *item); //Checks if an Instruction is present in the CDS
		bool BuildCDSVisitedBB(BasicBlock *blk); //Returns if the BasicBlock has already been visited
		void EnumerateBlocks(); //temporary function for giving a name to all the basic blocks     
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
		EnumerateBlocks();
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

	//Gives each of the Basic Blocks a unique name
	void ControlFlowAnalysis::EnumerateBlocks()     
	{
	    unsigned counter=0;
	    std::stringstream ss;
	    std::string blockname;
	    for (Function::iterator fs=F->begin(), fe=F->end(); fs != fe; ++fs)
	    {
	        ss << counter;
	        std::string blockname = ss.str();
	        fs->setName(blockname);
	        counter++;
	        ss.str(std::string());
	    }
	    return;
	}

	//---------------------------------------------------------------------------------------------------------------------
	// Checks to see if a BasicBlock has already been analysed and added to the visited list.
	// (TODO: Change the way that blocks are comapred, this relies on names again...)
	//---------------------------------------------------------------------------------------------------------------------
	bool ControlFlowAnalysis::BuildCDSVisitedBB(BasicBlock *blk)
	{ 
	    for(std::vector<BasicBlock *>::iterator it = visited->begin(); it != visited->end(); ++it){
	        BasicBlock *curr = *it;
	        if(blk->getName() == curr->getName()){return true;} //Yes, we have already seen this node and these branches are related to a loop
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
	        if(curr->getName() == item->getName()){return true;}
	    }
	    return false;
	}

	//---------------------------------------------------------------------------------------------------------------------
	// Helper function for building the CDS
	// Tests to see if the exit from a basic block is a branch, if it is a conditional branch it determines the
	// instructions that imapct on the condition and adds them to the CDS. (This needs to be upgraded so that it
	// actually adds the branch instructions to the CDS as well)
	//---------------------------------------------------------------------------------------------------------------------
	void ControlFlowAnalysis::BuildCDSHelperBranchCondition(BasicBlock *blk, unsigned &namecount)   
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
	                if(!ICond->hasName())
	                {
	                    std::stringstream OperationName;
	                    OperationName << "C" << namecount++;
	                    ICond->setName(OperationName.str());
	                }
	                CDS->push_back(ICond);
	            }
	            for(unsigned i=0; i<ICond->getNumOperands(); i++)
	            {
	                Value *o = ICond->getOperand(i);
	                if(!(isa<Constant>(o))) 
	                {
	                    if(!o->hasName()) //if it doesn't have a name give it one
	                    {
	                        std::stringstream operandName;
	                        operandName << "C" << namecount++;
	                        o->setName(operandName.str());
	                    }
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
	void ControlFlowAnalysis::BuildCDSHelperDependancyFillIn(BasicBlock *blk, unsigned &namecount)
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
	                if(c->getName() == d->getName())
	                {
	                    //we have a match, we need to add the arguments to the CDS (and check if they are already in there)
	                    for(unsigned i=0; i<c->getNumOperands(); i++)
	                    {
	                        Value *o = c->getOperand(i);
	                        if(!(isa<Constant>(o))) //Make sure it is not a constant!
	                        {
	                            if(!o->hasName()) //if it doesn't have a name give it one
	                            {
	                                std::stringstream operandName;
	                                operandName << "C" << namecount++;
	                                o->setName(operandName.str());
	                            }
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
	void ControlFlowAnalysis::BuildCDSHelper(BasicBlock * blk, unsigned &namecount) 
	{
	    visited->push_back(blk); //Mark that we have visited this location (infinite loop prevention)
	    BuildCDSHelperBranchCondition(blk, namecount);
	    BuildCDSHelperDependancyFillIn(blk, namecount);
	    for(pred_iterator PI=pred_begin(blk), E=pred_end(blk); PI != E; ++PI)
	    {
	        BasicBlock *Pred = *PI;
	        if(!BuildCDSVisitedBB(Pred))
	        {
	            BuildCDSHelper(Pred, namecount);
	        }
	    }
	    return;
	}

	//---------------------------------------------------------------------------------------------------------------------
	//Builds the control dependency sets (CDS)
	//---------------------------------------------------------------------------------------------------------------------
	void ControlFlowAnalysis::BuildCDS()                                                                                                                                                                  
	{
	    unsigned namecount =0;
	    for(std::vector<BasicBlock *>::iterator it = exits->begin(); it != exits->end(); ++it){
	        BasicBlock *curr = *it;
	        BuildCDSHelper(curr, namecount);
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
