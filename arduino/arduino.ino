// declare libraries
#include "Ultrasonic.h"

// 0 Accident/Breakdown
// 1 Road Vehicles
// 2 Pedestrians
// 3 Not Pedestrian or Vehicles
// 4 Disabled Pedestrians
// remapping Labels in the mapLabels method (see below)

// declare pins and variables
Ultrasonic ultrasonic(19);
const int LEDRed = 11;
const int LEDGreen = 12;
const int LEDPurple = A6;

// variable to store distance of ultrasonic sensor
long RangeInCentimeters;

int range_threshold = 15;

// 0 is LOW, 1 is HIGH, variable to store state of lights
int LEDGreenState = 0;
int LEDPurpleState = 0;
int LEDRedState = 0;

// setting up hardware connections
void setup()
{
    pinMode(LEDRed, OUTPUT);
    pinMode(LEDPurple, OUTPUT);
    pinMode(LEDGreen, OUTPUT);
    Serial.begin(9600);
}

// store previous class_index
int prev_class_index = -1;

// previous range value
int prev_range_index = -1;

// store duration of amber light in ms
int amber_duration = 200;
int red_duration = 300;
int disabled_duration = 600;

// loop to check for stuff and send stuff
void loop()
{
    // Initialize class_index to an invalid value
    int class_index = -1;

    // output variable
    String LEDOutput;
    String delayDuration;

    // temp variables to compare when lights change
    int currentGreenState = LEDGreenState;
    int currentPurpleState = LEDPurpleState;
    int currentRedState = LEDRedState;
    int unmappedIndex = -1;

    // measure distance
    RangeInCentimeters = ultrasonic.MeasureInCentimeters(); // Measure the distance

    // Receive data from Python
    if (Serial.available() > 0)
    {
        unmappedIndex = Serial.parseInt(); // Read the integer sent from Python
        class_index = mapLabels(unmappedIndex);
        int ultrasonic_index = (RangeInCentimeters <= range_threshold) ? 2 : (prev_range_index == 2) ? 1
                                                                                                     : 0;

        // if prev_class_index light = red, new class_index = green, change class_index to 9 to show amber light first
        class_index = (prev_class_index == 2 && class_index == 0) ? 9 : class_index;

        // TODO: fix light stuck at amber after pedestrians detected
        if (ultrasonic_index <= class_index)
        {
            switch (class_index)
            {
            case 0:
                LEDRedState = 0;
                LEDPurpleState = 0;
                LEDGreenState = 1;
                LEDOutput = "Green";
                break;
            case 1:
                // maybe change later, not really sure in what case car appears
                LEDRedState = 1;
                LEDPurpleState = 0;
                LEDGreenState = 0;
                LEDOutput = "Red";
                delayDuration = "3";
                break;
            case 2:
                LEDRedState = 1;
                LEDPurpleState = 0;
                LEDGreenState = 0;
                LEDOutput = "Red";
                delayDuration = "2";
                break;
            case 3:
                LEDRedState = 1;
                LEDPurpleState = 0;
                LEDGreenState = 0;
                LEDOutput = "Red";
                // delay(disabled_duration);
                delayDuration = "6";
                break;
            case 4:
                LEDRedState = LEDPurpleState = LEDGreenState = 1; // maybe replace by a method that lets LEDs flash
                LEDOutput = "Crash";
                break;
            case 9:
                LEDRedState = 0;
                LEDPurpleState = 1;
                LEDGreenState = 0;
                LEDOutput = "Orange";
                break;
            }
        }
        else
            switch (ultrasonic_index)
            {
            case 0:
                LEDRedState = 0;
                LEDPurpleState = 0;
                LEDGreenState = 1;
                LEDOutput = "Green";
                break;
            case 1:
                LEDRedState = 0;
                LEDPurpleState = 1;
                LEDGreenState = 0;
                LEDOutput = "Orange";
                break;
            case 2:
                LEDRedState = 1;
                LEDPurpleState = 0;
                LEDGreenState = 0;
                LEDOutput = "Red";
                break;
            }

        // switching LEDs on/off
        digitalWrite(LEDGreen, LEDGreenState);
        digitalWrite(LEDPurple, LEDPurpleState);
        digitalWrite(LEDRed, LEDRedState);

        prev_class_index = class_index;
        prev_range_index = ultrasonic_index;
        // send data to server whenever light changes
        if (
            ((currentGreenState != LEDGreenState) || (currentPurpleState != LEDPurpleState) || (currentRedState != LEDRedState)))
        {
            if (LEDOutput != "" && class_index > -1 && RangeInCentimeters > 0)
            {
                Serial.print(LEDOutput + ",");
                Serial.print(String(unmappedIndex + 1) + ",");
                Serial.print(String(RangeInCentimeters) + ",");
                Serial.print("delayDuration: " + delayDuration + "s");
                Serial.println();
            }
            else
            {

                Serial.println("No change");
            }
        }
        else
        {
            Serial.println("No change");
        }
    }
}

// priority:
// 0 Not Pedestrian or Vehicles
// 1 Road Vehicles
// 2 Pedestrians
// 3 Disabled Pedestrians
// 4 Accident/Breakdown

int mapLabels(int i)
{
    if (i == 0)
    {
        return 4; // Accident/Breakdown
    }
    else if (i == 1)
    {
        // maybe change later, not really sure in what case car appears
        return 1; // Road Vehicles
    }
    else if (i == 2)
    {
        return 2; // Pedestrians
    }
    else if (i == 3)
    {
        return 0; // Not Pedestrian or Vehicles
    }
    else if (i == 4)
    {
        return 3; // Disabled Pedestrians
    }
    return -1;
}
