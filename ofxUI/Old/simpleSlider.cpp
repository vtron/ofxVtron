/*
 *  simpleSlider.cpp
 *  openFrameworks
 *
 *  Created by StephenVarga on 9/11/09.
 *  Copyright 2009. All rights reserved.
 *
 */

#include "simpleSlider.h"


//------------------------------------------------------------------
void simpleSlider::setup() {
	addEventListeners();
	
	//Setup Slider
	sliderSize=20;
	trackWidth=200;
	bIsBeingDragged=false;
}



//------------------------------------------------------------------
void simpleSlider::update(ofEventArgs &args) {
	pct=sliderPos.x/(trackWidth-sliderSize);
}



//------------------------------------------------------------------
void simpleSlider::draw(ofEventArgs &args) {
	//Draw Track
	ofSetColor(0, 0, 0);
	ofRect(pos.x-1, pos.y-1, trackWidth+2, sliderSize+2);
	
	ofSetColor(200, 200, 200);
	ofRect(pos.x, pos.y, trackWidth, sliderSize);
	
	//Draw Slider
	ofSetColor(0,0,0);
	ofRect(pos.x+sliderPos.x, pos.y, sliderSize, sliderSize);
	
	//Draw Label
	if(label!="") {
		ofSetColor(0, 0, 0);
		ofDrawBitmapString(label, pos.x, pos.y-10);
	}
}



//------------------------------------------------------------------
void simpleSlider::moveSlider(int _x) {
	if(_x>=pos.x&&_x<=pos.x+trackWidth-sliderSize) {
		sliderPos.x=_x-pos.x;
	}
}


//------------------------------------------------------------------
void simpleSlider::setPos(int _x, int _y) {
	pos.set(_x,_y);
}



//------------------------------------------------------------------
//------------------------------------------------------------------
//Event Listeners

void simpleSlider::addEventListeners() {
	ofAddListener(ofEvents.update,this,&simpleSlider::update);
	ofAddListener(ofEvents.draw,this,&simpleSlider::draw);
	ofAddListener(ofEvents.mousePressed,this,&simpleSlider::mousePressed);
	ofAddListener(ofEvents.mouseDragged,this,&simpleSlider::mouseDragged);
	ofAddListener(ofEvents.mouseReleased,this,&simpleSlider::mouseReleased);
}


//------------------------------------------------------------------
void simpleSlider::mousePressed(ofMouseEventArgs & _args) {
	int sliderLoc=sliderPos.x+pos.x;
	
	if(_args.x>sliderLoc&&_args.x<sliderLoc+sliderSize&&_args.y>pos.y&&_args.y<pos.y+sliderSize) {
		bIsBeingDragged=true;
		
		moveSlider(_args.x);
	}
}



//------------------------------------------------------------------
void simpleSlider::mouseDragged(ofMouseEventArgs & _args) {
	if(bIsBeingDragged) {
		moveSlider(_args.x);
	}
}



//------------------------------------------------------------------
void simpleSlider::mouseReleased(ofMouseEventArgs & _args) {
	bIsBeingDragged=false;
}








