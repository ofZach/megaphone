#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){

    
    float xPos = 200;
    for (int i = 0; i < 50; i++){
        Glyph * g = new Glyph();
        g->pos().set(xPos, 400);
        g->setScale(10);
        glyphs.push_back(g);
        
        
        xPos += g->absBounds().width * 50 + 4;
    }

    
    //g.pos().set(300,300);
    //g.setScale(20);


    glyph.pos().set(ofGetWidth() / 2, ofGetHeight() / 2);
    glyph.setScale(50);
}

//--------------------------------------------------------------
void testApp::update()
{
    if (spline.size() > 0) {
        vector<ofPoint>& points = spline.getVertices();
        float totalLength = spline.getPerimeter();
        float currLength = 0;
        float currPct = 0;

        for (int i = 0; i < 50; i++) {
            // get the point on the spline
            float tn = (1.0 - currPct) * (points.size() - 1);
            ofVec2f prevPoint = points[(int)ofClamp(tn, 0, points.size() - 1)];
            ofVec2f nextPoint = points[(int)ofClamp(tn + 1, 0, points.size() - 1)];
            float subt = tn - (int)tn;
            ofPoint currPoint = prevPoint.getInterpolated(nextPoint, subt);

            // update the glyph position if it's different
            if (currPoint != glyphs[i]->pos()) {
                glyphs[i]->prv().set(glyphs[i]->pos());
                glyphs[i]->pos().set(currPoint);
            }
            glyphs[i]->update();

            // if we are moving from right to left, set the offset for the next object based on the current object's width
            if (prevPoint.x >= nextPoint.x) {
                currLength += glyphs[i]->absBounds().width + 10;
            }
            // if we are moving from left to right, set the offset for the next object based on its own width
            else if (i < glyphs.size() - 1) {
                currLength += glyphs[i+1]->absBounds().width + 10;
            }

            currPct = currLength / totalLength;
            if (currPct > 1) break;
        }
    }
}

//--------------------------------------------------------------
void testApp::draw(){
    if (bShowSpline) {
        ofSetColor(255);
        spline.draw();
    }

//    ofScale(0.2,0.2);
    ofFill();
    for (int i = 0; i < 50; i++){
        glyphs[i]->draw();
        glyphs[i]->debug();

    }

//    glyph.draw();

}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
    if (key == 'b') {
        bShowSpline ^= true;
    }
    else {
        glyphs.clear();
        float xPos = 200;
        for (int i = 0; i < 50; i++){
            Glyph  * g = new Glyph;
            g->pos().set(xPos, 400);
            g->setScale(10);
            glyphs.push_back(g);
            
            
            xPos += g->absBounds().width * 50 + 10;
        }
    }
    
    /*g = Glyph();
    g.pos().set(300,300);
    g.setScale(20);*/
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

    
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{
    static float pointDistance = 10;

    ofPoint& lastPoint = spline.getVertices().back();
    float lastDistance = lastPoint.distance(ofVec2f(x, y));

    // add interpolating points up until the current point over the distance from the last path point
    for (int i = 0; i < (int)(lastDistance / pointDistance); i++) {
        spline.addVertex((x - lastPoint.x) / lastDistance * pointDistance * (i+1) + lastPoint.x,
                         (y - lastPoint.y) / lastDistance * pointDistance * (i+1) + lastPoint.y);
    }
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{
    spline.clear();
    spline.addVertex(x, y);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{
    
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}