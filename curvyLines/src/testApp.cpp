#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){	
	
	ofBackground(0,0,0);
	
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
	
	
	for (int i = 0; i < 10; i++){
        blob temp;
        blobs.push_back(temp);
        blobs[blobs.size()-1].setup();
    }
    
    VF.setupField(60,40,ofGetWidth(), ofGetHeight());
	
}

//--------------------------------------------------------------
void testApp::update(){

	// on every frame 
	// we reset the forces
	// add in any forces on the particle
	// perfom damping and
	// then update
	
	
	for (int i = 0; i < blobs.size(); i++){
        blobs[i].resetForces();
        blobs[i].addInternalForces();
    }
    
    for (int i = 0; i < blobs.size(); i++){
        for (int j = 0; j < i; j++){
            blobs[i].repelFromBlob(blobs[j]);
        }
    }
    
    for (int i = 0; i < blobs.size(); i++){
		 for (int j = 0; j < blobs[i].particles.size(); j++){
             ofVec2f frc;
             frc = VF.getForceFromPos(blobs[i].particles[j]->pos.x, blobs[i].particles[j]->pos.y);
             blobs[i].particles[j]->addForce(frc.x, frc.y);
         }
	}
    
    
    
    
    for (int i = 0; i < blobs.size(); i++){
        blobs[i].update();
    }
}

//--------------------------------------------------------------
void testApp::draw(){
	
    VF.fadeField(0.99);
    
    VF.draw();

	ofSetColor(0xffffff);
	
	for (int i = 0; i < blobs.size(); i++){
        blobs[i].draw();
    }
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 
	
	
	
	
}

//--------------------------------------------------------------
void testApp::keyReleased  (int key){ 
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	
	float diffx = x - prevMouseX;
	float diffy = y - prevMouseY;
	
	VF.addInwardCircle(mouseX, mouseY, 600, 0.1f);
    
    VF.addClockwiseCircle(mouseX, mouseY, 260, 0.3f);
	
	prevMouseX = x;
	prevMouseY = y;
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	prevMouseX = x; 
	prevMouseY = y;
}


//--------------------------------------------------------------
void testApp::mouseReleased(){
	
	//particles[0].bFixed = false;
}

