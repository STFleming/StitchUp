//===-- StitchUpBackendTargetInfo.cpp --------------------------------------===//
//
// This file is distributed under the LegUp license. See LICENSE for details.
//
//===----------------------------------------------------------------------===//
//
// StitchUp Backend Target Implementation
//
//===----------------------------------------------------------------------===//

#include "StitchUpTargetMachine.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/TargetRegistry.h"
using namespace llvm;

Target llvm::TheStitchUpTarget;

//LLVMInitializeStitchUpTargetInfo

// Look in Target/TargetSelect.h to see how this is called
extern "C" void LLVMInitializeStitchUpTargetInfo() { 
  RegisterTarget<Triple::UnknownArch> X(TheStitchUpTarget, "stitchup", "StitchUp Verilog Backend");
}

extern "C" void LLVMInitializeStitchUpTargetMC() {}
