/*
 *  ofxKeyboardSettings.cpp
 *  ColorsOfMovement
 *
 *  Created by Paulo Barcelos on 7/19/11.
 *  Copyright 2011 paulobarcelos.com. All rights reserved.
 *
 */

#include "ofxKeyboardSettings.h"

///////////////////////////////////////////////////////////////////////////////////
// setup --------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////
void ofxKeyboardSettings::setup(int accessKey, string label)
{	
	this->accessKey = accessKey;
	this->label = label;
	
	lastProccessedKey = -1;
	isActive = false;
	curProperty = NULL;
	
	loadSettings();
}
///////////////////////////////////////////////////////////////////////////////////
// proccessKey --------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////
void ofxKeyboardSettings::proccessKey(int key)
{	
	if ((ofGetElapsedTimef() - lastProccessedKeyTime) > KEYBOARD_SETTINGS_IDLE_TIME) {
		lastProccessedKey = -1;
		//curProperty = NULL;
	}
	
	if(key == accessKey){
		if(key != lastProccessedKey)
			accessKeyStartTime = ofGetElapsedTimef();
		else {
			float diff = ofGetElapsedTimef() - accessKeyStartTime;
			if(diff > KEYBOARD_SETTINGS_HOLD_TIME){
				accessKeyStartTime = ofGetElapsedTimef();
				isActive ^= true;
				cout << isActive << endl;
			}
		}
	}
	
	lastProccessedKey = key;
	lastProccessedKeyTime = ofGetElapsedTimef();
	
	if(isActive){
		for (vector<ofxKeyboardProperty*>::iterator it = properties.begin(); it!=properties.end(); ++it) {
			if (key == (*it)->accessKey){
				curProperty = *it;
				curFloatProperty =	(ofxKeyboardFloatProperty*)curProperty;
				curIntProperty =	(ofxKeyboardIntProperty*)curProperty;
				curBoolProperty =	(ofxKeyboardBoolProperty*)curProperty;
				break;
			}
		}
		if(curProperty){
			switch (curProperty->type) {
				case FLOAT_TYPE:
					if(key == OF_KEY_UP)		setPropertyValue(curFloatProperty, (*(curFloatProperty->var)) + curFloatProperty->step);
					else if(key == OF_KEY_DOWN)	setPropertyValue(curFloatProperty, (*(curFloatProperty->var)) -	curFloatProperty->step);
					break;
				case INT_TYPE:
					if(key == OF_KEY_UP)		setPropertyValue(curIntProperty, (*(curIntProperty->var)) + curIntProperty->step);
					else if(key == OF_KEY_DOWN)	setPropertyValue(curIntProperty, (*(curIntProperty->var)) -	curIntProperty->step);
					break;
				case BOOL_TYPE:
					if(key == OF_KEY_UP)		setPropertyValue(curBoolProperty, true);
					else if(key == OF_KEY_DOWN)	setPropertyValue(curBoolProperty, false);
					break;
			}

		}
	}
}
///////////////////////////////////////////////////////////////////////////////////
// saveSettings -------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////
void ofxKeyboardSettings::saveSettings(){
	settings.saveFile(label+".xml");
}

///////////////////////////////////////////////////////////////////////////////////
// loadSettings -------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////
void ofxKeyboardSettings::loadSettings(){
	if(!settings.loadFile(label+".xml"))
		saveSettings();
}
///////////////////////////////////////////////////////////////////////////////////
// loadProperty -------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////
void ofxKeyboardSettings::loadProperty(ofxKeyboardProperty* property){
	ofxKeyboardFloatProperty* floatProperty = (ofxKeyboardFloatProperty*)property;
	ofxKeyboardIntProperty* intProperty = (ofxKeyboardIntProperty*)property;
	ofxKeyboardBoolProperty* boolProperty = (ofxKeyboardBoolProperty*)property;
	switch (property->type) {
		case FLOAT_TYPE:				
			setPropertyValue(floatProperty, (float)settings.getValue(label+":"+property->label, floatProperty->defaultValue));
			break;
		case INT_TYPE:				
			setPropertyValue(intProperty, (int)settings.getValue(label+":"+property->label, intProperty->defaultValue));
			break;
		case BOOL_TYPE:				
			setPropertyValue(boolProperty, (bool)settings.getValue(label+":"+property->label, boolProperty->defaultValue));
			break;
	}
}
///////////////////////////////////////////////////////////////////////////////////
// setPropertyValue ---------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////
void ofxKeyboardSettings::setPropertyValue(ofxKeyboardFloatProperty* property, float value){
	if (value < property->min)		value = property->min;
	else if (value > property->max) value = property->max;
	(*(property->var)) = value;
	settings.setValue(label+":"+property->label, value, 0);
	saveSettings();
}
void ofxKeyboardSettings::setPropertyValue(ofxKeyboardIntProperty* property, int value){
	if (value < property->min)		value = property->min;
	else if (value > property->max) value = property->max;
	(*(property->var)) = value;
	settings.setValue(label+":"+property->label, value, 0);
	saveSettings();
}
void ofxKeyboardSettings::setPropertyValue(ofxKeyboardBoolProperty* property, bool value){
	(*(property->var)) = value;
	settings.setValue(label+":"+property->label, value, 0);
	saveSettings();
}
///////////////////////////////////////////////////////////////////////////////////
// addProperty --------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////
ofxKeyboardFloatProperty* ofxKeyboardSettings::addProperty(float* var, int accessKey, string label, float min, float max, float step, float defaultValue){
	ofxKeyboardFloatProperty* property;
	property = new ofxKeyboardFloatProperty();
	property->type = FLOAT_TYPE;
	property->var = var;
	property->accessKey = accessKey;
	property->label = label;
	property->min = min;
	property->max = max;
	property->step = step;
	property->defaultValue = defaultValue;
	
	properties.push_back(property);
	
	loadProperty(property);
		
	return property;
}
ofxKeyboardIntProperty* ofxKeyboardSettings::addProperty(int* var, int accessKey, string label, int min, int max, int step, int defaultValue){
	ofxKeyboardIntProperty* property;
	property = new ofxKeyboardIntProperty();
	property->type = INT_TYPE;
	property->var = var;
	property->accessKey = accessKey;
	property->label = label;
	property->min = min;
	property->max = max;
	property->step = step;
	property->defaultValue = defaultValue;
	
	properties.push_back(property);
	
	loadProperty(property);
	
	return property;
}
ofxKeyboardBoolProperty* ofxKeyboardSettings::addProperty(bool* var, int accessKey, string label, bool defaultValue){
	ofxKeyboardBoolProperty* property;
	property = new ofxKeyboardBoolProperty();
	property->type = BOOL_TYPE;
	property->var = var;
	property->accessKey = accessKey;
	property->label = label;
	property->defaultValue = defaultValue;
	
	properties.push_back(property);
	
	loadProperty(property);
	
	return property;
}
