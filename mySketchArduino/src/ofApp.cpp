#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    ofSetBackgroundAuto(false);

    bSendSerialMessage = false;
    ofBackground(0,0,0);
//    ofSetLogLevel(OF_LOG_VERBOSE);
    serial1.listDevices();

    font.load("OpenSans-Bold.ttf", 64);
    vector <ofSerialDeviceInfo> deviceList = serial1.getDeviceList();
    // this should be set to whatever com port your serial1 device is connected to.
    // (ie, COM4 on a pc, /dev/tty.... on linux, /dev/tty... on a mac)
    // arduino users check in arduino app....
    int baud = 115200;
    serial1.setup(2, baud); //open the first device
    
    // this should be set to whatever com port your serial1 device is connected to.
    // (ie, COM4 on a pc, /dev/tty.... on linux, /dev/tty... on a mac)
    // arduino users check in arduino app....
    serial2.setup(4, baud); //open the first device
    nTimesRead = 0;
    nBytesRead = 0;
    readTime = 0;

    memset(bytesReadString, 0, 4);
}

//--------------------------------------------------------------
void ofApp::update(){

    if(serial1.available()>0){
        // (1) write the letter "a" to serial1:
        //serial1.writeByte('a');
        
        // (2) read
        // now we try to read 3 bytes
        // since we might not get them all the time 3 - but sometimes 0, 6, or something else,
        // we will try to read three bytes, as much as we can
        // otherwise, we may have a "lag" if we don't read fast enough
        // or just read three every time. now, we will be sure to
        // read as much as we can in groups of three...
        
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
        readTime = ofGetElapsedTimef();
        if(atoi(bytesReadString)<15001){
            newx1 = atoi(bytesReadString)/10;
        }
        memset(bytesReadString, 0, 4);

    
}
    if(serial2.available()>0){
            // (1) write the letter "a" to serial1:
            //serial1.writeByte('a');
            
            // (2) read
            // now we try to read 3 bytes
            // since we might not get them all the time 3 - but sometimes 0, 6, or something else,
            // we will try to read three bytes, as much as we can
            // otherwise, we may have a "lag" if we don't read fast enough
            // or just read three every time. now, we will be sure to
            // read as much as we can in groups of three...
            
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
            readTime = ofGetElapsedTimef();
            if(atoi(bytesReadString)<15001){
                newx2 = atoi(bytesReadString)/10;
            }
        memset(bytesReadString, 0, 4);

        
    }
    ofPoint pt;
    pt.set(newx1, newx2);
    line.addVertex(pt);
    


}
//--------------------------------------------------------------
void ofApp::draw(){
   //     ofSetColor(0);
     //   ofSetColor(220);
    //ofSetBackgroundAuto(false);
  //  ofSetColor(0, 0,0, 255); // contour (stroke) color
  //  ofDrawRectangle(0, 0,1000, 100);
 //   string msg;
 //   msg += "Sensor 1: " + ofToString(newx1) + "\n";
 //   msg += "Sensor 2: " + ofToString(newx2) + "\n";
   // msg += "(at time " + ofToString(readTime, 3) + ")";
 //   font.drawString(msg, 50, 100);


    ofFill();

   // ofDrawRectangle(0, 0,newx1, 50);
    int newRed = ofMap(newx1, 0, 1000, 0, 255); // newx = 21.5 a value [21, 22).
    int newBlue = ofMap(newx2, 0, 1000, 0, 255); // newx = 21.5 a value [21, 22).

    ofSetColor(newRed,newx2/10,newBlue);

    ofDrawRectangle(newx1, newx2,300, 300);
    ofNoFill();
    ofSetColor(255, 255,255, 255); // contour (stroke) color
    ofDrawRectangle(newx1, newx2,300, 300);
    for(int i = 0; i < 200; i ++){
        ofSetColor(newRed-i,newx2/10+i,newBlue);

        ofDrawRectangle(newx1+i, newx2-i,300, 300);
        ofNoFill();
        ofSetColor(255, 255,255, 255); // contour (stroke) color
        ofDrawRectangle(newx1, newx2,300, 300);
    }
    ofFill();

    line.draw();
    
  //  ofSetColor(newRed,newx2/10,newBlue);
 //   ofSetRectMode(OF_RECTMODE_CENTER);
 //   ofDrawRectangle(ofGetMouseX(), ofGetMouseY(), 50, 50);  // Draw a 50 x 50 rect
   // ofSetBackgroundAuto(false);


}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    bSendSerialMessage = true;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}

