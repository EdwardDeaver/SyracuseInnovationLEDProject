#pragma once

#include "ofMain.h"
#include "ofxOPC.h"
#include "ofxGui.h"
#include "ofxNetwork.h"
class ofApp : public ofBaseApp{
   
private:
    void resetTimeAndSpace(int secondsToWaitFor);
    int numberOfMinutesTillReset;
    float listenToSerial1(int divisionAmount);
    float listenToSerial2(int divisionAmount);
    void DrawSquares(int x);
    void setupOPCLeds(string IPAddress, int port, int stageWidth, int stageHeight, int numberOfStrips, int numberOfLEDS);
    void DrawLinearSquares(int input1, int input2, float topSquareHeight, bool left);
    bool audioToneSetup();
    ofPolyline line;
    
    float topSquareHeight;
    float widthStageConstantVariable;
    float widthStageConstantVariableDivide2;
    
    


public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    void keyReleased(int key);
    void exit();
    void audioOut(ofSoundBuffer &outBuffer);
    void  audioPlayBackAtPoint(float input, bool left);
    int sendToUDP(float sensor1, float sensor2, int timeSent);

    bool BackgroundHold;
    float newx1;
    float newx2;
    float timePerFrame;
    float currentTime;
    ofxOPC opcClient;
    float frameRateForCapture;
    float lastTime;

    deque <ofxNeoPixelStrip> strips;
    bool hide;
    int numberOfStrips;
    
    ofTrueTypeFont        myfont;

    bool        bSendSerialMessage;            // a flag for sending serial
    char        bytesRead[3];                // data from serial, we will be trying to read 3
    char        bytesReadString[4];            // a string needs a null terminator, so we need 3 + 1 bytes
    int            nBytesRead;                    // how much did we read?
    int            nTimesRead;                    // how many times did we read?
    float        readTime;                    // when did we last read?
    
    ofSerial    serial1;
    ofSerial    serial2;
    
    
    
    bool hasItRan;



    long movement;

    ofRectangle rect;

    map<int, ofxNeoPixelObject*> neoPixelObjects;
    
    //
    ofSoundPlayer   L82HZTone;
    ofSoundPlayer   R82HZTone;
    
    ofSoundPlayer   L130HZTone;
    ofSoundPlayer   R130HZTone;

    ofSoundPlayer   L174HZTone;
    ofSoundPlayer   R174HZTone;

    ofSoundPlayer   L261HZTone;
    ofSoundPlayer   R261HZTone;

    
    // Network

    ofxUDPManager udpConnection;
    int sentMAX;
    int sentAmount;
};

