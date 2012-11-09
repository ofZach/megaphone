//
//  File.cpp
//  pituApp02
//
//  Created by Patricio González Vivo on 8/6/12.
//  Copyright (c) 2012 PatricioGonzalezVivo.com. All rights reserved.
//

#include "ThresholdBar.h"

ThresholdBar::ThresholdBar(){
    x = 0;
    y = 0;
    width = 100;
    height = 10;
    
    minValue = 0.0;
    maxValue = 1.0;
    
    color.set(255,0,0);
    
    vertOriented = false;
};

void ThresholdBar::setLabel(string _label){
    label = _label;
    
    ofxXmlSettings XML;
    if (XML.loadFile("gui.xml")){
        XML.pushTag(label,0);
        threshold = XML.getValue("thr", 0.0);
        minValue = XML.getValue("min", 0.0);
        maxValue = XML.getValue("max", 0.0);
        x = XML.getValue("x", 0.0);
        y = XML.getValue("y", 0.0);
        width = XML.getValue("width", 100.0);
        height = XML.getValue("height", 10.0);
        XML.popTag();
    }
}

void ThresholdBar::setThreshold(float _threshold){
    threshold = _threshold;
    ofxXmlSettings XML;
    
    if (XML.loadFile("gui.xml")){
        if( XML.pushTag(label)){
            XML.setValue("thr", threshold);
            XML.setValue("min", minValue);
            XML.setValue("max", maxValue);
            XML.popTag();
            XML.saveFile("gui.xml");
        }
    }
};

void ThresholdBar::setValue(float _value){
    value = ofMap(_value, minValue, maxValue, 0.0, 1.0, true);
    
    color.setHue(100-(value*100));
    
    if ( value >= threshold){
        ofNotifyEvent(thresholdTrigger, value);
    }
};

float ThresholdBar::getActiveValue(){
    if ( value >= threshold ){
        return value - threshold;
    } else {
        return 0.0;
    }
};

void ThresholdBar::updateGUI(){
    if ( ofGetMousePressed() ){
        float mouseX = ofGetMouseX();
        float mouseY = ofGetMouseY();
        
        if ((mouseX >= x ) && 
            (mouseX <= x + width) &&
            (mouseY >= y ) && 
            (mouseY <= y + height)){
            
            if ( !vertOriented ){
                setThreshold( (mouseX - x) / width );
            } else {
                setThreshold( 1.0 - ((mouseY - y) / height) );
            }
        }
    }
    
    
};

void ThresholdBar::draw(){
    // draw the average volume:
	ofPushStyle();
    ofPushMatrix();
    ofTranslate(x, y, 0);
    
    
    if ( !vertOriented ){
        
        //  Box bar
        //
        ofNoFill();		
        ofSetColor(255,100);
        ofRect(0, 0, width, height);
        
        //  Value bar
        //
        float padding = height*0.2;
        ofFill();
        ofSetColor(color,200);
        ofRect( padding, padding, min(value*width, width-padding*2) , height-padding*2);
    
        //  Threshold Line
        //
        ofSetColor(255,150);
        ofLine(threshold*width, 0, 
               threshold*width, height);
        
        //  Draw Label
        //
        ofSetColor(255);
        ofDrawBitmapString(label, width + 5, height - 2);
    } else {
        //  Box bar
        //
        ofNoFill();		
        ofSetColor(255,100);
        ofRect(0, 0, width, height);
        
        //  Value bar
        //
        float padding = width*0.2;
        ofFill();
        ofSetColor(color,200);
        ofRect( padding,height - padding - min( value*height + padding, height + padding) , width-padding*2 , min(value*height,height-padding*2) );
        
        //  Threshold Line
        //
        ofSetColor(255,150);
        ofLine(0, height - threshold*height, 
               width, height - threshold*height);
    }
    
    ofPopMatrix();
	ofPopStyle();
};