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
	bool	isCallback;
	
	virtual void	load (){};	
	virtual void	keyPressed(int key){};	
	virtual void	draw(float x, float y, bool isCurProperty = false){};
	
	virtual void	beginDraw(){
						output = "";
						if(isCallback){
							 output += "(callback) ";
						}
						else {
							if (!allowControl) output += "(monitor) ";	
						}									
						output += label;
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

// ofxKeyboardProperty -----------------------------------
template <typename type, typename GetClass, typename SetClass, typename SetReturnClass,typename MinClass, typename MaxClass, typename StepClass, typename CallbackClass, typename CallbackReturnClass>
struct ofxKeyboardProperty : public ofxKeyboardBaseProperty {
	ofxKeyboardProperty(){
		getObject = NULL;
		setObject = NULL;
		minObject = NULL;
		maxObject = NULL;
		stepObject = NULL;
		callbackObject = NULL;
	}
	
	GetClass*		getObject;
	SetClass*		setObject;
	MinClass*		minObject;
	MaxClass*		maxObject;
	StepClass*		stepObject;
	CallbackClass*	callbackObject;
	
	SetReturnClass	(SetClass::*set)(type value);
	type			(GetClass::*get)();
	type			(MinClass::*min)();
	type			(MaxClass::*max)();
	type			(StepClass::*step)();
	CallbackReturnClass (CallbackClass::*callback)();
	type			defaultValue;
	
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
				if(isCallback){
					if	(key == OF_KEY_RIGHT || key == OF_KEY_LEFT || key == OF_KEY_RETURN) (callbackObject->*callback)();
				}
				else if (allowControl) {
					if		(key == OF_KEY_RIGHT && ((getObject->*get)() < (maxObject->*max)()) )	setValue((getObject->*get)() + (stepObject->*step)());
					else if	(key == OF_KEY_LEFT  && ((getObject->*get)() > (maxObject->*min)()) )	setValue((getObject->*get)() - (stepObject->*step)());
				}
			};
	
	void	draw(float x, float y, bool isCurProperty = false){
				ofxKeyboardBaseProperty::beginDraw();
				if(!isCallback){
					output += ": ";
					output += ofToString((getObject->*get)());
					if (allowControl) output += " (min " + ofToString((minObject->*min)()) + ", max " + ofToString((maxObject->*max)()) + ", step " + ofToString((stepObject->*step)()) + ")";
				}
				ofxKeyboardBaseProperty::endDraw(x, y, isCurProperty);
			};
};

template <typename type, typename CallbackReturnClass>
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
	
	CallbackReturnClass	(*staticCallbackPtr)();
	CallbackReturnClass	staticCallback(){return (*staticCallbackPtr)();};
	
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
	ofxKeyboardProperty<type, ofxKeyboardPropertyHelper<type, type>, ofxKeyboardPropertyHelper<type, type>, void, ofxKeyboardPropertyHelper<type, type>, ofxKeyboardPropertyHelper<type, type>, ofxKeyboardPropertyHelper<type, type>, ofxKeyboardPropertyHelper<type, type>, type>*
	addMonitor(type(*get)(), string label);
	
	template <typename type>
	ofxKeyboardProperty<type, ofxKeyboardPropertyHelper<type, type>, ofxKeyboardPropertyHelper<type, type>, void, ofxKeyboardPropertyHelper<type, type>, ofxKeyboardPropertyHelper<type, type>, ofxKeyboardPropertyHelper<type, type>, ofxKeyboardPropertyHelper<type, type>, type>*
	addMonitor(type* var, string label);
	
	template <typename type, typename GetClass>
	ofxKeyboardProperty<type, GetClass, ofxKeyboardPropertyHelper<type, type>, void, ofxKeyboardPropertyHelper<type, type>, ofxKeyboardPropertyHelper<type, type>, ofxKeyboardPropertyHelper<type, type>, ofxKeyboardPropertyHelper<type, type>, type>*
	addMonitor(GetClass* getObject, type(GetClass::*get)(), string label);
	
	// addCallback --------------------------------------------------
	template <typename CallbackReturnClass>
	ofxKeyboardProperty<int, ofxKeyboardPropertyHelper<int, int>, ofxKeyboardPropertyHelper<int, int>, void, ofxKeyboardPropertyHelper<int, int>, ofxKeyboardPropertyHelper<int, int>, ofxKeyboardPropertyHelper<int, int>, ofxKeyboardPropertyHelper<int, CallbackReturnClass>, CallbackReturnClass>*
	addCallback(CallbackReturnClass(*callback)(), string label);
	
	template <typename CallbackClass, typename CallbackReturnClass>
	ofxKeyboardProperty<int, ofxKeyboardPropertyHelper<int, int>, ofxKeyboardPropertyHelper<int, int>, void, ofxKeyboardPropertyHelper<int, int>, ofxKeyboardPropertyHelper<int, int>, ofxKeyboardPropertyHelper<int, int>, CallbackClass, CallbackReturnClass>*
	addCallback(CallbackClass* callbackObject, CallbackReturnClass(CallbackClass::*callback)(), string label);
	
	
	// addProperty -------------------------------------------------- 
	template <typename type>
	ofxKeyboardProperty<type, ofxKeyboardPropertyHelper<type, type>, ofxKeyboardPropertyHelper<type, type>, void, ofxKeyboardPropertyHelper<type, type>, ofxKeyboardPropertyHelper<type, type>, ofxKeyboardPropertyHelper<type, type>, ofxKeyboardPropertyHelper<type, type>, type>*
	addProperty(type* var, string label, type min, type max, type step, type defaultValue);
	
	template <typename type, typename GetClass, typename SetClass, typename SetReturnClass,typename MinClass, typename MaxClass, typename StepClass>
	ofxKeyboardProperty<type, GetClass, SetClass, SetReturnClass, MinClass, MaxClass, StepClass, ofxKeyboardPropertyHelper<type, type>, type>*
	addProperty(GetClass* getObject, type(GetClass::*get)(),
				SetClass* setObject, SetReturnClass(SetClass::*set)(type value),
				string label, 
				MinClass* minObject, type(MinClass::*min)(),
				MaxClass* maxObject, type(MaxClass::*max)(),
				StepClass* stepObject, type(StepClass::*step)(),
				type defaultValue
				);
	
	// addProperty (special case for booleans)-----------------------------------------
	template <typename GetClass, typename SetClass, typename SetReturnClass>
	ofxKeyboardProperty<bool, GetClass, SetClass, SetReturnClass, ofxKeyboardPropertyHelper<bool, bool>, ofxKeyboardPropertyHelper<bool, bool>, ofxKeyboardPropertyHelper<bool, bool>, ofxKeyboardPropertyHelper<bool, bool>, bool >*
	addProperty(GetClass* getObject, bool(GetClass::*get)(),
				SetClass* setObject, SetReturnClass(SetClass::*set)(bool value),
				string label,
				bool defaultValue);
	
	// As this is not a template, it can only be defined here inside the class, or in hte cpp file, but not in the end of this h files, as the other template methods
	ofxKeyboardProperty<bool, ofxKeyboardPropertyHelper<bool, bool>, ofxKeyboardPropertyHelper<bool, bool>, void, ofxKeyboardPropertyHelper<bool, bool>, ofxKeyboardPropertyHelper<bool, bool>, ofxKeyboardPropertyHelper<bool, bool>, ofxKeyboardPropertyHelper<bool, bool>, bool >*
	addProperty(bool* var, string label, bool defaultValue){
		
		ofxKeyboardPropertyHelper<bool, bool>* helper = new ofxKeyboardPropertyHelper<bool, bool>();
		helper->_var = var;
		
		return addProperty(helper, &ofxKeyboardPropertyHelper<bool, bool>::get,
						   helper, &ofxKeyboardPropertyHelper<bool, bool>::set,
						   label,
						   defaultValue);
	};
	
	// canonicalAddProperty --------------------------------------------------
	// This is the single entry points where all the properties are added
	template <typename type, typename GetClass, typename SetClass, typename SetReturnClass,typename MinClass, typename MaxClass, typename StepClass, typename CallbackClass, typename CallbackReturnClass>
	ofxKeyboardProperty<type, GetClass, SetClass, SetReturnClass, MinClass, MaxClass, StepClass, CallbackClass, CallbackReturnClass>*
	canonicalAddProperty(GetClass* getObject, type(GetClass::*get)(),
						 SetClass* setObject, SetReturnClass(SetClass::*set)(type value),
						 string label,
						 MinClass* minObject, type(MinClass::*min)(),
						 MaxClass* maxObject, type(MaxClass::*max)(),
						 StepClass* stepObject, type(StepClass::*step)(),
						 type defaultValue,
						 bool allowControl,
						 CallbackClass* callbackObject, CallbackReturnClass(CallbackClass::*callback)() 
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
ofxKeyboardProperty<type, ofxKeyboardPropertyHelper<type, type>, ofxKeyboardPropertyHelper<type, type>, void, ofxKeyboardPropertyHelper<type, type>, ofxKeyboardPropertyHelper<type, type>, ofxKeyboardPropertyHelper<type, type>, ofxKeyboardPropertyHelper<type, type>, type>*
ofxKeyboardSettings::addMonitor(type(*get)(), string label){
	
	ofxKeyboardPropertyHelper<type, type>* helper = new ofxKeyboardPropertyHelper<type, type>();
	helper->staticGetPtr = get;
	
	return addMonitor(helper, &ofxKeyboardPropertyHelper<type, type>::staticGet, label);
}

template <typename type>
ofxKeyboardProperty<type, ofxKeyboardPropertyHelper<type, type>, ofxKeyboardPropertyHelper<type, type>, void, ofxKeyboardPropertyHelper<type, type>, ofxKeyboardPropertyHelper<type, type>, ofxKeyboardPropertyHelper<type, type>, ofxKeyboardPropertyHelper<type, type>, type>*
ofxKeyboardSettings::addMonitor(type* var, string label){
	
	ofxKeyboardPropertyHelper<type, type>* helper = new ofxKeyboardPropertyHelper<type, type>();
	helper->_var = var;
	
	return addMonitor(helper, &ofxKeyboardPropertyHelper<type, type>::get, label);
}

template <typename type, typename GetClass>
ofxKeyboardProperty<type, GetClass, ofxKeyboardPropertyHelper<type, type>, void, ofxKeyboardPropertyHelper<type, type>, ofxKeyboardPropertyHelper<type, type>, ofxKeyboardPropertyHelper<type, type>, ofxKeyboardPropertyHelper<type, type>, type>*
ofxKeyboardSettings::addMonitor(GetClass* getObject, type(GetClass::*get)(), string label){
	
	ofxKeyboardPropertyHelper<type, type>* helper = new ofxKeyboardPropertyHelper<type, type>();
	
	type emptyInitializedObject = type();
	return canonicalAddProperty(getObject, get,
							    helper, &ofxKeyboardPropertyHelper<type, type>::set,
							    label,
							    helper, &ofxKeyboardPropertyHelper<type, type>::min,
							    helper, &ofxKeyboardPropertyHelper<type, type>::max,
							    helper, &ofxKeyboardPropertyHelper<type, type>::step,
							    emptyInitializedObject,
							    false,
								(ofxKeyboardPropertyHelper<type, type>*)NULL, &ofxKeyboardPropertyHelper<type, type>::get);
}

///////////////////////////////////////////////////////////////////////////////////
// addCallback --------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////
template <typename CallbackReturnClass>
ofxKeyboardProperty<int, ofxKeyboardPropertyHelper<int, int>, ofxKeyboardPropertyHelper<int, int>, void, ofxKeyboardPropertyHelper<int, int>, ofxKeyboardPropertyHelper<int, int>, ofxKeyboardPropertyHelper<int, int>, ofxKeyboardPropertyHelper<int, CallbackReturnClass>, CallbackReturnClass>*
ofxKeyboardSettings::addCallback(CallbackReturnClass(*callback)(), string label){
	ofxKeyboardPropertyHelper<int, CallbackReturnClass>* helper = new ofxKeyboardPropertyHelper<int, CallbackReturnClass>();
	helper->staticCallbackPtr = callback;
	
	return addCallback(helper, &ofxKeyboardPropertyHelper<int, CallbackReturnClass>::staticCallback, label);
}

template <typename CallbackClass, typename CallbackReturnClass>
ofxKeyboardProperty<int, ofxKeyboardPropertyHelper<int, int>, ofxKeyboardPropertyHelper<int, int>, void, ofxKeyboardPropertyHelper<int, int>, ofxKeyboardPropertyHelper<int, int>, ofxKeyboardPropertyHelper<int, int>, CallbackClass, CallbackReturnClass>*
ofxKeyboardSettings::addCallback(CallbackClass* callbackObject, CallbackReturnClass(CallbackClass::*callback)(), string label){
	return canonicalAddProperty((ofxKeyboardPropertyHelper<int, int>*)NULL, &ofxKeyboardPropertyHelper<int, int>::get,
							    (ofxKeyboardPropertyHelper<int, int>*)NULL, &ofxKeyboardPropertyHelper<int, int>::set,
							    label,
							    (ofxKeyboardPropertyHelper<int, int>*)NULL, &ofxKeyboardPropertyHelper<int, int>::min,
							    (ofxKeyboardPropertyHelper<int, int>*)NULL, &ofxKeyboardPropertyHelper<int, int>::max,
							    (ofxKeyboardPropertyHelper<int, int>*)NULL, &ofxKeyboardPropertyHelper<int, int>::step,
							    (int)NULL,
							    false,
								callbackObject, callback);
}

///////////////////////////////////////////////////////////////////////////////////
// addProperty --------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////
template <typename type>
ofxKeyboardProperty<type, ofxKeyboardPropertyHelper<type, type>, ofxKeyboardPropertyHelper<type, type>, void, ofxKeyboardPropertyHelper<type, type>, ofxKeyboardPropertyHelper<type, type>, ofxKeyboardPropertyHelper<type, type>, ofxKeyboardPropertyHelper<type, type>, type>*
ofxKeyboardSettings::addProperty(type* var, string label, type min, type max, type step, type defaultValue){
	
	ofxKeyboardPropertyHelper<type, type>* helper = new ofxKeyboardPropertyHelper<type, type>();
	helper->_var = var;
	helper->_min = min;
	helper->_max = max;
	helper->_step = step;
	
	return addProperty(helper, &ofxKeyboardPropertyHelper<type, type>::get,
					   helper, &ofxKeyboardPropertyHelper<type, type>::set,
					   label,
					   helper, &ofxKeyboardPropertyHelper<type, type>::min,
					   helper, &ofxKeyboardPropertyHelper<type, type>::max,
					   helper, &ofxKeyboardPropertyHelper<type, type>::step,
					   defaultValue);
}

template <typename type, typename GetClass, typename SetClass, typename SetReturnClass,typename MinClass, typename MaxClass, typename StepClass>
ofxKeyboardProperty<type, GetClass, SetClass, SetReturnClass, MinClass, MaxClass, StepClass, ofxKeyboardPropertyHelper<type, type>, type>*
ofxKeyboardSettings::addProperty(GetClass* getObject, type(GetClass::*get)(),
								 SetClass* setObject, SetReturnClass(SetClass::*set)(type value),
								 string label, 
								 MinClass* minObject, type(MinClass::*min)(),
								 MaxClass* maxObject, type(MaxClass::*max)(),
								 StepClass* stepObject, type(StepClass::*step)(),
								 type defaultValue
								 ){
	
	return canonicalAddProperty(getObject, get,
							    setObject, set,
							    label,
							    minObject, min,
							    maxObject, max,
							    stepObject, step,
							    defaultValue,
							    true,
							    (ofxKeyboardPropertyHelper<type, type>*)NULL, &ofxKeyboardPropertyHelper<type, type>::get);
}

///////////////////////////////////////////////////////////////////////////////////
// addProperty (special case for booleans)-----------------------------------------
///////////////////////////////////////////////////////////////////////////////////
template <typename GetClass, typename SetClass, typename SetReturnClass>
ofxKeyboardProperty<bool, GetClass, SetClass, SetReturnClass, ofxKeyboardPropertyHelper<bool, bool>, ofxKeyboardPropertyHelper<bool, bool>, ofxKeyboardPropertyHelper<bool, bool>, ofxKeyboardPropertyHelper<bool, bool>, bool >*
ofxKeyboardSettings::addProperty(GetClass* getObject, bool(GetClass::*get)(),
			SetClass* setObject, SetReturnClass(SetClass::*set)(bool value),
			string label,
			bool defaultValue){
	
	ofxKeyboardPropertyHelper<bool, bool>* helper = new ofxKeyboardPropertyHelper<bool, bool>();
	helper->_min = false;
	helper->_max = true;
	helper->_step = 1;
	
	return canonicalAddProperty(getObject, get,
							    setObject, set,
							    label,
							    helper, &ofxKeyboardPropertyHelper<bool, bool>::min,
							    helper, &ofxKeyboardPropertyHelper<bool, bool>::max,
							    helper, &ofxKeyboardPropertyHelper<bool, bool>::step,
							    defaultValue,
							    true,
							    (ofxKeyboardPropertyHelper<bool, bool>*)NULL, &ofxKeyboardPropertyHelper<bool, bool>::get);
}

///////////////////////////////////////////////////////////////////////////////////
// canonicalAddProperty -----------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////
template <typename type, typename GetClass, typename SetClass, typename SetReturnClass,typename MinClass, typename MaxClass, typename StepClass, typename CallbackClass, typename CallbackReturnClass>
ofxKeyboardProperty<type, GetClass, SetClass, SetReturnClass, MinClass, MaxClass, StepClass, CallbackClass, CallbackReturnClass>*
ofxKeyboardSettings::canonicalAddProperty(GetClass* getObject, type(GetClass::*get)(),
										  SetClass* setObject, SetReturnClass(SetClass::*set)(type value),
										  string label, 
										  MinClass* minObject, type(MinClass::*min)(),
										  MaxClass* maxObject, type(MaxClass::*max)(),
										  StepClass* stepObject, type(StepClass::*step)(),
										  type defaultValue,
										  bool allowControl,
										  CallbackClass* callbackObject, CallbackReturnClass(CallbackClass::*callback)()
										  ){
	ofxKeyboardProperty<type, GetClass, SetClass, SetReturnClass, MinClass, MaxClass, StepClass, CallbackClass, CallbackReturnClass>* property;
	property = new ofxKeyboardProperty<type, GetClass, SetClass, SetReturnClass, MinClass, MaxClass, StepClass, CallbackClass, CallbackReturnClass>();
	property->allowControl = allowControl;
	property->label = label;
	
	if(!callbackObject){
		property->getObject = getObject;	
		property->get = get;
		property->isCallback = false;
	}
	else {
		property->callbackObject = callbackObject;	
		property->callback = callback;
		property->isCallback = true;
	}
	
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
#endif