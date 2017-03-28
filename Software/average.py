import sys
import subprocess
import math

NUMREADINGS = 5

if len(sys.argv) != 2:
	print "Usage: %s <software/hardware>" % (sys.argv[0],)
	sys.exit(1)

if sys.argv[1] == "hardware":
	exename = "./triangles/hardware"
else:
	exename = "./triangles/software"

def getreading():
	p = subprocess.Popen([exename, "quiet"], stdout=subprocess.PIPE)
	(sout, serr) = p.communicate()
	return int(sout[sout.find(":")+2 : sout.find("/")])

vals = []
for i in range(NUMREADINGS):
	vals.append(getreading())

# Mean
total = 0.0
for i in vals:
	total += i
mean = float(total) / len(vals)
print "Mean: %.2f" % mean

# Std dev
difftotal = 0.0
for i in vals:
	difftotal += (i - mean)**2
var = difftotal / len(vals)
stddev = math.sqrt(var)
print "Std dev: %.2f" % stddev
