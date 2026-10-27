#include "doubleLinkedList.h"

void DoubleLinkedList::push(position item) {
    DoubleLinkedListItem* newItem = new DoubleLinkedListItem(nullptr, nullptr, item);

    if(this -> firstElement == nullptr) {
        this -> firstElement = newItem;

        return;
    }
    for(DoubleLinkedListItem* listItem = this -> firstElement; listItem != nullptr; listItem = listItem -> next) {
        if(listItem -> next == nullptr) {
            listItem -> next = newItem;
            listItem -> next -> prev = listItem;

            break;
        }
    }
}

bool DoubleLinkedList::existsByValue(position value) {
    for(DoubleLinkedListItem* item = this -> firstElement; item != nullptr; item = item -> next) {
        if(item -> value.col == value.col && item -> value.row == value.row) {
            return true;
        }
    }
    return false;
}

DoubleLinkedListItem* DoubleLinkedList::getLastItem() {
    DoubleLinkedListItem* tmp;

    for(DoubleLinkedListItem* item = this -> firstElement; item != nullptr; item = item -> next) {
        tmp = item;
    }
    return tmp;
}

void DoubleLinkedList::unshift(position position) {
    DoubleLinkedListItem* newItem = new DoubleLinkedListItem(nullptr, this->firstElement, position);

    this->firstElement->prev = newItem;
    this->firstElement = newItem;
}

int DoubleLinkedList::count() {
    int result = 0;

    for(DoubleLinkedListItem* item = this -> firstElement; item != nullptr; item = item -> next) {
        result += 1;
    }
    return result;
}
