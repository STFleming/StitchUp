#Python Script to expose the state register of a given StichUp/LegUp generated
#verilog input file.
import sys, getopt
import re

#Gets the number of bits for the current state register cur_state
def getStateBits(verilogin):
    vin = open(verilogin, 'r')
    lines = vin.readlines()
    for line in lines:
        m = re.search('reg (?:\[(\d+):(\d+)\])? cur_state;', line)
        if m:
            bmax = m.group(1) if m.group(1) else str(0) 
            bmin = m.group(2) if m.group(2) else str(0)
    	    return (bmax, bmin)
    print "ERROR: could not locate cur_state!\n"
    sys.exit(2)

#Find the topmost module port declaration and insert check_state signal
def insertTopCheckState(vinString, stateSize):
    regex = re.compile(r'^module (top|stitchup_top)$\n^\s*\(((?:\n\s*[A-z0-9]*,?)+)\n\s*\);', re.MULTILINE)
    m = regex.search(vinString)
    if m:
        return regex.sub('module '+m.group(1)+'('+m.group(2)+',\n\tcheck_state\n);\noutput wire [' + stateSize[0] +':'+stateSize[1]+'] check_state;\n\n', vinString)
    print "Unable to insert Top Check State!"
    return "error"

#Find the main instantiation in top and patch through the check_state register
def insertMainInstCheckState(vinString):
    regex = re.compile(r'(s)?main s?main_inst\(\n((?:\s*\.[A-z0-9]+\(\s*[A-z0-9]+\s*\),?)+)\n\);', re.MULTILINE)
    m = regex.search(vinString)
    if m:
        if m.group(1):
            s = regex.sub('smain smain_inst(\n'+m.group(2)+',\n\t.check_state( check_state )\n);\n', vinString)
        else:
            s = regex.sub('main main_inst(\n'+m.group(2)+',\n\t.check_state( check_state )\n);\n', vinString)
        return s 
    print "Unable to insert Main inst check state!"
    return "error"

#Insert the check_state signal into the port of the main module
def insertMainCheckState(vinString, stateSize):
    regex = re.compile(r'module (s)?main\n\(\n((?:\s*[A-z0-9]+,?)+)\n\);', re.MULTILINE)
    m = regex.search(vinString)
    if m:
        if m.group(1):
            s = regex.sub('module smain\n(\n' + m.group(2) + ',\n\tcheck_state\n);\noutput reg [' + stateSize[0] +':'+stateSize[1]+'] check_state;\n\n', vinString)
        else:
            s = regex.sub('module main\n(\n' + m.group(2) + ',\n\tcheck_state\n);\noutput reg [' + stateSize[0] +':'+stateSize[1]+'] check_state;\n\n', vinString)
        return s 
    print "Unable to insert Main check state!"
    return "error"

#assign the check_state signal with the cur_state signal
def insertCurStateCheckStateAssignment(vinString):
    regex = re.compile(r'always @\(\*\)\nbegin\nnext_state = cur_state;\n', re.MULTILINE)
    m = regex.search(vinString)
    if m:
        return regex.sub(m.group(0) + 'check_state = cur_state;\n', vinString)
    print "Unable to make main check state assignment!"
    return "error"

def main(argv):
    verilogin = ''
    verilogout = ''
    try:
            opts, args = getopt.getopt(argv, "hi:o", ["ifile=", "ofile="])
    except getopt.GetoptError:
            print 'Usage: python exposeStateReg.py --ifile input.v --ofile output.v'
            sys.exit(2)

    for opt, arg in opts:
        if opt == '-h':
            print 'Usage: python exposeStateReg.py --ifile input.v --ofile output.v'
            sys.exit()
        if opt in ("-i", "--ifile"):
            verilogin = arg
        if opt in ("-o", "--ofile"):
            verilogout = arg

    outfile = open(verilogout, 'w')
    infile = open(verilogin, 'r')
    vinString = infile.read()
    infile.close();

    stateSize = getStateBits(verilogin)
    vinString = insertTopCheckState(vinString, stateSize)
    vinString = insertMainInstCheckState(vinString)
    vinString = insertMainCheckState(vinString, stateSize)
    vinString = insertCurStateCheckStateAssignment(vinString)
    outfile.write(vinString)
    outfile.close()

if __name__ == "__main__":
            main(sys.argv[1:])
