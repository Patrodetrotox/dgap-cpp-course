#include "queue.h"
#include <exception>
#include <stdexcept>
#include <iostream>

using namespace std;

Node::Node() {
	val = 0;
	rhs = NULL;
	lhs = NULL;
}

Node::Node(const data& a, Node* l, Node* r) {
	val = a;
	lhs = l;
	rhs = r;
}

ListQueue::ListQueue() {
	head = NULL;
	last = NULL;
	length = 0;
}

void ListQueue::enqueue(const data& a) {
	Node* New = new Node(a, last);
	if(head == NULL) {
		head = New;
		last = New;
	} else {
		last->rhs = New;
		last = New;
	}
	length++;
}

data ListQueue::dequeue() {
	try {
		if(length == 0u) {
			throw runtime_error("Empty queue!");
		}

		Node buf = *head;
		if(length == 1u) {
			delete head;
			head = NULL;
			last = NULL;
		} else {
			delete head;
			head = buf.rhs;
			head->lhs = NULL;
		}
		length--;
		return buf.val;

	} catch (runtime_error& er) {
		cerr << endl << "Error: " << er.what() << endl;
		return 0;
	}
}

unsigned ListQueue::getLength() {
	return length;
}

void ArrayQueue::enqueue(const data& a) {

}

data ArrayQueue::dequeue() {

}

unsigned ArrayQueue::getLength() {

}
