#include <iostream>
#include <string>
#include "node.hpp"
#include "printTree.hpp"
#include <stdlib.h>
#include <time.h>

using namespace std;

int main()
{
	splayTree splay;
	time_t curTime = time(nullptr);
	srandom(curTime);

/*
	cout << "Inserting : " << "50" << endl;
	splay.insert(50);
	printTree splaypt(splay.getRoot());

	cout << "Inserting : " << "60" << endl;
	splay.insert(60);
	splaypt.print(splay.getRoot());

	cout << "Inserting : " << "40" << endl;
	splay.insert(40);
	splaypt.print(splay.getRoot());

	cout << "Inserting : " << "30" << endl;
	splay.insert(30);
	splaypt.print(splay.getRoot());

	cout << "Inserting : " << "20" << endl;
	splay.insert(20);
	splaypt.print(splay.getRoot());

	cout << "Inserting : " << "35" << endl;
	splay.insert(35);
	splaypt.print(splay.getRoot());

	cout << "Inserting : " << "45" << endl;
	splay.insert(45);
	splaypt.print(splay.getRoot());

	cout << "Inserting : " << "37" << endl;
	splay.insert(37);
	splaypt.print(splay.getRoot());
	return 0;

*/

	printTree splaypt(splay.getRoot());
	for (int i = 0; i < 50; ++i) {
		auto data  = random() % 100;
//		cout << "Now inserting : " << data << endl;
		splay.insert(data);
		splaypt.print(splay.getRoot());
		splay.verifyBST(splay.getRoot());
	}

	for (int i = 0; i < 50; ++i) {
		auto data  = random() % 100;
		cout << "Now deleting : " << data << endl;
		splaypt.print(splay.getRoot());
		splay.remove(data);
		cout << "Tree after deleting : " << data << endl;
		splaypt.print(splay.getRoot());
		splay.verifyBST(splay.getRoot());
	}

//	splay.insert(50);
//	splay.insert(60);
//	splay.insert(40);
//	splay.insert(30);
//	splay.insert(20);
//	splay.insert(35);
//	splay.insert(37);
//	splay.insert(36);
//	splay.insert(70);
//	printTree splaypt(splay.getRoot());
//	splaypt.print(splay.getRoot());

	return 0;
}
