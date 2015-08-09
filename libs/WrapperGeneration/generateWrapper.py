#Python Script to generate a verilog wrapper which instantiates the original circuit,
#the stitchup circuit, and the checking logic on the exposed state registers 
import sys, getopt
import re
import wrapperHelper as wH


def main(argv):
    original = ''
    stitchup = ''
    wrapper = ''
    try:
            opts, args = getopt.getopt(argv, "ho:i:w", ["orig=", "stitchup=", "wrapper="])
    except getopt.GetoptError:
            print 'Usage: python generateWrapper.py --origi original.v --stitchup stitchup.v --wrapper=protected.v'
            sys.exit(2)

    for opt, arg in opts:
        if opt == '-h':
            print 'Usage: python generateWrapper.py --origi original.v --stitchup stitchup.v --wrapper=protected.v'
            sys.exit()
        if opt in ("-o", "--orig"):
            original = arg 
        if opt in ("-s", "--stitchup"):
            stitchup = arg
        if opt in ("-w", "--wrapper"):
            wrapper = arg
    outfile = open(wrapper, 'w')
    origString = open(original, 'r').read()

    wrapperString = "//Stitchup toplevel wrapper for" + original + ", will flag an error on CFG faults.\n"
    
    signals = wH.getSignals(origString, 'top')

    #Declare the port list of the topmost module
    wrapperString += "\nmodule topmost(\n"
    for s in signals:
        wrapperString += '\t' + s +',\n'
    wrapperString = wrapperString[:-2]
    wrapperString += '\n);\n\n'

    #Instantiate the signals that are used in the I/O top level ports
    #For this we need to grab the direction
    (inputlist, outputlist) = wH.gatherIOLists(original, signals)

    #Instantiate the input/output with the correct widths
    for i in inputlist:
        wrapperString += 'input [' + str(i[1]) + ':' + str(i[2]) +'] ' + i[0] + ';\n' 
    for o in outputlist:
        wrapperString += 'output wire [' + str(o[1]) + ':' + str(o[2]) + '] ' + o[0] + ';\n'

    #Instantiate the check_state XOR checking logic
    #Get the bit width for the check_state register
    for s in outputlist:
        if s[0] == "check_state":
            checkStateMSB = str(s[1])
            checkStateLSB = str(s[2])
    wrapperString += '\nreg [' + checkStateMSB + ':' + checkStateLSB + '] orig_check_state;\n' 
    wrapperString += 'reg [' + checkStateMSB + ':' + checkStateLSB + '] stitchup_check_state;\n' 
    wrapperString += '\nalways @(posedge clk) begin\n'
    wrapperString += 'check_state = orig_check_state ^ stitchup_check_state;\n' 
    wrapperString += 'end\n'

    #Instantiate the original LegUp component
    wrapperString += '\ntop top_inst(\n'
    for s in signals: 
        if s == "check_state":
            wrapperString += '\t.check_state( orig_check_state )\n' 
        else:
            wrapperString += '\t.' + s + '(' + s + '),\n'
    wrapperString += ');\n'

    #Instantiate the StitchUp component
    wrapperString += '\nstitchup_top stitchup_top_inst(\n'
    for s in signals: 
        if s == "check_state":
            wrapperString += '\t.check_state( stitchup_check_state )\n' 
        else:
            wrapperString += '\t.' + s + '(' + s + '),\n'
    wrapperString += ');\n'


    wrapperString += 'endmodule\n'
    outfile.write(wrapperString)
    outfile.close()

if __name__ == "__main__":
            main(sys.argv[1:])
