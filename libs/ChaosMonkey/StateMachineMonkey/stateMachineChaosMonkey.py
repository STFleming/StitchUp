import sys, getopt
import re
from random import randint

def main(argv):
    infile = ''
    outfile = ''
    stateNum = 0 
    try:
            opts, args = getopt.getopt(argv, "hi:o:s:m", ["input=", "output=", "state=", "maxstate="])
    except getopt.GetoptError:
            print 'Usage: python stateMachineChaosMonkey.py --input <input file>.v --output <output file>.v --state <corrupted state number> --maxstate <largest state number>'
            sys.exit(2)

    for opt, arg in opts:
        if opt == '-h':
            print 'Usage: python stateMachineChaosMonkey.py --input <input file>.v --output <output file>.v --state <corrupted state number>'
            sys.exit()
        if opt in ("-i", "--input"):
            infile = arg
        if opt in ("-o", "--output"):
            outfile = arg
        if opt in ("-s", "--state"):
            stateNum = int(arg)
	if opt in ("-m", "--maxstate"):
            maxState = int(arg)
    assert(outfile != '')
    assert(infile != '')
    inputfile = open(infile, 'r')
    inLines = inputfile.readlines()
    inputfile.close()

    oldState = ''
    damagedState = ''

    for l in inLines:
        regex = re.compile(r'(parameter \[\d+:\d+\] (?:LEGUP_F_|LOSTSTATE_).* = \d+\'d)'+ str(stateNum) +';')
        m = regex.search(l)	
        if m:
	    oldState = l
            damagedState = m.group(1) + str(randint(0, maxState)) + ';\n' 	

    assert(oldState != '')
    assert(damagedState != '')
    origfile = open(infile, 'r')
    newfile = origfile.read()
    origfile.close()
    outtext = newfile.replace(oldState, damagedState)
    outfile = open(outfile, 'w')
    outfile.write(outtext)
    outfile.close()

if __name__ == "__main__":
            main(sys.argv[1:])
