/*
 *  advancedImage.h
 *  openFrameworks
 *
 *
 *  Created by StephenVarga on 9/30/09.
 *  Copyright 2009. All rights reserved.
 *
 */
#ifndef _ofxAdvancedImage
#define _ofxAdvancedImage

#include <iostream>
#include <fstream>

#include "Poco/Net/HTTPStreamFactory.h"
#include "Poco/URIStreamOpener.h"
#include "Poco/URI.h"
#include "Poco/StreamCopier.h"

#include "ofxImageFilters.h"




using Poco::Net::HTTPStreamFactory;
using Poco::URIStreamOpener;
using Poco::URI;
using Poco::StreamCopier;


#include "ofxFileLoader.h"
#include "ofMain.h"




class ofxAdvancedImage : public ofImage {
	public:
		ofxAdvancedImage();
		
		//------------------------------------------------------------------
		//Load Image from URL/Web
		void			loadFromUrl(string _url); //Loads from Memory
		void			loadFromUrl(string _url,string filename); //Saves image then loads it in
		
	
		//------------------------------------------------------------------
		//Filter Functions
		ofxImageFilters *filters;
	
		void			filter(int type=OF_FILTER_SUPER_FAST_BLUR, int radius=1);
		void			fade(int amount);
	
		//------------------------------------------------------------------
		//Masking Functions
		void			mask(ofImage maskImage);
		void			mask(unsigned char * pixels,int w, int h);
	
	
		//------------------------------------------------------------------
		//Wrappers to use altered pixels with image loading functions
		void			loadImage(string filename);
		void 			setFromPixels(unsigned char * pixels, int w, int h, int newType, bool bOrderIsRGB = true);
		void			resize(int newWidth, int newHeight);
		unsigned char * getPixels();
		void			grabScreen(int x, int y, int w, int h);
		void			setImageType(int type);
	
	
		
		//------------------------------------------------------------------
		//Gets rid of all effects, sets image back to original
		void			reset();


	private:
		//------------------------------------------------------------------
		//Load Image from Memory
		void loadFromData(unsigned char * datasource, int len);
	
		
		//------------------------------------------------------------------
		//Holder for pixels currently being filtered
		unsigned char * alteredPixels;
	
		//------------------------------------------------------------------
		ofxFileLoader loader;

};

#endif
