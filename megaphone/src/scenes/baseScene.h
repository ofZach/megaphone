
#pragma once


#include "ofMain.h"
#include "audioManager.h"


class baseScene {
	

	public: 
	
	virtual void setup(){};
	virtual void update(){};
	virtual void draw(){};
    virtual void clear(){};
    
    virtual void drawGui(){};
    
	
    analysisResults * results;
    
};

