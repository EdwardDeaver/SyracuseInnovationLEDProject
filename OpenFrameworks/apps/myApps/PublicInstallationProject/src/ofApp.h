#pragma once

#include "ofMain.h"
#include "ofxOPC.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{
   
private:
    void resetTimeAndSpace(int secondsToWaitFor);
    void DrawMeteors(float x1, float x2);
    int numberOfMinutesTillReset;
    float listenToSerial1(int divisionAmount);
    float listenToSerial2(int divisionAmount);
    void DrawSquares(int x);
    void setupOPCLeds(string IPAddress, int port, int stageWidth, int stageHeight, int numberOfStrips, int numberOfLEDS);

public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    void keyReleased(int key);
    void exit();
    bool BackgroundHold;
    float newx1;
    float newx2;
    float timePerFrame;
    float currentTime;
    ofxOPC opcClient;
    float frameRateForCapture;
    float lastTime;
    void DrawLinearSquares(int input1, int input2, float topSquareHeight);

    deque <ofxNeoPixelStrip> strips;
    
    bool hide;
    int numberOfStrips;
    
  //  ofTrueTypeFont        font;

    bool        bSendSerialMessage;            // a flag for sending serial
    char        bytesRead[3];                // data from serial, we will be trying to read 3
    char        bytesReadString[4];            // a string needs a null terminator, so we need 3 + 1 bytes
    int            nBytesRead;                    // how much did we read?
    int            nTimesRead;                    // how many times did we read?
    float        readTime;                    // when did we last read?
    
    ofSerial    serial1;
    ofSerial    serial2;
    
    
    ofPolyline line;
    
    bool hasItRan;
    
    ofLight pointLight;
    ofLight spotLight;
    ofLight directionalLight;

    ofMaterial material;
    ofImage ofLogoImage;

    float radius;
    ofVec3f center;
    bool bShiny;
    bool bSmoothLighting;
    bool bPointLight, bSpotLight, bDirLight;
    bool bUseTexture;

    long movement;

    ofRectangle rect;

    map<int, ofxNeoPixelObject*> neoPixelObjects;


    ofShader shader;

    ofxFloatSlider radius1;
    ofxColorSlider color;
    //  ofxVec2Slider center;


    ofxPanel gui;

    float topSquareHeight;

};
