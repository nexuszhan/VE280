#include "DlistImpl.h"
#include "Dlist.h"
#include <iostream>
// g++ -Wall -fsanitize=leak -fsanitize=address -o test test.cpp
using namespace std;

int main()
{
    Dlist<int> a;
    int* m1 = new int(1);
    int* m2 = new int(2);
    int* m3 = new int(3);
    int* m4 = new int(4);
    a.insertBack(m1);
    a.insertFront(m2);
    a.insertFront(m3);
    a.insertBack(m4);
    cout << a << endl;
    /*int* tmp = a.removeFront();
    int* tmp2 = a.removeBack();
    delete tmp;
    delete tmp2;*/
    Dlist<int> b(a);
    cout << b << endl;

    Dlist<int> c;
    c = a;
    cout << c << endl;

    return 0;
}