/*
 *  ofxColorTracker.h
 *  openFrameworks
 *
 *  Created by StephenVarga on 11/11/09.
 *  Copyright 2009. All rights reserved.
 *
 */
#ifndef _ofxColorTracker
#define _ofxColorTracker

#include "ofxOpenCv.h"
#include "ofMain.h"

typedef struct {
	int h;
	int s;
	int v;
} hsvColor;

class ofxColorTracker {
	
	public:
	ofxColorTracker();
		void setup(int _x, int _y, int _w, int _h);
		void update(unsigned char * pixels);
		void draw();
	
		void setColor(ofColor _color);
		hsvColor convertRgbToHsv(ofColor rgbColor);

		int x;
		int y;
	
		int w;
		int h;
	
		ofxCvColorImage			colorCvImage;
		ofxCvColorImage			colorHSVCvImage;
	
		ofxCvGrayscaleImage		grayscaleHueImage;
		ofxCvGrayscaleImage		grayscaleSatImage;
		ofxCvGrayscaleImage		grayscaleBriImage;
	
		ofxCvGrayscaleImage		grayscaleCvImage;
		
		unsigned char * colorHsvPixels;
	
		unsigned char * huePixels;
		unsigned char * satPixels;
		unsigned char * briPixels;
		unsigned char * grayPixels;
	
		//------------------------------------------------------------------
		//Tracking Data
		int hue, sat, val;
		
		int hueRange;
		int satRange;
		int valRange;
	
		ofxCvContourFinder contourFinder;
};

#endif
