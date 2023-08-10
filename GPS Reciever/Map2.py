import serial
import pygame
import math

pygame.init()

screen = pygame.display.set_mode((1000, 1000))

arduino = serial.Serial(port='/dev/ttyACM0', baudrate=115200, timeout=.1)

latCenter = 0
lonCenter = 0
Centered = False
lastlatRest = 0
lastlonRest = 0

firstCount = 0

while 1:
    msg = arduino.readline().decode("utf-8")
    firstCount += 1
    if firstCount > 50:
        if not (msg == ""):
            msg = msg.strip(";\n")
            msg = msg.split(",")
            if len(msg) == 3:
                if int(msg[2]) > 6:
                    for i in range(len(msg)):
                        msg[i] = float(msg[i])
                    print(msg[2])

                    latRest = float(msg[0]) * 1000 * 500
                    lonRest = float(msg[1]) * 1000 * 500
                    if not Centered:
                        latCenter = latRest
                        lonCenter = lonRest
                        Centered = True
                    else:
                        pygame.draw.line(screen, (255, 255, 255), (lastlatRest - latCenter + 500, lastlonRest - lonCenter + 500), (latRest - latCenter + 500, lonRest - lonCenter + 500))

                    lastlatRest = latRest
                    lastlonRest = lonRest
                    pygame.display.flip()
