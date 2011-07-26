/*
 *  ofxKeyboardSettings.h
 *  ofxKeyboardSettings
 *
 *  Created by Paulo Barcelos on 7/19/11.
 *  Copyright 2011 paulobarcelos.com. All rights reserved.
 *
 */

#ifndef _OFX_KEYBAORD_SETTINGS
#define _OFX_KEYBAORD_SETTINGS

#include "ofMain.h"
#include "ofxXmlSettings.h"

////////////////////////////////////////////////////////////
// CONSTANTS -----------------------------------------------
////////////////////////////////////////////////////////////
#define KEYBOARD_SETTINGS_HOLD_TIME 3.f
#define KEYBOARD_SETTINGS_IDLE_TIME 1.f

#define KEYBOARD_SETTINGS_WIDTH 550
#define KEYBOARD_SETTINGS_PROPERTY_HEIGHT 16
#define KEYBOARD_SETTINGS_VERTICAL_OFFSET 12

////////////////////////////////////////////////////////////
// STRUCTS -------------------------------------------------
////////////////////////////////////////////////////////////
struct ofxKeyboardBaseProperty {
	ofxXmlSettings* settingsXML;
	string	settingsLabel;
	string	label;
	bool	allowControl;
	
	virtual void	load (){};	
	virtual void	keyPressed(int key){};	
	virtual void	draw(int x, int y, bool isCurProperty = false){};
	
	virtual void	beginDraw(){
						output = "";
						if (!allowControl) output += "(monitor) ";				
						output += label + ": ";
					};
	virtual void	endDraw(int x, int y, bool isCurProperty){
						ofSetColor((isCurProperty)?0:50);
						ofRect(x, y - KEYBOARD_SETTINGS_VERTICAL_OFFSET, KEYBOARD_SETTINGS_WIDTH, KEYBOARD_SETTINGS_PROPERTY_HEIGHT);
						ofSetColor(255);
						ofDrawBitmapString(output, x+10, y);
					};
	
	string output;
};
// STATIC PROPERTY --------------------------------------------
template <typename type>
struct ofxKeyboardStaticProperty : public ofxKeyboardBaseProperty{
	type	(*get)();
	void	draw(int x, int y, bool isCurProperty = false){
		ofxKeyboardBaseProperty::beginDraw();		
		output += ofToString((*get)());
		ofxKeyboardBaseProperty::endDraw(x, y, isCurProperty);
	};
};
// VARIABLE PROPERTY ------------------------------------------
template <typename type>
struct ofxKeyboardProperty : public ofxKeyboardBaseProperty {
	type*	var;
	type	min;
	type	max;
	type	step;
	type	defaultValue;
	
	void	load (){
				setValue((type)settingsXML->getValue(settingsLabel+":"+label, defaultValue));
			};
	
	void	setValue (type value){
				if		(value < min) value = min;
				else if (value > max) value = max;
				*var = value;
				settingsXML->setValue(settingsLabel+":"+label, value, 0);
				settingsXML->saveFile(settingsLabel+".xml");
			};
	
	void	keyPressed(int key){
				if		(key == OF_KEY_RIGHT)	setValue(*var + step);
				else if	(key == OF_KEY_LEFT)	setValue(*var - step);
			};
	
	void	draw(int x, int y, bool isCurProperty = false){
				ofxKeyboardBaseProperty::beginDraw();
				output += ofToString(*var);
				if (allowControl) output += " (min " + ofToString(min) + ", max " + ofToString(max) + ", step " + ofToString(step) + ")";
				ofxKeyboardBaseProperty::endDraw(x, y, isCurProperty);
			};
};
// CONTROL PROPERTY ------------------------------------------
template <typename type, typename GetClass, typename SetClass, typename MinClass, typename MaxClass, typename StepClass>
struct ofxKeyboardControlProperty : public ofxKeyboardBaseProperty {
	GetClass*	getObject;
	SetClass*	setObject;
	MinClass*	minObject;
	MaxClass*	maxObject;
	StepClass*	stepObject;
	void	(SetClass::*set)(type value);
	type	(GetClass::*get)();
	type	(MinClass::*min)();
	type	(MaxClass::*max)();
	type	(StepClass::*step)();
	type	defaultValue;
	
	void	load (){
				setValue((type)settingsXML->getValue(settingsLabel+":"+label, defaultValue));
			};
	
	void	setValue (type value){
				if		(value < (minObject->*min)()) value = (minObject->*min)();
				else if (value > (maxObject->*max)()) value = (maxObject->*max)();
				(setObject->*set)(value);
				settingsXML->setValue(settingsLabel+":"+label, value, 0);
				settingsXML->saveFile(settingsLabel+".xml");
			};
	
	void	keyPressed(int key){
				if		(key == OF_KEY_RIGHT)	setValue((getObject->*get)() + (stepObject->*step)());
				else if	(key == OF_KEY_LEFT)	setValue((getObject->*get)() - (stepObject->*step)());
			};
	
	void	draw(int x, int y, bool isCurProperty = false){
				ofxKeyboardBaseProperty::beginDraw();
				output += ofToString((getObject->*get)());
				if (allowControl) output += " (min " + ofToString((minObject->*min)()) + ", max " + ofToString((maxObject->*max)()) + ", step " + ofToString((stepObject->*step)()) + ")";
				ofxKeyboardBaseProperty::endDraw(x, y, isCurProperty);
			};
};
template <typename type>
struct ofxKeyboardControlPropertyBoolHelper {
	type	min(){return 0;};
	type	max(){return 1;};
	type	step(){return 1;};
};
////////////////////////////////////////////////////////////
// CLASS DEFINITION ----------------------------------------
////////////////////////////////////////////////////////////
class ofxKeyboardSettings {
	
public:
	
	void				setup(int accessKey, string label);
	void				keyPressed(int key);
	void				draw(int x = 0, int y = 0);
	void				saveSettings();
	void				loadSettings();	
	
	
	// STATIC PROPERTY --------------------------------------------
	template <typename type>
	ofxKeyboardStaticProperty<type>*	addProperty(type(*get)(), string label);
	
	// VARIABLE PROPERTY (monitor)---------------------------------
	template <typename type>
	ofxKeyboardProperty<type>*	addProperty(type* var, string label);
	// VARIABLE PROPERTY ------------------------------------------
	template <typename type>
	ofxKeyboardProperty<type>*	addProperty(type* var, string label, type min, type max, type step, type defaultValue);
	// VARIABLE PROPERTY (special case for bool) ------------------ 
	template <typename type>
	ofxKeyboardProperty<type>*	addProperty(type* var, string label, type defaultValue);
	
	// CONTROL PROPERTY (monitor)---------------------------------
	template <typename type, typename GetClass>
	ofxKeyboardControlProperty<type, GetClass, GetClass, GetClass, GetClass, GetClass>*
	addProperty(GetClass* getObject, type(GetClass::*get)(),
				string label
				);	
	// CONTROL PROPERTY ------------------------------------------
	template <typename type, typename GetClass, typename SetClass, typename MinClass, typename MaxClass, typename StepClass>
	ofxKeyboardControlProperty<type, GetClass, SetClass, MinClass, MaxClass, StepClass>*
	addProperty(GetClass* getObject, type(GetClass::*get)(),
				SetClass* setObject, void(SetClass::*set)(type value),
				string label, 
				MinClass* minObject, type(MinClass::*min)(),
				MaxClass* maxObject, type(MaxClass::*max)(),
				StepClass* stepObject, type(StepClass::*step)(),
				type defaultValue
				);
	// CONTROL PROPERTY (special case for bool) ------------------
	template <typename type, typename GetClass, typename SetClass>
	ofxKeyboardControlProperty<type, GetClass, SetClass, ofxKeyboardControlPropertyBoolHelper<type>, ofxKeyboardControlPropertyBoolHelper<type>, ofxKeyboardControlPropertyBoolHelper<type> >*
	addProperty(GetClass* getObject, type(GetClass::*get)(),
				SetClass* setObject, void(SetClass::*set)(type value),
				string label, 
				type defaultValue
				);
		
private:	
	vector<ofxKeyboardBaseProperty*>	properties;
	ofxKeyboardBaseProperty*			curProperty;
	vector<ofxKeyboardBaseProperty*>::iterator curPropertyIterator;

	ofxXmlSettings		settings;
	
	int					accessKey, lastProccessedKey;
	float				accessKeyStartTime, lastProccessedKeyTime;
	
	string				label;	
	bool				isActive;	
};
///////////////////////////////////////////////////////////////////////////////////
// addProperty --------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////
// STATIC PROPERTY --------------------------------------------
template <typename type>
ofxKeyboardStaticProperty<type>* ofxKeyboardSettings::addProperty(type(*get)(), string label){
	ofxKeyboardStaticProperty<type>* property;
	property = new ofxKeyboardStaticProperty<type>();
	property->allowControl = false;
	property->get = get;
	property->label = label;
	
	properties.push_back(property);	
	
	curPropertyIterator = properties.begin();
}

// VARIABLE PROPERTY (monitor)---------------------------------
template <typename type>
ofxKeyboardProperty<type>* ofxKeyboardSettings::addProperty(type* var, string label){
	ofxKeyboardProperty<type>* property;
	property = new ofxKeyboardProperty<type>();
	property->settingsXML = &settings;
	property->settingsLabel = this->label;
	property->allowControl = false;
	property->var = var;
	property->label = label;
		
	properties.push_back(property);	
	
	curPropertyIterator = properties.begin();
	
	return property;
}
// VARIABLE PROPERTY ------------------------------------------
template <typename type>
ofxKeyboardProperty<type>* ofxKeyboardSettings::addProperty(type* var, string label, type min, type max, type step, type defaultValue){
	ofxKeyboardProperty<type>* property;
	property = new ofxKeyboardProperty<type>();
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
	
	curPropertyIterator = properties.begin();
	
	return property;
}
// VARIABLE PROPERTY (special case for bool) ------------------
template <typename type>
ofxKeyboardProperty<type>* ofxKeyboardSettings::addProperty(type* var, string label, type defaultValue){
	ofxKeyboardProperty<type>* property;
	property = new ofxKeyboardProperty<type>();
	property->settingsXML = &settings;
	property->settingsLabel = this->label;
	property->allowControl = true;
	property->var = var;
	property->label = label;
	property->min = false;
	property->max = true;
	property->step = true;
	property->defaultValue = defaultValue;
	property->load();
	
	properties.push_back(property);	
	
	curPropertyIterator = properties.begin();
	
	return property;
}
// CONTROL PROPERTY (monitor) ---------------------------------
template <typename type, typename GetClass>
ofxKeyboardControlProperty<type, GetClass, GetClass, GetClass, GetClass, GetClass>*
ofxKeyboardSettings::addProperty(GetClass* getObject, type(GetClass::*get)(),
								 string label
								 ){
	ofxKeyboardControlProperty<type, GetClass, GetClass, GetClass, GetClass, GetClass>* property;
	property = new ofxKeyboardControlProperty<type, GetClass, GetClass, GetClass, GetClass, GetClass>();
	property->allowControl = false;
	property->getObject = getObject;
	property->get = get;
	property->label = label;
	
	properties.push_back(property);	
	
	curPropertyIterator = properties.begin();
}
// CONTROL PROPERTY ------------------------------------------
template <typename type, typename GetClass, typename SetClass, typename MinClass, typename MaxClass, typename StepClass>
ofxKeyboardControlProperty<type, GetClass, SetClass, MinClass, MaxClass, StepClass>*
ofxKeyboardSettings::addProperty(GetClass* getObject, type(GetClass::*get)(),
								 SetClass* setObject, void(SetClass::*set)(type value),
								 string label, 
								 MinClass* minObject, type(MinClass::*min)(),
								 MaxClass* maxObject, type(MaxClass::*max)(),
								 StepClass* stepObject, type(StepClass::*step)(),
								 type defaultValue
								 ){
	ofxKeyboardControlProperty<type, GetClass, SetClass, MinClass, MaxClass, StepClass>* property;
	property = new ofxKeyboardControlProperty<type, GetClass, SetClass, MinClass, MaxClass, StepClass>();
	property->settingsXML = &settings;
	property->settingsLabel = this->label;
	property->allowControl = true;
	property->getObject = getObject;
	property->setObject = setObject;
	property->minObject = minObject;
	property->maxObject = maxObject;
	property->stepObject = stepObject;
	property->set = set;
	property->get = get;
	property->label = label;
	property->min = min;
	property->max = max;
	property->step = step;
	property->defaultValue = defaultValue;
	property->load();
	
	properties.push_back(property);	
	
	curPropertyIterator = properties.begin();
}
// CONTROL PROPERTY (special case for bool) ------------------
template <typename type, typename GetClass, typename SetClass>
ofxKeyboardControlProperty<type, GetClass, SetClass, ofxKeyboardControlPropertyBoolHelper<type>, ofxKeyboardControlPropertyBoolHelper<type>, ofxKeyboardControlPropertyBoolHelper<type> >*
ofxKeyboardSettings::addProperty(GetClass* getObject, type(GetClass::*get)(),
								 SetClass* setObject, void(SetClass::*set)(type value),
								 string label, 
								 type defaultValue
								 ){
	ofxKeyboardControlPropertyBoolHelper<type>* boolHelper = new ofxKeyboardControlPropertyBoolHelper<type>();
	
	ofxKeyboardControlProperty<type, GetClass, SetClass, ofxKeyboardControlPropertyBoolHelper<type>, ofxKeyboardControlPropertyBoolHelper<type>, ofxKeyboardControlPropertyBoolHelper<type> >* property;
	property = new ofxKeyboardControlProperty<type, GetClass, SetClass, ofxKeyboardControlPropertyBoolHelper<type>, ofxKeyboardControlPropertyBoolHelper<type>, ofxKeyboardControlPropertyBoolHelper<type> >();
	property->settingsXML = &settings;
	property->settingsLabel = this->label;
	property->allowControl = true;
	property->getObject = getObject;
	property->setObject = setObject;
	property->minObject = boolHelper;
	property->maxObject = boolHelper;
	property->stepObject = boolHelper;
	property->get = get;
	property->set = set;
	property->label = label;	
	property->min = &ofxKeyboardControlPropertyBoolHelper<type>::min;
	property->max = &ofxKeyboardControlPropertyBoolHelper<type>::max;
	property->step = &ofxKeyboardControlPropertyBoolHelper<type>::step;
	property->defaultValue = defaultValue;
	property->load();
	
	properties.push_back(property);	
	
	curPropertyIterator = properties.begin();
	
	return property;
}
#endif