import serial
import pygame
import math

pygame.init()

screen = pygame.display.set_mode((1000, 1000))

arduino = serial.Serial(port='/dev/ttyACM0', baudrate=115200, timeout=.1)

while 1:
    msg = arduino.readline().decode("utf-8")
    if not (msg == ""):
        msg = msg.strip(";\n")
        msg = msg.split(",")
        print(msg)
