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

    closeUpCameraMatrix.setTranslation(-15.0981, 259.202, 99.0417);
    closeUpCameraMatrix.setRotate(ofQuaternion(-0.240849, -0.0507347, -0.00102867, 0.969235));

    longShotCameraMatrix.setTranslation(197.004, 332.554, 541.263);
    longShotCameraMatrix.setRotate(ofQuaternion(-0.254887, 0.167731, 0.0449435, 0.951251));

    targetCameraMatrix = longShotCameraMatrix;

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
    // tween the camera to its target position
//    ofMatrix4x4 currCameraMatrix = camera.getLocalTransformMatrix();
//    ofVec3f currTranslation = currCameraMatrix.getTranslation();
//    ofVec3f targetTranslation = targetCameraMatrix.getTranslation();
//    ofVec3f offsetTranslation = targetTranslation - currTranslation;
//    currTranslation += (offsetTranslation * 0.2f);
//    ofVec4f currRotation = currCameraMatrix.getRotate().asVec4();
//    ofVec4f targetRotation = targetCameraMatrix.getRotate().asVec4();
//    ofVec4f offsetRotation = targetRotation - currRotation;
//    currRotation += (offsetRotation * 0.2f);
//    currCameraMatrix.setTranslation(currTranslation);
//    currCameraMatrix.setRotate(ofQuaternion(currRotation));
//    camera.setTransformMatrix(currCameraMatrix);


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

    else if (key >= '1' && key <= '6') {
        int pageMode = key - 48;
        for (int i = 0; i < rainPages.size(); i++) {
            rainPages[i]->begin(pageMode);
        }
        bShowAll = false;
        targetCameraMatrix = closeUpCameraMatrix;
    }

    else if (key == 'q') {
        for (int i = 0; i < rainPages.size(); i++) {
            rainPages[i]->begin(ofRandom(NumPageModes));
            rainPages[i]->bRains = true;
        }
        bShowAll = true;
        targetCameraMatrix = longShotCameraMatrix;
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
    else if (key == 'p') {
        cout << "Camera " << endl
             << "  trans = " << camera.getLocalTransformMatrix().getTranslation() << endl
             << "  rotat = " << camera.getLocalTransformMatrix().getRotate().asVec4() << endl
             << "  scale = " << camera.getLocalTransformMatrix().getScale() << endl;
        cout << "  dist  = " << camera.getDistance() << endl;
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