//
//  Page.h
//  FloatingPage
//
//  Created by Elie Zananiri on 2012-11-01.
//
//

#pragma once

#include "ofMain.h"

enum PageMode
{
    PageModeFlat = 0,
    PageModeVert,
    PageModeSway,
    PageModeFlex,
    PageModeFlip
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

        float posInc;
        float posSpeed;
        ofPoint pos;

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
