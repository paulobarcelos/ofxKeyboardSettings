#pragma once

#include "ofMain.h"
#include "ofxKeyboardSettings.h"

class Grayscale { //just to show how to handle properties controled by getters/setters	
	public:
		Grayscale(){grey = 0;};
		void	setGrey(int grey){this->grey = grey;};
		int		getGrey(){return grey;};
		int		getGreyMin(){return 0;};
		int		getGreyMax(){return 255;};
		int		getGreyStep(){return 1;};
	private:
		int		grey;	
};

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
	
		ofxKeyboardSettings settings;
	
		float		circleRadius;
		bool		drawFill;
		Grayscale	greyscale;
		
};
