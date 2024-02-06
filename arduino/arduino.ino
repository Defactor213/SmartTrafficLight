// declare libraries
#include "Ultrasonic.h"

// declare pins and variables
Ultrasonic ultrasonic(19);

// variable to store distance of ultrasonic sensor
long RangeInCentimeters;

int range_threshold = 15;

// setting up hardware connections
void setup()
{
    Serial.begin(9600);
}

// loop to check for stuff and send stuff
void loop()
{
    // measure distance
    RangeInCentimeters = ultrasonic.MeasureInCentimeters(); // Measure the distance

    // Log the range
    Serial.print(RangeInCentimeters);
    Serial.println();

    // Add a delay to avoid overwhelming the serial port
    delay(500);  // Adjust the delay time as needed
}
