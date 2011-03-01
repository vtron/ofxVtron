/*
 *  ofxFileLoader.h
 *  openFrameworks
 *
 *  Created by StephenVarga on 9/25/09.
 *  Copyright 2009. All rights reserved.
 *
 */
#ifndef _FILE_LOADER
#define _FILE_LOADER

#include <iostream>
#include <fstream>

#include "Poco/Net/HTTPStreamFactory.h"
#include "Poco/URIStreamOpener.h"
#include "Poco/URI.h"
#include "Poco/StreamCopier.h"
#include "Poco/Exception.h"
#include "Poco/Path.h"


using Poco::Net::HTTPStreamFactory;
using Poco::URIStreamOpener;
using Poco::URI;
using Poco::StreamCopier;
using Poco::Exception;
using Poco::Path;


//#include "ofxFileLoaderGlobals.h"


#include "ofMain.h"


//bool bFactoryLoaded;

//Class
class ofxFileLoader {
	
	public:
		string loadFromUrl(string _url);
		string loadFromUrlandSave(string _url,string filename);
		bool loadFromUrl(string _url,string filename);
};

#endif
