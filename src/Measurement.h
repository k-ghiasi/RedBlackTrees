#pragma once

class Measurement
{
private:
	long long int mRightRotations;
	long long int mLeftRotations;
	long long int mConditionals;
	long long int mTopDownVisit;
	long long int mBottomUpVisit;

public:
	Measurement() {};

	void resetCounters()
	{
		mRightRotations = 0;
		mLeftRotations = 0;
		mConditionals = 0;
		mTopDownVisit = 0;
		mBottomUpVisit = 0;
	}

	long long int getRightRotations(){ 
		return mRightRotations; 
	}

	void incRightRotations() {
		mRightRotations++;
	}

	long long int getLeftRotations(){ 
		return mLeftRotations; 
	}

	void incLeftRotations(){
		mLeftRotations++;
	}

	long long int getTopDownVisit() {
		return mTopDownVisit;
	}

	void incTopDownVisit() {
		mTopDownVisit++;
	}

	long long int getBottomUpVisit() {
		return mBottomUpVisit;
	}

	void incBottomUpVisit() {
		mBottomUpVisit++;
	}
};
