#Python Script to generate a verilog wrapper which instantiates the original circuit,
#the stitchup circuit, and the checking logic on the exposed state registers 
import sys, getopt
import re
import wrapperHelper as wH

def main(argv):
    stitchup = ''
    try:
            opts, args = getopt.getopt(argv, "hs", ["stitchup="])
    except getopt.GetoptError:
            print 'Usage: python renameStitchupTop.py --stitchup stitchup.v'
            sys.exit(2)

    for opt, arg in opts:
        if opt == '-h':
            print 'Usage: python renameStitchupTop.py --stitchup stitchup.v'
            sys.exit()
        if opt in ("-s", "--stitchup"):
            stitchup = arg 

    wH.prefixModule(stitchup, 'stitchup_')


if __name__ == "__main__":
            main(sys.argv[1:])
