//
//  Glyph.cpp
//  Tangram
//
//  Created by Elie Zananiri on 2012-10-26.
//
//

#include "Glyph.h"





//--------------------------------------------------------------
void buildColorLibrary(){
    
}

//--------------------------------------------------------------
void Glyph::setup(){
    _scale = 1;
    _rotation = 0;

    bool bWhich = ofRandom(0,1) > 0.5 ? true : false;

    int startIndex = ofRandom(0, GS->limbLibraryA.size() / 2);
    int stopIndex = ofRandom(startIndex + 1, GS->limbLibrary.size());
    for (int i = startIndex; i < stopIndex; i++) {
        
        Limb newLimb;
        
        if (bWhich){
            for (int j  = 0; j < GS->limbLibraryA[i].coords.size(); j++){
                newLimb.addCoord( GS->limbLibraryA[i].coords[j]);
            }
        } else {
            for (int j  = 0; j < GS->limbLibraryB[i].coords.size(); j++){
                newLimb.addCoord( GS->limbLibraryB[i].coords[j]);
            }
        }
        
        _limbs.push_back(newLimb);
        _bounds.growToInclude(_limbs.back().bounds());
        _limbs.back().mesh = bWhich ? GS->meshA[i] : GS->meshB[i];
        float rand = ofRandom(255,100);
        _limbs.back().setColor(ofColor(rand, rand,rand));


    }    

    ofRectangle rect = _bounds;
    ofPoint midPt = ofPoint(rect.x + rect.width/2, rect.y + rect.height/2);
    for (int i = 0; i < _limbs.size(); i++) {
        for (int j = 0; j < _limbs[i].coords().size(); j++) {
            _limbs[i].coords()[j] -= midPt;
        }
    }
    

}

//--------------------------------------------------------------
Glyph::~Glyph(){

}

//--------------------------------------------------------------
void Glyph::addLimb(Limb limb){
    _limbs.push_back(limb);
    _bounds.growToInclude(_limbs.back().bounds());
}

//--------------------------------------------------------------
void Glyph::clearLimbs(){
    _limbs.clear();
    _bounds.set(0, 0, 0, 0);
}

//--------------------------------------------------------------
vector<Limb>& Glyph::limbs(){
    return _limbs;
}

#pragma mark - Physics Methods




//--------------------------------------------------------------
void Glyph::update(){
    for (int i = 0; i < _limbs.size(); i++) {
        _limbs[i].update();
    }
}


void Glyph::draw(ofPoint from){
    
    ofRectangle boundsWindow = ofRectangle(from.x -ofGetWidth()/2,from.y - ofGetHeight()/2,ofGetWidth(), ofGetHeight());
    
    if (!boundsWindow.inside(_pos)) return;
    
    ofPushMatrix();
    ofTranslate(_pos.x, _pos.y);
    ofScale(_scale, _scale, _scale);
    ofRotate(_rotation, 0, 0, 1);
    ofNoFill();
    ofSetColor(255);
    ofRectangle rect = _bounds;
    //ofRect(rect);
    
    ofFill();
    //if (bAreWeSmall){
    for (int i = 0; i < _limbs.size(); i++) {
        _limbs[i].draw();
    }
    ofPopMatrix();

}

//--------------------------------------------------------------
void Glyph::draw(){

    ofPushMatrix();
    ofTranslate(_pos.x, _pos.y);
    ofScale(_scale, _scale, _scale);
    ofRotate(_rotation, 0, 0, 1);
        ofNoFill();
        ofSetColor(255);
        ofRectangle rect = _bounds;
        //ofRect(rect);
        
        ofFill();
        //if (bAreWeSmall){
        for (int i = 0; i < _limbs.size(); i++) {
           _limbs[i].draw();
        }
    ofPopMatrix();
}



//--------------------------------------------------------------
bool Glyph::animates(){
    return _bAnimates;
}

//--------------------------------------------------------------
void Glyph::setAnimates(bool animates){
    _bAnimates = animates;
    
    for (int i = 0; i < _limbs.size(); i++) {
        _limbs[i].setFlaps(_bAnimates);
    }
}

//--------------------------------------------------------------
void Glyph::setScale(float scale){
    //_targetScale = scale;
    origScale = scale;
    _scale = scale;
    
}

//--------------------------------------------------------------
void Glyph::setRotation(float rotation){
    _rotation = rotation;
}


