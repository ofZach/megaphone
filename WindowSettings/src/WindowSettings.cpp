//
//  WindowSettings.cpp
//  WindowSettings
//
//  Created by Elie Zananiri on 2012-11-02.
//
//

#include "WindowSettings.h"

#include "ofxXmlSettings.h"

int  gScreenWidth  = 1024;
int  gScreenHeight = 768;
int  gNumScreens   = 2;

//--------------------------------------------------------------
bool loadWindowSettings(const string& file)
{
    // load the settings from an XML file
    bool loadedOK = false;
    ofxXmlSettings xml;
    if (xml.loadFile(file)) {
        if (xml.pushTag("settings")) {
            for (int i = 0; i < xml.getNumTags("setting"); i++) {
                string name = xml.getAttribute("setting", "name", "", i);
                if (name.compare("screen_width") == 0) {
                    gScreenWidth = xml.getValue("setting", 1024, i);
                }
                else if (name.compare("screen_height") == 0) {
                    gScreenHeight = xml.getValue("setting", 768, i);
                }
                else if (name.compare("num_screens") == 0) {
                    gNumScreens = xml.getValue("setting", 2, i);
                }
                else {
                    ofLog(OF_LOG_ERROR, "Unrecognized window settings key " + name);
                }
            }

            loadedOK = true;
        }
        xml.popTag();
    }

    return loadedOK;
}
