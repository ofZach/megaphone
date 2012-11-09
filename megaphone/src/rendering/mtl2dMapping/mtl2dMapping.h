//
//  mtl2dMapping.h
//  ofxMTL2DMapping
//
//  Created by Hugues Bruyere on 12-06-06.
//

#pragma once

// OF
#include "ofMain.h"

// Addons
#include "ofxXmlSettings.h"

//Mapping
#include "mtl2dMappingVertex.h"
#include "mtl2dMappingQuad.h"
#include "mtl2dMappingTriangle.h"
#include "mtl2dMappingShape.h"

#include "mtlUtils.h"


//========================================================================
class mtl2dMapping {
    
    public:
        
        mtl2dMapping();
        virtual ~mtl2dMapping();
        
        void init(int width, int height);
        void update();
        
        void bind();
        void unbind();
        void drawFbo();
        void draw();
    
    
        void drawOutput();
    
    

        void mousePressed(int x, int y, int button);
        void keyPressed(int key);
    
        void chessBoard(int nbOfCol = 10);

    //private:
        ofFbo _fbo;
        ofxXmlSettings _shapesListXML;
        list<mtl2dMappingShape*> _pmShapes;

        void render();
    
        void createQuad(float _x, float _y);
        void createTriangle(float _x, float _y);
        void deleteShape();
    
        void loadShapesList(string _xmlPath);
        void saveShapesList(string _xmlPath);
};