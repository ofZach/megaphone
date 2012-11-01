//
//  FlatPage.h
//  FloatingPage
//
//  Created by Elie Zananiri on 2012-11-01.
//
//

#pragma once

#include "ofMain.h"

class FlatPage
{
public:
    float ty;
    ofPoint pos;
    float twirl;
    float tilt;

    int pause;

    ofPath path;

    FlatPage();
    void update();
    void draw();
};