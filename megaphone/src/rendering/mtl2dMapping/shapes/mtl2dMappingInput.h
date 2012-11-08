//
//  mtl2dMappingInput.h
//  ofxMTL2DMapping
//
//  Created by Hugues Bruyere on 12-06-05.
//

#pragma once

#include "ofMain.h"
#include "mtl2dMappingVertex.h"
#include "mtl2dMappingPolygon.h"


//--------------------------------------------------------------
//--------------------------------------------------------------
class mtl2dMappingInput : public mtl2dMappingPolygon {
	
    public:
        mtl2dMappingInput();
        ~mtl2dMappingInput();
    
        void calcHomography(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3);

        GLfloat homoMatrix[16];
};
