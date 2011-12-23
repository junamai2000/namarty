#include "namarty_core.h"
#include "parser.namarty.h"

namarty_core::namarty_core()
{
	_has_outputfilter = false;
	_readLength = 0;
	_tmpStackPoint = 0;
	_doBuffer.push(true);
	_forDepth = 0;

	// smarty property
	template_dir = "";
}

void namarty_core::buffer(const char* str)
{
	if (_doBuffer.top()) {
		_output += str;
	}
}

void namarty_core::setReadByte(long len)
{
	_readLength += len;
}

void namarty_core::setTmpStackPoint()
{
	_tmpStackPoint = _readLength;
}

void namarty_core::pushCurrentPosition(long offset)
{
	_fpPos.push(_tmpStackPoint + offset);
}

void namarty_core::setForCounter(long cnt)
{
	if (_forInfo.empty()) {
		cerr << "namarty_core::setForCounter: something is wrong." << endl;
		return;
	}
	_forInfo.top()._forCounter = cnt;
}

long namarty_core::getForCounter()
{
	if (_forInfo.empty()) {
		cerr << "namarty_core::getForCounter: something is wrong." << endl;
		return 0;
	}
	return _forInfo.top()._forCounter;
}

void namarty_core::startForeach()
{
	bool ret = true;
	_forDepth++;

	// at first time
	if (_forInfo.empty() || // first time
			_forInfo.size() != _forDepth || _forInfo.top().isFirst()) {
		forInfo _tmp;
		if (_doBuffer.size() && !_doBuffer.top()) {
			_doBuffer.push(false);
			_tmp._followStatus = true;
		} else {
			_doBuffer.push(true);
		}
		_forInfo.push(_tmp);
	}

	// arguments check
	if (!this->getAttribute(string(getDepth() + string("from")).c_str()) || !this->getAttribute("item")) {
		cerr << "compile error: foreach needs attribute from=, item=" << endl;
		exit(0);
		return;
	}
	this->pushCurrentPosition(-8);
	
	ret = concreteForeach(this->getAttribute(string(getDepth() + string("from")).c_str()), this->getAttribute("item"));
	if (!ret || !_doBuffer.top()) {
		// no loop
		_forInfo.top()._break = true;
		_doBuffer.pop();
		_doBuffer.push(false);
		if (!_fpPos.empty()) {
			_fpPos.pop();
		}
		return;
	}

}

void namarty_core::elseForeach()
{
	if (!_doBuffer.top() && _forInfo.top()._break && getForCounter()==0 && _forInfo.top()._followStatus == false) {
		_forInfo.top()._else = forInfo::DONE;
		_doBuffer.pop();
		_doBuffer.push(true);
	} else {
		_forInfo.top()._else = forInfo::YET;
		_doBuffer.pop();
		_doBuffer.push(false);
	}
}

void namarty_core::endForeach()
{
	_forDepth--;
	if (_forInfo.top()._else == forInfo::YET) {
		_doBuffer.pop();
		_doBuffer.push(true);
	} else if (_forInfo.top()._else == forInfo::DONE) {
		_doBuffer.pop();
		_doBuffer.push(false);
	}

	if (!_doBuffer.top() || _forInfo.top()._break) {
		_forInfo.pop();
		_doBuffer.pop();
		return;
	}

	_forInfo.top().readBlock();

	long startLoopPos = _fpPos.top();
	_fpPos.pop();
	_readLength = startLoopPos;

	fseek(_fp, startLoopPos, SEEK_SET);
	//fprintf(stderr, "endJump:%ld readLength:%ld\n", startLoopPos, _readLength);

	namarty_tplrestart(_fp, _yyscanner);
}
	
void namarty_core::setAttribute(const char* key, const char* val)
{
	_attribute.erase(key);
	_attribute.insert(pair<string, string>(key, val));
}

const char* namarty_core::getAttribute(const char* key)
{
	_aIt=_attribute.find(key);
	if(_aIt != _attribute.end()) {
		return _aIt->second.c_str();
	} else {
		return NULL;
	}
}

void namarty_core::display(const char* resource_name)
{
	string tmpFilename;
	tmpFilename = template_dir;
	tmpFilename += resource_name;
	_fp = fopen(tmpFilename.c_str(), "r");
	
	if (_fp == NULL) {
		cerr << "can not open template file" << endl;
		return;
	}

#ifdef	YYDEBUG
	extern int namarty_tpldebug;
	namarty_tpldebug = 1;
#endif

	namarty_tpllex_init(&_yyscanner);
	namarty_tplset_in(_fp, _yyscanner);
	namarty_tplset_extra(this, _yyscanner);

	if (namarty_tplparse(this, _yyscanner)) {
		cerr << "parse failed" << endl;
		return;
	}
	namarty_tpllex_destroy(_yyscanner);

	if (_has_outputfilter) {
		cout << through_outputfilter(_output.c_str()) << endl;
	} else {
		cout << _output;
	}
	fflush(stdout);
	fclose(_fp);
}

