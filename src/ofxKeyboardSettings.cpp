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
			if		(curProperty->type == FLOAT_TYPE){
				ofxKeyboardFloatProperty* floatProperty = (ofxKeyboardFloatProperty*)curProperty;
				if		(key == OF_KEY_RIGHT)	setPropertyValue(floatProperty, (*(floatProperty->var)) + floatProperty->step);
				else if	(key == OF_KEY_LEFT)	setPropertyValue(floatProperty, (*(floatProperty->var)) - floatProperty->step);
			}
			else if (curProperty->type == INT_TYPE){
				ofxKeyboardIntProperty* intProperty = (ofxKeyboardIntProperty*)curProperty;
				if		(key == OF_KEY_RIGHT)	setPropertyValue(intProperty, (*(intProperty->var)) + intProperty->step);
				else if	(key == OF_KEY_LEFT)	setPropertyValue(intProperty, (*(intProperty->var)) - intProperty->step);
			}
			else if (curProperty->type == BOOL_TYPE){
				ofxKeyboardBoolProperty* boolProperty = (ofxKeyboardBoolProperty*)curProperty;
				if		(key == OF_KEY_RIGHT)	setPropertyValue(boolProperty, true);
				else if	(key == OF_KEY_LEFT)	setPropertyValue(boolProperty, false);
			}
		}
		renderFBO();
	}
}
///////////////////////////////////////////////////////////////////////////////////
// draw ---------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////
void ofxKeyboardSettings::draw(int x, int y){
	if (isActive){
		ofSetColor(255);
		fbo.draw(x, y);
	}
}
///////////////////////////////////////////////////////////////////////////////////
// renderFBO ----------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////
void ofxKeyboardSettings::renderFBO(){
	fbo.begin();
	
	int verticalOffset = 16;
	
	stringstream reportStream;
	reportStream 
	<< "(" << (const char)accessKey << ") " << label << endl;
	
	ofFill();
	ofSetColor(255, 255, 255, 200);
	ofRect(0, 0, KEYBOARD_SETTINGS_WIDTH, KEYBOARD_SETTINGS_PROPERTY_HEIGHT);
	ofSetColor(0);
	ofDrawBitmapString(reportStream.str(), 0, KEYBOARD_SETTINGS_VERTICAL_OFFSET);	
	
	int propertyIndex = 1;
	for (vector<ofxKeyboardProperty*>::iterator it = properties.begin(); it!=properties.end(); ++it) {
		ofxKeyboardProperty* property = *it;
		
		stringstream propertyStream;
		propertyStream
		<< property->label << ": ";
		
		if (property->type == FLOAT_TYPE){
			ofxKeyboardFloatProperty* floatProperty = (ofxKeyboardFloatProperty*)property;
			propertyStream
			<< (*(floatProperty->var))
			<< " (min " << floatProperty->min
			<< ", max " << floatProperty->max
			<< ", step " << floatProperty->step
			<< ")";
		}
		else if (property->type == INT_TYPE){
			ofxKeyboardIntProperty* intProperty = (ofxKeyboardIntProperty*)property;
			propertyStream
			<< (*(intProperty->var))
			<< " (min " << intProperty->min
			<< ", max " << intProperty->max
			<< ", step " << intProperty->step
			<< ")";
		}
		else if (property->type == BOOL_TYPE){
			ofxKeyboardBoolProperty* boolProperty = (ofxKeyboardBoolProperty*)property;
			propertyStream
			<< ((*(boolProperty->var))?"true":"false");
		}
		
		int grey = (curProperty == property)?0:50;
		ofSetColor(grey, grey, grey, 200);
		ofRect(0, propertyIndex * KEYBOARD_SETTINGS_PROPERTY_HEIGHT, KEYBOARD_SETTINGS_WIDTH, KEYBOARD_SETTINGS_PROPERTY_HEIGHT);
		ofSetColor(0xffffff);
		ofDrawBitmapString(propertyStream.str(), 10, KEYBOARD_SETTINGS_VERTICAL_OFFSET + propertyIndex * KEYBOARD_SETTINGS_PROPERTY_HEIGHT);
		
		propertyIndex++;
	}
	
	fbo.end();
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
	if (property->type == FLOAT_TYPE){
		ofxKeyboardFloatProperty* floatProperty = (ofxKeyboardFloatProperty*)property;
		setPropertyValue(floatProperty, (float)settings.getValue(label+":"+property->label, floatProperty->defaultValue));
	}
	else if (property->type == INT_TYPE){
		ofxKeyboardIntProperty* intProperty = (ofxKeyboardIntProperty*)property;
		setPropertyValue(intProperty, (int)settings.getValue(label+":"+property->label, intProperty->defaultValue));
	}
	else if (property->type == BOOL_TYPE){
		ofxKeyboardBoolProperty* boolProperty = (ofxKeyboardBoolProperty*)property;
		setPropertyValue(boolProperty, (bool)settings.getValue(label+":"+property->label, boolProperty->defaultValue));
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
ofxKeyboardFloatProperty* ofxKeyboardSettings::addProperty(float* var, string label, float min, float max, float step, float defaultValue){
	ofxKeyboardFloatProperty* property;
	property = new ofxKeyboardFloatProperty();
	property->type = FLOAT_TYPE;
	property->var = var;
	property->label = label;
	property->min = min;
	property->max = max;
	property->step = step;
	property->defaultValue = defaultValue;
	
	properties.push_back(property);
	curPropertyIterator = properties.begin();
	
	loadProperty(property);
	
	fbo.allocate(KEYBOARD_SETTINGS_WIDTH, (properties.size() + 1) * KEYBOARD_SETTINGS_PROPERTY_HEIGHT);
		
	return property;
}
ofxKeyboardIntProperty* ofxKeyboardSettings::addProperty(int* var, string label, int min, int max, int step, int defaultValue){
	ofxKeyboardIntProperty* property;
	property = new ofxKeyboardIntProperty();
	property->type = INT_TYPE;
	property->var = var;
	property->label = label;
	property->min = min;
	property->max = max;
	property->step = step;
	property->defaultValue = defaultValue;
	
	properties.push_back(property);
	curPropertyIterator = properties.begin();
	
	loadProperty(property);
	
	fbo.allocate(KEYBOARD_SETTINGS_WIDTH, (properties.size() + 1) * KEYBOARD_SETTINGS_PROPERTY_HEIGHT);
	
	return property;
}
ofxKeyboardBoolProperty* ofxKeyboardSettings::addProperty(bool* var, string label, bool defaultValue){
	ofxKeyboardBoolProperty* property;
	property = new ofxKeyboardBoolProperty();
	property->type = BOOL_TYPE;
	property->var = var;
	property->label = label;
	property->defaultValue = defaultValue;
	
	properties.push_back(property);
	curPropertyIterator = properties.begin();
	
	loadProperty(property);
	
	fbo.allocate(KEYBOARD_SETTINGS_WIDTH, (properties.size() + 1) * KEYBOARD_SETTINGS_PROPERTY_HEIGHT);
	
	return property;
}
