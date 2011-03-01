/*
 *  ofxVtronImageTexture.cpp
 *  pennant
 *
 *  Created by Stephen Varga on 4/5/10.
 *  Copyright 2010. All rights reserved.
 *
 */

#include "ofxVtronImageTexture.h"


//------------------------------------------------------------------
void ofxVtronImageTexture::loadImage(string fileName) {
	ofImage * i = new ofImage();
	i->loadImage(fileName);
	i->setImageType(OF_IMAGE_COLOR_ALPHA);
	width = i->width;
	height = i->height;
	
	
	allocate(width, height, GL_RGBA);
	loadData(i->getPixels(), i->width, i->height, GL_RGBA);
	
	delete i;
}