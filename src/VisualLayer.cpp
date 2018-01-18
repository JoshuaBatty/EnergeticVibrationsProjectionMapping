//
//  VisualLayer.cpp
//  EnergeticVibrations_ProjectionMapping
//
//  Created by Joshua Batty on 30/10/17.
//
//

#include "VisualLayer.h"

//--------------------------------------------------------------
VisualLayer::VisualLayer(){
}

//--------------------------------------------------------------
void VisualLayer::init_params(){
    shader_params.clear();
    
    for(int i = 0; i < 7; i++){
        shader_params.push_back(ShaderParams());
    }
    
//    shader_params[HEXAGON_GRADIENT].params = {0.5,0.2,0.0};
//    shader_params[HEXAGON_GRADIENT].names = {"speed","circle_iter","iter"};
    
    shader_params[COLOUR_GRADIENT].params = {0.1,0.02,0.0,0.0};
    shader_params[COLOUR_GRADIENT].names = {"col_iter_x", "col_iter_y", "speed", "HUESHIFT"};
    
    shader_params[ESCHER_LIKE].params = {0.5,0.0,1.0,0.0};
    shader_params[ESCHER_LIKE].names = {"circle_offset", "circle_iter", "grid_iter", "HUESHIFT"};
    
    shader_params[FLOWER_OF_LIFE].params = {1.0,0.0,0.3,0.0};
    shader_params[FLOWER_OF_LIFE].names = {"brightness","time_scale","grid_size", "HUESHIFT"};

    shader_params[TRI_LATTICE].params = {1.0,0.0,1.0,0.0};
    shader_params[TRI_LATTICE].names = {"num_triangles","intensity","colour mix", "HUESHIFT"};
    
    shader_params[RADIAL_HEXAGON].params = {0.0,1.0,0.0,0.0};
    shader_params[RADIAL_HEXAGON].names = {"circle_iter","grid_size","time_skip", "HUESHIFT"};

    shader_params[OP_ART_TWISTER].params = {1.0,1.0,0.0,0.0};
    shader_params[OP_ART_TWISTER].names = {"iter_size","grid_size","speed", "HUESHIFT"};

    shader_params[PATTERN_MESH_2D].params = {0.0,0.0,0.0,0.0};
    shader_params[PATTERN_MESH_2D].names = {"speed","shape_iter","grid_iter", "HUESHIFT"};
}

//--------------------------------------------------------------
void VisualLayer::setup(string name, int scene_num){
    
    this->name = name; // Give our source a decent name
    scene_select = scene_num;
    allocate(LAYER_RENDER_SIZE_X, LAYER_RENDER_SIZE_Y); // Allocate our FBO source, decide how big it should be
    
    render_fbo.init(LAYER_RENDER_SIZE_X, LAYER_RENDER_SIZE_Y);
    
    scene_shader.load("shaders/passthrough.vert","shaders/shader_selector.frag");
    
    init_params();
}

//--------------------------------------------------------------
void VisualLayer::load_movie(string file){
    player.load(file);
}

//--------------------------------------------------------------
void VisualLayer::update(){
    //hue_offset = ofGetElapsedTimef() * 0.1;
    
/*
    if(ofGetFrameNum() % 120 == 0){
        scene_select = (int)ofRandom(8);
        hue_offset = ofRandom(PI*2);
        for(int i = 0; i < 8; i++){
            shader_params[i].params = {ofRandomuf(),ofRandomuf(),ofRandomuf()};
        }
    }
*/
    
    render_fbo.fbo.begin();
        ofClear(0,0,0,0);
        scene_shader.begin();
        scene_shader.setUniform3f("resolution", LAYER_RENDER_SIZE_X, LAYER_RENDER_SIZE_Y, 1);
        scene_shader.setUniform1f("time", ofGetElapsedTimef());
        scene_shader.setUniform1i("iFrame", ofGetFrameNum());
        scene_shader.setUniform1i("scene_select", scene_select);
        scene_shader.setUniform1f("param1", shader_params[scene_select].params[0]);
        scene_shader.setUniform1f("param2", shader_params[scene_select].params[1]);
        scene_shader.setUniform1f("param3", shader_params[scene_select].params[2]);
        scene_shader.setUniform1f("hue_offset", shader_params[scene_select].params[3]);
        ofDrawRectangle(0, 0, render_fbo.fbo.getWidth(), render_fbo.fbo.getHeight());

        scene_shader.end();
    render_fbo.fbo.end();
}

//--------------------------------------------------------------
void VisualLayer::draw(){
    ofClear(0,0,0,0);
    render_fbo.fbo.draw(0,0,fbo->getWidth(),fbo->getHeight());
}
