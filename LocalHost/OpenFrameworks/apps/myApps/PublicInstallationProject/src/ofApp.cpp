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
  
    // setup networking
    // ------------------------
    ofxUDPSettings settings;
    settings.sendTo("127.0.0.1", 11999);
    settings.blocking = false;
    udpConnection.Setup(settings);

    
    //Setup Audio files.
    bool audioSet = audioToneSetup();
    
    if(audioSet){
        
        ofBackground(0);
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
        int serial1DevicePort = 3;
        int serial2DevicePort = 6;
        serial1.listDevices();
        vector <ofSerialDeviceInfo> deviceList = serial1.getDeviceList();
        int baud = 115200;
        serial1.setup("ttyUSB0", baud); //open the first device
        serial2.setup("ttyUSB1", baud); //open the second device
        bSendSerialMessage = false;
        nTimesRead = 0;
        nBytesRead = 0;
        readTime = 0;
        memset(bytesReadString, 0, 4);
        // this is an intial value to make sure the background is reset.
        hasItRan = true;
        // Sets the window title.
        ofSetWindowTitle("Public Installation");
        topSquareHeight = opcClient.getStageCenterY()-(30*5);
        widthStageConstantVariable =(opcClient.getStageWidth()/numberOfStrips);
        widthStageConstantVariableDivide2 = widthStageConstantVariable/2;
        // start the sound stream with a sample rate of 44100 Hz, and a buffer
        // size of 512 samples per audioOut() call
      
        myfont.load("OpenSans-Bold.ttf", 20);
        //ONLY ENABLE IF TESTING

        sentMAX = 1;
        sentAmount = 0;
    }
}
// --------------------------------------------------------------
// Description: Loads and sets all ofSound objects for program.
// Precondition: The sound objects have been created.
// Post-condition: All data values set.
//                 Returns true.
bool ofApp::audioToneSetup(){
    //82 HZ
    //LEFT
    L82HZTone.load("tone83.mp3");
    L82HZTone.play();
    L82HZTone.setVolume(0.0);
    L82HZTone.setPan(-1.0);
    L82HZTone.setLoop(true);
    ////RIGHT////
    R82HZTone.load("tone83.mp3");
    R82HZTone.play();
    R82HZTone.setVolume(0.0);
    R82HZTone.setPan(1.0);
    R82HZTone.setLoop(true);

    //130HZ
    ///LEFT
    L130HZTone.load("tone130.mp3");
    L130HZTone.play();
    L130HZTone.setVolume(0.0);
    L130HZTone.setPan(-1.0);
    L130HZTone.setLoop(true);

    ////RIGHT////
    R130HZTone.load("tone130.mp3");
    R130HZTone.play();
    R130HZTone.setVolume(0.0);
    R130HZTone.setPan(1.0);
    R130HZTone.setLoop(true);

    //174HZ
    ///LEFT
    L174HZTone.load("tone174.mp3");
    L174HZTone.play();
    L174HZTone.setVolume(0.0);
    L174HZTone.setPan(-1.0);
    L174HZTone.setLoop(true);

    ////RIGHT////
    R174HZTone.load("tone174.mp3");
    R174HZTone.play();
    R174HZTone.setVolume(0.0);
    R174HZTone.setPan(1.0);
    R174HZTone.setLoop(true);

    //
    //261HZ
    ///LEFT
    L261HZTone.load("tone261.mp3");
    L261HZTone.play();
    L261HZTone.setVolume(0.0);
    L261HZTone.setPan(-1.0);
    L261HZTone.setLoop(true);

    ////RIGHT////
    R261HZTone.load("tone261.mp3");
    R261HZTone.play();
    R261HZTone.setVolume(0.0);
    R261HZTone.setPan(1.0);
    R261HZTone.setLoop(true);

    //

    return true;
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

//--------------------------------------------------------------
// Description: Ingests serial data, runs graphics effects, and sends data to OPC Client.
// Pre-conditions: opcClient is connected successfully.
// Post-conditons: Data is successfully sent to FadeCandy Server on different channels.
void ofApp::update()
{
    float dataDivisionAmount = 1;
    
    opcClient.update();
    
    //This will reset the stage background or will allow a background images to stay.
    opcClient.beginStage(hasItRan);
    if(hasItRan){
        hasItRan = BackgroundHold;
    }
    //Get Serial Data
    // Set the serial data of serial object 1 to newx1.
    if(serial1.available()>0){
        int temp = listenToSerial1(dataDivisionAmount);
       // ofLogNotice() << "temp: " << ofToString(temp);
        if(temp>100){

            newx1 = temp;

         //   ofLogNotice() << "nex1: " << ofToString(newx1);
        }
    }
    // Set the serial data of serial object 2 to newx2.
    if(serial2.available()>0){
        int temp = listenToSerial2(dataDivisionAmount);
        //ofLogNotice() << "temp: " << ofToString(temp);
        if(temp>100){

            newx2 = temp;

          //  ofLogNotice() << "nex2: " << ofToString(newx2);
        }    }


    


    
    // If it's been a set amount of time the program will reset the stage.
    resetTimeAndSpace(60*numberOfMinutesTillReset);

    // DETERMINE WHAT EFFECT TO USE:
    // Effect 1: Between 9AM - 5PM and it's an even hour run linear squares.
    //   if((ofGetHours()<17 && ofGetHours()>8) && ofGetHours()%2==1){
    BackgroundHold = false;
    // Left
    audioPlayBackAtPoint(newx1, true);
    // Right
    audioPlayBackAtPoint(newx2, false);
    // Lighting effect
    // Left
    DrawLinearSquares(newx1,0, topSquareHeight, true);
    // Right
    DrawLinearSquares(newx2,0, topSquareHeight, false);

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
    
  
    ofSoundUpdate();
    //ofLogNotice() << "amount: " << ofToString(sentAmount);
    //ofLogNotice() << "max: " << ofToString(sentMAX);
    
    // If the framerate is 30 FPS then 30 frames will pass per second. So in 2 seconds there will 60 frames.
    // So if I want my app to update the server ever 2 seconds then I want to incremement an amount every frame then check if it is 2 x CurrentFramerate
    // Then reset the amount
    if(sentAmount < sentMAX){
        sendToUDP(newx1, newx2, ofGetUnixTime());
         //ofLogNotice() << "value: " << ofToString(sentAmount);
         sentAmount++;
    }
    else{
        sentAmount++;
    }
    
    if(sentAmount>=ofGetFrameRate()*10){
        sentAmount = 0;
       //newx1 = ofRandom(7000);
       // newx2 = ofRandom(7000);

    }
}
//--------------------------------------------------------------
// Description: Draws things to the stage(normally). In this case will draw sensor information and led strips.
// Pre-conditionn: opcClient is successfully connected, a window has been created, and strips have been generated.
// Post-conditions: Sensor data is drawn to the window sucessfully.
void ofApp::draw()
{
    opcClient.drawStage(hide);
    // Show the grabber area and Draw the output
    for (int i = 0; i < strips.size(); i++) {
        strips[i].drawGrabRegion(true);
        strips[i].draw((opcClient.getStageWidth()+25)+(i*5), 5);
    }
    // Report Messages
    stringstream ss;
    stringstream Sensor1;
    stringstream Sensor2;

    ss << "FPS: " << ofToString(ofGetFrameRate(),2) << endl;
    Sensor1 << "SENSOR 1: " << ofToString(newx1) << endl;
    Sensor2 << "SENSOR 2: " << ofToString(newx2) << endl;
    
   
    ofSetColor(255,255,255);

    myfont.drawString(Sensor1.str(),400 ,680);
    ofDrawRectangle(widthStageConstantVariable-220,ofGetHeight()-450, 450,2);

    myfont.drawString(Sensor2.str(),400*3 + 90 ,680);
    ofDrawRectangle((widthStageConstantVariable)*2.52,ofGetHeight()-450, 450,2);
    
    //ofDrawBitmapStringHighlight(ss.str(), 5,ofGetHeight()-90);
   // ofDrawBitmapStringHighlight(Sensor1.str(), widthStageConstantVariable -30,ofGetHeight()-430);
//    ofDrawRectangle(widthStageConstantVariable*3-220,ofGetHeight()-450, 450,50);
  //  ofDrawBitmapStringHighlight(Sensor2.str(), widthStageConstantVariable*3 -30,ofGetHeight()-430);

}
//--------------------------------------------------------------
// Description: This is an event listener in OpenFrameworks that listens for a key press. This is not used in this application but without it the program fails to compile.
void ofApp::keyPressed(int key)
{
   
}

// Description: This is an event listener in OpenFrameworks that listens for changes in audio.
void ofApp::audioOut(ofSoundBuffer &outBuffer){

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
void ofApp::DrawLinearSquares(int input1, int input2, float topSquareHeight, bool left){
    int starter = 2;
    int stripAmount =numberOfStrips;
    if(left){
         starter = 0;
         stripAmount =numberOfStrips/2;
    }
    else{
        int starter = 2;
        int stripAmount =numberOfStrips;
    }
    BackgroundHold = true;
    if(input1>0){
        if(input1<1400){
            for(int i = starter; i<stripAmount; i++){
                for(int z = 0; z<70; z++){
                    ofPushMatrix();
                    ofSetColor((z*5),255-(z*5),178);
                    ofDrawRectangle(widthStageConstantVariableDivide2+(i*widthStageConstantVariable)-50,  topSquareHeight+z%70,100, (topSquareHeight/4)/20);
                    ofPopMatrix();
                }

               // ofPushMatrix();
               // ofSetColor(255,255,178);
               // ofDrawRectangle(widthStageConstantVariableDivide2+(i*widthStageConstantVariable)-50,  topSquareHeight,100, topSquareHeight/4);
               // ofPopMatrix();
            }
        }
       if(input1<1000){

            
           for(int i = starter; i<stripAmount; i++){
               for(int z = 0; z<70; z++){
                   ofPushMatrix();
                   ofSetColor((z*5),255-(z*5),178);
                    ofDrawRectangle(widthStageConstantVariableDivide2+(i*widthStageConstantVariable)-50,  ( topSquareHeight+topSquareHeight/4 )+z,100, (topSquareHeight/4)/20);
                    ofPopMatrix();
               }
               //  ofPushMatrix();
               //  ofSetColor(254,204,92);
               //  ofDrawRectangle(widthStageConstantVariableDivide2+(i*widthStageConstantVariable)-50,  topSquareHeight+topSquareHeight/4,100, topSquareHeight/4);
                // ofPopMatrix();
             }
         }
        if(input1<501){
//            frequency = 174.614;

              for(int i = starter; i<stripAmount; i++){
                  for(int z = 0; z<70; z++){
                       ofPushMatrix();
                       ofSetColor(255%(z+1),255-(z*2),3*z);
                       ofDrawRectangle(widthStageConstantVariableDivide2+(i*widthStageConstantVariable)-50,  ( topSquareHeight+topSquareHeight/2 )+z,100, (topSquareHeight/4)/20);
                       ofPopMatrix();
                  }
                  
                //  ofPushMatrix();
                //  ofSetColor(253,141,60);
                //  ofDrawRectangle(widthStageConstantVariableDivide2+(i*widthStageConstantVariable)-50,  topSquareHeight+topSquareHeight/2,100, topSquareHeight/4);
                //  ofPopMatrix();
              }
          }
        if(input1<300 && input1>0){
          
           // frequency = 261.626;
              for(int i = starter; i<stripAmount; i++){
                  for(int z = 0; z<80; z++){
                       ofPushMatrix();
                       ofSetColor(255,255%((z+1)*2),3*z);
                       ofDrawRectangle(widthStageConstantVariableDivide2+(i*widthStageConstantVariable)-50,  ( topSquareHeight+topSquareHeight/1.35 )+z,100, (topSquareHeight/3.7)/20);
                       ofPopMatrix();
                  }
               //   ofPushMatrix();
               //   ofSetColor(227,26,28);
               //   ofDrawRectangle(widthStageConstantVariableDivide2+(i*widthStageConstantVariable)-50,  topSquareHeight+topSquareHeight/1.35,100, topSquareHeight/3.7);
               //   ofPopMatrix();
              }
          }
    }
    else{
    }



}

// Description: Changes volume for audio being played given the input and range.
//              If left is true plays audio for left side. If false, plays for right side
// Precondition: The sound objects have been created and set.
// Post-condition: A given volume is set.
bool ofApp::audioPlayBackAtPoint(float input, bool left){
    if(left){
	
        if(input<=1400 && input>=1000){
            // PLAY 82 HZ
            L82HZTone.setVolume(1);
            L130HZTone.setVolume(0.0);
            L174HZTone.setVolume(0.0);
            L261HZTone.setVolume(0.0);
            return false;

        }
        if(input<1000 && input>=501){
            //PLAY 130 HZ
            L82HZTone.setVolume(0.0);
            L130HZTone.setVolume(1);
            L174HZTone.setVolume(0.0);
            L261HZTone.setVolume(0.0);
            return false;


        }
        if(input<500 && input>=300){
            //PLAY 174 HZ
            L82HZTone.setVolume(0.0);
            L130HZTone.setVolume(0.0);
            L174HZTone.setVolume(1);
            L261HZTone.setVolume(0.0);
            return false;

        }
        if(input<300 && input>=1){
            //PLAY 130HZ
            L82HZTone.setVolume(0.0);
            L130HZTone.setVolume(0.0);
            L174HZTone.setVolume(0.0);
            L261HZTone.setVolume(1);
            return false;

        }
        if(input<=0 || input>=1401){
            L82HZTone.setVolume(0.0);
            L130HZTone.setVolume(0.0);
            L174HZTone.setVolume(0.0);
            L261HZTone.setVolume(0.0);
            return false;


        }
    }
    /// ----------------------------------------------------------------------------------------
    // RIGHT
    if(!left){

        if(input<=0 || input>=1401){
             R130HZTone.setVolume(0.0);
             R82HZTone.setVolume(0.0);
             R174HZTone.setVolume(0.0);
             R261HZTone.setVolume(0.0);
            return false;
         }
        if(input<=1400 && input>=1000){
            // PLAY 82 HZ
            R130HZTone.setVolume(0.0);
            R82HZTone.setVolume(1);
            R174HZTone.setVolume(0.0);
            R261HZTone.setVolume(0.0);
            return false;
        }
        if(input<1000 && input>=501){
            // PLAY 130 HZ
            R82HZTone.setVolume(0.0);
            R130HZTone.setVolume(1);
            R174HZTone.setVolume(0.0);
            R261HZTone.setVolume(0.0);
            return false;

        }
        if(input<500 && input>=300){
            // PLAY 174.641 HZ
            R82HZTone.setVolume(0.0);
            R130HZTone.setVolume(0.0);
            R174HZTone.setVolume(1);
            R261HZTone.setVolume(0.0);
            return false;


        }
        if(input<300 && input>=1){
            // PLAY 174 HZ
            R82HZTone.setVolume(0.0);
            R130HZTone.setVolume(0.0);
            R174HZTone.setVolume(0.0);
            R261HZTone.setVolume(1);
            return false;


        }
    }
    return true;
    
}

// Based on Forum posts to convert data https://forum.openframeworks.cc/t/serial-binary-data/7039/6
// https://forum.openframeworks.cc/t/reading-serial-data-from-arduino/8161/4
// https://openframeworks.cc/documentation/communication/ofSerial/#show_readBytes
// https://openframeworks.cc/documentation/communication/ofSerial/#show_available
// Description: Listens for input on serial input 1.
// Pre-conditions: serial1 has been initialized, newx1 has been declared a global variable, and divisionAmount is an int.
// Post-conditions: newx1 is returned as a float.
float ofApp::listenToSerial1(float divisionAmount)
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
float ofApp::listenToSerial2(float divisionAmount)
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

// Description: Sends data to UDP server
// Pre-conditions: Sensor1 and sensor 2 are floats. And timeSet is an int.
// Post-conditions: returns 100.
int ofApp::sendToUDP(float sensor1, float sensor2, int timeSent){
    
    string json = "{\"sensor1\":";
    json += std::to_string(sensor1);
    json += ",\"sensor2\":";
    json += std::to_string(sensor2);
    json += ",\"timeSent\":";
    json += std::to_string(timeSent);
    json += "}";
    udpConnection.Send(json.c_str(),json.length());
    return 100;
}
