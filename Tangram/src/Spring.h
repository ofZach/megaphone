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
		Spring(Glyph *a, Glyph *b);

        void update();
        void debug();

        float distance() { return _distance; }
        float springiness() { return _springiness; }

    private:
		Glyph *_a;
        Glyph *_b;
		float  _distance;
		float  _springiness;
	
};
