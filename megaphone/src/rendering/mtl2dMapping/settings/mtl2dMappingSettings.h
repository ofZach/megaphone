//
//  mtl2dMappingSettings.h
//  ofxMTL2DMapping
//
//  Created by Hugues Bruyere on 12-06-04.
//


#pragma once

#include "ofMain.h"

//--------------------------------------------------------------
class mtl2dMappingSettings {
    public:
       
        // ---- available / activated options
        static const bool kIsManuallyCreatingShapeEnabled;   
        static const bool kIsManuallyAddingDeletingVertexEnabled;
    
        // ----
        static ofTrueTypeFont infoFont;
};