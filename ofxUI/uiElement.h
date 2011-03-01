/*
 *  uiElement.h
 *  openFrameworks
 *
 *  Created by StephenVarga on 10/10/09.
 *  Copyright 2009. All rights reserved.
 *
 */
#ifndef _uiElement
#define _uiElement

#include "uiObserver.h"
#include "ofMain.h"

class uiObserver;

class uiElement {
	public:
		uiElement();
		uiElement(const uiElement& u);
		virtual ~uiElement();
	
		uiObserver* ui;
	
		//virtual void setup();
		//virtual void update();
		//virtual void draw();
		
		virtual void keyPressed(int key);
		virtual void keyReleased(int key);
		
		virtual void resetMouseOver();
		virtual bool checkMouseOver(int x, int y);
	
		virtual void mouseMoved(int x, int y);
		virtual void mouseDragged(int x, int y, int button);
		virtual void mousePressed(int x, int y, int button);
		virtual void mouseReleased(int x, int y, int button);
	
		void enable();
		void disable();
	
		bool bEnabled;
	
		int elementID;
};

#endif
