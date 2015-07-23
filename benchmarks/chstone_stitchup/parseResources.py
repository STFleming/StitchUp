import sys, getopt
import re
import os
import csv

def gatherResource(filename, resource):
	p = open(filename, 'r')
	plines = p.readlines()
	amount=' '
	search_expr = ";\s" + re.escape(resource) + "\s*;\s*(\d+)\s"
	for line in plines:
		m = re.search(search_expr, line)
		if m:
			return int(m.group(1)) 
	return 0	

def main(argv):
	inputfile = ' '
	stitchupfile=' '
	name=' '
	try:
		opts, args = getopt.getopt(argv, "hi:n:s:",["ifile=","name=","stitchup="])
	except getopt.GetoptError:
		print "Usage -i full.rpt -s stitchup.rpt -n name"
		sys.exit(2)
	for opt, arg in opts:
		if opt == "-h":
			print "Usage -i full.rpt -su stitchup.rpt -n name"
			sys.exit()	
		elif opt in ("-i", "--ifile="):
			inputfile = arg
		elif opt in ("-s", "--stitchup="):
			stitchupfile = arg
		elif opt in ("-n", "--name="):
			name = arg

	ALM_f = gatherResource(inputfile, "ALMs needed [=A-B+C]")
	LUT_f = gatherResource(inputfile, "Combinational ALUT usage for logic")
	REG_f = gatherResource(inputfile, "Total registers")
	BRAM_f = gatherResource(inputfile, "M10K blocks")
	DSP_f = gatherResource(inputfile, "Total DSP Blocks")
	
	ALM_su = gatherResource(stitchupfile, "ALMs needed [=A-B+C]")
	LUT_su = gatherResource(stitchupfile, "Combinational ALUT usage for logic")
	REG_su = gatherResource(stitchupfile, "Total registers")
	BRAM_su = gatherResource(stitchupfile, "M10K blocks")
	DSP_su = gatherResource(stitchupfile, "Total DSP Blocks")

	ALM = float(ALM_su)/float(ALM_f) if ALM_f > 0 else "inf" 
	LUT = float(LUT_su)/float(LUT_f) if LUT_f > 0 else "inf" 
	REG = float(REG_su)/float(REG_f) if REG_f > 0 else "inf" 
	BRAM = float(BRAM_su)/float(BRAM_f) if BRAM_f > 0 else "inf" 
	DSP = float(DSP_su)/float(DSP_f) if DSP_f > 0 else "inf" 	

	#name, ALM, LUTS, REG, BRAM, DSP
	print name + ","+ str(ALM) + ","+ str(LUT) + "," + str(REG) + "," + str(BRAM) + "," + str(DSP) 
	
if __name__ == "__main__":
   main(sys.argv[1:])
