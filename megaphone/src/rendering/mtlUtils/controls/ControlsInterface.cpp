//
//  ControlsInterface.cpp
//  ofxMTL2DMapping
//
//  Created by Elie Zananiri on 12-04-18.
//

#include "ControlsInterface.h"

//--------------------------------------------------------------
ControlsInterface::ControlsInterface(int width, ofColor color, const string& file)
{
    _canvas = new ofxUIScrollableCanvas(0, 0, width, ofGetHeight());
    //must have modified ofxUI
    //_canvas->setStickyDistance(30.0f);
    _canvas->setScrollableDirections(false, true);
    _canvas->setColorBack(color);
    
    _file = file;
}

//--------------------------------------------------------------
bool ControlsInterface::getToggleValue(const string& name)
{
    return ((ofxUIToggle *)_canvas->getWidget(name))->getValue();
}

//--------------------------------------------------------------
float ControlsInterface::getSliderValue(const string& name)
{
    return ((ofxUISlider *)_canvas->getWidget(name))->getScaledValue();
}

//--------------------------------------------------------------
ofPoint ControlsInterface::get2DPadValue(const string& name)
{
    return ((ofxUI2DPad *)_canvas->getWidget(name))->getScaledValue();
}

//--------------------------------------------------------------
void ControlsInterface::save()
{
    _canvas->saveSettings(_file);
}

//--------------------------------------------------------------
void ControlsInterface::load()
{
    _canvas->loadSettings(_file);
}

//--------------------------------------------------------------
void ControlsInterface::enable()
{
    _canvas->enable();
}

//--------------------------------------------------------------
void ControlsInterface::disable()
{
    _canvas->disable();
}

//--------------------------------------------------------------
void ControlsInterface::toggle()
{
    _canvas->toggleVisible();
}

//--------------------------------------------------------------
bool ControlsInterface::isEnabled()
{
    return _canvas->isEnabled();
}

//--------------------------------------------------------------
bool ControlsInterface::isHit(int x, int y) {
    return _canvas->isHit(x, y);
}

