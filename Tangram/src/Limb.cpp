//
//  Limb.cpp
//  Tangram
//
//  Created by Elie Zananiri on 2012-10-26.
//
//

#include "Limb.h"

void Limb::addCoord(ofVec3f coord)
{
    _coords.push_back(coord);
}

void Limb::clearCoords()
{
    _coords.clear();
}

const vector<ofVec3f>& Limb::coords()
{
    return _coords;
}

void Limb::setColor(ofColor color)
{
    _color = color;
}

const ofColor& Limb::color()
{
    return _color;
}

void Limb::draw()
{
    ofSetColor(_color);
    ofBeginShape();
    for (int i = 0; i < _coords.size(); i++) {
        ofVertex(_coords[i]);
    }
    ofEndShape(true);
}
