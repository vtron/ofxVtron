/*
 *  uiObserver.cpp
 *  openFrameworks
 *
 *  Created by StephenVarga on 10/6/09.
 *  Copyright 2009. All rights reserved.
 *
 */

#include "uiObserver.h"

uiObserver* uiObserver::pUIObserver=NULL;

//------------------------------------------------------------------
//Singgleton Stuff
//------------------------------------------------------------------
uiObserver::uiObserver() {
	mouseX=0;
	mouseY=0;
	curElementID=0;
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
void uiObserver::addElement(uiElement * e) {
	e->elementID=curElementID;
	elements.push_back(e);
	
	curElementID+=1;
}


//------------------------------------------------------------------
void uiObserver::removeElement(uiElement * e) {
	for(int i=0;i<elements.size();i++)  {
		if(elements[i]->elementID==e->elementID) {
			elements.erase(elements.begin()+i);
			return;
		}
	}
}


//------------------------------------------------------------------
void uiObserver::addEventListeners() {
	//ofAddListener(ofEvents.setup,this,&uiObserver::setup);
	ofAddListener(ofEvents.update,this,&uiObserver::update);
	//ofAddListener(ofEvents.draw,this,&uiObserver::draw);
	
	ofAddListener(ofEvents.keyPressed,this,&uiObserver::keyPressed);
	ofAddListener(ofEvents.keyReleased,this,&uiObserver::keyReleased);
	
	ofAddListener(ofEvents.mouseMoved,this,&uiObserver::mouseMoved);
	ofAddListener(ofEvents.mousePressed,this,&uiObserver::mousePressed);
	ofAddListener(ofEvents.mouseDragged,this,&uiObserver::mouseDragged);
	ofAddListener(ofEvents.mouseReleased,this,&uiObserver::mouseReleased);
}

/*
//------------------------------------------------------------------
void uiObserver::setup(ofEventArgs &args) {
	for(int i=0;i<elements.size();i++) {
		if(elements[i]->bEnabled) elements[i]->setup();
	}
}
*/


//------------------------------------------------------------------
void uiObserver::update(ofEventArgs &args) {
	//This is a bit weird maybe could be implemented a bit better but it works
	//Reset Mouse Over vars on all elements
	for(int i=0;i<elements.size();i++) {
		elements[i]->resetMouseOver();
	}
	
	//Check for Mouse Over, if you hit one exit so you don't get multiple mouse overs!
	for(int i=0;i<elements.size();i++) {
		if(elements[i]->checkMouseOver(mouseX, mouseY)) return;
	}
}


/*
//------------------------------------------------------------------
void uiObserver::draw(ofEventArgs &args) {
	for(int i=0;i<elements.size();i++) {
		if(elements[i]->bEnabled) elements[i]->draw();
	}
}
*/

//------------------------------------------------------------------
void uiObserver::keyPressed(ofKeyEventArgs &args) {
	for(int i=0;i<elements.size();i++) {
		if(elements[i]->bEnabled) elements[i]->keyPressed(args.key);
	}
}

//------------------------------------------------------------------
void uiObserver::keyReleased(ofKeyEventArgs &args) {
	for(int i=0;i<elements.size();i++) {
		if(elements[i]->bEnabled) elements[i]->keyReleased(args.key);
	}
}


//------------------------------------------------------------------
void uiObserver::mouseMoved(ofMouseEventArgs & args) {
	for(int i=0;i<elements.size();i++) {
		mouseX=args.x;
		mouseY=args.y;
		if(elements[i]->bEnabled) elements[i]->mouseMoved(args.x, args.y);
	}
}

//------------------------------------------------------------------
void uiObserver::mousePressed(ofMouseEventArgs & args) {
	for(int i=0;i<elements.size();i++) {
		if(elements[i]->bEnabled)  elements[i]->mousePressed(args.x, args.y, args.button);
	}
}

//------------------------------------------------------------------
void uiObserver::mouseDragged(ofMouseEventArgs & args) {
	for(int i=0;i<elements.size();i++) {
		if(elements[i]->bEnabled) elements[i]->mouseDragged(args.x, args.y, args.button);
	}
}

//------------------------------------------------------------------
void uiObserver::mouseReleased(ofMouseEventArgs & args) {
	for(int i=0;i<elements.size();i++) {
		if(elements[i]->bEnabled) elements[i]->mouseReleased(args.x, args.y, args.button);
	}
}
