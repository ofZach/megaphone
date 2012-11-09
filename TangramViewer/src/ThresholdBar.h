//
//  ofxInputSensorBar.h
//  pituApp02
//
//  Created by Patricio González Vivo on 8/6/12.
//  Copyright (c) 2012 PatricioGonzalezVivo.com. All rights reserved.
//

#ifndef THRESHOLDBAR
#define THRESHOLDBAR

#include "ofMain.h"
#include "ofxXmlSettings.h"

class ThresholdBar : public ofRectangle {
public:
    
    ThresholdBar();

    void    setLabel(string label = "none");
    void    setMin(float _minValue){minValue = _minValue;};
    void    setMax(float _maxValue){maxValue = _maxValue;};
    
    void    setVerticalOriented( bool _vert = true){ vertOriented = _vert;};
    void    setThreshold(float _threshold);
    float   getThreshold(){return threshold;};
    
    void    setValue(float _value);
    float   getValue(){return value;};
    float   getActiveValue();
    
    ofEvent<float>  thresholdTrigger;
    
    void    updateGUI();
    void    draw();
    
    ofxXmlSettings XML;
private:
    string  label;
    ofColor color;
    float   maxValue;
    float   minValue;
    float   threshold;
    float   value;
    
    bool    vertOriented;
};
#endif
