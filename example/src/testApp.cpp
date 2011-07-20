#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){	
	// setup the keyboard interface
	settings.setup('s',			// access key (hold for 4 seconds to toogle the settings accessibility on/off)
				   "Settings"	// settings label (will define the xml filename) !!Spaces are not allowed!!
				   );
	
	// Add a control to a float property (this will automatically try to load it from the settings file, if it's not found, it will be automatically added with the default value);
	settings.addProperty(&circleRadius,		// pointer to var
						 'r',				// access key (the key you will need t press to access this property once the settings are active)
						 "Circle_Radius",	// property label (will define the xml tag) !!Spaces are not allowed!!
						 20.f,				// min
						 50.f,				// max
						 1.5f,				// step
						 30.f				// default value
						 );
	
	// Add a control to a int property 
	settings.addProperty(&numCircles,
						 'n',
						 "Number_of_Circles",
						 1,
						 5,
						 1,
						 1
						 );
	
	// Add a control to a bool property 
	settings.addProperty(&drawFill,
						 'f',
						 "Fill_Shape",
						 true
						 );
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
	ofSetColor(255, 0, 0);
	for (int i = 0; i < numCircles; i++) {
		if(drawFill)ofFill();
		else ofNoFill();
		ofCircle(50.f + i*100.f, ofGetHeight() / 2, circleRadius);
	}
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

	settings.proccessKey(key);
}
