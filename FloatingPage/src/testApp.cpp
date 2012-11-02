#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    ofBackground(0);

    bShowRain = true;
    bShowAll = true;

    camera.enableMouseInput();
    camera.tilt(-30);
    camera.rotate(20, 0, 1, 0);
    closeUpCameraMatrix = camera.getLocalTransformMatrix();
    longShotCameraMatrix = camera.getLocalTransformMatrix();

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

    addRainPages(1);
    rainPages.back()->begin(PageModeAll);
}

//--------------------------------------------------------------
void testApp::addRainPages(int num)
{
    for (int i = 0; i < num; i++) {
        Page *page = new Page();
        page->path.setFillColor(ofColor(255));
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
    ofSetColor(128);
    ofBeginShape();
    ofVertex(-groundSize, 0, -groundSize);
    ofVertex(-groundSize, 0, groundSize);
    ofVertex(groundSize, 0, groundSize);
    ofVertex(groundSize, 0, -groundSize);
    ofEndShape(true);

    // draw the pages
    if (bShowRain) {
        if (bShowAll) {
            for (int i = 0; i < rainPages.size(); i++) {
                rainPages[i]->draw();
            }
        }
        else if (rainPages.size() > 0) {
            rainPages[0]->draw();
        }
    }
    else {
//        flatPage.draw();
//        flexPage.draw();
//        swayPage.draw();
//        vertPage.draw();
//        flipPage.draw();
    }

//    // draw the 3d origin
//    static int axisLength = 10;
//    ofSetColor(255, 0, 0);
//    ofLine(0, 0, 0, axisLength, 0, 0);
//    ofSetColor(0, 255, 0);
//    ofLine(0, 0, 0, 0, axisLength, 0);
//    ofSetColor(0, 0, 255);
//    ofLine(0, 0, 0, 0, 0, axisLength);

    camera.end();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
    if (key == 'r') {
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
    
    else if (key == '1') {
        for (int i = 0; i < rainPages.size(); i++) {
            rainPages[i]->begin(PageModeFlat);
        }
        bShowAll = false;
    }
    else if (key == '2') {
        for (int i = 0; i < rainPages.size(); i++) {
            rainPages[i]->begin(PageModeFlex);
        }
        bShowAll = false;
    }
    else if (key == '3') {
        for (int i = 0; i < rainPages.size(); i++) {
            rainPages[i]->begin(PageModeFlip);
        }
    }
    else if (key == '4') {
        for (int i = 0; i < rainPages.size(); i++) {
            rainPages[i]->begin(PageModeVert);
        }
    }
    else if (key == '5') {
        for (int i = 0; i < rainPages.size(); i++) {
            rainPages[i]->begin(PageModeSway);
        }
    }
    else if (key == '6') {
        for (int i = 0; i < rainPages.size(); i++) {
            rainPages[i]->begin(PageModeAll);
        }
    }

    else if (key == 'q') {
        for (int i = 0; i < rainPages.size(); i++) {
            rainPages[i]->begin(ofRandom(NumPageModes));
            rainPages[i]->bRains = true;
        }
        bShowAll = true;
    }
    else if (key == 'w') {
        for (int i = 0; i < rainPages.size(); i++) {
            rainPages[i]->end();
            rainPages[i]->bRains = false;
        }
    }

    else if (key == 'z') {
        longShotCameraMatrix = camera.getLocalTransformMatrix();
    }
    else if (key == 'x') {
        closeUpCameraMatrix = camera.getLocalTransformMatrix();
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