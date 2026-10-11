#include <iostream>
#include <string>

class Node {
    public:
        int value;
        Node* next;
    
        Node(int num) {
            value = num;
            next = nullptr;
        }
};

// linked list
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

// stacks
    // linked list implementation
class Stack {
    public: 
        SinglyLinkedList list;
        int count = 0;

        // puts new node at the front of the list (making it the head)
        void push(int val) {
            list.prepend(new Node(val));
            count++;
        }

        // returning the head, then deleting it and shifting everything up one
        int pop() {
            int val = list.head->value;
            list.removeNodeAfter(nullptr);
            count--;
            return val;

            // list->head = list->head->next;
            // delete list
        }

        // return the head
        int peek() {
            return list.head->value;
        }

        // checking if the stack is empty 
        bool isEmpty() {
            if (count > 0) {
                return false;
            } else {
                return true;
            }
        }

        // how many items are in the stack
        int getLength() {
            return count;
        }

        // printing out the stack 
        void print() {
            list.printList();
        }
};

// queues
    // linked list implementation
class Queue {
    public: 
        SinglyLinkedList list;
        int count = 0;

        void enqueue(int val) {
            // adds to the end of the queue
            list.insert(list.tail, new Node(val));
            count++;
        }
        
        // returns and removes item at the front of the queue
        int dequeue() {
            
            int val = list.head->value;
            count--;
            list.removeNodeAfter(nullptr);
            return val;
        }
        
        // returns but doesn't remove item at the front of the queue
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

    // array implementation
class QueueArray {
    int arr[10];
    int capacity = 0;
   
    public:

    void enqueueArr(int val) {
        arr[capacity] = val;
        capacity++;
    }

    // return and remove item at front of queue then shift everything else up
    int dequeueArr() {
        int toReturn = arr[0];
        for (int i = 1; i < capacity; i++) {
            arr[i - 1] = arr[i];
        }

        capacity--;
        return toReturn;
    }

    int peekArr() {
        return arr[0];
    }

    std::string arrEmpty() {
        if (capacity == 0) {
            return "true";
        }

        return "false";
    }

    int arrLength() {
        return capacity;
    }

    void printQueue() {
        for (int i = 0; i < capacity; i++) {
            std::cout << arr[i] << std::endl;
        }
    }
};

class StackArray {
    public: 

    int arr[10];
    int capacity = 0;

    void pushArr(int val) {
        arr[capacity] = val;
        capacity++;
    }

    // return and remove item at front of queue then shift everything else up
    int popArr() {
        int toReturn = arr[capacity - 1];
        capacity--;
        return toReturn;
    }

    int peekArr() {
        return arr[capacity - 1];
    }

    std::string arrEmpty() {
        if (capacity == 0) {
            return "true";
        }

        return "false";
    }

    int arrLength() {
        return capacity;
    }

    void printStack() {
        for (int i = 0; i < capacity; i++) {
            std::cout << arr[i] << std::endl;
        }
    }

};

// deques
    // linked list version
    // will inherit the print function
class Deques {
    public: 
        SinglyLinkedList list;
        int count = 0;

        //push front = at the front of deque
        void pushFront(Node* node) {
            list.prepend(node);
            count++;
        }

        //push back = at the back of deque
        void pushBack(Node* node) {
            list.insert(list.tail, node);
            count++;
        }

        //popfront = returns and removes 
        int popFront() {
            int toReturn = list.head->value;
            list.removeNodeAfter(nullptr);  
            count--;
            return toReturn;
        } 
        //popback = returns and removes
        int popBack() {
            int toReturn = list.tail->value;

            if (list.head == list.tail) {
                list.removeNodeAfter(nullptr);
            } else {
                Node* prev = list.head;
                while (prev->next != list.tail) {
                    prev = prev->next;
                }
                list.removeNodeAfter(prev); 
            }
            count--;
            return toReturn;
        }
        //peekfront
        int peekFront() {
            return list.head->value;
        }
        //peekback
        int peekBack() {
            return list.tail->value;
        }
        //isEmpty
        std::string isEmpty() {
            if (count = 0) {
                return "true";
            }

            return "false";
        }

        //getLength
        int getLength() {
            return count;
        }

};

    // array version
class DequesArray {
    public:
    int arr[10];
    int capacity = 0;

    //push front = at the front of deque
    void pushFrontArr(int val) {
        capacity++;
        for (int i = capacity; i >= 0; i--) {
            arr[i] = arr[i - 1];
        }

        arr[0] = val;
    }

    //push back = at the back of deque
    void pushBackArr(int val) {
        arr[capacity] = val;
        capacity++;        
    }

    //popfront = returns and removes 
    int popFrontArr() {
        int toReturn = arr[0];
        for (int i = 1; i < capacity; i++) {
            arr[i - 1] = arr[i];
        }

        capacity--;
        return toReturn;
    }
    
    //popback = returns and removes
    int popBackArr() {
        int toReturn = arr[capacity - 1];
        capacity--;
        return toReturn;
    }

    //peekfront
    int peekFrontArr() {
        return arr[0];
    }

    //peekback
    int peekBackArr() {
        return arr[capacity - 1];
    }

    //isEmpty
    std::string isEmptyArr() {
        if (capacity == 0) {
            return "true";
        }

        return "false";
    }

    //getLength
    int getLengthArr() {
        return capacity;
    }

    void printDeque() {
        for (int i = 0; i < capacity; i++) {
            std::cout << arr[i] << std::endl;
        }
    }

};

int main() {
// queue array test
    std::cout << "Queues array test:" << std::endl;
    QueueArray* q = new QueueArray();

    std::cout << "Is empty before enqueue: " << q->arrEmpty() << std::endl;

    q->enqueueArr(3);
    q->enqueueArr(35);
    q->enqueueArr(2);
    q->enqueueArr(14);
    std::cout << "Is empty after enqueue: " << q->arrEmpty() << std::endl;
    std::cout << "Length after enqueue: " << q->arrLength() << std::endl;
    
    q->printQueue();

    q->dequeueArr();
    std::cout << "Queue after dequeue: " << std::endl;
    q->printQueue();
    std::cout << "Length after dequeue: " << q->arrLength() << std::endl;

    std::cout << "Peek: " << q->peekArr() << std::endl;

// stack array tests
    std::cout << "\nStacks array test:" << std::endl;
    StackArray* s = new StackArray();
    std::cout << "Is empty before push: " << s->arrEmpty() << std::endl;

    s->pushArr(4);
    s->pushArr(13);
    s->pushArr(6);
    s->pushArr(54);
    s->pushArr(2);

    std::cout << "Is empty after push: " << s->arrEmpty() << std::endl;
    std::cout << "Length after push: " << s->arrLength() << std::endl;
    
    s->printStack();
    std::cout << "Pop: " << s->popArr() << std::endl;
    
    std::cout << "Stack after pop: " << std::endl;
    s->printStack();
    std::cout << "Length after pop: " << s->arrLength() << std::endl;

    std::cout << "Peek: " << s->peekArr() << std::endl;

// //deques array test
    std::cout << "\nDeques array test:" << std::endl;
    DequesArray* d = new DequesArray();

    d->pushBackArr(12);
    d->pushBackArr(3);
    d->pushBackArr(2);
    std::cout << "Deque: " << std::endl;
    d->printDeque();
    std::cout << "Length: " << d->getLengthArr() << std::endl;


    d->pushFrontArr(10);
    std::cout << "Deque after pushFront: " << std::endl;
    d->printDeque();
    std::cout << "Length: " << d->getLengthArr() << std::endl;

    std::cout << "Peek front: " << d->peekFrontArr() << std::endl;
    std::cout << "Peek back: " << d->peekBackArr() << std::endl;
    std::cout << "Pop front: " << d->popFrontArr() << std::endl;
    std::cout << "Pop back: " << d->popBackArr() << std::endl;

// deques linked list test
    std::cout << "\nDeques linked list test:" << std::endl;
    Deques* dl = new Deques();
    Node* node1 = new Node(2);
    Node* node2 = new Node(5);
    Node* node3 = new Node(8);

    dl->pushBack(node1);
    dl->pushBack(node2);
    dl->pushFront(node3);

    std::cout << "Peek front: " << dl->peekFront() << std::endl;
    std::cout << "Pop back: " << dl->popBack() << std::endl;


// queues linked list test
    std::cout << "\nQueues linked list test:" << std::endl;
    Queue* qList = new Queue();
    qList->enqueue(12);
    qList->enqueue(5);
    qList->enqueue(10);
    std::cout << "Queue linked list: " << std::endl;
    qList->print();

    std::cout << "Dequeue: " << qList->dequeue() << std::endl;
    std::cout << "After dequeue: " << std::endl;
    qList->print();
    std::cout << "Peek: " << qList->dequeue() << std::endl;
    std::cout << "Get length: " << qList->dequeue() << std::endl;

    

// stacks linked list test
    std::cout << "\nStacks linked list test:" << std::endl;
    Stack* sList = new Stack();

    sList->push(8);
    sList->push(10);
    sList->push(7);
    std::cout << "Stack linked list: " << std::endl;
    sList->print();

    std::cout << "Pop: " << sList->pop() << std::endl;
    std::cout << "Stack after pop: " << std::endl;
    sList->print();

    std::cout << "Peek: " << sList->peek() << std::endl; 
    std::cout << "Length: " << sList->getLength() << std::endl;   

}