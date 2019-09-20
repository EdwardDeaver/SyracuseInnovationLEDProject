// Demonstrates the idea of window based communication. 

// Open Pixel Control Object
OPC opc;
float spacing;
float centerX;
float topY;

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
  colorMode(HSB,100);
  size(500, 500, P3D);
  opc = new OPC(this, "127.0.0.1", 7890);
  float spacing = width / 100;
  float centerX = width/2;
  float topY = height/2 - spacing * 3.5;
  for (int i = 0; i < 4; i++) {
    opc.ledStrip(i * 64, 60, centerX, topY + i * spacing, spacing, 0, false);
  }
}

// Draw is like a main method that runs in an infinite while loop. 
// The background is set to black, all LEDs off. 
void draw()
{
  background(0);
  println(frameRate);
  
  // Pulsing equation 
  // source: https://processing.org/discourse/beta/num_1259751964.html
  // The eeqution is used i the fill logic and rectangle size.
  float pulse = abs(sin(frameCount/20.0))*10.0;
  float a = 30;
  
  // Top row rectangles
  fill(height/2, abs(sin(frameCount/20))*10, frameCount/20);
  rect(width/2, height/(2.17),a+pulse*10,5);
  fill(50,20,frameCount);
  rect(width/2, height/(2.17),-1*(a+pulse*10),5);
  
  // 2nd row rectangles    
  fill(frameCount,20, frameCount/2);
  rect(width/2, height/(2.13),a+pulse*10,5);
  fill(200,20,10);
  rect(width/2, height/(2.13),-1*(a+pulse*10),5);
  
  // 3rd row rectangles
  fill(frameCount,20, frameCount/2);
  rect(width/2, height/(2.09),a+pulse*10,5);
  fill(50,100,255);
  rect(width/2, height/(2.09),-1*(a+pulse*10),5);
  
  // 4th row rectangles
  fill(frameCount,20, frameCount/2);
  rect(width/2, height/(2.05),a+pulse*10,5);
  fill(50,20,200);
  rect(width/2, height/(2.05),-1*(a+pulse*10),5);
}
