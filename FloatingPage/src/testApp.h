#pragma once

#include "ofMain.h"
#include "Page.h"

class testApp : public ofBaseApp
{
	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

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

        void addRainPages(int num);

        ofEasyCam camera;

        bool bShowAll;
        vector<Page *> rainPages;

        ofMatrix4x4 targetCameraMatrix;
        ofMatrix4x4 closeUpCameraMatrix;
        ofMatrix4x4 longShotCameraMatrix;

        ofxPanel gui;
        ofxLabel spacerLabel;
        ofxToggle drawGroundToggle;
        ofxToggle fillGroundToggle;
        ofxToggle drawAxesToggle;
        ofxToggle maskToggle;
        ofxFloatSlider offsetAmountTarget;
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
};
