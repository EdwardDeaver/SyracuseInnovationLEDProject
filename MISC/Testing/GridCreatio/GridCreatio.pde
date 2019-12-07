
// Demonstration that pokes colors into the LEDs directly instead
// of mapping them to pixels on the screen. You may want to do this
// if you have your own mapping scheme that doesn't fit well with
// a 2D display.

OPC opc;
PImage dot;
PImage im;
int numberOfLEDS = 4*60;

  float spacing;
float centerX;
float topY;
void setup()
{
  frameRate(60);

    size(500, 500, P2D);

  opc = new OPC(this, "127.0.0.1", 7890);
  im = loadImage("flames.jpeg");

      float spacing = width / 100;
float centerX = width/2;
float topY = height/2 - spacing * 3.5;
for (int i = 0; i < 4; i++) {
    opc.ledStrip(i * 64, 60, centerX, topY + i * spacing, spacing, 0, false);
}


}

void draw()
{
  println(frameRate);

  background(0);

  float pulse = abs(sin(frameCount/20.0))*10.0;
  float a = 30;
//fill(height/2, abs(sin(frameCount/20))*10, frameCount/20);
fill(255,20,0);
      rect(width/2, height/(2.17),a+pulse*10,5);
      fill(50,20,frameCount);
      rect(width/2, height/(2.17),-1*(a+pulse*10),5);
      
      fill(frameCount,20, frameCount/2);
      rect(width/2, height/(2.13),a+pulse*10,5);
      fill(200,20,10);
      rect(width/2, height/(2.13),-1*(a+pulse*10),5);

      fill(frameCount,20, frameCount/2);
      rect(width/2, height/(2.09),a+pulse*10,5);
      fill(50,100,255);
      rect(width/2, height/(2.09),-1*(a+pulse*10),5);
      
      fill(frameCount,20, frameCount/2);
      rect(width/2, height/(2.05),a+pulse*10,5);
      fill(50,20,200);
      rect(width/2, height/(2.05),-1*(a+pulse*10),5);
      
      // Scale the image so that it matches the width of the window
  int imHeight = im.height * width / im.width;

  // Scroll down slowly, and wrap around
  float speed = 0.05;
  float y = (millis() * -speed) % imHeight;
  
  // Use two copies of the image, so it seems to repeat infinitely  
  image(im, 0, y, width, imHeight);
  image(im, 0, y + imHeight, width, imHeight);
  // When you haven't assigned any LEDs to pixels, you have to explicitly
  // write them to the server. Otherwise, this happens automatically after draw().

}
