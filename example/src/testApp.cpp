#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){	
	// setup the keyboard interface
	settings.setup('s',			// access key (hold for 3 seconds to toogle the settings accessibility on/off)
				   "Settings"	// settings label (will define the xml filename) !!Spaces are not allowed!!
				   );
	
	// We are going to add some property "monitors" (this means we won't be able to change the value, just monitor them...)
	
	// Adding a "monitor" to a static function (useful for helpers like, ofGetFrameRate(), of ofGetWidth(), etc...)
	settings.addProperty(&ofGetFrameRate,	// pointer to function
						 "App_FPS"			// property label
						 );
	
	// Adding a "monitor" to an object function
	/*settings.addProperty(&greyscale,			// pointer to object
						 &Grayscale::getGrey,	// pointer to object function
						 "Greyscale_Control"	// label
						 );*/
	
	// Adding a "monitor" to any variable (needs to be able to be converted to string using ofToString())
	settings.addProperty(&circleRadius,				// pointer to var
						 "Circle_Radius_Monitor"	// property label
						 );
	
	
	// Now add the properties that we can actually control the values...
	
	// Add a float property (this will automatically try to load it from the settings file, if it's not found, it will be automatically added with the default value);
	settings.addProperty(&circleRadius,		// pointer to var
						 "Circle_Radius",	// property label (will define the xml tag) !!Spaces are not allowed!!
						 20.f,				// min
						 50.f,				// max
						 1.5f,				// step
						 30.f				// default value
						 );
	// (The above syntax will be the same for int, float, short, long or double) 
	
	// Add a control to a bool property (this will automatically try to load it from the settings file, if it's not found, it will be automatically added with the default value);
	settings.addProperty(&drawFill,		// pointer to var
						 "Fill_Shape",	// property label (will define the xml tag) !!Spaces are not allowed!!
						 true			// default value
						 );
	
	
	// This is very useful (but the code has a bit more boilerplate),
	// we can add properties that are controled by getters and setters...
	// For flexibility the getters/setters can be in different objects (so if your property don't have a min()/max()/step(), you can creta a custom object to handle them)
	settings.addProperty(&greyscale, &Grayscale::getGrey,		// pointer to get object, pointer to get function
						 &greyscale, &Grayscale::setGrey,		// pointer to set object, pointer to set function
						 "Greyscale_Control",					// property label (will define the xml tag) !!Spaces are not allowed!!
						 &greyscale, &Grayscale::getGreyMin,	// pointer to min object, pointer to min function
						 &greyscale, &Grayscale::getGreyMax,	// pointer to max object, pointer to max function
						 &greyscale, &Grayscale::getGreyStep,	// pointer to step object, pointer to step function
						 0										// default value
						 );
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
	ofSetColor(greyscale.getGrey());
	if(drawFill)ofFill();
	else ofNoFill();
	ofCircle(ofGetWidth()/2, ofGetHeight()/2, circleRadius);
	
	settings.draw(0,0);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

	settings.keyPressed(key);
}
