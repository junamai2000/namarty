#ifndef __NAMARTY_CORE_H
#define __NAMAERY_CORE_H
#include <string.h>
#include <stdlib.h>

#include <iostream>
#include <string>
#include <map>
#include <list>
#include <stack>

using namespace std;

class forInfo {
 private:
	bool _forReadBlock;

 public:
	enum ELSE_STATUS {NONE=0, DONE, YET} _else;
	long _forCounter;
	bool _break;
	bool _followStatus;

	forInfo() {
		_forCounter = 0;
		_else = forInfo::NONE;
		_forReadBlock = false;
		_break = false;
		_followStatus = false;
	}

	bool isFirst() {
		return !_forCounter;
	}
	void readBlock() {
		_forReadBlock = true;
	}
};

class namarty_core {
 private:
	// staus
	string _output;
	long _readLength;
	long _tmpStackPoint;
	
	// for statement
	stack<forInfo> _forInfo;
	long _forDepth;

	// every statmenst use this valuse
	stack<long> _fpPos;
	stack<bool> _doBuffer;

	// input data
	FILE *_fp;


	// attribute
	map<string, string> _attribute;
	map<string, string>::iterator _aIt;
	
	// the state of the lexer
	void *_yyscanner;


 
 protected:
	bool _has_outputfilter;
	void setForCounter(long cnt);
	long getForCounter();

	virtual bool concreteForeach(const char* from, const char* item) = 0; 

 public:
	// smarty compatible property
	char *template_dir;


	// buffer delete flag
	bool clear_buffer;
	namarty_core();


	// functions for compile
	virtual const char* get(const char* key) = 0;
	virtual const char *through_outputfilter(const char* buffer) = 0;
	void pushCurrentPosition(long offset = 0);
	void setReadByte(long len);
	void setTmpStackPoint();

	// foreach
	void startForeach();
	void endForeach();
	void elseForeach();
	string getDepth(int i=0) {
		char rawBuffer[20];
		snprintf(rawBuffer, 20, "%ld", _forDepth+i);
		return string(rawBuffer);
	};

	// if
	void startIf();
	void startElseIf();
	void startElse();
	void endIf();

	// attribute for every command
	void setAttribute(const char* key, const char* val);
	const char* getAttribute(const char* key);


	// original
	void buffer(const char* str);
	virtual const char* get_template_vars(const char* key) = 0;


	// smarty compatible functions 
	void display(const char* resource_name);
	//void assign(const char* key, const char* val);	
	virtual void assign(const char* key, void* val) = 0;	
	virtual void register_outputfilter(void *func) = 0;

};

#ifdef DEBUG
#define DMSG(str) fprintf(stderr, "%s ",str)
#else
#define DMSG(str)
#endif

#endif
