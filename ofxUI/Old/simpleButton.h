/*
 *  test.h
 *  openFrameworks
 *
 *  Created by StephenVarga on 10/6/09.
 *  Copyright 2009. All rights reserved.
 *
 */
#ifndef _simpleButton
#define _simpleButton

#include "uiObserver.h"
#include "ofMain.h"

class uiObserver;

class simpleButton {
	
	public:
		simpleButton();
		virtual ~simpleButton();
		uiObserver* ui;

		
		virtual void setup();
		virtual void update();
		virtual void draw();
		
		virtual void mouseMoved(int x, int y);
		virtual void mouseDragged(int x, int y, int button);
		virtual void mousePressed(int x, int y, int button);
		virtual void mouseReleased(int x, int y, int button);
	
		int x;
		int y;
		int width;
		int height;
		
		bool bDraw;
};

#endif