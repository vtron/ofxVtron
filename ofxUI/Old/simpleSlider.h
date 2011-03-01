/*
 *  simplesimpleSlider.h
 *  openFrameworks
 *
 *  Created by StephenVarga on 9/11/09.
 *  Copyright 2009. All rights reserved.
 *
 */
#ifndef _SIMPLE_SLIDER
#define _SIMPLE_SLIDER


#include "ofMain.h"
#include "ofEvents.h"


class simpleSlider {
	public:
		void setup();
		void update(ofEventArgs &args);
		void draw(ofEventArgs &args);
	
		void mousePressed(ofMouseEventArgs & _args);
		void mouseDragged(ofMouseEventArgs & _args);
		void mouseReleased(ofMouseEventArgs & _args);
		void addEventListeners();
	
		void setPos(int _x,int _y);
		void moveSlider(int _x);
	
		
		int trackWidth;
		int sliderSize;
		int w;
		int h;
	
		bool bIsBeingDragged;
	
		float pct;
	
		string label;
	
	private:
		ofPoint pos;
		ofPoint sliderPos;
	
};

#endif
