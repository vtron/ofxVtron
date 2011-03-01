/*
 *  ofxFreeType2.h
 *  ofxFreeType2
 *
 *  Created by Stephen Varga on 8/3/10.
 *  Copyright 2010. All rights reserved.
 *
 */
#ifndef _ofxFreeType2
#define _ofxFreeType2

#include "ofMain.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

#include "ofConstants.h"
#include "ofGraphics.h"
#include "ofUtils.h"
#include "ofTypes.h"





#define START_CHAR 0
// 0 - 32 are control characters, no graphics needed.

//Character Info
typedef struct {
	int value;
	
	float height;
	float width;
	float left;
	float top;
	float bottom;
	
	float advance;
	float texWidth;
	float texHeight;
	float texXDiff;
	float texYDiff;
} charInfo; 

//Text Alignment for Text Areas
enum ofTextAlignment {
	OF_TEXT_ALIGN_LEFT,
	OF_TEXT_ALIGN_CENTER,
	OF_TEXT_ALIGN_RIGHT
};

//Outline Specific Settings
//For drawStringAsShapes and getCharacterAsPoints only
//Make this number smaller to create TTF shapes with more pts = slower but accurate
//Make this number larger to create TTF shapes with less pts  = faster but not as accurate
//Our default is 0.3 which removes segments that are less than 0.3 of a pixel in length
#define TTF_SHAPE_SIMPLIFICATION_AMNT (0.3)

class ofTTContour {
public:
	vector <ofPoint>pts;
};


class ofTTCharacter {
public:
	vector <ofTTContour> contours;
};


class ofxFreeType2 {
	
public:
	ofxFreeType2();
	~ofxFreeType2();
	
	void test();
	
	void	loadFont(string filepath, int size);
	void	loadFont(string filepath, int size, bool _bAntiAlias, bool _bFullCharacterSet, bool _bMakeContours = false);
	
	void	setResolution(int resolution);
	
	
	void	drawString(string s, int x, int y);
	void	drawTextArea(string s, int x, int y, int width, int height, ofTextAlignment alignment = OF_TEXT_ALIGN_LEFT, bool bHideOverflow = true);
	
	int		getLineHeight();
	void	setLineHeight(float height);
	
	int		getStringWidth(string s);
	int		getStringHeight(string s);
	int		getStringHeight(string s, int width);
	
	int		getMaxCharHeight(string s, bool bFromBaseline = false);
	
	ofRectangle		getStringBoundingBox(string s, float x, float y);
	
	//Outlines Specific
	void			drawStringAsShapes(string s, float x, float y);
	ofTTCharacter	getCharacterAsPoints(int character);
	
	
	vector <string> getTextAreaLines(string s, int width);
	
private:
	bool bFaceLoaded;
	
	//Characters
	bool		bFullCharacterSet;
	bool		bAntiAlias;
	int			nChars;
	charInfo	*chars;
	GLuint		*charTextures;	// Textures for each character
	
	//Font Properties
	int			resolution;
	int			fontSize;
	int			lineHeight;
	
	void		drawChar(int c);
	
	
	//Outlines
	bool bMakeContours;
	vector <ofTTCharacter> charOutlines;
	void		drawCharAsShape(int c);
};

#endif
