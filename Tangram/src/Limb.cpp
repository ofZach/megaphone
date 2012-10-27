//
//  Limb.cpp
//  Tangram
//
//  Created by Elie Zananiri on 2012-10-26.
//
//

#include "Limb.h"

//--------------------------------------------------------------
Limb::Limb()
{
    _bFlaps = false;
}

//--------------------------------------------------------------
void Limb::addCoord(ofVec3f coord)
{
    _coords.push_back(coord);
}

//--------------------------------------------------------------
void Limb::clearCoords()
{
    _coords.clear();
}

//--------------------------------------------------------------
const vector<ofVec3f>& Limb::coords()
{
    return _coords;
}

//--------------------------------------------------------------
void Limb::setColor(ofColor color)
{
    _color = color;
}

//--------------------------------------------------------------
const ofColor& Limb::color()
{
    return _color;
}

//--------------------------------------------------------------
bool Limb::flaps()
{
    return _bFlaps;
}

//--------------------------------------------------------------
void Limb::enableFlapping(ofVec3f flapAxis)
{
    _bFlaps = true;
    _flapAxis = flapAxis;
    _flapRotation = 0;
    _flapSpeed = ofRandom(1, 5);
    _flapDirection = (ofRandomuf() < 0.5)? 1:-1;
}

//--------------------------------------------------------------
void Limb::disableFlapping()
{
    _bFlaps = false;
}

//--------------------------------------------------------------
void Limb::update()
{
    if (_bFlaps) {
        _flapRotation += (_flapSpeed * _flapDirection);
        if (ABS(_flapRotation) >= 90) {
            _flapDirection *= -1;
        }
    }
}

//--------------------------------------------------------------
void Limb::draw()
{
    if (_bFlaps) {
        ofTranslate(_coords[0]);
        ofRotate(_flapRotation, _flapAxis.x, _flapAxis.y, _flapAxis.z);
        ofTranslate(_coords[0] * -1);
    }

    ofSetColor(_color);
    ofBeginShape();
    for (int i = 0; i < _coords.size(); i++) {
        ofVertex(_coords[i]);
    }
    ofEndShape(true);
}
