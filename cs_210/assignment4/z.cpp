#include <iostream>
using namespace std;

// Struct lets you make custom data types, classes are better and more powerful

struct Node {
    int value;
    Node* next;
};

// Chaining: 

int main() {

    // using {} braces for the struct instance 
    Node* first = new Node{100, nullptr};
    Node* second = new Node{200, nullptr};
    Node* third = new Node{300, nullptr};

    first->next = second;
    second->next = third;
 
    Node* third_ = new Node(100, nullptr);
    Node* second_ = new Node{200, third_};
    Node* first_ = new Node{300, second_};


    // chaining
    Node* chain = new Node{100, new Node{200, new Node{300, nullptr}}};

    cout << first->value;

    // for getting just the memory address
    cout << first;

    // prints out 100
    cout << chain->value;

    // prints out 300
    cout << chain->next->next->value;

}