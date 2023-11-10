from matplotlib import pyplot as plt

file = open("Raw Data/Drive0003.txt", "r")

raw_data = file.readlines()

data = []
for i in raw_data:
    data.append(i.strip("\n"))

listlist = []
for i in data:
    i = i.split(",")
    listlist.append(i)

speeds = []
indexes = []
for i in range(len(listlist)):
    speeds.append(float(listlist[i][4]))
    indexes.append(i)

for i in speeds:
    print(i)
plt.plot(speeds)

plt.show()
