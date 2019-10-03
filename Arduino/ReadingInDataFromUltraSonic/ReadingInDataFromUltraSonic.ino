// defines pins numbers
#define trigPin 4
#define echoPin 5

// defines variables
long duration;
int distance;
const int numReadings = 30;

int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
float total = 0;                  // the running total
int average = 0;                // the average 

// Sets up the pins to be input/outout 
void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(115200); // Starts the serial communication
}
// Main loop in C++ arduino. This will run forever. 
///////////////////
// Sends data via serial:
// Output: 
// 5000 == 30 inches
// 10000 == 60 inches
// 15000 == 90 inches
void loop() {
     
       // Adds current reading to total
      total = total + GetDistanceData();
      // next input count:
      readIndex = readIndex + 1;
     // if we're at the max number of readings
      if (readIndex >= numReadings) {
        // calculate the average:
        average = total / numReadings;        
        Serial.println(average);
        readIndex = 0;
        total = 0;
      }
      delay(40);
}
//
// returns the number of CM of the distance
int GetDistanceData(){
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH); 
  // Calculating the distance
  distance= duration*0.034/2;
  return duration;
}
