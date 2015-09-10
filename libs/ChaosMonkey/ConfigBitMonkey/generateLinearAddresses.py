import sys, getopt
import re
import math

def main(argv):
    infile = ''
    outfile = ''
    stateNum = 0 
    try:
            opts, args = getopt.getopt(argv, "hi:o:s:m:n", ["input=", "output="])
    except getopt.GetoptError:
            print 'Usage: python stateMachineChaosMonkey.py --input <input ebd> --output <output linear addresses>'
            sys.exit(2)

    for opt, arg in opts:
        if opt == '-h':
            print 'Usage: python stateMachineChaosMonkey.py --input <input ebd> --output <output linear addresses>'
            sys.exit()
        if opt in ("-i", "--input"):
            infile = arg
        if opt in ("-o", "--output"):
            outfile = arg
    assert(outfile != '')
    assert(infile != '')
    inputfile = open(infile, 'r')
    inLines = inputfile.readlines()
    inputfile.close()

    frameSize = 101 #Size of the frame in 32 bit words

    frame = 0
    linecount = 0  
    essentialBits = 0
    for l in inLines:
       	for c in l:
            if c == '1':
		essentialBits = essentialBits + 1 	
	if essentialBits > 0:
	    print 'Frame: ' + str(frame) + '  eb: ' + str(essentialBits)
	    essentialBits = 0
        linecount = linecount + 1
        frame = math.floor(linecount / 101)

if __name__ == "__main__":
            main(sys.argv[1:])
