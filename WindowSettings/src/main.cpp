#include "ofMain.h"
#include "testApp.h"
#include "ofAppGlutWindow.h"
#include "WindowSettings.h"

//========================================================================
int main()
{
    loadWindowSettings("window-settings.xml");

    ofAppGlutWindow window;
    window.setGlutDisplayString("rgba double samples>=4 depth");
	ofSetupOpenGL(&window, gDisplayWidth, gDisplayHeight, OF_WINDOW);
	ofRunApp(new testApp());
}
