#pragma once

class Measurement
{
private:
	long long int mRightRotations;
	long long int mLeftRotations;
	long long int mConditionals;

public:
	Measurement() {};

	void resetCounters()
	{
		mRightRotations = 0;
		mLeftRotations = 0;
		mConditionals = 0;
	}

	long long int getRightRotations(){ 
		return mRightRotations; 
	}

	long long int getLeftRotations(){ 
		return mLeftRotations; 
	}

	void incRightRotations(){
		mRightRotations++;
	}

	void incLeftRotations(){
		mLeftRotations++;
	}
};
