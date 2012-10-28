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
        vector<Limb>& limbs();

        const ofVec3f& position();
        void setPosition(ofVec3f position);
        void moveTo(ofVec3f position);
        void moveBy(ofVec3f offset);

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
