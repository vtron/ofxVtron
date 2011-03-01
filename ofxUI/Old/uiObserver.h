/*
 *  uiObserver.h
 *  openFrameworks
 *
 *  Created by StephenVarga on 10/6/09.
 *  Copyright 2009. All rights reserved.
 *
 */
#ifndef _uiObserver
#define _uiObserver

#include "simpleButton.h"
#include "ofMain.h"

class simpleButton;

class uiObserver {
	public:
		static uiObserver* getInstance();
	
		void addButton(simpleButton * b);
	
		
	private:
		uiObserver();
	
		static uiObserver* pUIObserver;
		
		void setup(ofEventArgs &args);
		void update(ofEventArgs &args);
		void draw(ofEventArgs &args);
		
		void mouseMoved(ofMouseEventArgs & args);
		void mousePressed(ofMouseEventArgs & args);
		void mouseDragged(ofMouseEventArgs & args);
		void mouseReleased(ofMouseEventArgs & args);
	
	
		void addEventListeners();
	
		vector < simpleButton *> buttons;
	
		bool bHasStarted;
		
		//~uiObserver();
};

#endif
