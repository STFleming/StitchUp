#include <iostream>
#include <fstream>
#include "../Verilog/FiniteStateMachine.h"
#include "../Verilog/RTL.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/IR/Instructions.h"
#include <regex>

static cl::opt<string> 
OriginalSchedule("orig_sched", cl::desc("Specify the schedule of the original LegUp circuit for StitchUp to reference")
		 ,cl::value_desc("filename")); 

struct BBstates {
	unsigned stateCount;
	std::string stateRef;
};

typedef std::vector<BBstates>::iterator BBState_iterator;

 class LostStateInjector {
	public:
		LostStateInjector(Function *f, FiniteStateMachine *fsm_in);	
		~LostStateInjector();
		void populateOrigBBStateNum();
		void populateStitchupBBStateNum();
		void incrementStitchupBBStateNum(BasicBlock *blk);
		void locateLostStates();
		void injectLostStates();
		unsigned getLostStateCount(BasicBlock *blk);
		void removeLostState(BasicBlock *blk);
		void setFSM(FiniteStateMachine *in); 
		bool hasLostStates(std::string blkName);
		void addDummyInstToFirstBB();
		
		BBState_iterator orig_begin();
		BBState_iterator orig_end();
		BBState_iterator stitchup_begin();
		BBState_iterator stitchup_end();
		BBState_iterator lost_begin();
		BBState_iterator lost_end();

		//Debugging
		void print();
		 	
	private:
		std::vector<BBstates> origBBStateNum;
		std::vector<BBstates> stitchupBBStateNum;
		std::vector<BBstates> lostStates; //stateRef the BB we are refering to, stateCount = number of lost states
		FiniteStateMachine *fsm;
		Function *F;

 }; //namespace LostStateInjector

	BBState_iterator LostStateInjector::orig_begin() { return origBBStateNum.begin(); }
	BBState_iterator LostStateInjector::orig_end() { return origBBStateNum.end(); }
	BBState_iterator LostStateInjector::stitchup_begin() { return stitchupBBStateNum.begin(); }
	BBState_iterator LostStateInjector::stitchup_end() { return stitchupBBStateNum.end(); }
	BBState_iterator LostStateInjector::lost_begin() { return lostStates.begin(); }
	BBState_iterator LostStateInjector::lost_end() { return lostStates.end(); }

	LostStateInjector::LostStateInjector(Function *f, FiniteStateMachine* fsm_in)
	{
		fsm = fsm_in;
		F = f;
		populateOrigBBStateNum();
		populateStitchupBBStateNum();
		locateLostStates();
	}

	void LostStateInjector::setFSM(FiniteStateMachine *in) { fsm = in; return; }

	LostStateInjector::~LostStateInjector()
	{
	}	

	void LostStateInjector::populateOrigBBStateNum()
	{
		std::ifstream origSchedule(OriginalSchedule);
		if(!origSchedule.is_open()) { errs() << "Cannot open original schedule!\n"; assert(true);}	
		std::string line;				
		std::regex e ("Basic Block: %(BB\\d*) Num States: (\\d+)");
		std::smatch group;
		while(getline(origSchedule, line))
		{
			if(std::regex_search(line, group, e))
			{
				BBstates newBB;
				std::string t = group[2];
				newBB.stateCount = atoi(t.c_str());
				newBB.stateRef	= group[1];
				origBBStateNum.push_back(newBB);
			}	
		}
		origSchedule.close();
	}	
	
	void LostStateInjector::populateStitchupBBStateNum()
	{
		for(FiniteStateMachine::StateListType::iterator it=fsm->begin(), fin=fsm->end();it != fin; ++it)
		{
			State s = *it;
			BasicBlock *blk = s.getBasicBlock();
			if(blk!=NULL)
			{
				incrementStitchupBBStateNum(blk);
			}		
		}	
	}

	
	void LostStateInjector::incrementStitchupBBStateNum(BasicBlock *blk)
	{
		for(BBState_iterator it =stitchup_begin(), fin=stitchup_end(); it != fin; ++it)
		{
			if(blk->getName() == it->stateRef){	
				it->stateCount++;
				return;	
			}	
		} 
		//we need to add the entry
		BBstates newBB;
		newBB.stateCount = 1;
		newBB.stateRef = blk->getName();
		stitchupBBStateNum.push_back(newBB);	
		return;
	}


	void LostStateInjector::locateLostStates()
	{
		for(BBState_iterator it_su=stitchup_begin(), fin_su=stitchup_end(); it_su != fin_su; ++it_su)
		{
			BBstates su = *it_su;
			for(BBState_iterator it_o=orig_begin(), fin_o=orig_end(); it_o != fin_o; ++it_o)
			{
				BBstates orig = *it_o; 
				if(orig.stateRef == su.stateRef)
				{
					if(orig.stateCount != su.stateCount)
					{
						BBstates l;
						l.stateRef = orig.stateRef;
						l.stateCount = (orig.stateCount - su.stateCount);
						lostStates.push_back(l);	
					}
				}
			}
		}
	}

	unsigned LostStateInjector::getLostStateCount(BasicBlock *blk)
	{
		if(blk==NULL) { return 0; }
		for(BBState_iterator it=lost_begin(), fin=lost_end(); it != fin; ++it)
		{
			BBstates l = *it;
			if(blk->getName() == l.stateRef)
				return l.stateCount;
		}		
		return 0;
	}

	void LostStateInjector::removeLostState(BasicBlock *blk)
	{
		if(blk==NULL) { return; }
		for(BBState_iterator it=lost_begin(), fin=lost_end(); it != fin; ++it)
		{
			BBstates l = *it;
			if(blk->getName() == l.stateRef)
			{
				lostStates.erase(it);
				return;
			}
		}		
		return;
	}

	void LostStateInjector::injectLostStates()
	{
		for(FiniteStateMachine::StateListType::iterator it = fsm->begin(), fin=fsm->end(); it != fin; ++it)
		{
			State * s = it;
			if(s->isTerminating())
			{
				BasicBlock *blk = s->getBasicBlock();
				unsigned lostStateCount = getLostStateCount(blk);
				if(lostStateCount > 0)
				{
					//save the original transition
					State::Transition origTran = s->getTransition();
					State::Transition newTran;
					//Blank the current state transition
					s->setTerminating(false);
					s->setTransition(newTran);
					for(unsigned ls=0; ls < lostStateCount; ls++)
					{
						//Create the new state
						std::string LostStateName = "LOSTSTATE_" + blk->getName().str() 
									    + "_" + to_string(ls);	
						State *t;
						t = fsm->newState(s, LostStateName);
						t->setBasicBlock(blk);
					
						//Make the current state transition to it
						s->setDefaultTransition(t);
						//point to the newly created state
						s = t;
					}
					//give the original transition info to the last state added
					s->setTransition(origTran);
					s->setTerminating(true);
					removeLostState(blk);
				}	
				errs() << "\n";
			}
		}
	}


	bool LostStateInjector::hasLostStates(std::string blkName)
	{
		for(BBState_iterator it=lost_begin(), fin=lost_end(); it != fin; ++it)
		{
			BBstates t = *it;
			if(t.stateRef == blkName)
				return true;
		}
		return false;
	}

	void LostStateInjector::addDummyInstToFirstBB()
	{
        	for(Function::iterator it=F->begin(), fin=F->end(); it != fin; ++it)
        	{   
        	        BasicBlock *blk = it; 
        	        if(blk->getName() == "BB")
        	        {   
        	                AllocaInst* pa = new AllocaInst(Type::getInt32Ty(blk->getContext()), 0, "Dummy");
        	                TerminatorInst *T = blk->getTerminator(); 
        	                blk->getInstList().insert(T, pa);        
        	        }
        	} 
		return;
	}

	void LostStateInjector::print()
	{
		for(BBState_iterator it=stitchup_begin(), fin=stitchup_end(); it != fin; ++it)
		{
			BBstates t = *it;
			errs() << t.stateRef << "\t" << to_string(t.stateCount) << "\n"; 
		} 
	}

