import sys, getopt
import re

def getSignals(origString, modulename):
    regex = re.compile(r'module '+modulename+'\(\n((?:\s*[A-z0-9]+,?)*)\n\);', re.MULTILINE)
    m = regex.search(origString)
    if m:
        portregex = re.compile(r'\s*([A-z0-9]+),?')
        p = portregex.search(m.group(1))
        return re.findall(portregex, m.group(1))
    return "error"

#Check to see if an item already exists
def inList(siglist, signame):
        for s in siglist:
                if signame == s[0]:
                        return True
        return False


#Function that takes the list of ports for the module and returns if they are inputs or outputs
def gatherIOLists(inputfile, lop):
        inputvfile = open(inputfile, 'r')
        vin = inputvfile.readlines()
        inputvfile.close()

        inputlist = [ ]
        outputlist = [ ]

        for p in lop:
                r_i = "input\s+(\[(\d+):(\d+)\])?\s*" + re.escape(p) + "\s*;"
                r_o = "output\s+(?:reg|wire)?\s*(\[(\d+):(\d+)\])?\s*" + re.escape(p) + "\s*;"
                for l in vin:
                        item = [ ]
                        f_i = re.search(r_i, l)
                        f_o = re.search(r_o, l)
                        if f_i:
                                if not inList(inputlist, p) and not inList(outputlist, p):
                                        item.append(p)
                                        if f_i.group(1) is not None:
                                                item.append(int(f_i.group(2)))
                                                item.append(int(f_i.group(3)))
                                        else:
                                                item.append(0)
                                                item.append(0)
                                        inputlist.append(item)

                        if f_o:
                                if not inList(outputlist, p) and not inList(inputlist, p):
                                        item.append(p)
                                        if f_o.group(1) is not None:
                                                item.append(int(f_o.group(2)))
                                                item.append(int(f_o.group(3)))
                                        else:
                                                item.append(0)
                                                item.append(0)
                                        outputlist.append(item)
        return (inputlist, outputlist)

#Function to gather the module name from a verilog file
def gatherModuleName(inputString):
        regex = re.compile(r'module ([A-z0-9]+)\s*\(', re.MULTILINE)
        m = regex.search(inputString)
        if m:
            return m.group(1)
        print "Error, could not locate a module!"

#prefxModule prefixes a modules name with a string so that we
#can instantiate both the presynthesis and the post synthesis 
#versions
def prefixModule(modulefilename, prefix, module):
        f = open(modulefilename, 'r')
        filedata = f.read()
        f.close()
        newmodule = prefix + module

        newdata = filedata.replace(module, newmodule)

        f = open(modulefilename, 'w')
        f.write(newdata)
        f.close()

        return

