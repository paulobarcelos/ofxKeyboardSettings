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

#define KEYBOARD_SETTINGS_HOLD_TIME 3.f
#define KEYBOARD_SETTINGS_IDLE_TIME 1.f

#define KEYBOARD_SETTINGS_WIDTH 550
#define KEYBOARD_SETTINGS_PROPERTY_HEIGHT 16
#define KEYBOARD_SETTINGS_VERTICAL_OFFSET 12

#define BOOL_TYPE 0
#define INT_TYPE 1
#define FLOAT_TYPE 2


#include "ofMain.h"
#include "ofxXmlSettings.h"

struct ofxKeyboardProperty {
	int		type;
	string	label;
};
struct ofxKeyboardFloatProperty : public ofxKeyboardProperty{;
	float*	var;
	float	min;
	float	max;
	float	step;
	float	defaultValue;
};
struct ofxKeyboardIntProperty : public ofxKeyboardProperty{
	int*	var;
	int		min;
	int		max;
	int		step;
	int		defaultValue;
};
struct ofxKeyboardBoolProperty : public ofxKeyboardProperty{
	bool*	var;
	bool	defaultValue;
};
////////////////////////////////////////////////////////////
// CLASS DEFINITION ----------------------------------------
////////////////////////////////////////////////////////////
class ofxKeyboardSettings {
	
public:
	
	void				setup(int accessKey, string label);
	void				proccessKey(int key);
	void				draw(int x = 0, int y = 0);
	void				saveSettings();
	void				loadSettings();
	
	
	ofxKeyboardFloatProperty*	addProperty(float* var, string label, float min, float max, float step, float defaultValue);
	ofxKeyboardIntProperty*		addProperty(int* var, string label, int min, int max, int step, int defaultValue);
	ofxKeyboardBoolProperty*	addProperty(bool* var, string label, bool defaultValue);
	
private:
	
	vector<ofxKeyboardProperty*>	properties;
	ofxKeyboardProperty*			curProperty;
	vector<ofxKeyboardProperty*>::iterator curPropertyIterator;
	
	
	void				loadProperty(ofxKeyboardProperty* property);
	
	void				setPropertyValue(ofxKeyboardFloatProperty* property, float value);
	void				setPropertyValue(ofxKeyboardIntProperty* property, int value);
	void				setPropertyValue(ofxKeyboardBoolProperty* property, bool value);
	
	void				renderFBO();
	ofFbo				fbo;
	
	ofxXmlSettings		settings;
	
	int					accessKey, lastProccessedKey;
	string				label;
	
	float				accessKeyStartTime, lastProccessedKeyTime;
	
	bool				isActive;	
};
#endif