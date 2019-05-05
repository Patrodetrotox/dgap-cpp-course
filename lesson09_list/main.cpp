
#include "list.h"
///#include <list>
#include <iostream>
#include <algorithm>

using namespace std;

int main()
{
    List<int> l;

    l.push_front(0);
    l.push_front(1);
    l.push_front(2);
    l.push_front(3);

    l.push_back(-1);

    List<int>::iterator it = l.begin(); // front

    do {
        cout << *it << " ";
    } while (!(++it == l.end()));
    cout << endl;

    for (auto &i : l)
        cout << i << " ";
    cout << endl;

    cout << "Found: " << *find(l.begin(), l.end(), 1) << endl;

#if 0
    int &i = l.front();
    l.pop_front();
    cout << i;  // Forbidden
#endif

    return 0;
}

