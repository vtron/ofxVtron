/*
 *  radioButtonSet.cpp
 *  openFrameworks
 *
 *  Created by StephenVarga on 11/2/09.
 *  Copyright 2009. All rights reserved.
 *
 */

#include "radioButtonSet.h"

//------------------------------------------------------------------
radioButtonSet::radioButtonSet() {
}

//------------------------------------------------------------------
radioButtonSet::~radioButtonSet() {
}

//------------------------------------------------------------------
void radioButtonSet::addButton(radioButton &box) {
	radioButtons.push_back(&box);
}


//------------------------------------------------------------------
void radioButtonSet::removeButton(radioButton &box) {
	for(int i=0;i<radioButtons.size();i++) {
		if(radioButtons[i]->elementID==box.elementID) {
			radioButtons.erase(radioButtons.begin()+i);
			return;
		}
	}
}


//------------------------------------------------------------------
void radioButtonSet::resetBoxes() {
	for(int i=0; i<radioButtons.size(); i++) {
		radioButtons[i]->bChecked=false;
	}
}


//------------------------------------------------------------------
void radioButtonSet::mousePressed(int x, int y, int button) {
	for(int i=0;i<radioButtons.size(); i++) {
		if(radioButtons[i]->bMouseOver) {
			resetBoxes();
			radioButtons[i]->bChecked=true;
			return;
		}
	}
}

