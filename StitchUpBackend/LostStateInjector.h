#include <iostream>
#include <fstream>
#include "../Verilog/FiniteStateMachine.h"
#include "llvm/Support/CommandLine.h"
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
		BBState_iterator begin();
		BBState_iterator end();
		void print();
	private:
		std::vector<BBstates> origBBStateNum;
		FiniteStateMachine *fsm;

 }; //namespace LostStateInjector

	BBState_iterator LostStateInjector::begin()
	{	return origBBStateNum.begin();		}

	BBState_iterator LostStateInjector::end()
	{	return origBBStateNum.end();		}

	LostStateInjector::LostStateInjector(FiniteStateMachine *in)
	{
		fsm = in;
		populateOrigBBStateNum();
	}

	LostStateInjector::~LostStateInjector()
	{
		//print();
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
	

	void LostStateInjector::print()
	{
		for(BBState_iterator it=begin(), fin=end(); it != fin; ++it)
		{
			BBstates t = *it;
			errs() << t.stateRef << "\t" << to_string(t.stateCount) << "\n"; 
		} 
	}

} //namespace stchup
