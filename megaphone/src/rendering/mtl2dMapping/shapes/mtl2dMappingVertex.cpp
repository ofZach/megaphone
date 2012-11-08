//
//  mtl2dMappingVertex.cpp
//  ofxMTL2DMapping
//
//  Created by Hugues Bruyere on 12-06-06.
//

#include "mtl2dMappingVertex.h"
#include "mtl2dMappingSettings.h"

mtl2dMappingVertex* mtl2dMappingVertex::activeVertex = NULL;

//--------------------------------------------------------------
mtl2dMappingVertex::mtl2dMappingVertex()
{
	disableAllEvents();
	enableMouseEvents();
	enabled = false;
	
	//Vertex
	setSize(30, 30);
	toBeRemoved = false;
    isDefiningTectureCoord = false;
	updateCenter();
}

//--------------------------------------------------------------
mtl2dMappingVertex::~mtl2dMappingVertex()
{
}

//--------------------------------------------------------------
void mtl2dMappingVertex::init(float _x, float _y) 
{
	setPos(_x, _y);
	
	//Center
	updateCenter();
}

//--------------------------------------------------------------
void mtl2dMappingVertex::kill()
{
	killMe();
}

//--------------------------------------------------------------
void mtl2dMappingVertex::update()
{
    
}

//--------------------------------------------------------------
void mtl2dMappingVertex::drawBack()
{
    ofNoFill();
    if(isMouseDown() || activeVertex == this) {
        ofSetColor(255, 255, 255, 200);
        ofCircle(x+15, y+15, 20);
    } else if(isMouseOver()) {
        ofSetColor(255, 255, 255, 125);
        ofCircle(x+15, y+15, 20);
    } else {
        //ofSetColor(255, 255, 255, 80);
    }		
}

//--------------------------------------------------------------
void mtl2dMappingVertex::drawTop()
{
    ofNoFill();
    if(isMouseDown() || activeVertex == this) {
        ofSetColor(255, 255, 0, 120);
    } else if(isMouseOver()) {
        ofSetColor(255, 0, 0, 120);
    } else {
        ofSetColor(255, 255, 255, 80);
    }
    
    //ofCircle(x+11, y+11, 8);
    ofRect(x+11, y+11, 8, 8);		
}

//--------------------------------------------------------------
mtl2dMappingVertex* mtl2dMappingVertex::getActiveVertex() 
{
	return activeVertex;
}

//--------------------------------------------------------------
void mtl2dMappingVertex::updateCenter() 
{
	center.set(x+15, y+15);
}

//--------------------------------------------------------------
void mtl2dMappingVertex::snapIt(float _x, float _y) 
{
    setPos(_x, _y);
    updateCenter();
}


#pragma mark -
#pragma mark up with keyboard
//--------------------------------------------------------------
void mtl2dMappingVertex::left() 
{
    snapIt(x-1, y);
}

//--------------------------------------------------------------
void mtl2dMappingVertex::up() 
{
    snapIt(x, y-1);
}

//--------------------------------------------------------------
void mtl2dMappingVertex::right() 
{
    snapIt(x+1, y);
}

//--------------------------------------------------------------
void mtl2dMappingVertex::down() 
{
    snapIt(x, y+1);
}


#pragma mark -
#pragma mark Interactive Obj Callback methods
//--------------------------------------------------------------
void mtl2dMappingVertex::onRollOver(int x, int y) 
{
}

//--------------------------------------------------------------
void mtl2dMappingVertex::onRollOut() 
{
}

//--------------------------------------------------------------
void mtl2dMappingVertex::onMouseMove(int x, int y)
{
}

//--------------------------------------------------------------
void mtl2dMappingVertex::onDragOver(int x, int y, int button) 
{
	if(activeVertex == this) {
		this->x = x - width/2;
		this->y = y - height/2;
		updateCenter();
	}
}

//--------------------------------------------------------------
void mtl2dMappingVertex::onDragOutside(int x, int y, int button) 
{
	if(activeVertex == this) {
		this->x = x - width/2;
		this->y = y - height/2;
		updateCenter();
	}
}

//--------------------------------------------------------------
void mtl2dMappingVertex::onPress(int x, int y, int button) 
{
	if (button == 0) {
		activeVertex = this;
	} else if (button == 2 && !isDefiningTectureCoord && mtl2dMappingSettings::kIsManuallyAddingDeletingVertexEnabled) {
		toBeRemoved = true;
		activeVertex = NULL;
	}
}

//--------------------------------------------------------------
void mtl2dMappingVertex::setAsActive() 
{
    activeVertex = this;
}

//--------------------------------------------------------------
void mtl2dMappingVertex::onRelease(int x, int y, int button) 
{
	if(activeVertex == this) {
		activeVertex = NULL;
	}
}

//--------------------------------------------------------------
void mtl2dMappingVertex::onReleaseOutside(int x, int y, int button) 
{
	if(activeVertex == this) {
		activeVertex = NULL;
	}
}