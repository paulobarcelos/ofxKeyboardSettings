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

#define BOOL_TYPE 0
#define INT_TYPE 1
#define FLOAT_TYPE 2


#include "ofMain.h"
#include "ofxXmlSettings.h"

struct ofxKeyboardProperty {
	int		type;
	string	label;
	int		accessKey;
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
	void				draw();
	void				saveSettings();
	void				loadSettings();
	
	
	ofxKeyboardFloatProperty*	addProperty(float* var, int accessKey, string label, float min, float max, float step, float defaultValue);
	ofxKeyboardIntProperty*		addProperty(int* var, int accessKey, string label, int min, int max, int step, int defaultValue);
	ofxKeyboardBoolProperty*	addProperty(bool* var, int accessKey, string label, bool defaultValue);
	
private:
	
	vector<ofxKeyboardProperty*>	properties;
	ofxKeyboardProperty*			curProperty;
	ofxKeyboardFloatProperty*		curFloatProperty;
	ofxKeyboardIntProperty*			curIntProperty;
	ofxKeyboardBoolProperty*		curBoolProperty;
	
	
	void				loadProperty(ofxKeyboardProperty* property);
	
	void				setPropertyValue(ofxKeyboardFloatProperty* property, float value);
	void				setPropertyValue(ofxKeyboardIntProperty* property, int value);
	void				setPropertyValue(ofxKeyboardBoolProperty* property, bool value);
	
	void				drawProperty(ofxKeyboardFloatProperty* property, stringstream reportStream);
	void				drawProperty(ofxKeyboardIntProperty* property, stringstream reportStream);
	void				drawProperty(ofxKeyboardBoolProperty* property, stringstream reportStream);
	
	int					x,y;
	
	ofxXmlSettings		settings;
	
	int					accessKey, lastProccessedKey;
	string				label;
	
	float				accessKeyStartTime, lastProccessedKeyTime;
	
	bool				isActive;	
};
#endif