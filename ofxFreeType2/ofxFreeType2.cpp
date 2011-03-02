/*
 *  ofxFreeType2.cpp
 *  ofxFreeType2
 *
 *  Created by Stephen Varga on 8/3/10.
 *  Copyright 2010. All rights reserved.
 *
 */

#include "ofxFreeType2.h"

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//Outline Font Static Functions


static bool printVectorInfo = false;

//This is for polygon/contour simplification - we use it to reduce the number of points needed in
//representing the letters as openGL shapes - will soon be moved to ofGraphics.cpp

// From: http://softsurfer.com/Archive/algorithm_0205/algorithm_0205.htm
// Copyright 2002, softSurfer (www.softsurfer.com)
// This code may be freely used and modified for any purpose
// providing that this copyright notice is included with it.
// SoftSurfer makes no warranty for this code, and cannot be held
// liable for any real or imagined damage resulting from its use.
// Users of this code must verify correctness for their application.

typedef struct{
	ofPoint P0;
	ofPoint P1;
}Segment;

// dot product (3D) which allows vector operations in arguments
#define dot(u,v)   ((u).x * (v).x + (u).y * (v).y + (u).z * (v).z)
#define norm2(v)   dot(v,v)        // norm2 = squared length of vector
#define norm(v)    sqrt(norm2(v))  // norm = length of vector
#define d2(u,v)    norm2(u-v)      // distance squared = norm2 of difference
#define d(u,v)     norm(u-v)       // distance = norm of difference

static void simplifyDP(float tol, ofPoint* v, int j, int k, int* mk ){
    if (k <= j+1) // there is nothing to simplify
        return;
	
    // check for adequate approximation by segment S from v[j] to v[k]
    int     maxi	= j;          // index of vertex farthest from S
    float   maxd2	= 0;         // distance squared of farthest vertex
    float   tol2	= tol * tol;  // tolerance squared
    Segment S		= {v[j], v[k]};  // segment from v[j] to v[k]
    ofPoint u;
	u				= S.P1 - S.P0;   // segment direction vector
    double  cu		= dot(u,u);     // segment length squared
	
    // test each vertex v[i] for max distance from S
    // compute using the Feb 2001 Algorithm's dist_ofPoint_to_Segment()
    // Note: this works in any dimension (2D, 3D, ...)
    ofPoint  w;
    ofPoint   Pb;                // base of perpendicular from v[i] to S
    float  b, cw, dv2;        // dv2 = distance v[i] to S squared
	
    for (int i=j+1; i<k; i++){
        // compute distance squared
        w = v[i] - S.P0;
        cw = dot(w,u);
        if ( cw <= 0 ) dv2 = d2(v[i], S.P0);
        else if ( cu <= cw ) dv2 = d2(v[i], S.P1);
        else {
            b = (float)(cw / cu);
            Pb = S.P0 + u*b;
            dv2 = d2(v[i], Pb);
        }
        // test with current max distance squared
        if (dv2 <= maxd2) continue;
		
        // v[i] is a new max vertex
        maxi = i;
        maxd2 = dv2;
    }
    if (maxd2 > tol2)        // error is worse than the tolerance
    {
        // split the polyline at the farthest vertex from S
        mk[maxi] = 1;      // mark v[maxi] for the simplified polyline
        // recursively simplify the two subpolylines at v[maxi]
        simplifyDP( tol, v, j, maxi, mk );  // polyline v[j] to v[maxi]
        simplifyDP( tol, v, maxi, k, mk );  // polyline v[maxi] to v[k]
    }
    // else the approximation is OK, so ignore intermediate vertices
    return;
}



//-------------------------------------------------------------------
// needs simplifyDP which is above
static vector <ofPoint> ofSimplifyContour(vector <ofPoint> &V, float tol){
	int n = V.size();
	
	vector <ofPoint> sV;
	sV.assign(n, ofPoint());
	
    int    i, k, m, pv;            // misc counters
    float  tol2 = tol * tol;       // tolerance squared
    ofPoint * vt = new ofPoint[n];
	int * mk = new int[n];
	
	memset(mk, 0, sizeof(int) * n );
	
    // STAGE 1.  Vertex Reduction within tolerance of prior vertex cluster
    vt[0] = V[0];              // start at the beginning
    for (i=k=1, pv=0; i<n; i++) {
        if (d2(V[i], V[pv]) < tol2) continue;
		
        vt[k++] = V[i];
        pv = i;
    }
    if (pv < n-1) vt[k++] = V[n-1];      // finish at the end
	
    // STAGE 2.  Douglas-Peucker polyline simplification
    mk[0] = mk[k-1] = 1;       // mark the first and last vertices
    simplifyDP( tol, vt, 0, k-1, mk );
	
    // copy marked vertices to the output simplified polyline
    for (i=m=0; i<k; i++) {
        if (mk[i]) sV[m++] = vt[i];
    }
	
	//get rid of the unused points
	if( m < (int)sV.size() ) sV.erase( sV.begin()+m, sV.end() );
	
	delete [] vt;
	delete [] mk;
	
	return sV;
}



//------------------------------------------------------------
static void quad_bezier(vector <ofPoint> &ptsList, float x1, float y1, float x2, float y2, float x3, float y3, int res){
	for(int i=0; i <= res; i++){
        double t = (double)i / (double)(res);
        double a = pow((1.0 - t), 2.0);
        double b = 2.0 * t * (1.0 - t);
        double c = pow(t, 2.0);
        double x = a * x1 + b * x2 + c * x3;
        double y = a * y1 + b * y2 + c * y3;
		ptsList.push_back(ofPoint((float)x, (float)y));
    }
}



//-----------------------------------------------------------
static void cubic_bezier(vector <ofPoint> &ptsList, float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3, int res){
	float   ax, bx, cx;
    float   ay, by, cy;
    float   t, t2, t3;
    float   x, y;
	
    // polynomial coefficients
    cx = 3.0f * (x1 - x0);
    bx = 3.0f * (x2 - x1) - cx;
    ax = x3 - x0 - cx - bx;
	
    cy = 3.0f * (y1 - y0);
    by = 3.0f * (y2 - y1) - cy;
    ay = y3 - y0 - cy - by;
	
	
    int resolution = res;
	
    for (int i = 0; i < resolution; i++){
    	t 	=  (float)i / (float)(resolution-1);
    	t2 = t * t;
    	t3 = t2 * t;
		x = (ax * t3) + (bx * t2) + (cx * t) + x0;
    	y = (ay * t3) + (by * t2) + (cy * t) + y0;
    	ptsList.push_back(ofPoint(x,y) );
    }
}



//--------------------------------------------------------
static ofTTCharacter makeContoursForCharacter(FT_Face &face);
static ofTTCharacter makeContoursForCharacter(FT_Face &face){
	
	//int num			= face->glyph->outline.n_points;
	int nContours	= face->glyph->outline.n_contours;
	int startPos	= 0;
	
	char * tags		= face->glyph->outline.tags;
	FT_Vector * vec = face->glyph->outline.points;
	
	ofTTCharacter charOutlines;
	
	for(int k = 0; k < nContours; k++){
		if( k > 0 ){
			startPos = face->glyph->outline.contours[k-1]+1;
		}
		int endPos = face->glyph->outline.contours[k]+1;
		
		if( printVectorInfo )printf("--NEW CONTOUR\n\n");
		
		vector <ofPoint> testOutline;
		ofPoint lastPoint;
		
		for(int j = startPos; j < endPos; j++){
			
			if( FT_CURVE_TAG(tags[j]) == FT_CURVE_TAG_ON ){
				lastPoint.set((float)vec[j].x, (float)-vec[j].y, 0);
				if( printVectorInfo )printf("flag[%i] is set to 1 - regular point - %f %f \n", j, lastPoint.x, lastPoint.y);
				testOutline.push_back(lastPoint);
				
			}else{
				if( printVectorInfo )printf("flag[%i] is set to 0 - control point \n", j);
				
				if( FT_CURVE_TAG(tags[j]) == FT_CURVE_TAG_CUBIC ){
					if( printVectorInfo )printf("- bit 2 is set to 2 - CUBIC\n");
					
					int prevPoint = j-1;
					if( j == 0){
						prevPoint = endPos-1;
					}
					
					int nextIndex = j+1;
					if( nextIndex >= endPos){
						nextIndex = startPos;
					}
					
					ofPoint nextPoint( (float)vec[nextIndex].x,  -(float)vec[nextIndex].y );
					
					//we need two control points to draw a cubic bezier
					bool lastPointCubic =  ( FT_CURVE_TAG(tags[prevPoint]) != FT_CURVE_TAG_ON ) && ( FT_CURVE_TAG(tags[prevPoint]) == FT_CURVE_TAG_CUBIC);
					
					if( lastPointCubic ){
						ofPoint controlPoint1((float)vec[prevPoint].x,	(float)-vec[prevPoint].y);
						ofPoint controlPoint2((float)vec[j].x, (float)-vec[j].y);
						ofPoint nextPoint((float) vec[nextIndex].x,	-(float) vec[nextIndex].y);
						
						cubic_bezier(testOutline, lastPoint.x, lastPoint.y, controlPoint1.x, controlPoint1.y, controlPoint2.x, controlPoint2.y, nextPoint.x, nextPoint.y, 8);
					}
					
				}else{
					
					ofPoint conicPoint( (float)vec[j].x,  -(float)vec[j].y );
					
					if( printVectorInfo )printf("- bit 2 is set to 0 - conic- \n");
					if( printVectorInfo )printf("--- conicPoint point is %f %f \n", conicPoint.x, conicPoint.y);
					
					//If the first point is connic and the last point is connic then we need to create a virutal point which acts as a wrap around
					if( j == startPos ){
						bool prevIsConnic = (  FT_CURVE_TAG( tags[endPos-1] ) != FT_CURVE_TAG_ON ) && ( FT_CURVE_TAG( tags[endPos-1]) != FT_CURVE_TAG_CUBIC );
						
						if( prevIsConnic ){
							ofPoint lastConnic((float)vec[endPos - 1].x, (float)-vec[endPos - 1].y);
							lastPoint = (conicPoint + lastConnic) / 2;
							
							if( printVectorInfo )	printf("NEED TO MIX WITH LAST\n");
							if( printVectorInfo )printf("last is %f %f \n", lastPoint.x, lastPoint.y);
						}
					}
					
					//bool doubleConic = false;
					
					int nextIndex = j+1;
					if( nextIndex >= endPos){
						nextIndex = startPos;
					}
					
					ofPoint nextPoint( (float)vec[nextIndex].x,  -(float)vec[nextIndex].y );
					
					if( printVectorInfo )printf("--- last point is %f %f \n", lastPoint.x, lastPoint.y);
					
					bool nextIsConnic = (  FT_CURVE_TAG( tags[nextIndex] ) != FT_CURVE_TAG_ON ) && ( FT_CURVE_TAG( tags[nextIndex]) != FT_CURVE_TAG_CUBIC );
					
					//create a 'virtual on point' if we have two connic points
					if( nextIsConnic ){
						nextPoint = (conicPoint + nextPoint) / 2;
						if( printVectorInfo )printf("|_______ double connic!\n");
					}
					if( printVectorInfo )printf("--- next point is %f %f \n", nextPoint.x, nextPoint.y);
					
					quad_bezier(testOutline, lastPoint.x, lastPoint.y, conicPoint.x, conicPoint.y, nextPoint.x, nextPoint.y, 8);
					
					if( nextIsConnic ){
						lastPoint = nextPoint;
					}
				}
			}
			
			//end for
		}
		
		for(int g =0; g < (int)testOutline.size(); g++){
			testOutline[g] /= 64.0f;
		}
		
		charOutlines.contours.push_back(ofTTContour());
		
		if( testOutline.size() ){
			charOutlines.contours.back().pts = ofSimplifyContour(testOutline, (float)TTF_SHAPE_SIMPLIFICATION_AMNT);
		}else{
			charOutlines.contours.back().pts = testOutline;
		}
	}
	
	return charOutlines;
}












//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//Actual Class Starts Here

ofxFreeType2::ofxFreeType2() {
	bFaceLoaded		= false;
	bMakeContours	= false;
	resolution		= 0;
}


ofxFreeType2::~ofxFreeType2() {
	if (bFaceLoaded){
		if (chars != NULL){
			delete[] chars;
		}
		
		if (charTextures != NULL){
			for (int i = 0; i < nChars; i++){
				glDeleteTextures(1, &charTextures[i]);
			}
			delete[] charTextures;
		}
	}
}





//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//Loading

//--------------------------------------------------------------
void ofxFreeType2::loadFont(string filepath, int size) {
	loadFont(filepath, size, true, false,false);
}



//--------------------------------------------------------------
void ofxFreeType2::loadFont(string filepath, int size, bool _bAntiAlias, bool _bFullCharacterSet, bool _bMakeContours) {
	//Init vars
	fontSize			=  size;
	lineHeight			= fontSize;
	bAntiAlias			= _bAntiAlias;
	bFullCharacterSet	= _bFullCharacterSet;
	bMakeContours		= _bMakeContours;
	
	
	//Clear chars if a font has already been loaded
	if (bFaceLoaded){
		if (chars != NULL){
			delete[] chars;
		}
		
		if (charTextures != NULL){
			for (int i = 0; i < nChars; i++){
				glDeleteTextures(1, &charTextures[i]);
			}
			delete[] charTextures;
		}
	}
	
	bFaceLoaded = false;
	
	
	//FreeType Vars
	FT_Library	library;
	FT_Face		face;
	FT_Error error;
	
	//Init library
	if (FT_Init_FreeType(&library)) { 
		ofLog(OF_LOG_ERROR,"Freetype Error: Unable to Initialize Library");
		return;
	}
	
	
	
	//Load Font Face
	error = FT_New_Face(library,
						ofToDataPath(filepath).c_str(),
						0,
						&face );
	
	if (error == FT_Err_Unknown_File_Format) { //Font opened but unsupported
		ofLog(OF_LOG_ERROR,"Freetype Error: Unknown Format");
		return;
	} else if (error) { //Font can not be read/is broken
		ofLog(OF_LOG_ERROR,"Freetype Error: Couldn't open font.");
		return;
	}
	
	
	//Set Size (pts or pixels)
	if(resolution) error = FT_Set_Char_Size(face, 0,  (float(size) * (72.0/float(resolution)))*64.0, resolution, resolution);	  
	else error = FT_Set_Pixel_Sizes(face, size, size);
	
	if(error) {
		ofLog(OF_LOG_ERROR,"Freetype Error: Couldn't set font size. Font may be fixed width?");
		return;
	}
	
	//Get total Chars from Face
	nChars	= bFullCharacterSet ? 256 : 128;
	nChars -= START_CHAR;
	
	//Init arrays, generate textures
	chars			= new charInfo[nChars];
	charTextures	= new GLuint[nChars];
	glGenTextures(nChars, charTextures);
	
	//Clear/Create contour array if necessary
	if(bMakeContours){
		charOutlines.clear();
		charOutlines.assign(nChars, ofTTCharacter());
	}
	
	
	//Create Characters
	FT_Glyph glyph;
	FT_BBox	bbox;
	
	for(int i=START_CHAR; i<nChars; i++) {
		//Load Glyph
		int charIndex	= i - START_CHAR;
		
		if(bAntiAlias) {
			error = FT_Load_Char(face, i, FT_LOAD_RENDER | FT_LOAD_FORCE_AUTOHINT);
		} else {
			error = FT_Load_Char(face, i, FT_LOAD_RENDER | FT_LOAD_MONOCHROME);
		}
		
		if(error) {
			ofLog(OF_LOG_ERROR,"Freetype Error: Couldn't load glyph.");
			return;
		}
		
		
		
		//Create Outline if necessary
		if(bMakeContours){
			if( printVectorInfo )printf("\n\ncharacter %c: \n", char( i ) );
			
			//int character = i + NUM_CHARACTER_TO_START;
			charOutlines[charIndex] = makeContoursForCharacter( face );
		}
		
		
		//Get Size of Bitmap, convert from fractional pixel format
		float bmpW	= face->glyph->metrics.width >> 6;
		float bmpH	= face->glyph->metrics.height >> 6;
		
		
		//Create Texture Sizes
		//Add 2px to avoid edges on either side
		
		int width  = ofNextPow2(bmpW+1);
		int height = ofNextPow2(bmpH+1);
		if (width == 1) width = 2;
		if (height == 1) height = 2;
		
		//Set Info for This Glyph
		FT_Get_Glyph(face->glyph, &glyph );
		FT_Glyph_Get_CBox(glyph, 0, &bbox );
		
		float xMin = bbox.xMin >> 6;
		float xMax = bbox.xMax >> 6;
		float yMin = bbox.yMin >> 6;
		float yMax = bbox.yMax >> 6;
		
		chars[charIndex].value 			= i;
		chars[charIndex].width 			= xMax - xMin;
		chars[charIndex].height 		= yMax - yMin;
		chars[charIndex].left			= face->glyph->bitmap_left;
		chars[charIndex].top			= yMax;
		chars[charIndex].bottom			= yMin;
		chars[charIndex].advance		= face->glyph->metrics.horiAdvance >> 6;
		
		//Set the position of the texture, centered with pow2 tex
		chars[charIndex].texWidth 		= bmpW;
		chars[charIndex].texHeight 		= bmpH;
		chars[charIndex].texXDiff	= (float)bmpW/(float)width;
		chars[charIndex].texYDiff	= (float)bmpH/(float)height;
		
		// Allocate Memory For The Texture Data.
		unsigned char* expanded_data = new unsigned char[2 * width * height];
		
		//Clear Texture Data
		for(int j=0; j <height;j++) {
			for(int k=0; k < width; k++){
				expanded_data[2*(k+(j*width))] = 255;   // every luminance pixel = 255
				expanded_data[2*(k+(j*width))+1] = 0;
			}
		}
		
		
		//Create texture from bitmap of glyph
		FT_Bitmap& bitmap= face->glyph->bitmap;
		
		if (bAntiAlias){
			//-----------------------------------
			for(int j=0; j <height; j++) {
				for(int k=0; k < width; k++){
					if ((k<bitmap.width) && (j<bitmap.rows)){
						//Offset pixels into texture by 1px to avoid edges
						//expanded_data[2*(((j+1)*width)+k +1) + 1] = bitmap.buffer[k + bitmap.width*(j)];
						int thisPixel = (((j * width) + k) * 2) + 1;
						expanded_data[thisPixel] = bitmap.buffer[k + bitmap.width*(j)];
					}
				}
			}
			//-----------------------------------
		} else {
			//-----------------------------------
			// true type packs monochrome info in a
			// 1-bit format, hella funky
			// here we unpack it:
			unsigned char *src =  bitmap.buffer;
			for(int j=0; j <bitmap.rows;j++) {
				unsigned char b=0;
				unsigned char *bptr =  src;
				for(int k=0; k < bitmap.width ; k++){
					if (k%8==0) b=(*bptr++);
					
					//Offset pixels into texture by 1px to avoid edges
					expanded_data[2*(k+j*width)+1] = b&0x80 ? 255 : 0;
					
				    b <<= 1;
				}
				src += bitmap.pitch;
			}
			//-----------------------------------
		}
		
		//Now we just setup some texture paramaters.
		glBindTexture( GL_TEXTURE_2D, charTextures[charIndex]);
		
		if (bAntiAlias){
			glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		} else {
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		}
		
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		
		//Here we actually create the texture itself, notice
		//that we are using GL_LUMINANCE_ALPHA to indicate that
		//we are using 2 channel data.
		
		glTexImage2D(	GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, width, height,
						0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, expanded_data );
		
		
		//Clear Expanded Data
		delete [] expanded_data;
	}
	
	bFaceLoaded = true;
}



//--------------------------------------------------------------
void ofxFreeType2::setResolution(int _resolution) {
	resolution = _resolution;
}




//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//Bitmap Drawing


//--------------------------------------------------------------
void ofxFreeType2::drawChar(int c) {
	int cu = c;
	
	GLint width		= chars[cu].texWidth;
	GLint height	= chars[cu].texHeight;
	
	GLfloat	x1, y1, x2, y2;
	GLfloat tx1, ty1, tx2, ty2;
	
	if (glIsTexture(charTextures[cu])) {
		glPushMatrix();
		glTranslatef(0, -chars[cu].top, 0);
		
		//Vert Coords
		x1		= 0;
		y1		= 0;
		x2		= width;
		y2		= height;
		
		//Tex Coords
		tx1		= 0; 
		ty1		= 0;
		tx2		= chars[cu].texXDiff;
		ty2		= chars[cu].texYDiff;
		
		
		glBindTexture(GL_TEXTURE_2D, charTextures[cu]);
		glNormal3f(0, 0, 1);
		
		GLfloat verts[] = { x1,y1,
							x1, y2,
							x2, y2,
							x2, y1 };
		
		
		
		GLfloat tex_coords[] = { tx1, ty1,
								 tx1, ty2,
								 tx2, ty2,
								 tx2, ty1
		};
		
		glEnableClientState( GL_TEXTURE_COORD_ARRAY );
		glTexCoordPointer(2, GL_FLOAT, 0, tex_coords );
		glEnableClientState( GL_VERTEX_ARRAY );
		glVertexPointer(2, GL_FLOAT, 0, verts );
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4 );
		glDisableClientState( GL_TEXTURE_COORD_ARRAY );	
		
		glPopMatrix();
	}
}



//--------------------------------------------------------------
void ofxFreeType2::drawString(string s, int x, int y) {
	//Check to see that the font has been loaded!
	if (!bFaceLoaded){
    	ofLog(OF_LOG_ERROR,"Error : font not allocated, please load font first!");
    	return;
    };
	
	
	
	// Record the current "alpha state, blend func, etc"
	#ifndef TARGET_OF_IPHONE
		glPushAttrib(GL_COLOR_BUFFER_BIT);
	#else
		GLboolean blend_enabled = glIsEnabled(GL_BLEND);
		GLboolean texture_2d_enabled = glIsEnabled(GL_TEXTURE_2D);
		GLint blend_src, blend_dst;
		glGetIntegerv( GL_BLEND_SRC, &blend_src );
		glGetIntegerv( GL_BLEND_DST, &blend_dst );
	#endif
	
	
	// (b) enable our regular ALPHA blending!
    glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// (c) enable texture once before we start drawing each char (no point turning it on and off constantly)
	glEnable(GL_TEXTURE_2D);
	// (d) store the current matrix position and do a translation to the drawing position
	
	
	
	//Draw the String
	GLint		index	= 0;
	GLfloat		xPos	= x;
	GLfloat		yPos	= y;
	
	//Add Line height to draw string from top left and not baseline
	yPos += getMaxCharHeight(s,true);
	
	bool firstChar		= true;
	
	int stringLen = (int)s.length();
	
	for(int i=0; i < stringLen; i++) {
		int c	= (unsigned char)s[i];
		
		//If it is a new line, move Cursor
		if(c == '\n') {
			xPos = x;
			yPos += lineHeight;
			firstChar = true;
		} else {
			c -= START_CHAR;
			//Move to bitmap left pos if not the first char in a line
			if(!firstChar || stringLen == 1) xPos += chars[c].left;
			
			//Draw Char
			glPushMatrix();
			glTranslatef(xPos, yPos, 0);
				ofFill();
				drawChar(c);
			glPopMatrix();
			
			//Add advance spacing, reset first char
			xPos += chars[c].advance - chars[c].left;
			
			firstChar	= false;
		}
	}
	
	
	
	
	glDisable(GL_TEXTURE_2D);
    // (c) return back to the way things were (with blending, blend func, etc)
	#ifndef TARGET_OF_IPHONE
		glPopAttrib();
	#else
		if( !blend_enabled )
			glDisable(GL_BLEND);
		if( !texture_2d_enabled )
			glDisable(GL_TEXTURE_2D);
		glBlendFunc( blend_src, blend_dst );
	#endif
	
}





//--------------------------------------------------------------
void ofxFreeType2::drawTextArea(string s, int x, int y, int width, int height, ofTextAlignment alignment, bool bHideOverflow, bool bDrawOutlines) {
	//Check to see that the font has been loaded!
	if (!bFaceLoaded){
    	ofLog(OF_LOG_ERROR,"Error : font not allocated, please load font first!");
    	return;
    };
	
	if (bDrawOutlines && !bMakeContours){
		ofLog(OF_LOG_ERROR,"Error : contours not created for this font - call loadFont with makeContours set to true");
		return;
	}
	
	//Keep Track of Cursor Position
	int xPos = 0;
	int yPos = 0;
	
	//Record Lines
	vector <string> lines = getTextAreaLines(s,width);
	
	string	formattedText = "";
	
	switch (alignment) {
		case OF_TEXT_ALIGN_LEFT:
			//Add all lines together, seperate with '\n'
			for(int i=0; i<lines.size(); i++) {
				//Check for overflow, if we are hiding it we are finished
				if(bHideOverflow && yPos + lineHeight > height) {
					break;
				}
				
				//Add Text
				formattedText += lines[i];
				
				//Advance line
				if(i!=lines.size()-1) {
					formattedText += '\n';
					yPos += lineHeight;
				}
			}
			
			//Draw the line as string
			if(bDrawOutlines) {
				drawStringAsShapes(formattedText, x, y);
			} else {
				drawString(formattedText, x, y);
			}
			
			break;
		case OF_TEXT_ALIGN_CENTER:
			//Draw each line individually, aligning each individually
			yPos = y;
			
			for(int i=0; i<lines.size();i++) {
				//Check for overflow, if we are hiding it we are finished
				if(bHideOverflow && yPos - y + lineHeight > height) {
					break;
				}
				
				xPos = x+ width/2 - getStringWidth(lines[i])/2;
				if(bDrawOutlines) {
					drawStringAsShapes(lines[i], xPos, yPos);
				} else {
					drawString(lines[i], xPos, yPos);
				}
				yPos += lineHeight;
			}
			break;
		case OF_TEXT_ALIGN_RIGHT:
			//Draw each line individually, aligning each individually
			yPos = y;
			
			for(int i=0; i<lines.size();i++) {
				//Check for overflow, if we are hiding it we are finished
				if(bHideOverflow && yPos - y + lineHeight > height) {
					break;
				}
				
				xPos = x + width - getStringWidth(lines[i]);
				
				if(bDrawOutlines) {
					drawStringAsShapes(lines[i], xPos, yPos);
				} else {
					drawString(lines[i], xPos, yPos);
				}
				
				yPos += lineHeight;
			}
		default:
			break;
	}
}





//--------------------------------------------------------------
vector <string> ofxFreeType2::getTextAreaLines(string s, int width) {
	//Record Lines
	vector <string> lines;
	string curLine = "";
	
	//Cursor Pos
	int xPos = 0;
	
	int stringLen = (int)s.length();
	
	
	bool bFirstChar = true;
	
	for(int i=0; i<stringLen; i++) {
		int c	= (unsigned char)s[i];
		int cu	= c - START_CHAR;
		
		//If first char we ignore the left space
		if(!bFirstChar) xPos += chars[cu].left;
		
		xPos += chars[cu].advance;
		
		bool bNewLine = false;
		
		//Check for new line or line break, otherwise add char to string
		
		if(xPos > width || c == '\n') { //We need a new line!
			//If it isn't a new line and it has a space, push current word to next line
			if(curLine.find(' ')!=string::npos && c != '\n') { 
				int startOfLastWord = curLine.find_last_of(' ');
				i -= curLine.length() - startOfLastWord; 
				curLine = curLine.substr(0, startOfLastWord);
			} else {
				//Ignore blank spaces
				if(c != '\n')  {
					//Make sure we don't keep pushing 1 char to the next line
					//if the text box gets utra narrow,or endless loop of death!
					if(curLine.length() > 0) {
						i--;
					} else {
						curLine += c;
					}
				}
			}
			
			//Add Line
			lines.push_back(curLine);
			
			//Reset Vars and advance Y
			xPos = 0;
			bFirstChar = true;
			curLine = "";
		} else { //We're fine, add the letter to the line
			curLine += c;
		}
	}
	
	//Add the last line
	if(curLine != "") {
		lines.push_back(curLine);
	}
	
	
	
	return lines;
}



//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//Outline Drawing

//-----------------------------------------------------------
void ofxFreeType2::drawCharAsShape(int c) {
	//Make sure we're not out of bounds wtih loaded characters
	if (c >= nChars){
		ofLog(OF_LOG_ERROR,"Error : char (%i) not allocated -- line %d in %s", (c - START_CHAR), __LINE__,__FILE__);
		return;
	}
	
	ofTTCharacter & charRef = charOutlines[c];
	
	glPushMatrix();
	//glTranslatef(0, -chars[c].top, 0);
	
	ofBeginShape();
	for(int k = 0; k < (int)charRef.contours.size(); k++){
		if( k!= 0)ofNextContour(true);
		for(int i = 0; i < (int)charRef.contours[k].pts.size(); i++){
			ofVertex(charRef.contours[k].pts[i].x, charRef.contours[k].pts[i].y);
		}
	}
	ofEndShape( true );
	glPopMatrix();
}



//-----------------------------------------------------------
void ofxFreeType2::drawStringAsShapes(string s, float x, float y) {
	//Make sure the font was loaded and Contours were created
	if (!bFaceLoaded){
    	ofLog(OF_LOG_ERROR,"Error : font not allocated -- line %d in %s", __LINE__,__FILE__);
    	return;
    };
	
	
	if (!bMakeContours){
		ofLog(OF_LOG_ERROR,"Error : contours not created for this font - call loadFont with makeContours set to true");
		return;
	}
	
	GLint		index	= 0;
	GLfloat		xPos	= x;
	GLfloat		yPos	= y;
	
	//Add Line height to draw string from top left and not baseline
	yPos += getMaxCharHeight(s,true);
	
	bool firstChar		= true;
	int stringLen = (int)s.length();
	
	
	for(int i=0; i < stringLen; i++) {
		int c	= (unsigned char)s[i];
		
		//If it is a new line, move Cursor
		if(c == '\n') {
			xPos = x;
			yPos += lineHeight;
			firstChar = true;
		} else {
			c -= START_CHAR;
			//Move to bitmap left pos if not the first char in a line
			//if(!firstChar) xPos += chars[cy].left;
			
			//Draw Char
			glPushMatrix();
			glTranslatef(xPos, yPos, 0);
			ofFill();
			drawCharAsShape(c);
			glPopMatrix();
			
			//Add advance spacing, reset first char
			xPos += chars[c].advance;
			
			firstChar	= false;
		}
	}
}


//--------------------------------------------------------------
void ofxFreeType2::drawTextAreaAsShapes(string s, int x, int y, int width, int height, ofTextAlignment alignment, bool bHideOverflow) {
	drawTextArea(s,x,y,width,height,alignment,bHideOverflow,true);
}





//--------------------------------------------------------------
//--------------------------------------------------------------
//String Info/Formatting


//--------------------------------------------------------------
int ofxFreeType2::getLineHeight() {
	return lineHeight;
}



//--------------------------------------------------------------
void ofxFreeType2::setLineHeight(float height) {
	lineHeight = height;
}



//--------------------------------------------------------------
int ofxFreeType2::getStringWidth(string s) {
	ofRectangle boundingBox = getStringBoundingBox(s, 0, 0);
	
	return boundingBox.width;
}


//--------------------------------------------------------------
int ofxFreeType2::getStringHeight(string s) {
	ofRectangle boundingBox = getStringBoundingBox(s, 0, 0);
	
	return boundingBox.height;
}

//--------------------------------------------------------------
int ofxFreeType2::getStringHeight(string s, int width) {
	vector <string> lines = getTextAreaLines(s,width);
	
	return (lineHeight * lines.size());
}



//--------------------------------------------------------------
ofRectangle ofxFreeType2::getStringBoundingBox(string s, float x, float y) {
	ofRectangle boundingBox;
	boundingBox.x		= x;
	boundingBox.y		= y;
	boundingBox.width	= 0;
	boundingBox.height	= getMaxCharHeight(s);
	
	float curWidth = 0;
	
	int stringLen = (int)s.length();
	bool bFirstChar = true;
	
	for(int i=0; i<stringLen; i++) {
		int c = (unsigned char)s[i];
		
		//Is this a new line? If so see if the previous line is the longest
		if(c == '\n') {
			boundingBox.width = max(boundingBox.width,curWidth);
			curWidth = 0;
			
			boundingBox.height += lineHeight;
			
			continue;
		} else {
			c -= START_CHAR;
		}
		
		//Add the width of the char
		if(!bFirstChar) curWidth += chars[c].left;
		
		//Check to see if next char is a line return,
		unsigned char nextChar =  ' ';
		if(i!=stringLen-1) {
			nextChar = (unsigned char)s[i+1];
		} 
		
		
		if(nextChar=='\n' || i==stringLen-1) {
			//if(chars[cy].texWidth < chars[cy].advance) {
			//	curWidth += chars[cy].advance;
			//} else {
				curWidth += chars[c].advance;
			//}
		} else {
			curWidth += chars[c].advance;
		}
	}
	
	boundingBox.width = max(boundingBox.width,curWidth);
	return boundingBox;
}




//--------------------------------------------------------------
int ofxFreeType2::getMaxCharHeight(string s, bool bFromBaseline) {
	float maxTop	  = 0;
	float minBottom = 0;
	
	float maxHeight = 0;
	
	int stringLen = (int)s.length();
	for(int i=0; i<stringLen; i++) {
		int thisChar = (unsigned char)s[i];
		
		if(thisChar == '\n')  {
			break;
		} else {
			thisChar -= START_CHAR;
			maxTop		= max(chars[thisChar].top,maxTop);
			minBottom	= min(chars[thisChar].bottom, minBottom);
		}
	}
	
	
	if(bFromBaseline) {
		maxHeight = maxTop;
	} else {
		maxHeight = maxTop - minBottom;
	}
	
	return maxHeight;
}


