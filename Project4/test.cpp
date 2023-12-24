#include <iostream>
#include "binaryTree.h"
//g++ -fsanitize=leak -fsanitize=address -o test test.cpp binaryTree.cpp
using namespace std;

int main()
{
    Node* n1=new Node("f",8);
    Node* n2=new Node("g",9);
    Node* n3=new Node("d",7);
    Node* n4=new Node("e",3,n1,n2);
    Node* n5=new Node("b",2,n3,n4);
    Node* n6=new Node("c",5);
    Node* n7=new Node("a",4,n5,n6);
    BinaryTree tree(n7);
    BinaryTree cp = tree.copy();

    Node* m1=new Node("1",2);
    Node* m3=new Node("3",5);
    Node* m2=new Node("2",4,m1,m3);
    BinaryTree tree2(m2);

    cout << tree.findPath("b") << endl;
    cout << tree.sum() << endl;
    cout << tree.depth() << endl;
    tree.preorder_num(); cout << endl;
    tree.inorder_str(); cout << endl;
    tree.postorder_num(); cout << endl;
    cout << cp.sum() << endl;
    cout << tree2.covered_by(tree) << endl;
    cout << tree2.contained_by(tree) << endl;
    cout << tree2.allPathSumGreater(3) << endl;

    return 0;
}