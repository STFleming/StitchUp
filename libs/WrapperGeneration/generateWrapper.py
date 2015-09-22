#Python Script to generate a verilog wrapper which instantiates the original circuit,
#the stitchup circuit, and the checking logic on the exposed state registers 
import sys, getopt
import re
import wrapperHelper as wH


def main(argv):
    original = ''
    stitchup = ''
    wrapper = ''
    dmrflag = False
    noProtect = False
    try:
            opts, args = getopt.getopt(argv, "ho:i:w:n:d", ["orig=", "stitchup=", "wrapper=", "np", "dmr"])
    except getopt.GetoptError:
            print 'Usage: python generateWrapper.py --origi original.v --stitchup stitchup.v --wrapper=protected.v --np --dmr'
            sys.exit(2)

    for opt, arg in opts:
        if opt == '-h':
            print 'Usage: python generateWrapper.py --origi original.v --stitchup stitchup.v --wrapper=protected.v --np --dmr'
            sys.exit()
        if opt in ("-o", "--orig"):
            original = arg 
        if opt in ("-s", "--stitchup"):
            stitchup = arg
        if opt in ("-w", "--wrapper"):
            wrapper = arg
	if opt in ("--np"):
	    noProtect = True
        if opt in ("--dmr"):
	    dmrflag = True
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
        if o[0] == 'finish' or o[0] == 'return_val':
            wrapperString += 'output reg[' + str(o[1]) + ':' + str(o[2]) + '] ' + o[0] + ';\n'
	elif o[0] == 'check_state': 
            wrapperString += 'output reg[31:0] check_state;\n'
        else:
            wrapperString += 'output wire [' + str(o[1]) + ':' + str(o[2]) + '] ' + o[0] + ';\n'

    wrapperString += 'wire [31:0] result;\n\n'

    wrapperString += '\nwire finish_orig, finish_stitchup;\n'
    wrapperString += 'always @ (posedge clk)\nbegin\n'
    wrapperString += '\tif(reset==1)\n'
    wrapperString += '\tbegin\n'
    wrapperString += '\t\treturn_val <= 0;\n'
    wrapperString += '\t\tfinish <= 0;\n'
    wrapperString += '\tend\n'
    wrapperString += '\tif(finish_orig==1)\n'
    wrapperString += '\tbegin\n'
    wrapperString += '\t\treturn_val <= result;\n'
    wrapperString += '\t\tfinish <= 1;\n'
    wrapperString += '\tend\n'
    wrapperString += 'end\n'

    if noProtect == False :
    	#Instantiate the check_state XOR checking logic
    	#Get the bit width for the check_state register
    	for s in outputlist:
    	    if s[0] == "check_state":
    	        checkStateMSB = str(s[1])
    	        checkStateLSB = str(s[2])
    	wrapperString += '\nwire [' + checkStateMSB + ':' + checkStateLSB + '] orig_check_state;\n' 
    	wrapperString += 'wire [' + checkStateMSB + ':' + checkStateLSB + '] stitchup_check_state;\n' 
    	wrapperString += 'reg [0:0] errorFlag;\n' 
    	wrapperString += '\nalways @(posedge clk) begin\n'
    	wrapperString += '\tif(reset==1)\n'
    	wrapperString += '\tbegin\n'
    	wrapperString += '\t\terrorFlag <=  0;\n'
    	wrapperString += '\t\tcheck_state <= 0;\n'
    	wrapperString += '\tend\n'
    	wrapperString += '$display(\"%t, su=%d, orig=%d\",$time, stitchup_check_state, orig_check_state);\n'
    	wrapperString += '\tif (((orig_check_state ^ stitchup_check_state) != 0) && errorFlag == 0)\n'
    	wrapperString += '\tbegin\n'
    	wrapperString += '\t\tcheck_state <= 1;\n'
    	wrapperString += '\t\terrorFlag <= 1;\n'
    	wrapperString += '\tend\n'
    	wrapperString += 'check_state = orig_check_state ^ stitchup_check_state;\n' 
    	wrapperString += 'end\n'

    #Instantiate the original LegUp component
    wrapperString += '\ntop top_inst(\n'
    for s in signals: 
        if s == "check_state":
            wrapperString += '\t.check_state( orig_check_state )\n' 
	elif s == "finish":
	    wrapperString += '\t.finish( finish_orig ),\n'
	elif s == "return_val":
	    wrapperString += '\t.return_val( result ),\n'
        else:
            wrapperString += '\t.' + s + '(' + s + '),\n'
    wrapperString += ');\n'

    if noProtect == False :
    	#Instantiate the StitchUp component
	if dmrflag == True:
    	    wrapperString += '\ntop stitchup_top_inst(\n'
	else:
    	    wrapperString += '\nstitchup_top stitchup_top_inst(\n'

    	for s in signals: 
    	    if s == "check_state":
    	        wrapperString += '\t.check_state( stitchup_check_state )\n' 
    	    elif s == "return_val":
    	        wrapperString += '\t.return_val( open ),\n'
    	    elif s == "finish":
    	        wrapperString += '\t.finish( finish_stitchup ),\n'
    	    else:
    	        wrapperString += '\t.' + s + '(' + s + '),\n'
    	wrapperString += ');\n'


    wrapperString += 'endmodule\n'
    outfile.write(wrapperString)
    outfile.close()

if __name__ == "__main__":
            main(sys.argv[1:])
