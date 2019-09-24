#include "MegunoLink.h"
#include "Filter.h"
// defines pins numbers
#define trigPin 4
#define echoPin 5
ExponentialFilter<float> FilteredDistance(5, 0);

// defines variables
long duration;
int distance;
//int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average


void setup() {
pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin, INPUT); // Sets the echoPin as an Input
Serial.begin(9600); // Starts the serial communication

}

void loop() {
    
    float RawDistance = GetDistanceData();
    

      
    
    FilteredDistance.Filter(RawDistance);
  int SmoothDistance = FilteredDistance.Current();
  byte myInt = (byte)SmoothDistance;
  Serial.write(myInt);
  delay(50);


}

int GetDistanceData(){
// Clears the trigPin
digitalWrite(trigPin, LOW);
delayMicroseconds(5);

// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);

// Reads the echoPin, returns the sound wave travel time in microseconds
duration = pulseIn(echoPin, HIGH);

// Calculating the distance
distance= (duration/2)/29.1;

return distance;
}
