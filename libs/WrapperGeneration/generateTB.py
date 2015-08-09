#Python Script to generate a verilog wrapper which instantiates the original circuit,
#the stitchup circuit, and the checking logic on the exposed state registers 
import sys, getopt
import re
import wrapperHelper as wH

def main(argv):
    simscript = ''
    testbench = ''
    wrapper = ''
    try:
            opts, args = getopt.getopt(argv, "hw:t:s", ["wrapper=", "testbench=", "simscript="])
    except getopt.GetoptError:
            print 'Usage: python generateTB.py --wrapper stitchup_orig_wrapper.v --tb testbench.v --simscript vsim_script'
            sys.exit(2)

    for opt, arg in opts:
        if opt == '-h':
            print 'Usage: python generateTB.py --wrapper stitchup_orig_wrapper.v --tb testbench.v --simscript vsim_script'
            sys.exit()
        if opt in ("-w", "--wrapper"):
            wrapper = arg 
        if opt in ("-t", "--tb"):
            testbench = arg 
        if opt in ("-s", "--simscript"):
            simscript = arg 
    infile = open(wrapper, 'r')
    tboutfile = open(testbench, 'w')
    simoutfile = open(simscript, 'w')

    inString = infile.read(); 

    signals = wH.getSignals(inString)
    (inputlist, outputlist) = wH.gatherIOLists(infile, signals)

    outfile.write(inString)
    outfile.close()

if __name__ == "__main__":
            main(sys.argv[1:])
