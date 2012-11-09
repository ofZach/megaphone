#include "testApp.h"
#include "paperScene.h"


//--------------------------------------------------------------
void testApp::setup(){
    
    
	//---------------------------------------------------
    // audio samples for testing
    ASL.loadFile("samples/1-1.wav");
    ASL.bPlaying = false;
    
	
	
    //---------------------------------------------------
    // setup audio
    soundStream.listDevices();
	int bufferSize = 256;
	left = new float[bufferSize];
    right = new float[bufferSize];
    AM.setup();
    nBuffersRecorded = 0;
    audioDataThread = new float[ bufferSize * 100]; // almost half a second
    audioDataMainThread = new float [ bufferSize * 100];        
    soundStream.setup(this, 2, 2, 44100, bufferSize, 4);
    scenes.push_back(new tanagramScene());
    for (int i = 0; i < scenes.size(); i++){
        scenes[i]->results = &AM.results;
    }
    scenes[0]->setup();   
    bDrawAudioManager = false;

    
    //---------------------------------------------------
    // 
    ofSetFrameRate(1000);  // as fast as possible
    ofSetVerticalSync(false);
    
    
    ofBackground(0,0,0);

    //---------------------------------------------------
    blender.setup(PROJECTOR_WIDTH, PROJECTOR_HEIGHT, PROJECTOR_COUNT, PIXEL_OVERLAP);
	blender.gamma = 2.45;
    blender.gamma2 = 2.45;
	blender.blendPower = 1;
	blender.luminance = 0;
    
    
    _mapping = new mtl2dMapping();
    _mapping->init(blender.getDisplayWidth(), blender.getDisplayHeight());
    
    
    grabScreen.allocate(768 * (3.0/8.0), 768, GL_RGBA);
    
    blah.allocate(768, 2048);
    
}

void testApp::exit(){
    soundStream.close();
    ofSleepMillis(100);
}



//--------------------------------------------------------------
void testApp::update(){

    
#ifdef USE_MUTEX_SYSTEM
// this is disabled for now.  not really sure
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
#endif
    
    
    ofClear(0,0,0,255);
    scenes[0]->update();
    
    _mapping->update();

    
}



//--------------------------------------------------------------
void testApp::draw(){
    
    ofBackground(0);
    
    if (ofGetFrameNum() < 10) return;
    
    if (bDrawAudioManager) AM.draw();
    
    
    blah.begin();
    ofClear(0,0,0,255);
    
    ofPushMatrix();
    
    //ofTranslate(-mouseX, -mouseY);
    scenes[0]->draw(); 
    
    ofPopMatrix();
    
	ofClearAlpha();
    blah.end();
    
    ofSetColor(255,255,255);
    blah.draw(500, 800,300, -800);
    
    
    scenes[0]->drawGui();
    
    
    ///grabScreen.loadScreenData(300, 0, 768*(3.0/8.0), 768);
    
    //ofBackground(127,127,127);

    
    //cout << _mapping->_fbo.getWidth() << endl;
    // ----
    _mapping->bind();
    //_mapping->chessBoard(40);
    
    ofSetColor(255,255,255);
    ofRotate(-90, 0,0,1);
    ofTranslate(-768,0);
	
	
    blah.draw(0, 2048,768,-2048);
	
    _mapping->unbind();
    
       
    
   
    ofSetBackgroundAuto(false);
  
    //call blender.begin() to draw onto the blendable canvas
	blender.begin();
	ofClear(0,0,0,255);
	//light gray backaground
	ofSetColor(0, 0, 0);
	//ofRect(0, 0, blender.getCanvasWidth(), blender.getCanvasHeight());
	
	//thick grid lines for blending
	ofSetColor(255, 255, 255);
	ofSetLineWidth(3);
	
	//vertical line
	for(int i = 0; i <= blender.getCanvasWidth(); i+=30){
		ofLine(i, 0, i, blender.getCanvasHeight());
	}
	
	//horizontal lines
	for(int j = 0; j <= blender.getCanvasHeight(); j+= 30){
		ofLine(0, j, blender.getCanvasWidth(), j);		
	}
	
//	//instructions
//	ofSetColor(255, 255, 255);
//	ofRect(10, 10, 300, 100);
//	ofSetColor(0, 0, 0);
//	ofDrawBitmapString("SPACE - toggle show blend\n[g/G] - adjust gamma\n[p/P] - adjust blend power\n[l/L] adjust luminance", 15, 35);
    
    
    float scale = 2048.0f / 1024.0;
    ofPushMatrix();
    ofScale(scale, scale);
    _mapping->drawOutput();
    ofPopMatrix();

    ofSetColor(255,255,255);
    //_mapping->drawFbo();
	
	//call when you are finished drawing
	blender.end();
	
	//this draws to the main window
    
    ofPushMatrix();
    
    
    scale = 1024.0  / 2048.0f;
    
    ofScale(scale, scale);
	//blender.draw();
    ofPopMatrix();
    
    
    ofPushMatrix();
    ofTranslate(1024, 0);
    blender.draw();
    ofPopMatrix();
    
    
    
   
    
    if (ControlsMapping::controlsMapping()->mappingMode() == MAPPING_MODE_INPUT) {
        ofPushMatrix();
        ofScale(0.5, 0.5);
    }
    //-------- mapping of the towers/shapes
    _mapping->draw();
    //_mapping->drawFbo();
    
    if (ControlsMapping::controlsMapping()->mappingMode() == MAPPING_MODE_INPUT) {
        ofPopMatrix();
       
    }
    
    // Draw some instructions.
    ofSetColor(0);
    ofDrawBitmapString("'m' open the mapping controls.\n", 20, 20);
    
    ofDrawBitmapStringHighlight(ofToString(ofGetFrameRate()), ofGetWidth()-100, 100);
   

    ofDrawBitmapStringHighlight(ofToString(ofGetFrameRate()), ofGetWidth()-100, 100);

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

    
//    if (key == 'S'){
//        
//        ((paperScene *)scenes[0])->gui.saveToFile("settings/paperSettings.xml");
//    }
    
    
    _mapping->keyPressed(key);
    
//    if (key == 'l'){
//        ofSetWindowShape(1400, (3/8.0) * 1400);
//    }
    
    if (key == 's'){
        ASL.bPlaying = true;
        ASL.playbackCount = 0;
    }
    
    if (key == 'a'){
        bDrawAudioManager = !bDrawAudioManager;
    }
    
    
    
    //hit spacebar to toggle the blending strip
	if(key == ' '){
		//toggling this variable effects whether the blend strip is shown
		blender.showBlend = !blender.showBlend;
	}
	
	// more info here on what these variables do
	// http://local.wasp.uwa.edu.au/~pbourke/texture_colour/edgeblend/
	
	else if(key == 'g'){
		blender.gamma  -= .05;
		blender.gamma2 -= .05;
	}
	else if(key == 'G'){
		blender.gamma  += .05;
		blender.gamma2 += .05;
	}
	else if(key == 'l'){
		blender.luminance  -= .05;
		blender.luminance2 -= .05;
	}
	else if(key == 'L'){
		blender.luminance  += .05;
		blender.luminance2 += .05;
	}
	else if(key == 'p'){
		blender.blendPower  -= .05;
		blender.blendPower2 -= .05;
	}
	else if(key == 'P'){
		blender.blendPower  += .05;
		blender.blendPower2 += .05;
	}

    
    //cout << blender.gamma << endl;
    
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    
}


ofxMSAInteractiveObjectMod test;
//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    
    
    float scale = 1.0;
    if (ControlsMapping::controlsMapping()->mappingMode() == MAPPING_MODE_INPUT) scale = 2.0;
    
    ofMouseEventArgs mouseEventArgs;
    mouseEventArgs.x = x * scale;
    mouseEventArgs.y = y * scale;
    //mouseEventArgs.button = button;
    ofNotifyEvent( mtlScaleOption::instance().ofEvents().mouseMoved, mouseEventArgs );
}

//
//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

    float scale = 1.0;
    if (ControlsMapping::controlsMapping()->mappingMode() == MAPPING_MODE_INPUT) scale = 2.0;
    
    //blender.setup(PROJECTOR_WIDTH, PROJECTOR_HEIGHT, PROJECTOR_COUNT, x);
	ofMouseEventArgs mouseEventArgs;
    mouseEventArgs.x = x * scale;
    mouseEventArgs.y = y * scale;
    mouseEventArgs.button = button;
    ofNotifyEvent( mtlScaleOption::instance().ofEvents().mouseDragged, mouseEventArgs );
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    //_mapping->mousePressed(x, y, button);
    
    float scale = 1.0;
    if (ControlsMapping::controlsMapping()->mappingMode() == MAPPING_MODE_INPUT) scale = 2.0;
    
    // ok this is VERY ghetto: 
    ofMouseEventArgs mouseEventArgs;
    mouseEventArgs.x = x * scale;
    mouseEventArgs.y = y * scale;
    mouseEventArgs.button = button;
    ofNotifyEvent( mtlScaleOption::instance().ofEvents().mousePressed, mouseEventArgs );
    
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    
    float scale = 1.0;
    if (ControlsMapping::controlsMapping()->mappingMode() == MAPPING_MODE_INPUT) scale = 2.0;
    
    ofMouseEventArgs mouseEventArgs;
    mouseEventArgs.x = x * scale;
    mouseEventArgs.y = y * scale;
    mouseEventArgs.button = button;
    ofNotifyEvent( mtlScaleOption::instance().ofEvents().mouseReleased, mouseEventArgs );
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
                output[i*2] = 0; //left[i];  // ouput = input
                output[i*2 + 1] = 0; //left[i];
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
    
    
#ifdef USE_MUTEX_SYSTEM
//    mutex.lock();
//        if (nBuffersRecorded < 100){
//            memcpy( audioDataThread + nBuffersRecorded * bufferSize, left, bufferSize * sizeof(float));
//            nBuffersRecorded++;
//        } else {
//            printf("skip! ?\n");
//        }
//        mutex.unlock();
//    
#endif    
    
    
    AM.update(left, 256);
      
}