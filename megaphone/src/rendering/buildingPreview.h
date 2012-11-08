//
//  buildingPreview.h
//  megaphone
//
//  Created by molmol on 11/7/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef megaphone_buildingPreview_h
#define megaphone_buildingPreview_h


class buildingPreview {
    
    
    ofMesh mesh;
    ofFbo world;
    ofEasyCam cam;
    
    void setup(){
        world.allocate(768, 2048);
        mesh.setMode(OF_PRIMITIVE_TRIANGLES);
        
        float w = 768;
        float h = 1024*2;
        
        float wSize = 8*100;
        float hSize = 3*100 / 2.0;
        
        
        //float pinchAmount = ofMap(mouseX, 0, ofGetWidth(), 0,1);
        float pinchAmount = 0.1f;
        
        mesh.addTexCoord(ofPoint(w/2, pinchAmount*h));
        mesh.addVertex(ofPoint(0,0,0));
        mesh.addTexCoord(ofPoint(w/2, h));
        mesh.addVertex(ofPoint(0,wSize,0));
        mesh.addTexCoord(ofPoint(w, h));
        mesh.addVertex(ofPoint(0,wSize,hSize));
        
        
        mesh.addTexCoord(ofPoint(w/2, pinchAmount*h));
        mesh.addVertex(ofPoint(0,0,0));
        mesh.addTexCoord(ofPoint(w, 0));
        mesh.addVertex(ofPoint(0,0,hSize));
        mesh.addTexCoord(ofPoint(w, h));
        mesh.addVertex(ofPoint(0,wSize,hSize));
        
        mesh.addTexCoord(ofPoint(w/2, pinchAmount*h));
        mesh.addVertex(ofPoint(0,0,0));
        mesh.addTexCoord(ofPoint(w/2, h));
        mesh.addVertex(ofPoint(0,wSize,0));
        mesh.addTexCoord(ofPoint(0, h));
        mesh.addVertex(ofPoint(hSize,wSize,0));
        
        mesh.addTexCoord(ofPoint(w/2, pinchAmount*h));
        mesh.addVertex(ofPoint(0,0,0));
        mesh.addTexCoord(ofPoint(0, 0));
        mesh.addVertex(ofPoint(hSize,0,0));
        mesh.addTexCoord(ofPoint(0,h));
        mesh.addVertex(ofPoint(hSize,wSize,0));
        
        cam.setupPerspective();
    }
    
    void begin(){
        world.begin();
        ofClear(0,0,0, 255);
        
    }
    
    void end(){
        world.end();
    }
    
    void draw(){
        //--- for testing on building looks.  not really mapped well yet. 
        ofSetColor(255,255,255);
        cam.begin();
        world.getTextureReference().bind();
        mesh.draw();
        world.getTextureReference().unbind();
        ofSetColor(255,255,255,10);
        mesh.drawWireframe();
        cam.end();
    }
}

#endif
