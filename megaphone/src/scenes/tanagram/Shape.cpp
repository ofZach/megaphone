//
//  Shape.cpp
//  glyphsOnSpline
//
//  Created by Patricio Gonzalez Vivo on 11/1/12.
//
//

#include "Shape.h"

Shape::Shape(){
    size = 20;
    damping = 0.05f;
    
    actual.rot.set(0.0,0.0,0.0);
    target.rot.set(0.0,0.0,0.0);
    
    bDebug = NULL;
    bTrail = NULL;
	
	noiseSeedA = ofRandom(0,0.55);
	noiseSeedB = ofRandom(0,0.55);
	
	lockedAmount = 0;
}

void Shape::update(float _posLerp, float _rotLerp){
    
	
	float lockAmount = ofMap(ofGetMouseX(), 0, ofGetWidth(), 0, 1);
	
	noiseSeedA += 0.005;
	noiseSeedB += 0.005;
	
	float noiseRotationX = ofNoise(noiseSeedA, noiseSeedB, ofGetElapsedTimef()/100.0);
	float noiseRotationY = ofNoise(noiseSeedA, ofGetElapsedTimef()/100.0, noiseSeedB);
	float noiseRotationZ = ofNoise(ofGetElapsedTimef()/100.0, noiseSeedB, noiseSeedA);

	
	float rotationLockAmount = ofMap(lockAmount, 0,1, 0, 0.9, true);
    actual.rot.x = (rotationLockAmount) * noiseRotationX * 360 + (1-rotationLockAmount) * ofLerp(actual.rot.x, target.rot.x, _rotLerp);
    actual.rot.y = (rotationLockAmount) * noiseRotationY * 360 + (1-rotationLockAmount) * ofLerp(actual.rot.y, target.rot.y, _rotLerp);
    actual.rot.z = (rotationLockAmount) * noiseRotationZ * 360 + (1-rotationLockAmount) * ofLerp(actual.rot.z, target.rot.z, _rotLerp);

	
	float noisePositionX = ofSignedNoise(noiseSeedA, noiseSeedB, -ofGetElapsedTimef()/100.0);
	float noisePositionY = ofSignedNoise(noiseSeedA, -ofGetElapsedTimef()/100.0, noiseSeedB);
	float noisePositionZ = ofSignedNoise(-ofGetElapsedTimef()/100.0, noiseSeedB, noiseSeedA);
	
	actual.pos.x = (lockAmount) * noisePositionX * 200 + (1-lockAmount) * ofLerp(actual.pos.x,target.pos.x,_posLerp);
	actual.pos.y = (lockAmount) * noisePositionY * 200 + (1-lockAmount) * ofLerp(actual.pos.y,target.pos.y,_posLerp);
	actual.pos.z = (lockAmount) * noisePositionZ * 200 + (1-lockAmount) *  ofLerp(actual.pos.z,target.pos.z,_posLerp);
	
   // if ( getDistanceToTarget() > 10) {
//        //addAttractionForce(target.pos, 1000, 0.3);
//    } else {
//        actual.pos.x = noisePositionX; //ofLerp(actual.pos.x,target.pos.x,_posLerp);
//        actual.pos.y = noisePositionY;// ofLerp(actual.pos.y,target.pos.y,_posLerp);
//        actual.pos.z = noisePositionZ; // ofLerp(actual.pos.z,target.pos.z,_posLerp);
//    }
    
    vel += acc;
    vel *= 1.0f - damping;
    actual.pos += vel;
    acc *= 0;
    
    if (bTrail != NULL){
        if (*bTrail){
            trail.push_back(actual);
            while(trail.size() > 10){
                trail.erase(trail.begin());
            }
        }
    }
}




void Shape::draw(){
    //  Make the shape
    //
	
	float lockAmount = ofMap(ofGetMouseX(), 0, ofGetWidth(), 0, 1);

	
    ofMesh mesh;
    mesh.setMode(OF_PRIMITIVE_TRIANGLES);
    for (int i = 0; i < getVertices().size(); i++) {
        mesh.addVertex(getVertices()[i]);
    }
    
    //  Draw Trail
    //
    if (bTrail != NULL){
        if (*bTrail){
            for (int j = 0; j < trail.size(); j++) {
                ofPushMatrix();
                ofTranslate(trail[j].pos);
                ofRotateY(trail[j].rot.y);
                ofRotateX(trail[j].rot.x);
                ofRotateZ(trail[j].rot.z);
                
                ofFill();
                ofColor trailColor = color;
                float pct = trailColor.getSaturation()*((float)j/(float)trail.size());
                //            trailColor.setSaturation( pct );
                trailColor.a = pct;
                ofSetColor(trailColor);
                mesh.draw();
                ofPopMatrix();
            }
        }
    }
    
    
    //  Draw Shape
    //
    ofPushStyle();
    ofPushMatrix();
    ofTranslate(actual.pos);
    ofRotateY(actual.rot.y);
    ofRotateX(actual.rot.x);
    ofRotateZ(actual.rot.z);
    
    ofFill();
    ofSetColor(color);
	
	
	
	

    mesh.draw();
    ofPopMatrix();
	
	
	nTrailLines = 8;
	if (trailLines.size() != nTrailLines){
		for (int i = 0; i < nTrailLines; i++){
			trailLines.push_back( ofPolyline());
		}
	} else {
		
		ofMatrix4x4 start;
		ofMatrix4x4 R1 = ofMatrix4x4::newRotationMatrix(actual.rot.y,0,1,0);
		ofMatrix4x4 R2 = ofMatrix4x4::newRotationMatrix(actual.rot.x,1,0,0);
		ofMatrix4x4 R3 = ofMatrix4x4::newRotationMatrix(actual.rot.z,0,0,1);
		ofMatrix4x4 T = ofMatrix4x4::newTranslationMatrix(actual.pos);
		
		
		for (int i = 0; i < nTrailLines; i++){
			float t = ofMap(i, 0, nTrailLines-1, 0, getVertices().size()-1);
			int who = (int)t % getVertices().size();
			int whoP1 = (int)(t+1) % getVertices().size();
			float diff = t - (int)t;
			ofPoint pt(0,0);
			
			//cout << who << " " << diff << " " << whoP1 << " " <<  getVertices().size() <<  endl;
			
			pt =  getVertices()[who] * (1-diff) + (diff) * getVertices()[whoP1];
			ofVec3f c = pt*R3*R2*R1*T;
			trailLines[i].addVertex(c);
			
			
			ofMesh mesh;
			mesh.setMode(OF_PRIMITIVE_LINE_STRIP);
			
			for (int j = 0; j < trailLines[i].getVertices().size(); j++){
				float alpha = ofMap(j, 0, trailLines[i].getVertices().size()-1, 0, 255, true);
				//cout << j << " " << alpha << endl;
				mesh.addColor( ofColor(255,255,255,1.0*alpha * lockAmount));
				mesh.addVertex(trailLines[i].getVertices()[j]);
				
			}
			mesh.draw();
			
			if (trailLines[i].getVertices().size() > 100) trailLines[i].getVertices().erase(trailLines[i].getVertices().begin());
			
			
		}
		
	}
	
	//int nTrailLines;
	//vector < ofPolyline > trailLines;
	
	
	
	
	

    
    if(bDebug){
        ofPushMatrix();
        ofTranslate(target.pos);
        ofSetColor(255);
        ofLine(-5,0,5,0);
        ofLine(0, -5, 0, 5);
        ofLine(0, 0, -5, 0, 0, 5);
        
        ofRotateY(target.rot.y);
        ofRotateX(target.rot.x);
        ofRotateZ(target.rot.z);
        
        ofEnableSmoothing();
//        mesh.drawWireframe();
        ofNoFill();
        ofBeginShape();
        for (int i = 0; i < getVertices().size(); i++) {
            ofVertex(getVertices()[i]);
        }
        ofEndShape(true);
        
        ofPopMatrix();
        
        ofPushMatrix();
        ofTranslate(actual.pos);
        
        ofFill();
        ofSetColor(255,100);
        ofSphere(0,0,0, 7);
//        ofCircle(0,0, 7);
//        
//        ofDrawBitmapString("x: "+ofToString(target.rot.x), 15,-15);
//        ofDrawBitmapString("y: "+ofToString(target.rot.y), 15,0);
//        ofDrawBitmapString("z: "+ofToString(target.rot.z), 15,15);
        
        ofPopMatrix();
        ofDisableSmoothing();
    }
    
    ofPopStyle();
}

//------------------------------------------------- SETUP
void Shape::makeNumber( int _num, float _scale ){
    clear();
    
    actual.pos.set(0,0);
    target.pos.set(0,0);
    
    //  Choose of Shapes
    //
    switch (_num) {
        case 0:
            addVertex(ofVec3f(0, 0));
            addVertex(ofVec3f(2, 2));
            addVertex(ofVec3f(0, 4));
            break;
        case 1:
            addVertex(ofVec3f(0, 0));
            addVertex(ofVec3f(4, 0));
            addVertex(ofVec3f(2, 2));
            break;
        case 2:
            addVertex(ofVec3f(4, 0));
            addVertex(ofVec3f(4, 2));
            addVertex(ofVec3f(3, 1));
            break;
        case 3:
            addVertex(ofVec3f(2, 2));
            addVertex(ofVec3f(3, 1));
            addVertex(ofVec3f(4, 2));
            addVertex(ofVec3f(4, 2));
            addVertex(ofVec3f(3, 3));
            addVertex(ofVec3f(2, 2));
            break;
        case 4:
            addVertex(ofVec3f(1, 3));
            addVertex(ofVec3f(2, 2));
            addVertex(ofVec3f(3, 3));
            break;
        case 5:
            addVertex(ofVec3f(2, 4));
            addVertex(ofVec3f(0, 4));
            addVertex(ofVec3f(1, 3));
            addVertex(ofVec3f(1, 3));
            addVertex(ofVec3f(3, 3));
            addVertex(ofVec3f(2, 4));
            break;
        case 6:
            addVertex(ofVec3f(4, 2));
            addVertex(ofVec3f(4, 4));
            addVertex(ofVec3f(2, 4));
            break;
    }
    close();
    
    //  Centered
    //
    for (int i = 0; i < getVertices().size(); i++){
        getVertices()[i] -= ofPoint(2,2);
        getVertices()[i] *= _scale;
    }
    target.pos = actual.pos = this->getCentroid2D();
    for (int i = 0; i < getVertices().size(); i++){
        getVertices()[i] -= target.pos;
    }
    ofRectangle rect = getBoundingBox();
    size = MAX(rect.width,rect.height);
    
    //  Colors Palette
    //
    vector<ofColor> colors;
    colors.push_back(ofColor(255,255,255));
    color = colors[ _num%colors.size() ];
}

void Shape::makeFromLimb(int _limbNum, Glyph &_glyph ){
    
    Limb limb = _glyph.limbs()[_limbNum];
    for (int i = 0; i < limb.coords().size(); i++){
        ofPoint coord = limb.coords()[i];
        coord *= _glyph.scale();
        this->addVertex(coord);
    }
    close();
    
    target.pos = actual.pos = this->getCentroid2D();
    for (int i = 0; i < getVertices().size(); i++){
        getVertices()[i] -= target.pos;
    }
    
    ofRectangle rect = limb.getBoundingBox();
    size = MAX(this->getBoundingBox().width,this->getBoundingBox().height);
    
    color = _glyph.limbs()[_limbNum].color();
}

void Shape::setTargetPos( ofPoint _pos ){
    target.pos = _pos;
};

void Shape::setTargetRot( ofPoint _rot ){
    target.rot.x = _rot.x;
    target.rot.y = _rot.y;
    target.rot.z = _rot.z;
};

//----------------------------------------------------- Questions
bool Shape::mouseHover(){
    return inside(ofGetMouseX()-ofGetWidth()*0.5-actual.pos.x,
                  ofGetMouseY()-ofGetHeight()*0.5-actual.pos.y);
}

ofPoint Shape::getTargetRot(){
    return ofPoint(target.rot.x,target.rot.y,target.rot.z);
};

ofPoint Shape::getTargetPos(){
    return target.pos;
}

float Shape::getDistanceToTarget(){
    return target.pos.distance(actual.pos);
}

//----------------------------------------------------- ACTIONS
void Shape::flipH(){
    target.rot.x += 180;
    while (target.rot.x >= 360) {
        target.rot.x -= 360;
    }
}

void Shape::flipV(){
    target.rot.y += 180;
    while (target.rot.y >= 360) {
        target.rot.y -= 360;
    }
}

void Shape::turnLeft(){
    if ( target.rot.z < 5){
        target.rot.z = 360;
    }
    
    target.rot.z -= 5;
}

void Shape::turnRight(){
    target.rot.z += 5;
    
    while (target.rot.z >= 360) {
        target.rot.z -= 360;
    }
}


void Shape::addForce(ofPoint &_force){
    acc += _force;
}
//------------------------------------------------------------ PHYSICS
void Shape::addRepulsionForce(ofPoint posOfForce, float radius, float scale){
    
	// ----------- (2) calculate the difference & length
	
	ofPoint diff	= actual.pos - posOfForce;
	float length	= diff.length();
	
	// ----------- (3) check close enough
	
	bool bAmCloseEnough = true;
    if (radius > 0){
        if (length > radius){
            bAmCloseEnough = false;
        }
    }
	
	// ----------- (4) if so, update force
    
	if (bAmCloseEnough == true){
		float pct = 1 - (length / radius);  // stronger on the inside
        diff.normalize();
        ofPoint dir = ofPoint(diff * scale * pct);
        addForce(dir);
    }
}

void Shape::addAttractionForce(ofPoint posOfForce, float radius, float scale){
	
	// ----------- (2) calculate the difference & length
	
	ofPoint diff	= actual.pos - posOfForce;
	float length	= diff.length();
	
	// ----------- (3) check close enough
	
	bool bAmCloseEnough = true;
    if (radius > 0){
        if (length > radius){
            bAmCloseEnough = false;
        }
    }
	
	// ----------- (4) if so, update force
    
	if (bAmCloseEnough == true){
		float pct = 1 - (length / radius);  // stronger on the inside
		diff.normalize();
        ofPoint dir = ofPoint(diff * scale * pct) * -1;
        addForce(dir);
    }
}

void Shape::addRepulsionForce(Shape &s, float radius, float scale){
	
	// ----------- (1) make a vector of where this particle p is:
	ofPoint posOfForce;
	posOfForce.set(s.actual.pos);
	
	// ----------- (2) calculate the difference & length
	
	ofVec2f diff	= actual.pos - posOfForce;
	float length	= diff.length();
	
	// ----------- (3) check close enough
	
	bool bAmCloseEnough = true;
    if (radius > 0){
        if (length > radius){
            bAmCloseEnough = false;
        }
    }
	
	// ----------- (4) if so, update force
    
	if (bAmCloseEnough == true){
		float pct = 1 - (length / radius);  // stronger on the inside
		diff.normalize();
        ofPoint dir = ofPoint(diff * scale * pct);
        addForce(dir);
		dir *= -1.0;
        s.addForce(dir);
    }
}

void Shape::addRepulsionForce(Shape &s, float scale){
	
	// ----------- (1) make a vector of where this particle p is:
	ofPoint posOfForce;
	posOfForce.set(s.actual.pos);
	
	// ----------- (2) calculate the difference & length
	
	ofVec2f diff	= actual.pos - posOfForce;
	float length	= diff.length();
    float radius    = size*2.f + s.size*2.f;
	
	// ----------- (3) check close enough
	
	bool bAmCloseEnough = true;
    if (radius > 0){
        if (length > radius){
            bAmCloseEnough = false;
        }
    }
	
	// ----------- (4) if so, update force
    
	if (bAmCloseEnough == true){
		float pct = 1 - (length / radius);  // stronger on the inside
		diff.normalize();
        ofPoint dir = ofPoint(diff * scale * pct);
        addForce(dir);
		dir *= -1.0;
        s.addForce(dir);
    }
}

void Shape::addAttractionForce(Shape &s, float radius, float scale){
	
	// ----------- (1) make a vector of where this particle p is:
	ofPoint posOfForce;
	posOfForce.set(s.actual.pos);
	
	// ----------- (2) calculate the difference & length
	
	ofPoint diff	= actual.pos - posOfForce;
	float length	= diff.length();
	
	// ----------- (3) check close enough
	
	bool bAmCloseEnough = true;
    if (radius > 0){
        if (length > radius){
            bAmCloseEnough = false;
        }
    }
	
	// ----------- (4) if so, update force
    
	if (bAmCloseEnough == true){
		float pct = 1 - (length / radius);  // stronger on the inside
		diff.normalize();
        ofPoint dir = ofPoint(diff * scale * pct);
        s.addForce(dir);
		dir *= -1.0;
        addForce(dir);
    }
	
}

void Shape::addClockwiseForce(Shape &s, float radius, float scale){
	
	// ----------- (1) make a vector of where this particle p is:
	ofPoint posOfForce;
	posOfForce.set(s.actual.pos);
	
	// ----------- (2) calculate the difference & length
	
	ofPoint diff	= actual.pos - posOfForce;
	float length	= diff.length();
	
	// ----------- (3) check close enough
	
	bool bAmCloseEnough = true;
    if (radius > 0){
        if (length > radius){
            bAmCloseEnough = false;
        }
    }
	
	// ----------- (4) if so, update force
    
	if (bAmCloseEnough == true){
		float pct = 1 - (length / radius);  // stronger on the inside
		diff.normalize();
        
		acc.x -= diff.y * scale * pct;
        acc.y += diff.x * scale * pct;
        
        ofPoint dir = ofPoint(diff * scale * pct);
        s.addForce(dir);
		s.acc.x += diff.y * scale * pct;
        s.acc.y -= diff.x * scale * pct;
    }
}

void Shape::addCounterClockwiseForce(Shape &s, float radius, float scale){
	
	// ----------- (1) make a vector of where this particle p is:
	ofPoint posOfForce;
	posOfForce.set(s.actual.pos);
	
	// ----------- (2) calculate the difference & length
	
	ofPoint diff	= actual.pos - posOfForce;
	float length	= diff.length();
	
	// ----------- (3) check close enough
	
	bool bAmCloseEnough = true;
    if (radius > 0){
        if (length > radius){
            bAmCloseEnough = false;
        }
    }
	
	// ----------- (4) if so, update force
    
	if (bAmCloseEnough == true){
		float pct = 1 - (length / radius);  // stronger on the inside
		diff.normalize();
		acc.x += diff.y * scale * pct;
        acc.y -= diff.x * scale * pct;
		s.acc.x -= diff.y * scale * pct;
        s.acc.y += diff.x * scale * pct;
		
    }
}

