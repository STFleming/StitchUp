#include <iostream>
#include <fstream>
#include "../Verilog/FiniteStateMachine.h"
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

namespace stchup {
 class LostStateInjector {
	public:
		LostStateInjector(FiniteStateMachine *fsm);	
		~LostStateInjector();
		void populateOrigBBStateNum();
		void populateStitchupBBStateNum();
		void incrementStitchupBBStateNum(BasicBlock *blk);
		void locateLostStates();
		BBState_iterator orig_begin();
		BBState_iterator orig_end();
		BBState_iterator stitchup_begin();
		BBState_iterator stitchup_end();
		void print();
		 	
	private:
		std::vector<BBstates> origBBStateNum;
		std::vector<BBstates> stitchupBBStateNum;
		FiniteStateMachine *fsm;

 }; //namespace LostStateInjector

	BBState_iterator LostStateInjector::orig_begin()
	{	return origBBStateNum.begin();		}

	BBState_iterator LostStateInjector::orig_end()
	{	return origBBStateNum.end();		}

	BBState_iterator LostStateInjector::stitchup_begin()
	{	return stitchupBBStateNum.begin();		}

	BBState_iterator LostStateInjector::stitchup_end()
	{	return stitchupBBStateNum.end();		}

	LostStateInjector::LostStateInjector(FiniteStateMachine *in)
	{
		fsm = in;
		populateOrigBBStateNum();
		populateStitchupBBStateNum();
	}

	LostStateInjector::~LostStateInjector()
	{
		locateLostStates();
	}	

	void LostStateInjector::populateOrigBBStateNum()
	{
		std::ifstream origSchedule(OriginalSchedule);
		if(!origSchedule.is_open()) { errs() << "Cannot open original schedule!\n"; assert(true);}	
		std::string line;				
		std::regex e ("Basic Block: %(BB\\d+)? Num States: (\\d+)");
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
		for(FiniteStateMachine::StateListType::iterator it = fsm->begin(), fin=fsm->end();
			it != fin; ++it)
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
			BBstates t = *it;
			if(blk->getName() == t.stateRef){	
				t.stateCount++;
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
						errs() << orig.stateRef << " has " << (orig.stateCount - su.stateCount)
						<< " lost states.\n";
				}
			}
		}
	}

	void LostStateInjector::print()
	{
		for(BBState_iterator it=orig_begin(), fin=orig_end(); it != fin; ++it)
		{
			BBstates t = *it;
			errs() << t.stateRef << "\t" << to_string(t.stateCount) << "\n"; 
		} 
	}

} //namespace stchup
