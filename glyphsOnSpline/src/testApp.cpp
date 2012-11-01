#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){

    
    float xPos = 200;
    
    ofSeedRandom(0);
    
    float lastWidth = 0;
    
    
    for (int i = 0; i < 50; i++){
        Glyph * g = new Glyph();
        g->setScale(10);
        
        
        float curWidth = g->absBounds().width;
    
        xPos += (curWidth*10)/2 + (lastWidth*10)/2;
        cout << " i " << i << " " << xPos - 200 << endl;
        g->pos().set(xPos, 400);
        glyphs.push_back(g);
        
        lastWidth = g->absBounds().width;
        //xPos += g->absBounds().width + 4;
    }

    
    //g.pos().set(300,300);
    //g.setScale(20);


    //glyph.pos().set(ofGetWidth() / 2, ofGetHeight() / 2);
    //glyph.setScale(50);
}

//--------------------------------------------------------------
void testApp::update()
{
    
    
    float angleSmooth = 0;
    
    if (spline.size() > 0) {
        vector<ofPoint>& points = spline.getVertices();
        float totalLength = spline.getPerimeter();
        float currLength = 0;
        float currPct = 0;

        for (int i = 0; i < 50; i++) {
            // get the point on the spline
            
            //cout << i << " " << currLength << endl;
            
            int indexPct = -1;
            float len = 0; 
            float indexPctA, indexPctB;
            
            for (int j = 1; j < spline.size(); j++){
                float pctA = (float) len / totalLength;
                
                len += (spline[j-1] - spline[j]).length();
                
                float pctB = (float) len / totalLength;
                
                
                if (currPct >= pctA && currPct <= pctB){
                    indexPct = j - 1;
                    indexPctA = pctA;
                    indexPctB = pctB;
                }
            }
            
            
            //float tn = (1.0 - currPct) * (points.size() - 1);
            ofVec2f prevPoint = points[(int)ofClamp(indexPct, 0, points.size() - 1)];
            ofVec2f nextPoint = points[(int)ofClamp(indexPct + 1, 0, points.size() - 1)];
            
            ofVec2f afterPoint = points[(int)ofClamp(indexPct + 2, 0, points.size() - 1)];
            float subt = (indexPctB -  currPct)/(indexPctB - indexPctA); 
            ofPoint currPoint = subt * prevPoint + (1-subt) * nextPoint;
            
            glyphs[i]->pos().set(currPoint);
            float angle = atan2(afterPoint.y - prevPoint.y, afterPoint.x - prevPoint.x);
            
            
            float angleDiff = angle - angleSmooth;
            if (angleDiff < -PI) angleDiff += TWO_PI;
            if (angleDiff > PI) angleDiff -= TWO_PI;
            
            angleSmooth += 0.62f * angleDiff;
            
            
            glyphs[i]->setRotation(angleSmooth * RAD_TO_DEG);
                
            glyphs[i]->update();

            currLength += (glyphs[i]->absBounds().width/2 * 10);
            if (i < glyphs.size() - 1) {
                currLength += (glyphs[i+1]->absBounds().width / 2)* 10;
            }
            
            currLength += 5 + 5 * sin(ofGetElapsedTimef());
            
            
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
        //glyphs[i]->debug();

    }

//    glyph.draw();

}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
    if (key == 'b') {
        bShowSpline ^= true;
    }
    else if (key == 's'){
        float xPos = 200;
        glyphs.clear();
        for (int i = 0; i < 50; i++){
            Glyph * g = new Glyph();
            g->pos().set(xPos, 400);
            g->setScale(10);
            glyphs.push_back(g);
            
            
            xPos += 50 * i + 4;
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

    
    spline.addVertex(x, y);
    
//    ofPoint& lastPoint = spline.getVertices().back();
//    float lastDistance = lastPoint.distance(ofVec2f(x, y));
//
//    // add interpolating points up until the current point over the distance from the last path point
//    for (int i = 0; i < (int)(lastDistance / pointDistance); i++) {
//        spline.addVertex((x - lastPoint.x) / lastDistance * pointDistance * (i+1) + lastPoint.x,
//                         (y - lastPoint.y) / lastDistance * pointDistance * (i+1) + lastPoint.y);
//    }
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