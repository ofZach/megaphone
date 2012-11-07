//
//  glyphShapes.h
//  megaphone
//
//  Created by molmol on 11/4/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "ofMain.h"


class limbShape {
    
public: 
    void addCoord(ofPoint coord){
        coords.push_back(coord);
    }
    void clearCoords(){
        coords.clear();
    }
    
    vector < ofPoint > coords;
    
};



class glyphShapes {
    
public: 
    
    vector <limbShape> limbLibrary;
    vector <limbShape> limbLibraryA;
    vector <limbShape> limbLibraryB;
    
    vector < ofMesh * > meshA;
    vector < ofMesh * > meshB;
    
    
    
    void buildGlyphLimbs(){
        
        
        limbLibrary.clear();
        
        limbShape limb;
        limb.addCoord(ofVec3f(0, 0));
        limb.addCoord(ofVec3f(2, 2));
        limb.addCoord(ofVec3f(0, 4));
        limbLibrary.push_back(limb);
        
        limb.clearCoords();
        limb.addCoord(ofVec3f(0, 0));
        limb.addCoord(ofVec3f(4, 0));
        limb.addCoord(ofVec3f(2, 2));
        limbLibrary.push_back(limb);
        
        limb.clearCoords();
        limb.addCoord(ofVec3f(4, 0));
        limb.addCoord(ofVec3f(4, 2));
        limb.addCoord(ofVec3f(3, 1));
        limbLibrary.push_back(limb);
        
        limb.clearCoords();
        limb.addCoord(ofVec3f(2, 2));
        limb.addCoord(ofVec3f(3, 1));
        limb.addCoord(ofVec3f(4, 2));
        limb.addCoord(ofVec3f(4, 2));
        limb.addCoord(ofVec3f(3, 3));
        limb.addCoord(ofVec3f(2, 2));
        limbLibrary.push_back(limb);
        
        limb.clearCoords();
        limb.addCoord(ofVec3f(1, 3));
        limb.addCoord(ofVec3f(2, 2));
        limb.addCoord(ofVec3f(3, 3));
        limbLibrary.push_back(limb);
        
        limb.clearCoords();
        limb.addCoord(ofVec3f(2, 4));
        limb.addCoord(ofVec3f(0, 4));
        limb.addCoord(ofVec3f(1, 3));
        limb.addCoord(ofVec3f(1, 3));
        limb.addCoord(ofVec3f(3, 3));
        limb.addCoord(ofVec3f(2, 4));
        limbLibrary.push_back(limb);
        
        limb.clearCoords();
        limb.addCoord(ofVec3f(4, 2));
        limb.addCoord(ofVec3f(4, 4));
        limb.addCoord(ofVec3f(2, 4));
        limbLibrary.push_back(limb);
        
        for (int i = 0; i < limbLibrary.size(); i++){
            for (int j = 0; j < limbLibrary[i].coords.size(); j++){
                limbLibrary[i].coords[j] -= ofPoint(2,2);   
            }
        }
        
        limbLibraryA = limbLibrary;
        limbLibraryB = limbLibrary;
        
    
        for (int i = 0; i < limbLibrary.size(); i++){
            for (int j = 0; j < limbLibrary[i].coords.size(); j++){
                limbLibraryB[i].coords[j].x *= -1;   
            }
        }
        
//        
//        //    if (ofRandom(0,1) > 0.5){
//        //        for (int i = 0; i < limbLibrary.size(); i++){
//        //            for (int j = 0; j < limbLibrary[i].coords.size(); j++){
//        //                limbLibrary[i].coords[j].y = -1;   
//        //            }
//        //        }
//        //    }
//        
        for (int i = 0; i < limbLibraryA.size(); i++){
            for (int j = 0; j < limbLibraryA[i].coords.size(); j++){
                float ang = PI/2 * (int)ofRandom(0,50);
                //cout << ang / (PI/2) << endl;
                ofPoint pos = limbLibraryA[i].coords[j];
                float len = pos.length();
                float angle = atan2(pos.y, pos.x);
                angle += PI/2 * 3;
                
                limbLibraryA[i].coords[j].set( len * cos(angle), len * sin(angle));
                
            }
        }
        
        
        for (int i = 0; i < limbLibraryB.size(); i++){
            for (int j = 0; j < limbLibraryB[i].coords.size(); j++){
                float ang = PI/2 * (int)ofRandom(0,50);
                //cout << ang / (PI/2) << endl;
                ofPoint pos = limbLibraryB[i].coords[j];
                float len = pos.length();
                float angle = atan2(pos.y, pos.x);
                angle += PI/2 * 3;
                
                limbLibraryB[i].coords[j].set( len * cos(angle), len * sin(angle));
                
            }
        }
        
        
        
        for (int i = 0; i < limbLibraryA.size(); i++){
            ofMesh * mesh = new ofMesh();
            mesh->setMode(OF_PRIMITIVE_TRIANGLES);
            for (int j = 0; j < limbLibraryA[i].coords.size(); j++){
                mesh->addVertex(limbLibraryA[i].coords[j]);
            }
            meshA.push_back(mesh);
        }
        
        for (int i = 0; i < limbLibraryB.size(); i++){
            ofMesh * mesh = new ofMesh();
            mesh->setMode(OF_PRIMITIVE_TRIANGLES);
            for (int j = 0; j < limbLibraryB[i].coords.size(); j++){
                mesh->addVertex(limbLibraryB[i].coords[j]);
            }
            meshB.push_back(mesh);
        }
        
    }
    
    
    
};


