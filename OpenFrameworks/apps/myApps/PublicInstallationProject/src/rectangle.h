//
//  rectangle.hpp
//  PublicInstallationProject
//
//  Created by edward on 10/28/19.
//
#include "ofMain.h"

class rectangle {

    public:

        rectangle();
        void draw();
        void interpolateByPct(float myPct);

        ofPoint pos;
        ofPoint posa;
        ofPoint    posb;
        float pct;    // what pct are we between "a" and "b"
};
