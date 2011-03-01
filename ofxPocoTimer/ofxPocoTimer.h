/*
 *  ofxPocoTimer.h
 *  openFrameworks
 *
 *  Created by StephenVarga on 11/24/09.
 *  Copyright 2009. All rights reserved.
 *	
 *  Based on code posted by Bernard on OF Forums
 *  http://www.openframeworks.cc/forum/viewtopic.php?f=8&t=2440&hilit=audio+timing
 */
#ifndef _ofxPocoTimer
#define _ofxPocoTimer


#include "ofMain.h"

#include "Poco/Stopwatch.h"
#include "Poco/Thread.h"
#include "Poco/Timestamp.h"
#include "Poco/Timer.h"

using Poco::Stopwatch;
using Poco::Thread;
using Poco::Timer;
using Poco::TimerCallback;

//Class to call the stopwatch and hold times, not sure if it needs to be seperate but makes it a bit cleaner
class seqTimer {
	public:
		seqTimer() {
			tSecs=0;
			tMillis=0;
			
			stopwatch.start();
		}
		
		void onTimer(Timer& timer) {
			tMicros=stopwatch.elapsed();
			tMillis=tMicros/1000.0f;
			tSecs=tMillis/1000.0f;
			//void seqTimerFunc(Poco::Timestamp::TimeDiff curTime); // prototype of my function
			//seqTimerFunc(stopwatch.elapsed()); // function call
		}
	
		float tSecs;
		float tMillis;
		float tMicros;
	
	private:
		static void setTime(int tMicros);
		
		Stopwatch stopwatch;
		
};


//Main class, instantiates timer class and has calls to grab times
class ofxPocoTimer {
	public:
		ofxPocoTimer();
	
		float getElapsedTimef();
		float getElapsedTimeMillis();
		float getElapsedTimeMicros();
		
		seqTimer sTimer;
		Timer * timer;
};



#endif
