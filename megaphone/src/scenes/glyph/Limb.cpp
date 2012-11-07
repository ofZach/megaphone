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
    _bFlaps = true;
    _bounds.set(0, 0, 0, 0);
}

//--------------------------------------------------------------
void Limb::addCoord(ofVec3f coord)
{
    _coords.push_back(coord);
    _bounds.growToInclude(_coords.back());
}

//--------------------------------------------------------------
void Limb::clearCoords()
{
    _coords.clear();
    _bounds.set(0, 0, 0, 0);
}

//--------------------------------------------------------------
void Limb::setColor(ofColor color)
{
    _color = color;
}

//--------------------------------------------------------------
void Limb::setFlaps(bool flaps)
{
    _bFlaps = flaps;
}

//--------------------------------------------------------------
void Limb::setupFlapping(ofVec3f flapAxis)
{
    _bFlaps = true;
    _flapAxis = flapAxis;
    _flapRotation = 0;
    _flapSpeed = ofRandom(1, 5);
    _flapDirection = (ofRandomuf() < 0.5)? 1:-1;
}


ofRectangle Limb::getBoundingBox(){
    ofRectangle rect;
    for (int i = 0; i < _coords.size(); i++) {
        if (i == 0) rect.set(_coords[i].x, _coords[i].y, 0,0);
        else rect.growToInclude(_coords[i].x, _coords[i].y);
    }
    return rect;
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
//    if (_bFlaps) {
//        ofTranslate(_coords[0]);
//        ofRotate(_flapRotation, _flapAxis.x, _flapAxis.y, _flapAxis.z);
//        ofTranslate(_coords[0] * -1);
//    }

    ofSetColor(_color);
    //mesh->draw();
    
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < _coords.size(); i++) {
        glVertex2f(_coords[i].x, _coords[i].y);
    }
    glEnd();
    
    
//    if (mesh.getVertices().size() == 0){
//        if (_coords.size() == 4) mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
//        else mesh.setMode(OF_PRIMITIVE_TRIANGLES);
//        
//        //ofBeginShape();
//        for (int i = 0; i < _coords.size(); i++) {
//            mesh.addVertex(_coords[i]);
//        }
//        //mesh.drawWireframe();
//        mesh.draw();
//    } else {
//        //cout << 
    
//    }
    
}
