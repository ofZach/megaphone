#include "paperScene.h"

//--------------------------------------------------------------
void paperScene::setup(){
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    ofBackground(0);

    camera.disableMouseInput();
    camera.tilt(-30);
    camera.rotate(20, 0, 1, 0);

    closeUpCameraMatrix.setTranslation(40.3154, 68.0548, 110.766);
    closeUpCameraMatrix.setRotate(ofQuaternion(-0.254887, 0.167731, 0.0449435, 0.951251));

    longShotCameraMatrix.setTranslation(242.633, 457.804, 637.342);
    longShotCameraMatrix.setRotate(ofQuaternion(-0.254887, 0.167731, 0.0449435, 0.951251));

    targetCameraMatrix = longShotCameraMatrix;
    camera.setTransformMatrix(targetCameraMatrix);

    ofSetSmoothLighting(true);
    pointLight.setPosition(0, groundSize, 0);
    pointLight.setDiffuseColor( ofColor(255.f, 255.f, 255.f));
	pointLight.setSpecularColor( ofColor(255.f, 255.f, 255.f));
    pointLight.setAmbientColor(ofColor(100,100,100));
    
    //pointLight.setSpotConcentration(0.3);
    //cameraLight
    
    
    /*cameraLight.setPosition(0, groundSize, 0);
    cameraLight.setDiffuseColor( ofColor(50,50,50));
	cameraLight.setSpecularColor( ofColor(80,80,80));
    */


	material.setShininess(64);
	material.setSpecularColor(ofFloatColor(1.0, 1.0, 1.0));

    // build the ground
    ofVec3f vertices[] = {
        ofPoint(-groundSize, 0, -groundSize),
        ofPoint(-groundSize, 0,  groundSize),
        ofPoint( groundSize, 0,  groundSize),
        ofPoint( groundSize, 0, -groundSize)
    };
    ofVec3f normals[] = {
        ofPoint(0, 1, 0),
        ofPoint(0, 1, 0),
        ofPoint(0, 1, 0),
        ofPoint(0, 1, 0)
    };
    ofIndexType indices[] = {
        0, 1, 2,
        2, 3, 0
    };
    groundMesh.addVertices(vertices, 4);
    groundMesh.addNormals(normals, 4);
    groundMesh.addIndices(indices, 6);

    addPages(1);

    gui.setup("Controls");
    gui.add(drawGroundToggle.setup("draw ground", true));
    gui.add(fillGroundToggle.setup("fill ground", true));
    gui.add(drawAxesToggle.setup("draw axes", false));
    gui.add(maskToggle.setup("mask", false));
    gui.add(spacerLabel.setup("spacer", ""));
    gui.add(enableLightToggle.setup("enable light", true));
    gui.add(drawLightToggle.setup("draw light", false));
    gui.add(lightPos.setup("light pos", 1, 0, 1));
    gui.add(debugMesh.setup("debug mesh", false));
    gui.add(spacerLabel.setup("spacer", ""));
    gui.add(offsetAmountTarget.setup("offset", 0, 0, 1));
    gui.add(spacerLabel.setup("spacer", ""));
	gui.add(twirlAmountTarget.setup("twirl", 0.1, 0, 1));
	gui.add(tiltAmountTarget.setup("tilt", 0, 0, 1));
	gui.add(flipAmountTarget.setup("flip", 0, 0, 1));
	gui.add(swayAmountTarget.setup("sway", 1, 0, 1));
	gui.add(spacerLabel.setup("spacer", ""));
	gui.add(bendTail.setup("bend tail", true));
    gui.add(bendWings.setup("bend wings", false));
    gui.add(bendFresh.setup("bend fresh", false));
    gui.add(topBendAmount.setup("top bend", 0, 0, 1));
    gui.add(bottomBendAmount.setup("bottom bend", 0.5, 0, 1));
    gui.add(spacerLabel.setup("spacer", ""));
	gui.add(tornadoAmountTarget.setup("tornado", 0, 0, 1));
	gui.add(speedAmountTarget.setup("speed", 0, 0, 1));
	gui.add(expandAmountTarget.setup("expand", 1, 0, 1));
	gui.add(liftAmountTarget.setup("lift", 1, 0, 1));
    gui.add(spacerLabel.setup("spacer", ""));
    gui.add(cameraZoom.setup("camera zoom", 0, 0, 1));
    gui.add(cameraMouseToggle.setup("mouse control", false));
	gui.add(snapCloseUp.setup("save close up"));
	gui.add(snapLongShot.setup("save long shot"));
	gui.add(spacerLabel.setup("spacer", ""));
	gui.add(addOneButton.setup("add one page"));
    gui.add(addTenButton.setup("add ten pages"));
    gui.add(clearButton.setup("clear all pages"));

    cameraZoom.addListener(this, &paperScene::cameraZoomChanged);
    cameraMouseToggle.addListener(this, &paperScene::cameraMouseTogglePressed);
    snapCloseUp.addListener(this, &paperScene::snapCloseUpPressed);
    snapLongShot.addListener(this, &paperScene::snapLongShotPressed);
    addOneButton.addListener(this, &paperScene::addOneButtonPressed);
    addTenButton.addListener(this, &paperScene::addTenButtonPressed);
    clearButton.addListener(this, &paperScene::clearButtonPressed);
    addToggleListeners();
}

//--------------------------------------------------------------
void paperScene::addToggleListeners()
{
    bendTail.addListener(this, &paperScene::bendTailTogglePressed);
    bendWings.addListener(this, &paperScene::bendWingsTogglePressed);
    bendFresh.addListener(this, &paperScene::bendFreshTogglePressed);
}

//--------------------------------------------------------------
void paperScene::removeToggleListeners()
{
    bendTail.removeListener(this, &paperScene::bendTailTogglePressed);
    bendWings.removeListener(this, &paperScene::bendWingsTogglePressed);
    bendFresh.removeListener(this, &paperScene::bendFreshTogglePressed);
}

//--------------------------------------------------------------
void paperScene::addPages(int num)
{
    for (int i = 0; i < num; i++) {
        Page *page = new Page();
        page->path.setFillColor(ofColor(255));
        page->rainSpeed.set(ofRandom(-2, -5), -1 * ofRandom(2, 10), ofRandom(10));

        pages.push_back(page);
    }
}

//--------------------------------------------------------------
void paperScene::update()
{
    static float lerpRatio = 0.2;

    offsetAmount = ofLerp(offsetAmount, offsetAmountTarget, lerpRatio);
    if (ABS(offsetAmountTarget - offsetAmount) < 0.01) offsetAmount = offsetAmountTarget;

    twirlAmount = ofLerp(twirlAmount, twirlAmountTarget, lerpRatio);
    if (ABS(twirlAmountTarget - twirlAmount) < 0.01) twirlAmount = twirlAmountTarget;

    tiltAmount = ofLerp(tiltAmount, tiltAmountTarget, lerpRatio);
    if (ABS(tiltAmountTarget - tiltAmount) < 0.01) tiltAmount = tiltAmountTarget;

    flipAmount = ofLerp(flipAmount, flipAmountTarget, lerpRatio);
    if (ABS(flipAmountTarget - flipAmount) < 0.01) flipAmount = flipAmountTarget;

    swayAmount = ofLerp(swayAmount, swayAmountTarget, lerpRatio);
    if (ABS(swayAmountTarget - swayAmount) < 0.01) swayAmount = swayAmountTarget;

    tornadoAmount = ofLerp(tornadoAmount, tornadoAmountTarget, lerpRatio);
    if (ABS(tornadoAmountTarget - tornadoAmount) < 0.01) tornadoAmount = tornadoAmountTarget;

    speedAmount = ofLerp(speedAmount, speedAmountTarget, lerpRatio);
    if (ABS(speedAmountTarget - speedAmount) < 0.01) speedAmount = speedAmountTarget;

    expandAmount = ofLerp(expandAmount, expandAmountTarget, lerpRatio);
    if (ABS(expandAmountTarget - expandAmount) < 0.01) expandAmount = expandAmountTarget;

    liftAmount = ofLerp(liftAmount, liftAmountTarget, lerpRatio);
    if (ABS(liftAmountTarget - liftAmount) < 0.01) liftAmount = liftAmountTarget;

    if (!camera.getMouseInputEnabled()) {
        // tween the camera to its target position
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
    }

    pointLight.setPosition(0, groundSize * lightPos * 10, 0);
    
    //pointLight.setPosition(  pointLight.getPosition() * 0.5f + camera.getPosition() * 0.5);
    

    for (int i = 0; i < pages.size(); i++) {
        pages[i]->update();
    }
}

//--------------------------------------------------------------
void paperScene::draw()
{
    
    //cameraLight.setPosition(camera.getPosition());
    
    
    
    camera.begin();

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    if (drawLightToggle) {
        ofSetColor(pointLight.getDiffuseColor());
        ofSphere(pointLight.getPosition(), 10.f);
    }

    if (enableLightToggle) {
        ofEnableLighting();
        pointLight.enable();
        //cameraLight.enable();
        material.begin();
        
        glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,1);
    }

    if (drawGroundToggle) {
        ofSetColor(128);
        if (fillGroundToggle) groundMesh.draw();
        else groundMesh.drawWireframe();
    }

    ofSetColor(255);

    // draw the pages
    for (int i = 0; i < pages.size(); i++) {
        pages[i]->draw();
    }

    if (enableLightToggle) {
        material.end();
        ofDisableLighting();
    }

    if (drawAxesToggle) {
        ofPushStyle();
        ofSetLineWidth(2);

        // draw the 3d axes
        ofSetColor(255, 0, 0);
        ofLine(0, 0, 0, groundSize, 0, 0);
        ofSetColor(0, 255, 0);
        ofLine(0, 0, 0, 0, groundSize, 0);
        ofSetColor(0, 0, 255);
        ofLine(0, 0, 0, 0, 0, groundSize);

        ofPopStyle();
    }

    camera.end();

    if (maskToggle) {
        // mask out the edges to end up with a 3:8 ratio display (2 x 3:4)
        int maskHeight = ofGetHeight();
        int windowWidth = maskHeight * 3 / 8;
        int maskWidth = (ofGetWidth() - windowWidth) / 2;

        ofPushStyle();
        ofSetColor(0);

        ofRect(0, 0, maskWidth, maskHeight);
        ofRect(ofGetWidth() - maskWidth, 0, maskWidth, maskHeight);

        ofPopStyle();
    }

    glDisable(GL_DEPTH_TEST);

    // draw the controls
    ofSetColor(255);
    gui.draw();
    ofDrawBitmapString(ofToString(ofGetFrameRate(), 2) + " fps", ofGetWidth() - 50, ofGetHeight() - 10);
}

//--------------------------------------------------------------
void paperScene::bendTailTogglePressed(bool& pressed)
{
    // radio button hack
    removeToggleListeners();

    bendWings = false;
    bendFresh = false;

    addToggleListeners();
}

//--------------------------------------------------------------
void paperScene::bendWingsTogglePressed(bool& pressed)
{
    // radio button hack
    removeToggleListeners();

    bendTail  = false;
    bendFresh = false;

    addToggleListeners();
}

//--------------------------------------------------------------
void paperScene::bendFreshTogglePressed(bool& pressed)
{
    // radio button hack
    removeToggleListeners();

    bendTail  = false;
    bendWings = false;

    addToggleListeners();
}

//--------------------------------------------------------------
void paperScene::cameraZoomChanged(float& amount)
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
void paperScene::cameraMouseTogglePressed(bool& pressed)
{
    if (pressed)
        camera.enableMouseInput();
    else
        camera.disableMouseInput();
}

//--------------------------------------------------------------
void paperScene::snapCloseUpPressed(bool& pressed)
{
    if (pressed)
        closeUpCameraMatrix = camera.getLocalTransformMatrix();
}

//--------------------------------------------------------------
void paperScene::snapLongShotPressed(bool& pressed)
{
    if (pressed)
        longShotCameraMatrix = camera.getLocalTransformMatrix();
}

//--------------------------------------------------------------
void paperScene::addOneButtonPressed(bool& pressed)
{
    if (pressed) addPages(1);
}

//--------------------------------------------------------------
void paperScene::addTenButtonPressed(bool& pressed)
{
    if (pressed) addPages(10);
}

//--------------------------------------------------------------
void paperScene::clearButtonPressed(bool& pressed)
{
    if (pressed) {
        for (int i = 0; i < pages.size(); i++) {
            delete pages[i];
        }
        pages.clear();
    }
}

////--------------------------------------------------------------
//void paperScene::keyPressed(int key)
//{
//    if (key == 'z') {
//        longShotCameraMatrix = camera.getLocalTransformMatrix();
//    }
//    else if (key == 'x') {
//        closeUpCameraMatrix = camera.getLocalTransformMatrix();
//    }
//    else if (key == 'p') {
//        cout << "Camera " << endl
//             << "  trans = " << camera.getLocalTransformMatrix().getTranslation() << endl
//             << "  rotat = " << camera.getLocalTransformMatrix().getRotate().asVec4() << endl
//             << "  scale = " << camera.getLocalTransformMatrix().getScale() << endl;
//        cout << "  dist  = " << camera.getDistance() << endl;
//    }
//}

/*
//--------------------------------------------------------------
void paperScene::keyReleased(int key){

}

//--------------------------------------------------------------
void paperScene::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void paperScene::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void paperScene::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void paperScene::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void paperScene::windowResized(int w, int h){

}

//--------------------------------------------------------------
void paperScene::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void paperScene::dragEvent(ofDragInfo dragInfo){ 

}


*/