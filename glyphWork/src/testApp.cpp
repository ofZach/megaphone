#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){

    
    float xPos = 200;
    for (int i = 0; i < 50; i++){
        Glyph g;
        g.pos().set(xPos, 400);
        g.setScale(50);
        glyphs.push_back(g);
        
        
        xPos += g.absBounds().width * 50 + 4;
    }
    
    
    //g.pos().set(300,300);
    //g.setScale(20);
    
}

//--------------------------------------------------------------
void testApp::update(){

    for (int i = 0; i < 50; i++){
        glyphs[i].update();
    }
    
}

//--------------------------------------------------------------
void testApp::draw(){
    ofScale(0.2,0.2);
    ofFill();
    for (int i = 0; i < 50; i++){
        glyphs[i].draw();
        glyphs[i].debug();
        
    }
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
    glyphs.clear();
    float xPos = 200;
    for (int i = 0; i < 50; i++){
        Glyph g;
        g.pos().set(xPos, 400);
        g.setScale(50);
        glyphs.push_back(g);
        
        
        xPos += g.absBounds().width * 50 + 10;
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
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

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