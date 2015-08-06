#Automated Tests

These build tests are used to determine whether the StitchUp front end analysis passes / backend code generation
are performing correctly. Each folder performs a different suite of tests, and each contains a script runChecks
that runs each tests separately and compares the outcome against a golden result.

##./ControlFlowAnalysis
This folder checks the front end analysis pass in libs/Transforms/LLVMStitchUp which is responsible
for determining the set of instructions which touch control (CDS) for a given llvm bitcode and 
generating a new replica llvm bitcode file containing only the CDS instructions.
tldr; this creates an llvm program for the shadow control path

##./StitchUpBackend
This folder checks the state machine produced by the codegen pass in libs/Targets/StitchUpBackend 
