#pragma once
#include "Measurement.h"
#include <string>
using namespace std;

template<class Key, class Value>
class Map: public Measurement
{
public:
	Map() { draw_disabled = false; }
	virtual void  put(Key k, Value v)=0;
	virtual Value get(Key k)=0;
	virtual void  remove(Key key)=0;
	virtual int   size() = 0;
	virtual string drawTree(string text, Key selectedKey, void* deficiencyParent=NULL, char deficiencyDirection='-')=0;
	
	bool draw_disabled;
};

