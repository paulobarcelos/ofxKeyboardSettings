/*
 *  ofxKeyboardSettings.h
 *  ColorsOfMovement
 *
 *  Created by Paulo Barcelos on 7/19/11.
 *  Copyright 2011 paulobarcelos.com. All rights reserved.
 *
 */

#ifndef _OFX_KEYBOARD_SETTINGS
#define _OFX_KEYBOARD_SETTINGS

#define KEYBOARD_SETTINGS_HOLD_TIME 4.f
#define KEYBOARD_SETTINGS_IDLE_TIME 1.f

#include "ofMain.h"
#include "ofxXmlSettings.h"

struct ofxKeyboardProperty {
	string	type;
	string	label;
	int		accessKey;
};
struct ofxKeyboardFloatProperty : public ofxKeyboardProperty{
	float*	var;
	float	min;
	float	max;
	float	step;
};
struct ofxKeyboardIntProperty : public ofxKeyboardProperty{
	int*	var;
	int		min;
	int		max;
	int		step;
};
struct ofxKeyboardBoolProperty : public ofxKeyboardProperty{
	bool*	var;
};

////////////////////////////////////////////////////////////
// CLASS DEFINITION ----------------------------------------
////////////////////////////////////////////////////////////
class ofxKeyboardSettings {
	
public:
	
	void				setup(int accessKey, string filename);
	void				proccessKey(int key);
	void				saveSettings();
	void				loadSettings();
	
	ofxKeyboardFloatProperty*	addProperty(float* var, int accessKey, string label, float min, float max, float step);
	ofxKeyboardIntProperty*		addProperty(int* var, int accessKey, string label, int min, int max, int step);
	ofxKeyboardBoolProperty*	addProperty(bool* var, int accessKey, string label);
	
private:
	
	vector<ofxKeyboardProperty*>	properties;
	ofxKeyboardProperty*			curProperty;
	//vector<ofxKeyboardFloatProperty*>	floatProperties;
	//vector<ofxKeyboardIntProperty*>		intProperties;
	//vector<ofxKeyboardBoolProperty*>	boolProperties;
	
	ofxXmlSettings		settings;
	
	int					accessKey, lastProccessedKey;
	string				filename;
	
	float				accessKeyStartTime, lastProccessedKeyTime;
	
	bool				isActive;	
};
#endif