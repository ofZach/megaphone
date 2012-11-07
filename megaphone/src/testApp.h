#pragma once

#include "ofMain.h"
#include "audioManager.h"
#include "baseScene.h"
#include "typeScene.h"
#include "audioSampleLoader.h"



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
    
        void audioOut(float * output, int bufferSize, int nChannels); 
        void audioIn(float * input, int bufferSize, int nChannels); 
        float * left;
        float * right;
        
        ofSoundStream soundStream;
        audioManager AM;
    
        int nBuffersRecorded;
        float * audioDataThread;
        float * audioDataMainThread;
    
    
        ofMutex mutex;

    
        vector < baseScene *  > scenes;
        
        audioSampleLoader ASL;
    
    
    
        ofFbo world;
    
    
        ofMesh mesh;
    
    
        ofEasyCam cam;
    
    
        
};
