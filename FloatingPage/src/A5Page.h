//
//  A5Page.h
//  FloatingPage
//
//  Created by Elie Zananiri on 2012-11-01.
//
//

#include "ofMain.h"

class A5Page
{
public:
    float ty;
    ofPoint pos;
    float twirl;
    float tilt;

    int pause;

    float jump;

    ofPath path;

    A5Page();
    void bend(float pct);
    void update();
    void draw();
};