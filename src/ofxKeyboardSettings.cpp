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
				break;
			}
		}
		if(curProperty){
			/*if (curProperty->type == "int" || curProperty->type == "float")
			{
				if(key == OF_KEY_UP)
				else if(key == OF_KEY_DOWN)
			}*/
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
		default:
			break;
	}
}
///////////////////////////////////////////////////////////////////////////////////
// setPropertyValue -------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////
void ofxKeyboardSettings::setPropertyValue(ofxKeyboardFloatProperty* property, float value){
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
