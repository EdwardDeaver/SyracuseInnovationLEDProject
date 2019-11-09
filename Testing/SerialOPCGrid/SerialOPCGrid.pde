// Demonstrates the idea of window based communication. 
import processing.serial.*;
PShape circle;
// Open Pixel Control Object
OPC opc;
float spacing;
float centerX;
float topY;
Serial myPort;  // Create object from Serial class
String val;     // Data received from the serial port



// This creates the mesh.
// The framerate is set at 90. It will never hold at 90 but should stay an an acceptable 50+ fps with minimal dips. 
// opc uses the OPC class to connect to the fcserver over a websocket. 
// spacing is the spacing in between led strip rows. 
// centerX is the center point of the row 
// topY is the height of the row.
// The for loop creates 4 LED strips that are 60 LEDs and sets the pixel value. 

void setup()
{
  frameRate(90);
  size(500, 500, P3D);
  opc = new OPC(this, "127.0.0.1", 7890);
  float spacing = width / 100;
  float centerX = width/2;
  float topY = height/2 - spacing * 3.5;
  for (int i = 0; i < 4; i++) {
    opc.ledStrip(i * 64, 60, centerX, topY + i * spacing, spacing, 0, false);
  }
  String portName = Serial.list()[1]; //change the 0 to a 1 or 2 etc. to match your port
  myPort = new Serial(this, portName, 115200);
  circle = createShape(ELLIPSE, 0, 0, 200, 200);
  circle.setStroke(color(255));  
}

// Draw is like a main method that runs in an infinite while loop. 
// The background is set to black, all LEDs off. 
void draw()
{
  background(0);
  shape(circle);
  while (myPort.available() > 0) {
    val = myPort.readStringUntil('\n');         // read it and store it in val
    println(val);
    float x = float(val);
    if(x<500){
      circle.setFill(color(20));
    }
    else{
      circle.setFill(color(100));
    }
  }
}
