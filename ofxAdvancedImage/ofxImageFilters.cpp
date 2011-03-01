/*
 *  ofxImageFilters.cpp
 *  openFrameworks
 *
 *  Created by StephenVarga on 10/4/09.
 *  Copyright 2009. All rights reserved.
 *
 */

#include "ofxImageFilters.h"

unsigned char* pixelsBlurred=NULL;

//------------------------------------------------------------------
// this is the "comparison" function
int compare( const void *a,  const void *b){			
	
	// the const void means -- this function doesn't have a "set" type -- 
	// it's very C style. 
	
	// we first (step1) convert a and b to float points
	// (step2) we dereference to get the values held by a and b
	// I know this is crazy !!  but it works and is fast !! 
	// for those in the know, STL vector < > has sorting functionality too
	// I will show you that at some point....
	// kk.
	
	float *aPtr = ( float *)a;	// (step1) casting pointer types to be of the type float *
	float *bPtr = ( float *)b;
	float valuea = *aPtr;		// (step2) get the value out of ptr a and b...
	float valueb = *bPtr;
	if (valuea > valueb) return 1;
	else return -1;
}







//------------------------------------------------------------------
ofxImageFilters::ofxImageFilters(int w, int h, int imageType) {
	imageSize=0;
	allocate(w,h,imageType);
	pixelsBlurred=NULL;
}

//------------------------------------------------------------------
ofxImageFilters::~ofxImageFilters() {
}


//------------------------------------------------------------------
void ofxImageFilters::allocate(int w, int h, int imageType) {
	
	int imageSize;
	
	switch(imageType) {
		case OF_IMAGE_GRAYSCALE:
			pixelsBlurred = new unsigned char[w*h];
			break;
		case OF_IMAGE_COLOR:
			pixelsBlurred = new unsigned char[w*h*3];
			break;
		case OF_IMAGE_COLOR_ALPHA:
			pixelsBlurred = new unsigned char[w*h*4];
			break;
	}
}

//------------------------------------------------------------------
void ofxImageFilters::fade(unsigned char * _pixels, int w, int h, int amount, int imageType) {
	int imageSize;
	
	switch(imageType) {
		case OF_IMAGE_GRAYSCALE:
			fadeGrayscale(_pixels, w, h, amount);
			break;
		case OF_IMAGE_COLOR:
			fadeRGB(_pixels, w, h, amount);
			break;
		case OF_IMAGE_COLOR_ALPHA:
			fadeRGBA(_pixels, w, h, amount);
			break;
	}
}


//------------------------------------------------------------------
void ofxImageFilters::superFastBlur(unsigned char * _pixels, int w, int h, int radius, int imageType) {
	
	int imageSize;
	
	switch(imageType) {
		case OF_IMAGE_GRAYSCALE:
			superFastBlurGrayscale(_pixels, w, h, radius);
			break;
		case OF_IMAGE_COLOR:
			superFastBlurRGB(_pixels, w, h, radius);
			break;
		case OF_IMAGE_COLOR_ALPHA:
			superFastBlurRGBAlpha(_pixels, w, h, radius);
			break;
	}
}



//------------------------------------------------------------------
void ofxImageFilters::blurNormal(unsigned char * _pixels, int w, int h, int imageType) {
	
	int imageSize;
	
	switch(imageType) {
		case OF_IMAGE_GRAYSCALE:
			blurNormalGrayscale(_pixels, w, h);
			break;
		case OF_IMAGE_COLOR:
			blurNormalRGB(_pixels, w, h);
			break;
		case OF_IMAGE_COLOR_ALPHA:
			blurNormalRGBAlpha(_pixels, w, h);
			break;
	}
}



//------------------------------------------------------------------
void ofxImageFilters::median(unsigned char * _pixels, int w, int h, int imageType) {
	
	int imageSize;
	
	switch(imageType) {
		case OF_IMAGE_GRAYSCALE:
			imageSize=w*h;
			medianGrayscale(_pixels, w, h);
			break;
		case OF_IMAGE_COLOR:
			medianRGB(_pixels, w, h);
			break;
		case OF_IMAGE_COLOR_ALPHA:
			medianRGBAlpha(_pixels, w, h);
			break;
	}
}



//------------------------------------------------------------------
void ofxImageFilters::edges(unsigned char * _pixels, int w, int h, int imageType) {
	int kernel[9] = {	1,0,-1,
						1,0,-1,
						1,0,-1 };
	
	int kernelWeight = 0;
	
	int imageSize;
	
	switch (imageType) {
		case OF_IMAGE_GRAYSCALE:
			imageSize=w*h;
			kernelBlurGrayscale(_pixels, kernel, kernelWeight,w,h);
			break;
		case OF_IMAGE_COLOR:
			imageSize=w*h*3;
			kernelBlurRGB(_pixels, kernel, kernelWeight,w,h);
			break;
		case OF_IMAGE_COLOR_ALPHA:
			imageSize=w*h*4;
			kernelBlurRGBAlpha(_pixels, kernel, kernelWeight,w,h);
			break;
	}
	
	for(int i=0;i<imageSize; i++) {
		_pixels[i]=pixelsBlurred[i];
	}
}



//------------------------------------------------------------------
void ofxImageFilters::sharpen(unsigned char * _pixels, int w, int h, int imageType) {
	int kernel[9] = {	-1,-1,-1,
						-1,9,-1,
						-1,-1,-1};
	
	int kernelWeight = 1;
	
	int imageSize;
	
	switch (imageType) {
		case OF_IMAGE_GRAYSCALE:
			imageSize=w*h;
			kernelBlurGrayscale(_pixels, kernel, kernelWeight,w,h);
			break;
		case OF_IMAGE_COLOR:
			imageSize=w*h*3;
			kernelBlurRGB(_pixels, kernel, kernelWeight,w,h);
			break;
		case OF_IMAGE_COLOR_ALPHA:
			imageSize=w*h*4;
			kernelBlurRGBAlpha(_pixels, kernel, kernelWeight,w,h);
			break;
	}
	
	
	for(int i=0;i<imageSize; i++) {
		_pixels[i]=pixelsBlurred[i];
	}
}


//------------------------------------------------------------------
void ofxImageFilters::gaussian(unsigned char * _pixels, int w, int h, int imageType) {
	int kernel[9] = {	1,2,1,
						2,4,2,
						1,2,1};
	
	int kernelWeight = 16;
	
	int imageSize;
	
	switch (imageType) {
		case OF_IMAGE_GRAYSCALE:
			imageSize=w*h;
			kernelBlurGrayscale(_pixels, kernel, kernelWeight,w,h);
			break;
		case OF_IMAGE_COLOR:
			imageSize=w*h*3;
			kernelBlurRGB(_pixels, kernel, kernelWeight,w,h);
			break;
		case OF_IMAGE_COLOR_ALPHA:
			imageSize=w*h*4;
			kernelBlurRGBAlpha(_pixels, kernel, kernelWeight,w,h);
			break;
	}
	
	
	for(int i=0;i<imageSize; i++) {
		_pixels[i]=pixelsBlurred[i];
	}
}


//------------------------------------------------------------------
void ofxImageFilters::prewitt(unsigned char * _pixels, int w, int h, int imageType) {
	int kernel[9] = {	1,1,1,
						0,0,0,
						-1,-1,-1};
	
	int kernelWeight = 0;
	
	int imageSize;
	
	switch (imageType) {
		case OF_IMAGE_GRAYSCALE:
			imageSize=w*h;
			kernelBlurGrayscale(_pixels, kernel, kernelWeight,w,h);
			break;
		case OF_IMAGE_COLOR:
			imageSize=w*h*3;
			kernelBlurRGB(_pixels, kernel, kernelWeight,w,h);
			break;
		case OF_IMAGE_COLOR_ALPHA:
			imageSize=w*h*4;
			kernelBlurRGBAlpha(_pixels, kernel, kernelWeight,w,h);
			break;
	}
	
	for(int i=0;i<imageSize; i++) {
		_pixels[i]=pixelsBlurred[i];
	}
}





//------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------
//Private Functions


//------------------------------------------------------------------
//FADE GRAYSCALE, ALPHA, RGB

//------------------------------------------------------------------
void ofxImageFilters::fadeGrayscale(unsigned char * _pixels, int w, int h,int amount) {
	for(int i=0; i<w*h; i++) {
		_pixels[i]=MAX(0,_pixels[i]-amount);
	}
}


//------------------------------------------------------------------
void ofxImageFilters::fadeRGB(unsigned char * _pixels, int w, int h,int amount) {
	for(int i=0; i<w*h*3; i++) {
		_pixels[i]=MAX(0,_pixels[i]-amount);
	}
}


//------------------------------------------------------------------
void ofxImageFilters::fadeRGBA(unsigned char * _pixels, int w, int h,int amount) {
	for(int i=0; i<w*h; i++) {
		int thisAlphaPixel=(i*4)+3;
		int value=MAX(0,_pixels[thisAlphaPixel]-amount);
		_pixels[thisAlphaPixel]=value;
	}
}






//------------------------------------------------------------------
//KERNEL BLUR GRAYSCALE, ALPHA, RGB

//------------------------------------------------------------------
void ofxImageFilters::kernelBlurGrayscale(unsigned char * _pixels, int _kernel[9], int _kernelWeight, int w, int h) {
	//Blur from pixels into pixelsBlurred:
	for (int i = 0; i < w; i++){
		for (int j = 0; j < h; j++){
			
			// deal with the edges !! :
			
			int pixIndex = j * w + i;
			
			int i_m_1 = MAX(0, i-1);
			int i_p_1 = MIN(w-1, i+1);
			int j_m_1 = MAX(0, j-1);
			int j_p_1 = MIN(h-1, j+1);
			
			int ne = (j_m_1 * w + i_m_1);
			int n_ = (j_m_1 * w + i    );
			int nw = (j_m_1 * w + i_p_1);
			int _e = (j     * w + i_m_1);
			int me = (j     * w + i    );
			int _w = (j     * w + i_p_1);
			int se = (j_p_1 * w + i_m_1);
			int s_ = (j_p_1 * w + i    );
			int sw = (j_p_1 * w + i_p_1);
			
			int newValue =	_kernel[0] * _pixels[ne] + _kernel[1] * _pixels[n_] + _kernel[2] * _pixels[nw] + 
			_kernel[3] * _pixels[_e] + _kernel[4] * _pixels[me] + _kernel[5] * _pixels[_w] + 
			_kernel[6] * _pixels[se] + _kernel[7] * _pixels[s_] + _kernel[8] * _pixels[sw];
			
			
			if (_kernelWeight > 0) {
				newValue /= _kernelWeight;
			}
				
			if (newValue < 0) newValue = 0;
			if (newValue > 255) newValue = 255;
			
			
			pixelsBlurred[pixIndex]  = newValue;
		}
	}
}



//------------------------------------------------------------------
void ofxImageFilters::kernelBlurRGB(unsigned char * _pixels, int _kernel[9], int _kernelWeight, int w, int h) {
	//Blur from pixels into pixelsBlurred:
	for (int i = 0; i < w; i++){
		for (int j = 0; j < h; j++){
			
			// deal with the edges !! :
			int pixIndex = ((j * w) +i)*3;
			
			int i_m_1 = MAX(0, i-1);
			int i_p_1 = MIN(w-1, i+1);
			int j_m_1 = MAX(0, j-1);
			int j_p_1 = MIN(h-1, j+1);
			
			int nw = (j_m_1 * w + i_m_1)*3;
			int n_ = (j_m_1 * w + i    )*3;
			int ne = (j_m_1 * w + i_p_1)*3;
			int _w = (j     * w + i_m_1)*3;
			int me = (j     * w + i    )*3;
			int _e = (j     * w + i_p_1)*3;
			int sw = (j_p_1 * w + i_m_1)*3;
			int s_ = (j_p_1 * w + i    )*3;
			int se = (j_p_1 * w + i_p_1)*3;
			
			for(int k=0; k<3; k++) {
				
				int newValue =	_kernel[0] * _pixels[ne+k] + _kernel[1] * _pixels[n_+k] + _kernel[2] * _pixels[nw+k] + 
				_kernel[3] * _pixels[_e+k] + _kernel[4] * _pixels[me+k] + _kernel[5] * _pixels[_w+k] + 
				_kernel[6] * _pixels[se+k] + _kernel[7] * _pixels[s_+k] + _kernel[8] * _pixels[sw+k];
				
				if (_kernelWeight > 0){
					newValue /= _kernelWeight;
				} 
					
				if (newValue < 0) newValue = 0;
				if (newValue > 255) newValue = 255;
				
				
				pixelsBlurred[pixIndex+k]  = newValue;
			}
		}
	}
}




//------------------------------------------------------------------
void ofxImageFilters::kernelBlurRGBAlpha(unsigned char * _pixels, int _kernel[9], int _kernelWeight, int w, int h) {
	//Blur from pixels into pixelsBlurred:
	for (int i = 0; i < w; i++){
		for (int j = 0; j < h; j++){
			
			// deal with the edges !! :
			int pixIndex = ((j * w) +i)*4;
			
			int i_m_1 = MAX(0, i-1);
			int i_p_1 = MIN(w-1, i+1);
			int j_m_1 = MAX(0, j-1);
			int j_p_1 = MIN(h-1, j+1);
			
			int nw = (j_m_1 * w + i_m_1)*4;
			int n_ = (j_m_1 * w + i    )*4;
			int ne = (j_m_1 * w + i_p_1)*4;
			int _w = (j     * w + i_m_1)*4;
			int me = (j     * w + i    )*4;
			int _e = (j     * w + i_p_1)*4;
			int sw = (j_p_1 * w + i_m_1)*4;
			int s_ = (j_p_1 * w + i    )*4;
			int se = (j_p_1 * w + i_p_1)*4;
			
			for(int k=0; k<4; k++) {
				
				int newValue =	_kernel[0] * _pixels[ne+k] + _kernel[1] * _pixels[n_+k] + _kernel[2] * _pixels[nw+k] + 
				_kernel[4] * _pixels[_e+k] + _kernel[4] * _pixels[me+k] + _kernel[5] * _pixels[_w+k] + 
				_kernel[6] * _pixels[se+k] + _kernel[7] * _pixels[s_+k] + _kernel[8] * _pixels[sw+k];
				
				if (_kernelWeight > 0){
					newValue /= _kernelWeight;
				} 
					
				if (newValue < 0) newValue = 0;
				if (newValue > 255) newValue = 255;
				
				
				pixelsBlurred[pixIndex+k]  = newValue;
			}
		}
	}
	
}



//------------------------------------------------------------------
//NORMAL BLUR GRAYSCALE, ALPHA, RGB


//------------------------------------------------------------------
void ofxImageFilters::blurNormalGrayscale(unsigned char * _pixels, int w, int h) {
	unsigned char pixelsBlurred[w*h];
	
	// now, let's blur from pixels into pixelsBlurred:
	for (int i = 0; i < w; i++){
		for (int j = 0; j < h; j++){
			// deal with the edges !! :
			int pixIndex = j * w + i;
			
			int i_m_1 = MAX(0, i-1);
			int i_p_1 = MIN(w-1, i+1);
			int j_m_1 = MAX(0, j-1);
			int j_p_1 = MIN(h-1, j+1);
			
			int nw = (j_m_1 * w + i_m_1);
			int n_ = (j_m_1 * w + i    );
			int ne = (j_m_1 * w + i_p_1);
			int _w = (j     * w + i_m_1);
			int me = (j     * w + i    );
			int _e = (j     * w + i_p_1);
			int sw = (j_p_1 * w + i_m_1);
			int s_ = (j_p_1 * w + i    );
			int se = (j_p_1 * w + i_p_1);
			
			int newValue =	_pixels[ne] + _pixels[n_] + _pixels[nw] + _pixels[_e] + _pixels[me] + _pixels[_w] + 
			_pixels[se] + _pixels[s_] + _pixels[sw];
			
			newValue /= 9;
			
			pixelsBlurred[pixIndex]  = newValue;
		}
	}
	
	for(int i=0;i<w*h; i++) {
		_pixels[i]=pixelsBlurred[i];
	}
}



//------------------------------------------------------------------
void ofxImageFilters::blurNormalRGB(unsigned char * _pixels, int w, int h) {
	unsigned char pixelsBlurred[w*h*3];
	
	// now, let's blur from pixels into pixelsBlurred:
	for (int i = 0; i < w; i++){
		for (int j = 0; j < h; j++){
			// deal with the edges !! :
			int pixIndex = ((j * w) +i)*3;
			
			int i_m_1 = MAX(0, i-1);
			int i_p_1 = MIN(w-1, i+1);
			int j_m_1 = MAX(0, j-1);
			int j_p_1 = MIN(h-1, j+1);
			
			int nw = (j_m_1 * w + i_m_1)*3;
			int n_ = (j_m_1 * w + i    )*3;
			int ne = (j_m_1 * w + i_p_1)*3;
			int _w = (j     * w + i_m_1)*3;
			int me = (j     * w + i    )*3;
			int _e = (j     * w + i_p_1)*3;
			int sw = (j_p_1 * w + i_m_1)*3;
			int s_ = (j_p_1 * w + i    )*3;
			int se = (j_p_1 * w + i_p_1)*3;
				
			for(int k=0; k<3; k++) {
				int newValue =	_pixels[ne+k] + _pixels[n_+k] + _pixels[nw+k] + _pixels[_e+k] + _pixels[me+k] + _pixels[_w+k] + 
				_pixels[se+k] + _pixels[s_+k] + _pixels[sw+k];
				
				newValue /= 9;
				
				pixelsBlurred[pixIndex+k]  = newValue;
			}
		}
	}
	
	for(int i=0; i<w*h*3; i++) {
		_pixels[i]=pixelsBlurred[i];
	}
}




//------------------------------------------------------------------
void ofxImageFilters::blurNormalRGBAlpha(unsigned char * _pixels, int w, int h) {
	unsigned char pixelsBlurred[w*h*4];
	
	// now, let's blur from pixels into pixelsBlurred:
	for (int i = 0; i < w; i++){
		for (int j = 0; j < h; j++){
			// deal with the edges !! :
			int pixIndex = ((j * w) +i)*4;
			
			int i_m_1 = MAX(0, i-1);
			int i_p_1 = MIN(w-1, i+1);
			int j_m_1 = MAX(0, j-1);
			int j_p_1 = MIN(h-1, j+1);
			
			int nw = (j_m_1 * w + i_m_1)*4;
			int n_ = (j_m_1 * w + i    )*4;
			int ne = (j_m_1 * w + i_p_1)*4;
			int _w = (j     * w + i_m_1)*4;
			int me = (j     * w + i    )*4;
			int _e = (j     * w + i_p_1)*4;
			int sw = (j_p_1 * w + i_m_1)*4;
			int s_ = (j_p_1 * w + i    )*4;
			int se = (j_p_1 * w + i_p_1)*4;
			
			for(int k=0; k<4; k++) {
				int newValue =	_pixels[ne+k] + _pixels[n_+k] + _pixels[nw+k] + _pixels[_e+k] + _pixels[me+k] + _pixels[_w+k] + 
				_pixels[se+k] + _pixels[s_+k] + _pixels[sw+k];
				
				newValue /= 9;
				
				pixelsBlurred[pixIndex+k]  = newValue;
			}
		}
	}
	
	for(int i=0; i<w*h*4; i++) {
		_pixels[i]=pixelsBlurred[i];
	}
}


//------------------------------------------------------------------
//MEDIAN BLUR GRAYSCALE, ALPHA, RGB

//------------------------------------------------------------------
void ofxImageFilters::medianGrayscale(unsigned char * _pixels, int w, int h) {
	unsigned char pixelsBlurred[w*h];
	
	// now, let's blur from pixels into pixelsBlurred:
	for (int i = 0; i < w; i++){
		for (int j = 0; j < h; j++){
			
			// deal with the edges !! :
			int pixIndex = j * w + i;
			
			int i_m_1 = MAX(0, i-1);
			int i_p_1 = MIN(w-1, i+1);
			int j_m_1 = MAX(0, j-1);
			int j_p_1 = MIN(h-1, j+1);
			
			int pixArray[9];
			
			int nw = (j_m_1 * w + i_m_1);
			int n_ = (j_m_1 * w + i    );
			int ne = (j_m_1 * w + i_p_1);
			int _w = (j     * w + i_m_1);
			int me = (j     * w + i    );
			int _e = (j     * w + i_p_1);
			int sw = (j_p_1 * w + i_m_1);
			int s_ = (j_p_1 * w + i    );
			int se = (j_p_1 * w + i_p_1);
			
			//Copy into array, could be done above but this is clearer
			pixArray[0] = _pixels[nw];
			pixArray[1] = _pixels[n_];
			pixArray[2] = _pixels[ne];
			pixArray[3] = _pixels[_w];
			pixArray[4] = _pixels[me];
			pixArray[5] = _pixels[_e];
			pixArray[6] = _pixels[sw];
			pixArray[7] = _pixels[s_];
			pixArray[8] = _pixels[se];
			
			//Sort them!
			qsort (pixArray, 9, sizeof(int), compare);
			
			//Grab the greatest value
			pixelsBlurred[pixIndex]=pixArray[4];
		}
	}
	
	for(int i=0;i<w*h; i++) {
		_pixels[i]=pixelsBlurred[i];
	}
}


//------------------------------------------------------------------
void ofxImageFilters::medianRGB(unsigned char * _pixels, int w, int h) {
	unsigned char pixelsBlurred[w*h*3];
	
	// now, let's blur from pixels into pixelsBlurred:
	for (int i = 0; i < w; i++){
		for (int j = 0; j < h; j++){
			
			// deal with the edges !! :
			int pixIndex = ((j * w) +i)*3;
			
			int i_m_1 = MAX(0, i-1);
			int i_p_1 = MIN(w-1, i+1);
			int j_m_1 = MAX(0, j-1);
			int j_p_1 = MIN(h-1, j+1);
			
			int pixArray[9];
			
			int nw = (j_m_1 * w + i_m_1)*3;
			int n_ = (j_m_1 * w + i    )*3;
			int ne = (j_m_1 * w + i_p_1)*3;
			int _w = (j     * w + i_m_1)*3;
			int me = (j     * w + i    )*3;
			int _e = (j     * w + i_p_1)*3;
			int sw = (j_p_1 * w + i_m_1)*3;
			int s_ = (j_p_1 * w + i    )*3;
			int se = (j_p_1 * w + i_p_1)*3;
			
			for(int k=0; k<3; k++) {
				//Copy into array, could be done above but this is clearer
				pixArray[0] = _pixels[nw+k];
				pixArray[1] = _pixels[n_+k];
				pixArray[2] = _pixels[ne+k];
				pixArray[3] = _pixels[_w+k];
				pixArray[4] = _pixels[me+k];
				pixArray[5] = _pixels[_e+k];
				pixArray[6] = _pixels[sw+k];
				pixArray[7] = _pixels[s_+k];
				pixArray[8] = _pixels[se+k];
				
				//Sort them!
				qsort (pixArray, 9, sizeof(int), compare);
				
				//Grab the greatest value
				pixelsBlurred[pixIndex+k]=pixArray[4];
			}
		}
	}
	
	for(int i=0;i<w*h*3; i++) {
		_pixels[i]=pixelsBlurred[i];
	}
}



//------------------------------------------------------------------
void ofxImageFilters::medianRGBAlpha(unsigned char * _pixels, int w, int h) {
	unsigned char pixelsBlurred[w*h*4];
	
	// now, let's blur from pixels into pixelsBlurred:
	for (int i = 0; i < w; i++){
		for (int j = 0; j < h; j++){
			
			// deal with the edges !! :
			int pixIndex = ((j * w) +i)*4;
			
			int i_m_1 = MAX(0, i-1);
			int i_p_1 = MIN(w-1, i+1);
			int j_m_1 = MAX(0, j-1);
			int j_p_1 = MIN(h-1, j+1);
			
			int pixArray[9];
			
			int nw = (j_m_1 * w + i_m_1)*4;
			int n_ = (j_m_1 * w + i    )*4;
			int ne = (j_m_1 * w + i_p_1)*4;
			int _w = (j     * w + i_m_1)*4;
			int me = (j     * w + i    )*4;
			int _e = (j     * w + i_p_1)*4;
			int sw = (j_p_1 * w + i_m_1)*4;
			int s_ = (j_p_1 * w + i    )*4;
			int se = (j_p_1 * w + i_p_1)*4;
			
			for(int k=0; k<4; k++) {
				//Copy into array, could be done above but this is clearer
				pixArray[0] = _pixels[nw+k];
				pixArray[1] = _pixels[n_+k];
				pixArray[2] = _pixels[ne+k];
				pixArray[3] = _pixels[_w+k];
				pixArray[4] = _pixels[me+k];
				pixArray[5] = _pixels[_e+k];
				pixArray[6] = _pixels[sw+k];
				pixArray[7] = _pixels[s_+k];
				pixArray[8] = _pixels[se+k];
				
				//Sort them!
				qsort (pixArray, 9, sizeof(int), compare);
				
				//Grab the greatest value
				pixelsBlurred[pixIndex+k]=pixArray[4];
			}
		}
	}
	
	for(int i=0;i<w*h*4; i++) {
		_pixels[i]=pixelsBlurred[i];
	}
}



//------------------------------------------------------------------
//Super Fast Blur Filters
//created by Mario Klingemann <http://incubator.quasimondo.com>
//converted to C++ by Mehmet Akten, <http://www.memo.tv>
//converted for RGB Alpha and Grayscale by Steve Varga <http://www.vargatron.com>

//------------------------------------------------------------------
void ofxImageFilters::superFastBlurGrayscale(unsigned char * pix, int w, int h, int radius) {
	if (radius<1) return;
	int wm=w-1;
	int hm=h-1;
	int wh=w*h;
	int div=radius+radius+1;
	int pixsum,x,y,i,p,p1,p2,yp,yi,yw;
	int *vMIN = new int[MAX(w,h)];
	int *vMAX = new int[MAX(w,h)];
	
	unsigned char *dv=new unsigned char[256*div];
	for (i=0;i<256*div;i++) dv[i]=(i/div);
	
	yw=yi=0;
	
	for (y=0;y<h;y++){
		pixsum=0;
		for(i=-radius;i<=radius;i++){
			p = (yi + MIN(wm, MAX(i,0)));
			pixsum += pix[p];
		}
		for (x=0;x<w;x++){
			
			pixelsBlurred[yi]=dv[pixsum];
			
			if(y==0){
				vMIN[x]=MIN(x+radius+1,wm);
				vMAX[x]=MAX(x-radius,0);
			} 
			p1 = (yw+vMIN[x]);
			p2 = (yw+vMAX[x]);
			
			pixsum += pix[p1] - pix[p2];
			
			yi++;
		}
		yw+=w;
	}
	
	for (x=0;x<w;x++){
		pixsum=0;
		yp=-radius*w;
		for(i=-radius;i<=radius;i++){
			yi=MAX(0,yp)+x;
			pixsum+=pixelsBlurred[yi];
			yp+=w;
		}
		yi=x;
		for (y=0;y<h;y++){
			pix[yi]      = dv[pixsum];
			if(x==0){
				vMIN[y]=MIN(y+radius+1,hm)*w;
				vMAX[y]=MAX(y-radius,0)*w;
			} 
			p1=x+vMIN[y];
			p2=x+vMAX[y];
			
			pixsum+=pixelsBlurred[p1]-pixelsBlurred[p2];
			
			yi+=w;
		}
	}
	
	delete vMIN;
	delete vMAX;
	delete dv;
}



//------------------------------------------------------------------
void ofxImageFilters::superFastBlurRGB(unsigned char * pix, int w, int h, int radius) {
	if (radius<1) return;
	int wm=w-1;
	int hm=h-1;
	int wh=w*h;
	int div=radius+radius+1;
	unsigned char *r=new unsigned char[wh];
	unsigned char *g=new unsigned char[wh];
	unsigned char *b=new unsigned char[wh];
	int rsum,gsum,bsum,x,y,i,p,p1,p2,yp,yi,yw;
	int *vMIN = new int[MAX(w,h)];
	int *vMAX = new int[MAX(w,h)];
	
	unsigned char *dv=new unsigned char[256*div];
	for (i=0;i<256*div;i++) dv[i]=(i/div);
	
	yw=yi=0;
	
	for (y=0;y<h;y++){
		rsum=gsum=bsum=0;
		for(i=-radius;i<=radius;i++){
			p = (yi + MIN(wm, MAX(i,0))) * 3;
			rsum += pix[p];
			gsum += pix[p+1];
			bsum += pix[p+2];
		}
		for (x=0;x<w;x++){
			
			r[yi]=dv[rsum];
			g[yi]=dv[gsum];
			b[yi]=dv[bsum];
			
			if(y==0){
				vMIN[x]=MIN(x+radius+1,wm);
				vMAX[x]=MAX(x-radius,0);
			} 
			p1 = (yw+vMIN[x])*3;
			p2 = (yw+vMAX[x])*3;
			
			rsum += pix[p1]      - pix[p2];
			gsum += pix[p1+1]   - pix[p2+1];
			bsum += pix[p1+2]   - pix[p2+2];
			
			yi++;
		}
		yw+=w;
	}
	
	for (x=0;x<w;x++){
		rsum=gsum=bsum=0;
		yp=-radius*w;
		for(i=-radius;i<=radius;i++){
			yi=MAX(0,yp)+x;
			rsum+=r[yi];
			gsum+=g[yi];
			bsum+=b[yi];
			yp+=w;
		}
		yi=x;
		for (y=0;y<h;y++){
			pix[yi*3]      = dv[rsum];
			pix[yi*3 + 1]   = dv[gsum];
			pix[yi*3 + 2]   = dv[bsum];
			if(x==0){
				vMIN[y]=MIN(y+radius+1,hm)*w;
				vMAX[y]=MAX(y-radius,0)*w;
			} 
			p1=x+vMIN[y];
			p2=x+vMAX[y];
			
			rsum+=r[p1]-r[p2];
			gsum+=g[p1]-g[p2];
			bsum+=b[p1]-b[p2];
			
			yi+=w;
		}
	}
	
	
	delete r;
	delete g;
	delete b;
	
	delete vMIN;
	delete vMAX;
	delete dv;
}


//------------------------------------------------------------------
void ofxImageFilters::superFastBlurRGBAlpha(unsigned char * pix, int w, int h, int radius) {
	if (radius<1) return;
	int wm=w-1;
	int hm=h-1;
	int wh=w*h;
	int div=radius+radius+1;
	unsigned char *r=new unsigned char[wh];
	unsigned char *g=new unsigned char[wh];
	unsigned char *b=new unsigned char[wh];
	unsigned char *a=new unsigned char[wh];
	int rsum,gsum,bsum,asum,x,y,i,p,p1,p2,yp,yi,yw;
	int *vMIN = new int[MAX(w,h)];
	int *vMAX = new int[MAX(w,h)];
	
	unsigned char *dv=new unsigned char[256*div];
	for (i=0;i<256*div;i++) dv[i]=(i/div);
	
	yw=yi=0;
	
	for (y=0;y<h;y++){
		rsum=gsum=bsum=asum=0;
		for(i=-radius;i<=radius;i++){
			p = (yi + MIN(wm, MAX(i,0))) * 4;
			rsum += pix[p];
			gsum += pix[p+1];
			bsum += pix[p+2];
			asum += pix[p+3];
		}
		for (x=0;x<w;x++){
			
			r[yi]=dv[rsum];
			g[yi]=dv[gsum];
			b[yi]=dv[bsum];
			a[yi]=dv[asum];
			
			if(y==0){
				vMIN[x]=MIN(x+radius+1,wm);
				vMAX[x]=MAX(x-radius,0);
			} 
			p1 = (yw+vMIN[x])*4;
			p2 = (yw+vMAX[x])*4;
			
			rsum += pix[p1]     - pix[p2];
			gsum += pix[p1+1]   - pix[p2+1];
			bsum += pix[p1+2]   - pix[p2+2];
			asum += pix[p1+3]   - pix[p2+3];
			
			yi++;
		}
		yw+=w;
	}
	
	for (x=0;x<w;x++){
		rsum=gsum=bsum=0;
		yp=-radius*w;
		for(i=-radius;i<=radius;i++){
			yi=MAX(0,yp)+x;
			rsum+=r[yi];
			gsum+=g[yi];
			bsum+=b[yi];
			asum+=a[yi];
			yp+=w;
		}
		yi=x;
		for (y=0;y<h;y++){
			pix[yi*4]		= dv[rsum];
			pix[yi*4 + 1]   = dv[gsum];
			pix[yi*4 + 2]   = dv[bsum];
			pix[yi*4 + 3]   = dv[asum];
			if(x==0){
				vMIN[y]=MIN(y+radius+1,hm)*w;
				vMAX[y]=MAX(y-radius,0)*w;
			} 
			p1=x+vMIN[y];
			p2=x+vMAX[y];
			
			rsum+=r[p1]-r[p2];
			gsum+=g[p1]-g[p2];
			bsum+=b[p1]-b[p2];
			asum+=a[p1]-a[p2];
			
			yi+=w;
		}
	}
	
	delete r;
	delete g;
	delete b;
	delete a;
	
	delete vMIN;
	delete vMAX;
	delete dv;
}


