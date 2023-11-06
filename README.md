# Internet of Things: Smart Traffic Light for Discrete Right Turn

[![Open in Visual Studio Code](https://classroom.github.com/assets/open-in-vscode-718a45dd9cf7e7f842a935f5ebbe5719a5e09af4491e668f4dbf3b35d5cca122.svg)](https://classroom.github.com/online_ide?assignment_repo_id=11677311&assignment_repo_type=AssignmentRepo)

This application allows you to use a Teachable Machine Keras model to perform real-time machine learning inference on a live video stream from a USB webcam. The script captures video from the webcam, preprocesses each frame, makes predictions using the Teachable Machine model, and overlays the predicted class on the frame.

## Prerequisites

We will be running this project under the assumption that you are using windows.

Before running this script, you'll need the following:

    Python 3.x installed on your system.
    A USB webcam connected to your computer.
    A Teachable Machine model exported as a Keras .h5 file.
    A Arduino Nano 33 BLE Sense Rev2.
    Ultrasound sensors and 3x lights for Arduino.

## Setting Up the Application

Follow these steps to set up the application:

1.  Clone the Repository
2.  Create a Virtual Environment

    `python -m venv myenv` where myenv is the name of your virtual environment (use `env` for simplicity).

    `myenv\Scripts\activate` to activate the virtual environment

3.  Install Required Libraries

    `pip install -r requirements.txt`

4.  Obtain the Teachable Machine Model
    You'll need a Teachable Machine model exported as a Keras .h5 file. You can create one using the Teachable Machine website provided by Google. Export the model as a Keras .h5 file and save it to the project directory inside `ML_Model`.

5.  Run the Application
    Run the script using the following command:

        ```python server.py```

This will launch the webcam-based machine learning inference application. Press 'q' to exit the webcam stream.

## Arduino File

The `arduino.ino` file is flashed onto the arduino nano 33 ble sense rev2.
The arduino file requires the ultrasound library, which can be found [here](https://wiki.seeedstudio.com/Grove-Ultrasonic_Ranger/).
using arduino IDE, add the ultrasound library as zip, then flash the arduino file onto the arduino.
