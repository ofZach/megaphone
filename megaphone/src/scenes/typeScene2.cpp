


#include "typeScene2.h"
#include "ofMain.h"
#include "glyphShapes.h"



ofPoint ptMe;


void typeScene2::setup(){
    angle = 0;
    bSilence= false;
    
    for (int i = 0; i < 10; i++){
        blob temp;
        blobs.push_back(temp);
        blobs[blobs.size()-1].setup();
    }
    
    blobs.back().particles.clear();
    blobs.back().springs.clear();
}




void typeScene2::update(){
    
    
    //get volume and pitch from the results. 
    volume = results->aubioRMS;
    pitch = results->aubioPitch;
    
    bool bLoudEnoughLastFrame = bLoudEnough;
    float pitchSmoothLastFrame = pitchSmooth;
    
    if (volume > 0.005){
        
        if (bLoudEnoughLastFrame == false){
            if (total.size()> 2){
                total.getVertices().erase(total.getVertices().begin(), total.getVertices().end()-2);
            }
        }
        
        bLoudEnough = true;
        bSilence = false;
        
        
    } else {
        bLoudEnough = false;
        
        if (bLoudEnoughLastFrame == true){
            
            blob sdf;
            blobs.push_back(sdf);
            //blobs.back().particles.clear();
            //blobs.back().springs.clear();
            
            ptMe.set(600,600);
            
            bSilence = true;
//            
//            while (angle < -PI) angle += TWO_PI;
//            while (angle > PI) angle -= TWO_PI;
//            
//            
//            
//            
//            if (total.size()> 2){
//            
//                glyphDistance = 0;
//                
//                if (currentBlob.particles.size() > 2){
//                    blobs.push_back(currentBlob);
//                    blobs.back().setupSprings();
//                    blobs.back().originalSize= total.getPerimeter();
//                }
//                total.getVertices().erase(total.getVertices().begin(), total.getVertices().end()-2);
//                
//            }
//            //total.clear();
//            currentBlob.particles.clear();
            
            
        }
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
            
        }
    }
    
//    if (bSilence == true){
//        float angleMap = ofMap( ofNoise(ofGetElapsedTimef()/100) * 20, 0,20, PI * 0.02, -PI * 0.02, true);
//        float distance = 1.8;
//        
//        
//        angleDiffs.push_back(angleMap);
//        distances.push_back(distance);
//    }
    
    
    
    
    
    if (angleDiffs.size() > 0){
        
        angle += angleDiffs[0];
        float dist = distances[0];
        
        
        particle * temp = new particle();
        temp->pos = ofPoint(ptMe.x, ptMe.y);
        temp->vel = ofPoint(0,0); //ofPoint ( cos(angle) * dist, sin(angle) * dist);
     
        if (blobs.back().particles.size() > 0){
            particle * me = blobs.back().particles.back();
            spring s;
            s.particleA = me;
            s.particleB = temp;
            s.distance = dist;
            s.springiness = 0.2;
            blobs.back().springs.push_back(s);
        }
        blobs.back().particles.push_back(temp);
    

        ptMe += ofPoint( cos(angle) * dist*0.2, sin(angle) * dist*0.2);
        
        
        //ofPoint lastVertex = total.getVertices()[ total.getVertices().size() - 1 ];
        //ofPoint newVeretx = lastVertex + ofPoint( cos(angle) * dist, sin(angle) * dist);
        angleDiffs.erase(angleDiffs.begin());
        //total.addVertex(newVeretx.x, newVeretx.y);
        distances.erase(distances.begin());
        
    }
//    if (total.getVertices().size() > 0){
//        catchPoint = 0.9f * catchPoint + 0.1f * total.getVertices()[total.getVertices().size()-1];
//    }
    
  
    
    
    
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
//            blobs[i].particles[j]->addAttractionForce(catchPoint.x, catchPoint.y, 20000, 0.1);
//            blobs[i].particles[j]->addRepulsionForce(catchPoint.x, catchPoint.y, 200, 0.4);
//            blobs[i].particles[j]->addClockwiseForce(catchPoint.x, catchPoint.y,600, 0.1);
//            
            blobs[i].particles[j]->bounceOffWalls();
            
            
            
        }
    }

    
    for (int i = 0; i < blobs.size(); i++){
        blobs[i].update();
    }
}


void typeScene2::draw(){
    
   
    ofRect(0,0,100,100);
   
    
	ofSetHexColor(0xffffff);
	
	for (int i = 0; i < blobs.size(); i++){
        blobs[i].draw();
    }
    ofPopMatrix();

    
    
}