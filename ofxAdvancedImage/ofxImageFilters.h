/*
 *  ofxImageFilters.h
 *  openFrameworks
 *
 *  Created by StephenVarga on 10/4/09.
 *  Copyright 2009. All rights reserved.
 *
 */
#ifndef _ofxImageFilters
#define _ofxImageFilters

//Constants
#define 	OF_FILTER_NORMAL			0x00
#define 	OF_FILTER_MEDIAN			0x01
#define 	OF_FILTER_EDGES				0x03
#define 	OF_FILTER_SHARPEN			0x04
#define 	OF_FILTER_GAUSSIAN			0x05
#define 	OF_FILTER_PREWITT			0x06
#define 	OF_FILTER_SUPER_FAST_BLUR	0x07

#include "ofMain.h"


class ofxImageFilters {
	
	public:
		ofxImageFilters(int w, int h, int imageType);
		~ofxImageFilters();
		
		void allocate(int w, int h, int imageType);
	
		//unsigned char * pixelsBlurred;
	
		int imageSize;
	
		//------------------------------------------------------------------
		//Fade Image
		void fade(unsigned char * _pixels, int w, int h, int amount, int imageType);
		
		//------------------------------------------------------------------
		//Filters
		void blurNormal(unsigned char * _pixels, int w, int h, int imageType);
		void median(unsigned char * _pixels, int w, int h, int imageType);
		void edges(unsigned char * _pixels, int w, int h, int imageType);
		void sharpen(unsigned char * _pixels, int w, int h, int imageType);
		void gaussian(unsigned char * _pixels, int w, int h, int imageType);
		void prewitt(unsigned char * _pixels, int w, int h, int imageType);
	
		//------------------------------------------------------------------
		//Superfast blur adapted by Mehmet Akten from Mario Klingemann
		void superFastBlur(unsigned char *pix, int w, int h, int radius, int imageType);
	
		
	private:
		//------------------------------------------------------------------
		//Fade
		void fadeGrayscale(unsigned char * _pixels, int w, int h,int amount);
		void fadeRGB(unsigned char * _pixels, int w, int h,int amount);
		void fadeRGBA(unsigned char * _pixels, int w, int h,int amount);

	
		//------------------------------------------------------------------
		//Normal Blur
		void blurNormalGrayscale(unsigned char * _pixels, int w, int h);
		void blurNormalRGB(unsigned char * _pixels, int w, int h);
		void blurNormalRGBAlpha(unsigned char * _pixels, int w, int h);

		//------------------------------------------------------------------
		//Median Blur
		void medianGrayscale(unsigned char * _pixels, int w, int h);
		void medianRGB(unsigned char * _pixels, int w, int h);
		void medianRGBAlpha(unsigned char * _pixels, int w, int h);
	
	
		//------------------------------------------------------------------
		//Super Fast Blur
		void superFastBlurGrayscale(unsigned char *pix, int w, int h, int radius);
		void superFastBlurRGB(unsigned char *pix, int w, int h, int radius);
		void superFastBlurRGBAlpha(unsigned char *pix, int w, int h, int radius);
	
		//------------------------------------------------------------------
		//Kernel Filters
		void kernelBlurGrayscale(unsigned char * _pixels, int _kernel[9], int _kernelWeight, int w, int h);
		void kernelBlurRGB(unsigned char * _pixels, int _kernel[9], int _kernelWeight, int w, int h);
		void kernelBlurRGBAlpha(unsigned char * _pixels, int _kernel[9], int _kernelWeight, int w, int h);
	
	
};

#endif
