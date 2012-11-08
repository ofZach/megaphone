//
//  mtl2dMappingVertex.h
//  ofxMTL2DMapping
//
//  Created by Hugues Bruyere on 12-06-06.
//

#pragma once

//OF
#include "ofMain.h"
#include "ofVec2f.h"

//MSA Libs
#include "ofxMSAInteractiveObject.h"

//--------------------------------------------------------------
//--------------------------------------------------------------
class mtl2dMappingVertex : public ofxMSAInteractiveObject {
	
public:
    
    static mtl2dMappingVertex* activeVertex;
    
    mtl2dMappingVertex();
    ~mtl2dMappingVertex();
	
    void kill();
    void update();
    void updateCenter();
    void drawBack();
    void drawTop();
    
    void snapIt(float _x, float _y);
	
    void init(float _x, float _y);
    
    static mtl2dMappingVertex* getActiveVertex();
    
    virtual void onRollOver(int x, int y);
    virtual void onRollOut();
    virtual void onMouseMove(int x, int y);
    virtual void onDragOver(int x, int y, int button);
    virtual void onDragOutside(int x, int y, int button);
    virtual void onPress(int x, int y, int button);
    virtual void onRelease(int x, int y, int button);
    virtual void onReleaseOutside(int x, int y, int button);
    
    bool toBeRemoved;
    bool isDefiningTectureCoord;
    ofVec2f center;
    
    void setAsActive();
    
    void left();
    void up();
    void right();
    void down();
	
};
