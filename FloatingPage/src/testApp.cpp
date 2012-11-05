#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    ofBackground(0);

    bShowAll = true;

    camera.disableMouseInput();
    camera.tilt(-30);
    camera.rotate(20, 0, 1, 0);

    closeUpCameraMatrix.setTranslation(40.3154, 68.0548, 110.766);
    closeUpCameraMatrix.setRotate(ofQuaternion(-0.254887, 0.167731, 0.0449435, 0.951251));

    longShotCameraMatrix.setTranslation(242.633, 457.804, 637.342);
    longShotCameraMatrix.setRotate(ofQuaternion(-0.254887, 0.167731, 0.0449435, 0.951251));

    targetCameraMatrix = longShotCameraMatrix;
    camera.setTransformMatrix(targetCameraMatrix);

    addRainPages(1);

    gui.setup("Controls");
	gui.add(twirlAmountTarget.setup("twirl", 0.1, 0, 1));
	gui.add(tiltAmountTarget.setup("tilt", 0, 0, 1));
	gui.add(flipAmountTarget.setup("flip", 0, 0, 1));
	gui.add(swayAmountTarget.setup("sway", 1, 0, 1));
	gui.add(bendTail.setup("bend tail", true));
    gui.add(bendWings.setup("bend wings", false));
    gui.add(bendFresh.setup("bend fresh", false));
    gui.add(topBendAmount.setup("top bend", 0, 0, 1));
    gui.add(bottomBendAmount.setup("bottom bend", 0.5, 0, 1));
    gui.add(tornadoAmountTarget.setup("tornado", 0, 0, 1));
    gui.add(cameraZoom.setup("camera zoom", 0, 0, 1));
    gui.add(addOneButton.setup("add one page"));
    gui.add(addTenButton.setup("add ten pages"));
    gui.add(clearButton.setup("clear all pages"));

    cameraZoom.addListener(this, &testApp::cameraZoomChanged);
    addOneButton.addListener(this, &testApp::addOneButtonPressed);
    addTenButton.addListener(this, &testApp::addTenButtonPressed);
    clearButton.addListener(this, &testApp::clearButtonPressed);
    addToggleListeners();
}

//--------------------------------------------------------------
void testApp::addToggleListeners()
{
    bendTail.addListener(this, &testApp::bendTailTogglePressed);
    bendWings.addListener(this, &testApp::bendWingsTogglePressed);
    bendFresh.addListener(this, &testApp::bendFreshTogglePressed);
}

//--------------------------------------------------------------
void testApp::removeToggleListeners()
{
    bendTail.removeListener(this, &testApp::bendTailTogglePressed);
    bendWings.removeListener(this, &testApp::bendWingsTogglePressed);
    bendFresh.removeListener(this, &testApp::bendFreshTogglePressed);
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
    static float lerpRatio = 0.2;

    twirlAmount = ofLerp(twirlAmount, twirlAmountTarget, lerpRatio);
    tiltAmount = ofLerp(tiltAmount, tiltAmountTarget, lerpRatio);
    flipAmount = ofLerp(flipAmount, flipAmountTarget, lerpRatio);
    swayAmount = ofLerp(swayAmount, swayAmountTarget, lerpRatio);
    tornadoAmount = ofLerp(tornadoAmount, tornadoAmountTarget, lerpRatio);

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

    ofMatrix4x4 newCameraTransform = camera.getLocalTransformMatrix();
    ofVec3f currTranslation = newCameraTransform.getTranslation();
    ofVec3f targetTranslation = targetCameraMatrix.getTranslation();
    ofVec3f offsetTranslation = targetTranslation - currTranslation;
    currTranslation += (offsetTranslation * lerpRatio);
    ofVec4f currRotation = newCameraTransform.getRotate().asVec4();
    ofVec4f targetRotation = targetCameraMatrix.getRotate().asVec4();
    ofVec4f offsetRotation = targetRotation - currRotation;
    currRotation += (offsetRotation * lerpRatio);
    newCameraTransform.setTranslation(currTranslation);
    newCameraTransform.setRotate(ofQuaternion(currRotation));
    camera.setTransformMatrix(newCameraTransform);

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
    if (bShowAll) {
        for (int i = 0; i < rainPages.size(); i++) {
            rainPages[i]->draw();
        }
    }
    else if (rainPages.size() > 0) {
        rainPages[0]->draw();
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

    // draw the controls
    glDisable(GL_DEPTH_TEST);
    gui.draw();
}

//--------------------------------------------------------------
void testApp::bendTailTogglePressed(bool& pressed)
{
    // radio button hack
    removeToggleListeners();

    bendWings = false;
    bendFresh = false;

    addToggleListeners();
}

//--------------------------------------------------------------
void testApp::bendWingsTogglePressed(bool& pressed)
{
    // radio button hack
    removeToggleListeners();

    bendTail  = false;
    bendFresh = false;

    addToggleListeners();
}

//--------------------------------------------------------------
void testApp::bendFreshTogglePressed(bool& pressed)
{
    // radio button hack
    removeToggleListeners();

    bendTail  = false;
    bendWings = false;

    addToggleListeners();
}

//--------------------------------------------------------------
void testApp::cameraZoomChanged(float& amount)
{
    ofVec3f startTranslation = longShotCameraMatrix.getTranslation();
    ofVec3f targetTranslation = closeUpCameraMatrix.getTranslation();
    ofVec3f offsetTranslation = targetTranslation - startTranslation;
    startTranslation += (offsetTranslation * amount);
    targetCameraMatrix.setTranslation(startTranslation);

    ofVec4f startRotation = longShotCameraMatrix.getRotate().asVec4();
    ofVec4f targetRotation = closeUpCameraMatrix.getRotate().asVec4();
    ofVec4f offsetRotation = targetRotation - startRotation;
    startRotation += (offsetRotation * amount);
    targetCameraMatrix.setRotate(ofQuaternion(startRotation));
}

//--------------------------------------------------------------
void testApp::addOneButtonPressed(bool& pressed)
{
    if (pressed) addRainPages(1);
}

//--------------------------------------------------------------
void testApp::addTenButtonPressed(bool& pressed)
{
    if (pressed) addRainPages(10);
}

//--------------------------------------------------------------
void testApp::clearButtonPressed(bool& pressed)
{
    if (pressed) {
        for (int i = 0; i < rainPages.size(); i++) {
            delete rainPages[i];
        }
        rainPages.clear();
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
    if (key == 'z') {
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