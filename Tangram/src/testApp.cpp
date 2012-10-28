#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{
    ofSetVerticalSync(true);
    mode = 1;

    // Create an alphabet.
    for (int i = 0; i < 26; i++) {
        alphabet.push_back(Glyph());
    }
    alphaScale = 16;
    alphaPos.set(20, ofGetHeight() - 20 - alphaScale * 4);
    linePos.set(20, 20 + alphaScale * 4);
}

//--------------------------------------------------------------
void testApp::update()
{
    for (int i = 0; i < words.size(); i++) {
        words[i].update();
    }
}

//--------------------------------------------------------------
void testApp::draw()
{
    ofBackground(255);

    for (int i = 0; i < words.size(); i++) {
        words[i].draw();
    }

    // Draw some instructions.
    ofSetColor(0);
    ofDrawBitmapString("Mode [1] Click to create, drag to move.\nMode [2] Drag to spray.\nMode [3] Type to create, [RETURN] to push.\n[DELETE] to clear.", 20, 20);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
    if (key == '1') mode = 1;
    else if (key == '2') mode = 2;
    else if (key == '3') {
        mode = 3;

        // Add a new Word to start with.
        words.push_back(Word());
    }

    if (mode == 1 || mode == 2) {
        if (key == OF_KEY_BACKSPACE || OF_KEY_DEL) {
            // Remove all the Words.
            words.clear();
        }
    }
    else if (mode == 3) {
        if (key >= 'a' && key <= 'z') {
            int index = key - 'a';

            // Add a new Word if none exist yet.
            if (words.size() == 0) {
                words.push_back(Word());
            }

            // Add a new Glyph to the last Word.
            Glyph newGlyph = alphabet[index];
            newGlyph.moveTo(alphaPos);
            newGlyph.setScale(alphaScale);
            newGlyph.setAnimates(false);

            words.back().addGlyph(newGlyph);

            // Set the next Glyph position.
            alphaPos.x += alphaScale * 4;
        }
        else if (key == OF_KEY_BACKSPACE && words.size() > 0) {
            // If the last Word has no more Glyphs, remove the last Word.
            if (words.back().glyphs().size() == 0) {
                words.pop_back();
            }
            else {
                // Remove the last Glyph from the last Word.
                words.back().glyphs().pop_back();
            }

            // Set the next Glyph position.
            alphaPos.x -= alphaScale * 4;
        }
        else if (key == OF_KEY_DEL) {
            // Remove all the Words.
            words.clear();
            linePos.set(20, 20 + alphaScale * 4);
        }
        else if (key == OF_KEY_RETURN && words.size() > 0) {
            Word& lastWord = words.back();
            
            // Push the last Word on the page.
            lastWord.pushTo(linePos, 0.1);
            for (int i = 0; i < lastWord.glyphs().size(); i++) {
                lastWord.glyphs()[i].setScale(alphaScale);
                lastWord.glyphs()[i].setAnimates(true);
            }

            // Add a new Word.
            words.push_back(Word());
            alphaPos.set(20, ofGetHeight() - 20 - alphaScale * 4);

            // Set the next line position.
            linePos.y += alphaScale * 4;
        }
        else if (key == ' ') {
            // Add a space.
            alphaPos.x += alphaScale * 4;
        }
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key)
{

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y)
{

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{
    prevMousePos = mousePos;
    mousePos.set(x, y);

    if (mode == 1) {  // DRAG MODE
        if (words.size() > 0) {
            words.back().moveTo(mousePos);
        }
    }
    else if (mode == 2) {  // SPRAY MODE
        if (prevMousePos.distance(mousePos) > 10) {
            float targetScale = ofMap(mousePos.distance(prevMousePos), 0, MAX(ofGetWidth(), ofGetHeight()), 10, 500);
            float targetRotation = RAD_TO_DEG * (atan2(prevMousePos.y - mousePos.y, prevMousePos.x - mousePos.x));

            // Add a new Glyph to the last Word.
            Glyph glyph;
            glyph.moveTo(mousePos);
            glyph.setScale(targetScale);
            glyph.setRotation(targetRotation);
            glyph.addVelocity((mousePos - prevMousePos) * 0.5);
            words.back().addGlyph(glyph);
        }
    }
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{
    mousePos.set(x, y);

    if (mode == 1) {  // DRAG MODE
        // Add a new Word, holding a single Glyph.
        Word word;
        word.addGlyph(Glyph());
        words.push_back(word);
    }
    else if (mode == 2) {  // SPRAY MODE
        // Add a new Word.
        words.push_back(Word());
        
        mouseDragged(x, y, button);
    }
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{
    if (mode == 1) {  // DRAG MODE
        if (words.size() > 0) {
            words.back().glyphs().back().addVelocity((mousePos - prevMousePos) * 0.5);
        }
    }
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg)
{

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo)
{

}
