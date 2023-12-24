#include "binaryTree.h"
#include "huffmanTree.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
//g++ --std=c++17 -fsanitize=leak -fsanitize=address -o decompress decompress.cpp binaryTree.cpp huffmanTree.cpp
using namespace std;

// EFFECT: find and print the "str" in "huffTree" according to "encode"
void decodeWord(const HuffmanTree & huffTree,const string & encode)
{
    // Decode the file by following 0(left) and 1(right) to find the str
    Node* node=huffTree.root;
    for (unsigned i=0; i<encode.length(); i++)
    {
        if (encode[i]=='0')
            node=node->leftSubtree();
        else
            node=node->rightSubtree();
    }
    cout << node->getstr();
}

// EFFECT: print the decoded texts to standard output
void decode(ifstream & inFile,const HuffmanTree & huffTree)
{
    string encode;
    while (inFile >> encode)
        decodeWord(huffTree,encode);
}

int main(int argc, char *argv[])
{
    // TODO: implement this function
    HuffmanTree huffTree(argv[1]);
    ifstream inFile;
    inFile.open(argv[2]);

    decode(inFile,huffTree);
    
    inFile.close();

    return 0;
}