#Python Script to generate a verilog wrapper which instantiates the original circuit,
#the stitchup circuit, and the checking logic on the exposed state registers 
import sys, getopt
import re
import wrapperHelper as wH

def main(argv):
    simscript = ''
    testbenchfile = ''
    wrapper = ''
    try:
            opts, args = getopt.getopt(argv, "hw:t:s", ["wrapper=", "tb=", "simscript="])
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
            testbenchfile = arg 
        if opt in ("-s", "--simscript"):
            simscript = arg 

    infile = open(wrapper, 'r')
    tboutfile = open(testbenchfile, 'w')
    simoutfile = open(simscript, 'w')

    inString = infile.read(); 

    signals = wH.getSignals(inString, 'topmost')
    (inputlist, outputlist) = wH.gatherIOLists(wrapper, signals)

    testbench = "//Testbench file for the stitchup wrapped project.\m"
    testbench += "\nmodule tbtop (\n);\n\n"
     
    #Instantiate a register for each input 
    for i in inputlist:
        testbench += 'reg [' + str(i[1]) +':'+ str(i[2])+'] ' +i[0] + ';\n' 
    #Instantiate a wire for each output
    for o in outputlist:
        testbench += 'wire [' +str(o[1]) +':'+ str(o[2]) +'] ' + o[0] + ';\n'
    testbench += '\n\n'

    #Initial conditions for clock and reset signals
    #Pulses the reset condition
    testbench += 'initial begin\n'
    testbench += '\tclk = 0;\n'
    testbench += '\treset = 0;\n'
    testbench += '\t#10 reset = 1;\n'
    testbench += '\t#10 reset = 0;\n'
    testbench += 'end\n\n'
    
    #Clock generation
    testbench += 'always begin\n'
    testbench += '\t#5 clk = ~clk;\n'
    testbench += 'end\n\n'

    #Assert statement to makesure that the Error flag has never been signalled
    testbench += 'always @(posedge clk) begin\n'
    testbench += '\tif (!(check_state == 6\'b000000)) begin\n'
    testbench += '\t\t $display(\"Assertion Failed! %d\", check_state);\n'
    testbench += '\t\t $finish;\n'
    testbench += '\tend\n'
    testbench += 'end\n'

    testbench += '\n\nendmodule\n'

    tboutfile.write(testbench)
    tboutfile.close()

    sim = 'rm -r -f work\n'
    sim += 'vlib work\n'
    sim += 'vlog ./' + testbenchfile + '\n'  
    sim += 'vsim -c tbtop -do \"run 10000000 ; echo [simstats]; quit -f;\"\n'

    simoutfile.write(sim)
    simoutfile.close()

if __name__ == "__main__":
            main(sys.argv[1:])
