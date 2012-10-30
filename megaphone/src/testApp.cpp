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

   //cout << AM.results.RMS << " " << AM.results.pitch << endl;
}

//--------------------------------------------------------------
void testApp::draw(){
    
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