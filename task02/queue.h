#pragma once

#include <string>
#include<exception>

template <typename data>
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

template <typename data>
class ListQueue: public BasicQueue<data> {
public:

	ListQueue():
		head(NULL),
		last(NULL),
		length(0) {
	}

	~ListQueue() {
		Node<data>* buf;
		while( last != NULL) {
			buf = last->lhs;
			delete last;
			last = buf;
		}
	}

	void enqueue(const data& a) override;

	data dequeue() override;

	unsigned getLength() override {
		return length;
	}

private:

	template <typename data2>
	struct Node {

		Node():
			val(0),
			rhs(NULL),
			lhs(NULL) {
		}

		Node(const data2& a, Node* l = NULL, Node* r = NULL):
			val(a),
			lhs(l),
			rhs(r) {
		}
		data2 val;
		Node<data2>* lhs;
		Node<data2>* rhs;
	};

	Node<data>* head;  //first to leave
	Node<data>* last;  // last to leave (tail)
	unsigned length;
};

template <typename data, int N = 64>
class ArrayQueue: public BasicQueue<data> {
public:
	ArrayQueue():
		size(N),
		first(0),
		last(0),
		is_empty(true) {
	}

	void enqueue(const data& a) override;

	data dequeue() override;

	unsigned getLength() override {
		if( last > first) {
			return last - first + 1;
		} else if( last < first){
			return (N - first + 1 + last);   // (N - first) elements before N and (last + 1) elements before last (including 0)
		} else if(is_empty) {
			return 0;
		} else {
			return 1;
		}
	}

private:
	unsigned size;
	unsigned first;    //number of the first to leave
	unsigned last;     //number of the last in queue
	bool is_empty;
	data q[N];
};

// bigger than usual definitions from here

template <typename data>
void ListQueue<data>::enqueue(const data& a) {
	Node<data>* New = new Node<data>(a, last);
	if(head == NULL) {
		head = New;
		last = New;
	} else {
		last->rhs = New;
		last = New;
	}
	length++;
}

template <typename data>
data ListQueue<data>::dequeue() {

	if(length == 0u) {
		throw std::range_error("Empty queue!");
	}

	Node<data> buf = *head;
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
}

template <typename data, int N>
void ArrayQueue<data, N>::enqueue(const data& a) {

	if(is_empty) {
		is_empty = false;
		q[first] = a;
		q[last] = a;
	} else {
		if(first == (last + 1) % N) {
			throw std::range_error("Queue is already full!");
		}

		last = (last + 1) % N;
		q[last] = a;
	}
}

template <typename data, int N>
data ArrayQueue<data, N>::dequeue() {

	if(is_empty) {
		throw std::range_error("Queue is empty!");
	}

	data buf;
	buf = q[first];
	if(first == last) {
		is_empty = true;
	} else {
		first = (first + 1) % N;
	}
	return buf;
}
