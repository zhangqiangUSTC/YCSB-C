#!/usr/bin/python
import ConfigParser
import os
import getopt
import sys
import ConfigParser

class myconf(ConfigParser.ConfigParser):
        def __init__(self,defaults=None):
            ConfigParser.ConfigParser.__init__(self,defaults=None)
        def optionxform(self, optionstr):
            return optionstr
        
def modifyCon(filename,section,key,value):
    cf = myconf()
    cf.read(filename)
    cf.set(section,key,value);
    if key == "locationCacheSize":
            cf.set("misc","hashTableDefaultSize",str(int(value)+2))
    with open(filename,"w+") as f:
        cf.write(f)

def main():
    try:
        opts, args = getopt.getopt(sys.argv[1:], "h", ["help"])
    except getopt.error, msg:
        print msg
        print "for help use --help"
        sys.exit(2)
    modifyCon(args[0],args[1],args[2],args[3])

if __name__ == "__main__":
    main()
