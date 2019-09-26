# -*- coding: utf-8 -*-
"""
Created on Thu Sep  5 18:01:44 2019

@author: Alberto Cruz
:||Attempt at interpretation of json file into pyhton data structure
and later on graph the info
"""
import json
import matplotlib.pyplot as plt
import numpy as np
from matplotlib.widgets import Cursor, Button;
from matplotlib.widgets import MultiCursor
import matplotlib.mlab as mlab
import matplotlib.cbook as cbook


def fmt(x, y):
    return 'x: {x:0.2f}\ny: {y:0.2f}'.format(x = x, y = y)

class DataCursor(object):
    # https://stackoverflow.com/a/4674445/190597
    """A simple data cursor widget that displays the x,y location of a
    matplotlib artist when it is selected."""
    def __init__(self, artists, x = [], y = [], tolerance = 5, offsets = (-20, 20),
                 formatter = fmt, display_all = False):
        """Create the data cursor and connect it to the relevant figure.
        "artists" is the matplotlib artist or sequence of artists that will be
            selected.
        "tolerance" is the radius (in points) that the mouse click must be
            within to select the artist.
        "offsets" is a tuple of (x,y) offsets in points from the selected
            point to the displayed annotation box
        "formatter" is a callback function which takes 2 numeric arguments and
            returns a string
        "display_all" controls whether more than one annotation box will
            be shown if there are multiple axes.  Only one will be shown
            per-axis, regardless.
        """
        self._points = np.column_stack((x,y))
        self.formatter = formatter
        self.offsets = offsets
        self.display_all = display_all
        if not cbook.iterable(artists):
            artists = [artists]
        self.artists = artists
        self.axes = tuple(set(art.axes for art in self.artists))
        self.figures = tuple(set(ax.figure for ax in self.axes))

        self.annotations = {}
        for ax in self.axes:
            self.annotations[ax] = self.annotate(ax)

        for artist in self.artists:
            artist.set_picker(tolerance)
        for fig in self.figures:
            fig.canvas.mpl_connect('pick_event', self)

    def annotate(self, ax):
        """Draws and hides the annotation box for the given axis "ax"."""
        annotation = ax.annotate(self.formatter, xy = (0, 0), ha = 'right',
                xytext = self.offsets, textcoords = 'offset points', va = 'bottom',
                bbox = dict(boxstyle = 'round,pad=0.5', fc = 'yellow', alpha = 0.5),
                arrowprops = dict(arrowstyle = '->', connectionstyle = 'arc3,rad=0')
                )
        annotation.set_visible(False)
        return annotation

    def snap(self, x, y):
        """Return the value in self._points closest to (x, y).
        """
        idx = np.nanargmin(((self._points - (x,y))**2).sum(axis = -1))
        return self._points[idx]
    def __call__(self, event):
        """Intended to be called through "mpl_connect"."""
        # Rather than trying to interpolate, just display the clicked coords
        # This will only be called if it's within "tolerance", anyway.
        x, y = event.mouseevent.xdata, event.mouseevent.ydata
        annotation = self.annotations[event.artist.axes]
        if x is not None:
            if not self.display_all:
                # Hide any other annotation boxes...
                for ann in self.annotations.values():
                    ann.set_visible(False)
            # Update the annotation in the current axis..
            x, y = self.snap(x, y)
            annotation.xy = x, y
            annotation.set_text(self.formatter(x, y))
            annotation.set_visible(True)
            event.canvas.draw()


SeeMtr1 = False
SeeMtr2 = True
SeeMtr3 = False


TimeList_mtr_1 = []
SpeedList_mtr_1 = []
TimeList_mtr_2 = []
SpeedList_mtr_2 = []
TimeList_mtr_3 = []
SpeedList_mtr_3 = []

with open('SpeedTimeTest.json', 'r') as f:
    distros_dict = json.load(f)

for distro in distros_dict:
    #print(distro['Version'])
    TimeList_mtr_1.append(distro['Time_mtr_1'])
    SpeedList_mtr_1.append(distro['Speed_mtr_1'])
    TimeList_mtr_2.append(distro['Time_mtr_2'])
    SpeedList_mtr_2.append(distro['Speed_mtr_2'])
    TimeList_mtr_3.append(distro['Time_mtr_3'])
    SpeedList_mtr_3.append(distro['Speed_mtr_3'])
#Console display for debugging
seperator = ', '
if (SeeMtr1 == True):
    print("Motor 1 time: " + seperator.join(TimeList_mtr_1))
    print("Motor 1 speed: " + seperator.join(SpeedList_mtr_1) +"\n")
if (SeeMtr2 == True):
    print("Motor 2 time: " + seperator.join(TimeList_mtr_2))
    print("Motor 2 speed: " + seperator.join(SpeedList_mtr_2)+"\n")
if (SeeMtr3 == True):
    print("Motor 3 time: " + seperator.join(TimeList_mtr_3))
    print("Motor 3 speed: " + seperator.join(SpeedList_mtr_3)+"\n")

SpeedList_mtr_1 = [ int(x) for x in SpeedList_mtr_1 ]
SpeedList_mtr_2 = [ int(x) for x in SpeedList_mtr_2 ]
SpeedList_mtr_3 = [ int(x) for x in SpeedList_mtr_3 ]
TimeList_mtr_1 = [ int(x) for x in TimeList_mtr_1 ]
TimeList_mtr_2 = [ int(x) for x in TimeList_mtr_2 ]
TimeList_mtr_3 = [ int(x) for x in TimeList_mtr_3 ]

# plotting the points way 1
fig = plt.figure()
ax = fig.add_subplot(111,  facecolor='#ffe8cc')
fig.set_size_inches(18.5, 10.5, forward=True)
ax.set_ylim([8,30])

if (SeeMtr1 == True):
    plt.plot(TimeList_mtr_1, SpeedList_mtr_1, label = "Motor 1",
         color='green', linewidth = 3, marker='o',
          markerfacecolor='blue', markersize=10)
if (SeeMtr2 == True):
    plt.plot(TimeList_mtr_2, SpeedList_mtr_2, label = "Motor 2",
         color='red', linewidth = 3, marker='o',
        markerfacecolor='yellow', markersize=10)
if (SeeMtr3 == True):
    plt.plot(TimeList_mtr_3, SpeedList_mtr_3, label = "Motor 3",
         color='black', linewidth = 3, marker='o',
        markerfacecolor='orange', markersize=10)

cursor = Cursor(ax, horizOn =True, vertOn =True, color= "green", linewidth=2.0)



# naming the x axis 
plt.xlabel('Time - axis') 
# naming the y axis 
plt.ylabel('Speed - axis') 
  
# giving a title to my graph 
plt.title('Time v Speed') 

plt.legend()






# for i,j in zip(TimeList_mtr_1,SpeedList_mtr_1):
#     ax.annotate(str(j), xy=(i,j), xytext=(0,15), textcoords='offset points')
# for k,p in zip(TimeList_mtr_2,SpeedList_mtr_2):
#     ax.annotate(str(p), xy=(k,p), xytext=(0,15), textcoords='offset points')

# function to show the plot
if (SeeMtr1 == True):
    x_1, y_1 = DataCursor(ax, TimeList_mtr_1, SpeedList_mtr_1).offsets
if (SeeMtr2 == True):
    x_2, y_2 = DataCursor(ax, TimeList_mtr_1, SpeedList_mtr_2).offsets
if (SeeMtr3 == True):
    x_3, y_3 = DataCursor(ax, TimeList_mtr_1, SpeedList_mtr_3).offsets

if (SeeMtr1 == True):
    DataCursor(ax, TimeList_mtr_1, SpeedList_mtr_1)
if (SeeMtr2 == True):
    DataCursor(ax, TimeList_mtr_1, SpeedList_mtr_2)
if (SeeMtr3 == True):
    DataCursor(ax, TimeList_mtr_1, SpeedList_mtr_3)
plt.rc('grid', linestyle="-", color='black')
plt.grid(True)
plt.show() 