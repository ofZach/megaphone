//
//  WindowSettings.h
//  WindowSettings
//
//  Created by Elie Zananiri on 2012-11-02.
//
//

#pragma once

#include "ofMain.h"

#define gCanvasWidth    (gScreenWidth)
#define gCanvasHeight   (gScreenHeight * gNumScreens)

#define gDisplayWidth   (gScreenWidth * gNumScreens)
#define gDisplayHeight  (gScreenHeight)

extern int  gScreenWidth;
extern int  gScreenHeight;
extern int  gNumScreens;

bool loadWindowSettings(const string& file);
