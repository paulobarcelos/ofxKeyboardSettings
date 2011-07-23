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

#define KEYBOARD_SETTINGS_BOOL_TYPE 0
#define KEYBOARD_SETTINGS_INT_TYPE 1
#define KEYBOARD_SETTINGS_FLOAT_TYPE 2
#define KEYBOARD_SETTINGS_DOUBLE_TYPE 3
//#define KEYBOARD_SETTINGS_CONTROL_BOOL_TYPE 4
//#define KEYBOARD_SETTINGS_CONTROL_INT_TYPE 5
//#define KEYBOARD_SETTINGS_CONTROL_FLOAT_TYPE 8


#include "ofMain.h"
#include "ofxXmlSettings.h"

struct ofxKeyboardProperty {
	int		type;
	string	label;
};
struct ofxKeyboardDoubleProperty : public ofxKeyboardProperty{;
	double*	var;
	double	min;
	double	max;
	double	step;
	double	defaultValue;
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
	
	
	ofxKeyboardDoubleProperty*	addProperty(double* var, string label, double min, double max, double step, double defaultValue);
	ofxKeyboardFloatProperty*	addProperty(float* var, string label, float min, float max, float step, float defaultValue);
	ofxKeyboardIntProperty*		addProperty(int* var, string label, int min, int max, int step, int defaultValue);
	ofxKeyboardBoolProperty*	addProperty(bool* var, string label, bool defaultValue);
		
private:
	
	vector<ofxKeyboardProperty*>	properties;
	ofxKeyboardProperty*			curProperty;
	vector<ofxKeyboardProperty*>::iterator curPropertyIterator;
	
	void				onAddProperty();
	
	void				loadProperty(ofxKeyboardDoubleProperty* property);
	void				loadProperty(ofxKeyboardFloatProperty* property);
	void				loadProperty(ofxKeyboardIntProperty* property);
	void				loadProperty(ofxKeyboardBoolProperty* property);
	
	void				setProperty(ofxKeyboardDoubleProperty* property, double value);
	void				setProperty(ofxKeyboardFloatProperty* property, float value);
	void				setProperty(ofxKeyboardIntProperty* property, int value);
	void				setProperty(ofxKeyboardBoolProperty* property, bool value);
	
	void				renderFBO();
	ofFbo				fbo;
	
	ofxXmlSettings		settings;
	
	int					accessKey, lastProccessedKey;
	string				label;
	
	float				accessKeyStartTime, lastProccessedKeyTime;
	
	bool				isActive;	
};
#endif