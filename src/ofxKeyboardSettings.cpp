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
		//renderFBO();
	}
}
///////////////////////////////////////////////////////////////////////////////////
// draw ---------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////
void ofxKeyboardSettings::draw(int x, int y){
	if (isActive){
		renderFBO();
		ofSetColor(255);
		fbo.draw(x, y);
	}
}
///////////////////////////////////////////////////////////////////////////////////
// renderFBO ----------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////
void ofxKeyboardSettings::renderFBO(){
	fbo.begin();
	glEnable(GL_BLEND);
	
	int verticalOffset = 16;
	
	stringstream reportStream;
	reportStream 
	<< "(" << (const char)accessKey << ") " << label << endl;
	
	ofFill();
	ofSetColor(255);
	ofRect(0, 0, KEYBOARD_SETTINGS_WIDTH, KEYBOARD_SETTINGS_PROPERTY_HEIGHT);
	ofSetColor(0);
	ofDrawBitmapString(reportStream.str(), 0, KEYBOARD_SETTINGS_VERTICAL_OFFSET);	
	
	int propertyIndex = 1;
	for (vector<ofxKeyboardProperty*>::iterator it = properties.begin(); it!=properties.end(); ++it) {
		ofxKeyboardProperty* property = *it;
		
		property->draw(0, KEYBOARD_SETTINGS_VERTICAL_OFFSET + propertyIndex * KEYBOARD_SETTINGS_PROPERTY_HEIGHT, (curProperty == property));
				
		propertyIndex++;
	}
	glDisable(GL_BLEND);
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
// addProperty --------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////
ofxKeyboardDoubleProperty* ofxKeyboardSettings::addProperty(double* var, string label){
	ofxKeyboardDoubleProperty* property;
	property = new ofxKeyboardDoubleProperty();
	property->allowControl = false;
	property->var = var;
	property->label = label;
	
	properties.push_back(property);	
	
	onAddProperty();
	
	return property;
}
ofxKeyboardDoubleProperty* ofxKeyboardSettings::addProperty(double* var, string label, double min, double max, double step, double defaultValue){
	ofxKeyboardDoubleProperty* property;
	property = new ofxKeyboardDoubleProperty();
	property->settingsXML = &settings;
	property->settingsLabel = this->label;
	property->allowControl = true;
	property->var = var;
	property->label = label;
	property->min = min;
	property->max = max;
	property->step = step;
	property->defaultValue = defaultValue;
	property->load();
	
	properties.push_back(property);	
	
	onAddProperty();
	
	return property;
}

ofxKeyboardFloatProperty* ofxKeyboardSettings::addProperty(float* var, string label){
	ofxKeyboardFloatProperty* property;
	property = new ofxKeyboardFloatProperty();
	property->allowControl = false;
	property->var = var;
	property->label = label;
	
	properties.push_back(property);	
	
	onAddProperty();
	
	return property;
}
ofxKeyboardFloatProperty* ofxKeyboardSettings::addProperty(float* var, string label, float min, float max, float step, float defaultValue){
	ofxKeyboardFloatProperty* property;
	property = new ofxKeyboardFloatProperty();
	property->settingsXML = &settings;
	property->settingsLabel = this->label;
	property->allowControl = true;
	property->var = var;
	property->label = label;
	property->min = min;
	property->max = max;
	property->step = step;
	property->defaultValue = defaultValue;
	property->load();
	
	properties.push_back(property);	
	
	onAddProperty();
	
	return property;
}
ofxKeyboardStaticIntProperty* ofxKeyboardSettings::addProperty(int(* get)(), string label){
	ofxKeyboardStaticIntProperty* property;
	property = new ofxKeyboardStaticIntProperty();
	property->allowControl = false;
	property->get = get;
	
	properties.push_back(property);	
	
	onAddProperty();
}
ofxKeyboardIntProperty* ofxKeyboardSettings::addProperty(int* var, string label){
	ofxKeyboardIntProperty* property;
	property = new ofxKeyboardIntProperty();
	property->allowControl = false;
	property->var = var;
	property->label = label;
	
	properties.push_back(property);	
	
	onAddProperty();
	
	return property;
}
ofxKeyboardIntProperty* ofxKeyboardSettings::addProperty(int* var, string label, int min, int max, int step, int defaultValue){
	ofxKeyboardIntProperty* property;
	property = new ofxKeyboardIntProperty();
	property->settingsXML = &settings;
	property->settingsLabel = this->label;
	property->allowControl = true;
	property->var = var;
	property->label = label;
	property->min = min;
	property->max = max;
	property->step = step;
	property->defaultValue = defaultValue;
	property->load();
	
	properties.push_back(property);	
	
	onAddProperty();
	
	return property;
}

ofxKeyboardBoolProperty* ofxKeyboardSettings::addProperty(bool* var, string label){
	ofxKeyboardBoolProperty* property;
	property = new ofxKeyboardBoolProperty();
	property->allowControl = false;
	property->var = var;
	property->label = label;
	
	properties.push_back(property);	
	
	onAddProperty();
	
	return property;
}
ofxKeyboardBoolProperty* ofxKeyboardSettings::addProperty(bool* var, string label, bool defaultValue){
	ofxKeyboardBoolProperty* property;
	property = new ofxKeyboardBoolProperty();
	property->settingsXML = &settings;
	property->settingsLabel = this->label;
	property->allowControl = true;
	property->var = var;
	property->label = label;
	property->defaultValue = defaultValue;
	property->load();
	
	properties.push_back(property);	
	
	onAddProperty();
	
	return property;
}
///////////////////////////////////////////////////////////////////////////////////
// onAddProperty ------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////
void ofxKeyboardSettings::onAddProperty(){
	curPropertyIterator = properties.begin();
	fbo.allocate(KEYBOARD_SETTINGS_WIDTH, (properties.size() + 1) * KEYBOARD_SETTINGS_PROPERTY_HEIGHT);
}