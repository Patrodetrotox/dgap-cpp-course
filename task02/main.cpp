#include <iostream>
#include "queue.h"

using namespace std;

int main() {
	ListQueue q;
	q.enqueue(5);
	q.enqueue(4);
	q.enqueue(3);
	cout << q.dequeue() << " " << q.dequeue() << " " << q.dequeue() << " " << q.dequeue() << endl;
    return 0;
}
