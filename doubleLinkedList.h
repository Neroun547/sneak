#include "doubleLinkedListItem.h"

class DoubleLinkedList {
    public:
        DoubleLinkedListItem* firstElement;

    public:
        void unshift(position position);
        void push(position position);
        bool existsByValue(position position);
        DoubleLinkedListItem* getLastItem();
        int count();

    ~DoubleLinkedList() {

    }
};

