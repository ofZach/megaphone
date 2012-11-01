//
//  FlipperPage.cpp
//  FloatingPage
//
//  Created by Elie Zananiri on 2012-11-01.
//
//

#include "FlipperPage.h"

FlipperPage::FlipperPage()
{
    ty = 0;
    pause = 0;

    bend(0, 0);
}

void FlipperPage::bend(float topPct, float bottomPct)
{
    path.clear();

    // try to keep the page area the same as it bends...

    static int pageSize = 20;
    path.moveTo(  ofPoint(-pageSize, pageSize * 0.5 * topPct,    -pageSize * (1 - topPct * 0.5)));
    path.lineTo(  ofPoint( pageSize, pageSize * 0.5 * topPct,    -pageSize * (1 - topPct * 0.5)));

    path.bezierTo(ofPoint( pageSize, pageSize * 0.25 * topPct,   -pageSize * (1 - topPct * 0.5)),
                  ofPoint( pageSize, 0,                          -pageSize * (1 - topPct * 0.5) * 0.5),
                  ofPoint( pageSize, 0,                           0));
    path.bezierTo(ofPoint( pageSize, 0,                           pageSize * (1 - bottomPct) * 0.5),
                  ofPoint( pageSize, pageSize * 0.25 * bottomPct, pageSize * (1 - bottomPct)),
                  ofPoint( pageSize, pageSize * 0.5 * bottomPct,  pageSize * (1 - bottomPct)));

    path.lineTo(  ofPoint(-pageSize, pageSize * 0.5 * bottomPct,  pageSize * (1 - bottomPct)));

    path.bezierTo(ofPoint(-pageSize, pageSize * 0.25 * bottomPct, pageSize * (1 - bottomPct)),
                  ofPoint(-pageSize, 0,                           pageSize * (1 - bottomPct) * 0.5),
                  ofPoint(-pageSize, 0,                           0));
    path.bezierTo(ofPoint(-pageSize, 0,                           -pageSize * (1 - topPct * 0.5) * 0.5),
                  ofPoint(-pageSize, pageSize * 0.25 * topPct,    -pageSize * (1 - topPct * 0.5)),
                  ofPoint(-pageSize, pageSize * 0.5 * topPct,     -pageSize * (1 - topPct * 0.5)));
}

void FlipperPage::update()
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

        bend(pos.y / apex, -twirlInc * 2);

        // tilt more as you reach the apex
        tilt = sin(twirlInc) * 2;

        // flip around
        flipAngle += MAX(twirlInc, 0.02);

        if (pos.y <= 1) {
            // pause for a bit
            pause = 30;
        }
    }
}

void FlipperPage::draw()
{
    ofPushMatrix();
    ofTranslate(pos);
//    ofTranslate(realPos);
    ofRotate(RAD_TO_DEG * flipAngle, 0, 0, 1);
    ofRotate(RAD_TO_DEG * twirl, 0, 1, 0);
    ofRotate(RAD_TO_DEG * tilt, 1, 0, 1);

    path.draw();

    //    static int pageSize = 20;
    //    ofSetColor(200, 0, 0);

    //    ofBeginShape();
    //    ofVertex(-pageSize, 0, -pageSize);
    //    ofVertex(-pageSize, 0, pageSize);
    //    ofVertex(pageSize, 0, pageSize);
    //    ofVertex(pageSize, 0, -pageSize);
    //    ofEndShape(true);
    
    ofPopMatrix();
}
