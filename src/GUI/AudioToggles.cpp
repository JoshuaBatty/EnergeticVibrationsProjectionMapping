//
//  AudioToggles.cpp
//  EnergeticVibrations_ProjectionMapping
//
//  Created by Joshua Batty on 16/1/18.
//
//

#include "AudioToggles.h"

//------------------------------------
void AudioToggles::setup(ofxImGui::Gui &gui){
    selected = 0;
    
    vector<string> names = {"off_", "bass_", "mid_", "high_"};
    
    for(int i = 0; i < 4; i++){
        Toggle t;
        t.b = false;
        if(i==0) t.b = true;
        
        ofImage image_on;
        ofImage image_off;
        image_on.load("buttons/" + names[i] + "on.png");
        image_off.load("buttons/" + names[i] + "off.png");
        t.buttonOnID = gui.loadImage(image_on);
        t.buttonOffID = gui.loadImage(image_off);
        
        toggles.push_back(t);
    }
}

//------------------------------------
void AudioToggles::draw(string name, ofVec2f pos, ofVec2f size){

    
    auto mainSettings = ofxImGui::Settings();
    mainSettings.windowPos = ofVec2f(pos.x-15, pos.y-15);
    mainSettings.windowSize = ofVec2f(size.x, size.y);
    
    static bool no_titlebar = true;
    static bool no_border = true;
    static bool no_resize = true;
    static bool no_move = true;
    static bool no_scrollbar = true;
    static bool no_collapse = true;
    static bool no_menu = true;
    
    // Demonstrate the various window flags. Typically you would just use the default.
    ImGuiWindowFlags window_flags = 0;
    if (no_titlebar)  window_flags |= ImGuiWindowFlags_NoTitleBar;
    if (!no_border)   window_flags |= ImGuiWindowFlags_ShowBorders;
    if (no_resize)    window_flags |= ImGuiWindowFlags_NoResize;
    if (no_move)      window_flags |= ImGuiWindowFlags_NoMove;
    if (no_scrollbar) window_flags |= ImGuiWindowFlags_NoScrollbar;
    if (no_collapse)  window_flags |= ImGuiWindowFlags_NoCollapse;
    if (!no_menu)     window_flags |= ImGuiWindowFlags_MenuBar;
    
    
    if (ofxImGui::BeginWindow(name, mainSettings, window_flags))
    {
        for(int i = 0; i < toggles.size(); i++){
            ImTextureID texID = (ImTextureID)(uintptr_t) (toggles[i].b ? toggles[i].buttonOnID : toggles[i].buttonOffID);
            
            if(ImGui::ImageButton(texID, ImVec2(73,52))){
                toggles[i].b = true;
                selected = i;
            }ImGui::SameLine(0,3); // squish the toggles closer togther
        }
    }
    ofxImGui::EndWindow(mainSettings);
    
    // Make sure only the selected shaders toggle is active.
    for(int i = 0; i < toggles.size(); i++){
        if(i != selected){
            toggles[i].b = false;
        }
    }
    
}
