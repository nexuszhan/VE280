#ifndef __DLISTIMPL_H__
#define __DLISTIMPL_H__

#include "Dlist.h"

template <class T>
bool Dlist<T>::isEmpty() const
{
    if (first==nullptr)
        return true;
    return false;
}

template <class T>
void Dlist<T>::insertFront(T* op)
{
    if (isEmpty())
    {   
        // When list is empty, create a node whose nest and prev are both nullptrs
        node* newNode = new node{nullptr,nullptr,op};
        first = last = newNode;
    }
    else if (first==last)
    {   
        // When there is only one node is list, set first as a new node whose prev is nullptr
        // and update last's prev
        node* newNode = new node{last,nullptr,op};
        first = newNode;
        last->prev = newNode;
    }
    else
    {   
        // When more than one node in list, set first's prev to be a new node whose next is first
        // and set first as the new node
        node* newNode = new node{first,nullptr,op};
        first->prev = newNode;
        first = newNode;
    }
}

template <class T>
void Dlist<T>::insertBack(T *op)
{
    if (isEmpty())
    {   
        // When list is empty, create a node whose nest and prev are both nullptrs
        node* newNode = new node{nullptr,nullptr,op};
        first = last = newNode;
    }
    else if (first==last)
    {   
        // When there is only one node is list, set last as a new node whose next is nullptr
        // and update first's next
        node* newNode = new node{nullptr,first,op};
        last = newNode;
        first->next = newNode;
    }
    else
    {
        // When more than one node in list, set last's next to be a new node whose prev is last
        // and set last as the new node
        node* newNode = new node{nullptr,last,op};
        last->next = newNode;
        last = newNode;
    }
}

template <class T>
T* Dlist<T>::removeFront()
{
    if (isEmpty())
        throw EmptyList();
    else
    {
        T* temp = first->op; // Hold the pointer to op
        node* cp = first; // Hold first
        if (first==last)
        {
            // When only one node in list, just set first and last to be nullptrs
            first = last = nullptr;
        }
        else
        {
            // When more than one node in list, set first to the next of current first 
            // and the prev of it to nullptr
            first = first->next;
            first->prev = nullptr;
        }
        delete cp;
        return temp;
    }
}

template <class T>
T* Dlist<T>::removeBack()
{
    if (isEmpty())
        throw EmptyList();
    else
    {
        T* temp = last->op; // Hold the pointer to op
        node* cp = last; // Hold last
        if (first==last)
        {
            // When only one node in list, just set first and last to be nullptrs
            last = first = nullptr;
        }
        else
        {
             // When more than one node in list, set last to the prev of current last 
            // and the next of it to nullptr
            last = last->prev;
            last->next = nullptr;
        }
        delete cp;
        return temp;
    }
}

template <class T>
Dlist<T>::Dlist() : first(nullptr),last(nullptr)
{}

template <class T>
Dlist<T>::Dlist(const Dlist<T> & l)
{
    copyAll(l);
}

template <class T>
Dlist<T> & Dlist<T>::operator=(const Dlist<T> &l)
{
    removeAll(); // Remove all existing nodes first
    copyAll(l);
    return *this;
}

template <class T>
Dlist<T>::~Dlist()
{
    removeAll();
}

template <class T>
void Dlist<T>::removeAll()
{
    node* cur = first;
    node* nxt = first; // Store the next node of cur
    while (cur != nullptr)
    {
        nxt = cur->next;
        delete cur->op;
        delete cur;
        cur = nxt;
    }
    first = last = nullptr; // Set the list as empty
}

template <class T>
void Dlist<T>::copyAll(const Dlist<T> & l)
{
    first = last = nullptr; // For safety
    node* curL = l.first;
    while (curL != nullptr)
    {
        insertBack(new T(*curL->op)); // The pointer must be a deep copy of op
        curL = curL->next;
    }
}

#endif