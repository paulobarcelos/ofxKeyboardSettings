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
			if		(curProperty->type == KEYBOARD_SETTINGS_DOUBLE_TYPE){
				ofxKeyboardDoubleProperty* doubleProperty = (ofxKeyboardDoubleProperty*)curProperty;
				if		(key == OF_KEY_RIGHT)	setProperty(doubleProperty, (*(doubleProperty->var)) + doubleProperty->step);
				else if	(key == OF_KEY_LEFT)	setProperty(doubleProperty, (*(doubleProperty->var)) - doubleProperty->step);
			}
			else if		(curProperty->type == KEYBOARD_SETTINGS_FLOAT_TYPE){
				ofxKeyboardFloatProperty* floatProperty = (ofxKeyboardFloatProperty*)curProperty;
				if		(key == OF_KEY_RIGHT)	setProperty(floatProperty, (*(floatProperty->var)) + floatProperty->step);
				else if	(key == OF_KEY_LEFT)	setProperty(floatProperty, (*(floatProperty->var)) - floatProperty->step);
			}
			else if (curProperty->type == KEYBOARD_SETTINGS_INT_TYPE){
				ofxKeyboardIntProperty* intProperty = (ofxKeyboardIntProperty*)curProperty;
				if		(key == OF_KEY_RIGHT)	setProperty(intProperty, (*(intProperty->var)) + intProperty->step);
				else if	(key == OF_KEY_LEFT)	setProperty(intProperty, (*(intProperty->var)) - intProperty->step);
			}
			else if (curProperty->type == KEYBOARD_SETTINGS_BOOL_TYPE){
				ofxKeyboardBoolProperty* boolProperty = (ofxKeyboardBoolProperty*)curProperty;
				if		(key == OF_KEY_RIGHT)	setProperty(boolProperty, true);
				else if	(key == OF_KEY_LEFT)	setProperty(boolProperty, false);
			}
			/*else if (curProperty->type == KEYBOARD_SETTINGS_CONTROL_INT_TYPE){
				ofxKeyboardControlIntProperty* controlIntProperty = (ofxKeyboardControlIntProperty*)curProperty;
				if		(key == OF_KEY_RIGHT)	setProperty(controlIntProperty, controlIntProperty->(* get)() + controlIntProperty->(* step)());
				else if	(key == OF_KEY_LEFT)	setProperty(controlIntProperty, controlIntProperty->(* get)() - controlIntProperty->(* step)());
			}*/
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
		
		stringstream propertyStream;
		propertyStream
		<< property->label << ": ";
		
		if (property->type == KEYBOARD_SETTINGS_DOUBLE_TYPE){
			ofxKeyboardDoubleProperty* floatProperty = (ofxKeyboardDoubleProperty*)property;
			propertyStream
			<< (*(floatProperty->var))
			<< " (min " << floatProperty->min
			<< ", max " << floatProperty->max
			<< ", step " << floatProperty->step
			<< ")";
		}
		else if (property->type == KEYBOARD_SETTINGS_FLOAT_TYPE){
			ofxKeyboardFloatProperty* floatProperty = (ofxKeyboardFloatProperty*)property;
			propertyStream
			<< (*(floatProperty->var))
			<< " (min " << floatProperty->min
			<< ", max " << floatProperty->max
			<< ", step " << floatProperty->step
			<< ")";
		}
		else if (property->type == KEYBOARD_SETTINGS_INT_TYPE){
			ofxKeyboardIntProperty* intProperty = (ofxKeyboardIntProperty*)property;
			propertyStream
			<< (*(intProperty->var))
			<< " (min " << intProperty->min
			<< ", max " << intProperty->max
			<< ", step " << intProperty->step
			<< ")";
		}
		else if (property->type == KEYBOARD_SETTINGS_BOOL_TYPE){
			ofxKeyboardBoolProperty* boolProperty = (ofxKeyboardBoolProperty*)property;
			propertyStream
			<< ((*(boolProperty->var))?"true":"false");
		}
		/*else if (property->type == KEYBOARD_SETTINGS_CONTROL_INT_TYPE){
			ofxKeyboardControlIntProperty* controlIntProperty = (ofxKeyboardControlIntProperty*)property;
			propertyStream
			<< controlIntProperty->(* get)()
			<< " (min " << controlIntProperty->(* min)()
			<< ", max " << controlIntProperty->(* max)()
			<< ", step " << controlIntProperty->(* step)()
			<< ")";
		}*/
		
		int grey = (curProperty == property)?0:50;
		ofSetColor(grey);
		ofRect(0, propertyIndex * KEYBOARD_SETTINGS_PROPERTY_HEIGHT, KEYBOARD_SETTINGS_WIDTH, KEYBOARD_SETTINGS_PROPERTY_HEIGHT);
		ofSetColor(255);
		ofDrawBitmapString(propertyStream.str(), 10, KEYBOARD_SETTINGS_VERTICAL_OFFSET + propertyIndex * KEYBOARD_SETTINGS_PROPERTY_HEIGHT);
		
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
// loadProperty -------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////
void ofxKeyboardSettings::loadProperty(ofxKeyboardDoubleProperty* property){
	setProperty(property, (double)settings.getValue(label+":"+property->label, property->defaultValue));
}
void ofxKeyboardSettings::loadProperty(ofxKeyboardFloatProperty* property){
	setProperty(property, (float)settings.getValue(label+":"+property->label, property->defaultValue));
}
void ofxKeyboardSettings::loadProperty(ofxKeyboardIntProperty* property){
	setProperty(property, (int)settings.getValue(label+":"+property->label, property->defaultValue));
}
void ofxKeyboardSettings::loadProperty(ofxKeyboardBoolProperty* property){
	setProperty(property, (bool)settings.getValue(label+":"+property->label, property->defaultValue));
}
void ofxKeyboardSettings::loadProperty(ofxKeyboardControlIntProperty* property){
	setProperty(property, (int)settings.getValue(label+":"+property->label, property->defaultValue));
}
///////////////////////////////////////////////////////////////////////////////////
// setProperty ---------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////
void ofxKeyboardSettings::setProperty(ofxKeyboardDoubleProperty* property, double value){
	if (value < property->min)		value = property->min;
	else if (value > property->max) value = property->max;
	(*(property->var)) = value;
	settings.setValue(label+":"+property->label, value, 0);
	saveSettings();
}
void ofxKeyboardSettings::setProperty(ofxKeyboardFloatProperty* property, float value){
	if (value < property->min)		value = property->min;
	else if (value > property->max) value = property->max;
	(*(property->var)) = value;
	settings.setValue(label+":"+property->label, value, 0);
	saveSettings();
}
void ofxKeyboardSettings::setProperty(ofxKeyboardIntProperty* property, int value){
	if (value < property->min)		value = property->min;
	else if (value > property->max) value = property->max;
	(*(property->var)) = value;
	settings.setValue(label+":"+property->label, value, 0);
	saveSettings();
}
void ofxKeyboardSettings::setProperty(ofxKeyboardBoolProperty* property, bool value){
	(*(property->var)) = value;
	settings.setValue(label+":"+property->label, value, 0);
	saveSettings();
}
/*void ofxKeyboardSettings::setProperty(ofxKeyboardControlIntProperty* property, int value){
	if (value < property->(* min)())		value = property->(* min)();
	else if (value > property->(* max)())	value = property->(* max)();
	property->(* set)(value);
	settings.setValue(label+":"+property->label, value, 0);
	saveSettings();
}*/
///////////////////////////////////////////////////////////////////////////////////
// addProperty --------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////
ofxKeyboardDoubleProperty* ofxKeyboardSettings::addProperty(double* var, string label, double min, double max, double step, double defaultValue){
	ofxKeyboardDoubleProperty* property;
	property = new ofxKeyboardDoubleProperty();
	property->type = KEYBOARD_SETTINGS_DOUBLE_TYPE;
	property->var = var;
	property->label = label;
	property->min = min;
	property->max = max;
	property->step = step;
	property->defaultValue = defaultValue;
	
	properties.push_back(property);
	
	loadProperty(property);
	
	onAddProperty();
	
	return property;
}
ofxKeyboardFloatProperty* ofxKeyboardSettings::addProperty(float* var, string label, float min, float max, float step, float defaultValue){
	ofxKeyboardFloatProperty* property;
	property = new ofxKeyboardFloatProperty();
	property->type = KEYBOARD_SETTINGS_FLOAT_TYPE;
	property->var = var;
	property->label = label;
	property->min = min;
	property->max = max;
	property->step = step;
	property->defaultValue = defaultValue;
	
	properties.push_back(property);
	
	loadProperty(property);
	
	onAddProperty();
	
	return property;
}
ofxKeyboardIntProperty* ofxKeyboardSettings::addProperty(int* var, string label, int min, int max, int step, int defaultValue){
	ofxKeyboardIntProperty* property;
	property = new ofxKeyboardIntProperty();
	property->type = KEYBOARD_SETTINGS_INT_TYPE;
	property->var = var;
	property->label = label;
	property->min = min;
	property->max = max;
	property->step = step;
	property->defaultValue = defaultValue;
	
	properties.push_back(property);
	
	loadProperty(property);
	
	onAddProperty();
	
	return property;
}
ofxKeyboardBoolProperty* ofxKeyboardSettings::addProperty(bool* var, string label, bool defaultValue){
	ofxKeyboardBoolProperty* property;
	property = new ofxKeyboardBoolProperty();
	property->type = KEYBOARD_SETTINGS_BOOL_TYPE;
	property->var = var;
	property->label = label;
	property->defaultValue = defaultValue;
	
	properties.push_back(property);
	
	loadProperty(property);
	
	onAddProperty();
	
	return property;
}
ofxKeyboardControlIntProperty* ofxKeyboardSettings::addProperty(int (* get)(), int (* set)(int value), string label, int (* min)(), int (* max)(), int (* step)(), int defaultValue){
	ofxKeyboardControlIntProperty* property;
	property = new ofxKeyboardControlIntProperty();
	property->type = KEYBOARD_SETTINGS_CONTROL_INT_TYPE;
	property->get = get;
	property->set = set;
	property->label = label;
	property->min = min;
	property->max = max;
	property->step = step;
	property->defaultValue = defaultValue;
	
	properties.push_back(property);
	
	loadProperty(property);
	
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