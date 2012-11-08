//
//  mtl2dMappingTriangle.cpp
//  ofxMTL2DMapping
//
//  Created by Hugues Bruyere on 12-10-31.
//

#include "mtl2dMappingTriangle.h"

//--------------------------------------------------------------
//--------------------------------------------------------------
mtl2dMappingTriangle::mtl2dMappingTriangle()
{

}

//--------------------------------------------------------------
mtl2dMappingTriangle::~mtl2dMappingTriangle(){

}

//--------------------------------------------------------------
void mtl2dMappingTriangle::render()
{
    ofSetColor(255, 255, 255, 255);
    
    glPushMatrix();
    glBegin(GL_POLYGON);
    
    for (int i = 0; i < inputPolygon->polyline->size(); i++) {
        glTexCoord2f(inputPolygon->polyline->getVertices()[i].x, inputPolygon->polyline->getVertices()[i].y);
        glVertex2f(polyline->getVertices()[i].x, polyline->getVertices()[i].y);
    }
    
    glEnd();
    glPopMatrix();
}

//--------------------------------------------------------------
void mtl2dMappingTriangle::createDefaultShape()
{
    shapeSettings["type"] = "triangle";
    inputPolygon = new mtl2dMappingInput();
    
    int xOffset = ofGetWidth()/2.0;
    int yOffset = ofGetHeight()/2.0;

    int x = 50;
    int y = 0;
    
    mtl2dMappingVertex* newVertex = new mtl2dMappingVertex();
    newVertex->init(xOffset + x - newVertex->width/2, yOffset + y - newVertex->height/2);
    vertices.push_back(newVertex);
    
    // Input
    newVertex = new mtl2dMappingVertex();
    newVertex->init(xOffset + x - newVertex->width/2, yOffset + y - newVertex->height/2);
    newVertex->isDefiningTectureCoord = true;
    inputPolygon->vertices.push_back(newVertex);
    
    x = 0;
    y = 100;
    
    //Create a new vertex
    newVertex = new mtl2dMappingVertex();
    newVertex->init(xOffset + x - newVertex->width/2, yOffset + y - newVertex->height/2);
    vertices.push_back(newVertex);
    
    // Input
    newVertex = new mtl2dMappingVertex();
    newVertex->init(xOffset + x - newVertex->width/2, yOffset + y - newVertex->height/2);
    newVertex->isDefiningTectureCoord = true;
    inputPolygon->vertices.push_back(newVertex);
    
    x = 100;
    y = 100;
    
    //Create a new vertex
    newVertex = new mtl2dMappingVertex();
    newVertex->init(xOffset + x - newVertex->width/2, yOffset + y - newVertex->height/2);
    vertices.push_back(newVertex);
    
    // Input
    newVertex = new mtl2dMappingVertex();
    newVertex->init(xOffset + x - newVertex->width/2, yOffset + y - newVertex->height/2);
    newVertex->isDefiningTectureCoord = true;
    inputPolygon->vertices.push_back(newVertex);
    
    inputPolygon->init(shapeId);
}