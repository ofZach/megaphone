//
//  OtherPage.h
//  FloatingPage
//
//  Created by Elie Zananiri on 2012-11-01.
//
//

#pragma once

#include "ofMain.h"

class OtherPage
{
public:
    float ty;
    ofPoint pos;
    float twirl;
    float tilt;

    int pause;

    ofPath path;

    OtherPage();
    void bend(float topPct, float bottomPct);
    void update();
    void draw();
};