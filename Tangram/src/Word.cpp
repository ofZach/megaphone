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
Word::~Word()
{

    for (int i = 0; i < _glyphs.size(); i++) {
        delete _glyphs[i];
    }
    _glyphs.clear();
}

//--------------------------------------------------------------
void Word::addGlyph(Glyph *glyph)
{
    _glyphs.push_back(glyph);
}

//--------------------------------------------------------------
void Word::resetForces()
{
    for (int i = 0; i < _glyphs.size(); i++) {
        _glyphs[i]->resetForce();
    }
}

//--------------------------------------------------------------
void Word::addInternalForces()
{
    for (int i = 0; i < _glyphs.size(); i++) {
        _glyphs[i]->addRepulsionForce(ofVec2f(ofGetMouseX(), ofGetMouseY()), 200, 0.7f);

        for (int j = 0; j < i; j++) {
            _glyphs[i]->addRepulsionForce(_glyphs[j], 100, 0.1);
        }
    }

//    for (int i = 0; i < springs.size(); i++){
//        springs[i].update();
//    }
}

//--------------------------------------------------------------
void Word::repelFromWord(Word *otherWord)
{
    if (_bounds.intersects(otherWord->bounds())) {
        for (int i = 0; i < otherWord->glyphs().size(); i++) {
            for (int j = 0; j < _glyphs.size(); j++) {
                otherWord->glyphs()[i]->addRepulsionForce(_glyphs[j], 40, 10.4);
            }
        }
    }
}

////--------------------------------------------------------------
//void Word::moveTo(ofVec3f position)
//{
//    if (_glyphs.size() == 0) return;
//
//    ofVec3f offset = position - _glyphs[0].position();
//    
//    for (int i = 0; i < _glyphs.size(); i++) {
//        _glyphs[i].moveBy(offset);
//    }
//}

////--------------------------------------------------------------
//void Word::pushTo(ofVec3f position, float scalar)
//{
//    if (_glyphs.size() == 0) return;
//
//    ofVec3f offset = position - _glyphs[0].position();
//
//    for (int i = 0; i < _glyphs.size(); i++) {
//        _glyphs[i].addVelocity(offset * scalar);
//    }
//}

//--------------------------------------------------------------
void Word::update()
{
    for (int i = 0; i < _glyphs.size(); i++) {
        _glyphs[i]->bounceOffWalls();
        _glyphs[i]->addDampingForce();
        _glyphs[i]->update();
    }

    // Calculate the new bounds.
    if (_glyphs.size() > 0) {
        _bounds = _glyphs[0]->absBounds();
        for (int i = 1; i < _glyphs.size(); i++) {
            _bounds.growToInclude(_glyphs[i]->absBounds());
        }
    }
}

//--------------------------------------------------------------
void Word::draw()
{
    for (int i = 0; i < _glyphs.size(); i++) {
        _glyphs[i]->draw();
    }
}

//--------------------------------------------------------------
void Word::debug()
{
    for (int i = 0; i < _glyphs.size(); i++) {
        _glyphs[i]->debug();
    }

    ofNoFill();
    ofSetColor(0);
    ofRect(_bounds);
    ofFill();
}
