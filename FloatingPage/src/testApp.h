#pragma once

#include "ofMain.h"
#include "FlatPage.h"
#include "CurvePage.h"
#include "OtherPage.h"
#include "FallingPage.h"
#include "A5Page.h"

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

        ofEasyCam camera;
        FlatPage flatPage;
        CurvePage curvePage;
        OtherPage otherPage;
        FallingPage fallingPage;
        A5Page a5Page;

};
