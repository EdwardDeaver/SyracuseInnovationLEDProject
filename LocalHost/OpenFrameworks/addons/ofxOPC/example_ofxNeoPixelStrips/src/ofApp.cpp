#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    ofSetFrameRate(60);
    effect = 0;
    
    // Connect to the fcserver
    opcClient.setup("127.0.0.1", 7890);
    
    // You define the stage size and draw into the stage
    opcClient.setupStage(500, 500);
    
    defaultEffects.setup(opcClient.getStageCenterX(), opcClient.getStageCenterY(), opcClient.getStageWidth(), opcClient.getStageHeight());
        
    stick.setupLedStrip(50, 50, 8, 7);
    strip40px.setupLedStrip(100,50, 40, 7);
    strip60px.setupLedStrip(150,50, 60, 7);
    strip144px.setupLedStrip(200,50, 144, 3);
}
//--------------------------------------------------------------
void ofApp::update()
{
    ofSetWindowTitle("example_ofxNeoPixelStrips");
    
    opcClient.update();
    
    // Now Draw the effects to the stage
    opcClient.beginStage();
    
    // Draw what you want rendered here
    
    // For now here are some default effects
    defaultEffects.draw(effect);
    
    opcClient.endStage();
    
    // New Get Method
    opcClient.getStagePixels(stick.getPixelCoordinates(), stick.colors);
    opcClient.getStagePixels(strip40px.getPixelCoordinates(), strip40px.colors);
    opcClient.getStagePixels(strip60px.getPixelCoordinates(), strip60px.colors);
    opcClient.getStagePixels(strip144px.getPixelCoordinates(), strip144px.colors);
    
    // If the client is not connected do not try and send information
    if (!opcClient.isConnected()) {
        // Will continue to try and reconnect to the Pixel Server
        opcClient.tryConnecting();
    }
    else {
        // Write out the first set of data
        opcClient.writeChannelOne(stick.colorData());
        opcClient.writeChannelTwo(strip40px.colorData());
        opcClient.writeChannelThree(strip60px.colorData());
    }
}
//--------------------------------------------------------------
void ofApp::draw()
{
    ofBackground(0);
    opcClient.drawStage(hide);
    
    // Show the grabber area
    stick.drawGrabRegion(hide);
    strip40px.drawGrabRegion(hide);
    strip60px.drawGrabRegion(hide);
    strip144px.drawGrabRegion(hide);
    
    // Draw the output
    stick.draw(opcClient.getStageWidth()+25, 10);
    strip40px.draw(opcClient.getStageWidth()+50, 10);
    strip60px.draw(opcClient.getStageWidth()+75, 10);
    strip144px.draw(opcClient.getStageWidth()+100, 10);
    
    // Report Messages
    stringstream ss;
    ss << "Press Left and Right to Change Effect Mode" << endl;
    ss << "FPS: " << ofToString((int)(ofGetFrameRate())) << endl;
    ofDrawBitmapStringHighlight(ss.str(), 5,ofGetHeight()-30);
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    if (key == OF_KEY_LEFT) {
        effect--;
    }
    if (key == OF_KEY_RIGHT) {
        effect++;
    }
    if (key == ' ') {
        hide = !hide;
    }
}
//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
    
}
//--------------------------------------------------------------
void ofApp::exit()
{
    // Close Connection
    opcClient.close();
}
