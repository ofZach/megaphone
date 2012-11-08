//
//  mtl2dMappingShape.cpp
//  ofxMTL2DMapping
//
//  Created by Hugues Bruyere on 12-06-06.
//

#include "mtl2dMappingShape.h"
#include "ControlsMapping.h"

mtl2dMappingShape*  mtl2dMappingShape::activeShape         = NULL;
mtl2dMappingShape*  mtl2dMappingShape::previousActiveShape = NULL;
int                 mtl2dMappingShape::activeShapeCurrVertexId = -1;

//--------------------------------------------------------------
//--------------------------------------------------------------
void mtl2dMappingShape::resetActiveShapeVars(){
    delete mtl2dMappingShape::activeShape;
    mtl2dMappingShape::activeShape              = NULL;
    delete mtl2dMappingShape::previousActiveShape;
    mtl2dMappingShape::previousActiveShape      = NULL;
    mtl2dMappingShape::activeShapeCurrVertexId  = -1;
    delete mtl2dMappingVertex::activeVertex;
    mtl2dMappingVertex::activeVertex            = NULL;
}



//--------------------------------------------------------------
//--------------------------------------------------------------
mtl2dMappingShape::mtl2dMappingShape()
{
}

//--------------------------------------------------------------
mtl2dMappingShape::~mtl2dMappingShape(){
}


#pragma mark -
#pragma mark Lifecycle
//--------------------------------------------------------------
void mtl2dMappingShape::init(int sId, bool defaultShape)
{
    _super::init(sId, defaultShape);
    
    calcHomography();
}

//--------------------------------------------------------------
void mtl2dMappingShape::destroy()
{
    _super::destroy();
    
    // ----
    delete inputPolygon;
}

//--------------------------------------------------------------
void mtl2dMappingShape::update()
{
    _super::update();
    
    
    if(activePolygon != this && activePolygon != inputPolygon)
        return;
    
    if(activePolygon == this || activePolygon == inputPolygon) {
        setAsActiveShape();
    }
    
    if (activeShape == this) {
        if (ControlsMapping::controlsMapping()->mappingMode() == MAPPING_MODE_INPUT) {
            inputPolygon->update();
        }
        
        // ---- recalculate the homography transformation matrix.
        calcHomography();
	}
}


//--------------------------------------------------------------
void mtl2dMappingShape::draw()
{		
    //--------
    ofEnableAlphaBlending();
        
    // ---- OUTPUT MODE
    if(ControlsMapping::controlsMapping()->mappingMode() == MAPPING_MODE_OUTPUT) {
        _super::draw();

    // ---- INPUT MODE
    } else if (ControlsMapping::controlsMapping()->mappingMode() == MAPPING_MODE_INPUT) {
        inputPolygon->draw();
    }
    
	ofDisableAlphaBlending();	
}


//--------------------------------------------------------------
void mtl2dMappingShape::drawID() 
{
    // ---- OUTPUT MODE
    if(ControlsMapping::controlsMapping()->mappingMode() == MAPPING_MODE_OUTPUT) {        
        _super::drawID();
    
    // ---- INPUT MODE
    } else if (ControlsMapping::controlsMapping()->mappingMode() == MAPPING_MODE_INPUT) {
        inputPolygon->drawID();
    }
}

//--------------------------------------------------------------
void mtl2dMappingShape::setAsActiveShape()
{
	if (activeShape != this) {
        previousActiveShape = activeShape;
        activeShape = this;
        activeShapeCurrVertexId = -1;
    }
}
