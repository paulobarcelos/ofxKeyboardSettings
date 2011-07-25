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

template <typename type>
struct ofxKeyboardStaticProperty : public ofxKeyboardBaseProperty{
	type	(*get)();
	void	draw(int x, int y, bool isCurProperty = false){
				ofxKeyboardBaseProperty::beginDraw();		
				output += ofToString((*get)());
				ofxKeyboardBaseProperty::endDraw(x, y, isCurProperty);
			};
};
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
/*
// DOUBLE -------------------------------------------------------------
struct ofxKeyboardDoubleProperty : public ofxKeyboardProperty{
	double*	var;
	double	min;
	double	max;
	double	step;
	double	defaultValue;
	
	void	keyPressed(int key){
				if		(key == OF_KEY_RIGHT)	setValue(*var + step);
				else if	(key == OF_KEY_LEFT)	setValue(*var - step);
			};
	void	load (){
				setValue((double)settingsXML->getValue(settingsLabel+":"+label, defaultValue));
			};
	void	setValue (double value){
				if		(value < min) value = min;
				else if (value > max) value = max;
				*var = value;
				settingsXML->setValue(settingsLabel+":"+label, value, 0);
				settingsXML->saveFile(settingsLabel+".xml");
			};
	void	draw(int x, int y, bool isCurProperty = false){
				begingDraw();
				output += ofToString(*var);
				if (allowControl) output += " (min " + ofToString(min) + ", max " + ofToString(max) + ", step " + ofToString(step) + ")";	
				endDraw(x, y, isCurProperty);
			};
};

// FLOAT -------------------------------------------------------------
struct ofxKeyboardFloatProperty : public ofxKeyboardProperty{
	float*	var;
	float	min;
	float	max;
	float	step;
	float	defaultValue;
	
	void	keyPressed(int key){
				if		(key == OF_KEY_RIGHT)	setValue(*var + step);
				else if	(key == OF_KEY_LEFT)	setValue(*var - step);
			};
	void	load (){
				setValue((float)settingsXML->getValue(settingsLabel+":"+label, defaultValue));
			};
	void	setValue (float value){
				if		(value < min) value = min;
				else if (value > max) value = max;
				*var = value;
				settingsXML->setValue(settingsLabel+":"+label, value, 0);
				settingsXML->saveFile(settingsLabel+".xml");
			};
	void	draw(int x, int y, bool isCurProperty = false){
				begingDraw();		
				output += ofToString(*var);
				if (allowControl) output += " (min " + ofToString(min) + ", max " + ofToString(max) + ", step " + ofToString(step) + ")";
				endDraw(x, y, isCurProperty);
			};
};

// INT --------------------------------------------------------------
struct ofxKeyboardIntProperty : public ofxKeyboardProperty{
	int*	var;
	int		min;
	int		max;
	int		step;
	int		defaultValue;
	
	void	keyPressed(int key){
				if		(key == OF_KEY_RIGHT)	setValue(*var + step);
				else if	(key == OF_KEY_LEFT)	setValue(*var - step);
			};
	void	load (){
				setValue((int)settingsXML->getValue(settingsLabel+":"+label, defaultValue));
			};
	void	setValue (int value){
				if		(value < min) value = min;
				else if (value > max) value = max;
				*var = value;
				settingsXML->setValue(settingsLabel+":"+label, value, 0);
				settingsXML->saveFile(settingsLabel+".xml");
			};
	void	draw(int x, int y, bool isCurProperty = false){
				begingDraw();			
				output += ofToString(*var);
				if (allowControl) output += " (min " + ofToString(min) + ", max " + ofToString(max) + ", step " + ofToString(step) + ")";
				endDraw(x, y, isCurProperty);
			};
};

struct ofxKeyboardStaticIntProperty : public ofxKeyboardProperty{
	int		(*get)();
	void	draw(int x, int y, bool isCurProperty = false){
			begingDraw();			
			output += ofToString((*get)());
			endDraw(x, y, isCurProperty);
		};
};

template <typename GetClass, typename SetClass, typename MinClass, typename MaxClass, typename StepClass>
struct ofxKeyboardControlIntProperty : public ofxKeyboardProperty{
	GetClass*	getObject;
	SetClass*	setObject;
	MinClass*	minObject;
	MaxClass*	maxObject;
	StepClass*	stepObject;
	void	(SetClass::*set)(int value);
	int		(GetClass::*get)();
	int		(MinClass::*min)();
	int		(MaxClass::*max)();
	int		(StepClass::*step)();
	int		defaultValue;
	
	void	keyPressed(int key){
		if		(key == OF_KEY_RIGHT)	setValue((getObject->*get)() + (stepObject->*step)());
		else if	(key == OF_KEY_LEFT)	setValue((getObject->*get)() - (stepObject->*step)());
	};
	void	load (){
		setValue((int)settingsXML->getValue(settingsLabel+":"+label, defaultValue));
	};
	void	setValue (int value){
		if		(value < (minObject->*min)()) value = (minObject->*min)();
		else if (value > (maxObject->*max)()) value = (maxObject->*max)();
		(setObject->*set)(value);
		settingsXML->setValue(settingsLabel+":"+label, value, 0);
		settingsXML->saveFile(settingsLabel+".xml");
	};
	void	draw(int x, int y, bool isCurProperty = false){
		begingDraw();			
		output += ofToString((getObject->*get)());
		if (allowControl) output += " (min " + ofToString((minObject->*min)()) + ", max " + ofToString((maxObject->*max)()) + ", step " + ofToString((stepObject->*step)()) + ")";
		endDraw(x, y, isCurProperty);
	};
};
// BOOL -------------------------------------------------------------
struct ofxKeyboardBoolProperty : public ofxKeyboardProperty{
	bool*	var;
	bool	defaultValue;
	
	void	keyPressed(int key){
				if		(key == OF_KEY_RIGHT)	setValue(true);
				else if	(key == OF_KEY_LEFT)	setValue(false);
			};
	void	load (){
				setValue((bool)settingsXML->getValue(settingsLabel+":"+label, defaultValue));
			};
	void	setValue (bool value){
				*var = value;
				settingsXML->setValue(settingsLabel+":"+label, value, 0);
				settingsXML->saveFile(settingsLabel+".xml");
			};
	void	draw(int x, int y, bool isCurProperty = false){
				begingDraw();
				output += (*var)?"true":"false";
				endDraw(x, y, isCurProperty);
			};
};*/
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
	
	/*ofxKeyboardDoubleProperty*	addProperty(double* var, string label);
	ofxKeyboardFloatProperty*	addProperty(float* var, string label);
	ofxKeyboardIntProperty*		addProperty(int* var, string label);
	ofxKeyboardBoolProperty*	addProperty(bool* var, string label);*/
	
	template <typename type>
	ofxKeyboardProperty<type>*	addProperty(type* var, string label);
	
	template <typename type>
	ofxKeyboardProperty<type>*	addProperty(type* var, string label, type min, type max, type step, type defaultValue);
	
	// bool 
	template <typename type>
	ofxKeyboardProperty<type>*	addProperty(type* var, string label, type defaultValue);
	
	//static
	template <typename type>
	ofxKeyboardStaticProperty<type>*	addProperty(type(*get)(), string label);
	
	//control
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
	
	/*ofxKeyboardFloatProperty*	addProperty(float* var, string label, float min, float max, float step, float defaultValue);
	ofxKeyboardIntProperty*		addProperty(int* var, string label, int min, int max, int step, int defaultValue);
	ofxKeyboardBoolProperty*	addProperty(bool* var, string label, bool defaultValue);*/
		
	/*ofxKeyboardStaticIntProperty*	addProperty(int(* get)(), string label);
	
	template <typename GetClass>
	ofxKeyboardControlIntProperty<GetClass, GetClass, GetClass, GetClass, GetClass>*
								addProperty(GetClass* getObject, int(GetClass::*get)(),
											string label
											);
	
	template <typename GetClass, typename SetClass, typename MinClass, typename MaxClass, typename StepClass>
	ofxKeyboardControlIntProperty<GetClass, SetClass, MinClass, MaxClass, StepClass>*
								addProperty(GetClass* getObject, int(GetClass::*get)(),
											SetClass* setObject, void(SetClass::*set)(int value),
											string label, 
											MinClass* minObject, int(MinClass::*min)(),
											MaxClass* maxObject, int(MaxClass::*max)(),
											StepClass* stepObject, int(StepClass::*step)(),
											int defaultValue
											);*/
		
private:
	
	vector<ofxKeyboardBaseProperty*>	properties;
	ofxKeyboardBaseProperty*			curProperty;
	vector<ofxKeyboardBaseProperty*>::iterator curPropertyIterator;
	
	void				onAddProperty();
	
	void				renderFBO();
	ofFbo				fbo;
	
	ofxXmlSettings		settings;
	
	int					accessKey, lastProccessedKey;
	string				label;
	
	float				accessKeyStartTime, lastProccessedKeyTime;
	
	bool				isActive;	
};
///////////////////////////////////////////////////////////////////////////////////
// addProperty --------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////
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
	
	onAddProperty();
	
	return property;
}
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
	
	onAddProperty();
	
	return property;
}
// bool
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
	
	onAddProperty();
	
	return property;
}
// static
template <typename type>
ofxKeyboardStaticProperty<type>* ofxKeyboardSettings::addProperty(type(*get)(), string label){
	ofxKeyboardStaticProperty<type>* property;
	property = new ofxKeyboardStaticProperty<type>();
	property->allowControl = false;
	property->get = get;
	
	properties.push_back(property);	
	
	onAddProperty();
}
// control
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
	
	onAddProperty();
}
/*
template <typename GetClass>
ofxKeyboardControlIntProperty<GetClass, GetClass, GetClass, GetClass, GetClass>*
ofxKeyboardSettings::addProperty(GetClass* getObject, int(GetClass::*get)(),
								 string label
								 ){
	ofxKeyboardControlIntProperty<GetClass, GetClass, GetClass, GetClass, GetClass>* property;
	property = new ofxKeyboardControlIntProperty<GetClass, GetClass, GetClass, GetClass, GetClass>();
	property->allowControl = false;
	property->getObject = getObject;
	property->get = get;
	
	properties.push_back(property);	
	
	onAddProperty();
}	
template <typename GetClass, typename SetClass, typename MinClass, typename MaxClass, typename StepClass>
ofxKeyboardControlIntProperty<GetClass, SetClass, MinClass, MaxClass, StepClass>*
ofxKeyboardSettings::addProperty(GetClass* getObject, int(GetClass::*get)(),
								 SetClass* setObject, void(SetClass::*set)(int value),
								 string label, 
								 MinClass* minObject, int(MinClass::*min)(),
								 MaxClass* maxObject, int(MaxClass::*max)(),
								 StepClass* stepObject, int(StepClass::*step)(),
								 int defaultValue
								 ){
	ofxKeyboardControlIntProperty<GetClass, SetClass, MinClass, MaxClass, StepClass>* property;
	property = new ofxKeyboardControlIntProperty<GetClass, SetClass, MinClass, MaxClass, StepClass>();
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
	
	onAddProperty();
}	
*/
#endif