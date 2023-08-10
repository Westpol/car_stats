import math

val = 51.4877090454

latRest = (float(val) - (math.floor(float(val) * 100.0) / 100.0)) * 1000 * 100

print(latRest)
