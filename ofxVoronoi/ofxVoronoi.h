/*
 *  ofxVoronoi.h
 *  openFrameworks
 *
 *  Created by StephenVarga on 9/27/09.
 *  Copyright 2009. All rights reserved.
 *
 */
#ifndef _OFX_VORONOI
#define _OFX_VORONOI


#include "ofMain.h"
#include "VoronoiDiagramGenerator.h"

typedef struct {
	int x1;
	int y1;
	int x2;
	int y2;
} line;

class ofxVoronoi {
	
	public:
		
		void createLines(vector <ofPoint> points, int minX, int maxX, int minY, int maxY, int minDist=0);
		void createLines(vector <int> xPoints, vector <int> yPoints, int minX, int maxX, int minY, int maxY, int minDist=0);
	
		VoronoiDiagramGenerator voronoi;
	
		vector <line> lines;
};

#endif
