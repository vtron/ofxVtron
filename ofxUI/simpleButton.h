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

#include "uiElement.h"
#include "ofMain.h"

class simpleButton : public uiElement {
	
	public:
		simpleButton();
		//simpleButton(const simpleButton& s);
		virtual ~simpleButton();
		
		void  resetMouseOver();
		bool checkMouseOver(int _x, int _y);
		
		virtual void mouseMoved(int x, int y);
		virtual void mouseDragged(int x, int y, int button);
		virtual void mousePressed(int x, int y, int button);
		virtual void mouseReleased(int x, int y, int button);
		
		void setPos(int _x, int _y);
		void setWidth(int w);
		void setHeight(int h);
		void setSize(int w, int h);
	
		float x;
		float y;
		float width;
		float height;
	
		bool bMouseOver;
		bool bMousePressed;
};

#endif