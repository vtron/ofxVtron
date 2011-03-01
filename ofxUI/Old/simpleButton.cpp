/*
 *  simpleButton.cpp
 *  openFrameworks
 *
 *  Created by StephenVarga on 10/6/09.
 *  Copyright 2009. All rights reserved.
 *
 */
#include "simpleButton.h"
class uiObserver;


simpleButton::simpleButton() {
	ui=uiObserver::getInstance();
	ui->addButton(this);
	
	x=0;
	y=0;
	width=0;
	height=0;
	bDraw=false;
}

simpleButton::~simpleButton() {
	//delete ui;
}


//--------------------------------------------------------------
void simpleButton::setup() {
	
}

//--------------------------------------------------------------
void simpleButton::update(){
	
}

//--------------------------------------------------------------
void simpleButton::draw() {
	
}

//--------------------------------------------------------------
void simpleButton::mouseMoved(int x, int y ) {
	
}

//--------------------------------------------------------------
void simpleButton::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void simpleButton::mousePressed(int x, int y, int button){
	
}

//--------------------------------------------------------------
void simpleButton::mouseReleased(int x, int y, int button){
	
}