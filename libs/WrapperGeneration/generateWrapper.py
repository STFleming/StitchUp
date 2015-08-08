#Python Script to generate a verilog wrapper which instantiates the original circuit,
#the stitchup circuit, and the checking logic on the exposed state registers 
import sys, getopt
import re

def getSignals(origString):
    regex = re.compile(r'module top\(\n((?:\s*[A-z0-9]+,?)*)\n\);', re.MULTILINE)
    m = regex.search(origString)
    if m:
        portregex = re.compile(r'\s*([A-z0-9]+),?')
        p = portregex.search(m.group(1))
        return re.findall(portregex, m.group(1))
    return "error"

#Check to see if an item already exists
def inList(siglist, signame):
        for s in siglist:
                if signame == s[0]:
                        return True
        return False

#Function that takes the list of ports for the module and returns if they are inputs or outputs
def gatherIOLists(inputfile, lop):
        inputvfile = open(inputfile, 'r')
        vin = inputvfile.readlines()
        inputvfile.close()

        inputlist = [ ]
        outputlist = [ ]

        for p in lop:
                r_i = "input\s+(\[(\d+):(\d+)\])?\s*" + re.escape(p) + "\s*;"
                r_o = "output\s+(?:reg)?\s*(\[(\d+):(\d+)\])?\s*" + re.escape(p) + "\s*;"
                for l in vin:
                        item = [ ]
                        f_i = re.search(r_i, l)
                        f_o = re.search(r_o, l)
                        if f_i:
                                if not inList(inputlist, p) and not inList(outputlist, p):
                                        item.append(p)
                                        if f_i.group(1) is not None:
                                                item.append(int(f_i.group(2)))
                                                item.append(int(f_i.group(3)))
                                        else:
                                                item.append(0)
                                                item.append(0)
                                        inputlist.append(item)

                        if f_o:
                                if not inList(outputlist, p) and not inList(inputlist, p):
                                        item.append(p)
                                        if f_o.group(1) is not None:
                                                item.append(int(f_o.group(2)))
                                                item.append(int(f_o.group(3)))
                                        else:
                                                item.append(0)
                                                item.append(0)
                                        outputlist.append(item)
        return (inputlist, outputlist)

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
    
    signals = getSignals(origString)

    #Declare the port list of the topmost module
    wrapperString += "\nmodule topmost(\n"
    for s in signals:
        wrapperString += '\t' + s +',\n'
    wrapperString = wrapperString[:-2]
    wrapperString += '\n);\n\n'

    #Instantiate the signals that are used in the I/O top level ports
    #For this we need to grab the direction
    (inputlist, outputlist) = gatherIOLists(original, signals)

    #Instantiate the input/output with the correct widths
    for i in inputlist:
        wrapperString += 'input [' + str(i[1]) + ':' + str(i[2]) +'] ' + i[0] + ';\n' 
    for o in outputlist:
        wrapperString += 'output wire [' + str(o[1]) + ':' + str(o[2]) + '] ' + o[0] + ';\n'

    #Instantiate the original LegUp component
    wrapperString += '\ntop top_inst(\n'
    for s in signals: 
        wrapperString += '\t.' + s + '(' + s + '_orig),\n'
    wrapperString = wrapperString[:-2]
    wrapperString += '\n);\n'

    #Instantiate the StitchUp component
    wrapperString += '\nstitchup_top stitchup_top_inst(\n'
    for s in signals: 
        wrapperString += '\t.' + s + '(stitchup_' + s + '),\n'
    wrapperString = wrapperString[:-2]
    wrapperString += '\n);\n'


    wrapperString += 'endmodule\n'
    outfile.write(wrapperString)
    outfile.close()

if __name__ == "__main__":
            main(sys.argv[1:])
