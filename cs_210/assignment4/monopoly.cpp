#include <iostream>

class Node {
    public:
        int value;
        Node* next;
        Node* previous;
    
        Node(int num) {
            value = num;
            next = nullptr;
            previous = nullptr;
        }
};

class CircularLinkedList {
    public:
        Node* head;
        Node* tail;
        Node* current; 

        CircularLinkedList() {
            head = nullptr;
            tail = nullptr;
        }

        Node* search(int val) {
            Node* currentNode = head;
            if (currentNode->value == head->value) {
                return head;
            }

            currentNode = currentNode->next;
            while (currentNode != head) {
                if (currentNode->value == val) {
                    return currentNode;
                }
                currentNode = currentNode->next; 
            }

            return nullptr;
        }

        void insert(Node* curr, Node* newNode) {
            if (head == nullptr) {
                head = newNode;
                tail = newNode;
            } else if (curr == tail) {
                tail->next = newNode;
                newNode->previous = tail;
                tail = newNode;
                tail->next = head;

            } else {
                newNode->next = curr->next;
                curr->next->previous = newNode;
                curr->next = newNode;
                newNode->previous = curr;
            }
        }

        void removeNodeAfter(Node* curr) {
            if (curr == nullptr) { //removing the head
                Node* toRemove = head;
                head = head->next;
                delete toRemove;

                if (head == nullptr) {
                    tail = nullptr;
                }
            } else if (curr->next) { // if there exists a node after to remove
                Node* toRemove = curr->next;
                Node* succeedingNode = toRemove->next;
                curr->next = succeedingNode;
                delete toRemove;

                if (succeedingNode == nullptr) {
                    tail = curr; // if removing the tail node
                }
            }
        }

        void printList() {
            current = head;
            while(current != nullptr) {
                std::cout << current->value << " " << std::endl; 
                current = current->next;
            }
        }
};

int main () {

}