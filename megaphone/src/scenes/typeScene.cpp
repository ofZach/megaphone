


#include "typeScene.h"
#include "ofMain.h"
#include "glyphShapes.h"



typedef struct {
    ofPoint point;
    float angle;
} pointAngleOnLine;

pointAngleOnLine ptOnPolyline( float distance, ofPolyline & line){
    
    pointAngleOnLine PAOL;
    
    float totalLength = (float)line.getPerimeter();
    float pct = distance / totalLength;
    float len = 0;
    int indexPct = -1;
    float indexPctA = -1;
    float indexPctB = -1;
    
    for (int j = 1; j < line.size(); j++){
        float pctA = (float) len / totalLength;
        len += (line[j-1] - line[j]).length();
        float pctB = (float) len / totalLength;
        if (pct >= pctA && pct <= pctB){
            indexPct = j - 1;
            indexPctA = pctA;
            indexPctB = pctB;
        }
    }
    
    ofVec2f prevPoint = line[(int)ofClamp(indexPct, 0, line.size() - 1)];
    ofVec2f nextPoint = line[(int)ofClamp(indexPct + 1, 0, line.size() - 1)];
    ofVec2f afterPoint = line[(int)ofClamp(indexPct + 2, 0, line.size() - 1)];
    float subt = (indexPctB -  pct)/(indexPctB - pct); 
    ofPoint currPoint = subt * prevPoint + (1-subt) * nextPoint;
    float angle = atan2(afterPoint.y - prevPoint.y, afterPoint.x - prevPoint.x);

    PAOL.point = currPoint;
    PAOL.angle = angle;
    
    return PAOL;
    
}


void typeScene::setup(){
    
    angle = 0;
    total.addVertex(ofPoint(ofGetWidth()/2, ofGetWidth()/2));
    bSilence= false;
    GS.buildGlyphLimbs();

    
}





void typeScene::update(){
    
    
    //get volume and pitch from the results. 
    volume = results->aubioRMS;
    pitch = results->aubioPitch;
    
    
    
    bool bLoudEnoughLastFrame = bLoudEnough;
    float pitchSmoothLastFrame = pitchSmooth;
    
    if (volume > 0.005){
        
        if (bLoudEnoughLastFrame == false){
            if (total.size()> 2){
                
                total.getVertices().erase(total.getVertices().begin(), total.getVertices().end()-2);
                glyphDistance = 0;
                //total.erase(total.begin(), total.end()-2);
                
                //blah = total;
            }
        }
        
        bLoudEnough = true;
        bSilence = false;
        
        
    } else {
        bLoudEnough = false;
        
        if (bLoudEnoughLastFrame == true){
            
            bSilence = true;
            
            while (angle < -PI) angle += TWO_PI;
            while (angle > PI) angle -= TWO_PI;

            if (total.size()> 2){
                glyphDistance = 0;
                if (currentBlob.particles.size() > 2){
                    blobs.push_back(currentBlob);
                    blobs.back().setupSprings();
                    blobs.back().originalSize= total.getPerimeter();
                }
                total.getVertices().erase(total.getVertices().begin(), total.getVertices().end()-2);
            }
            //total.clear();
            currentBlob.particles.clear();
            currentBlob.glyphs.clear();
            //total.addVertex(ofPoint(ofGetWidth()/2, ofGetWidth()/2));
            
            glyphs.clear();
            
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
        
        
        
        
        ofPoint lastVertex = total.getVertices()[ total.getVertices().size() - 1 ];
        ofPoint newVeretx = lastVertex + ofPoint( cos(angle) * dist, sin(angle) * dist);
        angleDiffs.erase(angleDiffs.begin());
        total.addVertex(newVeretx.x, newVeretx.y);
        distances.erase(distances.begin());
        
    }
    if (total.getVertices().size() > 0){
        catchPoint = 0.9f * catchPoint + 0.1f * total.getVertices()[total.getVertices().size()-1];
    }
    
  
    
    float totalLength = total.getPerimeter();
    
    if (glyphDistance < totalLength && total.size() > 2 && !bSilence){
        
        Glyph g;
        g.GS = &GS;
        g.setup();
        g.setScale(5 + 20 * ofMap(volume,0,0.3,0,1, true));
        
        float wAdder = 0;
        if (glyphs.size() > 0){
            wAdder += (glyphs[glyphs.size()-1].bounds().width / 2) * glyphs[glyphs.size()-1].scale();
        }
        
        float w = (g.bounds().width / 2) * g.scale();
   
        
        if ((glyphDistance + wAdder + w) < totalLength){
        
                pointAngleOnLine PAOL = ptOnPolyline(glyphDistance + wAdder + w, total);
                ofPoint pt = PAOL.point;
                g.pos().set(pt);
                g.setRotation(PAOL.angle * RAD_TO_DEG);
                glyphs.push_back(g);
                glyphDistance += (wAdder + w);
                particle * p = new particle();
                p->pos = pt;
                currentBlob.particles.push_back( p);
                currentBlob.glyphs.push_back(g);
            
        }
    
            
    }
    
    
    
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
            blobs[i].particles[j]->addAttractionForce(catchPoint.x, catchPoint.y, 20000, 0.1);
            blobs[i].particles[j]->addRepulsionForce(catchPoint.x, catchPoint.y, 200, 0.4);
            blobs[i].particles[j]->addClockwiseForce(catchPoint.x, catchPoint.y,600, 0.1);
        }
    }

    
    for (int i = 0; i < blobs.size(); i++){
        blobs[i].update();
    }
}


void typeScene::draw(){
    
    if (total.getVertices().size() > 0){
        ofPushMatrix();
        ofTranslate(-catchPoint.x + ofGetWidth()/2, -catchPoint.y + ofGetHeight()/2);        
        ofPopMatrix();
    }
    
    ofPushMatrix();
    ofTranslate(-catchPoint.x + ofGetWidth()/2, -catchPoint.y + ofGetHeight()/2);
    
    
    for (int i = 0; i < glyphs.size(); i++){
        
        
        //cout << glyphs[i].id << " --- XXx " << glyphs[i]._limbs.size() << endl;
        
        //cout << glyphs[i].id << " --- " << endl;
        glyphs[i].draw();
    }
    
    
   
    
	ofSetHexColor(0xffffff);
	
	for (int i = 0; i < blobs.size(); i++){
        blobs[i].draw();
    }
    ofPopMatrix();

    
    
}