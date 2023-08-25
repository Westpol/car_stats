import math


def haversine_distance(coord1, coord2):
    # Radius of the Earth in meters
    r = 6371000

    lat1, lon1 = coord1
    lat2, lon2 = coord2

    # Convert degrees to radians
    lat1 = math.radians(lat1)
    lon1 = math.radians(lon1)
    lat2 = math.radians(lat2)
    lon2 = math.radians(lon2)

    # Haversine formula
    dlat = lat2 - lat1
    dlon = lon2 - lon1
    a = math.sin(dlat / 2) ** 2 + math.cos(lat1) * math.cos(lat2) * math.sin(dlon / 2) ** 2

    # Ensure the argument of the square root is non-negative and within valid range
    a = min(1, max(0, a))

    c = 2 * math.atan2(math.sqrt(a), math.sqrt(1 - a))

    # Calculate distance
    _distance = r * c
    return _distance


file = open("Drive0003.txt", "r")

overallDistance = 0
firstRun = True

for i in file.readlines():
    msg = i.split(",")[3].split("$")
    spd = i.split(",")[4]
    if not firstRun:
        distance = haversine_distance(lastCoord, (float(msg[0]), float(msg[1])))
        overallDistance += distance
    else:
        firstRun = False
    lastCoord = (float(msg[0]), float(msg[1]))

print("Distance:", overallDistance, "meters")
