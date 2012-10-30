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
#include "Spring.h"

//--------------------------------------------------------------
//--------------------------------------------------------------
class Word
{
    public:
        Word();
        ~Word();

        void addGlyph(Glyph *glyph);
        vector<Glyph *>& glyphs() { return _glyphs; }

        void resetForces();
        void addInternalForces();

        void repelFromWord(Word *otherWord);

//        void moveTo(ofVec3f position);
//        void pushTo(ofVec3f position, float scalar);
        void update();
        void draw();
        void debug();

        const ofRectangle& bounds() { return _bounds; }

    private:
        vector<Glyph *>     _glyphs;
        vector<Spring *>    _springs;
        ofRectangle         _bounds;
};
