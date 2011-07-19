/*
 *  ofxKeyboardSettings.h
 *  ColorsOfMovement
 *
 *  Created by Paulo Barcelos on 7/19/11.
 *  Copyright 2011 paulobarcelos.com. All rights reserved.
 *
 */

#ifndef _OFX_KEYBOARD_SETTINGS
#define _OFX_KEYBOARD_SETTINGS

#define KEYBOARD_SETTINGS_HOLD_TIME 4.f
#define KEYBOARD_SETTINGS_IDLE_TIME 1.f

#include "ofMain.h"
#include "ofxXmlSettings.h"

////////////////////////////////////////////////////////////
// CLASS DEFINITION ----------------------------------------
////////////////////////////////////////////////////////////
class ofxKeyboardSettings {
	
public:
	
	void				setup(int accessKey, string filename);
	void				proccessKey(int key);
	void				saveSettings();
	void				loadSettings();
	
private:
	
	ofxXmlSettings		settings;
	
	int					accessKey, lastProccessedKey;
	string				filename;
	
	float				accessKeyStartTime, lastProccessedKeyTime;
	
	bool				isActive;	
};
#endif