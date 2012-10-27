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

class Glyph
{
public:
    Glyph();
    void addLimb(Limb limb);
    void clearLimbs();
    const vector<Limb>& limbs();

    void move(int x, int y);
    void update();
    void draw();

    void setScale(float scale);

private:

    float _x, _y;
    float _px, _py;
    float _scale, _targetScale;
    float _rotation, _targetRotation;

    vector<Limb> _limbs;
};
