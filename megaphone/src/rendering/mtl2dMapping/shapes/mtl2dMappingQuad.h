//
//  mtl2dMappingQuad.h
//  ofxMTL2DMapping
//
//  Created by Hugues Bruyere on 12-10-31.
//

#pragma once

#include "mtl2dMappingShape.h"


//--------------------------------------------------------------
//--------------------------------------------------------------
class mtl2dMappingQuad : public mtl2dMappingShape {
	
    public:
        
        mtl2dMappingQuad();
        ~mtl2dMappingQuad();
        
    protected:
        virtual void createDefaultShape();
        virtual void render();
        virtual void calcHomography();
};
