//
//  mtl2dMappingPolygon.cpp
//  ofxMTL2DMapping
//
//  Created by Hugues Bruyere on 12-06-06.
//

#include "mtl2dMappingPolygon.h"

mtl2dMappingPolygon*  mtl2dMappingPolygon::activePolygon         = NULL;
mtl2dMappingPolygon*  mtl2dMappingPolygon::previousActivePolygon = NULL;

//--------------------------------------------------------------
//--------------------------------------------------------------
void mtl2dMappingPolygon::resetActivePolygonVars(){
    delete mtl2dMappingPolygon::activePolygon;
    mtl2dMappingPolygon::activePolygon = NULL;
    delete mtl2dMappingPolygon::previousActivePolygon;
    mtl2dMappingPolygon::previousActivePolygon = NULL;
    delete mtl2dMappingVertex::activeVertex;
    mtl2dMappingVertex::activeVertex = NULL;
}

//--------------------------------------------------------------
//--------------------------------------------------------------
mtl2dMappingPolygon::mtl2dMappingPolygon()
{    
    disableAllEvents();
	enableMouseEvents();
    
    // ----
    polyline = new ofPolyline();
    shapeId = -1;
}

//--------------------------------------------------------------
mtl2dMappingPolygon::~mtl2dMappingPolygon(){
    destroy();
}


#pragma mark -
#pragma mark Lifecycle

//--------------------------------------------------------------
void mtl2dMappingPolygon::init(int sId, bool defaultShape) 
{
	shapeId = sId;
	
    if (defaultShape) {
        createDefaultShape();
    }
    
    updatePolyline();
}

//--------------------------------------------------------------
void mtl2dMappingPolygon::destroy()
{
    // ----
    while(!vertices.empty()) delete vertices.back(), vertices.pop_back();
	vertices.clear();
    
    // ----
    polyline->clear();
    delete polyline;
}

//--------------------------------------------------------------
void mtl2dMappingPolygon::update()
{
	if (activePolygon == this) {        
        list<mtl2dMappingVertex*>::iterator it;
        for (it=vertices.begin(); it!=vertices.end(); it++) {
            mtl2dMappingVertex* vertex = *it;                
            if (vertex->toBeRemoved) {
                vertex->kill();
                vertices.remove(vertex);
            }
        }
        
        updatePolyline();
        
	} else {
        disableVertices();
	}
}


//--------------------------------------------------------------
void mtl2dMappingPolygon::draw()
{		
    ofEnableAlphaBlending();
        
    // ----
    list<mtl2dMappingVertex*>::iterator it;
    for (it=vertices.begin(); it!=vertices.end(); it++) {
        mtl2dMappingVertex* vertex = *it;
        vertex->drawBack();
    }

    // ----
    //Shape
    ofBeginShape();
        ofFill();
        if(activePolygon == this) {
            ofSetColor(0, 255, 0, 150);
        } else {
            ofSetColor(255, 0, 0, 150);
        }
        
        for (it=vertices.begin(); it!=vertices.end(); it++) {
            mtl2dMappingVertex* vertex = *it;
            ofVertex(vertex->center.x, vertex->center.y);
        }
    ofEndShape(true);

    // ----
    //Vertices
    for (it=vertices.begin(); it!=vertices.end(); it++) {
        mtl2dMappingVertex* vertex = *it;
        vertex->drawTop();
    }

	ofDisableAlphaBlending();	
}


//--------------------------------------------------------------
void mtl2dMappingPolygon::drawID() 
{
    ofSetHexColor(0x000000);
    ofFill();
    ofRect(_centroid2D.x - 10, _centroid2D.y - 10, 20, 20);
    ofSetHexColor(0xFFFFFF);
    
    int xOffset;
    int yOffset = 3;
    if (shapeId < 10) {
        xOffset = -3;
    } else if (shapeId < 100) {
        xOffset = -7;
    } else {
        xOffset = -11;
    }
    
    mtl2dMappingSettings::infoFont.drawString(ofToString(shapeId), _centroid2D.x + xOffset, _centroid2D.y + yOffset);
}



#pragma mark -
#pragma mark Update Shape's elemets

//--------------------------------------------------------------
void mtl2dMappingPolygon::updatePosition(int xInc, int yInc)
{
    list<mtl2dMappingVertex*>::iterator it;
	for (it=vertices.begin(); it!=vertices.end(); it++) {
		mtl2dMappingVertex* vertex = *it;
		vertex->setPos(vertex->x + xInc, vertex->y + yInc);
        vertex->updateCenter();
	}
    
    updatePolyline();
    
}

//--------------------------------------------------------------
void mtl2dMappingPolygon::updatePolyline()
{
    // ----
    polyline->clear();
    
    list<mtl2dMappingVertex*>::iterator it;
    for (it=vertices.begin(); it!=vertices.end(); it++) {
        mtl2dMappingVertex* vertex = *it;
        polyline->addVertex(vertex->center.x, vertex->center.y);
    }
    
    // ----
    _centroid2D = polyline->getCentroid2D();

    
    // ---- Interactive obj
    boundingBox = polyline->getBoundingBox();
    setPos(boundingBox.x, boundingBox.y);
    setSize(boundingBox.width, boundingBox.height);
}

//--------------------------------------------------------------
bool mtl2dMappingPolygon::hitTest(int tx, int ty)
{
	if (polyline->inside(tx, ty)) {
        return true;
    } else {
        return false;
    }
}

//--------------------------------------------------------------
void mtl2dMappingPolygon::setAsActive()
{
	if (activePolygon != this) {
        previousActivePolygon = activePolygon;
        activePolygon = this;
        
        if (previousActivePolygon) {
            previousActivePolygon->disable();
            previousActivePolygon->disableVertices();
        }
        
        enableVertices();
    }
}

#pragma mark -
#pragma mark Mapping Vertices

//--------------------------------------------------------------
void mtl2dMappingPolygon::enableVertices()
{
    list<mtl2dMappingVertex*>::iterator it;
    for (it=vertices.begin(); it!=vertices.end(); it++) {
        mtl2dMappingVertex* vertex = *it;
        vertex->enabled = true;
    }
}

//--------------------------------------------------------------
void mtl2dMappingPolygon::disableVertices()
{
    list<mtl2dMappingVertex*>::iterator it;
    for (it=vertices.begin(); it!=vertices.end(); it++) {
        mtl2dMappingVertex* vertex = *it;
        vertex->enabled = false;
    }
}


#pragma mark -
#pragma mark Modify Shape at run time
//--------------------------------------------------------------
void mtl2dMappingPolygon::addPoint(int x, int y)
{
	//Check if we are not clicking on an already existing vertex
	list<mtl2dMappingVertex*>::iterator it;
	for (it=vertices.begin(); it!=vertices.end(); it++) {
		mtl2dMappingVertex* vertex = *it;
		
		//CE IF JE DOIS POUVOIR LE REMPLACER PAR UNE VAR STATIC.
		if (vertex->isMouseOver()){
			cout << "EXISTING VERTEX \n";
			return;
		}
	}
	
	//Check if we are not an existing line
	if (vertices.size() >= 2) {
		int nextVertex = 0;
		list<mtl2dMappingVertex*>::iterator it;
		list<mtl2dMappingVertex*>::iterator itNext;
		for (it=vertices.begin(); it!=vertices.end(); it++) {
			it++;
			if (it == vertices.end()) {
				itNext = vertices.begin();
			} else {
				itNext = it;
			}
			it--;
			mtl2dMappingVertex* vertex = *it;
			mtl2dMappingVertex* nextVertex = *itNext;
			
			double dist = distanceToSegment(vertex->center, nextVertex->center, ofVec2f(x, y));
			if ( dist < 10.0) {
				//Create a new vertex
				mtl2dMappingVertex* newVertex = new mtl2dMappingVertex();
				newVertex->init(x-newVertex->width/2, y-newVertex->height/2);
				newVertex->enabled = true;
				it++;
				vertices.insert(it, newVertex);
				it--;
				return;
			}
		}
	}
	
	//Create a new vertex
	mtl2dMappingVertex* newVertex = new mtl2dMappingVertex();
	newVertex->init(x-newVertex->width/2, y-newVertex->height/2);
	newVertex->enabled = true;
	vertices.push_back(newVertex);
}


#pragma mark -
#pragma mark ofxMSAInteractiveObject related
//--------------------------------------------------------------
void mtl2dMappingPolygon::disable()
{
    disableAllEvents();
}

//--------------------------------------------------------------
void mtl2dMappingPolygon::enable()
{
    disableAllEvents();
	enableMouseEvents();
}

#pragma mark -
#pragma mark Interactive Obj Callback methods
//--------------------------------------------------------------
void mtl2dMappingPolygon::onRollOver(int x, int y)
{
}

//--------------------------------------------------------------
void mtl2dMappingPolygon::onRollOut()
{
}

//--------------------------------------------------------------
void mtl2dMappingPolygon::onMouseMove(int x, int y)
{
}

//--------------------------------------------------------------
void mtl2dMappingPolygon::onDragOver(int x, int y, int button)
{    
    if(mtl2dMappingVertex::activeVertex)
        return;

    if(activePolygon == this) {
        updatePosition(x - _grabAnchor.x, y - _grabAnchor.y);
        _grabAnchor.set(x, y);
	}
}

//--------------------------------------------------------------
void mtl2dMappingPolygon::onDragOutside(int x, int y, int button)
{
    if(mtl2dMappingVertex::activeVertex)
        return;
    
    if(activePolygon == this) {
        updatePosition(x - _grabAnchor.x, y - _grabAnchor.y);
        _grabAnchor.set(x, y);
	}
}

//--------------------------------------------------------------
void mtl2dMappingPolygon::onPress(int x, int y, int button)
{    
    _grabAnchor.set(x, y);

    if(mtl2dMappingVertex::activeVertex)
        return;
    
    setAsActive();
}

//--------------------------------------------------------------
void mtl2dMappingPolygon::onRelease(int x, int y, int button)
{

}

//--------------------------------------------------------------
void mtl2dMappingPolygon::onReleaseOutside(int x, int y, int button)
{
}
