//
//  mtl2dMappingShape.h
//  ofxMTL2DMapping
//
//  Created by Hugues Bruyere on 12-06-06.
//

#pragma once

//OF
#include "ofMain.h"

#include "mtl2dMappingPolygon.h"
#include "mtl2dMappingSettings.h"
#include "mtlUtils.h"
#include "mtl2dMappingInput.h"
#include "mtl2dMappingVertex.h"

//--------------------------------------------------------------
//--------------------------------------------------------------
class mtl2dMappingShape : public mtl2dMappingPolygon {
	
    public:
        // ----
        static mtl2dMappingShape* activeShape;
        static mtl2dMappingShape* previousActiveShape;
        static int activeShapeCurrVertexId;
        static void resetActiveShapeVars();
    
        // ----
        mtl2dMappingShape();
        ~mtl2dMappingShape();
    
        map<string,string> shapeSettings;
        mtl2dMappingInput* inputPolygon;
    
        void init(int sId, bool defaultShape = false);

        void update();
        void draw();
        void drawID();
        virtual void render() {};
    

    protected:
        void destroy();
        void setAsActiveShape();

        virtual void calcHomography() {};
        virtual void createDefaultShape() {};
    
    private:
        typedef mtl2dMappingPolygon _super;

};
