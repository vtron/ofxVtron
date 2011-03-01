/*
 *  ofxVtronImageTexture.h
 *  pennant
 *
 *  Created by Stephen Varga on 4/5/10.
 *  Copyright 2010. All rights reserved.
 *
 */
#ifndef _ofxVtronImageTexture
#define _ofxVtronImageTexture


#include "ofMain.h"
#include "ofTexture.h"


class ofxVtronImageTexture : public ofTexture {
	
	public:
		void loadImage(string fileName);
		int width,height;
		
};

#endif
