//
//  Glyph.h
//  Tangram
//
//  Created by Elie Zananiri on 2012-10-26.
//
//

#pragma once

#include "ofMain.h"
#include "Limb.h"
#include "glyphShapes.h"

//--------------------------------------------------------------
//--------------------------------------------------------------
class Glyph
{
    public:
    Glyph(){}
    ~Glyph();

    
    
        void setup();
        void addLimb(Limb limb);
        void clearLimbs();
        vector<Limb>& limbs();

        void update();
        void draw();
        void draw(ofPoint from);
        void debug();
    
        void normalize();
        

        ofVec2f& pos() { return _pos; }

        bool animates();
        void setAnimates(bool animates);

        float scale() { return _scale; }
        void setScale(float scale);

        float rotation() { return _rotation; }
        void setRotation(float rotation);

        const ofRectangle& bounds() { return _bounds; }
        
        glyphShapes * GS;
    

        ofVec2f _pos;

        
        float origScale;
    
        

        float   _scale, _targetScale;
        float   _rotation, _targetRotation;

        bool    _bAnimates;

        ofRectangle  _bounds;
        vector<Limb> _limbs;
};
