/*
 *  CustomSlider.cpp
 *  Created by Golan Levin on 2/24/12.
 *
 */

#include "CustomSlider.h"

//----------------------------------------------------
CustomSlider::CustomSlider(){
	bWasSetup = false;
}

//----------------------------------------------------
CustomSlider::~CustomSlider(){
	clear();
}

//-----------------------------------------------------------------------------------------------------------------------
void CustomSlider::setup(float inx, float iny, float inw, float inh, float loVal, float hiVal, float initialValue, bool bVert, bool bDrawNum){
	x = inx;
	y = iny; 
	width = inw; 
	height = inh;
	box.set(x,y, width, height); 
	numberDisplayPrecision = 2;
	
	bVertical = bVert;
	bDrawNumber = bDrawNum;
	bHasFocus = false;

	lowValue = loVal;
	highValue = hiVal;
	percent = ofMap(initialValue, lowValue,highValue, 0,1); 
	percent = ofClamp(percent, 0,1);
	
	labelString = ""; 
	
    thumb_radius = 20;
    
	if(!bWasSetup){
		ofAddListener(ofEvents().draw, this, &CustomSlider::draw);
		ofAddListener(ofEvents().mouseMoved, this, &CustomSlider::mouseMoved);
		ofAddListener(ofEvents().mousePressed, this, &CustomSlider::mousePressed);
		ofAddListener(ofEvents().mouseReleased, this, &CustomSlider::mouseReleased);
		ofAddListener(ofEvents().mouseDragged, this, &CustomSlider::mouseDragged);
		bWasSetup = true;
	}
}

//----------------------------------------------------
void CustomSlider::clear(){
	if(bWasSetup){
		ofRemoveListener(ofEvents().draw, this, &CustomSlider::draw);
		ofRemoveListener(ofEvents().mouseMoved, this, &CustomSlider::mouseMoved);
		ofRemoveListener(ofEvents().mousePressed, this, &CustomSlider::mousePressed);
		ofRemoveListener(ofEvents().mouseReleased, this, &CustomSlider::mouseReleased);
		ofRemoveListener(ofEvents().mouseDragged, this, &CustomSlider::mouseDragged);
	}
	bWasSetup = false;
}

//----------------------------------------------------
void CustomSlider::setLabelString (string str){
	labelString = str;
}


//----------------------------------------------------
void CustomSlider::draw(ofEventArgs& event){
	
	ofPushStyle();
	ofEnableAlphaBlending();
	ofDisableSmoothing();
	ofPushMatrix();
	ofTranslate(x,y,0);
	
	// Use different alphas if we're actively maniupulating me. 
	float spineAlpha  = (bHasFocus) ? 192:128;
	
	// draw box outline
	ofNoFill();
	ofSetLineWidth(18.0);
	ofSetColor(25,25,25, 255);
    ofDrawLine(-10, 0, width+10, 0);
	ofDrawLine(-10,height, width+10,height);
    ofSetLineWidth(10.0);
    ofDrawLine(-5, 0, -5, height);
    ofDrawLine(width+5, 0, width+5, height);

	// draw spine
	ofSetLineWidth(1.0);
	ofSetColor(255,255,255, spineAlpha);
	if (bVertical){
		ofDrawLine(width/2,0, width/2,height); 
	} else {
		ofDrawLine(0,height/2, width,height/2); 
	}
	
	// draw thumb
    ofSetCircleResolution(160);
    float thumbX = ofMap(percent, 0,1, 0 + thumb_radius,width - thumb_radius, true);
	ofSetLineWidth(3.0);
	ofSetColor(25, 25, 25, 255);
    ofDrawCircle(thumbX, height/2, 22);
    ofSetColor(255, 26, 34, 255);
    ofFill();
    ofDrawCircle(thumbX, height/2, thumb_radius);
	
	// draw numeric value 
	if (bHasFocus){
		ofSetColor(0); 
	} else {
		ofSetColor(128); 
	}
	if (bVertical){
		ofDrawBitmapString( ofToString(getValue(),numberDisplayPrecision), width+5,height);
	} else {
		ofDrawBitmapString( ofToString(getValue(),numberDisplayPrecision), width+5,height/2 + 4);
		
		float labelStringWidth = labelString.size();
		ofDrawBitmapString( labelString, 0-labelStringWidth*8-5, height/2 + 4); 
	}	

	ofPopMatrix();
	ofPopStyle();
}

//----------------------------------------------------
float CustomSlider::getValue(){
	// THIS IS THE MAIN WAY YOU GET THE VALUE FROM THE SLIDER!
	float out = ofMap(percent, 0,1, lowValue,highValue, true); 
	return out;
}


//----------------------------------------------------
// Probably not used very much. 
float CustomSlider::getLowValue(){
	return lowValue;
}
float CustomSlider::getHighValue(){
	return highValue;
}
float CustomSlider::getPercent(){
	return percent;
}

//----------------------------------------------------
// Probably not used very much. 
void CustomSlider::setLowValue(float lv){
	lowValue = lv;
}
void CustomSlider::setHighValue(float hv){
	highValue = hv; 
}
void CustomSlider::setPercent (float p){
	// Set the slider's percentage from the outside. 
	p = ofClamp(p, 0,1);
	percent	= p;
}
void CustomSlider::setNumberDisplayPrecision(int prec){
	numberDisplayPrecision = prec;
}
		
//----------------------------------------------------
void CustomSlider::mouseMoved(ofMouseEventArgs& event){
	bHasFocus = false;
}
void CustomSlider::mouseDragged(ofMouseEventArgs& event){
	if (bHasFocus){
		updatePercentFromMouse (event.x, event.y); 
	}
}
void CustomSlider::mousePressed(ofMouseEventArgs& event){
	bHasFocus = false;
	if (box.inside(event.x, event.y)){
		bHasFocus = true;
		updatePercentFromMouse (event.x, event.y); 
	}
}
void CustomSlider::mouseReleased(ofMouseEventArgs& event){
	if (bHasFocus){
		if (box.inside(event.x, event.y)){
			updatePercentFromMouse (event.x, event.y); 
		}
	}
	bHasFocus = false;
}

//----------------------------------------------------
void CustomSlider::updatePercentFromMouse (int mx, int my){
    percent = ofMap(mx, x + thumb_radius, (x+width) - thumb_radius,  0,1, true);
}
		

