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

    outf = open(outfile, 'w')

    frameSize = 101 #Size of the frame in 32 bit words
    offset = 109 #Size of the zero padding

    frame = 0
    linecount = 0  
    word = 0
    bit = 0
    for l in inLines:
        bit = 0
	word = (linecount - offset) % frameSize
        frame = int(math.floor((linecount - offset) / frameSize))
       	for c in l:
            if c == '1' and linecount > offset:
		outf.write(str(frame) + ',' + str(word) + ',' + str(bit) + '\n')
	    bit = bit + 1
        linecount = linecount + 1

    outf.close()

if __name__ == "__main__":
            main(sys.argv[1:])
