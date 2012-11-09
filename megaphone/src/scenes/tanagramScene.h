#pragma once

#include "Tangram.h"
#include "baseScene.h"
#include "ofxGui.h"
#include "glyph.h"


class tanagramScene : public baseScene {
	
public:
    void setup();
    void update();
    void draw();
	void drawDebug();
	
	
	
    void exit();

    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    //  Audio
    //
    //void audioIn(float * input, int bufferSize, int nChannels);
//    ofSoundStream   soundStream;
//    float           smoothedVol;
//    ThresholdBar    micBar;
    
    //  Visual
    //
    Glyph           *glyph;
    Tangram         tangram;

    //  Control
    //
    ofxPanel        gui;
    ofxFloatSlider  posLerp;
    ofxFloatSlider  rotLerp;
    ofxFloatSlider  camLerp;
    ofxIntSlider    wakeUpDist;
    
    ofDirectory     dataDir;
    int             selectedFile;
    
    float           cameraAngle;
    float           tangramAngle;
    int             width, height;
    bool            bDebug;
};
