/*
 *  ofxVtronUtils.h
 *  openFrameworks
 *
 *  Created by StephenVarga on 11/2/09.
 *  Copyright 2009. All rights reserved.
 *
 */
#ifndef _ofxVtronUtils
#define _ofxVtronUtils

#define OF_BLEND_MODE_NORMAL	0
#define OF_BLEND_MODE_MULTIPLY	1
#define OF_BLEND_MODE_SCREEN	2
#define OF_BLEND_MODE_OVERLAY	3


#include "ofMain.h"


//--------------------------------------------------------------
//COLOR FUNCTIONS

static ofColor lerpColor(ofColor color1, ofColor color2, float pct) {
	ofColor returnColor;
	
	returnColor.r=ofLerp(color1.r, color2.r, pct);
	returnColor.g=ofLerp(color1.g, color2.g, pct);
	returnColor.b=ofLerp(color1.b, color2.b, pct);
	
	return returnColor;
}

//--------------------------------------------------------------
static ofColor hsbToRgb(float h, float s, float b) {
	
	
	//Based on formala from
	//http://ilab.usc.edu/wiki/index.php/HSV_And_H2SV_Color_Space#HSV_Transformation_C_.2F_C.2B.2B_Code_2
	
	//Init Color Object
	ofColor rgbColor;
	rgbColor.set(0,0,0);
	
	//Convert from 0-255 to HSV Ranges (0-360,0-100,0-255
	h=ofMap(h,0.0f,255.0f,0.0f,360.0f);
	s=ofMap(s,0.0f,255.0f,0.0f,100.0f);
	
	//Normalize Values, keep them in range
	if(h>360) h=360;
	if(s>100) s=100;
	if(b>255) b=255;
	
	//Convert Values
	if(b==0) {
		return rgbColor;
	} else if(s==0) {
		rgbColor.r=b;
		rgbColor.g=b;
		rgbColor.b=b;
		return rgbColor;
	}
	
	s=s/100; //Convert saturation to percent to work with formula
	const double hf = h / 60.0;
	const int    i  = (int) floor( hf );                              
	const double f  = hf - i;                                         
	const double pb  = b * ( 1 - s );                                 
	const double qb  = b * ( 1 - s * f );                             
	const double tb  = b * ( 1 - s * ( 1 - f ) );
	
	switch (i) { //Find out which hue is dominant, then convert other values
		case 0:
			rgbColor.r=b;
			rgbColor.g=tb;
			rgbColor.b=pb;
			break;
		case 1:
			rgbColor.r=qb;
			rgbColor.g=b;
			rgbColor.b=pb;
			break;
		case 2:
			rgbColor.r=pb;
			rgbColor.g=b;
			rgbColor.b=tb;
			break;
		case 3:
			rgbColor.r=pb;
			rgbColor.g=qb;
			rgbColor.b=b;
			break;
		case 4:
			rgbColor.r=tb;
			rgbColor.g=pb;
			rgbColor.b=b;
			break;
		case 5:
			rgbColor.r=b;
			rgbColor.g=pb;
			rgbColor.b=qb;
			break;
		case 6:
			rgbColor.r=b;
			rgbColor.g=tb;
			rgbColor.b=pb;
			break;
		case -1:
			rgbColor.r=b;
			rgbColor.g=pb;
			rgbColor.b=qb;
			break;
		default:
			break; 
	}
	
	return rgbColor;
}

//--------------------------------------------------------------
static void setBlendMode(int blendMode) {
	switch (blendMode) {
		case OF_BLEND_MODE_NORMAL:
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			break;
		case OF_BLEND_MODE_MULTIPLY:
			glBlendFunc(GL_ZERO, GL_SRC_COLOR);
			break;
		case OF_BLEND_MODE_SCREEN:
			glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
			break;
		case OF_BLEND_MODE_OVERLAY:
			glBlendFunc(GL_SRC_ALPHA,GL_ONE);
			break;
		default:
			break;
	}
}


//--------------------------------------------------------------
//String Functions

//--------------------------------------------------------------
static string ofxVtronToUpper(string s) {
	for(int i=0; i<s.length();i++) {
		s[i]=toupper(s[i]);
	}
	
	return s;
}

//------------------------------------------------------------------
//Animation Functions


//------------------------------------------------------------------
static void xenoToPoint(float &curAmount, float targetAmount, float spd){
	curAmount = spd * targetAmount + (1-spd) * curAmount; 
}






#endif
