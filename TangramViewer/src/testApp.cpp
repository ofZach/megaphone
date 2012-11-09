#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetVerticalSync(true);
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    
    ofSeedRandom(0); 
    
    //  SOUND
    //
	soundStream.setup(this, 0, 2, 44100, 256, 4);
    smoothedVol     = 0.0;
    micBar.setLabel("mic");
    
    //  VISUAL
    //
    glyph = new Glyph();
    glyph->setScale(100);
//    tangram.createFromGlyph( glyph );
    tangram.createSet();
    tangram.bDebug = false;
    tangram.bEdit = false;

    cameraAngle = 0.0;
    tangramAngle = 0.0;
    
    //  CONTROL
    //
    gui.setup("Controls");
    gui.add(posLerp.setup("position_amount", 0.05, 0.0, 0.1));
    gui.add(rotLerp.setup("rotation_amount", 0.05, 0.0, 0.1));
    gui.add(camLerp.setup("camera_amount",0.05,0.0,0.1));
    gui.add(wakeUpDist.setup("wakeUpDist", 100, 50, 500));
    gui.loadFromFile("gui.xml");
    gui.setPosition(ofPoint(5,45));
    dataDir.listDir("tangramShape");
    selectedFile = 0;
    bDebug = true;
}

//--------------------------------------------------------------
void testApp::update(){
    
    //  Audio
    //
    micBar.setValue(smoothedVol);
    if (bDebug){
        micBar.updateGUI();
    }
    
    //cameraAngle = ofLerp(cameraAngle, tangramAngle, camLerp);
    
    tangram.addCentralRepulsionForce( micBar.getActiveValue() );
    tangram.update(posLerp, rotLerp, camLerp, wakeUpDist);
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void testApp::draw(){
	
	
	
    ofBackgroundGradient(ofColor(80,80,80), ofColor::black);
    
    ofPushMatrix();
    ofTranslate(ofGetWidth()*0.5, ofGetHeight()*0.5);
    ofRotateY(cameraAngle);
    tangram.draw();
    ofPopMatrix();
    
    //  Draw the average volume
    //
    if (bDebug){
        ofSetColor(255);
        gui.draw();
        micBar.draw();
    }
}

void testApp::exit(){
    gui.saveToFile("gui.xml");
}

//--------------------------------------------------------------
void testApp::audioIn(float * input, int bufferSize, int nChannels){
	float curVol = 0.0;
	
	// samples are "interleaved"
	int numCounted = 0;
    
    float left[bufferSize];
    float right[bufferSize];
    
	//lets go through each sample and calculate the root mean square which is a rough way to calculate volume
	for (int i = 0; i < bufferSize; i++){
		left[i]		= input[i*2]*0.5;
		right[i]	= input[i*2+1]*0.5;
        
		curVol += left[i] * left[i];
		curVol += right[i] * right[i];
		numCounted+=2;
	}
	
	//this is how we get the mean of rms :)
	curVol /= (float)numCounted;
	
	// this is how we get the root of rms :)
	curVol = sqrt( curVol );
	
	smoothedVol *= 0.93;
	smoothedVol += 0.07 * curVol;
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
    if (key == ' '){
//        delete glyph;
//        glyph = new Glyph();
//        glyph->setScale(100);
//        tangram.createFromGlyph( glyph );
        
        tangram.load( "tangramShape/" + dataDir.getFile(selectedFile).getFileName() );
        tangramAngle = ofRandom(-50,50);
        tangram.rotateY(tangramAngle);
		
		//tangramAngle = ofRandom(-50,50);
		//tangram.rotateX(ofRandom(-50,50));
//		
//		tangramAngle = ofRandom(-50,50);
		//tangram.rotateZ(ofRandom(-50,50));
		
		
        tangram.addCentralRepulsionForce(1.0);
        selectedFile = (selectedFile+1)%dataDir.size();
    } else if (key == 'm'){
        bDebug = !bDebug;
    } else if (key == 'd'){
        tangram.bDebug = !tangram.bDebug;
    } else if (key == 'e'){
        tangram.bEdit = !tangram.bEdit;
    } else if (key == 't'){
        tangram.bTrail = !tangram.bTrail;
    } else if (key == 'z'){
        tangram.bSortZ = !tangram.bSortZ;
    } else if (key == 's'){
        string filename = "t"+ofToString(ofGetDay())+"-"+ofToString(ofGetHours())+"-"+ofToString(ofGetMinutes())+"-"+ofToString(ofGetSeconds());
        tangram.save("tangramShape/"+filename+".tan");
    }
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
void testApp::mouseReleased(int x, int y, int button)
{
    
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