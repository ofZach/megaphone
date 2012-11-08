//
//  ControlsMapping.h
//  ofxMTL2DMapping
//
//  Created by Hugues Bruyere on 12-06-03.
//

#pragma once

#include "ofEvents.h"

#include "ControlsInterface.h"
#include "mtl2dMappingMode.h"

#define kControlsMappingPanelWidth 320

#define kSettingMappingSave                 "MAPPING:SAVE"
#define kSettingMappingLoad                 "MAPPING:LOAD"

#define kSettingMappingEditShapes           "MAPPING:EDIT_SHAPES"
#define kSettingMappingCreateNewQuad        "MAPPING:CREATE_NEW_QUAD"
#define kSettingMappingCreateNewTriangle    "MAPPING:CREATE_NEW_TRAINGLE"

#define kSettingMappingShowShapesId         "MAPPING:SHOW_SHAPES_ID"

#define kSettingMappingModeOutput           "MAPPING_MODE:OUTPUT"
#define kSettingMappingModeInput            "MAPPING_MODE:INPUT"

//--------------------------------------------------------------
//--------------------------------------------------------------
class ControlsMapping : public ControlsInterface
{
    private:
        static ControlsMapping *_controlsMapping;
        
        bool _saveMapping;
        bool _loadMapping;
        
        bool _editShapes;
        bool _createNewQuad;
        bool _createNewTriangle;
        bool _showShapesId;
    
        bool _mappingModeChanged;
        mtl2dMappingMode _mappingMode;
                            
    public:
        static ControlsMapping * controlsMapping();

        ControlsMapping(int width, const string& file);
        
        void uiEvent(ofxUIEventArgs &event);
    
        const bool& saveMapping() { return _saveMapping; }
        void resetSaveMapping();
        const bool& loadMapping() { return _loadMapping; }
        void resetLoadMapping();
    
        const bool& editShapes() { return _editShapes; }
        const bool& createNewQuad() { return _createNewQuad; }
        const bool& createNewTriangle() { return _createNewTriangle; }
        void resetCreateNewShape();
        const bool& showShapesId() { return _showShapesId; }
    
        const bool& mappingModeChanged() { return _mappingModeChanged; }
        mtl2dMappingMode mappingMode() { return _mappingMode; }
        void resetMappingChangedFlag();

    
};
