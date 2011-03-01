/*
 *  uiElement.cpp
 *  openFrameworks
 *
 *  Created by StephenVarga on 10/10/09.
 *  Copyright 2009. All rights reserved.
 *
 */

#include "uiElement.h"

class uiObserver;

//--------------------------------------------------------------
uiElement::uiElement() {
	bEnabled=true;
	
	ui=uiObserver::getInstance();
	enable();
}


//--------------------------------------------------------------
uiElement::uiElement(const uiElement& u) {
	bEnabled=true;
	ui=uiObserver::getInstance();
	
	enable();
}

//--------------------------------------------------------------
uiElement::~uiElement() {
	disable();
}

//--------------------------------------------------------------
void uiElement::enable() {
	bEnabled=true;
	ui->addElement(this);
}

void uiElement::disable() {
	bEnabled=false;
	ui->removeElement(this);
}

//void uiElement::setup() {}
//void uiElement::update(){}
//void uiElement::draw() {}
void uiElement::keyPressed(int key) {}
void uiElement::keyReleased(int key) {}
void uiElement:: resetMouseOver() {}

bool uiElement::checkMouseOver(int x, int y) {
	return false;
}

void uiElement::mouseMoved(int x, int y ) {}
void uiElement::mouseDragged(int x, int y, int button){}
void uiElement::mousePressed(int x, int y, int button){}
void uiElement::mouseReleased(int x, int y, int button){}