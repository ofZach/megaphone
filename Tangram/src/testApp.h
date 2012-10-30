#pragma once

#include "ofMain.h"

#include "vectorField.h"
#include "Word.h"

//--------------------------------------------------------------
//--------------------------------------------------------------
class testApp : public ofBaseApp
{
	public:
		void setup();
        void update();
		void draw();

        void clear();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
        void gotMessage(ofMessage msg);

        int mode;
        bool bDebug;

        vectorField VF;
        ofVec2f crawlerInward;
        ofVec2f crawlerClockwise;

        vector<Word *> words;
        ofVec2f prevMousePos, mousePos;

        vector<Glyph> alphabet;
        float alphaScale;
        ofVec3f alphaPos;
        ofVec3f linePos;
};
