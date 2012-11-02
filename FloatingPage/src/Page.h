//
//  Page.h
//  FloatingPage
//
//  Created by Elie Zananiri on 2012-11-01.
//
//

#pragma once

#include "ofMain.h"

static int groundSize = 400;
static int pageSize = 20;
static int apex = 400;

enum PageMode
{
    PageModeFlat = 0,
    PageModeVert,
    PageModeSway,
    PageModeFlex,
    PageModeFlip,

    NumPageModes
};

class Page
{
    public:
        Page();

        void setMode(int newMode);
        int mode;

        void rebuild();
        void rebuild(float bendPct);
        void rebuild(float bendTopPct, float bendBottomPct);
    
        void update();
        void draw();

        bool bPausesOnGround;
        int  pauseRemaining;
        int  floatDuration;

        bool bHops;
        float posInc;
        float posSpeed;
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

        bool bSways;
        float swayInc;
        float swaySpeed;
        float swayAngle;
        float swayRadius;
        ofPoint swayPos;

        ofPath path;
};
