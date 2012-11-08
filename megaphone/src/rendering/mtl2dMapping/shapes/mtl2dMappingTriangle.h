//
//  mtl2dMappingTriangle.h
//  ofxMTL2DMapping
//
//  Created by Hugues Bruyere on 12-10-31.
//

#pragma once

#include "mtl2dMappingShape.h"

//--------------------------------------------------------------
//--------------------------------------------------------------
class mtl2dMappingTriangle : public mtl2dMappingShape {
	
    public:
        
        mtl2dMappingTriangle();
        ~mtl2dMappingTriangle();
        
    protected:
        virtual void createDefaultShape();
        virtual void render();

};
