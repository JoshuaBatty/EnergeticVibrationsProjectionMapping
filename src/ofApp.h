#pragma once

//#define WINDOWS_TOUCH
#define OSC_PORT 9002

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxImGui.h"
#include "GuiTheme.h"
#include "GuiInterface.h"
#include "ofxJSON.h"

#include "SurfaceMask.h"
#include "VisualLayer.h"
#include "Settings.h"
#include "ofxPiMapper.h"

#include "nodel/nodel_interpreter.hpp"

//----------------WINDOWS ONLY
#ifdef WINDOWS_TOUCH
#include "ofxWinTouchHook.h"
#endif

#include <fstream>

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
    
        void build_shader_src();
    
        void setupProjectionWindow();
        void drawProjections(ofEventArgs & args);
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
        //----------------WINDOWS ONLY
#ifdef WINDOWS_TOUCH
        // Touches //
        void touchDown(ofTouchEventArgs & touch);
        void touchMoved(ofTouchEventArgs & touch);
        void touchUp(ofTouchEventArgs & touch);
        map<int, ofTouchEventArgs> touchMap;
#endif
        ///------------- OSC
        void update_osc();
        ofxOscReceiver receiver;
        vector<float> volumes;

        ///------------- MAPPING
        ofxPiMapper mapper;
        ofFbo projection_fbo;
        SurfaceMask surface_mask;
    
        ///------------- LAYERS
        vector<VisualLayer*> layers;
//    shared_ptr<ofShader> scene_shader;
        ofShader scene_shader;
        bool isShaderDirty;
    
    ofShader s;


        ///------------- GUI
        ofxImGui::Gui gui;
        GuiTheme gui_theme;
		int clear_touch_in_two_frames;
    
        // Gui Themes Colour
        ofVec4f theme_colour;
    
        GuiInterface gui_interface;

private:
		// Tom added to turn mouse on / off
		bool mouse;
		void toggle_shaders();
		nodel_interpreter::Nodel nodel;
};
