import pygame

pygame.init()

screen = pygame.display.set_mode((1000, 1000))

latCenter = 0
lonCenter = 0
Centered = False
lastlatRest = 0
lastlonRest = 0

firstCount = 0

file = open("Drive0001.txt", "r")

for i in file.readlines():
    msg = i.split(",")[3].split("$")
    latRest = float(msg[0]) * 1000 * 7
    lonRest = float(msg[1]) * 1000 * 7
    if not Centered:
        latCenter = latRest
        lonCenter = lonRest
        Centered = True
    else:
        pygame.draw.line(screen, (255, 255, 255), (lastlatRest - latCenter + 500, lastlonRest - lonCenter + 500), (latRest - latCenter + 500, lonRest - lonCenter + 500))

    lastlatRest = latRest
    lastlonRest = lonRest
    pygame.display.flip()

while 1:
    pygame.event.pump()
