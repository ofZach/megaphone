//
//  A5Page.cpp
//  FloatingPage
//
//  Created by Elie Zananiri on 2012-11-01.
//
//

#include "A5Page.h"

A5Page::A5Page()
{
    ty = 0;
    pause = 0;
    jump = 0;

    bend(0);
}

void A5Page::bend(float pct)
{
    path.clear();

    static int pageSize = 20;
//    path.moveTo(ofPoint(-pageSize, pageSize * 0.5 * topPct, -pageSize));
//    path.lineTo(ofPoint(pageSize, pageSize * 0.5 * topPct, -pageSize));
//    path.bezierTo(ofPoint(pageSize, pageSize * 0.25 * topPct, -pageSize), ofPoint(pageSize, 0, -pageSize/2), ofPoint(pageSize, 0, 0));
//    path.bezierTo(ofPoint(pageSize, 0, pageSize/2), ofPoint(pageSize, pageSize * 0.25 * bottomPct, pageSize), ofPoint(pageSize, pageSize * 0.5 * bottomPct, pageSize));
//    path.lineTo(ofPoint(-pageSize, pageSize * 0.5 * bottomPct, pageSize));
//    path.bezierTo(ofPoint(-pageSize, pageSize * 0.25 * bottomPct, pageSize), ofPoint(-pageSize, 0, pageSize/2), ofPoint(-pageSize, 0, 0));
//    path.bezierTo(ofPoint(-pageSize, 0, -pageSize * 0.5), ofPoint(-pageSize, pageSize * 0.25 * topPct, -pageSize), ofPoint(-pageSize, pageSize * 0.5 * topPct, -pageSize));

    float bendY = pageSize * pct;
    float bendZ = pageSize * pct * 4;

    path.moveTo(ofPoint(-pageSize, 0, -pageSize));
    path.lineTo(ofPoint(pageSize, 0, -pageSize));
    path.lineTo(ofPoint(pageSize, 0, pageSize - ABS(bendZ)));
    path.bezierTo(ofPoint(pageSize, 0, pageSize - ABS(bendZ) * 0.5), ofPoint(pageSize, bendY * 0.5, pageSize), ofPoint(pageSize, bendY, pageSize));  // B
    path.lineTo(ofPoint(-pageSize, bendY, pageSize));
    path.bezierTo(ofPoint(-pageSize, bendY * 0.5, pageSize), ofPoint(-pageSize, 0, pageSize - ABS(bendZ) * 0.5), ofPoint(-pageSize, 0, pageSize - ABS(bendZ)));  // B
    path.lineTo(ofPoint(-pageSize, 0, -pageSize));



//    path.bezierTo(ofPoint(pageSize, 0                                               , -pageSize), ofPoint(pageSize, 0, -pageSize/2), ofPoint(pageSize, 0, 0));
//    path.bezierTo(ofPoint(pageSize, 0, pageSize/2), ofPoint(pageSize, pageSize * 0.25 * bottomPct, pageSize), ofPoint(pageSize, pageSize * 0.5 * bottomPct, pageSize));
//    path.lineTo(ofPoint(-pageSize, pageSize * 0.5 * bottomPct, pageSize));
//    path.bezierTo(ofPoint(-pageSize, pageSize * 0.25 * bottomPct, pageSize), ofPoint(-pageSize, 0, pageSize/2), ofPoint(-pageSize, 0, 0));
//    path.bezierTo(ofPoint(-pageSize, 0, -pageSize * 0.5), ofPoint(-pageSize, pageSize * 0.25 * topPct, -pageSize), ofPoint(-pageSize, pageSize * 0.5 * topPct, -pageSize));

}

void A5Page::update()
{
    if (pause > 0) --pause;
    else {
        static int apex = 200;

        // move up and down
        ty += 0.01;
        pos.y = ABS(sin(ty)) * apex;

        // twirl slower as you reach the apex
//        float twirlInc = (apex - pos.y) * 0.001;
//        twirl += twirlInc;

        // bend on a sine wave
        ++jump;
        bend(sin(jump * 0.1) * 0.5);

        if (pos.y <= 1) {
            // pause for a bit
            pause = 30;
            jump = 0;
        }
    }
}

void A5Page::draw()
{
    ofPushMatrix();
    ofTranslate(pos);
    ofTranslate(0, 20);
    ofRotate(90, 1, 0, 0);
//    ofRotate(RAD_TO_DEG * twirl, 0, 1, 0);
//    ofRotate(RAD_TO_DEG * tilt, 1, 0, 1);

    path.draw();
    
    ofPopMatrix();
}
