#include <iostream>
using namespace std;

// Struct lets you make custom data types, classes are better and more powerful

struct Node {
    int value;
    Node* next;
    Node* prev;
};

// Chaining: 

int main() {

    // using {} braces for the struct instance 
    Node* first = new Node{100, nullptr};
    Node* second = new Node{200, nullptr};
    Node* third = new Node{300, nullptr};

    first->next = second;
    second->next = third;
    third->next = first;

    first->prev = third;
    second->prev = first;
    third->prev = second;

    cout << first->next->next->next->value;
    
    // prints out 100
    cout << first->next->prev;

}