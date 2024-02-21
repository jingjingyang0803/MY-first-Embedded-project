#!/usr/bin/env python3

import time

class Foreground:
    Green = "\033[38;5;46m"
    Yellow = "\033[38;5;226m"
    Red = "\033[38;5;196m"

class Background:
    Grey = "\033[48;5;248m"
    DarkGrey = "\033[48;5;238m"
    Green = "\033[48;5;46m"
    Yellow = "\033[48;5;226m"
    Red = "\033[48;5;196m"

Reset = "\033[0;0m"

def Position(x, y):
    return f"\033[{x};{y}H"

def Crossroad(tf1, tf2):
    VerticalRoad()
    HorizontalRoad()
    TrafficLight1(tf1)
    TrafficLight2(tf2)

def VerticalRoad():
    print(f"{Background.Grey}{Foreground.Red}")
    print(f"{Position(4, 25)}   |   ")
    print(f"{Position(5, 25)}   |   ")
    print(f"{Position(6, 25)}   |   ")
    print(f"{Position(7, 25)}   |   ")
    print(f"{Position(8, 25)}   |   ")
    print(f"{Position(9, 25)}   |   ")
    print(f"{Position(10, 25)}   |   ")
    print(f"{Position(11, 25)}   |   ")
    print(f"{Position(12, 25)}   |   ")
    print(f"{Position(13, 25)}   |   ")
    print(f"{Position(14, 25)}       ")
    print(f"{Position(15, 25)}       ")
    print(f"{Position(16, 25)}       ")
    print(f"{Position(17, 25)}   |   ")
    print(f"{Position(18, 25)}   |   ")
    print(f"{Position(19, 25)}   |   ")
    print(f"{Position(20, 25)}   |   ")
    print(f"{Position(21, 25)}   |   ")
    print(f"{Position(22, 25)}   |   ")
    print(f"{Position(23, 25)}   |   ")
    print(f"{Position(24, 25)}   |   ")
    print(f"{Position(25, 25)}   |   ")
    print(f"{Position(26, 25)}   |   ")

def HorizontalRoad():
    print(f"{Background.Grey}{Foreground.Red}")
    print(f"{Position(14, 0)}                                                       ")
    print(f"{Position(15, 0)}-------------------------------------------------------")
    print(f"{Position(16, 0)}                                                       ")

def TrafficLight1(state):
    print(f"{Position(12, 34)}{Background.DarkGrey}      ")

    if state == 0:
        print(f"{Position(12, 34)}{Background.Red}  ")
    elif state == 1:
        print(f"{Position(12, 34)}{Background.Red}  ")
        print(f"{Position(12, 36)}{Background.Yellow}  ")
    elif state == 2:
        print(f"{Position(12, 38)}{Background.Green}  ")
    elif state == 3:
        print(f"{Position(12, 36)}{Background.Yellow}  ")

def TrafficLight2(state):
    print(f"{Background.DarkGrey}")
    print(f"{Position(18, 34)}   ")
    print(f"{Position(19, 34)}   ")
    print(f"{Position(20, 34)}   ")

    if state == 0:
        print(f"{Background.Red}{Position(18, 34)}   ")
    elif state == 1:
        print(f"{Background.Red}{Position(18, 34)}   ")
        print(f"{Background.Yellow}{Position(19, 34)}   ")
    elif state == 2:
        print(f"{Background.Green}{Position(20, 34)}   ")
    elif state == 3:
        print(f"{Background.Yellow}{Position(19, 34)}   ")


while True:
    Crossroad(0, 2)
    time.sleep(2)
    Crossroad(1, 3)
    time.sleep(2)
    Crossroad(2, 0)
    time.sleep(2)
    Crossroad(3, 1)
    time.sleep(2)
