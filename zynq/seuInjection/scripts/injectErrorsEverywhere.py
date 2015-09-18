import sys, getopt
import os
import time
import random

def main(argv):
    linaddrsfile = ''
    bitfile = ''
    try:
            opts, args = getopt.getopt(argv, "hl:b", ["la=", "bit="])
    except getopt.GetoptError:
            print 'Usage: python injectErrorsEverywhere.py --la <linear addresses>.la --bit <bitstream>'
            sys.exit(2)

    for opt, arg in opts:
        if opt == '-h':
            print 'Usage: python injectErrorsEverywhere.py --la <linear addresses>.la --bit <bitstream>'
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
    exec_cmd = '../sw_driver/bin/hlsKicker '

    os.system(reconfig_cmd)
    time.sleep(250/1000.0)

    os.system('../sw_driver/bin/injectSetup')
    time.sleep(250/1000.0)

    for addr in addrLines:
        inject_cmd = '../sw_driver/bin/injectOnly ' + str(addr)
        os.system(inject_cmd)
        time.sleep(100/1000.0)
	os.system(exec_cmd + str(addr))

    #inject_cmd = '../sw_driver/bin/injectOnly ' + str(random.choice(addrLines))
    #time.sleep(10/1000.0)
    #os.system(inject_cmd)

    os.system(exec_cmd)
    os.system('../sw_driver/bin/injectFinish')
    

if __name__ == "__main__":
    main(sys.argv[1:])

