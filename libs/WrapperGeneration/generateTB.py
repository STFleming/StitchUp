#Python Script to generate a verilog wrapper which instantiates the original circuit,
#the stitchup circuit, and the checking logic on the exposed state registers 
import sys, getopt
import re
import wrapperHelper as wH

def main(argv):
    simscript = ''
    testbenchfile = ''
    wrapper = ''
    stitchup=''
    original=''
    try:
        opts, args = getopt.getopt(argv, "hw:t:s:u:i", ["wrapper=", "tb=", "simscript=", "stitchup=", "orig="])
    except getopt.GetoptError:
            print 'Usage: python generateTB.py --wrapper stitchup_orig_wrapper.v --tb testbench.v --simscript vsim_script --stitchup stitchup.v --orig original.v'
            sys.exit(2)

    for opt, arg in opts:
        if opt == '-h':
            print 'Usage: python generateTB.py --wrapper stitchup_orig_wrapper.v --tb testbench.v --simscript vsim_script --stitchup stitchup.v --orig original.v'
            sys.exit()
        if opt in ("-w", "--wrapper"):
            wrapper = arg 
        if opt in ("-t", "--tb"):
            testbenchfile = arg 
        if opt in ("-s", "--simscript"):
            simscript = arg 
        if opt in ("-u", "--stitchup"):
            stitchup = arg 
        if opt in ("-i", "--orig"):
            original = arg 

    infile = open(wrapper, 'r')
    tboutfile = open(testbenchfile, 'w')
    workoutfile = open(simscript + '_worklib', 'w')
    simoutfile = open(simscript, 'w')

    inString = infile.read(); 

    signals = wH.getSignals(inString, 'topmost')
    (inputlist, outputlist) = wH.gatherIOLists(wrapper, signals)

    testbench = "//Testbench file for the stitchup wrapped project.\n"
    testbench += "`timescale 1 ns / 1 ns\n"
    testbench += "\nmodule tbtop (\n);\n\n"
     
    #Instantiate a register for each input 
    for i in inputlist:
        testbench += 'reg [' + str(i[1]) +':'+ str(i[2])+'] ' +i[0] + ';\n' 
    #Instantiate a wire for each output
    for o in outputlist:
        testbench += 'wire [' +str(o[1]) +':'+ str(o[2]) +'] ' + o[0] + ';\n'
        if o[0] == "check_state":
            check_MSB = str(o[1]) #This is needed for the assertion check.
    testbench += 'integer log;\n'
    testbench += '\n\n'

    #Clock generation
    testbench += 'initial\n\tclk = 0;\nalways @(clk)\n\tclk <= #10 ~clk;\n\n'

    #Initial conditions for clock and reset signals
    #Pulses the reset condition
    testbench += 'initial begin\n\tlog = $fopen(\"res.log\",\"w\");\n//$monitor("At t=%t clk=%b %b %b %b %d", $time, clk, reset, start, finish, return_val);\n@(negedge clk);\nreset <= 1;\n@(negedge clk);\nreset <= 0;\nstart <= 1;\n@(negedge clk);\nstart <= 0;\nend\n\n'


    #Assert statement to makesure that the Error flag has never been signalled
    testbench += 'always @(negedge clk) begin\n'
    testbench += '\tif (check_state != 0) begin\n'
    testbench += '\t\t$fwrite(log, "%t,\t%d,\t%d,\t%d,\t1",$time, finish, return_val, check_state);\n'
    testbench += '\t\t$display(\"CONFIGURATION MISMATCH, CFG ERROR DETECTED! %d\", check_state);\n'
    testbench += '\t\t$fclose(log);\n'
    testbench += '\t\t$finish;\n'
    testbench += '\tend\n'
    testbench += 'end\n\n'

    #End condition
    testbench += 'always @(finish) begin\n'
    testbench += '\tif (finish == 1) begin\n'
    testbench += '\t\t$fwrite(log, "%t,\t%d,\t%d,\t%d,\t0",$time, finish, return_val, check_state);\n'
    testbench += '\t\t$display("At t=%t clk=%b finish=%b return_val=%d", $time, clk, finish, return_val);\n'
    testbench += '\t\t$display("Cycles: %d", ($time-50)/20);\n'
    testbench += '\t\t$fclose(log);\n'
    testbench += '\t\t$finish;\n'
    testbench += '\tend\n'
    testbench += 'end\n\n'
    
    #Memory
    testbench += 'initial begin\nwaitrequest <= 1;\n@(negedge clk);\n@(negedge clk);\nwaitrequest <= 0;\nend\n\n'

    #Instantiate the wrapper module
    testbench += 'topmost dut(\n'
    for s in signals:
        testbench += '\t.' + s + '( '+ s + ' ),\n'
    testbench = testbench[:-2]   
    testbench += '\n);\n'

    testbench += '\n\nendmodule\n'

    tboutfile.write(testbench)
    tboutfile.close()

    #Build the file to compile the work libs
    worklibs = 'rm -r -f work\n'
    worklibs += 'source ./modelsim.config\n'
    worklibs += 'vlib work\n'
    worklibs += 'vlog ${GENERIC_DIVIDER_LIBS}*.v\n'
    worklibs += 'vlog ${VERILOG_LIBS}*.v ./'+wrapper+' ./'+testbenchfile+' ./'+stitchup+' ./'+original+'\n'
    worklibs += 'vlog ${SYSTEMVERILOG_LIBS}*.v\n'  

    workoutfile.write(worklibs)
    workoutfile.close()

    sim = 'source ./modelsim.config\n'
    sim += 'vlog ./'+wrapper+' ./'+testbenchfile+' ./'+stitchup+' ./'+original+'\n'
    sim += 'vsim -c tbtop -do \"run 1000000000 ; echo [simstats]; quit -f;\"\n'

    simoutfile.write(sim)
    simoutfile.close()

if __name__ == "__main__":
            main(sys.argv[1:])
