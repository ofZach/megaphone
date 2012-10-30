//
//  Spring.h
//  Tangram
//
//  Created by Elie Zananiri on 2012-10-29.
//
//

#pragma once

#include "ofMain.h"

#include "Glyph.h"

//--------------------------------------------------------------
//--------------------------------------------------------------
class Spring
{
	public:
		Spring();
		
		Glyph * glyphA;
		Glyph * glyphB;
		
		float distance;
		float springiness;	 // this is the k, springiness constant
	
		void update();
		void debug();
};
