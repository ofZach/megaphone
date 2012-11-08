//
//  mtl2dMappingQuad.cpp
//  ofxMTL2DMapping
//
//  Created by Hugues Bruyere on 12-10-31.
//

#include "mtl2dMappingQuad.h"

//--------------------------------------------------------------
//--------------------------------------------------------------
mtl2dMappingQuad::mtl2dMappingQuad()
{

}

//--------------------------------------------------------------
mtl2dMappingQuad::~mtl2dMappingQuad(){

}

//--------------------------------------------------------------
void mtl2dMappingQuad::render()
{
    ofSetColor(255, 255, 255, 255);
    
    glPushMatrix();
    glMultMatrixf(inputPolygon->homoMatrix);
    glBegin(GL_POLYGON);
    
    int i = 0;
    list<mtl2dMappingVertex*>::iterator it;
    for (it=inputPolygon->vertices.begin(); it!=inputPolygon->vertices.end(); it++) {
        mtl2dMappingVertex* vertex = *it;
        glTexCoord2f(vertex->center.x, vertex->center.y);
        glVertex2f(inputPolygon->polyline->getVertices()[i].x - inputPolygon->boundingBox.x, inputPolygon->polyline->getVertices()[i].y - inputPolygon->boundingBox.y);
        i++;
    }
    
    glEnd();
    glPopMatrix();
}


//--------------------------------------------------------------
void mtl2dMappingQuad::createDefaultShape()
{
    shapeSettings["type"] = "quad";
    
    inputPolygon = new mtl2dMappingInput();
    
    
    int xOffset = ofGetWidth()/2.0;
    int yOffset = ofGetHeight()/2.0;
    
    //Create a new vertex
    int x = 0;
    int y = 0;
    mtl2dMappingVertex* newVertex = new mtl2dMappingVertex();
    newVertex->init(xOffset + x - newVertex->width/2, yOffset + y - newVertex->height/2);
    vertices.push_back(newVertex);
    
    // Input
    newVertex = new mtl2dMappingVertex();
    newVertex->init(xOffset + x - newVertex->width/2, yOffset + y - newVertex->height/2);
    newVertex->isDefiningTectureCoord = true;
    inputPolygon->vertices.push_back(newVertex);
    
    x = 100;
    y = 0;
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
    newVertex = new mtl2dMappingVertex();
    newVertex->init(xOffset + x - newVertex->width/2, yOffset + y - newVertex->height/2);
    vertices.push_back(newVertex);
    
    // Input
    newVertex = new mtl2dMappingVertex();
    newVertex->init(xOffset + x - newVertex->width/2, yOffset + y - newVertex->height/2);
    newVertex->isDefiningTectureCoord = true;
    inputPolygon->vertices.push_back(newVertex);
    
    x = 0;
    y = 100;
    newVertex = new mtl2dMappingVertex();
    newVertex->init(xOffset + x - newVertex->width/2, yOffset + y - newVertex->height/2);
    vertices.push_back(newVertex);
    
    // Input
    newVertex = new mtl2dMappingVertex();
    newVertex->init(xOffset + x - newVertex->width/2, yOffset + y - newVertex->height/2);
    newVertex->isDefiningTectureCoord = true;
    inputPolygon->vertices.push_back(newVertex);
    
    // ----
    inputPolygon->init(shapeId);
}

//--------------------------------------------------------------
void mtl2dMappingQuad::calcHomography()
{
    updatePolyline();
    
    inputPolygon->calcHomography(polyline->getVertices()[0].x, polyline->getVertices()[0].y,
                              polyline->getVertices()[1].x, polyline->getVertices()[1].y,
                              polyline->getVertices()[2].x, polyline->getVertices()[2].y,
                              polyline->getVertices()[3].x, polyline->getVertices()[3].y);
}