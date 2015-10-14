import sys, getopt
import re
import math

def main(argv):
    infile = ''
    outfile = ''
    maskfile = ''
    try:
            opts, args = getopt.getopt(argv, "hi:o:m", ["input=", "output=", "mask="])
    except getopt.GetoptError:
            print 'Usage: python filterLinearAddresses.py --input <input>.la --output <output linear addresses>.la --mask <masks>.la'
            sys.exit(2)

    for opt, arg in opts:
        if opt == '-h':
            print 'Usage: python filterLinearAddresses.py --input <input>.la --output <output linear addresses>.la --mask <masks>.la'
            sys.exit()
        if opt in ("-i", "--input"):
            infile = arg
        if opt in ("-o", "--output"):
            outfile = arg
        if opt in ("-m", "--mask"):
            maskfile = arg
    assert(outfile != '')
    assert(infile != '')
    inputfile = open(infile, 'r')
    maskingfile = open(maskfile, 'r')
    inLines = inputfile.readlines()
    maskLines = maskingfile.readlines()
    maskingfile.close()
    inputfile.close()

    outf = open(outfile, 'w')

    count = 0
    remove=False
    for l in inLines:
        count = count + 1
        for m in maskLines:
            if(l == m):
                remove=True
                break
        if remove != True:
            outf.write(l)
        remove=False
        print float(count)/float(len(inLines))
    outf.close()

if __name__ == "__main__":
            main(sys.argv[1:])
