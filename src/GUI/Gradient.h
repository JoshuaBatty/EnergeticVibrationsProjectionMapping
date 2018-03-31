//
//  Gradient.hpp
//  EnergeticVibrations_ProjectionMapping
//
//  Created by Joshua Batty on 16/1/18.
//
//

#pragma once 

#include "ofMain.h"

class Gradient{
public:
    
    Gradient();
    
    void set_colour(float r, float g, float b);
    void draw(int is_slider, int verticle_or_horizontal, float perc, int x, int y, int w, int h);

private:
    /// Red Shader Gradient
    ofShader gradient;
};
