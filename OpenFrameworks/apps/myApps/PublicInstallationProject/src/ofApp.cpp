#include "ofApp.h"
// APP DESCRIPTION //
// Allow control of graphics, and LEDs via distance sensors.
// Written by: Edward Deaver, IV November, 2019.
// Assigment: Computer Science Capstone.
// Purpose: To inform about SMART Tech.
// SmartNotMagic.
// APP DESCRIPTION //

//--------------------------------------------------------------

// Description: Setup runs once and sets global variables.
// Pre-conditions: numberOfStrips, movement, lastTime, BackgroundHold, numberOfMinutesTillReset, bSendSerialMessage, nTimesRead, nBytesRead, readTime, hasItRan  are declared as a public variable in ofApp.h
// Post-conditions: Serial1 and Serial 2 are created. A window is created.
void ofApp::setup()
{
    // Stage dimensions.
    int stageWidth = 1800;
    int stageHeight =900;
    // Number of LED Strips.
    numberOfStrips = 4;
    // Number of individual LEDs per light strip.
    int ledsPerStrip = 60;
    
    // Movement is the color variable. Not needed. Should remove. //
    movement = 1;
    // Number of minutes till background should be reset.
    numberOfMinutesTillReset = 60;
    // Not used. /// Remove. ///
    lastTime = ofGetElapsedTimef();
    // This says whether to reset the background.
    BackgroundHold = true;
    // Openframeworks framerate.
    ofSetFrameRate(60);
    float timePerFrame = 1.0 / frameRateForCapture;
    float currentTime = ofGetElapsedTimef();
    
    // Connect to the fcserver
    setupOPCLeds("127.0.0.1",7890,stageWidth, stageHeight,numberOfStrips, ledsPerStrip);
    
    // This is all for connecting and setting the serial input.
    int serial1DevicePort = 2;
    int serial2DevicePort = 4;
    serial1.listDevices();
    vector <ofSerialDeviceInfo> deviceList = serial1.getDeviceList();
    int baud = 115200;
    serial1.setup(serial1DevicePort, baud); //open the first device
    serial2.setup(serial2DevicePort, baud); //open the second device
    bSendSerialMessage = false;
    nTimesRead = 0;
    nBytesRead = 0;
    readTime = 0;
    memset(bytesReadString, 0, 4);
    // this is an intial value to make sure the background is reset.
    hasItRan = true;
    // Sets the window title.
    ofSetWindowTitle("Public Installation");
    //Loads a shader.
    shader.load("shadersGL3/shader");
    topSquareHeight = opcClient.getStageCenterY()-(30*5);
}

//--------------------------------------------------------------
// Description: Ingests serial data, runs graphics effects, and sends data to OPC Client.
// Pre-conditions: opcClient is connected successfully.
// Post-conditons: Data is successfully sent to FadeCandy Server on different channels.
void ofApp::update()
{
    int dataDivisionAmount = 10;
    
    opcClient.update();
    
    //This will reset the stage background or will allow a background images to stay.
    opcClient.beginStage(hasItRan);
    if(hasItRan){
        hasItRan = BackgroundHold;
    }
    //Get Serial Data
    // Set the serial data of serial object 1 to newx1.
    if(serial1.available()>0){
        newx1 =  listenToSerial1(dataDivisionAmount);
    }
    // Set the serial data of serial object 2 to newx2.
    if(serial2.available()>0){
        newx2 = listenToSerial2(dataDivisionAmount);
    }

    // If it's been a set amount of time the program will reset the stage.
    resetTimeAndSpace(60*numberOfMinutesTillReset);
    
    // DETERMINE WHAT EFFECT TO USE:
    // Effect 1: Between 9AM - 5PM and it's an even hour run linear squares.
  //  if((ofGetHours()<17 && ofGetHours()>9) && ofGetHours()%2==1){
        BackgroundHold = false;
        int oldx = ofRandom(1000);
        int newx = ofRandom(2000);
        DrawLinearSquares(1501,newx, topSquareHeight);
   // }
/*    // Effect 2: Between 9AM - 5PM and it's an odd hour run Draw Meteors.
    if((ofGetHours()<17 && ofGetHours()>9) && ofGetHours()%2==1){
        DrawMeteors(newx1, newx2);
    }
    // Effect 3: Between 5PM - 8AM  draw sqaures. Not interactive.
//    if(ofGetHours()>=17 || ofGetHours()<=9){
//        DrawSquares(1);
    }
*/
    // End stage
    opcClient.endStage();
    // Obtain a block of pixels and their data from the led strips on screen.
    for (int i = 0; i < strips.size(); i++) {
        opcClient.getStagePixels(strips[i].getPixelCoordinates(), strips[i].colors);
    }
    // If the client is not connected do not try and send information
    if (!opcClient.isConnected()) {
        // Will continue to try and reconnect to the Pixel Server
        opcClient.tryConnecting();
    }
    else {
        // Write out the data
        for (int i = 0; i < strips.size(); i++)
        {
            opcClient.writeChannel(i+1, strips[i].colorData());
        }
    }
    movement++;
}
//--------------------------------------------------------------
// Description: Draws things to the stage(normally). In this case will draw sensor information and led strips.
// Pre-conditionn: opcClient is successfully connected, a window has been created, and strips have been generated.
// Post-conditions: Sensor data is drawn to the window sucessfully.
void ofApp::draw()
{
   // gui.draw();
    opcClient.drawStage(hide);
    // Show the grabber area and Draw the output
    for (int i = 0; i < strips.size(); i++) {
        strips[i].drawGrabRegion(true);
        strips[i].draw((opcClient.getStageWidth()+25)+(i*5), 5);
    }
    // Report Messages
    stringstream ss;
    ss << "FPS: " << ofToString(ofGetFrameRate(),2) << endl;
    ss << "SENSOR 1:" << ofToString(newx1) << endl;
    ss << "SENSOR 2:" << ofToString(newx2) << endl;
    ofDrawBitmapStringHighlight(ss.str(), 5,ofGetHeight()-90);
}
//--------------------------------------------------------------
// Description: This is an event listener in OpenFrameworks that listens for a key press. This is not used in this application but without it the program fails to compile.
void ofApp::keyPressed(int key)
{
   
}
//--------------------------------------------------------------
// Description: This is an event listener in OpenFrameworks that listens for a key press. This is not used in this application but without it the program fails to compile.
void ofApp::keyReleased(int key)
{
    
}
//--------------------------------------------------------------
// Description: Exits the program and closes connection to fcserver.
// Pre-conditions: The program is running. opcClient is connected to the fcserver.
// Post-conditions:  Application stops running.
void ofApp::exit()
{
    // Close Connection
    opcClient.close();
}

// Description:  Reset on screen data by wiping the screen with a rectangle, resets the line object, and resets the timer.
// Pre-conditions: secondsToWaitFor is an int. ofGetElapsedTimef started running at the beginning of the program.
// Post-condition: Elapsed time counter is reset.
void ofApp::resetTimeAndSpace(int secondsToWaitFor)
{
    currentTime = ofGetElapsedTimef();
    if(currentTime >=secondsToWaitFor){
        ofClear(0);
        ofBackground(0);
        ofFill();
        ofSetColor(0, 0, 0);
        ofDrawRectangle(0, 0,opcClient.getStageWidth(), opcClient.getStageHeight());
        line.clear();
        ofResetElapsedTimeCounter();
    }
}
// Description: Draws more squares on screen as the user gets closer.
// Pre-conditions: input1, input2 is an int, and topSquareHeight is a float.
// Post-conditions: None.
void ofApp::DrawLinearSquares(int input1, int input2, float topSquareHeight){
    BackgroundHold = true;
    ofSetColor(255,222,50);
    if(input1<2500){
        for(int i = 0; i<numberOfStrips; i++){
            ofPushMatrix();
            ofDrawRectangle(((opcClient.getStageWidth()/numberOfStrips)/2)+(i*(opcClient.getStageWidth()/numberOfStrips))-50,  topSquareHeight,100, topSquareHeight/4);
            ofPopMatrix();
        }
    }
   if(input1<2000){
         for(int i = 0; i<numberOfStrips; i++){
             ofPushMatrix();
             ofSetColor(255,0,50);
             ofDrawRectangle(((opcClient.getStageWidth()/numberOfStrips)/2)+(i*(opcClient.getStageWidth()/numberOfStrips))-50,  topSquareHeight+topSquareHeight/4,100, topSquareHeight/4);
             ofPopMatrix();
         }
     }
    if(input1<1500){
          for(int i = 0; i<numberOfStrips; i++){
              ofPushMatrix();
              ofSetColor(50,0,50);
              ofDrawRectangle(((opcClient.getStageWidth()/numberOfStrips)/2)+(i*(opcClient.getStageWidth()/numberOfStrips))-50,  topSquareHeight+topSquareHeight/2,100, topSquareHeight/4);
              ofPopMatrix();
          }
      }
    if(input1<1000){
          for(int i = 0; i<numberOfStrips; i++){
              ofPushMatrix();
              ofDrawRectangle(((opcClient.getStageWidth()/numberOfStrips)/2)+(i*(opcClient.getStageWidth()/numberOfStrips))-50,  topSquareHeight+topSquareHeight/1.35,100, topSquareHeight/3.7);
              ofPopMatrix();
          }
      }


}
// Description: This uses time to control the coloring of rectangles.
// Pre-conditions: x is an int.
// Post-conditions: none.
void ofApp::DrawSquares(int x)
{
    BackgroundHold = true;
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofFill();
    // ofDrawRectangle(0, 0,newx1, 50);
    // ofSeedRandom(0);
   for(int i = (opcClient.getStageWidth()/numberOfStrips/3.5); i < opcClient.getStageWidth(); i=i+(opcClient.getStageWidth()/numberOfStrips)){
     //   ofSetColor((int)ofRandom(0,255), x,x*2, 255); // contour (stroke) color
      //ofTranslate(50+i, 50, 0);

      // int movement = 1;

      //  ofDrawRectangle(100+i, 100, 100, 100);
        for(int z = opcClient.getStageCenterY()-(30*9); z < opcClient.getStageHeight(); z=z+5){
 //           ofPushMatrix();
    ///
   //         ofPushStyle();
              ofSetColor(z*ofGetElapsedTimeMillis()%movement*i, z,z, z%255); // contour (stroke) color
            //ofTranslate(50+i, 50, 0);


              ofDrawRectangle(100+i, 100+z, 100, 100);
            ofPopMatrix();
            ofPopStyle();
            movement= movement+20;


        }

 //   ofRotateYDeg(i);
   }
}

// Description: Draws squares the persist on screen that are set at the user input of x1/x2.
// Pre-conditions: x1/x2 are floats, line variable has been created.
// Post-conditions: None.
void ofApp::DrawMeteors(float x1, float x2)
{
    
    BackgroundHold = false;
    ofFill();
    // ofDrawRectangle(0, 0,newx1, 50);
    //    x1 = ofRandom(2000);
    //  x2 = ofRandom(2000);
    ofPoint pt;
    pt.set(x1, x2);
    line.addVertex(pt);
    int newRed = ofMap(x1, 0, 1000, 0, 255); // newx = 21.5 a value [21, 22).
    int newBlue = ofMap(x2, 0, 1000, 0, 255); // newx = 21.5 a value [21, 22).
       // ofSetColor(newRed,x2/10,newBlue);
     //   ofDrawRectangle(x1, x2,100, 100);
    ofPushMatrix();
    ofNoFill();
    ofSetColor(255, 255,255, 255); // contour (stroke) color
    ofDrawRectangle(x1, x2,100, 100);
    for(int i = 0; i < 200; i ++){
        ofSetColor(newRed-i,x2/ofGetMinutes()*ofGetHours()+i,ofGetMinutes()*ofGetHours());
        
        ofPushMatrix();

        ofDrawRectangle(x1+i, x2-i,100, 100);
       // ofDrawTriangle(x1+i, x2-i*2,x1+i, x2-i,90,40);
        ofDrawLine(x1, x2,100,100);
        ofDrawCircle(x1,x2,100+i);

        ofNoFill();
        ofSetColor(255, 255,255, 255); // contour (stroke) color
        ofPushMatrix();

        ofDrawRectangle(x1, x2,100, 100);
        ofPopMatrix();
        ofPopMatrix();
        ofPopStyle();
        ofPopStyle();
    }
    ofFill();
    line.draw();
    ofPopMatrix();
    ofPopStyle();
    ofPopStyle();
}
// Description: Listens for input on serial input 1.
// Pre-conditions: serial1 has been initialized, newx1 has been declared a global variable, and divisionAmount is an int.
// Post-conditions: newx1 is returned as a float.
float ofApp::listenToSerial1(int divisionAmount)
{

    nTimesRead = 0;
    nBytesRead = 0;
    int nRead  = 0;  // a temp variable to keep count per read

    unsigned char bytesReturned[6];

    memset(bytesReadString, 0, 6);
    memset(bytesReturned, 0, 6);

    while( (nRead = serial1.readBytes( bytesReturned, 6)) > 0){
      nTimesRead++;
      nBytesRead = nRead;
    };

    memcpy(bytesReadString, bytesReturned, 6);

    bSendSerialMessage = false;
    if(atoi(bytesReadString)<15001){
      newx1 = atoi(bytesReadString)/divisionAmount;
    }
    memset(bytesReadString, 0, 4);
    return newx1;
}
// Description: Listens for input on serial input 2.
// Pre-conditions: serial2 has been initialized, newx2 has been declared a global variable, and divisionAmount is an int.
// Post-conditions: newx2 is returned as a float.
float ofApp::listenToSerial2(int divisionAmount)
{

    nTimesRead = 0;
    nBytesRead = 0;
    int nRead  = 0;  // a temp variable to keep count per read

    unsigned char bytesReturned[6];

    memset(bytesReadString, 0, 6);
    memset(bytesReturned, 0, 6);

    while( (nRead = serial2.readBytes( bytesReturned, 6)) > 0){
     nTimesRead++;
     nBytesRead = nRead;
    };

    memcpy(bytesReadString, bytesReturned, 6);

    bSendSerialMessage = false;
    if(atoi(bytesReadString)<15001){
     newx2 = atoi(bytesReadString)/divisionAmount;
    }
    memset(bytesReadString, 0, 4);
    return newx2;
}
// Description: Creates Neopxiel objects for each led strip.
// Pre-conditions: IPAddress is a string. port, stageWidth, stageHeight, numberOfStrips, and numberOfLEDS are ints. OPCClient is connected.
// Post-conditions: The LEDS strips are created.
void ofApp::setupOPCLeds(string IPAddress, int port, int stageWidth, int stageHeight, int numberOfStrips, int numberOfLEDS){
    opcClient.setup(IPAddress, port);
    
    // You define the stage size and draw into the stage
    opcClient.setupStage(stageWidth, stageHeight);

    for (int x = 0; x < numberOfStrips; x++) {
        strips.push_back(ofxNeoPixelStrip(((opcClient.getStageWidth()/numberOfStrips)/2)+(x*(opcClient.getStageWidth()/numberOfStrips)), opcClient.getStageCenterY()-(30*5), numberOfLEDS, 5));
    }
    
}
/*
          // ofDrawRectangle(0, 0,newx1, 50);
     // ofSeedRandom(0);
     
      for(int i = (opcClient.getStageWidth()/numberOfStrips/3.5); i < opcClient.getStageWidth(); i=i+(opcClient.getStageWidth()/numberOfStrips)){
        //   ofSetColor((int)ofRandom(0,255), x,x*2, 255); // contour (stroke) color
         //ofTranslate(50+i, 50, 0);

         // int movement = 1;

         //  ofDrawRectangle(100+i, 100, 100, 100);
          for(int z = opcClient.getStageCenterY()-(30*9); z < i*100; z=z+100){
    //           ofPushMatrix();
       ///
      //         ofPushStyle();
                 ofSetColor(z*ofGetElapsedTimeMillis()%movement*i, z,z+movement, 255); // contour (stroke) color
               //ofTranslate(50+i, 50, 0);


                 ofDrawRectangle(100+i, 100+z, 100, 100);
               ofPopMatrix();
               ofPopStyle();
               movement= movement+20;


           }

    
}
}

*/
////////////////////////////////////////////////////////////////////////////////

/*
       BackgroundHold = false;
       ofEnableBlendMode(OF_BLENDMODE_ADD);
          ofFill();
          // ofDrawRectangle(0, 0,newx1, 50);
     // ofSeedRandom(0);
       
      for(int i = (opcClient.getStageWidth()/numberOfStrips/3.5); i < opcClient.getStageWidth(); i=i+(opcClient.getStageWidth()/numberOfStrips)){
        //   ofSetColor((int)ofRandom(0,255), x,x*2, 255); // contour (stroke) color
         //ofTranslate(50+i, 50, 0);

         // int movement = 1;

         //  ofDrawRectangle(100+i, 100, 100, 100);
          for(int z = opcClient.getStageCenterY()-(30*9); z < x*100; z=z+100){
    //           ofPushMatrix();
       ///
      //         ofPushStyle();
                 ofSetColor(z*ofGetElapsedTimeMillis()%movement*i, z,z+movement, 255); // contour (stroke) color
               //ofTranslate(50+i, 50, 0);


                 ofDrawRectangle(100+i, 100+z, 100, 100);
               ofPopMatrix();
               ofPopStyle();
               movement= movement+20;


           }

    //   ofRotateYDeg(i);
   */
