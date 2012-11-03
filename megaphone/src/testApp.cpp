#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){


    soundStream.listDevices();
    
	int bufferSize = 256;
    
	left = new float[bufferSize];
    right = new float[bufferSize];
    
    
    AM.setup();

    

    total.addVertex(ofPoint(ofGetWidth()/2, ofGetWidth()/2));

    
    
    float xPos = 200;
    float lastWidth = 0;
    for (int i = 0; i < 200; i++){
        Glyph * g = new Glyph();
        g->setScale(10);
        
        
        float curWidth = g->absBounds().width;
        
        xPos += (curWidth*10)/2 + (lastWidth*10)/2;
        //cout << " i " << i << " " << xPos - 200 << endl;
        g->pos().set(xPos, 400);
        glyphs.push_back(g);
        
        lastWidth = g->absBounds().width;
        //xPos += g->absBounds().width + 4;
    }
    
    
    
    nBuffersRecorded = 0;
    audioDataThread = new float[ bufferSize * 100]; // almost half a second
    audioDataMainThread = new float [ bufferSize * 100];
        
    
    soundStream.setup(this, 0, 2, 44100, bufferSize, 4);
    
    
}

void testApp::exit(){
    soundStream.close();
    ofSleepMillis(100);
}



float distanceAdder = 0;
float distanceAdderPct = 0;
//--------------------------------------------------------------
void testApp::update(){

    
    
    int nFramesAudio = 0; 
    mutex.lock();
    memcpy( audioDataMainThread, audioDataThread, nBuffersRecorded * 256 * sizeof(float));
    nFramesAudio = nBuffersRecorded;
    nBuffersRecorded = 0;
    mutex.unlock();
    
    for (int i = 0; i < nFramesAudio; i++){
        AM.update(audioDataMainThread + i * 256, 256);
        pitch = AM.results.aubioPitch;
        volume = AM.results.aubioRMS;
    }
    
    //    AM.update(left, bufferSize);
    //    pitch = AM.results.aubioPitch;
    //    volume = AM.results.aubioRMS;
    
    
    
   
    bool bLoudEnoughLastFrame = bLoudEnough;
    float pitchSmoothLastFrame = pitchSmooth;
    
    if (volume > 0.005){
        bLoudEnough = true;
    } else {
        bLoudEnough = false;
    }
    
    
    
    if (bLoudEnough){
        
        //cout << log2(pitch/440.0 + 1) * 100.0 << endl;
        
        // if we were not loud enough last frame, and loud enough this frame, don't smooth: 
        if (!bLoudEnoughLastFrame){
            pitchSmooth = log2(pitch/440.0 + 1) * 100.0;
        } else {
            pitchSmooth = 0.97f * pitchSmooth + 0.03 * log2(pitch/440.0 + 1) * 100.0;
        
            float diffPitch = pitchSmooth - pitchSmoothLastFrame;
            float angleMap = ofMap(diffPitch, -20,20, PI, -PI, false);
            float distance = ofMap(volume, 0.001, 0.04, 0.4, 6.5, true) * 2.0;
            angleDiffs.push_back(angleMap);
            distances.push_back(distance);
            
            if (angleDiffs.size() > 30){
                angleDiffs.erase(angleDiffs.begin());
                distances.erase(distances.begin());
                
            }
            
            
            distanceAdder += distance;
            
            distanceAdderPct = ofClamp(distanceAdder/20.0, 0,1);
//            if (distanceAdder > 20){
//                distanceAdder = 0;
//                glyphs.erase(glyphs.begin());
//                Glyph * g = new Glyph();
//                g->setScale(10);
//                g->pos().set(0,0);
//                glyphs.push_back(g);
//            }
            
            
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
    
    if (total.size() > 600){
        //total.getVertices().erase(total.getVertices().begin());
    }
    
    
    ofPolyline spline = total;
    
    if (spline.size() > 0) {
        vector<ofPoint>& points = spline.getVertices();
        float totalLength = spline.getPerimeter();
        float currLength = 0;
        float currPct = 0 - 0.05 * distanceAdderPct;
        
        for (int i = 0; i < glyphs.size(); i++) {
            
            glyphs[i]->pos().set(0,0);
            // get the point on the spline
            
            //cout << i << " " << currLength << endl;
            
            int indexPct = -1;
            float len = 0; 
            float indexPctA, indexPctB;
            
            for (int j = 1; j < spline.size(); j++){
                float pctA = (float) len / totalLength;
                
                len += (spline[j-1] - spline[j]).length();
                
                float pctB = (float) len / totalLength;
                
                
                if (currPct >= pctA && currPct <= pctB){
                    indexPct = j - 1;
                    indexPctA = pctA;
                    indexPctB = pctB;
                }
            }
            
            
            //float tn = (1.0 - currPct) * (points.size() - 1);
            ofVec2f prevPoint = points[(int)ofClamp(indexPct, 0, points.size() - 1)];
            ofVec2f nextPoint = points[(int)ofClamp(indexPct + 1, 0, points.size() - 1)];
            
            ofVec2f afterPoint = points[(int)ofClamp(indexPct + 2, 0, points.size() - 1)];
            float subt = (indexPctB -  currPct)/(indexPctB - indexPctA); 
            ofPoint currPoint = subt * prevPoint + (1-subt) * nextPoint;
            
            glyphs[i]->pos().set(currPoint);
            float angle = atan2(afterPoint.y - prevPoint.y, afterPoint.x - prevPoint.x);
            
            
            float angleDiff = angle - angleSmooth;
            if (angleDiff < -PI) angleDiff += TWO_PI;
            if (angleDiff > PI) angleDiff -= TWO_PI;
            
            angleSmooth += 0.62f * angleDiff;
            
            
            glyphs[i]->setRotation(angleSmooth * RAD_TO_DEG);
            
            glyphs[i]->update();
            
            
            
            currLength += (glyphs[i]->absBounds().width/2 * 10);
            if (i < glyphs.size() - 1) {
                currLength += (glyphs[i+1]->absBounds().width / 2)* 10;
            }
            
            if (i == 0){
                currLength -=  (glyphs[i]->absBounds().width * 10) * distanceAdderPct;
            }
            
            currLength += 5;
            
            
            currPct = currLength / totalLength;
            if (currPct > 1) break;
        }
        
        
    }
    

}



//--------------------------------------------------------------
void testApp::draw(){
    
    if (ofGetFrameNum() < 10) return;
    
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
    
    AM.volume.draw();
    AM.onset.draw();

    if (total.getVertices().size() > 0){
        
        
        ofPushMatrix();
        ofTranslate(-catchPoint.x + ofGetWidth()/2, -catchPoint.y + ofGetHeight()/2);
        
        
        glBegin(GL_LINE_STRIP);
        for (int i = 0; i < total.getVertices().size(); i++){
            glVertex3f(total.getVertices()[i].x, total.getVertices()[i].y, total.getVertices()[i].z);
        }
        glEnd();
        //ofEndShape();
        
        //total.draw();
        ofPopMatrix();

    }
    
    
    ofPushMatrix();
    ofTranslate(-catchPoint.x + ofGetWidth()/2, -catchPoint.y + ofGetHeight()/2);
    
    for (int i = 0; i < glyphs.size(); i++){
        glyphs[i]->draw();
        //glyphs[i]->debug();
        
    }
    
    ofPopMatrix();
    
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

    total.clear();
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

    mutex.lock();
    if (nBuffersRecorded < 100){
        memcpy( audioDataThread + nBuffersRecorded * bufferSize, left, bufferSize * sizeof(float));
        nBuffersRecorded++;
    } else {
        printf("skip! ?\n");
    }
    
    mutex.unlock();
    
    
    
    //    AM.update(left, bufferSize);
//    pitch = AM.results.aubioPitch;
//    volume = AM.results.aubioRMS;
    
}