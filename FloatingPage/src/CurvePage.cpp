//
//  CurvePage.cpp
//  FloatingPage
//
//  Created by Elie Zananiri on 2012-11-01.
//
//

#include "CurvePage.h"

CurvePage::CurvePage()
{
    ty = 0;
    pause = 0;

    bend(0, 0);
}

void CurvePage::bend(float topPct, float bottomPct)
{
    path.clear();

    static int pageSize = 20;
    path.moveTo(ofPoint(-pageSize, pageSize * 0.5 * topPct, -pageSize));
    path.lineTo(ofPoint(pageSize, pageSize * 0.5 * topPct, -pageSize));
    path.bezierTo(ofPoint(pageSize, pageSize * 0.25 * topPct, -pageSize), ofPoint(pageSize, 0, -pageSize/2), ofPoint(pageSize, 0, 0));
    path.bezierTo(ofPoint(pageSize, 0, pageSize/2), ofPoint(pageSize, pageSize * 0.25 * bottomPct, pageSize), ofPoint(pageSize, pageSize * 0.5 * bottomPct, pageSize));
    path.lineTo(ofPoint(-pageSize, pageSize * 0.5 * bottomPct, pageSize));
    path.bezierTo(ofPoint(-pageSize, pageSize * 0.25 * bottomPct, pageSize), ofPoint(-pageSize, 0, pageSize/2), ofPoint(-pageSize, 0, 0));
    path.bezierTo(ofPoint(-pageSize, 0, -pageSize * 0.5), ofPoint(-pageSize, pageSize * 0.25 * topPct, -pageSize), ofPoint(-pageSize, pageSize * 0.5 * topPct, -pageSize));
}

void CurvePage::update()
{
    if (pause > 0) --pause;
    else {
        static int apex = 200;

        // move up and down
        ty += 0.01;
        pos.y = ABS(sin(ty)) * apex;

        // twirl slower as you reach the apex
        float twirlInc = (apex - pos.y) * 0.001;
        twirl += twirlInc;

        // bend more as you reach the apex
        bend(2 * pos.y / apex, -pos.y / apex);
//        tilt = sin(twirlInc) * 2;

        if (pos.y <= 1) {
            // pause for a bit
            pause = 30;
        }
    }
}

void CurvePage::draw()
{
    ofPushMatrix();
    ofTranslate(pos);
    ofRotate(RAD_TO_DEG * twirl, 0, 1, 0);
    ofRotate(RAD_TO_DEG * tilt, 1, 0, 1);

    path.draw();
    
    ofPopMatrix();
}

