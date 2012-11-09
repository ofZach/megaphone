#pragma once

#include "ofMain.h"
#include "audioManager.h"
#include "baseScene.h"
#include "typeScene.h"
#include "audioSampleLoader.h"
#include "ofxProjectorBlend.h"
#include "mtl2dMapping.h"
#include "ControlsMapping.h"

#define PIXEL_OVERLAP 30
#define PROJECTOR_COUNT 2
#define PROJECTOR_WIDTH 1024
#define PROJECTOR_HEIGHT 768




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
        bool bDrawAudioManager;
    
    
        int nBuffersRecorded;
        float * audioDataThread;
        float * audioDataMainThread;
    
        ofMutex mutex;
        vector < baseScene *  > scenes;
        audioSampleLoader ASL;
    
    
        // rendering things: 
        ofxProjectorBlend blender;
        mtl2dMapping* _mapping;
    
        //bool bDrawForProjector
        
    

};
