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
        addLimb(limbLibrary[i]);
        Limb& newLimb = _limbs.back();
        
        int colorIndex = ofRandom(colorLibrary.size());
        newLimb.setColor(colorLibrary[colorIndex]);

        if (ofRandomuf() > 0.5f) {
            // Add some flapping!
            switch (i) {
                case 1:
                    newLimb.setupFlapping(ofVec3f(1, 1, 0));
                    break;
                case 2:
                    newLimb.setupFlapping(ofVec3f(1, -1, 0));
                    break;
                case 3:
                    newLimb.setupFlapping(ofVec3f(1, 1, 0));
                    break;
                case 4:
                    newLimb.setupFlapping(ofVec3f(1, 1, 0));
                    break;
                case 5:
                    newLimb.setupFlapping(ofVec3f(1, 0, 0));
                    break;
                case 6:
                    newLimb.setupFlapping(ofVec3f(1, -1, 0));
                    break;
            }
        }
    }    
}

//--------------------------------------------------------------
void Glyph::addLimb(Limb limb)
{
    _limbs.push_back(limb);
    _bounds.growToInclude(_limbs.back().bounds());
}

//--------------------------------------------------------------
void Glyph::clearLimbs()
{
    _limbs.clear();
    _bounds.set(0, 0, 0, 0);
}

//--------------------------------------------------------------
vector<Limb>& Glyph::limbs()
{
    return _limbs;
}

//--------------------------------------------------------------
const ofVec3f& Glyph::position()
{
    return _position;
}

//--------------------------------------------------------------
void Glyph::setPosition(ofVec3f position)
{
    _prevPosition = _position;
    _position.set(position);
}

//--------------------------------------------------------------
void Glyph::moveTo(ofVec3f position)
{
    setPosition(position);

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
void Glyph::moveBy(ofVec3f offset)
{
    moveTo(_position + offset);
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
void Glyph::addVelocity(ofVec3f velocity)
ofRectangle Glyph::absBounds()
{
    _velocity += velocity;
    ofRectangle absBounds;

    // Calculate bounds taking the rotation into account.
    float vx, vy;  // Unrotated vertex coords.
    float ang = DEG_TO_RAD * _rotation;

    // top-left
    vx = _pos.x;
    vy = _pos.y;
    float x0 = (vx - _pos.x) * cosf(ang) - (vy - _pos.y) * sinf(ang) + _pos.x;
    float y0 = (vx - _pos.x) * sinf(ang) - (vy - _pos.y) * cosf(ang) + _pos.y;
    absBounds.set(x0, y0, 0, 0);

    // top-right
    vx = _pos.x + _bounds.width * _scale;
    vy = _pos.y;
    float x1 = (vx - _pos.x) * cosf(ang) - (vy - _pos.y) * sinf(ang) + _pos.x;
    float y1 = (vx - _pos.x) * sinf(ang) - (vy - _pos.y) * cosf(ang) + _pos.y;
    absBounds.growToInclude(x1, y1);

    // bottom-right
    vx = _pos.x + _bounds.width * _scale;
    vy = _pos.y + _bounds.height * _scale;
    float x2 = (vx - _pos.x) * cosf(ang) - (vy - _pos.y) * sinf(ang) + _pos.x;
    float y2 = (vx - _pos.x) * sinf(ang) - (vy - _pos.y) * cosf(ang) + _pos.y;
    absBounds.growToInclude(x2, y2);

    // bottom-left
    vx = _pos.x;
    vy = _pos.y + _bounds.height * _scale;
    float x3 = (vx - _pos.x) * cosf(ang) - (vy - _pos.y) * sinf(ang) + _pos.x;
    float y3 = (vx - _pos.x) * sinf(ang) - (vy - _pos.y) * cosf(ang) + _pos.y;
    absBounds.growToInclude(x3, y3);

    return absBounds;
}
