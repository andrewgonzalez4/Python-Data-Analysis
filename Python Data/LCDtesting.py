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


# with open('LCDtest.json', 'r') as f:
#     lcdjson = json.load(f)
lcdjson = [{"PID": True,
            "Motors":{
                "Motor 1": {
                    "Speed":[10, 15, 20, 25, 30, 35, 40],
                    "Position":[2, 3, 7, 3, -1, 10, 5]
                },
                "Motor 2": {
                    "Speed":[11, 15, 20, 25, 30, 35, 40],
                    "Position":[2, 3, 7, 3, -1, 10, 5]
                },
                "Motor 3": {
                    "Speed":[12, 15, 20, 25, 30, 35, 40],
                    "Position":[2, 3, 7, 3, -1, 10, 5]
                },
                "Motor 4": {
                    "Speed":[13, 15, 20, 25, 30, 35, 40],
                    "Position":[2, 3, 7, 3, -1, 10, 5]
                },
                "Motor 5": {
                    "Speed":[14, 15, 20, 25, 30, 35, 40],
                    "Position":[2, 3, 7, 3, -1, 10, 5]
                },
                "Motor 6": {
                    "Speed":[15, 15, 20, 25, 30, 35, 40],
                    "Position":[2, 3, 7, 3, -1, 10, 5]
                },
                "Motor 8": {
                    "Speed":[16, 15, 20, 25, 30, 35, 40],
                    "Position":[2, 3, 7, 3, -1, 10, 5]
                },
                "Motor 9": {
                    "Speed":[17, 15, 20, 25, 30, 35, 40],
                    "Position":[2, 3, 7, 3, -1, 10, 5]
                }
            },
            "Sensors": True}]
lcdjson2 = [{"PID": True,
            "Motors":
            [
                    [#Motor 1
                        [10, 15, 20, 25, 30, 35, 40],#Speed
                        [2, 3, 7, 3, -1, 10, 5]#Pos
                    ],

                    [
                        [11, 15, 20, 25, 30, 35, 40],
                        [2, 3, 7, 3, -1, 10, 5]
                    ],
                    [
                        [10, 15, 20, 25, 30, 35, 40],
                        [2, 3, 7, 3, -1, 10, 5]
                    ],

                    [
                        [11, 15, 20, 25, 30, 35, 40],
                        [2, 3, 7, 3, -1, 10, 5]
                    ],
                    [
                        [10, 15, 20, 25, 30, 35, 40],
                        [2, 3, 7, 3, -1, 10, 5]
                    ],

                    [
                        [11, 15, 20, 25, 30, 35, 40],
                        [2, 3, 7, 3, -1, 10, 5]
                    ],
                    [
                        [10, 15, 20, 25, 30, 35, 40],
                        [2, 3, 7, 3, -1, 10, 5]
                    ],

                    [
                        [11, 15, 20, 25, 30, 35, 40],
                        [2, 3, 7, 3, -1, 10, 5]
                    ]
            ],

            "Sensors": True}]

MotorList = []
MotorList2 = []
SpeedList = []
SpeedList2 = []
PosList = []
TimeLine = []
initTime = 0
maxVelsize = 0
for files in lcdjson:
    MotorList.append(files['Motors'])
    for motors in MotorList[0]:
        SpeedList.append(MotorList[0][motors]["Speed"])
        PosList.append(MotorList[0][motors]["Position"])
    for sizes in SpeedList:
        if len(sizes) > maxVelsize:
            maxVelsize = len(sizes)
    for values in SpeedList[0]:
        TimeLine.append(initTime)
        initTime += 0.02

for files in lcdjson2:
    MotorList2 = (files['Motors'])
# plotting the points way 1
fig = plt.figure()
ax = fig.add_subplot(111, facecolor='#ffe8cc')
fig.set_size_inches(18.5, 10.5, forward=True)
ax.set_ylim([8, 70])


plt.plot(TimeLine, MotorList2[0][0], label="Motor 1", #Manera 2 [Motor -1][Speed]
             color='green', linewidth=3, marker='o',
             markerfacecolor='blue', markersize=10)

plt.plot(TimeLine, SpeedList[1], label = "Motor 2",#Manera 1 [Motor -1]
         color='red', linewidth = 3, marker='o',
        markerfacecolor='yellow', markersize=10)

plt.plot(TimeLine, SpeedList[2], label = "Motor 3",
         color='black', linewidth = 3, marker='o',
        markerfacecolor='orange', markersize=10)


cursor = Cursor(ax, horizOn=True, vertOn=True, color="green", linewidth=2.0)

# naming the x axis
plt.xlabel('Time - axis')
# naming the y axis
plt.ylabel('Speed - axis')

# giving a title to my graph
plt.title('Time v Speed')

plt.legend()

x_1, y_1 = DataCursor(ax, TimeLine, SpeedList[0]).offsets
DataCursor(ax, TimeLine, SpeedList[0])
x_2, y_2 = DataCursor(ax, TimeLine, SpeedList[1]).offsets
DataCursor(ax, TimeLine, SpeedList[1])
x_3, y_3 = DataCursor(ax, TimeLine, SpeedList[2]).offsets
DataCursor(ax, TimeLine, SpeedList[2])

plt.rc('grid', linestyle="-", color='black')
plt.grid(True)
plt.show()