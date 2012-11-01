#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){


    soundStream.listDevices();
    
	int bufferSize = 256;
    
	left = new float[256];
    right = new float[256];
    
    
    AM.setup();

    soundStream.setup(this, 0, 2, 44100, bufferSize, 4);
    
    
    total.addVertex(ofPoint(ofGetWidth()/2, ofGetWidth()/2));
       
}

//--------------------------------------------------------------
void testApp::update(){

   
    bool bLoudEnoughLastFrame = bLoudEnough;
    float pitchSmoothLastFrame = pitchSmooth;
    
    if (volume > 0.01){
        bLoudEnough = true;
    } else {
        bLoudEnough = false;
    }
    
    
    if (bLoudEnough){
        
        // if we were not loud enough last frame, and loud enough this frame, don't smooth: 
        if (!bLoudEnoughLastFrame){
            pitchSmooth = pitch;
        } else {
            pitchSmooth = 0.97f * pitchSmooth + 0.03 * pitch;
            
            
            float diffPitch = pitchSmooth - pitchSmoothLastFrame;
            
            float angleMap = ofMap(diffPitch, -400,400, PI, -PI, true);
            float distance = ofMap(volume, 0.01, 0.04, 0.01, 0.5, true);
            angleDiffs.push_back(angleMap);
            distances.push_back(distance);
        }
        
        
        
        
    }
    
    
    
    
    
    if (angleDiffs.size() > 0){
        
        
        
        angle += angleDiffs[0];
        float dist = distances[0];
        ofPoint lastVertex = total.getVertices()[ total.getVertices().size() - 1 ];
        ofPoint newVeretx = lastVertex + ofPoint( cos(angle) * dist, sin(angle) * dist);
        angleDiffs.erase(angleDiffs.begin());
        total.addVertex(newVeretx.x, newVeretx.y);
        distances.erase(distances.begin());
        
    }
    if (total.getVertices().size() > 0){
        catchPoint = 0.9f * catchPoint + 0.1f * total.getVertices()[total.getVertices().size()-1];
    }
    

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
        //ofRect(x, y + fftBounds.height, (pctB-pctA) * fftBounds.width, -AM.results.fftOctaves[i] * 5);
    }
    ///cout << AM.results.fftOctaves
    
    
    
    if (total.getVertices().size() > 0){
        
        
        ofPushMatrix();
        ofTranslate(-catchPoint.x + ofGetWidth()/2, -catchPoint.y + ofGetHeight()/2);
        
        
        glBegin(GL_LINE_STRIP);
        for (int i = 0; i < total.getVertices().size(); i++){
            
            glVertex3f(total.getVertices()[i].x, total.getVertices()[i].y, total.getVertices()[i].z);
        }
        glEnd();
        ofEndShape();
        
        //total.draw();
        ofPopMatrix();
        
        
    }
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

 //   AL.addAngleLength(ofRandom(0,1), ofRandom(1,2));
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

float freqDiffSmooth = 0;
//--------------------------------------------------------------
void testApp::audioIn(float * input, int bufferSize, int nChannels){	
	
	for (int i = 0; i < bufferSize; i++){
		left[i]		= input[i*2];
		right[i]	= input[i*2+1];
	}
    
    AM.update(left, bufferSize);
    
    pitch = AM.results.aubioPitch;
    volume = AM.results.aubioRMS;
    
}