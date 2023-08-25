import serial


def data_handling(message, file_obj):
    if not (message == ""):
        message = message.split(",")
        if len(message) == 8:
            if int(message[2]) > 6:
                data = ""
                for i in message:
                    i = i.strip(";\n")
                    data += i
                    data += ","
                data = data[:-1]
                if (counter % 10) == 0:
                    print(data)
                file_obj.write(data + "\n")


while 1:
    try:
        arduino = serial.Serial(port='/dev/ttyACM0', baudrate=115200, timeout=.1)
        msg = arduino.readline().decode("utf-8")
        break
    except:
        print("GPS not connected")

file = open("Drive0003.txt", "w")

counter = 0
while 1:
    try:
        counter += 1
        msg = arduino.readline().decode("utf-8")
        data_handling(msg, file)
    except:
        file.close()
        exit()
