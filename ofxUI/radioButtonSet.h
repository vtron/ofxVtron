/*
 *  radioButtonSet.h
 *  openFrameworks
 *
 *  Created by StephenVarga on 11/2/09.
 *  Copyright 2009. All rights reserved.
 *
 */
#ifndef _radioButtonSet
#define _radioButtonSet

#include "uiElement.h"
#include "radioButton.h"
#include "ofMain.h"

class radioButtonSet : public uiElement{
	public:
		
		radioButtonSet();
		~radioButtonSet();
		void addButton(radioButton &box);
		void removeButton(radioButton &box);
	
		void resetBoxes();
	
		void mousePressed(int x, int y, int button);
	
		vector <radioButton *> radioButtons;
};

#endif
