#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    
    keyPressed('3');
    bDebug = false;

    // Create an alphabet.
    for (int i = 0; i < 26; i++) {
        alphabet.push_back(Glyph());
    }
    alphaScale = 8;

    VF.setupField(60, 40, ofGetWidth(), ofGetHeight());
}

//--------------------------------------------------------------
void testApp::clear()
{
    for (int i = 0; i < words.size(); i++) {
        delete words[i];
    }
    words.clear();
}

//--------------------------------------------------------------
void testApp::update()
{
    for (int i = 0; i < words.size(); i++) {
        words[i]->addInternalForces();
    }

    for (int i = 0; i < words.size(); i++) {
        for (int j = 0; j < i; j++) {
            words[i]->repelFromWord(words[j]);
        }
    }

    for (int i = 0; i < words.size(); i++) {
        for (int j = 0; j < words[i]->glyphs().size(); j++) {
            Glyph *glyph = words[i]->glyphs()[j];
            ofVec2f frc = VF.getForceFromPos(glyph->pos().x, glyph->pos().y);
            glyph->addForce(frc);
        }
	}

    for (int i = 0; i < words.size(); i++) {
        words[i]->update();
    }

    float t = ofGetElapsedTimef();
    crawlerInward.x = ofNoise(t * 0.50) * ofGetWidth();
    crawlerInward.y = ofNoise(t * 0.42) * ofGetHeight();
    VF.addInwardCircle(crawlerInward.x, crawlerInward.y, 600, 0.04f);
    crawlerClockwise.x = ofNoise(t * 1.00) * ofGetWidth();
    crawlerClockwise.y = ofNoise(t * 0.87) * ofGetWidth();
    VF.addClockwiseCircle(crawlerClockwise.x, crawlerClockwise.y, 260, 0.08f);

    for (int i = 0; i < words.size(); i++) {
        words[i]->resetForces();
    }

    VF.fadeField(0.99);
}

//--------------------------------------------------------------
void testApp::draw()
{
    ofBackground(255);

    if (bDebug) {
        VF.draw();
        ofCircle(crawlerInward, 10);
        ofCircle(crawlerClockwise, 10);

        for (int i = 0; i < words.size(); i++) {
            words[i]->debug();
        }
    }
    else {
        for (int i = 0; i < words.size(); i++) {
            words[i]->draw();
        }

        // Draw some instructions.
        ofSetColor(0);
        ofDrawBitmapString("Mode [1] Click to create, drag to move.\nMode [2] Drag to spray.\nMode [3] Type to create, [RETURN] to push.\nMode [4] Type to create, [RETURN] to push.\n[DELETE] to clear.", 20, 20);
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
    if (key == 9) bDebug ^= true;

    else if (key == '1') mode = 1;
    else if (key == '2') mode = 2;
    else if (key == '3') {
        mode = 3;

        alphaPos.set(20, ofGetHeight() - 20 - alphaScale * 4);
        linePos.set(ofGetWidth() - 20 - alphaScale * 4, 20 + alphaScale * 4);
    }

    if (mode == 1 || mode == 2) {
        if (key == OF_KEY_BACKSPACE || OF_KEY_DEL) {
            // Remove all the Words.
            clear();
        }
    }
    else if (mode == 3) {
        if (key >= 'a' && key <= 'z') {
            int index = key - 'a';

            // Add a new Word if none exist yet.
            if (words.size() == 0) {
                words.push_back(new Word());
            }

            Word *lastWord = words.back();

            // Push the older Glyphs over to make room.
            for (int i = 0; i < lastWord->glyphs().size(); i++) {
                lastWord->glyphs()[i]->addForce(ofVec2f(lastWord->glyphs().size() - i, 0));
            }

            // Add a new Glyph to the last Word.
            Glyph *newGlyph = new Glyph(alphabet[index]);
            newGlyph->setInitialCondition(alphaPos, ofVec2f());
            newGlyph->setScale(alphaScale);
            newGlyph->setAnimates(false);

            lastWord->addGlyph(newGlyph);
        }
        else if (key == OF_KEY_BACKSPACE && words.size() > 0) {
            Word *lastWord = words.back();

            // If the last Word has no more Glyphs, remove the last Word.
            if (lastWord->glyphs().size() == 0) {
                delete lastWord;
                words.pop_back();
            }
            else {
                // Remove the last Glyph from the last Word.
                lastWord->removeLastGlyph();

                if (lastWord->glyphs().size() > 0) {
                    // Pull back the last Glyph.
                    Glyph *lastGlyph = lastWord->glyphs().back();
                    lastGlyph->addForce(ofVec3f(ofRandom(-4, -6), ofRandom(-1, 1)));
                }
            }
        }
        else if (key == OF_KEY_DEL) {
            // Remove all the Words.
            clear();
            linePos.set(20, 20 + alphaScale * 4);
        }
        else if (key == OF_KEY_RETURN && words.size() > 0) {
            Word *lastWord = words.back();

            if (lastWord->glyphs().size() > 0) {
                // Push the Glyphs up.
                for (int i = 0; i < lastWord->glyphs().size(); i++) {
                    lastWord->glyphs()[i]->addForce(ofVec2f(0, (ofGetHeight() - linePos.y) * 0.5));
                }

                // Add a new Word.
                words.push_back(new Word());

                // Set the next line position.
                linePos.y += alphaScale * 4;
            }
        }
        else if (key == ' ' && words.size() > 0) {
            Word *lastWord = words.back();

            // Push the older Glyphs over to make room.
            for (int i = 0; i < lastWord->glyphs().size(); i++) {
                lastWord->glyphs()[i]->addForce(ofVec2f(lastWord->glyphs().size() - i, 0));
            }
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

    VF.addInwardCircle(x, y, 600, 0.1f);
    VF.addClockwiseCircle(x, y, 260, 0.3f);

    if (mode == 1) {  // DRAG MODE
        if (words.size() > 0) {
//            words.back().moveTo(mousePos);
        }
    }
    else if (mode == 2) {  // SPRAY MODE
        if (prevMousePos.distance(mousePos) > 10) {
            float targetScale = ofMap(mousePos.distance(prevMousePos), 0, MAX(ofGetWidth(), ofGetHeight()), 10, 500);
            float targetRotation = RAD_TO_DEG * (atan2(prevMousePos.y - mousePos.y, prevMousePos.x - mousePos.x));

            // Add a new Glyph to the last Word.
            Glyph *glyph = new Glyph();
            glyph->setInitialCondition(mousePos, ofVec2f());
            glyph->setScale(targetScale);
            glyph->setRotation(targetRotation);
            glyph->addForce((mousePos - prevMousePos) * 0.5);
            words.back()->addGlyph(glyph);
        }
    }
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{
    mousePos.set(x, y);

    if (mode == 1) {  // DRAG MODE
        // Add a new Word, holding a single Glyph.
        Word *word = new Word();
        word->addGlyph(new Glyph());
        words.push_back(word);
    }
    else if (mode == 2) {  // SPRAY MODE
        // Add a new Word.
        words.push_back(new Word());
        
        mouseDragged(x, y, button);
    }
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{
    if (mode == 1) {  // DRAG MODE
        if (words.size() > 0) {
            words.back()->glyphs().back()->addForce((mousePos - prevMousePos) * 0.5);
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
