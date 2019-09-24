/**
 * Simple Read
 * 
 * Read data from the serial port and change the color of a rectangle
 * when a switch connected to a Wiring or Arduino board is pressed and released.
 * This example works with the Wiring / Arduino program that follows below.
 */


import processing.serial.*;

Serial myPort;  // Create object from Serial class
int val;      // Data received from the serial port

void setup() 
{
fullScreen();
  // I know that the first port in the serial list on my mac
  // is always my  FTDI adaptor, so I open Serial.list()[0].
  // On Windows machines, this generally opens COM1.
  // Open whatever port is the one you're using.
  colorMode(HSB, 100);
  String portName = Serial.list()[3];
    println(Serial.list());

  myPort = new Serial(this, portName, 9600);
}

void draw()
{
    byte[] inBuffer = new byte[7];
int total =0;
int complete = 0;
  while (myPort.available() > 0) {
    inBuffer = myPort.readBytes();
    myPort.readBytes(inBuffer);
    if (inBuffer != null) {
      int[] myString =  int(inBuffer);
      for(int i =0; i<myString.length;i++){
        total = total + myString[i];
      }
      println(myString);
      println("total"+myString[0]);
      complete = myString[0];
    }
  }
  println("TOTALCOMPLETE"+complete);
  if(complete<100 && complete>10){
     float newValue = map(complete, 11, 99, 0, 255);

      fill(complete, complete, complete);
      rect(newValue/(width/2), newValue/(height/2), newValue*4, newValue*4);
      
      delay(30);
  }
}
