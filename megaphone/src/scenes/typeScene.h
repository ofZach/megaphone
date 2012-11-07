
#pragma once

#include "ofMain.h"
#include "Glyph.h"
#include "baseScene.h"
#include "audioManager.h"
#include "glyphShapes.h"


#include "particle.h"
#include "spring.h"
#include "vectorField.h"
#include "blob.h"


class typeScene : public baseScene {
	

	public: 
	
    void setup();
    void update();
    void draw();
    
    void clear(){
        total.clear();
    }
	
    vector < float > angleDiffs;
    vector < float > distances;
    ofPolyline total;
    float angle;
    float angleSmooth;
    ofPoint catchPoint;
    vector < Glyph > glyphs;
    
    
    float pitch;
    float volume;
    bool bLoudEnough;
    float pitchSmooth;      // non aubio needs some love -- works well w/ whisteling. 
    
    
    ofPolyline blah;
    
    float glyphDistance;
    
    bool bSilence;

    glyphShapes GS;
    

    
    //-------------------------------
    
    
    vectorField VF;
    vector < blob > blobs;
    float prevMouseX;
    float prevMouseY;
    
    
    
    blob currentBlob;
    
    
};

