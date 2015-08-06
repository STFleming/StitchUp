//===-- StitchUpTargetMachine.h - StitchUp backend ----------------*- C++ -*-===//
//
// This file is distributed under the LegUp license. See LICENSE for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares the TargetMachine that is used by the StitchUp backend.
//
//===----------------------------------------------------------------------===//

#ifndef STITCHUPTARGETMACHINE_H
#define STITCHUPTARGETMACHINE_H

#include "llvm/Target/TargetMachine.h"
#include "llvm/IR/DataLayout.h"

namespace llvm {

struct StitchUpTargetMachine : public TargetMachine {
  StitchUpTargetMachine(const Target &T, StringRef TT,
                   StringRef CPU, StringRef FS,
				   const TargetOptions &Options,
                   Reloc::Model RM, CodeModel::Model CM,
				   CodeGenOpt::Level OL)
    : TargetMachine(T, TT, CPU, FS, Options) {}
  

  virtual bool addPassesToEmitFile(PassManagerBase &PM,
                                   formatted_raw_ostream &o,
                                   CodeGenFileType FileType,
                                   bool DisableVerify,
                                   AnalysisID StartAfter,
                                   AnalysisID StopAfter);
  
  virtual const DataLayout *getDataLayout() const { return 0; }
};

extern Target TheStitchUpTarget;

} // End llvm namespace


#endif
