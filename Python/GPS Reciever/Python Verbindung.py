import serial

arduino = serial.Serial(port='/dev/ttyACM0', baudrate=115200, timeout=.1)

with open("test.txt", "w") as tt:
    while 1:
        msg = arduino.readline().decode("utf-8")
        if not (msg == ""):
            print(msg.strip("\n"))
            tt.write(msg)

