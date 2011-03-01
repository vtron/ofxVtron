/*
 *  ofxPocoTimer.cpp
 *  openFrameworks
 *
 *  Created by StephenVarga on 11/24/09.
 *  Copyright 2009. All rights reserved.
 *
 *  Based on code posted by Bernard on OF Forums
 *  http://www.openframeworks.cc/forum/viewtopic.php?f=8&t=2440&hilit=audio+timing
 */

#include "ofxPocoTimer.h"

Poco::Timestamp::TimeDiff hdelay = 125000; // delay between 2 events, in microseconds
Poco::Timestamp::TimeDiff nextTime = 0;	 // time when next event occurs (calculated in seqTimerFunc)

//--------------------------------------------------------------
ofxPocoTimer::ofxPocoTimer() {
	timer = new Timer(0, 10); // parameters : immediate and delay of 10 milliseconds (fast enough i think)
	timer->start(TimerCallback<seqTimer>(sTimer, &seqTimer::onTimer), Thread::PRIO_HIGHEST);
}


//--------------------------------------------------------------
float ofxPocoTimer::getElapsedTimef() {
	return sTimer.tSecs;
}

//--------------------------------------------------------------
float ofxPocoTimer::getElapsedTimeMillis() {
	return sTimer.tMillis;
}

//--------------------------------------------------------------
float ofxPocoTimer::getElapsedTimeMicros() {
	return sTimer.tMicros;
}

/*
//--------------------------------------------------------------
void seqTimerFunc(Poco::Timestamp::TimeDiff curTime);
void seqTimerFunc(Poco::Timestamp::TimeDiff curTime) { // Events that are regularely sent are processed here
	if(curTime >= nextTime) {
		nextTime += hdelay;
		//cout << (long)(curTime / 1000) << " .. " << (long)(nextTime / 1000) << endl;
		// Execute real messages (midi etc) here ...
		//setTime(curTime);
	}
}*/
