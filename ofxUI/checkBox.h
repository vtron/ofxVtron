/*
 *  checkBox.h
 *  openFrameworks
 *
 *  Created by StephenVarga on 11/2/09.
 *  Copyright 2009. All rights reserved.
 *
 */
#ifndef _checkBox
#define _checkBox

#include "simpleButton.h"
#include "ofMain.h"


class checkBox : public simpleButton {
	
	public:
		checkBox();
	
		virtual void mousePressed(int x, int y, int button);
		virtual void mouseReleased(int x, int y, int button);
	
		bool bChecked;
};

#endif
