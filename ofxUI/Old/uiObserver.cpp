/*
 *  uiObserver.cpp
 *  openFrameworks
 *
 *  Created by StephenVarga on 10/6/09.
 *  Copyright 2009. All rights reserved.
 *
 */

class simpleButton;
#include "uiObserver.h"

uiObserver* uiObserver::pUIObserver=NULL;

//------------------------------------------------------------------
//Singgleton Stuff
//------------------------------------------------------------------
uiObserver::uiObserver() {
	addEventListeners();
}


//------------------------------------------------------------------
uiObserver* uiObserver::getInstance() {
	if(pUIObserver==NULL) {
		pUIObserver=new uiObserver();
	}
	return pUIObserver;
}


//------------------------------------------------------------------
void uiObserver::addButton(simpleButton * b) {
	buttons.push_back(b);
}

//------------------------------------------------------------------
void uiObserver::addEventListeners() {
	ofAddListener(ofEvents.setup,this,&uiObserver::setup);
	ofAddListener(ofEvents.update,this,&uiObserver::update);
	ofAddListener(ofEvents.draw,this,&uiObserver::draw);
	
	ofAddListener(ofEvents.mouseMoved,this,&uiObserver::mouseMoved);
	ofAddListener(ofEvents.mousePressed,this,&uiObserver::mousePressed);
	ofAddListener(ofEvents.mouseDragged,this,&uiObserver::mouseDragged);
	ofAddListener(ofEvents.mouseReleased,this,&uiObserver::mouseReleased);
}


//------------------------------------------------------------------
void uiObserver::setup(ofEventArgs &args) {
	for(int i=0;i<buttons.size();i++) {
		buttons[i]->setup();
	}
}



//------------------------------------------------------------------
void uiObserver::update(ofEventArgs &args) {
	for(int i=0;i<buttons.size();i++) {
		buttons[i]->update();
	}
}



//------------------------------------------------------------------
void uiObserver::draw(ofEventArgs &args) {
	for(int i=0;i<buttons.size();i++) {
		buttons[i]->draw();
	}
}


//------------------------------------------------------------------
void uiObserver::mouseMoved(ofMouseEventArgs & args) {
	for(int i=0;i<buttons.size();i++) {
		buttons[i]->mouseMoved(args.x, args.y);
	}
}

//------------------------------------------------------------------
void uiObserver::mousePressed(ofMouseEventArgs & args) {
	for(int i=0;i<buttons.size();i++) {
		buttons[i]->mousePressed(args.x, args.y, args.button);
	}
}

//------------------------------------------------------------------
void uiObserver::mouseDragged(ofMouseEventArgs & args) {
	for(int i=0;i<buttons.size();i++) {
		buttons[i]->mouseDragged(args.x, args.y, args.button);
	}
}

//------------------------------------------------------------------
void uiObserver::mouseReleased(ofMouseEventArgs & args) {
	for(int i=0;i<buttons.size();i++) {
		buttons[i]->mouseReleased(args.x, args.y, args.button);
	}
}
