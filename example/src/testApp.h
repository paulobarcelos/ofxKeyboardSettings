#pragma once

#include "ofMain.h"
#include "ofxKeyboardSettings.h"

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
	
		ofxKeyboardSettings settings;
	
		float	circleRadius;
		int		numCircles;
		bool	drawFill;
		
};
