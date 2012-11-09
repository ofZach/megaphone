//
//  Tangram.cpp
//
//  Created by Patricio Gonzalez Vivo on 11/6/12.
//  Copyright (c) 2012 http://www.PatricioGonzalezVivo.com. All rights reserved.
//

#include "Tangram.h"

bool comparisonFunction(  Shape * a, Shape * b ) {
	return a->actual.pos.z < b->actual.pos.z;
}

Tangram::Tangram(){
    bTrail  = false;
    bDebug  = false;
    bEdit   = false;
    bSortZ  = true;
    
    selectedLimb = -1;
    scale = 1.0;
    targetScale = 1.0;
    
    ofAddListener(ofEvents().mousePressed, this, &Tangram::_mousePressed);
    ofAddListener(ofEvents().mouseDragged, this, &Tangram::_mouseDragged);
    ofAddListener(ofEvents().mouseReleased, this, &Tangram::_mouseReleased);
    ofAddListener(ofEvents().keyPressed, this, &Tangram::_keyPressed);
}

void Tangram::update(float _posLerp, float _rotLerp, float _camLerp, int _wakeUpDist){
    
    for(int i = 0; i < limbs.size(); i++){
        
        if (bEdit){
            limbs[i]->bDebug = limbs[i]->mouseHover();
        }
        
        if (bDebug)
            limbs[i]->bDebug = true;
        
        limbs[i]->update( _posLerp,  _rotLerp);			
		
		
		
        
        //if (limbs[i]->getDistanceToTarget() > _wakeUpDist)
//            break;
    }
    
    scale = ofLerp(scale, targetScale, _camLerp);
}

void Tangram::draw(){
    
    ofPushMatrix();
    ofScale(scale, scale, scale);
    
    if (bSortZ){
    //  Sort the Shapes depending their position on Z to avoid extrange behaviur 
    //
        vector<Shape*> sortByZ;
        sortByZ = limbs;
        sort( sortByZ.begin(), sortByZ.end(), comparisonFunction );
        
        for(int i = 0; i < sortByZ.size(); i++){
            sortByZ[i]->draw();
        }
        
    } else {
        for(int i = 0; i < limbs.size(); i++){
            limbs[i]->draw();
        }
    }
    
    ofPopMatrix();
}

// ----------------------------------------------------- SETUP
bool Tangram::load( string _file ){
    
    ofxXmlSettings XML;
    if (XML.loadFile(_file)){
        
        int total = XML.getNumTags("shape");
        
        //  If not have 7 shapes... delete everthing and make them
        //
        if ( total != limbs.size()){
            clear();
            for (int i = 0; i < total; i++){
                Shape *newShape = new Shape();
                newShape->makeNumber(i);
                newShape->bTrail = &bTrail;
                limbs.push_back( newShape );
                XML.popTag();
            }
        }
        
        //  Just load the new positions and rotations
        //
        targetScale = XML.getValue("scale", 1.0);
        for (int i = 0; i < total; i++){
            if ( XML.pushTag("shape",i ) ){
                
                limbs[i]->setTargetPos(ofPoint(XML.getValue("pos:x", 0.0),
                                             XML.getValue("pos:y", 0.0),
                                             XML.getValue("pos:z", 0.0)));
                limbs[i]->setTargetRot(ofPoint(XML.getValue("rot:x", 0.0),
                                             XML.getValue("rot:y", 0.0),
                                             XML.getValue("rot:z", 0.0)));
                XML.popTag();
            }
        }
        
        _centerShapes();
        
        return true;
    } else {
        return false;
    }
}

bool Tangram::save( string _file ){
    ofxXmlSettings XML;
    XML.loadFile(_file);
    
    for (int i = 0; i < limbs.size(); i++){
        XML.addTag("shape");
    }
    
    for (int i = 0; i < limbs.size(); i++){
        if ( XML.pushTag("shape",i ) ){
            
            XML.setValue("pos:x", limbs[i]->getTargetPos().x);
            XML.setValue("pos:y", limbs[i]->getTargetPos().y);
            XML.setValue("pos:z", limbs[i]->getTargetPos().z);
            XML.setValue("rot:x", limbs[i]->getTargetRot().x);
            XML.setValue("rot:y", limbs[i]->getTargetRot().y);
            XML.setValue("rot:z", limbs[i]->getTargetRot().z);
            
            XML.popTag();
        }
    }
    return XML.saveFile(_file);
}

void Tangram::createFromGlyph( Glyph *_glyph ){
    clear();
    
    for (int i = 0; i < _glyph->limbs().size(); i++){
        Shape *newShape = new Shape();
        newShape->makeFromLimb(i, *_glyph );
        newShape->bTrail = &bTrail;
        limbs.push_back( newShape );
    }
    
    _centerShapes();
}

void Tangram::createSet(){
    clear();
    
    for ( int i = 0; i < 7; i++){
        Shape *newShape = new Shape();
        newShape->makeNumber(i);
        newShape->bTrail = &bTrail;
        limbs.push_back( newShape );
    }
    
    _centerShapes();
}

void Tangram::clear(){
    for (int i = 0; i < limbs.size(); i++){
        delete limbs[i];
    }
    limbs.clear();
}

void Tangram::_centerShapes(){
    
    ofPolyline centers;
    for (int i = 0; i < limbs.size(); i++){
        centers.addVertex(limbs[i]->getTargetPos());
    }
    
    _moveShapesTo( centers.getBoundingBox().getCenter() );
}

void Tangram::_moveShapesTo(ofVec3f _targetPos){
    for(int i = 0; i < limbs.size();i++){
        limbs[i]->setTargetPos( limbs[i]->getTargetPos() - _targetPos);
    }
}

//------------------------------------------------- ACTIONS

void Tangram::rotateY(float _angle){
    
    ofMatrix4x4 matrix;
    matrix.makeIdentityMatrix();
    matrix.rotate( _angle, 0.0, 1.0, 0.0);
    
    for(int i = 0; i < limbs.size();i++){
        
        ofPoint pos = limbs[i]->getTargetPos();
        ofPoint rot = limbs[i]->getTargetRot();
        
        limbs[i]->setTargetRot( ofPoint(rot.x,rot.y-_angle,rot.z) );
        limbs[i]->setTargetPos( matrix * pos );
    }
}


void Tangram::rotateZ(float _angle){
    
    ofMatrix4x4 matrix;
    matrix.makeIdentityMatrix();
    matrix.rotate( _angle, 0.0, 0.0, 1.0);
    
    for(int i = 0; i < limbs.size();i++){
        
        ofPoint pos = limbs[i]->getTargetPos();
        ofPoint rot = limbs[i]->getTargetRot();
        
        limbs[i]->setTargetRot( ofPoint(rot.x,rot.y,rot.z-_angle) );
        limbs[i]->setTargetPos( matrix * pos );
    }
}

void Tangram::rotateX(float _angle){
    
    ofMatrix4x4 matrix;
    matrix.makeIdentityMatrix();
    matrix.rotate( _angle, 1.0, 0.0, 0.0);
    
    for(int i = 0; i < limbs.size();i++){
        
        ofPoint pos = limbs[i]->getTargetPos();
        ofPoint rot = limbs[i]->getTargetRot();
        
        limbs[i]->setTargetRot( ofPoint(rot.x-_angle,rot.y,rot.z) );
        limbs[i]->setTargetPos( matrix * pos );
    }
}





void Tangram::addCentralRepulsionForce( float _amount ){
    for(int i = 0; i < limbs.size(); i++){
        limbs[i]->addRepulsionForce( ofPoint(0,0), 0.1 + _amount*1000,0.5);
    }
}

//----------------------------------------------------------- EVENTS
void Tangram::_mousePressed(ofMouseEventArgs &e){
	if(bEdit){
        for(int i = 0; i < limbs.size(); i++){
            if (limbs[i]->mouseHover()){
                selectedLimb = i;
                break;
            }
        }
    }
}

void Tangram::_mouseDragged(ofMouseEventArgs &e){
    if (selectedLimb >= 0 && selectedLimb < limbs.size() ){
        limbs[ selectedLimb ]->setTargetPos(ofPoint(ofGetMouseX()-ofGetWidth()*0.5,
                                                    ofGetMouseY()-ofGetHeight()*0.5));
    }
}

void Tangram::_mouseReleased(ofMouseEventArgs &e){
    if (bEdit){
        selectedLimb = -1;
    }
}

void Tangram::_keyPressed(ofKeyEventArgs &e){
    if (bEdit){
        if (selectedLimb >= 0 && selectedLimb < limbs.size() ){
            if (e.key == OF_KEY_RIGHT){
                limbs[ selectedLimb ]->turnRight();
            } else if (e.key == OF_KEY_LEFT){
                limbs[ selectedLimb ]->turnLeft();
            } else if (e.key == 'h' || e.key == OF_KEY_UP){
                limbs[ selectedLimb ]->flipH();
            } else if (e.key == 'v' || e.key == OF_KEY_DOWN){
                limbs[ selectedLimb ]->flipV();
            }
        }
    }
}