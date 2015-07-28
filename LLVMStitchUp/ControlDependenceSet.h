//===-- ControlDependenceSet.h - ControlDependenceSet class definition -------*- C++ -*-===//
//
//                    StitchUp: Fault Tolerant High Level Synthesis 
//
// This file is distributed under the StitchUp 
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// Contains the Control Dependence Set data structure and methods for
/// operating over it. Used in the Control Flow Analysis to keep track of
/// instructions (LLVM IR) that have an effect on the CFG of the program.
///
//===----------------------------------------------------------------------===//

#include <vector>
#include <exception>
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/DerivedTypes.h"

using namespace llvm;

namespace StchUp{

	typedef std::vector<Value *>::iterator cds_iterator;
	class ControlDependenceSet {
		public:
			ControlDependenceSet(); //Default Constructor
			~ControlDependenceSet(); //Default Deconstructor
			bool checkIfExists(Value *v); //returns true if the Value is in the CDS
			void add(Value* v); //Adds a value to the CDS
			bool addIfNotPresent(Value *v); //Checks if a value is in the CDS and adds it to the set
			void remove(Value *v); //Removes from CDS
			void print(); //Prints the entire CDS, used for debugging	
			cds_iterator begin(); //Returns the begining of a CDS iterator	
			cds_iterator end(); //Returns the end of a CDS iterator
		private:
			std::vector<Value *>* CDS; //Set of LLVM values that influence control
			std::vector<PointerType *>* CMem; //Set of memory locations that influence control		
	
	}; //Class ControlDependenceSet

	//constructor
	ControlDependenceSet::ControlDependenceSet()
	{
		CDS = new std::vector<Value *>;
	}

	//deconstructor
	ControlDependenceSet::~ControlDependenceSet()
	{
		delete CDS;	
	}

	cds_iterator ControlDependenceSet::begin()
	{
		return CDS->begin();
	}

	cds_iterator ControlDependenceSet::end()
	{
		return CDS->end();
	}

	//Iterates through the CDS and returns true if value is found
	bool ControlDependenceSet::checkIfExists(Value *v)
	{
		for(std::vector<Value *>::iterator cdsIter = CDS->begin(), Final=CDS->end(); cdsIter != Final; ++cdsIter)
		{
			Value *curr = *cdsIter;
			if(curr == v) { return true; }	
		}
		return false;
	}


	void ControlDependenceSet::add(Value *v)
	{
		CDS->push_back(v); 
	}

	//returns true if it could sucessfully add the item
	bool ControlDependenceSet::addIfNotPresent(Value *v)
	{
		if(!checkIfExists(v)){
			add(v);
			return true;
		}
		return false;
	}	

	void ControlDependenceSet::remove(Value *v)
	{
		for(std::vector<Value *>::iterator cdsIter = CDS->begin(), Final=CDS->end(); cdsIter != Final; ++cdsIter)
		{
			Value *curr = *cdsIter;
			if(curr == v) { CDS->erase(cdsIter); return; }
		}
	}
	
	//used for debugging
	void ControlDependenceSet::print()
	{
		errs() << "{";
		for(cds_iterator it = this->begin(), itend=this->end(); it != itend; ++it)
		{
			Value *curr = *it;
			errs() << *curr <<",";
		}
		errs() << "}\n";
	}


} //namespace StchUp

