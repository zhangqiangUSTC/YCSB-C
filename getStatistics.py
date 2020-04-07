import re
import sys
import os

infilename = sys.argv[1]
outfilename = sys.argv[2]+infilename

if(not os.path.exists(sys.argv[2])):
    os.system("mkdir {0}".format(sys.argv[2]))

with open(infilename,'r') as infile:
    filetext = infile.read()
    readlatency = re.findall("([0-9.]+)us Read",filetext)[1]
    readops = re.findall("Read ops: ([0-9]+)",filetext)[1]
    readzerolatency = re.findall("([0-9.]+)us Zero",filetext)[1]
    readzeroops = re.findall("Zero-result ops: ([0-9]+)",filetext)[1]
    findtabletime = re.findall("findtable\.time.*?ave:([0-9.]+)",filetext)[1]
    findtablecnt = re.findall("findtable\.time.*?count:([0-9]+)",filetext)[1]
    internalgettime = re.findall("internalget\.time.*?ave:([0-9.]+)",filetext)[1]
    internalgetcnt = re.findall("internalget\.time.*?count:([0-9]+)",filetext)[1]
    releasetime = re.findall("release\.time.*?ave:([0-9.]+)",filetext)[1]
    releasecnt = re.findall("release\.time.*?count:([0-9]+)",filetext)[1]
    blockreadtime = re.findall("block\.read\.time.*?ave:([0-9.]+)",filetext)[1]
    blockreadcnt = re.findall("block\.read\.time.*?count:([0-9]+)",filetext)[1]
    blocknewtime = re.findall("block\.new\.time.*?ave:([0-9.]+)",filetext)[1]
    blocknewcnt = re.findall("block\.new\.time.*?count:([0-9]+)",filetext)[1]
    readfiletime = re.findall("block\.file\.read\.time.*?ave:([0-9.]+)",filetext)[1]
    readfinecnt = re.findall("block\.file\.read\.time.*?count:([0-9]+)",filetext)[1]
    readcachetime = re.findall("reader\.cache\.time.*?ave:([0-9.]+)",filetext)[1]
    readcachecnt = re.findall("reader\.cache\.time.*?count:([0-9]+)",filetext)[1]
    readnocachetime = re.findall("reader\.nocache\.time.*?ave:([0-9.]+)",filetext)[1]
    readnocachecnt = re.findall("reader\.nocache\.time.*?count:([0-9]+)",filetext)[1]
    filtermatchtime = re.findall("filter\.matches\.time.*?ave:([0-9.]+)",filetext)[1]
    filtermatchcnt = re.findall("filter\.matches\.time.*?count:([0-9.]+)",filetext)[1]
    with open(outfilename,'w') as outfile:
        outfile.writelines([readlatency+","+readops+"\n",readzerolatency+","+readzeroops+"\n",
        findtabletime+","+findtablecnt+"\n",internalgettime+","+internalgetcnt+"\n",
        releasetime+","+releasecnt+"\n",blockreadtime+","+blockreadcnt+"\n",
        blocknewtime+","+blocknewcnt+"\n",readfiletime+","+readfinecnt+"\n",
        readcachetime+","+readcachecnt+"\n",readnocachetime+","+readnocachecnt+"\n",
        filtermatchtime+","+filtermatchcnt+"\n"])
        outfile.close()
    infile.close()



