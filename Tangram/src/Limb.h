//
//  Limb.h
//  Tangram
//
//  Created by Elie Zananiri on 2012-10-26.
//
//

#pragma once

#include "ofMain.h"

//--------------------------------------------------------------
//--------------------------------------------------------------
class Limb
{
    public:
        Limb();

        void addCoord(ofVec3f coord);
        void clearCoords();
        const vector<ofVec3f>& coords();

        void setColor(ofColor color);
        const ofColor& color();

        bool flaps();
        void setFlaps(bool flaps);
        void setupFlapping(ofVec3f flapAxis);

        void update();
        void draw();

    private:
        vector<ofVec3f> _coords;
        ofColor _color;

        bool _bFlaps;
        float _flapRotation;
        float _flapSpeed;
        int _flapDirection;
        ofVec3f _flapAxis;
};
