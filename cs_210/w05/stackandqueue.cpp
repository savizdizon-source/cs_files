#include <iostream>
using namespace std;

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

        void prepend(Node* newNode) {
            if (head == nullptr) {
                head = newNode;
                tail = newNode;
            } else {
                newNode->next = head;
                head = newNode;
            }
        }
};

class Stack {
    public: 
        SinglyLinkedList list;
        int count = 0;

        void push(Node node) {
            list.prepend(new Node(node));
            count++;
        }

        int pop() {
            int val = list.head->value;
            list.removeNodeAfter(nullptr);
            count--;
            return val;
        }

        int peek() {
            return list.head->value;
        }

        bool isEmpty() {
            if (count > 0) {
                return false;
            } else {
                return true;
            }
        }

        int getLength() {
            return count;
        }

        void print() {
            list.printList();
        }
};

class Queue {
    public: 
        SinglyLinkedList list;
        int count = 0;

        void enqueue(int val) {
            // adds to the end of the queue
            list.insert(list.tail, new Node(val));
            count++;
        }

        int dequeue() {
            // returns and removes item at the front of the queue
            int val = list.head->value;
            count--;
            list.removeNodeAfter(nullptr);
            return val;
        }

        int peek() {
            // returns but doesn't remove item at the front of the queue
            return list.head->value;
        }

        bool isEmpty() {
            if (count > 0) {
                return false;
            } else {
                return true;
            }
        }

        int getLength() {
            return count;
        }

        void print() {
            list.printList();
        }
};

int main() {

    Node node1 = Node(34);
    Node node2 = Node(27);
    Node node3 = Node(4);
    Node node4 = Node(12);

    // stack tests
    Stack stack;
    stack.push(node1);
    stack.push(node2);
    stack.push(node3);
    stack.push(node4);

    stack.print();

    std::cout << "Peek: " << stack.peek() << std::endl;
    std::cout << "Pop: " << stack.pop() << std::endl; 
    
    stack.print();

    std::cout << "IsEmpty: " << stack.isEmpty() << std::endl;        
    std::cout << "GetLength: " << stack.getLength() << std::endl;

    // queue tests
    Queue q;
    q.enqueue(34);
    q.enqueue(23);
    q.enqueue(3);
    q.enqueue(12);

    q.print();

    std::cout << "Dequeue: " << q.dequeue() << std::endl;

    q.print();

    std::cout << "Peek: " << q.peek() << std::endl;
    std::cout << "IsEmpty: " << q.isEmpty() << std::endl;
    std::cout << "GetLength: " << q.getLength() << std::endl;

}