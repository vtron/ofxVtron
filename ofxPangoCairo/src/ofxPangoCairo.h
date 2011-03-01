/*
 *  ofxPangoCairo.h
 *  ofxPangoCairo
 *
 *  Created by Stephen Varga on 8/4/10.
 *  Copyright 2010. All rights reserved.
 *
 */
#ifndef _ofxPangoCairo
#define _ofxPangoCairo


#include "ofMain.h"
#include <stdio.h>
#include <math.h>
#include <pango/pangocairo.h>


class ofxPangoCairo {
	
	public:
		ofxPangoCairo();
		~ofxPangoCairo();
	
		void rendertext(cairo_t *cr);
		void setup();
		void update();
		void draw();
	
		cairo_t *cr;
		cairo_status_t status;
		cairo_surface_t *surface;
};

#endif
