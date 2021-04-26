#ifndef SortedLinkedList_h
#define SortedLinkedList_h

#ifndef nullptr
#define nullptr ( (void *) 0)
#endif

#include <stdlib.h>
#include <stdio.h>

// Stores objects in increasing order in the form of a linked list with a max length requirement
template <class T> class SortedLinkedList 
{
    public:

        SortedLinkedList(unsigned maxLen)
        {
            // Initial length is zero
            this -> len = 0;
            // Length capped a maxLen
            this -> maxLen = maxLen;
            
            headptr = nullptr;
            tailptr = nullptr;
            currentptr = nullptr;
        }

        // Free the linked list
        ~SortedLinkedList()
        {
            SortedListNode* cur = headptr;
            while (cur) 
            {
                headptr = cur -> next;
                std::cout<<"Destructor"<<std::endl;
                free(cur);
                cur = headptr;
            }
        }
        
        struct SortedListNode 
        {
            T obj;
            SortedListNode* next;
        };

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
        double filter;

        SortedListNode* headptr;        // Points to first element
        SortedListNode* tailptr;        // Points to last element
        SortedListNode* currentptr;     // Used in iterations

};

// Returns the maximum element
template <class T> T SortedLinkedList<T>::getMax()
{
    return tailptr -> obj;
}

// Returns the minimum element
template <class T> T SortedLinkedList<T>::getMin()
{
    return headptr -> obj;
}

// Returns current number of elements
template <class T> unsigned SortedLinkedList<T>::size() 
{
    return len;
}

template <class T> unsigned SortedLinkedList<T>::maxSize()
{
    return maxLen;
}

// Sticks obj where it should
template <class T> bool SortedLinkedList<T>::insert(T obj, T filter)
{  
    // Create a new node with thiis object
    SortedListNode* newNode = (SortedListNode*)malloc(sizeof(SortedListNode));
    newNode -> obj = obj;
    newNode -> next = nullptr;
    
    // This is our first element
    if (len == 0) 
    {
        // Idiot made the maxLen be zero or less, cannot insert anything
        if (maxLen <= len) 
        {
            free(newNode);
            return false;
        }

        headptr = newNode;
        tailptr = newNode;
        len++;

        return true;
    }

    // Otherwise, we must have a tailptr to our largest element. 
    // In the unlikely event our obj is smaller, we have to insert it in O(n);
    if (obj < tailptr -> obj)
    {
        // If our object is smaller than our head, then we insert it as the new head
        SortedListNode* cur = headptr;
        if (obj < cur -> obj) 
        {
            newNode -> next = headptr;
            headptr = newNode;
            len++;
        }
        // Otherwise, we know there must be at least two elements,
        // because  headptr -> obj <= obj < tailptr -> obj
        // So, we insert it right after the element whose next element is larger
        else
        {
            bool isNotFound = true;
            while (isNotFound)
            {
                // We know that the tailptr is larger than obj, so this must be satisfied.
                if ((cur -> next) -> obj > obj) 
                {
                    // Insert the new node between cur and cur -> next
                    newNode -> next = cur -> next;
                    cur -> next = newNode;
                    len++;
                    isNotFound = false;                    
                }
                cur = cur -> next;
            }
        }
    }
    // If it is larger or equal to tailptr -> obj, we can stick it at the end in O(1)
    // Note, for the autotuning purpose, we expect an increasing squence so it should always be O(1)
    else 
    {
        // The last object now points to this node
        tailptr -> next = newNode;
        // And our new node is now the last node
        tailptr = newNode;
        len++;
    }

    // Now we have inserted everything while maintaining (albeit not strictly) increasing order, most likely in O(1)
    // We are still within the length requirement, all is good.
    if (len <= maxLen) 
    {   
        return true;
    }
    // We need to decrease the total number of elements
    else 
    {
        cull(filter);
    }
    return true;
}

template <class T> bool SortedLinkedList<T>::cull(T filterObj) 
{
    // First remove all values less than filter
    SortedListNode* cur = headptr;

    // Keep freeing nodes and moving up the headptr while it is less than filterObj
    while (cur -> obj < filterObj)
    {   
        // If we are removing the last and largest element
        if (cur == tailptr || cur -> next == nullptr) 
        {
            free(cur);
            headptr = nullptr;
            tailptr = nullptr;
            len = 0;

            return true;
        }
        else
        {
            headptr = cur -> next;
            std::cout<<"cull filter"<<std::endl;
            free(cur);
            cur = headptr;
            len--;
        }
    }

    // We now know no values can be filtered out, so we should start calculating two-gaps
    if (len > maxLen && maxLen < 3) 
    {
        return false;
    }
    // From here out, we know that there are at least 3 elements
    else
    {
        // Only really used if we end up resorting to minTwoGap
        SortedListNode* curMinGap;
        SortedListNode* curEnd;
        double minTwoGap;

        // Remove the minimum two gap one by one
        while (len > maxLen) 
        {   
            cur = headptr;
            curMinGap = cur;
            curEnd = (cur -> next) -> next;
            double minTwoGap = curEnd -> obj - cur -> obj;

            // While we haven't just calculated the final node...
            while (curEnd != tailptr || curEnd -> next != nullptr) 
            {   
                cur = cur -> next;
                curEnd = curEnd -> next;
                double twoGap = curEnd -> obj - cur -> obj;
                
                if (twoGap < minTwoGap) 
                {
                    minTwoGap = twoGap;

                    // curMinGap is the  of the two-gap, not the element to remove
                    curMinGap = cur;
                }
            }

            // Here curMinGap points to the start of the gap, not the element to remove.
            /*
                pointers:   |  curMinGap  |                   |             |
                linkedlist: |  gap start  > element to remove >  gap end    |
            */
            cur = curMinGap;
            curMinGap = cur -> next;
            // Now:
            /*
                pointers:   |     cur     |     curMinGap     |             |
                linkedlist: |  gap start  > element to remove >  gap end    |
            */
            cur -> next = curMinGap -> next;
            std::cout<<"cull two gap"<<std::endl;
            free(curMinGap);
            len--;
            // Now:
            /*
                pointers:   |     cur     |             |
                linkedlist: |  gap start  >  gap end    |
            */
        } 
    }

    return true;
}

/*
    The idea here is the coder can do something like

    for (T* ptr = list.loopBegin(); ptr != nullptr; ptr = list.loopNext())
    {
        std::cout << ptr -> temp << std::endl; 
    }
 */

// Returns the first element, and sets currentptr to the headptr
template <class T> T* SortedLinkedList<T>::loopBegin()
{
    currentptr = headptr;
    return (T*)&(currentptr -> obj);
}

// Returns the next element if possible, and sets currentptr to the next element
template <class T> T* SortedLinkedList<T>::loopNext()
{
    // If this currently the pointer to the last object, there is no next. Return nullptr instead
    if (currentptr == nullptr || currentptr == tailptr || currentptr -> next == nullptr)
    {
        return nullptr;
    }

    // Otherwise, return the next value
    else
    {
        currentptr = currentptr -> next;
        return (T*)&(currentptr -> obj);
    }
}

#endif