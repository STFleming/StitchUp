//===-- ControlArrayRef.h - ControlArrayRef class definition -------*- C++ -*-===//
//
//                    StitchUp: Fault Tolerant High Level Synthesis 
//
// This file is distributed under the StitchUp 
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
///
/// A class that is used to track which arrays and memory locations are used
/// when making control flow decisions.
//===----------------------------------------------------------------------===//

#ifndef CONTROLARRAYREF_LIB 
#define CONTROLARRAYREF_LIB

#include <vector>
#include <exception>
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Value.h"

using namespace llvm;

namespace StchUp{

	typedef std::vector<Value *>::iterator carray_iterator;
	class ControlArrayRef {
		public:
			ControlArrayRef(); //Default Constructor
			~ControlArrayRef(); //Default Deconstructor
			bool checkIfExists(Value *v); //returns true if the Value is in the Carray
			bool checkIfExists(GetElementPtrInst *v);
			void add(Value* v); //Adds a value to the Carray
			bool addIfNotPresent(Value *v); //Checks if a value is in the Carray and adds it to the set
			bool addIfNotPresent(GetElementPtrInst *v); //Extracts the Value from GetElementPtrInst then calls addIfNotPresent
			void remove(Value *v); //Removes from Carray
			void print(); //Prints the entire Carray, used for debugging	
			carray_iterator begin(); //Returns the begining of a Carray iterator	
			carray_iterator end(); //Returns the end of a Carray iterator
		private:
			std::vector<Value *>* Carray; //Set of LLVM values that influence control
	
	}; //Class ControlArrayRef

	//constructor
	ControlArrayRef::ControlArrayRef()
	{
		Carray = new std::vector<Value *>;
	}

	//deconstructor
	ControlArrayRef::~ControlArrayRef()
	{
		delete Carray;	
	}

	carray_iterator ControlArrayRef::begin()
	{
		return Carray->begin();
	}

	carray_iterator ControlArrayRef::end()
	{
		return Carray->end();
	}

	//Iterates through the Carray and returns true if value is found
	bool ControlArrayRef::checkIfExists(Value *v)
	{
		for(std::vector<Value *>::iterator cdsIter = Carray->begin(), Final=Carray->end(); cdsIter != Final; ++cdsIter)
		{
			Value *curr = *cdsIter;
			if(curr == v) { return true; }	
		}
		return false;
	}

	bool ControlArrayRef::checkIfExists(GetElementPtrInst *v)
	{
		Value * array = v->getPointerOperand();
		return checkIfExists(array); 		
	}

	void ControlArrayRef::add(Value *v)
	{
		Carray->push_back(v); 
	}

	//returns true if it could sucessfully add the item
	bool ControlArrayRef::addIfNotPresent(Value *v)
	{
		if(!checkIfExists(v)){
			add(v);
			return true;
		}
		return false;
	}	

	//Extracts the Value from GetElementPtrInst then calls addIfNotPresent
	bool ControlArrayRef::addIfNotPresent(GetElementPtrInst *v)
	{
		Value * array = v->getPointerOperand();	
		return addIfNotPresent(array);
	}

	void ControlArrayRef::remove(Value *v)
	{
		for(std::vector<Value *>::iterator cdsIter = Carray->begin(), Final=Carray->end(); cdsIter != Final; ++cdsIter)
		{
			Value *curr = *cdsIter;
			if(curr == v) { Carray->erase(cdsIter); return; }
		}
	}
	
	//used for debugging
	void ControlArrayRef::print()
	{
		errs() << "{";
		for(carray_iterator it = this->begin(), itend=this->end(); it != itend; ++it)
		{
			Value *curr = *it;
			errs() << *curr <<",";
		}
		errs() << "}\n";
	}


} //namespace StchUp

#endif
