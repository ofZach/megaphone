#include "testApp.h"
#include "typeScene2.h"

//--------------------------------------------------------------
void testApp::setup(){
    
    soundStream.listDevices();
    
	int bufferSize = 256;
    
	left = new float[bufferSize];
    right = new float[bufferSize];
        
    AM.setup();

    nBuffersRecorded = 0;
    audioDataThread = new float[ bufferSize * 100]; // almost half a second
    audioDataMainThread = new float [ bufferSize * 100];
        
    soundStream.setup(this, 2, 2, 44100, bufferSize, 4);
    
    scenes.push_back(new typeScene());
    for (int i = 0; i < scenes.size(); i++){
        scenes[i]->results = &AM.results;
    }
    scenes[0]->setup();    
    
    ofSetFrameRate(1000);
    ofSetVerticalSync(false);
    
    
    ASL.loadFile("samples/1-1.wav");
    ASL.bPlaying = false;
    
    ofBackground(0,0,0);
    
    
    world.allocate(768, 2048);
    
    
    mesh.setMode(OF_PRIMITIVE_TRIANGLES);
    
    float w = 768;
    float h = 1024*2;
    
    float wSize = 8*100;
    float hSize = 3*100 / 2.0;
    
    
    //float pinchAmount = ofMap(mouseX, 0, ofGetWidth(), 0,1);
    float pinchAmount = 0.1f;
    
    mesh.addTexCoord(ofPoint(w/2, pinchAmount*h));
    mesh.addVertex(ofPoint(0,0,0));
    mesh.addTexCoord(ofPoint(w/2, h));
    mesh.addVertex(ofPoint(0,wSize,0));
    mesh.addTexCoord(ofPoint(w, h));
    mesh.addVertex(ofPoint(0,wSize,hSize));
    
    
    mesh.addTexCoord(ofPoint(w/2, pinchAmount*h));
    mesh.addVertex(ofPoint(0,0,0));
    mesh.addTexCoord(ofPoint(w, 0));
    mesh.addVertex(ofPoint(0,0,hSize));
    mesh.addTexCoord(ofPoint(w, h));
    mesh.addVertex(ofPoint(0,wSize,hSize));
    
    mesh.addTexCoord(ofPoint(w/2, pinchAmount*h));
    mesh.addVertex(ofPoint(0,0,0));
    mesh.addTexCoord(ofPoint(w/2, h));
    mesh.addVertex(ofPoint(0,wSize,0));
    mesh.addTexCoord(ofPoint(0, h));
    mesh.addVertex(ofPoint(hSize,wSize,0));
    
    mesh.addTexCoord(ofPoint(w/2, pinchAmount*h));
    mesh.addVertex(ofPoint(0,0,0));
    mesh.addTexCoord(ofPoint(0, 0));
    mesh.addVertex(ofPoint(hSize,0,0));
    mesh.addTexCoord(ofPoint(0,h));
    mesh.addVertex(ofPoint(hSize,wSize,0));

    
    
    
    
//    mesh.addVertex(ofPoint(0,0,0));
//    mesh.addVertex(ofPoint(0,8,0));
//    mesh.addVertex(ofPoint(1.5,8,0));
//    
//    mesh.addVertex(ofPoint(0,8,0));
//    mesh.addVertex(ofPoint(0,0,0));
//    mesh.addVertex(ofPoint(1.5,0,0));
//    
    
    
    cam.setupPerspective();
    
}

void testApp::exit(){
    soundStream.close();
    ofSleepMillis(100);
}



//--------------------------------------------------------------
void testApp::update(){

    
    //int nFramesAudio = 0; 
    
//    if (mutex.tryLock(10)){
//        memcpy( audioDataMainThread, audioDataThread, nBuffersRecorded * 256 * sizeof(float));
//        nFramesAudio = nBuffersRecorded;
//        nBuffersRecorded = 0;
//        mutex.unlock();
//    }
//    
//    for (int i = 0; i < nFramesAudio; i++){
//        AM.update(audioDataMainThread + i * 256, 256);
//    }
    
//    AM.update(left, bufferSize);
    
    
    
    ofClear(0,0,0,255);
    scenes[0]->update();
    
    
    
    
}



//--------------------------------------------------------------
void testApp::draw(){
    
    if (ofGetFrameNum() < 10) return;
    
    //AM.draw();
    
    scenes[0]->draw(); 

    //ofBackground(127,127,127);
    
//    world.begin();
//    ofClear(0,0,0, 255);
//    scenes[0]->draw();
//    world.end();
//    
//    
//    ofSetColor(255,255,255);
//    cam.begin();
//    world.getTextureReference().bind();
//    mesh.draw();
//    world.getTextureReference().unbind();
//    ofSetColor(255,255,255,10);
//    mesh.drawWireframe();
//    cam.end();
    
    ofDrawBitmapStringHighlight(ofToString(ofGetFrameRate()), ofGetWidth()-100, 100);
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

    
    if (key == 'l'){
        ofSetWindowShape(1400, (3/8.0) * 1400);
    }
    
    if (key == 's'){
        ASL.bPlaying = true;
        ASL.playbackCount = 0;
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

void testApp::audioOut(float * output, int bufferSize, int nChannels){
    
    
        for (int i = 0; i < bufferSize; i++){
            if (ASL.bPlaying == true){
                left[i] = output[i*2] = ASL.audioData[ASL.playbackCount * ASL.channels + 0];
                right[i] = output[i*2+1] = ASL.audioData[ASL.playbackCount * ASL.channels + 0];
                ASL.playbackCount++;
            } else {
                output[i*2] = 0;
                output[i*2 + 1] = 0;
            }
            
            if (ASL.playbackCount >= ASL.audioData.size()/ASL.channels){
                ASL.bPlaying = false;
            }
        }
    
    
}

//--------------------------------------------------------------
void testApp::audioIn(float * input, int bufferSize, int nChannels){	
	
    
    for (int i = 0; i < bufferSize; i++){
        if (ASL.bPlaying == true){
            //left[i] = ASL.audioData[ASL.playbackCount * ASL.channels + 0];
            //right[i] = ASL.audioData[ASL.playbackCount * ASL.channels + 0];
            //ASL.playbackCount++;
        } else {
            left[i]		= input[i*2];
            right[i]	= input[i*2+1];
        }
        
        
    }
    
    

//    mutex.lock();
//        if (nBuffersRecorded < 100){
//            memcpy( audioDataThread + nBuffersRecorded * bufferSize, left, bufferSize * sizeof(float));
//            nBuffersRecorded++;
//        } else {
//            printf("skip! ?\n");
//        }
//        mutex.unlock();
//    
    AM.update(left, 256);
      
}