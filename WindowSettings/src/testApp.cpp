#include "testApp.h"

#include "WindowSettings.h"

//--------------------------------------------------------------
void testApp::setup()
{
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    ofDisableArbTex();

    ballPos.set(gCanvasWidth / 2, gCanvasHeight / 2);
    ballRadius = 20;
    ballSpeed = 5;

    float texWidth;
    float texHeight;
    if (ofGetUsingArbTex()) {
        _canvasFbo.allocate(gScreenWidth, gScreenHeight * gNumScreens);
        texWidth = ofNextPow2(gScreenWidth);
        texHeight = ofNextPow2(gScreenHeight);
    }
    else {
        _canvasFbo.allocate(ofNextPow2(gScreenWidth), ofNextPow2(gScreenHeight) * gNumScreens);
        texWidth = gScreenWidth / (float)ofNextPow2(gScreenWidth);
        texHeight = gScreenHeight / (float)ofNextPow2(gScreenHeight);
    }
    _numVerts = gNumScreens * 4;
    _texCoords = new GLfloat[_numVerts * 2];
    for (int i = 0; i < gNumScreens; i++) {
        _texCoords[i * 4 * 2 + 0] = 0;                                  // x0
        _texCoords[i * 4 * 2 + 1] = texHeight * i / gNumScreens;        // y0
        _texCoords[i * 4 * 2 + 2] = texWidth;                           // x1
        _texCoords[i * 4 * 2 + 3] = texHeight * i / gNumScreens;        // y1
        _texCoords[i * 4 * 2 + 4] = texWidth;                           // x2
        _texCoords[i * 4 * 2 + 5] = texHeight * (i + 1) / gNumScreens;  // y2
        _texCoords[i * 4 * 2 + 6] = 0;                                  // x3
        _texCoords[i * 4 * 2 + 7] = texHeight * (i + 1) / gNumScreens;  // y3
    }
    _verts = new GLfloat[_numVerts * 2];
    for (int i = 0; i < gNumScreens; i++) {
        _verts[i * 4 * 2 + 0] = gDisplayWidth * i / gNumScreens;        // x0
        _verts[i * 4 * 2 + 1] = 0;                                      // y0
        _verts[i * 4 * 2 + 2] = gDisplayWidth * (i + 1) / gNumScreens;  // x1
        _verts[i * 4 * 2 + 3] = 0;                                      // y1
        _verts[i * 4 * 2 + 4] = gDisplayWidth * (i + 1) / gNumScreens;  // x2
        _verts[i * 4 * 2 + 5] = gDisplayHeight;                         // y2
        _verts[i * 4 * 2 + 6] = gDisplayWidth * i / gNumScreens;        // x3
        _verts[i * 4 * 2 + 7] = gDisplayHeight;                         // y3
    }
}

//--------------------------------------------------------------
void testApp::update()
{
    _canvasFbo.begin();
    {
        ofClear(128, 0);

        ballPos.y += ballSpeed;
        if ((ballPos.y + ballRadius) > gCanvasHeight) {
            ballPos.y = -ballRadius;
        }

        ofSetColor(ofColor::white);
        ofCircle(ballPos, ballRadius);
    }
    _canvasFbo.end();
}

//--------------------------------------------------------------
void testApp::draw()
{
    _canvasFbo.getTextureReference().bind();
    {
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glTexCoordPointer(2, GL_FLOAT, 0, _texCoords);
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(2, GL_FLOAT, 0, _verts);
        glDrawArrays(GL_QUADS, 0, _numVerts);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }
    _canvasFbo.getTextureReference().unbind();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

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