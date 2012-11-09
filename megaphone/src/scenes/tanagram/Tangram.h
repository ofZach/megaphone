//
//  Tangram.h
//
//  Created by Patricio Gonzalez Vivo on 11/6/12.
//  Copyright (c) 2012 http://www.PatricioGonzalezVivo.com. All rights reserved.
//

#ifndef TANGRAM
#define TANGRAM

#include "ofMain.h"

#include "Glyph.h"
#include "Shape.h"

#include "ofxGui.h"
#include "ofxXmlSettings.h"

class Tangram : public ofRectangle {
public:
    Tangram();
    
    bool    load( string _file );
    bool    save( string _file );
    
    void    clear();
    void    createSet();
    void    createFromGlyph( Glyph *_glyph );
    
    void    rotateY(float _angle);
	void    rotateX(float _angle);
	void    rotateZ(float _angle);
    void    addCentralRepulsionForce( float _amount );
    
    void    update(float _posLerp, float _rotLerp, float _camLerp, int _wakeUpDist);
    void    draw();
    
    bool    bSortZ;
    bool    bTrail;
    bool    bDebug;
    bool    bEdit;
    
private:
    virtual void    _mousePressed(ofMouseEventArgs &e);
    virtual void    _mouseDragged(ofMouseEventArgs &e);
    virtual void    _mouseReleased(ofMouseEventArgs &e);
    virtual void    _keyPressed(ofKeyEventArgs &e);
    
    void            _centerShapes();
    void            _moveShapesTo(ofVec3f _targetPos);
    
    vector<Shape*>   limbs;
    
    float           scale, targetScale;
    
    int             selectedLimb;
};

#endif
