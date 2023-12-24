#include "binaryTree.h"
#include "huffmanTree.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
//g++ --std=c++17 -fsanitize=leak -fsanitize=address -o compress compress.cpp binaryTree.cpp huffmanTree.cpp
using namespace std;

// EFFECT: return true if "n1" is "bigger" than "n2"
bool comp(Node* n1,Node* n2)
{   // Determine which node is "smaller"
    if (n1->getnum()<n2->getnum())
        return false;
    else if (n1->getnum()>n2->getnum())
        return true;
    else
        return (n1->getstr()>n2->getstr());
}

// EFFECT: return the input file name
string getFileName(const int & argc,char* argv[])
{
    string file_name;
    if (argc==2)
        file_name = argv[1];
    else
        file_name = argv[2];
    return file_name;
}

// EFFECT: build up the Node* array
// MODIFIES: "nodes"
void buildArray(vector<Node*> & nodes,const char & ch)
{
    char temp[2]={ch,'\0'};
    string s(temp); // Convert char to string to enable comparison
    bool flag=true;
    for (unsigned i=0; i<nodes.size(); i++)
    {
        if (nodes[i]->getstr()==s)
        {
            nodes[i]->incnum();
            flag=false;
        }
    }
    if (flag)
    {
        Node* newNode = new Node(s,1);
        nodes.push_back(newNode);
    }
}

// EFFECT: merge nodes until there is one left in nodes
// MODIFIES: "nodes"
void buildRoot(vector<Node*> & nodes)
{
    while (nodes.size()>1)
    {
        sort(nodes.begin(),nodes.end(),comp); // Sort nodes to be in a decreasing order
        // Merge the two smallest nodes
        Node* mergedNode = mergedNode->mergeNodes(nodes[nodes.size()-2],nodes[nodes.size()-1]);
        // Delete two smallest nodes and add the merged node to the array
        nodes.pop_back();
        nodes.pop_back();
        nodes.push_back(mergedNode);
    }
}

int main(int argc, char *argv[]) 
{
    // TODO: implement this function
    string file_name = getFileName(argc,argv);
    ifstream inFile;
    inFile.open(file_name);
    
    char ch;
    vector<Node*> nodes;
    while (inFile.get(ch))
        buildArray(nodes,ch); // Build the array of nodes
    inFile.close();

    buildRoot(nodes);
    
    HuffmanTree huffTree(nodes[0]); // The node in the array is the root node
    if (argc==3)
        huffTree.printTree();
    if (argc==2)
    {   // Compress file with Huffman Tree
        ifstream inFile2;
        inFile2.open(file_name);
        while (inFile2.get(ch))
        {
            char temp[2]={ch,'\0'};
            string s(temp);
            cout << huffTree.findPath(s) << " ";
        }
        inFile2.close();
    }

    return 0;
}