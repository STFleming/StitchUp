//===-- StitchUpPass.h - Legup Pass --------------------------------*- C++ -*-===//
//
// This file is distributed under the LegUp license. See LICENSE for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements the main LLVM pass for Legup
//
//===----------------------------------------------------------------------===//


#ifndef LEGUP_PASS_H
#define LEGUP_PASS_H

#include "llvm/Pass.h"
#include "llvm/IR/InstVisitor.h"
#include "llvm/Support/CommandLine.h"
#include "../Verilog/MinimizeBitwidth.h"
#include "../Verilog/MinimizeBitwidth.h"
#include "../Verilog/LVA.h"
#include "../Verilog/LegupConfig.h"
#include "llvm/Analysis/AliasAnalysis.h"
//#include "llvm/Analysis/ProfileInfo.h"
#include "llvm/Target/TargetMachine.h"
#include "../Verilog/GenerateRTL.h"
#include <set>

using namespace llvm;
using namespace legup;

namespace legup {

static std::string LEGUP_VERSION = "4.0";

class Allocation;
class HwModule;
class FunctionSortingWrapper;

/// StitchUpPass - This class is the LLVM pass that converts LLVM IR into
/// Verilog
/// @brief Legup Backend Pass
class StitchUpPass : public ModulePass, public InstVisitor<StitchUpPass> {
public:

    StitchUpPass(raw_ostream &o) : ModulePass(ID), Out(o), allocation(0) {}

    ~StitchUpPass();

    virtual const char *getPassName() const { return "StitchUpPass backend"; }

    /// doInitialization - Allocate RAMs for global variables
    virtual bool doInitialization(Module &M);

    /// runOnFunction - schedule each function and create HwModule object
    bool runOnModule(Module &M);

    /// doFinalization - print the verilog
    virtual bool doFinalization(Module &M);

private:

	void pipelineLabelSanityCheck(Module &M);
    void printVerilog(const std::set<const Function*> &AcceleratedFcts);
    void printResourcesFile(std::string fileName);
    void printBBStats(Function &F);
    std::vector<Function*> getDepthFirstSortedFunctions(Module &M);

    virtual void getAnalysisUsage(AnalysisUsage &AU) const {
        AU.addRequired<LoopInfo>();
        AU.addRequired<LiveVariableAnalysis>();
        AU.addRequired<MinimizeBitwidth>();
        AU.addRequired<AliasAnalysis>();
        //if (LEGUP_CONFIG->getParameterInt("LLVM_PROFILE")) {
            //AU.addRequired<ProfileInfo>();
        //}
        AU.setPreservesAll();
    }

    raw_ostream &Out;
    static char ID;
    Allocation *allocation;
    LiveVariableAnalysis *LVA;
    MinimizeBitwidth *MBW;
};

} // End stchup namespace

#endif
