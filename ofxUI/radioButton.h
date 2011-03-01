/*
 *  radioButton.h
 *  openFrameworks
 *
 *  Created by StephenVarga on 12/9/09.
 *  Copyright 2009. All rights reserved.
 *
 */
#ifndef _radioButton
#define _radioButton

#include "simpleButton.h"
#include "ofMain.h"


class radioButton : public simpleButton {
	
	public:
		radioButton();
	
		bool bChecked;
};

#endif
