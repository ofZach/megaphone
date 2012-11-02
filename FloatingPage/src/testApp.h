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

        void addRainPages(int num);

        ofEasyCam camera;

        Page flatPage;
        Page flexPage;
        Page swayPage;
        Page vertPage;
        Page flipPage;

        bool bShowRain;
        bool bShowAll;
        vector<Page *> rainPages;

        ofMatrix4x4 closeUpCameraMatrix;
        ofMatrix4x4 longShotCameraMatrix;

};
