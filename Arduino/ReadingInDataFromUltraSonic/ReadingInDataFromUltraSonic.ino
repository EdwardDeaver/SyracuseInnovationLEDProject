#include "MegunoLink.h"
#include "Filter.h"
// defines pins numbers
#define trigPin 4
#define echoPin 5
ExponentialFilter<float> FilteredDistance(15, 0);

// defines variables
long duration;
int distance;
const int numReadings = 50;

int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average
bool dataLoop = true;
int mapAverage2;
int mapAverage;
int old; 
int oldData; 
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
    if(RawDistance>200 && RawDistance<9000){
       // subtract the last reading:
      total = total - readings[readIndex];
      // read from the sensor:
      readings[readIndex] = GetDistanceData();
      // add the reading to the total:
      total = total + readings[readIndex];
      // advance to the next position in the array:
      readIndex = readIndex + 1;
    
    // if we're at the end of the array...
      if (readIndex >= numReadings) {
          // calculate the average:
        average = total / numReadings;
    // send it to the computer as ASCII digits
        FilteredDistance.Filter(average);
      
        int mapAverage  = map(FilteredDistance.Current(), 300, 4000, 0, 1000);
         if(mapAverage<1000){
                    Serial.println(average);

         }
        //if(abs(old-mapAverage) < 15){
      
          //Need to figure out how to get (let X be (old+mapAverage)/2)) X1(old)-X2(current) <15);
        //}
      readIndex = 0;
      }
old = map(FilteredDistance.Current(), 300, 4000, 0, 1000);
delay(30);
if(mapAverage>0){
     Serial.println("LOOPED");
          Serial.println(old);
          Serial.println(mapAverage);
          Serial.println((old+mapAverage)/2);
}
    }
   //map(FilteredDistance.Current(), 300, 4000, 0, 255);

    
  
  
  //if(abs(mapAverage2 - mapAverage) < 20){
  //    Serial.println(abs(mapAverage));
 //}
  //delay(20);
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
  //distance= (duration/2)/29.1;
  duration = map(duration, 300, 4000, 0, 1000);
  //Serial.println(duration);
  return duration;
}
