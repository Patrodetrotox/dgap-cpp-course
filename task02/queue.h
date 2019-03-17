#pragma once

#include <string>

typedef int data;

struct Node {
	Node();
	Node(const data& a, Node* l = NULL, Node* r = NULL);
	data val;
	Node* lhs;
	Node* rhs;
};

class BasicQueue {
public:

	// Add element to the end
	virtual void enqueue(const data& a) = 0;

	// Return first element
	virtual data dequeue() = 0;

	// Get total count of elements
	virtual unsigned getLength() = 0;
protected:
	unsigned length;
};

class ListQueue: public BasicQueue {
public:
	ListQueue();

	void enqueue(const data& a) override;

	data dequeue() override;

	unsigned getLength() override;

	/*
	* Fully implement class:
	* - declarations here
	* - definitions in queue.cpp
	* Could have infinite size
	*/

private:
	Node* head;
	Node* last;
};

class ArrayQueue: public BasicQueue {
public:
	ArrayQueue(unsigned max_size = 64);

	void enqueue(const data& a) override;

	data dequeue() override;

	unsigned getLength() override;

	/*
	* Fully implement class:
	* - declarations here
	* - definitions in queue.cpp
	* Only finite size implementation is required
	*/
private:
	unsigned size;
	unsigned current;
	data* q;
};
