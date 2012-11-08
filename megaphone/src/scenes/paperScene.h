#pragma once

#include "ofMain.h"
#include "Page.h"
#include "baseScene.h"



class paperScene : public baseScene {
    
	public:
    
		void setup();
		void update();
		void draw();

        void addPages(int num);

        ofEasyCam camera;

        ofLight pointLight;
    
        ofLight cameraLight;
    
        ofMaterial material;
    
        vector<Page *> pages;
        ofMesh groundMesh;
    
    void addToggleListeners();
    void removeToggleListeners();
    void bendTailTogglePressed(bool& pressed);
    void bendWingsTogglePressed(bool& pressed);
    void bendFreshTogglePressed(bool& pressed);
    void tornadoAmountChanged(float& amount);
    void cameraZoomChanged(float& amount);
    void addOneButtonPressed(bool& pressed);
    void addTenButtonPressed(bool& pressed);
    void clearButtonPressed(bool& pressed);
    void cameraMouseTogglePressed(bool& pressed);
    void snapCloseUpPressed(bool& pressed);
    void snapLongShotPressed(bool& pressed);

        ofMatrix4x4 targetCameraMatrix;
        ofMatrix4x4 closeUpCameraMatrix;
        ofMatrix4x4 longShotCameraMatrix;

        ofxPanel gui;
        ofxLabel spacerLabel;
        ofxToggle drawGroundToggle;
        ofxToggle fillGroundToggle;
        ofxToggle drawAxesToggle;
        ofxToggle maskToggle;
        ofxToggle enableLightToggle;
        ofxToggle drawLightToggle;
        ofxFloatSlider lightPos;
        ofxFloatSlider offsetAmountTarget;
        ofxFloatSlider alignAmountTarget;
        ofxFloatSlider twirlAmountTarget;
        ofxFloatSlider tiltAmountTarget;
        ofxFloatSlider flipAmountTarget;
        ofxFloatSlider swayAmountTarget;
        ofxFloatSlider tornadoAmountTarget;
        ofxFloatSlider speedAmountTarget;
        ofxFloatSlider expandAmountTarget;
        ofxFloatSlider liftAmountTarget;
        ofxFloatSlider cameraZoom;
        ofxToggle cameraMouseToggle;
        ofxButton snapCloseUp;
        ofxButton snapLongShot;
        ofxButton addOneButton;
        ofxButton addTenButton;
        ofxButton clearButton;
    
    
    
        float rmsSmooth;
    
    
    
};
