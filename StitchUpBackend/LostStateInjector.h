#include <iostream>
#include <fstream>
#include "../Verilog/FiniteStateMachine.h"
#include "llvm/Support/CommandLine.h"
#include <regex>

static cl::opt<string> 
OriginalSchedule("orig_sched", cl::desc("Specify the schedule of the original LegUp circuit for StitchUp to reference")
		 ,cl::value_desc("filename")); 

namespace stchup {
 class LostStateInjector {
	public:
		LostStateInjector(FiniteStateMachine *fsm);	
		void populateOrigBBStateNum();
	private:
		std::vector<int> origBBStateNum;
		FiniteStateMachine *fsm;

 }; //namespace LostStateInjector

	LostStateInjector::LostStateInjector(FiniteStateMachine *in)
	{
		fsm = in;
		populateOrigBBStateNum();
	}

	void LostStateInjector::populateOrigBBStateNum()
	{
		std::ifstream origSchedule(OriginalSchedule);
		if(!origSchedule.is_open()) { errs() << "Cannot open original schedule!\n"; assert(true);}	
		std::string line;				
		//std::regex expr("Basic\\sBlock\\s:\\s%BB(\\d*)\\sNum\\sStates:\\s(\\d+)");
		//std::smatch group;
		//while(getline(origSchedule, line))
		//{
		//	if(std::regex_search(line, group, expr))
		//	{
		//		for (auto x:group) errs() << x << "\t";
		//		errs() << "\n";
		//	}	
		//}
		origSchedule.close();
	}	

} //namespace stchup
