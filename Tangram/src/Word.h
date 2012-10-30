//
//  Word.h
//  Tangram
//
//  Created by Elie Zananiri on 2012-10-28.
//
//

#pragma once

#include "ofMain.h"
#include "Glyph.h"

//--------------------------------------------------------------
//--------------------------------------------------------------
class Word
{
    public:
        Word();
        void addGlyph(Glyph glyph);
        vector<Glyph>& glyphs() { return _glyphs; }

        void moveTo(ofVec3f position);
        void pushTo(ofVec3f position, float scalar);
        void update();
        void draw();

    private:
        vector<Glyph> _glyphs;
};
