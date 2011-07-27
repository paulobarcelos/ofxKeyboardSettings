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
	
	type	(*staticGetPtr)();
	type	staticGet(){return (*staticGetPtr)();};
	
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
	ofxKeyboardControlProperty<type, ofxKeyboardPropertyHelper<type>, ofxKeyboardPropertyHelper<type>, ofxKeyboardPropertyHelper<type>, ofxKeyboardPropertyHelper<type>, ofxKeyboardPropertyHelper<type> >*
	addMonitor(type(*get)(), string label);
	
	template <typename type>
	ofxKeyboardControlProperty<type, ofxKeyboardPropertyHelper<type>, ofxKeyboardPropertyHelper<type>, ofxKeyboardPropertyHelper<type>, ofxKeyboardPropertyHelper<type>, ofxKeyboardPropertyHelper<type> >*
	addMonitor(type* var, string label);
	
	template <typename type, typename GetClass>
	ofxKeyboardControlProperty<type, GetClass, ofxKeyboardPropertyHelper<type>, ofxKeyboardPropertyHelper<type>, ofxKeyboardPropertyHelper<type>, ofxKeyboardPropertyHelper<type> >*
	addMonitor(GetClass* getObject, type(GetClass::*get)(), string label);
	
	
	// addProperty -------------------------------------------------- 
	template <typename type>
	ofxKeyboardControlProperty<type, ofxKeyboardPropertyHelper<type>, ofxKeyboardPropertyHelper<type>, ofxKeyboardPropertyHelper<type>, ofxKeyboardPropertyHelper<type>, ofxKeyboardPropertyHelper<type> >*
	addProperty(type* var, string label, type min, type max, type step, type defaultValue);
	
	template <typename type, typename GetClass, typename SetClass, typename MinClass, typename MaxClass, typename StepClass>
	ofxKeyboardControlProperty<type, GetClass, SetClass, MinClass, MaxClass, StepClass>*
	addProperty(GetClass* getObject, type(GetClass::*get)(),
				SetClass* setObject, void(SetClass::*set)(type value),
				string label, 
				MinClass* minObject, type(MinClass::*min)(),
				MaxClass* maxObject, type(MaxClass::*max)(),
				StepClass* stepObject, type(StepClass::*step)(),
				type defaultValue,
				bool allowControl = true
				);
	
	
	
	// addProperty (special case for booleans)-----------------------------------------
	template <typename GetClass, typename SetClass>
	ofxKeyboardControlProperty<bool, GetClass, SetClass, ofxKeyboardPropertyHelper<bool>, ofxKeyboardPropertyHelper<bool>, ofxKeyboardPropertyHelper<bool> >*
	addProperty(GetClass* getObject, bool(GetClass::*get)(),
				SetClass* setObject, void(SetClass::*set)(bool value),
				string label,
				bool defaultValue);
	
	// As this is not a template, it can only be defined here inside the class, or in hte cpp file,not in the end of this h files, as the other template methods
	ofxKeyboardControlProperty<bool, ofxKeyboardPropertyHelper<bool>, ofxKeyboardPropertyHelper<bool>, ofxKeyboardPropertyHelper<bool>, ofxKeyboardPropertyHelper<bool>, ofxKeyboardPropertyHelper<bool> >*
	addProperty(bool* var, string label, bool defaultValue){
		
		ofxKeyboardPropertyHelper<bool>* helper = new ofxKeyboardPropertyHelper<bool>();
		helper->_var = var;
		
		return addProperty(helper, &ofxKeyboardPropertyHelper<bool>::get,
						   helper, &ofxKeyboardPropertyHelper<bool>::set,
						   label,
						   defaultValue);
	};
		
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
ofxKeyboardControlProperty<type, ofxKeyboardPropertyHelper<type>, ofxKeyboardPropertyHelper<type>, ofxKeyboardPropertyHelper<type>, ofxKeyboardPropertyHelper<type>, ofxKeyboardPropertyHelper<type> >*
ofxKeyboardSettings::addMonitor(type(*get)(), string label){
	
	ofxKeyboardPropertyHelper<type>* helper = new ofxKeyboardPropertyHelper<type>();
	helper->staticGetPtr = get;
	
	return addMonitor(helper, &ofxKeyboardPropertyHelper<type>::staticGet, label);
}

template <typename type>
ofxKeyboardControlProperty<type, ofxKeyboardPropertyHelper<type>, ofxKeyboardPropertyHelper<type>, ofxKeyboardPropertyHelper<type>, ofxKeyboardPropertyHelper<type>, ofxKeyboardPropertyHelper<type> >*
ofxKeyboardSettings::addMonitor(type* var, string label){
	
	ofxKeyboardPropertyHelper<type>* helper = new ofxKeyboardPropertyHelper<type>();
	helper->_var = var;
	
	return addMonitor(helper, &ofxKeyboardPropertyHelper<type>::get, label);
}

template <typename type, typename GetClass>
ofxKeyboardControlProperty<type, GetClass, ofxKeyboardPropertyHelper<type>, ofxKeyboardPropertyHelper<type>, ofxKeyboardPropertyHelper<type>, ofxKeyboardPropertyHelper<type> >*
ofxKeyboardSettings::addMonitor(GetClass* getObject, type(GetClass::*get)(), string label){
	
	ofxKeyboardPropertyHelper<type>* helper = new ofxKeyboardPropertyHelper<type>();
	
	return addProperty(getObject, get,
					   helper, &ofxKeyboardPropertyHelper<type>::set,
					   label,
					   helper, &ofxKeyboardPropertyHelper<type>::min,
					   helper, &ofxKeyboardPropertyHelper<type>::max,
					   helper, &ofxKeyboardPropertyHelper<type>::step,
					   (type)NULL,
					   false);
}

///////////////////////////////////////////////////////////////////////////////////
// addProperty --------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////
template <typename type>
ofxKeyboardControlProperty<type, ofxKeyboardPropertyHelper<type>, ofxKeyboardPropertyHelper<type>, ofxKeyboardPropertyHelper<type>, ofxKeyboardPropertyHelper<type>, ofxKeyboardPropertyHelper<type> >*
ofxKeyboardSettings::addProperty(type* var, string label, type min, type max, type step, type defaultValue){
	
	ofxKeyboardPropertyHelper<type>* helper = new ofxKeyboardPropertyHelper<type>();
	helper->_var = var;
	helper->_min = min;
	helper->_max = max;
	helper->_step = step;
	
	return addProperty(helper, &ofxKeyboardPropertyHelper<type>::get,
					   helper, &ofxKeyboardPropertyHelper<type>::set,
					   label,
					   helper, &ofxKeyboardPropertyHelper<type>::min,
					   helper, &ofxKeyboardPropertyHelper<type>::max,
					   helper, &ofxKeyboardPropertyHelper<type>::step,
					   defaultValue);
}

template <typename type, typename GetClass, typename SetClass, typename MinClass, typename MaxClass, typename StepClass>
ofxKeyboardControlProperty<type, GetClass, SetClass, MinClass, MaxClass, StepClass>*
ofxKeyboardSettings::addProperty(GetClass* getObject, type(GetClass::*get)(),
								 SetClass* setObject, void(SetClass::*set)(type value),
								 string label, 
								 MinClass* minObject, type(MinClass::*min)(),
								 MaxClass* maxObject, type(MaxClass::*max)(),
								 StepClass* stepObject, type(StepClass::*step)(),
								 type defaultValue,
								 bool allowControl
								 ){
	ofxKeyboardControlProperty<type, GetClass, SetClass, MinClass, MaxClass, StepClass>* property;
	property = new ofxKeyboardControlProperty<type, GetClass, SetClass, MinClass, MaxClass, StepClass>();
	property->allowControl = allowControl;
	property->getObject = getObject;	
	property->get = get;
	property->label = label;
	
	if (allowControl) {
		property->settingsXML = &settings;
		property->settingsLabel = this->label;
		property->setObject = setObject;
		property->minObject = minObject;
		property->maxObject = maxObject;
		property->stepObject = stepObject;
		property->set = set;
		property->min = min;
		property->max = max;
		property->step = step;
		property->defaultValue = defaultValue;
		property->load();
	}
	
	properties.push_back(property);	
	
	curPropertyIterator = properties.begin();
	
	return property;
}


///////////////////////////////////////////////////////////////////////////////////
// addProperty (special case for booleans)-----------------------------------------
///////////////////////////////////////////////////////////////////////////////////
template <typename GetClass, typename SetClass>
ofxKeyboardControlProperty<bool, GetClass, SetClass, ofxKeyboardPropertyHelper<bool>, ofxKeyboardPropertyHelper<bool>, ofxKeyboardPropertyHelper<bool> >*
ofxKeyboardSettings::addProperty(GetClass* getObject, bool(GetClass::*get)(),
			SetClass* setObject, void(SetClass::*set)(bool value),
			string label,
			bool defaultValue){
	
	ofxKeyboardPropertyHelper<bool>* helper = new ofxKeyboardPropertyHelper<bool>();
	helper->_min = false;
	helper->_max = true;
	helper->_step = 1;
	
	return addProperty(getObject, get,
					   setObject, set,
					   label,
					   helper, &ofxKeyboardPropertyHelper<bool>::min,
					   helper, &ofxKeyboardPropertyHelper<bool>::max,
					   helper, &ofxKeyboardPropertyHelper<bool>::step,
					   defaultValue);
}

#endif