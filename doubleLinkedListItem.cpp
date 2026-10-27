#include "doubleLinkedListItem.h"

DoubleLinkedListItem::DoubleLinkedListItem(DoubleLinkedListItem* prev, DoubleLinkedListItem* next, struct position value) {
    this -> prev = prev;
    this -> next = next;
    this -> value = value;
};


