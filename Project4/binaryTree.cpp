#include "binaryTree.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

/* ================================== Node =================================== */
Node::Node(const std::string &str, int num, Node *left, Node *right):str(str),num(num),left(left),right(right) 
{
    // TODO: implement this function.
}

Node* Node::leftSubtree() const
{
    // TODO: implement this function.  
    return left;
}

void Node::setleft(Node *n)
{
    // TODO: implement this function.  
    left = n;
}

Node* Node::rightSubtree() const
{
    // TODO: implement this function.  
    return right;
}

void Node::setright(Node *n)
{
    // TODO: implement this function.  
    right = n;
}

string Node::getstr() const 
{
    // TODO: implement this function.  
    return str;
}

int Node::getnum() const
{
    // TODO: implement this function.  
    return num;
}

void Node::incnum()
{
    // TODO: implement this function.
    num++;
}

Node *Node::mergeNodes(Node *leftNode, Node *rightNode)
{
    // TODO: implement this function.
    string mergedStr = leftNode->str + rightNode->str;
    int mergedNum = leftNode->num + rightNode->num;
    Node* mergedNode = new Node(mergedStr,mergedNum,leftNode,rightNode);
    return mergedNode;
}

/* =============================== Binary Tree =============================== */

BinaryTree::BinaryTree(Node *rootNode):root(rootNode)
{
    // TODO: implement this function.
}

// REQUIRES: The input rootNode points to a dynamically allocated node object,
//           if not being NULL.
// MODIFIES: this
// EFFECTS: delete all dynamically allocated node objects.
static void destructor_helper(Node* node)
{   // Delete nodes from bottom to top
    if (node==nullptr)
        return;
    destructor_helper(node->leftSubtree());
    destructor_helper(node->rightSubtree());
    delete node;
}

BinaryTree::~BinaryTree()
{
    // TODO: implement this function.
    destructor_helper(root);
}

// EFFECTS: return true if "s" is in tree with root "node"; otherwise, return false.
static bool findStr(const Node* node,const string & s)
{   // Check if s is in the tree whose root is node
    if (node==nullptr)
        return false;
    if (node->getstr()==s)
        return true;
    return findStr(node->leftSubtree(),s) || findStr(node->rightSubtree(),s);
}

// EFFECTS: return the path to "s".
static string findPath_helper(const Node* node,const string & s)
{   // Use findStr to find s is on left or right side of current node
    // Add "0" when it's on left while "1" when on right
    if (node->getstr()==s)
        return "";
    if (findStr(node->leftSubtree(),s))
        return "0"+findPath_helper(node->leftSubtree(),s);
    if (findStr(node->rightSubtree(),s))
        return "1"+findPath_helper(node->rightSubtree(),s);
    return ""; // Avoid warning(not supposed to run since it's guranteed that s is in the tree)
}

string BinaryTree::findPath(const string &s) const
{
    // TODO: implement this function.
    if (findStr(root,s))
        return findPath_helper(root,s);
    else
        return "-1";
}

// EFFECTS: return the sum of the tree with root "node"
static int sum_helper(const Node* node)
{   // Add the current node's "num" and then recurse for its left and right child
    if (node==nullptr)
        return 0;
    return (sum_helper(node->leftSubtree())+sum_helper(node->rightSubtree()))+node->getnum();
}

int BinaryTree::sum() const
{
    // TODO: implement this function.
    return sum_helper(root);
}

// EFFECTS: return the depth of the tree with root "node"
static int depth_helper(const Node* node)
{   // Find the depth by taking a maximum
    if (node==nullptr)
        return 0;
    return max(depth_helper(node->leftSubtree())+1,depth_helper(node->rightSubtree())+1);
}

int BinaryTree::depth() const
{
    // TODO: implement this function.
    return depth_helper(root);
}

// EFFECTS: print the "num"s in the tree with root "node" in preorder
static void preorder_num_helper(const Node* node)
{   // Help print "num" elements in preorder
    if (node==nullptr)
        return;
    cout << node->getnum() << " ";
    preorder_num_helper(node->leftSubtree());
    preorder_num_helper(node->rightSubtree());
}

void BinaryTree::preorder_num() const
{
    // TODO: implement this function.
    preorder_num_helper(root);
}

// EFFECTS: print "str"s in the tree with root "node" in inorder
static void inorder_str_helper(const Node* node)
{   // Help print "str" elements in inorder
    if (node==nullptr)
        return;
    inorder_str_helper(node->leftSubtree());
    cout << node->getstr() << " "; 
    inorder_str_helper(node->rightSubtree());
}

void BinaryTree::inorder_str() const
{
    // TODO: implement this function.
    inorder_str_helper(root);
}

// EFFECTS: print "num"s in the tree with root "node" in postorder
static void postorder_num_helper(const Node * node)
{   // Help print "num" elements in postorder
    if (node==nullptr)
        return;
    postorder_num_helper(node->leftSubtree());
    postorder_num_helper(node->rightSubtree());
    cout << node->getnum() << " ";
}

void BinaryTree::postorder_num() const
{
    // TODO: implement this function.
    postorder_num_helper(root);
}

// EFFECTS: return true if the sum of "num"s in all path if greater than "sum"; 
//          otherwise, return false
static bool allPathSumGreater_helper(const Node* node,const int sum,int cur_sum)
{   // Add "num" to current sum and compare it with sum when a route reaches end
    if (node==nullptr)
        return true;
    cur_sum += node->getnum();
    if (node->leftSubtree()==nullptr && node->rightSubtree()==nullptr)
        return (cur_sum>sum);
    return (allPathSumGreater_helper(node->leftSubtree(),sum,cur_sum) && 
            allPathSumGreater_helper(node->rightSubtree(),sum,cur_sum));
}

bool BinaryTree::allPathSumGreater(int sum) const{
    // TODO: implement this function.
    return allPathSumGreater_helper(root,sum,0);
}

// EFFECTS: return true if tree with root "nodeA" is covered by tree with root "nodeB"; 
//          otherwise, return false
static bool covered_by_helper(const Node* nodeA,const Node* nodeB)
{   // Check every node in the two trees and once the "num" is not equal, the result is false
    if (nodeA==nullptr)
        return true;
    else if (nodeB==nullptr)
        return false;
    else if (nodeA->getnum()==nodeB->getnum())
        return (covered_by_helper(nodeA->leftSubtree(),nodeB->leftSubtree()) && 
                covered_by_helper(nodeA->rightSubtree(),nodeB->rightSubtree()));
    else
        return false;
}

bool BinaryTree::covered_by(const BinaryTree &tree) const
{
    // TODO: implement this function.
    return covered_by_helper(root,tree.root);
}

// EFFECTS: return true if tree with root "nodeA" is contained by tree with root "nodeB"; 
//          otherwise, return false
static bool contained_by_helper(const Node* nodeA,const Node* nodeB)
{   // Recurse until nodeA is covered by nodeB or nodeB becomes empty
    if (covered_by_helper(nodeA,nodeB))
        return true;
    if (nodeB==nullptr)
        return false;
    return contained_by_helper(nodeA,nodeB->leftSubtree()) || contained_by_helper(nodeA,nodeB->rightSubtree());
}

bool BinaryTree::contained_by(const BinaryTree &tree) const
{
    // TODO: implement this function.
    return contained_by_helper(root,tree.root);
}

// EFFECTS: return a deep copy of this
static Node* copy_helper(const Node* node)
{   // Make a copy of current node and set the left and right of the copy
    Node* newNode = new Node(node->getstr(),node->getnum());
    if (node->leftSubtree()!=nullptr)
        newNode->setleft(copy_helper(node->leftSubtree()));
    if (node->rightSubtree()!=nullptr)
        newNode->setright(copy_helper(node->rightSubtree()));
    return newNode;
} 

BinaryTree BinaryTree::copy() const 
{
    // TODO: implement this function.
    if (root==nullptr)
        return BinaryTree();
    return BinaryTree(copy_helper(root));
}
