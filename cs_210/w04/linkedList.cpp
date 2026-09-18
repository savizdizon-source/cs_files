#include <iostream>

class Node {
    public:
        int value;
        Node* next;
    
        Node(int num) {
            value = num;
            next = nullptr;
        }
};

class SinglyLinkedList {
    public:
        Node* head;
        Node* tail;
        Node* current; 

        SinglyLinkedList() {
            head = nullptr;
            tail = nullptr;
        }

        Node* search(int val) {
            Node* currentNode = head;
            while (currentNode != nullptr) {
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
                tail = newNode;
            } else {
                newNode->next = curr->next;
                curr->next = newNode;
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

int main() {
    Node* node1 = new Node (1); 

    Node* node2 = new Node(2);

    Node* node3 = new Node(3);

    // std::cout << node1->value << node2->value << node3->value << std::endl;

    SinglyLinkedList list;

    list.insert(nullptr, node1);
    list.insert(node1, node2);
    list.insert(node2, node3);

    list.printList();

    std::cout << "Looking for element 2: " << list.search(2)->value << std::endl;

    list.removeNodeAfter(node1);

    list.printList();

}