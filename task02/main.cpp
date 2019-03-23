#include <iostream>
#include "queue.h"
#include "test_runner.h"

using namespace std;

void TestAll() {
	TestRunner tr;
	tr.RunTest(ListQueueTest, "List based queue test");
	tr.RunTest(ArrayQueueTest, "Array based queue test");
	tr.RunTest(BasicQueueTest, "Basic queue test");
}

int main() {

	TestAll(); // ends program if one of the tests fails

	// code should be here

    return 0;
}
