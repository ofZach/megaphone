//
//  Page.cpp
//  FloatingPage
//
//  Created by Elie Zananiri on 2012-11-01.
//
//

#include "Page.h"

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

//--------------------------------------------------------------
Page::Page()
{
    animateDuration = ofRandom(10);

    bGoingUp = true;
    startPos.set(0, 20, 0);
    pos.set(startPos);
    posInc = 0;
    posSpeed = 0.01;
    rainSpeed.set(0, -5, 0);
    tornadoAngle = ofRandom(M_TWO_PI);

    twirlAngle = 0;
    tiltAngle = 0;
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
    path.lineTo(ofPoint(-pageSize, 0, pageSize));
    path.lineTo(ofPoint(pageSize, 0, pageSize));
    path.lineTo(ofPoint(pageSize, 0, -pageSize));
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
}

//--------------------------------------------------------------
void Page::update()
{
    ++animateDuration;

    rotInc = sin(animateDuration * 0.1);

    // twirl, tilt, and flip around
    twirlAngle = rotInc * 5 * twirlAmount;

    tiltAngle = sin(rotInc * 0.01) * 100 * tiltAmount;

    if (flipAmount > 0) {
        flipAngle += MAX(rotInc, 0.02) * flipAmount;
    }
    else {
        if (flipAngle != 0) {
            while (flipAngle > M_TWO_PI) flipAngle -= M_TWO_PI;
            if (flipAngle + 0.2 < M_TWO_PI) {
                flipAngle += 0.2;
            }
            else {
                flipAngle = 0;
            }
        }
    }

    // sway back and forth
    swayInc += swaySpeed;
    swayAngle = (sin(swayInc) * swayAmount) - M_PI / 2;
    swayPos.x = cos(swayAngle) * swayRadius;
    swayPos.y = sin(swayAngle) * swayRadius + swayRadius;
    swayPos *= swayAmount;

    if (bendTail) {
        // bend the bottom of the page
        vertBendPct = sin(animateDuration * 0.1) * bottomBendAmount;
    }
    else if (bendWings) {
        // bend more 
        topBendPct = sin(animateDuration * 0.1) * topBendAmount;
        bottomBendPct = sin(animateDuration * 0.1) * bottomBendAmount;
    }
    else if (bendFresh) {
        // funky fresh bending
        topBendPct = sin(animateDuration * 0.1) * topBendAmount;
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
                tornadoOffset.y += tornadoAngle;
                if (tornadoRadius >= groundSize) {
                    bGoingUp = false;
                }
            }
            else {
                tornadoOffset.y -= tornadoAngle;
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

    path.draw();
    
    ofPopMatrix();
}
