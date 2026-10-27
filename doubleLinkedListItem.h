struct position {
    int row;
    int col;
};

class DoubleLinkedListItem {
    public:
        DoubleLinkedListItem* next;
        DoubleLinkedListItem* prev;
        struct position value;

        DoubleLinkedListItem(DoubleLinkedListItem* prev, DoubleLinkedListItem* next, struct position value);
};
