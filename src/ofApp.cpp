#include "ofApp.h"
#include "nodel/nodel_dep.hpp"

//changed
//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofDisableArbTex(); // Use GL_TEXTURE_2D Textures (normalized texture coordinates 0..1)
    ofBackground(0);
    ofEnableSmoothing();

	mouse = true;
	//----------------WINDOWS ONLY
#ifdef WINDOWS_TOUCH
	mouse = false;
	// enable the Windows Touch Hook
	ofxWinTouchHook::EnableTouch();

	// add touch listeners
	ofAddListener(ofxWinTouchHook::touchDown, this, &ofApp::touchDown);
	ofAddListener(ofxWinTouchHook::touchMoved, this, &ofApp::touchMoved);
	ofAddListener(ofxWinTouchHook::touchUp, this, &ofApp::touchUp);
#endif

    isShaderDirty = true; // initialise dirty shader

    ofDirectory shader_dir;
    shader_dir.listDir("shaders/Synths/");
    shader_dir.sort();
    
    cout << "dir size = " << shader_dir.size() << endl;
    
    static int num_layers = shader_dir.size();
    
    for(int i = 0; i < num_layers; i++){
        VisualLayer *layer = new VisualLayer();
        layers.push_back(layer);
        layers[i]->setup("Shader" + ofToString(1+i), i);        
        mapper.registerFboSource(layers[i]);
    }
    

    
    ofx::piMapper::VideoSource::enableAudio = false;
    ofx::piMapper::VideoSource::useHDMIForAudio = false;
    mapper.setup();
    
  
    //load fonts first
    gui_theme.load_font();
    
    //required call
    gui.setup();
    
    //load theme
    gui_theme.init_theme();
    
    gui_interface.setup(gui, mapper, num_layers);
    gui_interface.setup_shader_toggles(layers);
    
	clear_touch_in_two_frames = 0;

    projection_fbo.allocate(998,899,GL_RGBA);
    
    // OSC
    volumes = {1.0f,1.0f,1.0f};
    receiver.setup(OSC_PORT);
	nodel_interpreter::NodelDep nodel_dep(mapper, layers);
	nodel.setup(nodel_dep);
}



//--------------------------------------------------------------
void ofApp::setupProjectionWindow(){
    ofSetBackgroundColor(0);
    mapper._application.getSurfaceManager()->assign_projection_fbo(&projection_fbo);
    
    surface_mask.setup();
}

//--------------------------------------------------------------
void ofApp::drawProjections(ofEventArgs & args){
    ofShowCursor();
    
 
   if(projection_fbo.isAllocated()){
        projection_fbo.getTexture().draw(0,0,ofGetWidth(), ofGetHeight());
       
       surface_mask.set_source_texture(projection_fbo);
       surface_mask.update();
       surface_mask.draw();
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    if (isShaderDirty){
        // dirty shader pattern:
        shared_ptr<ofShader> tmpShader = shared_ptr<ofShader>(new ofShader);
        tmpShader->setGeometryInputType(GL_TRIANGLES);
        tmpShader->setGeometryOutputType(GL_TRIANGLE_STRIP);
        tmpShader->setGeometryOutputCount(4);
        
        if (tmpShader->load("shaders/passthrough.vert","shaders/shader_selector.frag")){
            scene_shader = tmpShader;
            
            for(int i = 0; i < layers.size(); i++){
                layers[i]->set_scene_shader(scene_shader);                
            }
            
            ofLogNotice() << ofGetTimestampString() << " reloaded Shader.";
        } else {
            ofLogError() << "Shader did not load successfully.";
        }
        isShaderDirty = false;
    }
    
    
	// This is hear incase we really need to start hacking ImGui
	// TO enable mouse events to be set from the touch screen. 
	// We need to call this function 2 frames after the last touch event happened 
	// So that button is hovered can be cleared. https://github.com/ocornut/imgui/issues/1470
	//if (ofGetFrameNum() - clear_touch_in_two_frames == 2) {
	//	ImGuiIO *io = &ImGui::GetIO();
	//	io->MousePos = ImVec2(-FLT_MAX, -FLT_MAX);
	//	cout << "frame cleared! -- frame num = " << ofGetFrameNum() << endl;
	//}

    update_osc();
    mapper.update();
	gui_interface.update_volumes(volumes);
    gui_interface.update_audio_reactivity(layers);
	
	nodel.try_run();
			
}

//--------------------------------------------------------------
void ofApp::update_osc(){
    // check for waiting messages
    while(receiver.hasWaitingMessages()){
        // get the next message
        ofxOscMessage m;
        receiver.getNextMessage(m);
        
        if(m.getAddress() == "/caco/0"){
            //cout << "m.getArgAsFloat(0) = " << m.getArgAsFloat(0) << endl;
            float bass = ofClamp(m.getArgAsFloat(2), 0.0, 1.0);
            float mid = ofClamp(m.getArgAsFloat(3), 0.0, 1.0);
            float high = ofClamp(m.getArgAsFloat(4), 0.0, 1.0);
            float b_shape = 4.0;
            float m_shape = 18.0;
            float h_shape = 18.0;
            float b_amp = ofMap(powf(bass - 1.0, b_shape), 1.0, 0.0, 0.0, 1.5);
            float m_amp = ofMap(powf(mid - 1.0, m_shape), 1.0, 0.0, 0.0, 3.0);
            float h_amp = ofMap(powf(high - 1.0, h_shape), 1.0, 0.0, 0.0, 10.0);

            float b_smooth = 0.1; // Make smaller for smoother
            float m_smooth = 0.1; // Make smaller for smoother
            float h_smooth = 0.05; // Make smaller for smoother

            //-------- Bass
            if (b_amp > volumes[0]) {
                volumes[0] = b_amp;
            } else {
                float diff = b_amp - volumes[0];
                volumes[0] += diff * b_smooth;
            }
            //-------- Mid
            if (m_amp > volumes[1]) {
                volumes[1] = m_amp;
            } else {
                float diff = m_amp - volumes[1];
                volumes[1] += diff * m_smooth;
            }
            //-------- High
            if (h_amp > volumes[2]) {
                volumes[2] = h_amp;
            } else {
                float diff = h_amp - volumes[2];
                volumes[2] += diff * h_smooth;
            }
        
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){

	ofShowCursor();
    this->gui.begin();
    
    int selected_layer = gui_interface.get_selected_shader();
    gui_interface.draw(layers[selected_layer]->shader_params[selected_layer]);
    
    this->gui.end();

    ofSetColor(255,255);
    mapper.draw();

//    int size = 100;
//    for(int i = 0; i < layers.size(); i++){
//        layers[i]->FboSource::drawPreview(i * size, ofGetHeight()-size, size, size);
//    }
    //layers[0]->hue_fbo.fbo.draw(0,0,ofGetWidth(),ofGetHeight());

    //cout << "selected surface = " << mapper._application.getSurfaceManager()->getSelectedSurface() << endl;
    
//    mapper._application.Gui::getSourcesEditorWidget();
//    ofx::piMapper::Gui::getSourcesEditorWidget().getLoadedTexCount();
}

void ofApp::toggle_shaders() {
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key) {
        case ' ':
            isShaderDirty = true;
            break;
        case 'm':
            mouse = !mouse;
            if (mouse) {
                cout << "Mouse mode on" << endl;
            }
            else {
                cout << "Touch mode on" << endl;
            }
            break;
        default:
            break;
	}

	mapper.keyPressed(key);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
		//mapper.keyReleased(key);

}


//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	if (mouse) {
		mapper.mouseDragged(x, y, button);
	}
	
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    // allow the ImGui duplicate and remove buttons to take precedence
    // over the layer selection and deselection. otherwise the layers
    // deselect when the button is clicked. This results in no selected objects
    if(!gui_interface.is_mouse_over_mapping_toggles()){
        // Make sure that the mouse is actually within the mapping
        // rectangle before registering mouse events so we dont
        // deselect the currently selected layer.
        if(gui_interface.is_mouse_inside_mapping_rect()){
			if (mouse) {
				mapper.mousePressed(x, y, button);
			}
        }
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	if (mouse) {
		mapper.mouseReleased(x, y, button);
	}

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}


#ifdef WINDOWS_TOUCH
//----------------WINDOWS ONLY
//--------------------------------------------------------------
void ofApp::touchDown(ofTouchEventArgs & touch){
	if (!gui_interface.is_touch_over_mapping_toggles(touch)) {
		// Make sure that the mouse is actually within the mapping
		// rectangle before registering mouse events so we dont
		// deselect the currently selected layer.

			touchMap[touch.id] = touch;
			gui_interface.touchDown(touchMap);
			if (!mouse) {
				mapper.touchDown(touchMap);
			}
	}

	// This is hear incase we really need to start hacking ImGui
	// TO enable mouse events to be set from the touch screen. 
	//ImGuiIO *io = &ImGui::GetIO();
	//io->MousePos = ImVec2(touch.x, touch.y);
	//io->MouseDown[0] = true;
}

//--------------------------------------------------------------
void ofApp::touchMoved(ofTouchEventArgs & touch){
    touchMap[touch.id] = touch;
	gui_interface.touchMoved(touchMap);
	if (!mouse) {
		mapper.touchMoved(touchMap);
	}

}

//--------------------------------------------------------------
void ofApp::touchUp(ofTouchEventArgs & touch){
  //TODO this probably needs to be erased after passing in?
	touchMap[touch.id] = touch;
	gui_interface.touchUp(touchMap);
	if (!mouse) {
		mapper.touchUp(touchMap);
	}
	touchMap.erase(touch.id);

	// This is hear incase we really need to start hacking ImGui
	// TO enable mouse events to be set from the touch screen. 
	//ImGuiIO *io = &ImGui::GetIO();
	//io->MouseDown[0] = false;
	//clear_touch_in_two_frames = ofGetFrameNum();
}
#endif
