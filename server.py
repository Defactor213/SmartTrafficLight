# server.py acts as the central hub that processes the logic of the webcam using Machine Learning
# import numpy as np
# teachable machine library
from teachable_machine import TeachableMachine

# opencv library: computer vision
import cv2 as cv

# time library: to wait between taking screenshots, can remove whenever
import time

# serial library: to send data to arduino
import serial
import json
import time

# VideoCapture to use webcam camera, 0 is default webcam on laptop, but usb webcam will override
#   (not sure if its the webcam software or the laptop software that does the override but we take those)
cap = cv.VideoCapture(0)

# Load the Teachable Machine Keras model
model = TeachableMachine(
    model_path="./ML_Model/keras_model.h5", labels_file_path="./ML_Model/labels.txt"
)

# serial port to send data to arduino
ser = serial.Serial("COM3", 9600)


# declare and store image somewhere
image_path = "./screenshot.jpg"

data_file = open("data.csv", "a")
cur_data = ""

server_start_time = time.time()
light_start_time = time.time()

while True:
    # read the image from the webcam
    _, img = cap.read()
    # save the image to the image_path
    cv.imwrite(image_path, img)

    # make predictions using the model
    result = model.classify_image(image_path)

    print("class_name:", result["class_name"])
    print("class_confidence: {:.2f}".format(round(result["class_confidence"], 2)))

    ser.write(str(result["class_index"]).encode())

    # display the image
    cv.imshow("Webcam", img)

    # if q is pressed, break out of the loop
    if cv.waitKey(1) & 0xFF == ord("q"):
        break

    # read a line from the serial port, as bytes
    line = ser.readline()

    # # read a line from the serial port, decode bytes to string
    line = line.decode("utf-8")

    # message should end with \n, this removes the \n and all other whitespace before and after the message
    line = line.strip()

    # if the read was successful, do something with it
    if line == "No change":
        print(line)

    elif line and line != "":
        print(f"=== ARDUINO DATA START ===")
        print(f"{line}")
        # print("Time:", time.time())

        # light has been on for X seconds since server started and current time
        light_time = time.time() - light_start_time
        # set current time as new light start time
        light_start_time = time.time()

        # recording of data
        print(line.split(","))
        light_color, class_index, range_in_centimeters, delay = line.split(",")
        data = {
            "timestamp": round(time.time()),
            "class_index": class_index,
            "range_in_centimeters": range_in_centimeters,
            "light_color": light_color,
            "time_light_has_been_on": light_time,
        }

        filename = "./logs/data.json"

        with open(filename) as file:
            listObj = json.load(file)

        listObj.append(data)

        with open(filename, "w") as json_file:
            json.dump(listObj, json_file, separators=(",", ": "))

        print(f"=== ARDUINO DATA END ===\n")
