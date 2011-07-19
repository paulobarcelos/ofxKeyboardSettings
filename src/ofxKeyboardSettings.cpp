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
void ofxKeyboardSettings::setup(int accessKey, string filename)
{	
	this->accessKey = accessKey;
	this->filename = filename;
	
	lastProccessedKey = -1;
	isActive = false;
	curProperty = NULL;
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
			if (curProperty->type == "int" || curProperty->type == "float")
			{
				/*if(key == OF_KEY_UP)
				else if(key == OF_KEY_DOWN)*/
			}
		}
	}
}
///////////////////////////////////////////////////////////////////////////////////
// saveSettings -------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////
void ofxKeyboardSettings::saveSettings(){
	settings.saveFile(filename);
}

///////////////////////////////////////////////////////////////////////////////////
// loadSettings ------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////
void ofxKeyboardSettings::loadSettings(){
	settings.loadFile(filename);
}
///////////////////////////////////////////////////////////////////////////////////
// addProperty --------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////
ofxKeyboardIntProperty* ofxKeyboardSettings::addProperty(int*var, int accessKey, string label, int min, int max, int step){
	ofxKeyboardIntProperty* property;
	property = new ofxKeyboardIntProperty();
	property->type = "int";
	property->var = var;
	property->accessKey = accessKey;
	property->label = label;
	property->min = min;
	property->max = max;
	property->step = step;
	
	properties.push_back(property);
	
	return property;
}
