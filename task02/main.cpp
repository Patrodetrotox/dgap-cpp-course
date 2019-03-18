#include <iostream>
#include "queue.h"

using namespace std;

int main() {

	//ListBased tests
	ListQueue<int> q1;
	ListQueue<string> q2;
	q1.enqueue(5);
	q1.enqueue(4);
	q1.enqueue(3);
	try {
		cout << q1.dequeue() << endl;
	} catch (range_error& er) {
		cerr << "Error: " << er.what() << endl;
	}

	q2.enqueue("Hello ");
	q2.enqueue("This ");
	q2.enqueue("World!");
	try {
		cout << q2.dequeue() << q2.dequeue() << q2.dequeue() << endl;
		cout << q2.dequeue() << endl;
	} catch(range_error& er) {
		cerr << "Error: " << er.what() << endl;
	}

	//ArrayBased tests

	ArrayQueue<int, 4> qq1;
	ArrayQueue<int> qq2;

	try {
		qq1.enqueue(1);
		qq1.enqueue(2);
		cout << qq1.getLength() << endl;
		qq1.enqueue(3);
		qq1.enqueue(4);
		qq1.enqueue(5);

	} catch(range_error& er) {
		cerr << "Error: " << er.what() << endl;
	}

	try {
		cout << qq1.dequeue() << " " << qq1.dequeue() << " " << qq1.dequeue() << " " << qq1.dequeue() << endl;
		cout << qq1.dequeue() << endl;
	} catch (range_error& er) {
		cerr << "Error: " << er.what() << endl;
	}

	//getLength test

	qq2.enqueue(1);
	qq2.enqueue(2);
	qq2.dequeue();
	qq2.dequeue(); // first = 2
	qq2.enqueue(3);
	qq2.enqueue(4); // last = 0
	cout << qq2.getLength() << endl;  //should return 2
    return 0;
}
