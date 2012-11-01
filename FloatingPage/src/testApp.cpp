#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{
    ofSetFrameRate(60);
    ofSetVerticalSync(true);

    camera.enableMouseInput();
    camera.tilt(-20);
    camera.rotate(10, 0, 1, 0);

    flatPage.pos.set(-70, 0, 70);
    flatPage.path.setFillColor(ofColor(200, 0, 0));

    curvePage.pos.set(70, 0, 70);
    curvePage.path.setFillColor(ofColor(0, 0, 200));

    otherPage.pos.set(70, 0, -70);
    otherPage.path.setFillColor(ofColor(200, 200, 0));

    fallingPage.pos.set(-70, 0, -70);
    fallingPage.path.setFillColor(ofColor(0, 200, 200));

    a5Page.path.setFillColor(ofColor(0, 200, 0));
}

//--------------------------------------------------------------
void testApp::update()
{
    flatPage.update();
    curvePage.update();
    otherPage.update();
    fallingPage.update();
    a5Page.update();
}

//--------------------------------------------------------------
void testApp::draw()
{
    camera.begin();

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    // draw the ground
    static int groundSize = 200;
    ofSetColor(255);
    ofBeginShape();
    ofVertex(-groundSize, 0, -groundSize);
    ofVertex(-groundSize, 0, groundSize);
    ofVertex(groundSize, 0, groundSize);
    ofVertex(groundSize, 0, -groundSize);
    ofEndShape(true);

    // draw the pages
    flatPage.draw();
    curvePage.draw();
    otherPage.draw();
    fallingPage.draw();
    a5Page.draw();

    // draw the 3d origin
    static int axisLength = 10;
    ofSetColor(255, 0, 0);
    ofLine(0, 0, 0, axisLength, 0, 0);
    ofSetColor(0, 255, 0);
    ofLine(0, 0, 0, 0, axisLength, 0);
    ofSetColor(0, 0, 255);
    ofLine(0, 0, 0, 0, 0, axisLength);

    camera.end();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

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