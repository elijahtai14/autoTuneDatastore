#ifndef SortedLinkedList_h
#define SortedLinkedList_h

#include <stdlib.h>
#include <iostream>

#ifndef nullptr
#define nullptr ( (void *) 0)
#endif

// Stores objects in increasing order in the form of a linked list with a max length requirement
template <class T> class SortedLinkedList 
{
    public: 

        SortedLinkedList(unsigned maxLen);

        // Constructor defaults to length zero
        SortedLinkedList();

        // Free the linked list
        ~SortedLinkedList();
        
        struct SortedListNode 
        {
            T obj;
            SortedListNode* next;
        };

        void setMax(unsigned maxLen); 

        T getMax();
        T getMin();

        unsigned size();
        unsigned maxSize();
        
        // Sticks obj where it should
        bool insert(T obj, T filter);
        bool cull(T filterObj);

        /*

            The idea here is the coder can do something like

            for (T* ptr = list.loopBegin(); ptr != nullptr; ptr = list.loopNext())
            {
                std::cout << ptr -> temp << std::endl; 
            }

        */
        // Returns the first element, and sets currentptr to the beginning
        T* loopBegin();
        // Returns the next element, and sets currentptr to the next element
        T* loopNext();
        
    private:

        unsigned len;
        unsigned maxLen;

        SortedListNode* headptr;        // Points to first element
        SortedListNode* tailptr;        // Points to last element
        SortedListNode* currentptr;     // Used in iterations

};
#endif