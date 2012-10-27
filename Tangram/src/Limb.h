//
//  Limb.h
//  Tangram
//
//  Created by Elie Zananiri on 2012-10-26.
//
//

#pragma once

#include "ofMain.h"

class Limb
{
public:
    void addCoord(ofVec3f coord);
    void clearCoords();
    const vector<ofVec3f>& coords();

    void setColor(ofColor color);
    const ofColor& color();

    void draw();

private:
    vector<ofVec3f> _coords;
    ofColor _color;
};
