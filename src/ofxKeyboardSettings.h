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
	virtual void	draw(float x, float y, bool isCurProperty = false){};
	
	virtual void	beginDraw(){
						output = "";
						if (!allowControl) output += "(monitor) ";				
						output += label + ": ";
					};
	virtual void	endDraw(float x, float y, bool isCurProperty){
						int grey = (isCurProperty)?0:50;
						ofSetColor(grey, grey, grey, 150);
						ofRect(x, y - KEYBOARD_SETTINGS_VERTICAL_OFFSET, KEYBOARD_SETTINGS_WIDTH, KEYBOARD_SETTINGS_PROPERTY_HEIGHT);
						ofSetColor(255);
						ofDrawBitmapString(output, x+10, y);
					};
	
	string output;
};
// ofxKeyboardStaticProperty ----------------------------------
template <typename type>
struct ofxKeyboardStaticProperty : public ofxKeyboardBaseProperty{
	type	(*get)();
	void	draw(float x, float y, bool isCurProperty = false){
		ofxKeyboardBaseProperty::beginDraw();		
		output += ofToString((*get)());
		ofxKeyboardBaseProperty::endDraw(x, y, isCurProperty);
	};
};
// ofxKeyboardProperty ----------------------------------------
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
				if		(key == OF_KEY_RIGHT && (*var < max))	setValue(*var + step);
				else if	(key == OF_KEY_LEFT  && (*var > min))	setValue(*var - step);
			};
	
	void	draw(float x, float y, bool isCurProperty = false){
				ofxKeyboardBaseProperty::beginDraw();
				output += ofToString(*var);
				if (allowControl) output += " (min " + ofToString(min) + ", max " + ofToString(max) + ", step " + ofToString(step) + ")";
				ofxKeyboardBaseProperty::endDraw(x, y, isCurProperty);
			};
};
// ofxKeyboardControlProperty -----------------------------------
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
				if		(key == OF_KEY_RIGHT && ((getObject->*get)() < (maxObject->*max)()) )	setValue((getObject->*get)() + (stepObject->*step)());
				else if	(key == OF_KEY_LEFT  && ((getObject->*get)() > (maxObject->*min)()) )	setValue((getObject->*get)() - (stepObject->*step)());
			};
	
	void	draw(float x, float y, bool isCurProperty = false){
				ofxKeyboardBaseProperty::beginDraw();
				output += ofToString((getObject->*get)());
				if (allowControl) output += " (min " + ofToString((minObject->*min)()) + ", max " + ofToString((maxObject->*max)()) + ", step " + ofToString((stepObject->*step)()) + ")";
				ofxKeyboardBaseProperty::endDraw(x, y, isCurProperty);
			};
};

template <typename type>
struct ofxKeyboardPropertyHelper {
	type*	_var;
	type	_min;
	type	_max;
	type	_step;
	void	set(type value){*_var = value;};
	type	get(){return *_var;};
	type	min(){return _min;};
	type	max(){return _max;};
	type	step(){return _step;};
};
////////////////////////////////////////////////////////////
// CLASS DEFINITION ----------------------------------------
////////////////////////////////////////////////////////////
class ofxKeyboardSettings {
	
public:
	
	void				setup(int accessKey, string label);
	void				keyPressed(int key);
	void				draw(float x = 0, float y = 0);
	void				saveSettings();
	void				loadSettings();	
	
	
	// addMonitor --------------------------------------------------
	template <typename type>
	ofxKeyboardProperty<type>*			addMonitor(type* var, string label);
	
	template <typename type>
	ofxKeyboardStaticProperty<type>*	addMonitor(type(*get)(), string label);	
	
	template <typename type, typename GetClass>
	ofxKeyboardControlProperty<type, GetClass, GetClass, GetClass, GetClass, GetClass>*
	addMonitor(GetClass* getObject, type(GetClass::*get)(),
				string label
				);
	
	// addProperty --------------------------------------------------
	// Variable pointer
	template <typename type>
	ofxKeyboardProperty<type>*	addProperty(type* var, string label, type min, type max, type step, type defaultValue);	
	// Special case for bool (defined in cpp)
	ofxKeyboardProperty<bool>*	addProperty(bool* var, string label, bool defaultValue);
	
	/*template <typename type>
	ofxKeyboardControlProperty<type, GetClass, SetClass, MinClass, MaxClass, StepClass>*
	addProperty(GetClass* getObject, type(GetClass::*get)(),
				SetClass* setObject, void(SetClass::*set)(type value),
				string label, 
				MinClass* minObject, type(MinClass::*min)(),
				MaxClass* maxObject, type(MaxClass::*max)(),
				StepClass* stepObject, type(StepClass::*step)(),
				type defaultValue
				);*/
	
	
	// Control
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
	// Special case for bool 
	template <typename GetClass, typename SetClass>
	ofxKeyboardControlProperty<bool, GetClass, SetClass, ofxKeyboardPropertyHelper<bool>, ofxKeyboardPropertyHelper<bool>, ofxKeyboardPropertyHelper<bool> >*
	addProperty(GetClass* getObject, bool(GetClass::*get)(),
				SetClass* setObject, void(SetClass::*set)(bool value),
				string label, 
				bool defaultValue
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
// addMonitor ---------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////
template <typename type>
ofxKeyboardProperty<type>* ofxKeyboardSettings::addMonitor(type* var, string label){
	ofxKeyboardProperty<type>* property;
	property = new ofxKeyboardProperty<type>();
	property->allowControl = false;
	property->var = var;
	property->label = label;
	
	properties.push_back(property);	
	
	curPropertyIterator = properties.begin();
	
	return property;
}

template <typename type>
ofxKeyboardStaticProperty<type>* ofxKeyboardSettings::addMonitor(type(*get)(), string label){
	ofxKeyboardStaticProperty<type>* property;
	property = new ofxKeyboardStaticProperty<type>();
	property->allowControl = false;
	property->get = get;
	property->label = label;
	
	properties.push_back(property);	
	
	curPropertyIterator = properties.begin();
}

template <typename type, typename GetClass>
ofxKeyboardControlProperty<type, GetClass, GetClass, GetClass, GetClass, GetClass>*
ofxKeyboardSettings::addMonitor(GetClass* getObject, type(GetClass::*get)(),
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
///////////////////////////////////////////////////////////////////////////////////
// addProperty --------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////
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
// Special case for bool 
template <typename GetClass, typename SetClass>
ofxKeyboardControlProperty<bool, GetClass, SetClass, ofxKeyboardPropertyHelper<bool>, ofxKeyboardPropertyHelper<bool>, ofxKeyboardPropertyHelper<bool> >*
ofxKeyboardSettings::addProperty(GetClass* getObject, bool(GetClass::*get)(),
								 SetClass* setObject, void(SetClass::*set)(bool value),
								 string label, 
								 bool defaultValue
								 ){
	ofxKeyboardPropertyHelper<bool>* boolHelper = new ofxKeyboardPropertyHelper<bool>();
	boolHelper->_min = false;
	boolHelper->_max = true;
	boolHelper->_step = 1;
	
	ofxKeyboardControlProperty<bool, GetClass, SetClass, ofxKeyboardPropertyHelper<bool>, ofxKeyboardPropertyHelper<bool>, ofxKeyboardPropertyHelper<bool> >* property;
	property = new ofxKeyboardControlProperty<bool, GetClass, SetClass, ofxKeyboardPropertyHelper<bool>, ofxKeyboardPropertyHelper<bool>, ofxKeyboardPropertyHelper<bool> >();
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
	property->min = &ofxKeyboardPropertyHelper<bool>::min;
	property->max = &ofxKeyboardPropertyHelper<bool>::max;
	property->step = &ofxKeyboardPropertyHelper<bool>::step;
	property->defaultValue = defaultValue;
	property->load();
	
	properties.push_back(property);	
	
	curPropertyIterator = properties.begin();
	
	return property;
}
#endif