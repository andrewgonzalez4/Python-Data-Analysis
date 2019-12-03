from math import sin,cos
import random
import matplotlib.pyplot as plt
import numpy as np

getX = lambda t,sr,sl,L : ((sr+sl)/2)*cos(t+(sr-sl)/(4*L))
getY = lambda t,sr,sl,L : ((sr+sl)/2)*sin(t+(sr-sl)/(4*L))
getTheta = lambda sr,sl,L : ((sr-sl)/(2*L))
getPose = lambda t,sr,sl,L: (getX(t,sr,sl,L), getY(t,sr,sl,L), getTheta(sr,sl,L))
toDegrees = lambda r: ((r/3.141592653589793238462643383)*180.0)

def cumSum(l):
    accumulator = 0
    sums = []
    for i in range(len(l)):
        accumulator += l[i]
        sums.append(accumulator)

    return sums

history = {"sl":[], "sr":[], "x":[], "y":[], "theta":[]}
x, y, theta = 0, 0, 0
for c in range(100):
    sr, sl, L = random.random(), random.random(), 7.5
    x,y,theta = getPose(theta,sr,sl,L)

    x = round(x,2)
    y = round(y,2)
    temp = round(toDegrees(theta), 2)

    history["x"].append(x)
    history["y"].append(y)
    history["theta"].append(temp)
    history["sl"].append(sl)
    history["sr"].append(sr)


fig, (ax1, ax2, ax3, ax4) = plt.subplots(4)

ax1.plot(cumSum(history["x"]), cumSum(history["y"]))
ax2.plot([x for x in range(len(history["theta"]))], history["theta"])
ax3.plot([x for x in range(len(history["sr"]))], history["sr"])
ax3.plot([x for x in range(len(history["sl"]))], history["sl"])

plt.show()
