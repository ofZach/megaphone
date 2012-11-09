/***********************************************************************
 
 Copyright (c) 2008, 2009, 2010, Memo Akten, www.memo.tv
 *** The Mega Super Awesome Visuals Company ***
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of MSA Visuals nor the names of its contributors 
 *       may be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS 
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE. 
 *
 * ***********************************************************************/ 
//

#include "ofxMSAInteractiveObjectMod.h"
#include "ofMain.h"


//

ofxMSAInteractiveObjectMod::ofxMSAInteractiveObjectMod() {
	_mouseOver	= false;
	_mouseDown	= false;
	enabled		= true;
	verbose		= false;
	
	enableAppEvents();
	disableMouseEvents();
	disableKeyEvents();
}

ofxMSAInteractiveObjectMod::~ofxMSAInteractiveObjectMod() {
    try {  
        disableAllEvents();  
    } catch (Poco::SystemException) {  
        return; // we're leaving anyways so no need to delete  
    }
}

void ofxMSAInteractiveObjectMod::killMe() {
	delete this;
}

void ofxMSAInteractiveObjectMod::enableAllEvents() {
	enableMouseEvents();
	enableKeyEvents();
	enableAppEvents();
}

void ofxMSAInteractiveObjectMod::disableAllEvents() {
	disableMouseEvents();
	disableKeyEvents();
	disableAppEvents();
}


void ofxMSAInteractiveObjectMod::enableMouseEvents() {
	ofAddListener(mtlScaleOption::instance().ofEvents().mousePressed, this, &ofxMSAInteractiveObjectMod::_mousePressed);
	ofAddListener(mtlScaleOption::instance().ofEvents().mouseMoved, this, &ofxMSAInteractiveObjectMod::_mouseMoved);
	ofAddListener(mtlScaleOption::instance().ofEvents().mouseDragged, this, &ofxMSAInteractiveObjectMod::_mouseDragged);
	ofAddListener(mtlScaleOption::instance().ofEvents().mouseReleased, this, &ofxMSAInteractiveObjectMod::_mouseReleased);
}

void ofxMSAInteractiveObjectMod::disableMouseEvents() {
	ofRemoveListener(mtlScaleOption::instance().ofEvents().mousePressed, this, &ofxMSAInteractiveObjectMod::_mousePressed);
	ofRemoveListener(mtlScaleOption::instance().ofEvents().mouseMoved, this, &ofxMSAInteractiveObjectMod::_mouseMoved);
	ofRemoveListener(mtlScaleOption::instance().ofEvents().mouseDragged, this, &ofxMSAInteractiveObjectMod::_mouseDragged);
	ofRemoveListener(mtlScaleOption::instance().ofEvents().mouseReleased, this, &ofxMSAInteractiveObjectMod::_mouseReleased);
}

void ofxMSAInteractiveObjectMod::enableKeyEvents() {
	ofAddListener(ofEvents().keyPressed, this, &ofxMSAInteractiveObjectMod::_keyPressed);
	ofAddListener(ofEvents().keyReleased, this, &ofxMSAInteractiveObjectMod::_keyReleased);
}

void ofxMSAInteractiveObjectMod::disableKeyEvents() {
	ofRemoveListener(ofEvents().keyPressed, this, &ofxMSAInteractiveObjectMod::_keyPressed);
	ofRemoveListener(ofEvents().keyReleased, this, &ofxMSAInteractiveObjectMod::_keyReleased);
}

void ofxMSAInteractiveObjectMod::enableAppEvents() {
	ofAddListener(ofEvents().setup, this, &ofxMSAInteractiveObjectMod::_setup);
	ofAddListener(ofEvents().update, this, &ofxMSAInteractiveObjectMod::_update);
	ofAddListener(ofEvents().draw, this, &ofxMSAInteractiveObjectMod::_draw);
	ofAddListener(ofEvents().exit, this, &ofxMSAInteractiveObjectMod::_exit);
}

void ofxMSAInteractiveObjectMod::disableAppEvents() {
	ofRemoveListener(ofEvents().setup, this, &ofxMSAInteractiveObjectMod::_setup);
	ofRemoveListener(ofEvents().update, this, &ofxMSAInteractiveObjectMod::_update);
	ofRemoveListener(ofEvents().draw, this, &ofxMSAInteractiveObjectMod::_draw);
	ofRemoveListener(ofEvents().exit, this, &ofxMSAInteractiveObjectMod::_exit);
}


void ofxMSAInteractiveObjectMod::setPos(float _x, float _y) {
	x = _x;
	y = _y;
}

void ofxMSAInteractiveObjectMod::setSize(float _w, float _h) {
	width = _w;
	height = _h;
}

void ofxMSAInteractiveObjectMod::setPosAndSize(float _x, float _y, float _w, float _h) {
	setPos(_x, _y);
	setSize(_w, _h);
}

bool ofxMSAInteractiveObjectMod::isMouseOver() {
	return _mouseOver;
}

bool ofxMSAInteractiveObjectMod::isMouseDown() {
	return _mouseDown;
}

int ofxMSAInteractiveObjectMod::getMouseX() {
	return _mouseX;
}

int ofxMSAInteractiveObjectMod::getMouseY() {
	return _mouseY;
}

int ofxMSAInteractiveObjectMod::getLastMouseButton() {
	return _mouseButton;
}

bool ofxMSAInteractiveObjectMod::hitTest(int tx, int ty) {
	return ((tx > x) && (tx < x + width) && (ty > y) && (ty < y + height));
}



void ofxMSAInteractiveObjectMod::_setup(ofEventArgs &e) {
	if(!enabled) return;
	setup();
}

void ofxMSAInteractiveObjectMod::_update(ofEventArgs &e) {
	if(!enabled) return;
	
	// check to see if object has moved, and if so update mouse events
	if(oldRect.x != this->x || oldRect.y != this->y || oldRect.width != this->width ||oldRect.height != this->height) {
		ofMouseEventArgs e;
		e.button = _mouseButton;
		e.x = _mouseX;
		e.y = _mouseY;
		if(_mouseDown) _mouseDragged(e);	
		else _mouseMoved(e);
		
		oldRect =  (ofRectangle) (*this);
	}
	update();
}

void ofxMSAInteractiveObjectMod::_draw(ofEventArgs &e) {
	if(!enabled) return;
	draw();
}

void ofxMSAInteractiveObjectMod::_exit(ofEventArgs &e) {
	if(!enabled) return;
	exit();
}


void ofxMSAInteractiveObjectMod::_mouseMoved(ofMouseEventArgs &e) {
	int x = e.x;
	int y = e.y;
	int button = e.button;
	if(verbose) printf("ofxMSAInteractiveObjectMod::_mouseMoved(x: %i, y: %i)\n", x, y);
	if(!enabled) return;
	
	_mouseX = x;
	_mouseY = y;
	
	if(hitTest(x, y)) {						// if mouse is over the object
		if(!_mouseOver) {						// if wasn't over previous frame
			onRollOver(x, y);						// call onRollOver
			_mouseOver = true;						// update flag
		}
		onMouseMove(x, y);						// and trigger onMouseMove
	} else if(_mouseOver) {					// if mouse is not over the object, but the flag is true (From previous frame)
		onRollOut();							// call onRollOut
		_mouseOver = false;						// update flag
	}
}


void ofxMSAInteractiveObjectMod::_mousePressed(ofMouseEventArgs &e) {
	int x = e.x;
	int y = e.y;
	int button = e.button;
	
	if(verbose) printf("ofxMSAInteractiveObjectMod::_mousePressed(x: %i, y: %i, button: %i)\n", x, y, button);
	if(!enabled) return;

	_mouseX = x;
	_mouseY = y;
	_mouseButton = button;
	
	if(hitTest(x, y)) {						// if mouse is over
		if(!_mouseDown) {						 // if wasn't down previous frame
			onPress(x, y, button);					// call onPress
			_mouseDown = true;						// update flag
		}
	} else {								// if mouse is not over
		onPressOutside(x, y, button);
	}
}

void ofxMSAInteractiveObjectMod::_mouseDragged(ofMouseEventArgs &e) {
	int x = e.x;
	int y = e.y;
	int button = e.button;
	
	if(verbose) printf("ofxMSAInteractiveObjectMod::_mouseDragged(x: %i, y: %i, button: %i)\n", x, y, button);
	if(!enabled) return;

	_mouseX = x;
	_mouseY = y;
	_mouseButton = button;

	if(hitTest(x, y)) {						// if mouse is over the object
		if(!_mouseOver) {						// if wasn't over previous frame
			//				onPress(x, y);							// call onPress - maybe not
			_mouseOver = true;						// update flag
		}
		onDragOver(x, y, button);				// and trigger onDragOver
	} else {
		if(_mouseOver) {					// if mouse is not over the object, but the flag is true (From previous frame)
			onRollOut();							// call onRollOut
			_mouseOver = false;						// update flag
		}
		if(_mouseDown) {
			onDragOutside(x, y, button);
		}
	}
}

void ofxMSAInteractiveObjectMod::_mouseReleased(ofMouseEventArgs &e) {
	int x = e.x;
	int y = e.y;
	int button = e.button;
	
	if(verbose) printf("ofxMSAInteractiveObjectMod::_mouseReleased(x: %i, y: %i, button: %i)\n", x, y, button);
	if(!enabled) return;

	_mouseX = x;
	_mouseY = y;
	_mouseButton = button;
	
	if(hitTest(x, y)) {
		onRelease(x, y, button);
	} else {
		if(_mouseDown) onReleaseOutside(x, y, button);
	}
	_mouseDown = false;
}


void ofxMSAInteractiveObjectMod::_keyPressed(ofKeyEventArgs &e) {
	int key = e.key;
	if(verbose) printf("ofxMSAInteractiveObjectMod::_keyPressed(key: %i)\n", key);
	if(!enabled) return;
	keyPressed(key);
}


void ofxMSAInteractiveObjectMod::_keyReleased(ofKeyEventArgs &e) {
	int key = e.key;	
	if(verbose) printf("ofxMSAInteractiveObjectMod::_keyReleased(key: %i)\n", key);
	if(!enabled) return;
	keyReleased(key);
}
