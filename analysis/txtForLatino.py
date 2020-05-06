#!/usr/bin/env python
import sys, os, subprocess, string, re
from optparse import OptionParser

def main():
    parser = OptionParser()
    parser.add_option("-t", "--txt_file", dest="txt_file",
                    help="Input txt file",
                    metavar="TXTFILE")
    (options, args) = parser.parse_args()
    txt_file = options.txt_file
    makeTable(txt_file)


def makeTable(fname):
    f = file(fname)
    for line in f:
        line = [ float(l) for l in line.split() ] if len(line.split()) == 12 else [0,0]
        if len(line) != 12:
            continue
        line[8] = abs((line[4] - line[8]))
        line[9] = abs((line[4] - line[9]))
        line[10] = abs((line[6] - line[10]))
        line[11] = abs((line[6] - line[11]))
        for n in line:
            print '{:8.3f}'.format(n),
        print ''
    f.close()

if __name__ == "__main__":
    # makeTable("./results/2018Data/tnpEleID/runABCD_HWW/passingMVA94Xwp90isoHWWiso0p06/egammaEffi.txt")
    main()


