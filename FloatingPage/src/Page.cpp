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
    bPausesOnGround = true;
    pauseRemaining = 0;
    floatDuration = 0;

    bHops = true;
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

    bSways = false;
    swayInc = 0;
    swaySpeed = 0.05;
    swayRadius = 50;

    mode = 0;

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
void Page::update()
{
    if (pauseRemaining > 0) {
        --pauseRemaining;
    }
    else {
        ++floatDuration;

        if (bHops) {
            // move up and down
            posInc += posSpeed;
            pos.y = ABS(sin(posInc)) * apex;
        }
        else {
            // rain down, repeat
            pos += rainSpeed;
            if (pos.y < 0) pos.y = apex * 2;
            if (pos.x < -groundSize) pos.x += groundSize * 2;
            else if (pos.x > groundSize) pos.x -= groundSize * 2;
            if (pos.z < -groundSize) pos.z += groundSize * 2;
            else if (pos.z > groundSize) pos.z -= groundSize * 2;
        }

        // increment slower as you reach the apex
        rotInc = (apex - pos.y) * rotSpeed;
        twirlAngle += rotInc;
        tiltAngle = sin(rotInc) * 2;

        // flip around
        flipAngle += MAX(rotInc, 0.02);

        // sway back and forth
        swayInc += swaySpeed;
        swayAngle = sin(swayInc) - M_PI / 2;
        swayPos.x = cos(swayAngle) * swayRadius;
        swayPos.y = sin(swayAngle) * swayRadius + swayRadius;

        if (mode == PageModeVert) {
            // bend the bottom of the page
            rebuild(sin(floatDuration * 0.1) * 0.5);
        }
        else if (mode == PageModeFlex) {
            // bend more as you reach the apex
            rebuild(-pos.y / apex, pos.y / apex);
        }
        else if (mode == PageModeFlip) {
            // funky fresh bending
            rebuild(pos.y / apex, -rotInc * 2);
        }

        if (bPausesOnGround && pos.y <= 1) {
            // pause for a bit
            pauseRemaining = 30;
            floatDuration = 0;
        }
    }
}

//--------------------------------------------------------------
void Page::draw()
{
    ofPushMatrix();
    ofTranslate(pos);
    if (bSways) {
        ofTranslate(swayPos);
        ofRotate(RAD_TO_DEG * (swayAngle + M_PI / 2), 0, 0, 1);
    }
    if (bFlips) {
        ofRotate(RAD_TO_DEG * flipAngle, 0, 0, 1);
    }
    if (mode == PageModeVert) {
        ofTranslate(0, pageSize);
        ofRotate(90, 1, 0, 0);
    }
    if (bTwirls) {
        ofRotate(RAD_TO_DEG * twirlAngle, 0, 1, 0);
    }
    if (bTilts) {
        ofRotate(RAD_TO_DEG * tiltAngle, 1, 0, 1);
    }

    path.draw();
    
    ofPopMatrix();
}