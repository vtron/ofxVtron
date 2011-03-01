/*
 *  simpleButton.cpp
 *  openFrameworks
 *
 *  Created by StephenVarga on 10/6/09.
 *  Copyright 2009. All rights reserved.
 *
 */
#include "simpleButton.h"

//--------------------------------------------------------------
//Constructors

//Main Constructor
simpleButton::simpleButton() {
	x=0;
	y=0;
	width=0;
	height=0;
	
	bMouseOver=false;
	bMousePressed=false;
}

//Destructor
simpleButton::~simpleButton() {
	disable();
}


//--------------------------------------------------------------
void simpleButton::setPos(int _x, int _y) {
	x=_x;
	y=_y;
}


//--------------------------------------------------------------
void simpleButton::setWidth(int _w) {
	width=_w;
}


//--------------------------------------------------------------
void simpleButton::setHeight(int _h) {
	height=_h;
}

//--------------------------------------------------------------
void simpleButton::setSize(int _w, int _h) {
	width=_w;
	height=_h;
}


//--------------------------------------------------------------
void simpleButton::resetMouseOver() {
	bMouseOver=false;
}

//--------------------------------------------------------------
bool simpleButton::checkMouseOver(int _x, int _y) {
	if(_x>x && _y>y && _x< x+width && _y< y+height) {
		bMouseOver=true;
	}
	
	return bMouseOver;
}

//--------------------------------------------------------------
void simpleButton::mouseMoved(int x, int y) {}
void simpleButton::mouseDragged(int x, int y, int button){}

//--------------------------------------------------------------
void simpleButton::mousePressed(int x, int y, int button){
	if(bMouseOver) {
		bMousePressed=true;
	} else {
		bMousePressed=false;
	}
}

//--------------------------------------------------------------
void simpleButton::mouseReleased(int x, int y, int button){
	bMousePressed=false;
}