/*
 *  ofxFileLoader.cpp
 *  openFrameworks
 *
 *  Created by StephenVarga on 9/25/09.
 *  Copyright 2009. All rights reserved.
 *
 */

#include "ofxFileLoader.h"

bool bFactoryLoaded=false;

//------------------------------------------------------------------
string ofxFileLoader::loadFromUrl(string _url) {
	//Poco code based on code from Zach and Theo on OF Forums
	//Register the factory, poco is not happy if we register the factory more than once
	if(!bFactoryLoaded){
		HTTPStreamFactory::registerFactory();
		bFactoryLoaded = true;
	}
	
	try {
		//Specify URL/Open Stream
		URI uri(_url);      
		std::auto_ptr<std::istream> pStr(URIStreamOpener::defaultOpener().open(uri));
		
		//Load URL into String
		string str;      
		StreamCopier::copyToString(*pStr.get(), str);
		
		return str;
	} catch (...) {
		return "Couldn't Load File!";
	}
	
	
}



//------------------------------------------------------------------
bool ofxFileLoader::loadFromUrl(string _url,string filename) {
	//Poco code from theo on OF forums
	//Register the factory, poco is not happy if we register the factory more than once
	if(!bFactoryLoaded){
		HTTPStreamFactory::registerFactory();
		bFactoryLoaded = true;
	}
	
	try {
		//specify out url and open stream
		URI uri(_url);
		
		std::auto_ptr<std::istream> pStr(URIStreamOpener::defaultOpener().open(uri));
		
		
		//create a file to save the stream to
		string fileLoc = ofToDataPath("") + filename;
		ofstream myfile;      
		myfile.open(ofToDataPath(fileLoc).c_str());
		
		//copy stream to file
		StreamCopier::copyStream(*pStr.get(), myfile);
		
		myfile.close();
		
		return true;
	} catch (...) {
		return false;
	}
}


//------------------------------------------------------------------
string ofxFileLoader::loadFromUrlandSave(string _url,string filename) {
	//Poco code from theo on OF forums
	//Register the factory, poco is not happy if we register the factory more than once
	if(!bFactoryLoaded){
		HTTPStreamFactory::registerFactory();
		bFactoryLoaded = true;
	}
	
	try {
		//specify out url and open stream
		URI uri(_url);
		
		std::auto_ptr<std::istream> pStr(URIStreamOpener::defaultOpener().open(uri));
		
		
		//create a file to save the stream to
		ofstream myfile;     
		myfile.open(ofToDataPath(filename).c_str());
		
		//copy stream to file
		StreamCopier::copyStream(*pStr.get(), myfile);
		
		myfile.close();
		
		
		string str;      
		StreamCopier::copyToString(*pStr.get(), str);
		
		return str;
	} catch (...) {
		return false;
	}
}


