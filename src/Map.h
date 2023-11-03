#pragma once
#include "Measurement.h"

template<class Key, class Value>
class Map: public Measurement
{
public:
	virtual void  put(Key k, Value v)=0;
	virtual Value get(Key k)=0;
	virtual void  remove(Key key)=0;
	virtual int   size() = 0;
};

