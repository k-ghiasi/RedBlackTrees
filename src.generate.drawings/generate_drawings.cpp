/*
Authros: 
		Sayed Kamaledin Ghiasi-Shirazi
		Taraneh Ghandi
		Ali Rahimi-Baigi
		Ali Taghizadeh
*/

#include <iostream>
#include <map>
#include <vector>
#include <ctime>
#include <string>
#include <assert.h>
#include <chrono>
#include <algorithm>
#include <fstream>

#include "ClassicalRedBlackBST.h"
#include "LeftLeaningRedBlackBST.h"
#include "ParitySeeking-23RedBlackBST.h"
#include "ParitySeeking-234RedBlackBST.h"
#include "PureParitySeeking-23RedBlackBST.h"
#include "PureParitySeeking-234RedBlackBST.h"

typedef ClassicalRedBlackBST<int, int> RbBst;
typedef LeftLeaningRedBlackBST<int, int> llRbBst;
typedef ParitySeeking23RedBlackBST<int, int> Ps23RbBst;
typedef ParitySeeking234RedBlackBST<int, int> Ps234RbBst;
typedef PureParitySeeking23RedBlackBST<int, int> Pps23RbBst;
typedef PureParitySeeking234RedBlackBST<int, int> Pps234RbBst;

using namespace std;

void testInsertionTime (Map<int,int>* rbt, int *arrayInsert, int length) {
    rbt->resetCounters();

    for (int i = 0; i < length; i++) {
        rbt->put(arrayInsert[i], arrayInsert[i]);
    }
}


void testDeletionTime(Map<int,int> *rbt, int *arrayDelete, int length) 
{
    rbt->resetCounters();
    for (int i = 0; i < length; i++) {
        rbt->remove(arrayDelete[i]);
    }
}


void test(int length, vector<pair<string, Map<int, int>* > >& rbtList
		  ,int* arrayInsert, int* arrayDelete) {
	
	for (int i = 0; i < rbtList.size(); i++) {
		rbtList[i].second->draw_disabled = false;
		testInsertionTime(rbtList[i].second, arrayInsert, length);
	}
	
	
	for (int i = 0; i < rbtList.size(); i++) {
		rbtList[i].second->draw_disabled = true;
		testDeletionTime(rbtList[i].second, arrayDelete, length);
	}
}

int main(int argc, char* argv[]) {
    srand(0);

	/*
	if (argc != 4){
		cerr << "usage: redBlackTrees  alg_num  number_of_data  repeats";
		return -1;
	}
		
	int alg_num = atoi(argv[1]);
	long int n   = atoi(argv[2]);
	int rep = atoi(argv[3]);
	*/
	int alg_num = 5;
	int n = 30;
	int rep = 1;
	
	vector < pair<string, Map<int, int>*>> rbtList;
	switch (alg_num){
		case 0:
			rbtList.push_back(pair<string, Map<int, int>*>(string("clrs"), new RbBst()));
			break;
		case 1:
			rbtList.push_back(pair<string, Map<int, int>*>(string("llrbt"), new llRbBst()));
			break;
		case 2:
			rbtList.push_back(pair<string, Map<int, int>*>(string("ps23rbt"), new Ps23RbBst()));
			break;
		case 3:
			rbtList.push_back(pair<string, Map<int, int>*>(string("ps234rbt"), new Ps234RbBst()));
			break;
		case 4:
			rbtList.push_back(pair<string, Map<int, int>*>(string("pps23rbt"), new Pps23RbBst()));
			break;
		case 5:
			rbtList.push_back(pair<string, Map<int, int>*>(string("pps234rbt"), new Pps234RbBst()));
			break;
		default:
			assert(0);
	}
	cout << endl << "n = " << n << endl;

	int *arrayInsert = new int[n];
	int *arrayDelete = new int[n];

	for (long i = 0; i < n; i++) {
		arrayInsert[i] = i;
		arrayDelete[i] = i;
	}

	for (int i = 0; i < rep; i++) {
		std::random_shuffle(arrayInsert, arrayInsert + n);
		std::random_shuffle(arrayDelete, arrayDelete + n);

		test(n, rbtList, arrayInsert, arrayDelete);
	}

	delete[] arrayInsert;
	delete[] arrayDelete;


	for (int i = 0; i < rbtList.size(); i++)
		delete rbtList[i].second;

    return 0;
}
