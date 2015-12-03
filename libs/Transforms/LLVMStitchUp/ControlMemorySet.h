//===-- ControlMemorySet.h - ControlMemorySet class definition -------*- C++ -*-===//
//
//                    StitchUp: Fault Tolerant High Level Synthesis 
//
// This file is distributed under the StitchUp 
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// Contains the Control Memory Set data structure and methods for
/// operating over it. Used in the Control Flow Analysis as part of the Control Dependence Set
/// to keep track of memory locations that have an effect on the CFG of the program.
///
//===----------------------------------------------------------------------===//

#ifndef CONTROLMEMORYSET_LIB
#define CONTROLMEMORYSET_LIB

#include <vector>
#include <exception>
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/DerivedTypes.h"

using namespace llvm;

namespace StchUp{

	typedef std::vector<Value *>::iterator cmem_iterator;
	class ControlMemorySet {
		public:
			ControlMemorySet(); //Default Constructor
			~ControlMemorySet(); //Default Deconstructor
			bool checkIfExists(Value *v); //returns true if the Value is in the Cmem
			void add(Value* v); //Adds a value to the Cmem
			bool addIfNotPresent(Value *v); //Checks if a value is in the Cmem and adds it to the set
			void remove(Value *v); //Removes from Cmem
			void print(); //Prints the entire Cmem, used for debugging	
			cmem_iterator begin(); //Returns the begining of a Cmem iterator	
			cmem_iterator end(); //Returns the end of a Cmem iterator
		private:
			std::vector<Value *>* Cmem; //Set of LLVM values that influence control
	
	}; //Class ControlMemorySet

	//constructor
	ControlMemorySet::ControlMemorySet()
	{
		Cmem = new std::vector<Value *>;
	}

	//deconstructor
	ControlMemorySet::~ControlMemorySet()
	{
		delete Cmem;	
	}

	cmem_iterator ControlMemorySet::begin()
	{
		return Cmem->begin();
	}

	cmem_iterator ControlMemorySet::end()
	{
		return Cmem->end();
	}

	//Iterates through the Cmem and returns true if value is found
	bool ControlMemorySet::checkIfExists(Value *v)
	{
		for(std::vector<Value *>::iterator cdsIter = Cmem->begin(), Final=Cmem->end(); cdsIter != Final; ++cdsIter)
		{
			Value *curr = *cdsIter;
			if(curr == v) { return true; }	
		}
		return false;
	}


	void ControlMemorySet::add(Value *v)
	{
		Cmem->push_back(v); 
	}

	//returns true if it could sucessfully add the item
	bool ControlMemorySet::addIfNotPresent(Value *v)
	{
		if(!checkIfExists(v)){
			add(v);
			return true;
		}
		return false;
	}	

	void ControlMemorySet::remove(Value *v)
	{
		for(std::vector<Value *>::iterator cdsIter = Cmem->begin(), Final=Cmem->end(); cdsIter != Final; ++cdsIter)
		{
			Value *curr = *cdsIter;
			if(curr == v) { Cmem->erase(cdsIter); return; }
		}
	}
	
	//used for debugging
	void ControlMemorySet::print()
	{
		errs() << "{";
		for(cmem_iterator it = this->begin(), itend=this->end(); it != itend; ++it)
		{
			Value *curr = *it;
			errs() << *curr <<",";
		}
		errs() << "}\n";
	}


} //namespace StchUp

#endif
