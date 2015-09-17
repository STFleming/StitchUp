import sys, getopt
import os
import time

def main(argv):
    linaddrsfile = ''
    bitfile = ''
    try:
            opts, args = getopt.getopt(argv, "hl:b", ["la=", "bit="])
    except getopt.GetoptError:
            print 'Usage: python injectErros.py --la <linear addresses>.la --bit <bitstream>'
            sys.exit(2)

    for opt, arg in opts:
        if opt == '-h':
            print 'Usage: python injectErros.py --la <linear addresses>.la --bit <bitstream>'
            sys.exit()
        if opt in ("-l", "--la"):
            linaddrsfile = arg
        if opt in ("-b", "--bit"):
            bitfile = arg
    assert(linaddrsfile != '')
    assert(bitfile != '')
    inputfile = open(linaddrsfile, 'r')
    addrLines = inputfile.readlines()
    inputfile.close()

    reconfig_cmd = 'cat ' + bitfile + ' > /dev/xdevcfg'
    exec_cmd = '../sw_driver/bin/hlsKicker >> res.csv'

    for addr in addrLines:
        inject_cmd = '../sw_driver/bin/injectErrors ' + str(addr)
        os.system(reconfig_cmd)
	time.sleep(150/1000.0)
        os.system(inject_cmd)
    	exec_cmd = '../sw_driver/bin/hlsKicker ' + str(addr) +' >> res.csv'
        os.system(exec_cmd)

if __name__ == "__main__":
    main(sys.argv[1:])

