/*
 *  ofxKeyboardSettings.cpp
 *  ofxKeyboardSettings
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
void ofxKeyboardSettings::keyPressed(int key)
{	
	if ((ofGetElapsedTimef() - lastProccessedKeyTime) > KEYBOARD_SETTINGS_IDLE_TIME)
		lastProccessedKey = -1;
	
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
		if (key == OF_KEY_UP) {			
			if (curPropertyIterator != properties.begin())
				--curPropertyIterator;
		}
		else if	(key == OF_KEY_DOWN) {
			if (curPropertyIterator != --(properties.end()))
				++curPropertyIterator;
		}
		curProperty = *curPropertyIterator;
		if(curProperty){
			if(curProperty->allowControl){
				curProperty->keyPressed(key);
			}
		}
	}
}
///////////////////////////////////////////////////////////////////////////////////
// draw ---------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////
void ofxKeyboardSettings::draw(float x, float y){
	if (isActive){
		int verticalOffset = 16;
		
		string output = "(" + ofToString((const char)accessKey) + ") " + label;
		
		ofFill();
		ofSetColor(255, 255, 255, 150);
		ofRect(x, y, KEYBOARD_SETTINGS_WIDTH, KEYBOARD_SETTINGS_PROPERTY_HEIGHT);
		ofSetColor(0);
		ofDrawBitmapString(output, x, y + KEYBOARD_SETTINGS_VERTICAL_OFFSET);	
		
		int propertyIndex = 1;
		for (vector<ofxKeyboardBaseProperty*>::iterator it = properties.begin(); it!=properties.end(); ++it) {
			ofxKeyboardBaseProperty* property = *it;
			
			property->draw(x, y + KEYBOARD_SETTINGS_VERTICAL_OFFSET + propertyIndex * KEYBOARD_SETTINGS_PROPERTY_HEIGHT, (curProperty == property));
			
			propertyIndex++;
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
	settings.loadFile(label+".xml");
	for (vector<ofxKeyboardBaseProperty*>::iterator it = properties.begin(); it!=properties.end(); ++it)
		(*it)->load();
}