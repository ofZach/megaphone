//
//  FlipperPage.h
//  FloatingPage
//
//  Created by Elie Zananiri on 2012-11-01.
//
//

#pragma once

#include "ofMain.h"

class FlipperPage
{
public:
    float ty;
    ofPoint pos;
    float twirl;
    float tilt;

    ofPoint realPos;
    float flipAngle;

    int pause;

    ofPath path;

    FlipperPage();
    void bend(float topPct, float bottomPct);

    void update();
    void draw();
};