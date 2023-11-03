/*
Authros: 
		Ali Rahimi-Baigi
		Ali Taghizadeh
		Taraneh Ghandi		
		Sayed Kamaledin Ghiasi-Shirazi		
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

#include "../src/ClassicalRedBlackBST.h"
#include "../src/LeftLeaningRedBlackBST.h"
#include "../src/ParitySeeking-23RedBlackBST.h"
#include "../src/ParitySeeking-234RedBlackBST.h"

typedef ClassicalRedBlackBST<int, int> RbBst;
typedef LeftLeaningRedBlackBST<int, int> llRbBst;
typedef ParitySeeking23RedBlackBST<int, int> Ps23RbBst;
typedef ParitySeeking234RedBlackBST<int, int> Ps234RbBst;



using namespace std;

void checkInorder(Map<int,int>& rbt, map<int, int> &m) {
    map<int, int>::iterator mapIt;
	int size1 = rbt.size();
	int size2 = m.size();
	assert(size1 == size2);
        for (mapIt = m.begin(); mapIt != m.end() ; mapIt++) {
			int key = mapIt->first;
			int true_value = mapIt->second;
            int rbt_value = rbt.get(key);
            assert(true_value == rbt_value);
        }
    }

void testInsertionTime (Map<int,int>* rbt, int *arrayInsert, int length, fstream &fout) {
    rbt->resetCounters();

    auto start = std::chrono::system_clock::now();

    for (int i = 0; i < length; i++) {
        rbt->put(arrayInsert[i], arrayInsert[i]);
    }
    auto end = std::chrono::system_clock::now();

    std::chrono::duration<double> elapsed_seconds = end - start;

    fout << rbt->getLeftRotations() << ", "
         << rbt->getRightRotations() << ", "
         << elapsed_seconds.count() << ", ";
}


void testDeletionTime(Map<int,int> *rbt, int *arrayDelete, int length, fstream &fout) 
{
	// Check correct handling of removal of non-existant keys.
	rbt->remove(length);
	rbt->remove(-1);

    rbt->resetCounters();
    auto start = std::chrono::system_clock::now();

    for (int i = 0; i < length; i++) {
        rbt->remove(arrayDelete[i]);
    }
    auto end = std::chrono::system_clock::now();

    std::chrono::duration<double> elapsed_seconds = end - start;

	fout << rbt->getLeftRotations() << ", "
		<< rbt->getRightRotations() << ", "
		<< elapsed_seconds.count();
}


void test(int length, vector<pair<string, Map<int, int>* > >& rbtList
		  ,int* arrayInsert, int* arrayDelete
		  ,fstream& fout) {

	map<int, int>*m = new map<int, int>[rbtList.size()];
	for (int j = 0; j < rbtList.size(); j++) {
		for (long i = 0; i < length; i++) {
			m[j][arrayInsert[i]] = arrayInsert[i];
			rbtList[j].second->put(arrayInsert[i], arrayInsert[i]);
			checkInorder(*rbtList[j].second, m[j]);
		}
	}

	for (int j = 0; j < rbtList.size(); j++) {
		for (long i = 0; i < length; i++) {
			m[j].erase(arrayInsert[i]);
			rbtList[j].second->remove(arrayInsert[i]);
			checkInorder(*rbtList[j].second, m[j]);
		}
	}


	fout << length << ", ";
	for (int i = 0; i < rbtList.size(); i++) {
		testInsertionTime(rbtList[i].second, arrayInsert, length, fout);
	}
	
	for (int i = 0; i < rbtList.size(); i++) {
		testDeletionTime(rbtList[i].second, arrayDelete, length, fout);
		if (i < rbtList.size() - 1)
			fout << ", ";
	}
	fout << "\n";
}

int main(int argc, char* argv[]) {
    srand(0);

	if (argc != 4){
		cerr << "usage: redBlackTrees  alg_num  number_of_data  repeats";
		return -1;
	}
		
	int alg_num = atoi(argv[1]);
	long int n   = atoi(argv[2]);
	int rep = atoi(argv[3]);
	
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
		default:
			assert(0);
	}
	cout << endl << "n = " << n << endl;

	string fileName = rbtList[0].first + "_" + to_string(n) + ".csv";
	fstream fout;
	fout.open(fileName, ios::out);
	fout << "n";
	for (int k = 0; k < 2; k++)
		for (int i = 0; i < rbtList.size(); i++) {
			for (int j = 0; j < 3; j++) {
				fout << "," << rbtList[i].first;
			}
		}
	fout << "\n";

	fout << "n";
	for (int i = 0; i < 2 * rbtList.size(); i++) {
		fout << ", LR, RR, time";
	}
	fout << "\n";

	int *arrayInsert = new int[n];
	int *arrayDelete = new int[n];

	for (long i = 0; i < n; i++) {
		arrayInsert[i] = i;
		arrayDelete[i] = i;
	}

	for (int i = 0; i < rep; i++) {
		std::random_shuffle(arrayInsert, arrayInsert + n);
		std::random_shuffle(arrayDelete, arrayDelete + n);

		test(n, rbtList, arrayInsert, arrayDelete, fout);
	}

	delete[] arrayInsert;
	delete[] arrayDelete;


	fout.close();
	for (int i = 0; i < rbtList.size(); i++)
		delete rbtList[i].second;

    return 0;
}