#include <algorithm>
#include <iostream>
#include <string>
#include "Dlist.h"
#include "Instr.h"
// g++ -Wall -Werror -O2 --std=c++17 -fsanitize=leak -fsanitize=address -o sam sam.cpp
// ./sam -s < ./sample.in
// 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16
using namespace std;
static const int MEMORYSIZE = 16; // Length of memory array

class Stack
{   // Class for storing operands(LIFO)
    private:
        Dlist<int> operands; // List of operands
    public:
        Stack() {Dlist<int> a; operands = a;}; // Constructor

        /*bool isEmpty() const {return operands.isEmpty();};
        // EFFECT: */

        void push(int* x) {operands.insertBack(x);};
        // EFFECT: insert a node whose op is x to the back of the queue
        // MODIFIES: this

        int* pop() {return operands.removeBack();};
        // EFFECT: remove the last node and return the pointer to int in it
        // MODIFIES: this

        void print() const {cout << operands << endl;};
        // EFFECT: print the stack
};

class Queue
{   // Class for storing instructions(FIFO)
    private:
        Dlist<Instr> instructions; // List of instructions
    public:
        Queue() {Dlist<Instr> a; instructions = a;}; // Constructor

        void push(Instr* x) {instructions.insertBack(x);};
        // EFFECT: insert a node whose op is x to the back of the queue
        // MODIFIES: this

        Instr* pop() {return instructions.removeFront();};
        // EFFECT: remove the first node and return the pointer to Instr in it
        // MODIFIES: this

        void print() const {cout << instructions << endl;};
        // EFFECT: print the queue
};

void init(Stack & ops,Queue & instrs,int* mem)
// EFFECT: initialize the operand stack, instruction queue and memory array
// MODIFIES: ops,instrs,mem
{
    int n,m;
    cin >> n >> m;
    // The pointers pushed must be dynamically allocated
    for (int i=0; i<n; i++)
    {
        int* op = new int;
        cin >> *op;
        ops.push(op);
    }
    for (int i=0; i<m; i++)
    {
        Instr* it = new Instr;
        cin >> *it;
        instrs.push(it);
    }
    for (int i=0; i<MEMORYSIZE; i++)
        cin >> mem[i];
}

bool process(Stack & ops,Queue & instrs,Instr* it,int* mem)
// EFFECT: follow the instruction it to do something
// MODIFIES: ops, instrs, mem
{
    if (it->name==InstrName::HALT)
    {   // Machine stops
        return false;
    } 
        
    // We need to delete everything poped at last
    if (it->name==InstrName::ADD)
    {
        int* num1 = ops.pop();
        int* num2 = ops.pop();
        int* sum = new int(*num1+*num2);
        ops.push(sum);

        delete num1; delete num2;
    }

    else if (it->name==InstrName::NOR)
    {
        int* num1 = ops.pop();
        int* num2 = ops.pop();
        int* result = new int(~(*num1|*num2));
        ops.push(result);

        delete num1; delete num2;
    }

    else if (it->name==InstrName::IFZ)
    {
        int* num = ops.pop();
        if (*num==0)
        {
            for (int i=0; i<it->parameter; i++)
            {
                Instr* temp = instrs.pop();
                delete temp;
            }
        }
        delete num;
    }

    else if (it->name==InstrName::LOAD)
    {
        int* num = ops.pop();
        ops.push(new int(mem[*num]));
        delete num;
    }

    else if (it->name==InstrName::STORE)
    {
        int* address = ops.pop();
        int* value = ops.pop();
        mem[*address] = *value;
        delete address; delete value;
    }

    else if (it->name==InstrName::POP)
    {
        int* temp = ops.pop();
        delete temp;
    }

    else if (it->name==InstrName::PUSHI)
    {
        ops.push(new int(it->parameter));
    }

    else if (it->name==InstrName::NOOP) {}

    return true;
}

/*void printStatus(const Stack & ops,const Queue & instrs,const int* mem)
{
    ops.print();
    instrs.print();
    for (int i=0; i<MEMORYSIZE; i++)
        cout << mem[i] << " ";
}*/

int main(int argc, char *argv[])
{
    //TODO:write your code here.
    // Initialize
    Stack ops;
    Queue instrs;
    int mem[MEMORYSIZE];
    init(ops,instrs,mem);

    if (argc==1)
    {
        while (1)
        {
            Instr* it = instrs.pop();
            bool flag = process(ops,instrs,it,mem);

            // Print machine status
            cout << *it << endl;
            ops.print();
            instrs.print();
            for (int i=0; i<MEMORYSIZE; i++)
                cout << mem[i] << " ";
            cout << endl;

            delete it;
            if (!flag)
                break; // End when flag is false, which means HALT occurs
        }
    }

    else if (argc==2)
    {
        while (1)
        {
            Instr* it = instrs.pop();
            bool flag = process(ops,instrs,it,mem);
            delete it;
            if (!flag)
                break; // End when flag is false, which means HALT occurs
        }
        
        // Print final machine status
        ops.print();
        for (int i=0; i<MEMORYSIZE; i++)
            cout << mem[i] << " ";
        cout << endl;
    }

    return 0;
}
