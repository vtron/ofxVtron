/*
 *  ofxAdvancedImage.cpp
 *  openFrameworks
 *
 *  Created by StephenVarga on 9/30/09.
 *  Copyright 2009. All rights reserved.
 *
 */

#include "ofxAdvancedImage.h"




//------------------------------------------------------------------
ofxAdvancedImage::ofxAdvancedImage() {

}



//------------------------------------------------------------------------------------------------
//PUBLIC METHODS
//------------------------------------------------------------------------------------------------

//------------------------------------------------------------------
void ofxAdvancedImage::loadFromUrl(string _url) {
	//Get the data
	string str=loader.loadFromUrl(_url);
	
	int bytesToRead = str.size();
	
	unsigned char buff[bytesToRead];
	memset(buff, 0, bytesToRead);
	
	for(int i = 0; i < bytesToRead; i++){
		buff[i] = str[i];
	}
	
	//Load the Image from the Stream
	loadFromData(buff, bytesToRead);
}



//------------------------------------------------------------------
void ofxAdvancedImage::loadFromUrl(string _url,string filename) {
	loader.loadFromUrl(_url, filename);
	
	loadImage(filename);
	
	reset();
}


//------------------------------------------------------------------
void ofxAdvancedImage::fade(int amount) {
	filters->fade(alteredPixels, width, height, amount, type);
	
	//Load the blurred image in
	if (myPixels.bAllocated == true && bUseTexture == true){
		tex.loadData(alteredPixels, myPixels.width, myPixels.height, myPixels.glDataType);
	}
}


//------------------------------------------------------------------
void ofxAdvancedImage::filter(int filterType, int radius) {
	//Apply the Filter
	switch (filterType) {
		case OF_FILTER_NORMAL:
			filters->blurNormal(alteredPixels,width,height, type);
			break;
		case OF_FILTER_MEDIAN:
			filters->median(alteredPixels,width,height,type);
			break;
		case OF_FILTER_EDGES:
			filters->edges(alteredPixels,width,height,type);
			break;
		case OF_FILTER_SHARPEN:
			filters->sharpen(alteredPixels,width,height,type);
			break;
		case OF_FILTER_GAUSSIAN:
			filters->gaussian(alteredPixels,width,height,type);
			break;
		case OF_FILTER_PREWITT:
			filters->prewitt(alteredPixels,width,height,type);
			break;
		case OF_FILTER_SUPER_FAST_BLUR:
			filters->superFastBlur(alteredPixels, width, height, radius, type);
			break;
		default:
			cout<<"Not a Filter Type!"<<endl;
	}
	
	
	//Load the blurred image in
	if (myPixels.bAllocated == true && bUseTexture == true){
		tex.loadData(alteredPixels, myPixels.width, myPixels.height, myPixels.glDataType);
	}
}



//------------------------------------------------------------------
//Masking Functions


//------------------------------------------------------------------
void ofxAdvancedImage::mask(ofImage maskImage) {
}

//------------------------------------------------------------------
void ofxAdvancedImage::mask(unsigned char * pixels, int w, int h) {
	if(w*h <=width*height) {
		//Make sure its an alpha image, this will kill effects so mask the image first!
		//setImageType(OF_IMAGE_COLOR_ALPHA);
		
		for(int i=0; i<width*height;i++) {
			int thisAlphaPixel=(i*4)+3;
			alteredPixels[thisAlphaPixel] = pixels[i];
		}
		
		//Load the blurred image in
		if (myPixels.bAllocated == true && bUseTexture == true){
			tex.loadData(alteredPixels, myPixels.width, myPixels.height, myPixels.glDataType);
		}
	} else {
		cout<< "Image is too big to be a mask!" <<endl;
	}
}


//------------------------------------------------------------------
//Wrappers for ofImage Functions

//------------------------------------------------------------------
void ofxAdvancedImage::loadImage(string filename) {
	ofImage::loadImage(filename);
	reset();
}

//------------------------------------------------------------------
void ofxAdvancedImage::setFromPixels(unsigned char * pixels, int w, int h, int newType, bool bOrderIsRGB) {
	ofImage::setFromPixels(pixels, w, h, newType, bOrderIsRGB);
	reset();
}


//------------------------------------------------------------------
void ofxAdvancedImage::resize(int newWidth, int newHeight) {
	ofImage::resize(newWidth, newHeight);
	reset();
}


//------------------------------------------------------------------
unsigned char * ofxAdvancedImage::getPixels() {
	return alteredPixels;
}


//------------------------------------------------------------------
void ofxAdvancedImage::grabScreen(int x, int y, int w, int h) {
	ofImage::grabScreen(x, y, w, h);
	
	reset();
}

//------------------------------------------------------------------
void ofxAdvancedImage::setImageType(int type) {
	ofImage::setImageType(type);
	reset();
}



//------------------------------------------------------------------------------------------------
//PRIVATE METHODS
//------------------------------------------------------------------------------------------------


//------------------------------------------------------------------
void ofxAdvancedImage::loadFromData(unsigned char * datasource, int len) {
	//Load image from memory code, based on Zach's code updated by madparker on OF Forum
	//if we already have a loaded image clear it
	// if(isValid()){
    clear();     
	// }
	
	//create a freeimage memory handle from the buffer address
	FIMEMORY *hmem = NULL;
	hmem = FreeImage_OpenMemory((BYTE *)datasource,len);
	if (hmem == NULL){ printf("couldn't create memory handle! \n"); return; }
	
	//get the file type!
	FREE_IMAGE_FORMAT fif = FreeImage_GetFileTypeFromMemory(hmem);
	
	//make the image!!
	putBmpIntoPixels(FreeImage_LoadFromMemory(fif, hmem, 0), myPixels);
	//  bmp = FreeImage_LoadFromMemory(fif, hmem, 0);
	
	//free our memory
	FreeImage_CloseMemory(hmem);
	
	if (getBmpFromPixels(myPixels) == NULL){ printf("couldn't create bmp! \n"); return; }
	
	//flip it!
	FreeImage_FlipVertical(getBmpFromPixels(myPixels));
	
	if (myPixels.bAllocated == true && bUseTexture == true){
		tex.allocate(myPixels.width, myPixels.height, myPixels.glDataType);
	}   
	
	swapRgb(myPixels);
	
	update();
	
	reset();
}



//------------------------------------------------------------------
void ofxAdvancedImage::reset() {
	//Load the basic image without any blurring or effects
	//if (myPixels.bAllocated == true && bUseTexture == true) {
		int numPixels=0;
		
		switch(type) {
			case OF_IMAGE_GRAYSCALE:
				numPixels=width*height;
				alteredPixels = new unsigned char [numPixels];
				filters->allocate(width, height, type);
				break;
			case OF_IMAGE_COLOR:
				numPixels=width*height*3;
				alteredPixels = new unsigned char [numPixels];
				filters->allocate(width, height, type);
				break;
			case OF_IMAGE_COLOR_ALPHA:
				numPixels=width*height*4;
				alteredPixels = new unsigned char [numPixels];
				filters->allocate(width, height, type);
				break;
		}
		
		for(int i=0; i<numPixels; i++) {
			alteredPixels[i] = myPixels.pixels[i];
		}
	
		tex.loadData(alteredPixels, myPixels.width, myPixels.height, myPixels.glDataType);
	//}
}
