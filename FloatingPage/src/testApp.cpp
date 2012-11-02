#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{
    ofSetFrameRate(60);
    ofSetVerticalSync(true);

    bShowWindows = false;

    camera.enableMouseInput();
    camera.tilt(-20);
    camera.rotate(10, 0, 1, 0);

    flatPage.pos.set(-70, 0, 70);
    flatPage.path.setFillColor(ofColor(200, 0, 0));

    flexPage.pos.set(70, 0, -70);
    flexPage.path.setFillColor(ofColor(200, 200, 0));
    flexPage.setMode(PageModeFlex);

    swayPage.pos.set(-70, 0, -70);
    swayPage.path.setFillColor(ofColor(0, 200, 200));
    swayPage.setMode(PageModeSway);

    vertPage.path.setFillColor(ofColor(0, 200, 0));
    vertPage.setMode(PageModeVert);

    flipPage.pos.set(70, 0, 70);
    flipPage.path.setFillColor(ofColor(200, 0, 200));
    flipPage.setMode(PageModeFlip);
}

//--------------------------------------------------------------
void testApp::addRainPages(int num)
{
    for (int i = 0; i < num; i++) {
        Page *page = new Page();
        page->pos.set(ofRandom(-groundSize, groundSize), 0, ofRandom(-groundSize, groundSize));
        page->path.setFillColor(ofColor(ofRandom(255)));
        page->setMode(ofRandom(NumPageModes));
        page->bHops = false;
        page->rainSpeed.set(ofRandom(-2, -5), -1 * ofRandom(2, 10), ofRandom(10));

        rainPages.push_back(page);
    }
}

//--------------------------------------------------------------
void testApp::update()
{
    flatPage.update();
    flexPage.update();
    swayPage.update();
    vertPage.update();
    flipPage.update();

    for (int i = 0; i < rainPages.size(); i++) {
        rainPages[i]->update();
    }
}

//--------------------------------------------------------------
void testApp::draw()
{
    camera.begin();

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    // draw the ground
    ofSetColor(255);
    ofBeginShape();
    ofVertex(-groundSize, 0, -groundSize);
    ofVertex(-groundSize, 0, groundSize);
    ofVertex(groundSize, 0, groundSize);
    ofVertex(groundSize, 0, -groundSize);
    ofEndShape(true);

    // draw the pages
    if (bShowRain) {
        for (int i = 0; i < rainPages.size(); i++) {
            rainPages[i]->draw();
        }
    }
    else {
        flatPage.draw();
        flexPage.draw();
        swayPage.draw();
        vertPage.draw();
        flipPage.draw();
    }

    // draw the 3d origin
    static int axisLength = 10;
    ofSetColor(255, 0, 0);
    ofLine(0, 0, 0, axisLength, 0, 0);
    ofSetColor(0, 255, 0);
    ofLine(0, 0, 0, 0, axisLength, 0);
    ofSetColor(0, 0, 255);
    ofLine(0, 0, 0, 0, 0, axisLength);

    if (bShowWindows) {
        // draw some windows on the front
        static int windowWidth = 30 * 2;
        static int windowHeight = 80 * 2;
        static int windowBase = 60;
        ofSetColor(0);
        ofBeginShape();
        ofVertex(groundSize, windowBase, -groundSize/2 - windowWidth/2);
        ofVertex(groundSize, windowBase + windowHeight, -groundSize/2 - windowWidth/2);
        ofVertex(groundSize, windowBase + windowHeight, -groundSize/2 + windowWidth/2);
        ofVertex(groundSize, windowBase, -groundSize/2 + windowWidth/2);
        ofEndShape(true);
        ofBeginShape();
        ofVertex(groundSize, windowBase, -windowWidth/2);
        ofVertex(groundSize, windowBase + windowHeight, -windowWidth/2);
        ofVertex(groundSize, windowBase + windowHeight, windowWidth/2);
        ofVertex(groundSize, windowBase, windowWidth/2);
        ofEndShape(true);
        ofBeginShape();
        ofVertex(groundSize, windowBase, groundSize/2 - windowWidth/2);
        ofVertex(groundSize, windowBase + windowHeight, groundSize/2 - windowWidth/2);
        ofVertex(groundSize, windowBase + windowHeight, groundSize/2 + windowWidth/2);
        ofVertex(groundSize, windowBase, groundSize/2 + windowWidth/2);
        ofEndShape(true);

        ofBeginShape();
        ofVertex(-groundSize/2 - windowWidth/2, windowBase, groundSize);
        ofVertex(-groundSize/2 - windowWidth/2, windowBase + windowHeight, groundSize);
        ofVertex(-groundSize/2 + windowWidth/2, windowBase + windowHeight, groundSize);
        ofVertex(-groundSize/2 + windowWidth/2, windowBase, groundSize);
        ofEndShape(true);
        ofBeginShape();
        ofVertex(-windowWidth/2, windowBase, groundSize);
        ofVertex(-windowWidth/2, windowBase + windowHeight, groundSize);
        ofVertex(windowWidth/2, windowBase + windowHeight, groundSize);
        ofVertex(windowWidth/2, windowBase, groundSize);
        ofEndShape(true);
        ofBeginShape();
        ofVertex(groundSize/2 - windowWidth/2, windowBase, groundSize);
        ofVertex(groundSize/2 - windowWidth/2, windowBase + windowHeight, groundSize);
        ofVertex(groundSize/2 + windowWidth/2, windowBase + windowHeight, groundSize);
        ofVertex(groundSize/2 + windowWidth/2, windowBase, groundSize);
        ofEndShape(true);
    }

    camera.end();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
    if (key == 'w') {
        bShowWindows ^= true;
    }
    else if (key == 'r') {
        bShowRain = true;
        addRainPages(20);
    }
    else if (key == ' ') {
        bShowRain = false;

        for (int i = 0; i < rainPages.size(); i++) {
            delete rainPages[i];
        }
        rainPages.clear();
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}