/*
 *  ofxVoronoi.cpp
 *  openFrameworks
 *
 *  Created by StephenVarga on 9/27/09.
 *  Copyright 2009. All rights reserved.
 *
 */

#include "ofxVoronoi.h"

//------------------------------------------------------------------
void ofxVoronoi::createLines(vector <ofPoint> points, int minX, int maxX, int minY, int maxY, int minDist) {
	vector <int> xPoints;
	vector <int> yPoints;
	
	for(int i=0; i<points.size();i++) {
		xPoints.push_back(points[i].x);
		yPoints.push_back(points[i].y);
	}
	
	createLines(xPoints,yPoints,minX,maxX,minY,maxY,minDist);
}

//------------------------------------------------------------------
void ofxVoronoi::createLines(vector <int> xPoints, vector <int> yPoints, int minX, int maxX, int minY, int maxY, int minDist) {
	lines.clear();
	
	//Make sure there are as many xPoints as yPoints, otherwise it won't work
	if(xPoints.size()!=yPoints.size()) {
		cout<<"Different number of xPoints and yPoints. Can not continue."<<endl;
		return;
	}
	
	//Generate the Vornoi Points
	voronoi.generateVoronoi(xPoints, yPoints, minX, maxX, minY, maxY, minDist);
	
	voronoi.resetIterator();
	
	float x1,y1,x2,y2;
	
	while(voronoi.getNext(x1,y1,x2,y2)) {
		line l;
		l.x1=x1;
		l.x2=x2;
		l.y1=y1;
		l.y2=y2;
		
		lines.push_back(l);
	}
}



