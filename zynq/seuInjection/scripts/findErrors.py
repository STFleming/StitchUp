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
    dE = False
    cE = False
    tO = False
    timeE = False

    DataOnlyErrors = 0
    TotalErrors = 0
    Timeouts = 0;
    IncorrectCycles = 0
    CaughtErrors = 0
    IncorrectCyclesCorrectData = 0
    CaughtErrorIncorrectCycles = 0
    

    with open(csvresfilename, 'rb') as csvfile:
	csvValues = csv.reader(csvfile, delimiter=',', quotechar='|')
	for row in csvValues:
		if row[0] != "26625903":
			if row[9] == 'ERROR':
				cE = True
			if row[9] == 'TIMEOUT':
				tO = True
			if row[4] != str(expectedval):
				dE = True
			if row[8] != str(cycles):
				timeE = True
			if dE or cE or timeE or tO:
				#Frame, bit, dE, cE
				Frame = int(row[0]) >> 12
				Word = int((int(row[0]) & 0xFFF) / 32)
				print row
				TotalErrors = TotalErrors + 1
			if tO:
				Timeouts = Timeouts + 1
			if timeE and not tO and not cE:
				IncorrectCycles = IncorrectCycles + 1
			if dE and not cE and not tO and not timeE:
				DataOnlyErrors = DataOnlyErrors + 1
			if cE:
				CaughtErrors = CaughtErrors + 1
			if cE and timeE:
				CaughtErrorIncorrectCycles = CaughtErrorIncorrectCycles + 1
			if not dE and timeE:	
				IncorrectCyclesCorrectData = IncorrectCyclesCorrectData + 1

			dE = False
			cE = False
			tO = False
			timeE = False

	print "Total Errors: " + str(TotalErrors)
	print "Data Only Errors: " + str(DataOnlyErrors)
	print "CaughtErrors: " + str(CaughtErrors)
	print "    CaughtErrors with Incorrect Cycles: " + str(CaughtErrorIncorrectCycles)
	print "Timeouts: " + str(Timeouts)
	print "IncorrectCycles: " + str(IncorrectCycles)
	print "IncorrectCycles with CorrectData: " + str(IncorrectCyclesCorrectData)

	per_DataOnly = (float(DataOnlyErrors)/float(TotalErrors)) * 100
	per_CaughtErrors = (float(CaughtErrors)/float(TotalErrors)) * 100
	per_Timeouts = (float(Timeouts)/float(TotalErrors)) * 100
	per_IncorrectCycles = (float(IncorrectCycles)/float(TotalErrors)) * 100
	print "Percentages"
	print "-----------\n"
	print "Data Only Errors: " + str(per_DataOnly)
	print "Caught Errors: " + str(per_CaughtErrors)
	print "Timeouts: " + str(per_Timeouts)
	print "Incorrect Cycles: " + str(per_IncorrectCycles)
	print "Total: " + str(per_DataOnly + per_CaughtErrors + per_Timeouts + per_IncorrectCycles)

if __name__ == "__main__":
    main(sys.argv[1:])

