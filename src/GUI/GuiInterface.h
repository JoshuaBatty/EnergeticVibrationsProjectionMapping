//
//  GuiInterface.hpp
//  EnergeticVibrations_ProjectionMapping
//
//  Created by Joshua Batty on 11/1/18.
//
//

#pragma once 

#include "ofMain.h"
#include "Gradient.h"
#include "CustomSlider.h"
#include "AudioToggles.h"
#include "ofxImGui.h"
#include "VisualLayer.h"
#include "PiMapperHelper.h"
#include "ConvertAssetColour.h"

struct ShaderToggle{
    bool b;
    ImTextureID buttonID;
};

struct ShaderState{
    //Shader Sliders
    vector<CustomSlider*> sliders;
    
    //Audio Select Toggles
    vector<AudioToggles> toggles;
};


class GuiInterface{
    public:
    GuiInterface();
    ~GuiInterface();
    void init_window_flags();
    void init_shader_variables(vector<VisualLayer*> &layers);
    void setup(ofxImGui::Gui &gui, ofxPiMapper& mapper, int num_layers, ofVec4f theme_colour);
    void setup_add_shape(ofxImGui::Gui &gui);
    void setup_shader_toggles(vector<VisualLayer*> &layers);
    void setup_mapping_panel(ofxImGui::Gui &gui);
    void setup_selected_layer(ofxImGui::Gui &gui);
    
    void push_back_shader_toggle(ofxImGui::Gui &gui, vector<VisualLayer*> &layers);
    
    void update_audio_reactivity(vector<VisualLayer*> &layers);
    void update_active_shader(int selected);
    void update_active_sliders();
    
    void draw(vector<ShaderVariable> &shader_variables);
    
    void draw_border(ofRectangle rect);
    void draw_border_background(ofRectangle rect);
    
    void draw_add_shape(ofRectangle rect);
    void draw_selected_layer(ofRectangle rect, vector<ShaderVariable> &shader_variables);
    void draw_audio_analysis(ofRectangle rect);
    void draw_shader_toggles(ofRectangle rect);
    void draw_duplicate_and_remove(ofRectangle rect);
    void draw_mapping_panel(ofRectangle rect);
    
    void set_theme_colour(ofVec4f theme_colour);
    
    void update_volumes(vector<float> volumes);
    vector<float> volumes;

    bool is_mouse_over_mapping_toggles();
	bool is_touch_over_mapping_toggles(ofVec2f);
    bool is_mouse_inside_mapping_rect();
    
    int get_selected_shader();
    
    // Event from PiMapper when sureface is selected
    void onSurfaceSelected(int & surfaceIndex);

	//----------------WINDOWS ONLY
	/* Touches */
	void touchDown(map<int, ofTouchEventArgs> touchMap);
	void touchMoved(map<int, ofTouchEventArgs> touchMap);
	void touchUp(map<int, ofTouchEventArgs> touchMap);

private:
    PiMapperHelper map_helper;
    
    ImGuiWindowFlags window_flags;
    
    ofRectangle add_shape_rect;
    ofRectangle selected_layer_rect;
    ofRectangle audio_analysis_rect;
    ofRectangle shader_toggles_rect;
    ofRectangle mapping_panel_rect;
    ofRectangle duplicate_remove_rect;
  
    ofTrueTypeFont font_large;
    ofTrueTypeFont font_mid;
    
    int param_gui_offset;
    ofVec2f padding; // used to offset the titles from their containing rectangle pos
    
    int num_layers;
    
    ofVec4f theme_colour;
    ConvertAssetColour asset_colour;

    /// Add Shape Panel
    GLuint quad_buttonID;
    GLuint triangle_buttonID;
    
    /// Mapping Panel
//    ofFbo mp_fbo;
//    ofShader mp_grid;
    GLuint remove_button_ID;
    GLuint duplicate_button_ID;
    bool mouse_over_remove_toggle;
    bool mouse_over_duplicate_toggle;
    
    ofImage img_background;
    ofImage img; // background template ref

    //Red Gradient Shader
    Gradient slider_gradient;
        
	vector<ShaderState> shader_states;

    CustomSlider sb_slider;

    //Shader Selection Buttons
    vector<ShaderToggle> shader_toggles;
    int selected_shader;

    static constexpr float IMGUI_PADDING = 20;
	static constexpr float TOGGLE_WIDTH = 110.0;
	static constexpr float TOGGLE_HEIGHT = 45.0;

	static constexpr float REMOVE_POS_X = 165.0;
	static constexpr float REMOVE_POS_Y = 82.0;

};
