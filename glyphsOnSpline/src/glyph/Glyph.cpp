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
    
    cout << "buildLimbLibrarys" <<endl;
    limbLibrary.clear();
    
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
    
    // let's center and then manipulate this all
    
    
    for (int i = 0; i < limbLibrary.size(); i++){
        for (int j = 0; j < limbLibrary[i].coords().size(); j++){
            limbLibrary[i].coords()[j] -= ofPoint(2,2);   
        }
    }
    
    if (ofRandom(0,1) > 0.5){
        for (int i = 0; i < limbLibrary.size(); i++){
            for (int j = 0; j < limbLibrary[i].coords().size(); j++){
                limbLibrary[i].coords()[j].x *= -1;   
            }
        }
    }

//    if (ofRandom(0,1) > 0.5){
//        for (int i = 0; i < limbLibrary.size(); i++){
//            for (int j = 0; j < limbLibrary[i].coords().size(); j++){
//                limbLibrary[i].coords()[j].y = -1;   
//            }
//        }
//    }
    
    //if (ofRandom(0,1) > 0.5){
        for (int i = 0; i < limbLibrary.size(); i++){
            for (int j = 0; j < limbLibrary[i].coords().size(); j++){
                float ang = PI/2 * (int)ofRandom(0,50);
                //cout << ang / (PI/2) << endl;
                ofPoint pos = limbLibrary[i].coords()[j];
                float len = pos.length();
                float angle = atan2(pos.y, pos.x);
                angle += PI/2 * 3;
                
                limbLibrary[i].coords()[j].set( len * cos(angle), len * sin(angle));
                
            }
        }
    //}
    
    
    
    for (int i = 0; i < limbLibrary.size(); i++){
        for (int j = 0; j < limbLibrary[i].coords().size(); j++){
            limbLibrary[i].coords()[j] += ofPoint(2,2);   
        }
    }
    
    
}

//--------------------------------------------------------------
void buildColorLibrary()
{
    /*colorLibrary.push_back(ofColor(245, 180, 69));
    colorLibrary.push_back(ofColor(221, 35, 66));
    colorLibrary.push_back(ofColor(93, 191, 185));
    colorLibrary.push_back(ofColor(206, 116, 172));
    colorLibrary.push_back(ofColor(86, 172, 220));
    colorLibrary.push_back(ofColor(0, 55, 115));*/
    colorLibrary.push_back(ofColor(0,0,0));
    colorLibrary.push_back(ofColor(100,100,100));
    colorLibrary.push_back(ofColor(50,50,50));
    
}

//--------------------------------------------------------------
Glyph::Glyph()
{
    _scale = _targetScale = 1;
    _rotation = _targetRotation = 0;

    //setInitialCondition(ofVec2f(), ofVec2f());
    _damping = 0.15;
    _bFixed = false;

    //if (limbLibrary.size() == 0) {
        buildLimbLibrary();
   // }
    if (colorLibrary.size() == 0) {
        buildColorLibrary();
    }

    // Build the Glyph out of continuous limbs.

    
//    int limbCount = ofRandom(1, 6);
//    bool usedFlags[limbLibrary.size()];
//    for (int i=0; i < limbLibrary.size(); i++) {
//        usedFlags[i] = false;
//    }
//    int usedCount = 0;
//    
//    // randomly select half the limbs.
//    do {
//        int index = ofRandom(limbLibrary.size());
//        if (!usedFlags[index]) {
//            addLimb(limbLibrary[index]);
//            Limb& newLimb = _limbs.back();
//
//            int colorIndex = ofRandom(colorLibrary.size());
//            newLimb.setColor(colorLibrary[colorIndex]);
//
//            usedFlags[index] = true;
//            usedCount++;
//        }
//    } while (usedCount < limbCount / 2);
//
//    // sequentially select the rest.
//    for (int i = 0; i < limbLibrary.size(); i++) {
//        if (!usedFlags[i]) {
//            addLimb(limbLibrary[i]);
//            Limb& newLimb = _limbs.back();
//
//            int colorIndex = ofRandom(colorLibrary.size());
//            newLimb.setColor(colorLibrary[colorIndex]);
//
//            usedFlags[i] = true;
//            usedCount++;
//
//            if (usedCount >= limbCount) break;
//        }
//    }


    int startIndex = ofRandom(0, limbLibrary.size() / 2);
    int stopIndex = ofRandom(startIndex + 1, limbLibrary.size());
    for (int i = startIndex; i < stopIndex; i++) {
        addLimb(limbLibrary[i]);
        Limb& newLimb = _limbs.back();
        
        int colorIndex = ofRandom(colorLibrary.size());
        newLimb.setColor(colorLibrary[colorIndex]);

//        if (ofRandomuf() > 0.5f) {
//            // Add some flapping!
//            switch (i) {
//                case 1:
//                    newLimb.setupFlapping(ofVec3f(1, 1, 0));
//                    break;
//                case 2:
//                    newLimb.setupFlapping(ofVec3f(1, -1, 0));
//                    break;
//                case 3:
//                    newLimb.setupFlapping(ofVec3f(1, 1, 0));
//                    break;
//                case 4:
//                    newLimb.setupFlapping(ofVec3f(1, 1, 0));
//                    break;
//                case 5:
//                    newLimb.setupFlapping(ofVec3f(1, 0, 0));
//                    break;
//                case 6:
//                    newLimb.setupFlapping(ofVec3f(1, -1, 0));
//                    break;
//            }
//        }
    }    
}

//--------------------------------------------------------------
Glyph::~Glyph()
{

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

////--------------------------------------------------------------
//void Glyph::setInitialCondition(ofVec2f pos, ofVec2f vel)
//{
//    _pos.set(pos);
//    _vel.set(vel);
//}
//
////--------------------------------------------------------------
//void Glyph::resetForce()
//{
//    // Should be reset every frame.
//    _acc.set(0, 0);
//}
//
////--------------------------------------------------------------
//void Glyph::addForce(ofVec2f frc)
//{
//    _acc += frc;
//}
//
////--------------------------------------------------------------
//void Glyph::addDampingForce()
//{
//    _acc -= _vel * _damping;
//}


//--------------------------------------------------------------
void Glyph::update(/*ofxSplineTool& spline*/)
{
    
//    if (_bFixed == false) {
//		_vel += _acc;
//		_pos += _vel;
//	}

    _targetRotation = RAD_TO_DEG * atan2(_pos.y - _prv.y, _pos.x - _prv.x);
    if ((_rotation - _targetRotation) > 180)
        _targetRotation += 360;
     if ((_targetRotation - _rotation) > 180)
        _targetRotation -= 360;


    //_scale = ofLerp(_scale, _targetScale, 0.2);
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
    
    
    float h = absBounds().height;
    float y = absBounds().y;

    //cout << pctOfBig << endl;
    
    ofPushMatrix();
//    ofTranslate(_pos.x - absBounds().x * _scale, _pos.y);
    ofTranslate(_pos.x, _pos.y);
    ofScale(_scale, _scale, _scale);
    ofRotate(_rotation, 0, 0, 1);
    ofTranslate(-_bounds.width / 2, -_bounds.height / 2);
    {
        
        
        //ofNoFill();
        //ofSetColor(255);
        //ofRect(absBounds());
        
        //if (bAreWeSmall){
        for (int i = 0; i < _limbs.size(); i++) {
            _limbs[i].draw();
        }
    //}
        
    }
    ofPopMatrix();

    ofSetColor(255,0,0);
    ofCircle(_pos, 2);

    ofPopStyle();
}

//--------------------------------------------------------------
void Glyph::debug()
{
//    ofPushStyle();
//    ofNoFill();
//    ofSetColor(128);
//    
//    
//    ofPushMatrix();
//    ofTranslate(_pos);
//    ofScale(_scale, _scale, _scale);
//    ofRotate(_rotation, 0, 0, 1);
//    {
//        ofRect(absBounds());
//    }
//    ofPopMatrix();
//
//    
//    
//    //
//    ofPopStyle();
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
    //_targetScale = scale;
    _scale = scale;
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

    for (int i = 0; i < _limbs.size(); i++) {
        if (i == 0) absBounds.set(_limbs[i].getBoundingBox());
        else absBounds.growToInclude(_limbs[i].getBoundingBox());
    }

    absBounds.width  *= _scale;
    absBounds.height *= _scale;
    
//    // Calculate bounds taking the rotation into account.
//    float vx, vy;  // Unrotated vertex coords.
//    float ang = DEG_TO_RAD * _rotation;
//
//    // top-left
//    vx = _pos.x;
//    vy = _pos.y;
//    float x0 = (vx - _pos.x) * cosf(ang) - (vy - _pos.y) * sinf(ang) + _pos.x;
//    float y0 = (vx - _pos.x) * sinf(ang) - (vy - _pos.y) * cosf(ang) + _pos.y;
//    absBounds.set(x0, y0, 0, 0);
//
//    // top-right
//    vx = _pos.x + _bounds.width * _scale;
//    vy = _pos.y;
//    float x1 = (vx - _pos.x) * cosf(ang) - (vy - _pos.y) * sinf(ang) + _pos.x;
//    float y1 = (vx - _pos.x) * sinf(ang) - (vy - _pos.y) * cosf(ang) + _pos.y;
//    absBounds.growToInclude(x1, y1);
//
//    // bottom-right
//    vx = _pos.x + _bounds.width * _scale;
//    vy = _pos.y + _bounds.height * _scale;
//    float x2 = (vx - _pos.x) * cosf(ang) - (vy - _pos.y) * sinf(ang) + _pos.x;
//    float y2 = (vx - _pos.x) * sinf(ang) - (vy - _pos.y) * cosf(ang) + _pos.y;
//    absBounds.growToInclude(x2, y2);
//
//    // bottom-left
//    vx = _pos.x;
//    vy = _pos.y + _bounds.height * _scale;
//    float x3 = (vx - _pos.x) * cosf(ang) - (vy - _pos.y) * sinf(ang) + _pos.x;
//    float y3 = (vx - _pos.x) * sinf(ang) - (vy - _pos.y) * cosf(ang) + _pos.y;
//    absBounds.growToInclude(x3, y3);

    return absBounds;
}
