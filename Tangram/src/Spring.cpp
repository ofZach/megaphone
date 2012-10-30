//
//  Spring.cpp
//  Tangram
//
//  Created by Elie Zananiri on 2012-10-29.
//
//

#include "spring.h"

//---------------------------------------------------------------------
Spring::Spring(Glyph *a, Glyph *b)
{
	_a = a;
    _b = b;
    _distance = MAX(40, _a->pos().distance(_b->pos()));
    _springiness = 0.52;
}

//---------------------------------------------------------------------
void Spring::update()
{
    ofVec2f delta = _a->pos() - _b->pos();
	float springForce = (_springiness * (_distance - delta.length()));
	ofVec2f frcToAdd = delta.normalized() * springForce;
	
	_a->addForce(frcToAdd);
	_b->addForce(frcToAdd * -1);
}

//---------------------------------------------------------------------
void Spring::debug()
{	
	ofLine(_a->pos(), _b->pos());
}