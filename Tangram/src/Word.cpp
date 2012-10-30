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
    _bounds.set(0, 0, 0, 0);
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

    // Calculate the new bounds.
    if (_glyphs.size() > 0) {
        _bounds = _glyphs[0].absBounds();
        for (int i = 1; i < _glyphs.size(); i++) {
            _bounds.growToInclude(_glyphs[i].absBounds());
        }
    }
}

//--------------------------------------------------------------
void Word::draw()
{
    for (int i = 0; i < _glyphs.size(); i++) {
        _glyphs[i].draw();
    }
}

//--------------------------------------------------------------
void Word::debug()
{
    for (int i = 0; i < _glyphs.size(); i++) {
        _glyphs[i].debug();
    }

    ofNoFill();
    ofSetColor(0);
    ofRect(_bounds);
    ofFill();
}
