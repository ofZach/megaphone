#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{
    ofSetVerticalSync(true);
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
}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
    if (key == ' ') glyphs.clear();
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
    if (glyphs.size() > 0) {
        glyphs[glyphs.size() - 1].move(x, y);
    }
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{
    // Add a new Glyph.
    glyphs.push_back(Glyph());
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{

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
