
tb.v: dfadd.v stitchup.dfadd.v
	python exposeStateReg.py --ifile dfadd.v --ofile dfadd_c.v
	python exposeStateReg.py --ifile stitchup.dfadd.v --ofile stitchup.dfadd_c.v
	python renameStitchupTop.py --stitchup stitchup.dfadd_c.v
	python generateWrapper.py --orig dfadd_c.v --stitchup stitchup.dfadd_c.v --wrapper wrapper.v
	python generateTB.py --wrapper wrapper.v --tb tb.v --simscript runVsim --stitchup stitchup.dfadd_c.v --orig dfadd_c.v
	chmod u+x runVsim

clean:
	rm -f *_c.v wrapper.v tb.v runVsim transcript
	rm -f -r work
