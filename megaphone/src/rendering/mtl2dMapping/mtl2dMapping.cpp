//
//  mtl2dMapping.cpp
//  ofxMTL2DMapping
//
//  Created by Hugues Bruyere on 12-06-06.
//

#include "mtl2dMapping.h"
#include "mtl2dMappingSettings.h"
#include "ControlsMapping.h"
#include "mtl2dMappingInput.h"

struct Comparator {
    bool operator()(mtl2dMappingShape* first, mtl2dMappingShape* second) const {
        return first->shapeId > second->shapeId;
    }
};

//--------------------------------------------------------------
//--------------------------------------------------------------
mtl2dMapping::mtl2dMapping() 
{
}

//--------------------------------------------------------------
mtl2dMapping::~mtl2dMapping() 
{
    // ----
    while(!_pmShapes.empty()) delete _pmShapes.back(), _pmShapes.pop_back();
	_pmShapes.clear();
}

//--------------------------------------------------------------
void mtl2dMapping::init(int width, int height)
{
    ControlsMapping::controlsMapping()->disable();
    
    // ----
    _fbo.allocate(width, height, GL_RGBA);

    // ----
    mtl2dMappingSettings::infoFont.loadFont("mapping/controls/ReplicaBold.ttf", 10);
    
    // ----
    loadShapesList("mapping/xml/shapes.xml");
    
}

//--------------------------------------------------------------
void mtl2dMapping::update()
{    
    // ---- save mapping to xml
    if(ControlsMapping::controlsMapping()->saveMapping()) {
        ControlsMapping::controlsMapping()->resetSaveMapping();
        saveShapesList("mapping/xml/shapes.xml");
    }
    
    
    // ---- load mapping from xml
    if(ControlsMapping::controlsMapping()->loadMapping()) {
        ControlsMapping::controlsMapping()->resetLoadMapping();
        loadShapesList("mapping/xml/shapes.xml");
    }
    
    // ----
    // Create a new shape
    if(ControlsMapping::controlsMapping()->createNewQuad()) {
        ControlsMapping::controlsMapping()->resetCreateNewShape();
        createQuad(ofGetWidth()/2, ofGetHeight()/2);
        return;
    }
    
    if(ControlsMapping::controlsMapping()->createNewTriangle()) {
        ControlsMapping::controlsMapping()->resetCreateNewShape();
        createTriangle(ofGetWidth()/2, ofGetHeight()/2);
        return;
    }
    
    if(ControlsMapping::controlsMapping()->mappingModeChanged()) {
        ControlsMapping::controlsMapping()->resetMappingChangedFlag();

        // ---- OUTPUT MODE
        if(ControlsMapping::controlsMapping()->mappingMode() == MAPPING_MODE_OUTPUT) {
            list<mtl2dMappingShape*>::iterator it;
            for (it=_pmShapes.begin(); it!=_pmShapes.end(); it++) {
                mtl2dMappingShape* shape = *it;
                shape->enable();
                shape->inputPolygon->disable();
            }
        // ---- INPUT MODE
        } else if (ControlsMapping::controlsMapping()->mappingMode() == MAPPING_MODE_INPUT) {
            list<mtl2dMappingShape*>::iterator it;
            for (it=_pmShapes.begin(); it!=_pmShapes.end(); it++) {
                mtl2dMappingShape* shape = *it;
                shape->disable();
                shape->inputPolygon->enable();
            }
        }
    
    }
    
    // ----
    if(!ControlsMapping::controlsMapping()->editShapes())
        return;
    
    // ----
    list<mtl2dMappingShape*>::iterator it;
    for (it=_pmShapes.begin(); it!=_pmShapes.end(); it++) {
        mtl2dMappingShape* shape = *it;
        shape->update();
    }
}


void mtl2dMapping::drawOutput(){
     render();
}



#pragma mark -
#pragma mark Draw / Edit Mode
//--------------------------------------------------------------
void mtl2dMapping::draw()
{
    
    if(ControlsMapping::controlsMapping()->editShapes()) {
        // ---- OUTPUT MODE
        if(ControlsMapping::controlsMapping()->mappingMode() == MAPPING_MODE_OUTPUT) {
            render();

          
        // ---- INPUT MODE
        } else if (ControlsMapping::controlsMapping()->mappingMode() == MAPPING_MODE_INPUT) {
            // ----
            drawFbo(); 
        }
        
        
        // ----
        list<mtl2dMappingShape*>::iterator it;
        for (it=_pmShapes.begin(); it!=_pmShapes.end(); it++) {
            mtl2dMappingShape* shape = *it;
            
            if(shape != mtl2dMappingShape::activeShape) {
                shape->draw();
            }
            
            
            if(ControlsMapping::controlsMapping()->showShapesId())  {
                shape->drawID();
            }
        }
        
        if(mtl2dMappingShape::activeShape) {
            //Draw active shape on top
            mtl2dMappingShape::activeShape->draw();
        }
    }
    else {
        render();
    }
}

#pragma mark -
#pragma mark FBO
//--------------------------------------------------------------
void mtl2dMapping::bind()
{
    _fbo.begin();
    ofClear(.0f, .0f, .0f, .0f);
    ofClearAlpha();
    
}

//--------------------------------------------------------------
void mtl2dMapping::unbind()
{
    _fbo.end();
}

//--------------------------------------------------------------
void mtl2dMapping::drawFbo()
{
    glColor3f(1.0f, 1.0f, 1.0f);
    _fbo.draw(0, 0);
}

#pragma mark -
#pragma mark Render - Mapping Mode
//--------------------------------------------------------------
void mtl2dMapping::render()
{
	
	ofDisableAlphaBlending();
	ofSetColor(255,255,255);
	
    _fbo.getTextureReference().bind();
    list<mtl2dMappingShape*>::iterator it;
    for (it=_pmShapes.begin(); it!=_pmShapes.end(); it++) {
        mtl2dMappingShape* shape = *it;
        shape->render();
    }
    _fbo.getTextureReference().unbind();
}


#pragma mark -
#pragma mark Shapes Related
//--------------------------------------------------------------
void mtl2dMapping::createQuad(float _x, float _y)
{
    mtl2dMappingShape* newShape = new mtl2dMappingQuad();
    newShape->init(_pmShapes.size(), true);
    _pmShapes.push_front(newShape);
}

//--------------------------------------------------------------
void mtl2dMapping::createTriangle(float _x, float _y)
{
    mtl2dMappingShape* newShape = new mtl2dMappingTriangle();
    newShape->init(_pmShapes.size(), true);
    _pmShapes.push_front(newShape);
}

//--------------------------------------------------------------
void mtl2dMapping::deleteShape()
{
    if (mtl2dMappingShape::activeShape) {
        _pmShapes.remove(mtl2dMappingShape::activeShape);
        mtl2dMappingShape::resetActiveShapeVars();
    }
}

//--------------------------------------------------------------
//void mtl2dMapping::disableAllShapes()
//{
//    // Disable all the shapes.
//    list<mtl2dMappingShape*>::iterator it;
//    for (it=_pmShapes.begin(); it!=_pmShapes.end(); it++) {
//        mtl2dMappingShape* shape = *it;
//        shape->disable();
//    }
//}

#pragma mark -
#pragma mark Mouse event
//--------------------------------------------------------------
void mtl2dMapping::mousePressed(int x, int y, int button)
{
    
    
    
    if (ControlsMapping::controlsMapping()->isHit(x, y))
        return;
    
    if(!ControlsMapping::controlsMapping()->editShapes())
        return;
    
    
    // ----
    // A vertex has been selected
    if (mtl2dMappingVertex::activeVertex || button == 2) {
      return;
    }
    
    // ----
    // Select an existing shape
    list<mtl2dMappingShape*>::iterator it;
    for (it=_pmShapes.begin(); it!=_pmShapes.end(); it++) {
        mtl2dMappingShape* shape = *it;
        bool grabbedOne = false;
        if(ControlsMapping::controlsMapping()->mappingMode() == MAPPING_MODE_OUTPUT) {
            if(shape->hitTest(x, y)) {
                grabbedOne = true;
                shape->enable();
            }
        } else if (ControlsMapping::controlsMapping()->mappingMode() == MAPPING_MODE_INPUT) {
            if(shape->inputPolygon->hitTest(x, y)) {
                grabbedOne = true;
                shape->inputPolygon->enable();
            }
        }
        
        if(grabbedOne) {
            // Put active shape at the top of the list
            _pmShapes.push_front(shape);
            _pmShapes.erase(it);
            
            return;
        }
    }
    
    // ----
    if(mtl2dMappingSettings::kIsManuallyAddingDeletingVertexEnabled && ControlsMapping::controlsMapping()->mappingMode() == MAPPING_MODE_OUTPUT) {
        // Add vertex to the selected shape
        if(mtl2dMappingShape::activeShape) {
            mtl2dMappingShape* shape = mtl2dMappingShape::activeShape;
            if (shape) {
                ofLog(OF_LOG_NOTICE, "Add vertex to shape %i", shape->shapeId);
                shape->addPoint(x, y);
            } else {
                ofLog(OF_LOG_NOTICE, "No shape has been selected, can not add a vertex");
            }
        }
    }
    
}

#pragma mark -
#pragma mark Keyboard event
//--------------------------------------------------------------
void mtl2dMapping::keyPressed(int key)
{    
    // ----
    switch (key) {
        case 9:  // TAB
            ofToggleFullscreen();
            break;
            
        case 'l':
            ControlsMapping::controlsMapping()->disable();
            
            break;
            
        case 'm':
            ControlsMapping::controlsMapping()->toggle();
            
            break;
            
        case 's':
            ControlsMapping::controlsMapping()->save();
            saveShapesList("mapping/xml/shapes.xml");

            break;
            
        case 356:
            //left
            if (ControlsMapping::controlsMapping()->editShapes()) {
                if(mtl2dMappingShape::activeShape && mtl2dMappingShape::activeShapeCurrVertexId >=0) {
                    mtl2dMappingVertex::activeVertex->left();
                }
            }
            break;
                
        case 357: //up
            if (ControlsMapping::controlsMapping()->editShapes()) {
                if(mtl2dMappingShape::activeShape && mtl2dMappingShape::activeShapeCurrVertexId >=0) {
                    mtl2dMappingVertex::activeVertex->up();
                }
            }
            break;
        
        case 358: //right
            if (ControlsMapping::controlsMapping()->editShapes()) {
                if(mtl2dMappingShape::activeShape && mtl2dMappingShape::activeShapeCurrVertexId >=0) {
                    mtl2dMappingVertex::activeVertex->right();
                }
            }
            break;
                
        case 359: //down
            if (ControlsMapping::controlsMapping()->editShapes()) {
                if(mtl2dMappingShape::activeShape && mtl2dMappingShape::activeShapeCurrVertexId >=0) {
                    mtl2dMappingVertex::activeVertex->down();
                }
            }
            break;
            
        case 127:
            deleteShape();
            break;
            
        case 8:
            deleteShape();
            break;
            
        case 'n':
            if (ControlsMapping::controlsMapping()->editShapes()) {
                if(mtl2dMappingShape::activeShape) {
                    mtl2dMappingShape::activeShapeCurrVertexId++;
                    if(mtl2dMappingShape::activeShapeCurrVertexId >=  mtl2dMappingShape::activeShape->vertices.size()) {
                        mtl2dMappingShape::activeShapeCurrVertexId = 0;
                    }
                    
                    //-----
                    list<mtl2dMappingVertex*>::iterator it;
                    
                    int vertexCounter = 0;
                    for (it=mtl2dMappingShape::activeShape->vertices.begin(); it!=mtl2dMappingShape::activeShape->vertices.end(); it++) {
                        mtl2dMappingVertex* vertex = *it;
                        
                        if(vertexCounter == mtl2dMappingShape::activeShapeCurrVertexId) {
                            vertex->setAsActive();
                            break;
                        }
                        
                        vertexCounter++;
                    }
                }
            }
            break;
    }
}


#pragma mark -
#pragma mark Load and Save Shapes List
//--------------------------------------------------------------
void mtl2dMapping::loadShapesList(string _xmlPath)
{
    // Delete everything
    while(!_pmShapes.empty()) delete _pmShapes.back(), _pmShapes.pop_back();
    _pmShapes.clear();
    mtl2dMappingShape::resetActiveShapeVars();
    
    int nbOfShapes = -1;
    
    //LOAD XML
    // ----
	//the string is printed at the top of the app
	//to give the user some feedback
	string feedBackMessage = "loading shapes.xml";
	ofLog(OF_LOG_NOTICE, "Status > " + feedBackMessage);
    
	//we load our settings file
	//if it doesn't exist we can still make one
	//by hitting the 's' key
	if( _shapesListXML.loadFile(_xmlPath) ){
		feedBackMessage = "shapes.xml loaded!";
	}else{
		feedBackMessage = "unable to load shapes.xml check data/ folder";
	}
    ofLog(OF_LOG_NOTICE, "Status > " + feedBackMessage);
    
    // ----
	//this is a more advanced use of ofXMLSettings
	//we are going to be reading multiple tags with the same name
	
	//lets see how many <root> </root> tags there are in the xml file
	int numRootTags = _shapesListXML.getNumTags("root");
	int numShapeTags = 0;
	
	//if there is at least one <root> tag we can read the list of cards
	//and then load their associated xml file
	if(numRootTags > 0){
		//we push into the last <root> tag
		//this temporarirly treats the tag as
		//the document root.
		_shapesListXML.pushTag("root", 0);
		
		//we see how many params/items we have stored in <card> tags
		numShapeTags = _shapesListXML.getNumTags("shape");
        ofLog(OF_LOG_NOTICE, "Status > numShapeTags :: " + ofToString(numShapeTags));
		
		if(numShapeTags > 0){			
			for(int i = 0; i < numShapeTags; i++){
				mtl2dMappingShape* newShape;
                nbOfShapes++;
				
//				int id = _shapesListXML.getAttribute("shape", "id", 0, i);
				
				_shapesListXML.pushTag("shape", i);
				
                //SHAPES SETTINGS
                int numShapeSettingTags = _shapesListXML.getNumTags("setting");
                
                string shapeType = _shapesListXML.getValue("setting", "nan", 0);

                if (shapeType == "quad") {
                     newShape = new mtl2dMappingQuad();
                } else if (shapeType == "triangle") {
                    newShape = new mtl2dMappingTriangle();
                } else {
                    newShape = new mtl2dMappingQuad();
                }
                
                if(numShapeSettingTags > 0) {
                    for(int j = 0; j < numShapeSettingTags; j++){
                        string key = _shapesListXML.getAttribute("setting", "key", "nc", j); 
                        string value = _shapesListXML.getValue("setting", "", j);
                        newShape->shapeSettings[key] = value;                        
                    }
                }	
				
                //OUTPUT VERTICES
                _shapesListXML.pushTag("outputVertices", 0);
                int numVertexItemTags = _shapesListXML.getNumTags("vertex");
                for (int j = 0; j < numVertexItemTags; j++) {
                    int x = _shapesListXML.getAttribute("vertex", "x", 0, j); 
                    int y = _shapesListXML.getAttribute("vertex", "y", 0, j);
                    
                    //Create a new vertex
                    mtl2dMappingVertex* newVertex = new mtl2dMappingVertex();
                    newVertex->init(x-newVertex->width/2, y-newVertex->height/2);
                    newShape->vertices.push_back(newVertex);
                }
                _shapesListXML.popTag();
                
                
                //INPUT QUADS
                _shapesListXML.pushTag("inputPolygon", 0);

                //Create a new vertex
                newShape->inputPolygon = new mtl2dMappingInput();
            
                //INPUT VERTICES
                numVertexItemTags = _shapesListXML.getNumTags("vertex");
                for (int k = 0; k < numVertexItemTags; k++) {
                    int x = _shapesListXML.getAttribute("vertex", "x", 0, k); 
                    int y = _shapesListXML.getAttribute("vertex", "y", 0, k);
                    
                    //Create a new vertex
                    mtl2dMappingVertex* newVertex = new mtl2dMappingVertex();
                    newVertex->init(x-newVertex->width/2, y-newVertex->height/2);
                    newVertex->isDefiningTectureCoord = true;
                    newShape->inputPolygon->vertices.push_back(newVertex);
                }
                
                newShape->inputPolygon->init(nbOfShapes);
                _shapesListXML.popTag();
                
                
                newShape->init(nbOfShapes);
                _pmShapes.push_front(newShape);
				
				_shapesListXML.popTag();
				
			}
		}
		
		//this pops us out of the <root> tag
		//sets the root back to the xml document
		_shapesListXML.popTag();
	}
}

//--------------------------------------------------------------
void mtl2dMapping::saveShapesList(string _xmlPath)
{
    
    list<mtl2dMappingShape*> pmShapesCopy;
    pmShapesCopy.resize (_pmShapes.size());
    copy (_pmShapes.begin(), _pmShapes.end(), pmShapesCopy.begin());
    pmShapesCopy.sort(Comparator());

    ofxXmlSettings newShapesListXML;
	int shapeCounter = 0;
	
	newShapesListXML.addTag("root");
	newShapesListXML.pushTag("root", 0);
	
	//Create/Update XML
    list<mtl2dMappingShape*>::reverse_iterator it;
    for (it=pmShapesCopy.rbegin(); it!=pmShapesCopy.rend(); it++) {
        mtl2dMappingShape* shape = *it;
		
		int tagNum = newShapesListXML.addTag("shape");
		newShapesListXML.addAttribute("shape", "id", shape->shapeId, tagNum);
		newShapesListXML.pushTag("shape", tagNum);
		
        //Shape settings
        map<string,string>::iterator itShape;
        for ( itShape=shape->shapeSettings.begin() ; itShape != shape->shapeSettings.end(); itShape++ ) {
            int tagNum = newShapesListXML.addTag("setting");
            cout << (*itShape).first << " => " << (*itShape).second << endl;
            
            newShapesListXML.addAttribute("setting", "key", (*itShape).first, tagNum);
            newShapesListXML.setValue("setting", (*itShape).second, tagNum);
        }
		
        //Output Vertex/Vertices
        tagNum = newShapesListXML.addTag("outputVertices");
        newShapesListXML.pushTag("outputVertices", tagNum);
        list<mtl2dMappingVertex*>::iterator itVertex;
        for (itVertex=shape->vertices.begin(); itVertex!=shape->vertices.end(); itVertex++) {
            mtl2dMappingVertex* vertex = *itVertex;
            
            int tagNum = newShapesListXML.addTag("vertex");
            newShapesListXML.addAttribute("vertex", "x", (int)vertex->center.x, tagNum);
            newShapesListXML.addAttribute("vertex", "y", (int)vertex->center.y, tagNum);
        }
        newShapesListXML.popTag();
        
        //Input Quads
        tagNum = newShapesListXML.addTag("inputPolygon");
        newShapesListXML.pushTag("inputPolygon", tagNum);
        //Vertices
        for (itVertex=shape->inputPolygon->vertices.begin(); itVertex!=shape->inputPolygon->vertices.end(); itVertex++) {
            mtl2dMappingVertex* vertex = *itVertex;
            
            int tagNum = newShapesListXML.addTag("vertex");
            newShapesListXML.addAttribute("vertex", "x", (int)vertex->center.x, tagNum);
            newShapesListXML.addAttribute("vertex", "y", (int)vertex->center.y, tagNum);
        }
        newShapesListXML.popTag();
		
		newShapesListXML.popTag();
		
		shapeCounter++;
	}
	
	//Save to file
	newShapesListXML.saveFile(_xmlPath);
    ofLog(OF_LOG_NOTICE, "Status > settings saved to xml!");

}

//--------------------------------------------------------------
void mtl2dMapping::chessBoard(int nbOfCol)
{
    ofSetColor(ofColor::white);
    ofFill();
    
    ofSeedRandom(0);
    
    int boardWidth = _fbo.getWidth();
    int boardHeight = _fbo.getHeight();
    
    float squareSize = boardWidth / nbOfCol;
    int nbOfRow = ceil(boardHeight / squareSize);
    for (int colId = 0; colId < nbOfCol; colId++) {
        for (int rowId = 0; rowId < nbOfRow; rowId++) {
            if ((colId + rowId) % 2 == 0) {
                ofSetColor(ofColor::white);
            } else {
                ofSetColor(ofColor::black);
            }
            
            if ((rowId) % 6 == 0) {
                ofSetColor(  ofColor::fromHsb( (rowId * 50) % 255 , 255, 255));
            }
            
            else if ((colId) % 6 == 0) {
                ofSetColor(  ofColor::fromHsb((colId * 50) % 255 , 255, 255));
            }
            
            
            ofRect(colId * squareSize, rowId * squareSize, squareSize, squareSize);
        }
    }
    
    //Frame
    ofNoFill();
    ofSetColor(ofColor::yellow);
    glLineWidth(8.0f);
    ofRect(.0f, .0f, boardWidth, boardHeight);
    glLineWidth(1.0f);
    
    ofFill();
    ofSetColor(ofColor::red);
    ofRect(60.0f, .0f, 20.0f, 20.0f);
    ofSetColor(ofColor::green);
    ofRect(80.0f, .0f, 20.0f, 20.0f);
    ofSetColor(ofColor::blue);
    ofRect(100.0f, .0f, 20.0f, 20.0f);
    
    ofSetColor(ofColor::white);
}


