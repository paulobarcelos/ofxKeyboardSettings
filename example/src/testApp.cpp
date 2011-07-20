#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){	
	// setup the keyboard interface
	settings.setup('s',			// access key (hold for 4 seconds to toogle the settings accessibility on/off)
				   "Settings"	// settings label (will define the xml filename)
				   );
	
	// Add a control to a float property (this will automatically try to load it from the settings file, if it's not found, it will be automatically added with the default value);
	settings.addProperty(&circleRadius,		// pointer to var
						 'r',				// access key (the key you will need t press to access this property once the settings are active)
						 "Circle Radius",	// property label (will define the xml tag)
						 20.f,				// min
						 50.f,				// max
						 1.5f,				// step
						 30.f				// default value, in case there is no settings file
						 );
	
	numCircles = 2;
	drawFill = true;
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
	for (int i = 0; i < numCircles; i++) {
		if(drawFill)ofFill();
		else ofNoFill();
		ofCircle(50.f + i*50.f, ofGetHeight() / 2, circleRadius);
	}
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

	settings.proccessKey(key);
}
