/*
 *  checkBox.cpp
 *  openFrameworks
 *
 *  Created by StephenVarga on 11/2/09.
 *  Copyright 2009. All rights reserved.
 *
 */

#include "simpleButton.h"
#include "checkBox.h"

checkBox::checkBox() {
	bChecked=false;
}

void checkBox::mousePressed(int x, int y, int button) {
	if(bMouseOver) {
		bChecked=true;
	}
}


void checkBox::mouseReleased(int x, int y, int button) {
}

