#!/usr/bin/env python # -*- coding: utf-8 -*-

import math
import time


def get_time():
    global current_interval
    return current_interval

# =============================================================================
# =============================================================================
# =============================================================================


class MecanumOdometry:
    # THANK YOU
    # https://www.isg-stuttgart.de/fileadmin/user_upload/virtuos/Heavy-Duty_Omni-Directional_Mecanum-Wheeled_Robot_for.pdf
    lx, ly, = 0, 0
    X, Y, T = 0, 0, 0
    r = 0
    PI = 3.14159265358979323846264338297

    # When using absolute position ONLY
    prev_t = time.time()
    dt = prev_t
    prev_s = [0, 0, 0, 0]

    def __init__(self, width, length, wheel_radius):
        self.lx = width / 2
        self.ly = length / 2
        self.prev_t = time.time()
        self.r = wheel_radius

    def calculate_pose(self, lf, lb, rf, rb):
        current_t = get_time()
        dt = current_t - self.prev_t

        ds = [(lf - self.prev_s[0])*(2*self.PI*self.r),     # Front left
                (lb - self.prev_s[1])*(2*self.PI*self.r),     # Back left
                (rf - self.prev_s[2])*(2*self.PI*self.r),     # Front right
                (rb - self.prev_s[3])*(2*self.PI*self.r)]     # Back right

        self.prev_s = [lf, lb, rf, rb]

        # (FrontRight + FrontLeft + BackLeft + BackRight) / 4
        dy_r = sum(ds)/4
        # (-FrontRight + FrontLeft - BackLeft + BackRight) / 4 
        dx_r = (-ds[2] + ds[0] - ds[1] + ds[3])/4
        # (FrontRight - FrontLeft - BackLeft + BackRight) / 4 * lr
        lr = (self.lx**2 + self.ly**2)**0.5
        dt_r = (ds[2] - ds[0] - ds[1] + ds[3])/(4*lr)

        theta = self.get_T()
        dx = dx_r*math.cos(theta) - dy_r * math.sin(theta)
        dy = dx_r*math.sin(theta) + dy_r * math.cos(theta)

        self.X += dx*dt
        self.Y += dy*dt
        self.T += dt_r*dt

        self.prev_t = current_t

    def get_X(self):
        return self.X

    def set_X(self, x):
        self.X = x

    def get_Y(self):
        return self.Y

    def set_Y(self, y):
        self.Y = y

    def get_T(self, degrees=False):
        if bool(degrees):
            return (180.0 / self.PI) * self.T
        else:
            return self.T

    def set_T(self, t, degrees=False):
        if bool(degrees):
            self.T = (self.PI / 180.0) * t
        else:
            self.T = t


# =============================================================================
# =============================================================================
# =============================================================================


import random
import matplotlib.pyplot as plt


def test():
    global current_interval

    # w = 287.00mm
    # l = 250.9839999999mm
    odometry = MecanumOdometry(11.2992, 9.88, 2)

    t = []
    Xs = []
    Ys = []
    Ts = []
    dlfs = []
    dlbs = []
    drfs = []
    drbs = []

    current_interval = 0

    for i in range(10):
        current_interval = i
        #  dlf = random.randint(-i, i)
        #  dlb = random.randint(-i, i)
        #  drb = random.randint(-i, i)
        #  drf = random.randint(-i, i)
        dlf, dlb, drf, drb = -i, -i, 0, 0

        odometry.calculate_pose(dlf, dlb, drf, drb)

        Xs.append(odometry.get_X())
        Ys.append(odometry.get_Y())
        Ts.append(odometry.get_T(True))
        dlfs.append(dlf)
        dlbs.append(dlb)
        drfs.append(drf)
        drbs.append(drb)
        t.append(i)

        time.sleep(0.1)

    plt.subplot(3, 2, 1)
    plt.plot(t, Xs, '.-')
    plt.title('Odometry')
    plt.ylabel('X Position')

    plt.subplot(3, 2, 3)
    plt.plot(t, Ys, '.-')
    plt.ylabel('Y Position')

    plt.subplot(3, 2, 5)
    plt.plot(t, Ts, '.-')
    plt.xlabel('time (s)')
    plt.ylabel('Angle (degrees)')

    plt.subplot(3, 2, 2)
    plt.plot(t, dlfs, '.-', label='d Left Front')
    plt.plot(t, dlbs, '.-', label='d Left Back')
    plt.legend(loc='lower right')
    plt.ylabel('Change in Left')

    plt.subplot(3, 2, 4)
    plt.plot(t, drfs, '.-', label='d Right Front')
    plt.plot(t, drbs, '.-', label='d Right Back')
    plt.legend(loc='lower right')
    plt.ylabel('Change in Right')

    plt.subplot(3, 2, 6)
    plt.plot(Xs, Ys, '.-')
    plt.xlabel('X Position')
    plt.ylabel('Y Position')

    plt.show()


if __name__ == "__main__":
    test()
