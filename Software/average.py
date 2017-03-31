import os
import sys
import subprocess
import math

NUMREADINGS = 5

if len(sys.argv) != 2 or sys.argv[1] not in ["hardware", "software"]:
    print "Usage: %s <software/hardware>" % (sys.argv[0],)
    sys.exit(1)

dirs = ["./triangles/", "./3ddemo/"]

def getreading(dir, exename):
    realdir = os.path.realpath(dir)
    p = subprocess.Popen(["./" + exename, "quiet"], cwd=realdir, stdout=subprocess.PIPE)
    (sout, serr) = p.communicate()
    start = sout.find("time:")+6
    return int(sout[start : sout.find("ms", start)])

def get_mean(vals):
    return float(sum(vals)) / len(vals)

def get_stddev(vals, mean):
    var = sum(map(lambda v: (float(v) - mean)**2, vals)) / len(vals)
    return math.sqrt(var)

for dir in dirs:
    print "Running " + dir + sys.argv[1]
    vals = []
    for i in range(NUMREADINGS):
        vals.append(getreading(dir, sys.argv[1]))

    # Mean
    mean = get_mean(vals)
    print "Mean: %.2f" % mean

    # Std dev
    stddev = get_stddev(vals, mean)
    print "Std dev: %.2f" % stddev
