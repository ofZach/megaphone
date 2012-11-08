//
//  mtl2dMappingPolygon.h
//  ofxMTL2DMapping
//
//  Created by Hugues Bruyere on 12-06-06.
//

#pragma once

//OF
#include "ofMain.h"

#include "mtl2dMappingSettings.h"
#include "mtlUtils.h"
#include "mtl2dMappingVertex.h"

//MSA Libs
#include "ofxMSAInteractiveObject.h"

//--------------------------------------------------------------
//--------------------------------------------------------------
class mtl2dMappingPolygon : public ofxMSAInteractiveObject {
	
    public:
    
        // ----
        static mtl2dMappingPolygon* activePolygon;
        static mtl2dMappingPolygon* previousActivePolygon;
        static void resetActivePolygonVars();
    
        // ----
        mtl2dMappingPolygon();
        ~mtl2dMappingPolygon();
    
        int shapeId;
        list<mtl2dMappingVertex*> vertices;
        ofPolyline *polyline;
        ofRectangle boundingBox;

        void init(int sId, bool defaultShape = false);
        
        void update();
        void draw();
        void addPoint(int x, int y);
        void drawID();
        virtual void render() {};
    
        // ---- ofxMSAInteractiveObject related
        void enable();
        void disable();
    
        virtual bool hitTest(int tx, int ty);

        virtual void onRollOver(int x, int y);
        virtual void onRollOut();
        virtual void onMouseMove(int x, int y);
        virtual void onDragOver(int x, int y, int button);
        virtual void onDragOutside(int x, int y, int button);
        virtual void onPress(int x, int y, int button);
        virtual void onRelease(int x, int y, int button);
        virtual void onReleaseOutside(int x, int y, int button);
 
    protected:
        ofPoint _centroid2D;
        ofPoint _grabAnchor;

        void destroy();
        void setAsActive();
        void disableVertices();
        void enableVertices();

        void updatePosition(int xInc, int yInc);
        void updatePolyline();

        virtual void createDefaultShape() {};
};
