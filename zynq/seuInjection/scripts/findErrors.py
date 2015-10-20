import sys, getopt
import os
import csv

def main(argv):
    csvresfilename = ''
    expectedval = ''
    cycles = ''
    try:
            opts, args = getopt.getopt(argv, "hi:e:c", ["input=", "expected=", "cycles="])
    except getopt.GetoptError:
            print 'Usage: python findErrors.py --input <results>.csv --expected <expected ans value> --cycles <expected number of cycles>'
            sys.exit(2)

    for opt, arg in opts:
        if opt == '-h':
            print 'Usage: python findErrors.py --input <results>.csv --expected <expected ans value> --cycles <expected number of cycles>'
            sys.exit()
        if opt in ("-i", "--input"):
            csvresfilename = arg
        if opt in ("-e", "--expected"):
            expectedval = arg
        if opt in ("-c", "--cycles"):
            cycles = arg
    assert(csvresfilename != '')

    DE_T_CE_T = 0;
    DE_F_CE_T = 0;
    DE_T_CE_F = 0;
    DE_F_CE_F = 0;
    TIMEOUTS = 0;
    CYCLES = 0;
    dE = False
    cE = False
    tO = False
    timeE = False

    with open(csvresfilename, 'rb') as csvfile:
	csvValues = csv.reader(csvfile, delimiter=',', quotechar='|')
	for row in csvValues:
		if row[8] == 'ERROR':
			cE = True
		if row[8] == 'TIMEOUT':
			tO = True
		if row[4] != str(expectedval):
			dE = True
		if row[8] != str(cycles):
			timeE = True
		if dE or cE:
			#Frame, bit, dE, cE
			Frame = int(row[0]) >> 12 
			Word = int((int(row[0]) & 0xFFF) / 32) 
			print str(Frame) + "," + str(Word) + "," + str(dE) + "," + str(cE)
			#print row
		if dE:
			if cE:
				DE_T_CE_T = DE_T_CE_T + 1;
			else:
				DE_T_CE_F = DE_T_CE_F + 1;	
		else:
			if cE:
				DE_F_CE_T = DE_F_CE_T + 1;
			else:
				DE_F_CE_F = DE_F_CE_F + 1;
		if tO:
			TIMEOUTS = TIMEOUTS + 1
		if timeE:
			CYCLES = CYCLES + 1
		dE = False
		cE = False
		tO = False
		timeE = False

	print "|Total Faults: " + str(DE_T_CE_T + DE_T_CE_F + DE_F_CE_T)
	print "|Data Only Faults: " + str(DE_T_CE_F)
	print "|StitchUp Detected Faults: " + str(DE_T_CE_T + DE_F_CE_T)
	print "|Total Timeouts: " + str(TIMEOUTS)
	print "|Total Cycle Errors: " + str(CYCLES)
	print "|Essential bits that are not critical: " + str(DE_F_CE_F)

if __name__ == "__main__":
    main(sys.argv[1:])

