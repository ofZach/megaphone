//
//  Page.cpp
//  FloatingPage
//
//  Created by Elie Zananiri on 2012-11-01.
//
//

#include "Page.h"

ofxToggle debugMesh;
float offsetAmount;
float twirlAmount;
float tiltAmount;
float flipAmount;
float swayAmount;
ofxToggle bendTail;
ofxToggle bendWings;
ofxToggle bendFresh;
ofxFloatSlider topBendAmount;
ofxFloatSlider bottomBendAmount;
float tornadoAmount;
float speedAmount;
float expandAmount;
float liftAmount;

float Page::sharedFlipAngle = 0;
int Page::sharedLastFlipFrame = 0;

//--------------------------------------------------------------
Page::Page()
{
    animateOffset = ofRandom(50);

    bGoingUp = true;
    startPos.set(0, 20, 0);
    pos.set(startPos);
    posInc = 0;
    posSpeed = 0.01;
    rainSpeed.set(0, -5, 0);
    tornadoAngle = ofRandom(M_TWO_PI);

    twirlAngle = 0;
    tiltAngle = 0;
    
    localFlipAngle = 0;
    flipAngle = 0;

    vertAngle = 0;
    vertOffsetY = 0;
    vertBendPct = 0;

    topBendPct = 0;
    bottomBendPct = 0;

    swayInc = 0;
    swayAngle = -M_PI/2;
    swaySpeed = 0.05;
    swayRadius = 50;

    rebuild();
}

//--------------------------------------------------------------
void Page::rebuild()
{
    path.clear();

    path.moveTo(ofPoint(-pageSize, 0, -pageSize));
    path.lineTo(ofPoint( pageSize, 0, -pageSize));
    path.lineTo(ofPoint( pageSize, 0,  pageSize));
    path.lineTo(ofPoint(-pageSize, 0,  pageSize));

    remesh();
}

//--------------------------------------------------------------
void Page::rebuild(float bendPct)
{
    path.clear();

    float bendY = pageSize * bendPct;
    float bendZ = ABS(pageSize * bendPct * 2);

    path.moveTo(ofPoint(-pageSize, 0, -pageSize));
    path.lineTo(ofPoint(pageSize, 0, -pageSize));
    path.lineTo(ofPoint(pageSize, 0, pageSize - bendZ));
    path.bezierTo(ofPoint(pageSize, 0, pageSize - bendZ * 0.5), ofPoint(pageSize, bendY * 0.5, pageSize), ofPoint(pageSize, bendY, pageSize));
    path.lineTo(ofPoint(-pageSize, bendY, pageSize));
    path.bezierTo(ofPoint(-pageSize, bendY * 0.5, pageSize), ofPoint(-pageSize, 0, pageSize - bendZ * 0.5), ofPoint(-pageSize, 0, pageSize - bendZ));
    path.lineTo(ofPoint(-pageSize, 0, -pageSize));

    remesh();
}

//--------------------------------------------------------------
void Page::rebuild(float bendTopPct, float bendBottomPct)
{
    path.clear();

    // try to keep the page area the same as it bends...

    float bendTopY = pageSize * 0.5 * bendTopPct;
    float bendTopZ = -pageSize * (1 - ABS(bendTopPct) * 0.5);
    
    float bendBottomY = pageSize * 0.5 * bendBottomPct;
    float bendBottomZ = pageSize * (1 - ABS(bendBottomPct) * 0.5);

    path.moveTo(ofPoint(-pageSize, bendTopY, bendTopZ));
    path.lineTo(ofPoint(pageSize, bendTopY, bendTopZ));

    path.bezierTo(ofPoint(pageSize, bendTopY * 0.5, bendTopZ),
                  ofPoint(pageSize, 0, bendTopZ * 0.5),
                  ofPoint(pageSize, 0, 0));
    path.bezierTo(ofPoint(pageSize, 0, bendBottomZ * 0.5),
                  ofPoint(pageSize, bendBottomY * 0.5, bendBottomZ),
                  ofPoint(pageSize, bendBottomY, bendBottomZ));

    path.lineTo(ofPoint(-pageSize, bendBottomY, bendBottomZ));

    path.bezierTo(ofPoint(-pageSize, bendBottomY * 0.5, bendBottomZ),
                  ofPoint(-pageSize, 0, bendBottomZ * 0.5),
                  ofPoint(-pageSize, 0, 0));
    path.bezierTo(ofPoint(-pageSize, 0, bendTopZ * 0.5),
                  ofPoint(-pageSize, bendTopY * 0.5, bendTopZ),
                  ofPoint(-pageSize, bendTopY, bendTopZ));

    remesh();
}

//--------------------------------------------------------------
void Page::remesh()
{
    mesh = path.getTessellation();

    // add a bunch of dummy normals
    for (int i=0; i < mesh.getNumVertices(); i++) {
        mesh.addNormal(ofVec3f());
    }

    // calculate the real normal for each vertex
    for (int i=0; i < mesh.getNumIndices(); i += 3) {
        ofVec3f a = mesh.getVertex(mesh.getIndex(i + 0));
        ofVec3f b = mesh.getVertex(mesh.getIndex(i + 1));
        ofVec3f c = mesh.getVertex(mesh.getIndex(i + 2));

        ofVec3f ab = b - a;
        ofVec3f ac = c - a;

        ofVec3f n = ab.cross(ac).normalized();  // gotta flip it, guess i'm winding backwards...
        mesh.setNormal(mesh.getIndex(i + 0), n);
        mesh.setNormal(mesh.getIndex(i + 1), n);
        mesh.setNormal(mesh.getIndex(i + 2), n);
    }
}

//--------------------------------------------------------------
void Page::update()
{
    animateCounter = (ofGetElapsedTimef() * 30.0) + animateOffset * offsetAmount;

    rotInc = sin(animateCounter * 0.1);

    // twirl and tilt
    twirlAngle = rotInc * 5 * twirlAmount;
    tiltAngle = sin(rotInc * 0.01) * 100 * tiltAmount;

    // flip around, always in the same direction
    if (flipAmount > 0) {
        localFlipAngle += MAX(rotInc, 0.02) * flipAmount;
    }
    else {
        if (localFlipAngle != 0) {
            while (localFlipAngle > M_TWO_PI) localFlipAngle -= M_TWO_PI;
            if (localFlipAngle + 0.2 < M_TWO_PI) {
                localFlipAngle += 0.2;
            }
            else {
                localFlipAngle = 0;
            }
        }
    }

    // flip the global value if it hasn't already been set this frame
    if (sharedLastFlipFrame < ofGetFrameNum()) {
        sharedFlipAngle = localFlipAngle;
        sharedLastFlipFrame = ofGetFrameNum();
    }

    // sync the flips...
    flipAngle = ofLerp(sharedFlipAngle, localFlipAngle, offsetAmount);

    // sway back and forth
    swayInc += swaySpeed;
    swayAngle = (sin(swayInc) * swayAmount) - M_PI / 2;
    swayPos.x = cos(swayAngle) * swayRadius;
    swayPos.y = sin(swayAngle) * swayRadius + swayRadius;
    swayPos *= swayAmount;

    if (bendTail) {
        // bend the bottom of the page
        vertBendPct = sin(animateCounter * 0.1) * bottomBendAmount;
    }
    else if (bendWings) {
        // bend more 
        topBendPct = sin(animateCounter * 0.1) * topBendAmount;
        bottomBendPct = sin(animateCounter * 0.1) * bottomBendAmount;
    }
    else if (bendFresh) {
        // funky fresh bending
        topBendPct = sin(animateCounter * 0.1) * topBendAmount;
        bottomBendPct = -rotInc * 2 * bottomBendAmount;
    }

    if (tornadoAmount > 0) {
        float tornadoRadius = MIN(pos.y, groundSize);
        ofVec3f posFromCenter(pos.x, 0, pos.z);
        float distFromCenter = posFromCenter.length();
        if ((int)distFromCenter < 1) {
            // move away from the center
            pos.x += cos(tornadoAngle);
            pos.z += sin(tornadoAngle);
        }
        else {
            // tornado!
            ofPoint center(0, pos.y, 0);
            tornadoOffset = pos.getPerpendicular(center);
            tornadoOffset.normalize();
            tornadoOffset *= (pos.y * 0.1);

            if (bGoingUp) {
                // ease up the angle
                tornadoOffset.y += ofMap(pos.y, startPos.y, startPos.y + 10, tornadoAngle / 10, tornadoAngle, true);
                if (tornadoRadius >= groundSize) {
                    bGoingUp = false;
                }
            }
            else {
                // ease down the angle
                tornadoOffset.y -= ofMap(pos.y, startPos.y + 10, startPos.y, tornadoAngle, tornadoAngle / 10, true);
                tornadoOffset -= (posFromCenter * 0.01);
                if (tornadoRadius < startPos.y) {
                    bGoingUp = true;
                }
            }

            tornadoOffset.y *= liftAmount;
            pos += tornadoOffset * speedAmount;
            
            ofVec3f newPosFromCenter(pos.x, 0, pos.z);
            float newDistFromCenter = newPosFromCenter.length();
            float deltaDistFromCenter = newDistFromCenter - distFromCenter;
            newPosFromCenter.scale(distFromCenter + deltaDistFromCenter * expandAmount);
            pos.x = newPosFromCenter.x;
            pos.z = newPosFromCenter.z;
        }
    }

    if (bendTail) {
        rebuild(vertBendPct);
    }
    else if (bendWings || bendFresh) {
        rebuild(topBendPct, bottomBendPct);
    }
    else {
        rebuild();
    }
}

//--------------------------------------------------------------
void Page::draw()
{
    ofPushMatrix();

    ofPoint currPos;
    currPos.x = ofLerp(startPos.x, pos.x, tornadoAmount);
    currPos.y = ofLerp(startPos.y, pos.y, tornadoAmount);
    currPos.z = ofLerp(startPos.z, pos.z, tornadoAmount);
    ofTranslate(currPos);

    // sway
    ofTranslate(swayPos);
    ofRotate(RAD_TO_DEG * (swayAngle + M_PI / 2), 0, 0, 1);

    // flip
    ofRotate(RAD_TO_DEG * flipAngle, 0, 0, 1);

    // vert bend
    ofTranslate(0, vertOffsetY);
    ofRotate(vertAngle, 1, 0, 0);

    // twirl
    ofRotate(RAD_TO_DEG * twirlAngle, 0, 1, 0);

    // tilt
    ofRotate(RAD_TO_DEG * tiltAngle, 1, 0, 1);

    if (debugMesh) {
        ofSetColor(255);
        mesh.drawWireframe();
        ofSetColor(255, 0, 0);
        for (int i=0; i < mesh.getNumIndices(); i++) {
            ofVec3f coord = mesh.getVertex(mesh.getIndex(i));
            ofVec3f norm = mesh.getNormal(mesh.getIndex(i));

            ofLine(coord, coord + (norm * 10));
        }
    }
    else {
        mesh.draw();
    }

    ofPopMatrix();
}
