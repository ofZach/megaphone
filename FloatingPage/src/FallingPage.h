//
//  FallingPage.h
//  FloatingPage
//
//  Created by Elie Zananiri on 2012-11-01.
//
//

#pragma once

#include "ofMain.h"

class FallingPage
{
public:
    float ty;
    ofPoint pos;
    float twirl;
    float tilt;

    ofPoint realPos;
    float ta;
    float pivotAngle;
    int pivotLength;

    int pause;

    ofPath path;

    FallingPage();
    void update();
    void draw();
};