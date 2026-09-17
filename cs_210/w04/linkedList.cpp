class Node {
    public:
        int value;
        int* valuep = &value;
        Node* next;
    
        Node(int* num) {
            valuep = num;
            next = nullptr;
        }
};

class SinglyLinkedList {
    private:
        Node* head;
        Node* tail;
        Node* current; 

    public:
        SinglyLinkedList() {
            head = nullptr;
            tail = nullptr;
        }

        Node* search(int val) {
            Node* currentNode = head;
            while (currentNode) {
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
};

int main() {
    
}