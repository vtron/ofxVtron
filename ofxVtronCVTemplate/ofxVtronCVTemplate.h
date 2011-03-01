/*
 *  ofxVtronCVTemplate.h
 *
 *  Created by Stephen Varga on 5/22/10.
 *	Based on code by Carles Gutierrez
 *  Copyright 2010. All rights reserved.
 *
 */

#ifndef OFX_VTRON_CV_TEMPLATE
#define OFX_VTRON_CV_TEMPLATE

#include "ofxCvConstants.h"
#include "ofxCvBlob.h"
#include "ofxCvGrayscaleImage.h"
#include "ofxCvColorImage.h"
#include <algorithm>

class ofxVtronCVTemplate {

  public:

    ofxVtronCVTemplate();
    ~ofxVtronCVTemplate();

	float getWidth();    //set after first findContours call
	float getHeight();  //set after first findContours call

    int  matchToTemplate( ofxCvGrayscaleImage& input,
                               ofxCvGrayscaleImage& tp1);
                               // approximation = don't do points for all points
                               // of the contour, if the contour runs
                               // along a straight line, for example...
	void draw();


  protected:
    int  res_w;
    int  res_h;

	CvPoint		minloc, maxloc;
	double		minval, maxval;

    ofxCvColorImage         resMtemplate;
    ofxCvGrayscaleImage     inputCopy;
    ofxCvGrayscaleImage     templateCopy;

    virtual void reset();

};



#endif
