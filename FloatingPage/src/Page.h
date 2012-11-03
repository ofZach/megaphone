//
//  Page.h
//  FloatingPage
//
//  Created by Elie Zananiri on 2012-11-01.
//
//

#pragma once

#include "ofMain.h"

static int groundSize = 300;
static int pageSize = 16;
static int apex = 200;

//--------------------------------------------------------------
//--------------------------------------------------------------
enum PageMode
{
    PageModeStatic = 0,
    PageModeFlat,
    PageModeVert,
    PageModeSway,
    PageModeFlex,
    PageModeFlip,
    PageModeAll,

    NumPageModes
};

//--------------------------------------------------------------
//--------------------------------------------------------------
enum PagePhase
{
    PagePhaseIdle = 0,
    PagePhaseSegue,
    PagePhaseAnimate
};

//--------------------------------------------------------------
//--------------------------------------------------------------
struct PageKeyframe
{
    bool bPosComplete;
    ofPoint pos;

    bool bVertComplete;
    float vertOffsetY;
    float vertAngle;
    float vertBendPct;

    bool bBendComplete;
    float topBendPct;
    float bottomBendPct;

    bool bTwirlComplete;
    float twirlAngle;

    bool bTiltComplete;
    float tiltAngle;

    bool bFlipComplete;
    float flipAngle;

    bool bSwayComplete;
    float swayAngle;
    ofPoint swayPos;

    int onCompletePhase;
    int onCompleteMode;

    bool isComplete() {
        return (bPosComplete && bVertComplete && bBendComplete && bTwirlComplete && bTiltComplete && bFlipComplete && bSwayComplete);
    }
};

//--------------------------------------------------------------
//--------------------------------------------------------------
class Page
{
    public:
        Page();

        void setMode(int newMode);
        int mode;

        void begin(int newMode);
        void end();

        PageKeyframe keyframe;
        int phase;

        void rebuild();
        void rebuild(float bendPct);
        void rebuild(float bendTopPct, float bendBottomPct);
    
        void update();
        void draw();

        int  animateDuration;

        bool bRains;
        float posInc;
        float posSpeed;
        ofPoint startPos;
        ofPoint pos;
        ofPoint rainSpeed;

        float rotInc;
        float rotSpeed;

        bool bTwirls;
        float twirlAngle;

        bool bTilts;
        float tiltAngle;

        bool bFlips;
        float flipAngle;

        float vertAngle;
        float vertOffsetY;
        float vertBendPct;

        float topBendPct;
        float bottomBendPct;

        bool bSways;
        float swayInc;
        float swaySpeed;
        float swayAngle;
        float swayRadius;
        ofPoint swayPos;

        ofPath path;
};
