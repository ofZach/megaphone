#pragma once

#include "ofMain.h"
#include "audioManager.h"
#include "Glyph.h"
#include "angleLengthLine.h"

class testApp : public ofBaseApp{

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
        void exit();
    
    
        void audioIn(float * input, int bufferSize, int nChannels); 
        float * left;
        float * right;
        
        ofSoundStream soundStream;
        audioManager AM;
    
    
     
    
    float pitch;
    float volume;
    bool bLoudEnough;
    float pitchSmooth;      // non aubio needs some love -- works well w/ whisteling. 
    
    
    vector < float > angleDiffs;
    vector < float > distances;
    ofPolyline total;
    float angle;
    float angleSmooth;
    ofPoint catchPoint;
    
    
    
    vector < Glyph *  > glyphs;
    
    
};
