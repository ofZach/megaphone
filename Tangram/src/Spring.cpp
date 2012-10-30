//
//  Spring.cpp
//  Tangram
//
//  Created by Elie Zananiri on 2012-10-29.
//
//

#include "spring.h"

//---------------------------------------------------------------------
Spring::Spring()
{
	glyphA = NULL;
	glyphB = NULL;
}

//---------------------------------------------------------------------
void Spring::update()
{
	if ((glyphA == NULL) || (glyphB == NULL)) {
		return;
	}
	
	ofVec2f pta = glyphA->pos();
	ofVec2f ptb = glyphB->pos();
	
	float theirDistance = (pta - ptb).length();
	float springForce = (springiness * (distance - theirDistance));
	ofVec2f frcToAdd = (pta-ptb).normalized() * springForce;
	
	glyphA->addForce(frcToAdd);
	glyphB->addForce(frcToAdd * -1);
}

//---------------------------------------------------------------------
void Spring::debug()
{	
	if ((glyphA == NULL) || (glyphB == NULL)) {
		return;
	}
	
	ofLine(glyphA->pos(), glyphB->pos());
}