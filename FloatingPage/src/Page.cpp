//
//  Page.cpp
//  FloatingPage
//
//  Created by Elie Zananiri on 2012-11-01.
//
//

#include "Page.h"

//--------------------------------------------------------------
Page::Page()
{
    animateDuration = 0;

    bRains = false;
    startPos.set(0, apex, 0);
    pos.set(startPos);
    posInc = 0;
    posSpeed = 0.01;
    rainSpeed.set(0, -5, 0);

    rotSpeed = 0.001;

    bTwirls = true;
    twirlAngle = 0;

    bTilts = true;
    tiltAngle = 0;

    bFlips = false;
    flipAngle = 0;

    vertAngle = 0;
    vertOffsetY = 0;
    vertBendPct = 0;

    topBendPct = 0;
    bottomBendPct = 0;

    bSways = false;
    swayInc = 0;
    swayAngle = -M_PI/2;
    swaySpeed = 0.05;
    swayRadius = 50;

    mode = PageModeStatic;
    phase = PagePhaseIdle;

    rebuild();
}

//--------------------------------------------------------------
void Page::setMode(int newMode)
{
    mode = newMode;

    if (mode == PageModeFlat) {
        bTwirls = true;
        bTilts = true;
        bSways = false;
        bFlips = false;

        rebuild();
    }
    else if (mode == PageModeVert) {
        bTwirls = false;
        bTilts = false;
        bSways = false;
        bFlips = false;
    }
    else if (mode == PageModeSway) {
        bTwirls = true;
        bTilts = false;
        bSways = true;
        bFlips = false;

        rebuild();
    }
    else if (mode == PageModeFlex) {
        bTwirls = true;
        bTilts = false;
        bSways = false;
        bFlips = false;
    }
    else if (mode == PageModeFlip) {
        bTwirls = true;
        bTilts = true;
        bSways = false;
        bFlips = true;
    }
    else if (mode == PageModeAll) {
        bTwirls = true;
        bTilts = true;
        bSways = true;
        bFlips = true;
    }
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
    float bendZ = ABS(pageSize * bendPct * 4);

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
    float bendTopZ = -pageSize * (1 - bendTopPct * 0.5);
    
    float bendBottomY = pageSize * 0.5 * bendBottomPct;
    float bendBottomZ = pageSize * (1 - bendBottomPct * 0.5);

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
void Page::begin(int newMode)
{
    if (mode == newMode) return;

    keyframe.bPosComplete = true;

    if (newMode == PageModeVert) {
        keyframe.bVertComplete = false;
        keyframe.vertOffsetY = pageSize;
        keyframe.vertAngle = 90;
        keyframe.vertBendPct = 0;
    }
    else {
        keyframe.bVertComplete = false;
        keyframe.vertOffsetY = 0;
        keyframe.vertAngle = 0;
        keyframe.vertBendPct = 0;
    }

    keyframe.bBendComplete = false;
    keyframe.topBendPct    = 0;
    keyframe.bottomBendPct = 0;

    keyframe.bTwirlComplete = false;
    keyframe.twirlAngle = 0;

    keyframe.bTiltComplete = false;
    keyframe.tiltAngle = 0;

    keyframe.bFlipComplete = false;
    keyframe.flipAngle = 0;

    keyframe.bSwayComplete = false;
    keyframe.swayAngle = -M_PI/2;
    keyframe.swayPos.set(0, 0, 0);

    keyframe.onCompleteMode = newMode;
    keyframe.onCompletePhase = PagePhaseAnimate;

    phase = PagePhaseSegue;
}

//--------------------------------------------------------------
void Page::end()
{
    if (mode == PageModeStatic) return;

    bRains = false;

    keyframe.bPosComplete = false;
    keyframe.pos.set(startPos);

    keyframe.bVertComplete = false;
    keyframe.vertOffsetY = 0;
    keyframe.vertAngle = 0;
    keyframe.vertBendPct = 0;

    keyframe.bBendComplete = false;
    keyframe.topBendPct = 0;
    keyframe.bottomBendPct = 0;

    keyframe.bTwirlComplete = false;
    keyframe.twirlAngle = 0;

    keyframe.bTiltComplete = false;
    keyframe.tiltAngle = 0;

    keyframe.bFlipComplete = false;
    keyframe.flipAngle = 0;

    keyframe.bSwayComplete = false;
    keyframe.swayAngle = -M_PI/2;
    keyframe.swayPos.set(0, 0, 0);

    keyframe.onCompleteMode = PageModeStatic;
    keyframe.onCompletePhase = PagePhaseIdle;

    phase = PagePhaseSegue;
}

//--------------------------------------------------------------
void Page::update()
{
    if (phase == PagePhaseIdle) return;

    else if (phase == PagePhaseSegue) {
        // transition to the keyframe values
        static float lerpRatio = 0.2;

        if (!keyframe.bPosComplete) {
            ofPoint offset = keyframe.pos - pos;
            pos += offset * lerpRatio;

            if (pos.distance(keyframe.pos) < 1) {
                pos.set(keyframe.pos);
                keyframe.bPosComplete = true;
            }
        }

        if (!keyframe.bVertComplete) {
            // move and rotate the page back
            vertOffsetY = ofLerp(vertOffsetY, keyframe.vertOffsetY, lerpRatio);
            vertAngle = ofLerp(vertAngle, keyframe.vertAngle, lerpRatio);

            // bend the page back flat
            vertBendPct = ofLerp(vertBendPct, keyframe.vertBendPct, lerpRatio);

            if (ABS(vertOffsetY - keyframe.vertOffsetY) < 1 && ABS(vertAngle - keyframe.vertAngle) < 1 && (vertBendPct - keyframe.vertBendPct) < 1) {
                vertOffsetY = keyframe.vertOffsetY;
                vertAngle = keyframe.vertAngle;
                vertBendPct = keyframe.vertBendPct;
                keyframe.bVertComplete = true;
            }
        }

        if (!keyframe.bBendComplete) {
            // bend the page back flat
            topBendPct = ofLerp(topBendPct, keyframe.topBendPct, lerpRatio);
            bottomBendPct = ofLerp(bottomBendPct, keyframe.bottomBendPct, lerpRatio);

            if (ABS(topBendPct - keyframe.topBendPct) < 1 && ABS(bottomBendPct - keyframe.bottomBendPct) < 1) {
                topBendPct = keyframe.topBendPct;
                bottomBendPct = keyframe.bottomBendPct;
                keyframe.bBendComplete = true;
            }
        }

        if (!keyframe.bTwirlComplete) {
            while (twirlAngle > M_TWO_PI) twirlAngle -= M_TWO_PI;
            twirlAngle = ofLerp(twirlAngle, keyframe.twirlAngle, lerpRatio);

            if (ABS(twirlAngle - keyframe.twirlAngle) < 1) {
                twirlAngle = keyframe.twirlAngle;
                keyframe.bTwirlComplete = true;
            }
        }

        if (!keyframe.bTiltComplete) {
            while (tiltAngle > M_TWO_PI) tiltAngle -= M_TWO_PI;
            tiltAngle = ofLerp(tiltAngle, keyframe.tiltAngle, lerpRatio);

            if (ABS(tiltAngle - keyframe.tiltAngle) < 1) {
                tiltAngle = keyframe.tiltAngle;
                keyframe.bTiltComplete = true;
            }
        }

        if (!keyframe.bFlipComplete) {
            while (flipAngle > M_TWO_PI) flipAngle -= M_TWO_PI;
            flipAngle = ofLerp(flipAngle, keyframe.flipAngle, lerpRatio);

            if (ABS(flipAngle - keyframe.flipAngle) < 1) {
                flipAngle = keyframe.flipAngle;
                keyframe.bFlipComplete = true;
            }
        }

        if (!keyframe.bSwayComplete) {
            while (swayAngle > M_TWO_PI) swayAngle -= M_TWO_PI;
            swayAngle = ofLerp(swayAngle, keyframe.swayAngle, lerpRatio);
            ofPoint offset = keyframe.swayPos - swayPos;
            swayPos += offset * lerpRatio;

            if (ABS(swayAngle - keyframe.swayAngle) < 1 && swayPos.distance(keyframe.swayPos) < 1) {
                swayAngle = keyframe.swayAngle;
                swayPos = keyframe.swayPos;
                keyframe.bSwayComplete = true;
            }
        }

        if (keyframe.isComplete()) {
            // set up the next state
            setMode(keyframe.onCompleteMode);
            phase = keyframe.onCompletePhase;
        }
    }
    else {  // phase == PagePhaseAnimate
        ++animateDuration;

        // increment slower as you reach the apex
        rotInc = sin(animateDuration) * rotSpeed;
        
        if (bTwirls) twirlAngle += rotInc;
        if (bTilts) tiltAngle = sin(rotInc) * 2;

        // flip around
        if (bFlips) flipAngle += MAX(rotInc, 0.02);

        // sway back and forth
        if (bSways) {
            swayInc += swaySpeed;
            swayAngle = sin(swayInc) - M_PI / 2;
            swayPos.x = cos(swayAngle) * swayRadius;
            swayPos.y = sin(swayAngle) * swayRadius + swayRadius;
        }

        if (mode == PageModeVert || mode == PageModeAll) {
            // bend the bottom of the page
            vertBendPct = sin(animateDuration * 0.1) * 0.5;
        }
        else if (mode == PageModeFlex) {
            // bend more 
            topBendPct = -sin(animateDuration * 0.1);
            bottomBendPct = sin(animateDuration * 0.07);
        }
        else if (mode == PageModeFlip) {
            // funky fresh bending
            topBendPct = sin(animateDuration * 0.1);
            bottomBendPct = -rotInc * 2;
        }
    }

    if (bRains) {
        // rain down, repeat
        pos += rainSpeed;
        if (pos.y < 0) pos.set(startPos); //pos.y = apex * 2;
        if (pos.x < -groundSize) pos.x += groundSize * 2;
        else if (pos.x > groundSize) pos.x -= groundSize * 2;
        if (pos.z < -groundSize) pos.z += groundSize * 2;
        else if (pos.z > groundSize) pos.z -= groundSize * 2;
    }

    if (mode == PageModeVert || mode == PageModeAll) {
        rebuild(vertBendPct);
    }
    else if (mode == PageModeFlex || mode == PageModeFlip) {
        rebuild(topBendPct, bottomBendPct);
    }
}

//--------------------------------------------------------------
void Page::draw()
{
    ofPushMatrix();
    ofTranslate(pos);

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
