import cv2 as cv
from teachable_machine import TeachableMachine
import serial

# Load the Teachable Machine Keras model
model = TeachableMachine(
    model_path="./ML_Model/keras_model.h5", labels_file_path="./ML_Model/labels.txt"
)

# Serial port to send data to Arduino
arduino_port = "COM8"  # Change this to the correct port
ser = serial.Serial(arduino_port, 9600)

# VideoCapture to use the webcam camera, 0 is the default webcam on a laptop, but USB webcam will override
cap = cv.VideoCapture(0)

# Declare and store the image somewhere
image_path = "./screenshot.jpg"

# Open a data file to append data
data_file_path = "data.csv"
data_file = open(data_file_path, "a")
cur_data = ""




print('Starting program')

while True:
    # Read a line from the serial port, as bytes
    line = ser.readline()

    # Decode bytes to string
    line = line.decode("utf-8")

    # Remove whitespace before and after the message
    line = line.strip()
    print(line + ' cm')

    # Always display the webcam feed
    _, img = cap.read()
    cv.imshow("Webcam", img)
    
    # If the read was successful and the range is less than 150, scan the image
    if int(line) <= 150:
        # Save the image to the image_path
        cv.imwrite(image_path, img)

        # Make predictions using the model
        result = model.classify_image(image_path)

        print("Class name:", result["class_name"])
        print("Class confidence: {:.2f}".format(round(result["class_confidence"], 2)))

        # write the data to the file
        cur_data = line + "," + result["class_name"] + "," + str(result["class_confidence"]) + "\n"
        data_file.write(cur_data)

    # If 'q' is pressed, break out of the loop
    if cv.waitKey(1) & 0xFF == ord("q"):
        break

# Release resources
cap.release()
cv.destroyAllWindows()
ser.close()
data_file.close()
