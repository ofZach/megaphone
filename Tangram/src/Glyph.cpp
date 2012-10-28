//
//  Glyph.cpp
//  Tangram
//
//  Created by Elie Zananiri on 2012-10-26.
//
//

#include "Glyph.h"

vector<Limb> limbLibrary;
vector<ofColor> colorLibrary;

//--------------------------------------------------------------
void buildLimbLibrary()
{
    // Add all the limbs.
    Limb limb;
    limb.addCoord(ofVec3f(0, 0));
    limb.addCoord(ofVec3f(2, 2));
    limb.addCoord(ofVec3f(0, 4));
    limbLibrary.push_back(limb);

    limb.clearCoords();
    limb.addCoord(ofVec3f(0, 0));
    limb.addCoord(ofVec3f(4, 0));
    limb.addCoord(ofVec3f(2, 2));
    limbLibrary.push_back(limb);

    limb.clearCoords();
    limb.addCoord(ofVec3f(4, 0));
    limb.addCoord(ofVec3f(4, 2));
    limb.addCoord(ofVec3f(3, 1));
    limbLibrary.push_back(limb);

    limb.clearCoords();
    limb.addCoord(ofVec3f(2, 2));
    limb.addCoord(ofVec3f(3, 1));
    limb.addCoord(ofVec3f(4, 2));
    limb.addCoord(ofVec3f(4, 2));
    limb.addCoord(ofVec3f(3, 3));
    limb.addCoord(ofVec3f(2, 2));
    limbLibrary.push_back(limb);

    limb.clearCoords();
    limb.addCoord(ofVec3f(1, 3));
    limb.addCoord(ofVec3f(2, 2));
    limb.addCoord(ofVec3f(3, 3));
    limbLibrary.push_back(limb);

    limb.clearCoords();
    limb.addCoord(ofVec3f(2, 4));
    limb.addCoord(ofVec3f(0, 4));
    limb.addCoord(ofVec3f(1, 3));
    limb.addCoord(ofVec3f(1, 3));
    limb.addCoord(ofVec3f(3, 3));
    limb.addCoord(ofVec3f(2, 4));
    limbLibrary.push_back(limb);

    limb.clearCoords();
    limb.addCoord(ofVec3f(4, 2));
    limb.addCoord(ofVec3f(4, 4));
    limb.addCoord(ofVec3f(2, 4));
    limbLibrary.push_back(limb);
}

//--------------------------------------------------------------
void buildColorLibrary()
{
    colorLibrary.push_back(ofColor(245, 180, 69));
    colorLibrary.push_back(ofColor(221, 35, 66));
    colorLibrary.push_back(ofColor(93, 191, 185));
    colorLibrary.push_back(ofColor(206, 116, 172));
    colorLibrary.push_back(ofColor(86, 172, 220));
    colorLibrary.push_back(ofColor(0, 55, 115));
}

//--------------------------------------------------------------
Glyph::Glyph()
{
    _scale = _targetScale = 1;
    _rotation = _targetRotation = 0;

    if (limbLibrary.size() == 0) {
        buildLimbLibrary();
    }
    if (colorLibrary.size() == 0) {
        buildColorLibrary();
    }

    // Build the Glyph out of continuous limbs.
    int startIndex = ofRandom(0, limbLibrary.size() / 2);
    int stopIndex = ofRandom(startIndex + 1, limbLibrary.size());
    for (int i = startIndex; i < stopIndex; i++) {
        _limbs.push_back(limbLibrary[i]);
        int colorIndex = ofRandom(colorLibrary.size());
        _limbs.back().setColor(colorLibrary[colorIndex]);

        if (ofRandomuf() > 0.5f) {
            // Add some flapping!
            switch (i) {
                case 1:
                    _limbs.back().setupFlapping(ofVec3f(1, 1, 0));
                    break;
                case 2:
                    _limbs.back().setupFlapping(ofVec3f(1, -1, 0));
                    break;
                case 3:
                    _limbs.back().setupFlapping(ofVec3f(1, 1, 0));
                    break;
                case 4:
                    _limbs.back().setupFlapping(ofVec3f(1, 1, 0));
                    break;
                case 5:
                    _limbs.back().setupFlapping(ofVec3f(1, 0, 0));
                    break;
                case 6:
                    _limbs.back().setupFlapping(ofVec3f(1, -1, 0));
                    break;
            }
        }
    }    
}

//--------------------------------------------------------------
void Glyph::addLimb(Limb limb)
{
    _limbs.push_back(limb);
}

//--------------------------------------------------------------
void Glyph::clearLimbs()
{
    _limbs.clear();
}

//--------------------------------------------------------------
const vector<Limb>& Glyph::limbs()
{
    return _limbs;
}

//--------------------------------------------------------------
void Glyph::moveTo(int x, int y)
{
    _prevPosition = _position;
    _position.set(x, y);

    _targetScale = ofMap(_position.distance(_prevPosition), 0, MAX(ofGetWidth(), ofGetHeight()), 10, 1000);
    _targetRotation = RAD_TO_DEG * (atan2(_prevPosition.y - _position.y, _prevPosition.x - _position.x));

    if ((_rotation - _targetRotation) > 180) {
        _targetRotation += 360;
    }
    else if ((_targetRotation - _rotation) > 180) {
        _targetRotation -= 360;
    }
}

//--------------------------------------------------------------
void Glyph::update()
{
    if (_velocity.length() >= 1) {
        _position += _velocity;
    }

    _scale = ofLerp(_scale, _targetScale, 0.2);
    _rotation = ofLerp(_rotation, _targetRotation, 0.2);
    _velocity *= 0.9f;

    for (int i = 0; i < _limbs.size(); i++) {
        _limbs[i].update();
    }
}

//--------------------------------------------------------------
void Glyph::draw()
{
    ofPushStyle();
    ofFill();

    ofPushMatrix();
    ofTranslate(_position);
    ofScale(_scale, _scale, _scale);
    ofRotate(_rotation, 0, 0, 1);
    {
        for (int i = 0; i < _limbs.size(); i++) {
            _limbs[i].draw();
        }
    }
    ofPopMatrix();

    ofPopStyle();
}

//--------------------------------------------------------------
bool Glyph::animates()
{
    return _bAnimates;
}

//--------------------------------------------------------------
void Glyph::setAnimates(bool animates)
{
    _bAnimates = animates;
    
    for (int i = 0; i < _limbs.size(); i++) {
        _limbs[i].setFlaps(_bAnimates);
    }
}

//--------------------------------------------------------------
void Glyph::setScale(float scale)
{
    _targetScale = scale;
}

//--------------------------------------------------------------
void Glyph::setRotation(float rotation)
{
    _targetRotation = rotation;
}

//--------------------------------------------------------------
void Glyph::setVelocity(ofVec3f velocity)
{
    _velocity = velocity;
}
