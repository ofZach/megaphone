//
//  ControlsMapping.cpp
//  ofxMTL2DMapping
//
//  Created by Hugues Bruyere on 12-06-03.
//

#include "ControlsMapping.h"
#include "mtl2dMappingSettings.h"

#include "ofMain.h"

//--------------------------------------------------------------
ControlsMapping * ControlsMapping::_controlsMapping = NULL;

//--------------------------------------------------------------
ControlsMapping * ControlsMapping::controlsMapping() 
{
    if (_controlsMapping == NULL) {
        _controlsMapping = new ControlsMapping(kControlsMappingPanelWidth, "mapping/controls/mapping.xml");
    }
    return _controlsMapping;
}

//--------------------------------------------------------------
ControlsMapping::ControlsMapping(int width, const string& file)
    : ControlsInterface(width, ofColor(0, 0, 255, 130), file)
{
    // set default values
    _saveMapping = false;
    _loadMapping = false;
    _editShapes = false;
    _createNewQuad = false;
    _createNewTriangle = false;
    _showShapesId = false;
    _mappingModeChanged = true; // initialized to true so that when the app launch the 'shapes' are correctly setted.
    _mappingMode = MAPPING_MODE_OUTPUT;
    
    static const int kWidgetWidth = width - 16;
    
    // add save / load controls
    _canvas->addWidgetDown(new ofxUILabel("SAVE MAPPING", OFX_UI_FONT_MEDIUM)); 
    _canvas->addWidgetDown(new ofxUIButton(kToggleSize, kToggleSize, false, kSettingMappingSave));
    _canvas->addWidgetDown(new ofxUIButton(kToggleSize, kToggleSize, false, kSettingMappingLoad));
        
    // add mapping controls
    ofxUISpacer *spacer = new ofxUISpacer(kWidgetWidth, kSpacerHeight);
    spacer->setDrawFill(false);
    _canvas->addWidgetDown(spacer);
    _canvas->addWidgetDown(new ofxUILabel("MAPPING", OFX_UI_FONT_MEDIUM)); 
    _canvas->addWidgetDown(new ofxUIToggle(kToggleSize, kToggleSize, _editShapes, kSettingMappingEditShapes));
    
    // add mapping mode controls
    _canvas->addWidgetDown(spacer);
    _canvas->addWidgetDown(new ofxUILabel("MAPPING MODE", OFX_UI_FONT_MEDIUM)); 
    _canvas->addWidgetDown(new ofxUIToggle(kToggleSize, kToggleSize, false, kSettingMappingModeOutput));
    _canvas->addWidgetDown(new ofxUIToggle(kToggleSize, kToggleSize, false, kSettingMappingModeInput));
    
    // add mapping shape controls
    if (mtl2dMappingSettings::kIsManuallyCreatingShapeEnabled) {
        _canvas->addWidgetDown(spacer);
        _canvas->addWidgetDown(new ofxUILabel("ADD SHAPE", OFX_UI_FONT_MEDIUM)); 
        _canvas->addWidgetDown(new ofxUIToggle(kToggleSize, kToggleSize, _editShapes, kSettingMappingCreateNewQuad));
        _canvas->addWidgetDown(new ofxUIToggle(kToggleSize, kToggleSize, _editShapes, kSettingMappingCreateNewTriangle));
    }
    
    // add mapping shape's details
    _canvas->addWidgetDown(spacer);
    _canvas->addWidgetDown(new ofxUILabel("SHAPE DATA", OFX_UI_FONT_MEDIUM));
    _canvas->addWidgetDown(new ofxUIToggle(kToggleSize, kToggleSize, _editShapes, kSettingMappingShowShapesId));

    
    // ----
    ofAddListener(_canvas->newGUIEvent, this, &ControlsMapping::uiEvent);
    
    load();
    
    if (getToggleValue(kSettingMappingModeOutput)) {
        _mappingMode = MAPPING_MODE_OUTPUT;
    } else if (getToggleValue(kSettingMappingModeInput)) {
        _mappingMode = MAPPING_MODE_INPUT;
    }
    
    // ---- So that nobody touch those
    ((ofxUIToggle *)_canvas->getWidget(kSettingMappingLoad))->setVisible(false);
    
}

//--------------------------------------------------------------
void ControlsMapping::uiEvent(ofxUIEventArgs &event)
{
    string name = event.widget->getName();
        
    // ----
    if (name == kSettingMappingSave) {
        _saveMapping = getToggleValue(name);
    }
    else if (name == kSettingMappingLoad) {
        _loadMapping = getToggleValue(name);
    }
    
    // ---- Editing
    else if (name == kSettingMappingEditShapes) {
        _editShapes = getToggleValue(name);

        ((ofxUILabel *)_canvas->getWidget("MAPPING MODE"))->setVisible(_editShapes);
        ((ofxUIToggle *)_canvas->getWidget(kSettingMappingModeInput))->setVisible(_editShapes);
        ((ofxUIToggle *)_canvas->getWidget(kSettingMappingModeOutput))->setVisible(_editShapes);
        
        ((ofxUILabel *)_canvas->getWidget("SHAPE DATA"))->setVisible(_editShapes);
        ((ofxUIToggle *)_canvas->getWidget(kSettingMappingShowShapesId))->setVisible(_editShapes);
        
        ((ofxUILabel *)_canvas->getWidget("ADD SHAPE"))->setVisible(_editShapes);
        ((ofxUIToggle *)_canvas->getWidget(kSettingMappingCreateNewQuad))->setVisible(_editShapes);
        ((ofxUIToggle *)_canvas->getWidget(kSettingMappingCreateNewTriangle))->setVisible(_editShapes);
    }
    else if (name == kSettingMappingShowShapesId) {
        _showShapesId = getToggleValue(name);
    }
    
    
    // ---- Creating a new shape
    else if (name == kSettingMappingCreateNewQuad) {
        // will happen only if mtl2dMappingSettings::kIsManuallyCreatingShapeEnabled is true
        _createNewQuad = getToggleValue(name);
    }
    else if (name == kSettingMappingCreateNewTriangle) {
        // will happen only if mtl2dMappingSettings::kIsManuallyCreatingShapeEnabled is true
        _createNewTriangle = getToggleValue(name);
    }
    
    
    // ----
    if (getToggleValue(name)) {
        if (name == kSettingMappingModeOutput) {
            _mappingModeChanged = true;
            _mappingMode = MAPPING_MODE_OUTPUT;

            ((ofxUIToggle *)_canvas->getWidget(kSettingMappingModeInput))->setValue(false);
            
            // ----
            if (mtl2dMappingSettings::kIsManuallyCreatingShapeEnabled) {
                ((ofxUILabel *)_canvas->getWidget("ADD SHAPE"))->setVisible(true);
                ((ofxUIToggle *)_canvas->getWidget(kSettingMappingCreateNewQuad))->setVisible(true);
                ((ofxUIToggle *)_canvas->getWidget(kSettingMappingCreateNewTriangle))->setVisible(true);
            }
        }
        else if (name == kSettingMappingModeInput) {
            _mappingModeChanged = true;
            _mappingMode = MAPPING_MODE_INPUT;
            
            ((ofxUIToggle *)_canvas->getWidget(kSettingMappingModeOutput))->setValue(false);
            
            // ----
            if (mtl2dMappingSettings::kIsManuallyCreatingShapeEnabled) {
                ((ofxUILabel *)_canvas->getWidget("ADD SHAPE"))->setVisible(false);
                ((ofxUIToggle *)_canvas->getWidget(kSettingMappingCreateNewQuad))->setVisible(false);
                ((ofxUIToggle *)_canvas->getWidget(kSettingMappingCreateNewTriangle))->setVisible(false);
            }
        }
    }
}

#pragma mark -
#pragma mark Reset widgets
//--------------------------------------------------------------
void ControlsMapping::resetSaveMapping()
{
    _saveMapping = false;
    ((ofxUIToggle *)_canvas->getWidget(kSettingMappingSave))->setValue(false);
}

//--------------------------------------------------------------
void ControlsMapping::resetLoadMapping()
{
    _loadMapping = false;
    ((ofxUIToggle *)_canvas->getWidget(kSettingMappingLoad))->setValue(false);
}

//--------------------------------------------------------------
void ControlsMapping::resetCreateNewShape()
{
    _createNewQuad = false;
    ((ofxUIToggle *)_canvas->getWidget(kSettingMappingCreateNewQuad))->setValue(false);
    
    _createNewTriangle = false;
    ((ofxUIToggle *)_canvas->getWidget(kSettingMappingCreateNewTriangle))->setValue(false);
}

//--------------------------------------------------------------
void ControlsMapping::resetMappingChangedFlag()
{
    _mappingModeChanged = false;
}

