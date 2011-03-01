/*
 *  ofxPangoCairo.cpp
 *  ofxPangoCairo
 *
 *  Created by Stephen Varga on 8/4/10.
 *  Copyright 2010. All rights reserved.
 *
 */

#include "ofxPangoCairo.h"


ofxPangoCairo::ofxPangoCairo() {
	surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 1024, 768);
	cr = cairo_create(surface);
	
	cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
	cairo_paint(cr);
	
	rendertext(cr);
	
	cairo_destroy(cr);
	status = cairo_surface_write_to_png(surface, ofToDataPath("out.png").c_str());
	cairo_surface_destroy(surface);
	
	if (status != CAIRO_STATUS_SUCCESS) {
		// if 'status' was not set to indicate a successful operation when saving as a png, error
		printf("Could not save to png, \"out.png\"\n");
	}
}



ofxPangoCairo::~ofxPangoCairo() {
}


//------------------------------------------------------------------
void ofxPangoCairo::rendertext(cairo_t *cr) {
	
	PangoLayout *layout;
	PangoFontDescription *desc;
	
	cairo_translate(cr, 10, 20);
	layout = pango_cairo_create_layout(cr);
	
	pango_layout_set_text(layout, "Hello World!", -1);
	desc = pango_font_description_from_string("Sans Bold 100");
	pango_layout_set_font_description(layout, desc);
	pango_font_description_free(desc);
	
	cairo_set_source_rgb(cr, 0.0, 0.0, 1.0);
	pango_cairo_update_layout(cr, layout);
	pango_cairo_show_layout(cr, layout);
	/*
	g_object_unref(layout);
	 */
}


//------------------------------------------------------------------
void ofxPangoCairo::setup() {
}



//------------------------------------------------------------------
void ofxPangoCairo::update() {
}



//------------------------------------------------------------------
void ofxPangoCairo::draw() {
}

