//
//  Word.cpp
//  Tangram
//
//  Created by Elie Zananiri on 2012-10-28.
//
//

#include "Word.h"

//--------------------------------------------------------------
Word::Word()
{
    
}

//--------------------------------------------------------------
void Word::addGlyph(Glyph glyph)
{
    _glyphs.push_back(glyph);
}

//--------------------------------------------------------------
{
}

//--------------------------------------------------------------
void Word::moveTo(ofVec3f position)
{
    if (_glyphs.size() == 0) return;

    ofVec3f offset = position - _glyphs[0].position();
    
    for (int i = 0; i < _glyphs.size(); i++) {
        _glyphs[i].moveBy(offset);
    }
}

//--------------------------------------------------------------
void Word::pushTo(ofVec3f position, float scalar)
{
    if (_glyphs.size() == 0) return;

    ofVec3f offset = position - _glyphs[0].position();

    for (int i = 0; i < _glyphs.size(); i++) {
        _glyphs[i].addVelocity(offset * scalar);
    }
}

//--------------------------------------------------------------
void Word::update()
{
    for (int i = 0; i < _glyphs.size(); i++) {
        _glyphs[i].update();
    }
}

//--------------------------------------------------------------
void Word::draw()
{
    for (int i = 0; i < _glyphs.size(); i++) {
        _glyphs[i].draw();
    }
}
