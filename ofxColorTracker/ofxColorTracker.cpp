/*
 *  ofxColorTracker.cpp
 *  openFrameworks
 *
 *  Created by StephenVarga on 11/11/09.
 *  Copyright 2009. All rights reserved.
 *
 */

#include "ofxColorTracker.h"

ofxColorTracker::ofxColorTracker() {
	hue=0;
	sat=0;
	val=0;
	
	hueRange = 20;
	satRange = 30;
	valRange = 25;
}

//------------------------------------------------------------------
void ofxColorTracker::setup(int _x, int _y, int _w, int _h) {
	x=_x;
	y=_y;
	w=_w;
	h=_h;
	
	colorHsvPixels=new unsigned char [w*h*3];
	huePixels=new unsigned char [w*h];
	satPixels=new unsigned char [w*h];
	briPixels=new unsigned char [w*h];
	grayPixels=new unsigned char [w*h];
	
	colorCvImage.allocate(w, h);
	colorHSVCvImage.allocate(w, h);
	
	grayscaleHueImage.allocate(w, h);
	grayscaleSatImage.allocate(w, h);
	grayscaleBriImage.allocate(w, h);
	
	grayscaleCvImage.allocate(w, h);
}




//------------------------------------------------------------------
void ofxColorTracker::update(unsigned char * pixels) {
	colorCvImage.setFromPixels(pixels, w, h);
	colorCvImage.mirror(false, true);
	colorHSVCvImage = colorCvImage;
	colorHSVCvImage.convertRgbToHsv();
	
	for (int i = 0; i < w*h; i++){
		huePixels[i]=colorHsvPixels[i*3];
		satPixels[i]=colorHsvPixels[i*3+1];
		briPixels[i]=colorHsvPixels[i*3+2];
	}
	
	grayscaleHueImage.setFromPixels(huePixels,w,h);
	grayscaleSatImage.setFromPixels(satPixels,w,h);
	grayscaleBriImage.setFromPixels(briPixels,w,h);
	
	colorHSVCvImage.convertToGrayscalePlanarImages(grayscaleHueImage, grayscaleSatImage, grayscaleBriImage);
	
	colorHsvPixels = colorHSVCvImage.getPixels();

	for (int i = 0; i < w*h; i++){
		if ((colorHsvPixels[i*3] > (hue - hueRange) && colorHsvPixels[i*3] < (hue + hueRange)) &&
			(colorHsvPixels[i*3+1] > (sat - satRange) && colorHsvPixels[i*3+1] < (sat + satRange)) &&
			(colorHsvPixels[i*3+2] > (val - valRange) && colorHsvPixels[i*3+2] < (val + valRange))) {
			
			grayPixels[i] = 255;
		} else {
			grayPixels[i] = 0;
		}
	}
	
	grayscaleCvImage.setFromPixels(grayPixels, w, h);
	
	contourFinder.findContours(grayscaleCvImage, 20, 2000, 10, false, true);
}


//------------------------------------------------------------------
void ofxColorTracker::draw() {
	ofFill();
	ofSetColor(255, 255, 255);
	ofRect(x-5,y-5, w+10,h+10);
	grayscaleCvImage.draw(x,y);
	
	for(int i=0;i<contourFinder.blobs.size();i++) {
		int _x=contourFinder.blobs[i].centroid.x;
		int _y=contourFinder.blobs[i].centroid.y;
		int w=contourFinder.blobs[i].boundingRect.width;
		int h=contourFinder.blobs[i].boundingRect.height;
		ofSetRectMode(OF_RECTMODE_CENTER);
		ofNoFill();
		ofSetColor(255, 0, 0);
		ofRect(x+_x, y+_y, w,h);
		ofSetRectMode(OF_RECTMODE_CORNER);
	}
}


//------------------------------------------------------------------
void ofxColorTracker::setColor(ofColor _color) {
	hsvColor convertedColor=convertRgbToHsv(_color);
	hue=convertedColor.h;
	sat=convertedColor.s;
	val=convertedColor.v;
}

//------------------------------------------------------------------
hsvColor ofxColorTracker::convertRgbToHsv(ofColor rgbColor){
	
	const double &r = rgbColor.r / 255.0f;
    const double &g = rgbColor.g / 255.0f;
    const double &b = rgbColor.b / 255.0f;
	
    double max	 = (r > g) ? ((r > b) ? r : b) : ((g > b) ? g : b);
    double min	 = (r < g) ? ((r < b) ? r : b) : ((g < b) ? g : b);
    double range = max - min;
    double val	 = max;
    double sat   = 0;
    double hue   = 0;
    
    if (max != 0)   sat = range/max;
    
    if (sat != 0) 
    {
		double h;
		
		if      (r == max)	h =     (g - b) / range;
		else if (g == max)	h = 2 + (b - r) / range;
		else		h = 4 + (r - g) / range;
		
		hue = h/6.;
	    
		if (hue < 0.)
			hue += 1.0;
    }
	
	hsvColor newColor;
	newColor.h=hue*255.0f;
	newColor.s=sat*255.0f;
	newColor.v=val*255.0f;
	
    return newColor;
}

