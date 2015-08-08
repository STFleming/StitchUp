#Python Script to generate a verilog wrapper which instantiates the original circuit,
#the stitchup circuit, and the checking logic on the exposed state registers 
import sys, getopt
import re

def instantiateTop(origString):
    orig_inst = ''
    stitchup_inst = ''
    regex = re.compile(r'module top\(\n((?:\s*[A-z0-9]+,?)*)\n\);', re.MULTILINE)
    m = regex.search(origString)
    if m:
        portregex = re.compile(r'\s*([A-z0-9]+),?')
        p = portregex.search(m.group(1))
        orig_inst = 'top top_inst(\n'
        for i in re.findall(portregex, m.group(1)):
            orig_inst += '\t.' + i + '(' + i + '_orig),\n'
        orig_inst = orig_inst[:-2]
        orig_inst += '\n);\n'
        return orig_inst
    return "error"

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
    stitchString = open(stitchup, 'r').read()
    origString = open(original, 'r').read()

    wrapperString = "//Stitchup toplevel wrapper for" + original + ", will flag an error on CFG faults.\n"

    print instantiateTop(origString) 

    outfile.write(wrapperString)
    outfile.close()

if __name__ == "__main__":
            main(sys.argv[1:])
