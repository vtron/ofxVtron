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

#include "uiElement.h"
#include "ofMain.h"

class uiElement;

class uiObserver {
	public:
		static uiObserver* getInstance();
	
		void addElement(uiElement * e);
		void removeElement(uiElement * e);
	
		
	private:
		uiObserver();
	
		static uiObserver* pUIObserver;
		
		void setup(ofEventArgs &args);
		void update(ofEventArgs &args);
		void draw(ofEventArgs &args);
	
		void keyPressed(ofKeyEventArgs &args);
		void keyReleased(ofKeyEventArgs &args);
	
		void mouseMoved(ofMouseEventArgs &args);
		void mousePressed(ofMouseEventArgs &args);
		void mouseDragged(ofMouseEventArgs &args);
		void mouseReleased(ofMouseEventArgs &args);
	
		void addEventListeners();
	
		int curElementID;
		vector <uiElement *> elements;
	
		int mouseX;
		int mouseY;
		
		~uiObserver();
};

#endif
