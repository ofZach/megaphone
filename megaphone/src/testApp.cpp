#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){


    soundStream.listDevices();
    
	int bufferSize = 256;
    
	left = new float[256];
    right = new float[256];
    
	soundStream.setup(this, 0, 2, 44100, bufferSize, 4);
    
    AM.setup();
    
}

//--------------------------------------------------------------
void testApp::update(){

   

}



//--------------------------------------------------------------
void testApp::draw(){
    
    
    ofDrawBitmapStringHighlight("rms: " + ofToString(AM.results.RMS), ofPoint(30,30));
    ofDrawBitmapStringHighlight("pitch: " + ofToString(AM.results.pitch), ofPoint(30,60));
    ofDrawBitmapStringHighlight("aubio rms: " + ofToString(AM.results.aubioRMS), ofPoint(30,90));
    ofDrawBitmapStringHighlight("aubio pitch: " + ofToString(AM.results.aubioPitch), ofPoint(30,120));
    
    ofRectangle fftBounds;
    fftBounds.set(30,150, 300, 100);
    ofFill();
    ofSetColor(100);
    ofRect(fftBounds);
    ofSetColor(255);
    for (int i = 0; i < AM.results.nFftOctaves; i++){
        float pctA = i/(float)AM.results.nFftOctaves; 
        float pctB = (i+1)/(float)AM.results.nFftOctaves; 
        
        float x = fftBounds.x + pctA * fftBounds.width;
        float y = fftBounds.y;
        ofRect(x, y + fftBounds.height, (pctB-pctA) * fftBounds.width, -AM.results.fftOctaves[i] * 5);
    }
    ///cout << AM.results.fftOctaves
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void testApp::audioIn(float * input, int bufferSize, int nChannels){	
	
	for (int i = 0; i < bufferSize; i++){
		left[i]		= input[i*2];
		right[i]	= input[i*2+1];
	}
    
    AM.update(left, bufferSize);
    
    
}