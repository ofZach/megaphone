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

    setInitialCondition(ofVec2f(), ofVec2f());
    _damping = 0.15;
    _bFixed = false;

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

#pragma mark - Physics Methods

//--------------------------------------------------------------
void Glyph::setInitialCondition(ofVec2f pos, ofVec2f vel)
{
    _pos.set(pos);
    _vel.set(vel);
}

//--------------------------------------------------------------
void Glyph::resetForce()
{
    // Should be reset every frame.
    _acc.set(0, 0);
}

//--------------------------------------------------------------
void Glyph::addForce(ofVec2f frc)
{
    _acc += frc;
}

//--------------------------------------------------------------
void Glyph::addDampingForce()
{
    _acc -= _vel * _damping;
}

//--------------------------------------------------------------
void Glyph::bounceOffWalls(bool bDampenCollision)
{
	bool bDidCollide = false;

	// Set the wall limits.
	float minX = 0;
	float minY = 0;
	float maxX = ofGetWidth();
	float maxY = ofGetHeight();

    ofRectangle bounds = absBounds();

	if (bounds.getMaxX() > ofGetWidth()) {
		_pos.x -= bounds.getMaxX() - ofGetWidth(); // move to the edge, (important!)
		_vel.x *= -1;
		bDidCollide = true;
	}
    else if (bounds.getMinX() < 0){
		_pos.x = 0; // move to the edge, (important!)
		_vel.x *= -1;
		bDidCollide = true;
	}

	if (bounds.getMaxY() > ofGetHeight()) {
		_pos.y -= bounds.getMaxY() - ofGetHeight(); // move to the edge, (important!)
		_vel.y *= -1;
		bDidCollide = true;
	}
    else if (bounds.getMinY() < 0) {
		_pos.y = 0; // move to the edge, (important!)
		_vel.y *= -1;
		bDidCollide = true;
	}

	if (bDidCollide && bDampenCollision) {
		_vel *= 0.3;
	}
}

//--------------------------------------------------------------
void Glyph::addRepulsionForce(ofVec2f posOfForce, float radius, float scale)
{
	ofVec2f deltaPos = _pos - posOfForce;
	float length = deltaPos.length();

    // If we're close enough, update the force.
    if ((radius == 0) || (length <= radius)) {
		float pct = 1 - (length / radius);
        deltaPos.normalize();
		_acc += deltaPos * scale * pct;
    }
}

//------------------------------------------------------------
void Glyph::addRepulsionForce(Glyph& glyph, float radius, float scale)
{
    ofVec2f posOfForce = glyph.pos();

    ofVec2f deltaPos = _pos - posOfForce;
	float length = deltaPos.length();

    // If we're close enough, update the force.
    if ((radius == 0) || (length <= radius)) {
		float pct = 1 - (length / radius);
        deltaPos.normalize();
		_acc += deltaPos * scale * pct;
        glyph.acc() -= deltaPos * scale * pct;
    }
}

//--------------------------------------------------------------
void Glyph::addAttractionForce(ofVec2f posOfForce, float radius, float scale)
{
    ofVec2f deltaPos = _pos - posOfForce;
	float length = deltaPos.length();

    // If we're close enough, update the force.
    if ((radius == 0) || (length <= radius)) {
		float pct = 1 - (length / radius);
        deltaPos.normalize();
		_acc -= deltaPos * scale * pct;
    }
}

//--------------------------------------------------------------
void Glyph::addAttractionForce(Glyph& glyph, float radius, float scale)
{
    ofVec2f posOfForce = glyph.pos();

    ofVec2f deltaPos = _pos - posOfForce;
	float length = deltaPos.length();

    // If we're close enough, update the force.
    if ((radius == 0) || (length <= radius)) {
		float pct = 1 - (length / radius);
        deltaPos.normalize();
		_acc += deltaPos * scale * pct;
        glyph.acc() -= deltaPos * scale * pct;
        glyph.acc() += deltaPos * scale * pct;
    }
}

//--------------------------------------------------------------
void Glyph::update()
{
    if (_bFixed == false) {
		_vel += _acc;
		_pos += _vel;
	}

    _scale = ofLerp(_scale, _targetScale, 0.2);
    _rotation = ofLerp(_rotation, _targetRotation, 0.2);

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
    ofTranslate(_pos);
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
void Glyph::debug()
{
    ofPushStyle();
    ofNoFill();
    ofSetColor(128);
    ofRect(absBounds());
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
ofRectangle Glyph::absBounds()
{
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
