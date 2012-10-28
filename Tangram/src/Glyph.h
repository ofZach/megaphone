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

//--------------------------------------------------------------
//--------------------------------------------------------------
class Glyph
{
    public:
        Glyph();
        void addLimb(Limb limb);
        void clearLimbs();
        const vector<Limb>& limbs();

        void moveTo(int x, int y);
        void update();
        void draw();

        bool animates();
        void setAnimates(bool animates);

        void setScale(float scale);
        void setRotation(float rotation);
        void setVelocity(ofVec3f velocity);

    private:
        ofVec3f _position, _prevPosition;
        ofVec3f _velocity;
        float _scale, _targetScale;
        float _rotation, _targetRotation;

        bool _bAnimates;

        vector<Limb> _limbs;
};
