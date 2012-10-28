#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{
    ofSetVerticalSync(true);
    mode = 1;
}

//--------------------------------------------------------------
void testApp::update()
{
    for (int i = 0; i < glyphs.size(); i++) {
        glyphs[i].update();
    }
}

//--------------------------------------------------------------
void testApp::draw()
{
    ofBackground(255);

    for (int i = 0; i < glyphs.size(); i++) {
        glyphs[i].draw();
    }

    // Draw some instructions.
    ofSetColor(0);
    ofDrawBitmapString("Mode [1] Click to create, drag to move.\nMode [2] Drag to spray.\n[SPACEBAR] to clear.", 20, 20);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
    if (key == ' ') glyphs.clear();
    else if (key == '1') mode = 1;
    else if (key == '2') mode = 2;
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
        if (glyphs.size() > 0) {
            glyphs.back().moveTo(mousePos);
        }
    }
    else if (mode == 2) {  // SPRAY MODE
        if (prevMousePos.distance(mousePos) > 10) {
            float targetScale = ofMap(mousePos.distance(prevMousePos), 0, MAX(ofGetWidth(), ofGetHeight()), 10, 500);
            float targetRotation = RAD_TO_DEG * (atan2(prevMousePos.y - mousePos.y, prevMousePos.x - mousePos.x));

            // Add a new Glyph.
            glyphs.push_back(Glyph());
            glyphs.back().moveTo(mousePos);
            glyphs.back().setScale(targetScale);
            glyphs.back().setRotation(targetRotation);
            glyphs.back().setVelocity((mousePos - prevMousePos) * 0.5);
        }
    }
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{
    mousePos.set(x, y);

    if (mode == 1) {  // DRAG MODE
        // Add a new Glyph.
        glyphs.push_back(Glyph());
    }
    else if (mode == 2) {  // SPRAY MODE
        mouseDragged(x, y, button);
    }
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{
    if (mode == 1) {  // DRAG MODE
        if (glyphs.size() > 0) {
            glyphs.back().setVelocity((mousePos - prevMousePos) * 0.5);
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
