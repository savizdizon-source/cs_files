#include <iostream> 
#include <vector>
#include <stdexcept>

class MinHeap {
    std::vector<int> minheap; // index 0 is the root

    public:

    void insert(int value) {
        minheap.push_back(value); // setting last index to the given value
        int temp = 0;
        int i = minheap.size() - 1;
        while (i > 0) {
            int parent = (i - 1) / 2;
            if (minheap.at(i) < minheap.at(parent)) {
                temp = minheap.at(parent);
                minheap.at(parent) = minheap.at(i);
                minheap.at(i) = temp;
                i = parent;
            } else {
                break;
            }
        }

    }

    int peekMin() {
        return minheap.at(0);
    }

    int removeMin() {
        // remove the root
        int toReturn = minheap.at(0); // the top of the heap
        minheap.at(0) = minheap.at(minheap.size() - 1); // making the root the last node
        minheap.pop_back();

        int size = minheap.size();
        int i = 0;

        while (true) {
            int left = 2 * i + 1;
            int right = 2 * i + 2;
            int smallest = i;

            if (left < size && minheap.at(left) < minheap.at(smallest)) {
                smallest = left;
            }
            if (right < size && minheap.at(right) < minheap.at(smallest)) {
                smallest = right;
            }

            if (smallest == i) {
                break; 
            }

            int temp = minheap.at(i);
            minheap.at(i) = minheap.at(smallest);
            minheap.at(smallest) = temp;
            i = smallest;
        }

        return toReturn;
    }

    void printHeap() {
        for (int num : minheap) {
            std::cout << num << std::endl;
        }
    }
};

class MaxHeap {
    std::vector<int> maxheap; // index 0 is the root

    public:

    void insert(int value) {
        maxheap.push_back(value); // setting last index to the given value
        int temp = 0;
        int i = maxheap.size() - 1;
        while (i > 0) {
            int parent = (i - 1) / 2;
            if (maxheap.at(i) > maxheap.at(parent)) {
                temp = maxheap.at(parent);
                maxheap.at(parent) = maxheap.at(i);
                maxheap.at(i) = temp;
                i = parent;
            } else {
                break;
            }
        }

    }

    int peekMax() {
        return maxheap.at(0);
    }

    int removeMax() {
        // remove the root
        int toReturn = maxheap.at(0); // the top of the heap
        maxheap.at(0) = maxheap.at(maxheap.size() - 1); // making the root the last node
        maxheap.pop_back(); // removing the last thing in the vector

        int size = maxheap.size();
        int i = 0;

        while (true) {
            int left = 2 * i + 1;
            int right = 2 * i + 2;
            int largest = i;

            if (left < size && maxheap.at(left) > maxheap.at(largest)) {
                largest = left;
            }
            if (right < size && maxheap.at(right) > maxheap.at(largest)) {
                largest = right;
            }

            if (largest == i) {
                break; 
            }

            int temp = maxheap.at(i);
            maxheap.at(i) = maxheap.at(largest);
            maxheap.at(largest) = temp;
            i = largest;
        }

        return toReturn;
    }

    void printHeap() {
        for (int num : maxheap) {
            std::cout << num << std::endl;
        }
    } 
};

class Treap {
private:
    struct Node {
        int key;
        int priority;
        Node* left;
        Node* right;

        Node(int newKey, int newPriority) {
            key = newKey;
            priority = newPriority;
            left = nullptr;
            right = nullptr;
        }
    };

    Node* root;

    Node* rotateRight(Node* top) {
        Node* newTop = top->left;
        top->left = newTop->right;
        newTop->right = top;
        return newTop;
    }

    Node* rotateLeft(Node* top) {
        Node* newTop = top->right;
        top->right = newTop->left;
        newTop->left = top;
        return newTop;
    }

    Node* insertNode(Node* current, int key, int priority) {
        if (current == nullptr) {
            return new Node(key, priority);
        }

        if (key < current->key) {
            current->left = insertNode(current->left, key, priority);
            if (current->left->priority < current->priority) {
                current = rotateRight(current);
            }
        } else if (key > current->key) {
            current->right = insertNode(current->right, key, priority);
            if (current->right->priority < current->priority) {
                current = rotateLeft(current);
            }
        }

        return current;
    }

    void printInOrderFrom(Node* current) {
        if (current == nullptr) {
            return;
        }
        printInOrderFrom(current->left);
        std::cout << current->key << "(" << current->priority << ") ";
        printInOrderFrom(current->right);
    }

    void printTreeFrom(Node* current, int depth) {
        if (current == nullptr) {
            return;
        }
        printTreeFrom(current->right, depth + 1);
        for (int i = 0; i < depth; i++) {
            std::cout << "      ";
        }
        std::cout << current->key << "(" << current->priority << ")" << std::endl;
        printTreeFrom(current->left, depth + 1);
    }

    void deleteAllFrom(Node* current) {
        if (current == nullptr) {
            return;
        }
        deleteAllFrom(current->left);
        deleteAllFrom(current->right);
        delete current;
    }

    Node* removeNode(Node* current, int key) {
        if (current == nullptr) {
            return nullptr;
        }

        if (key < current->key) {
            current->left = removeNode(current->left, key);
        } else if (key > current->key) {
            current->right = removeNode(current->right, key);
        } else {
            if (current->left == nullptr && current->right == nullptr) {
                delete current;
                return nullptr;
            }

            if (current->left == nullptr) {
                current = rotateLeft(current);
                current->left = removeNode(current->left, key);
            } else if (current->right == nullptr) {
                current = rotateRight(current);
                current->right = removeNode(current->right, key);
            } else if (current->left->priority < current->right->priority) {
                current = rotateRight(current);
                current->right = removeNode(current->right, key);
            } else {
                current = rotateLeft(current);
                current->left = removeNode(current->left, key);
            }
        }

        return current;
    }

public:

    int count = 0;

    Treap() {
        root = nullptr;
    }

    ~Treap() {
        deleteAllFrom(root);
    }

    int getRootKey() {
        if (root == nullptr) {
            throw std::out_of_range("Treap is empty");
        }
        return root->key;
    }

    int getRootPriority() {
        if (root == nullptr) {
            throw std::out_of_range("Treap is empty");
        }
        return root->priority;
    }

    bool insert(int key, int priority) {
        if (search(key)) {
            return false;
        }
        root = insertNode(root, key, priority);
        count++;
        return true;
    }

    bool search(int key) {
        Node* current = root;
        while (current != nullptr) {
            if (key == current->key) {
                return true;
            }
            if (key < current->key) {
                current = current->left;
            } else {
                current = current->right;
            }
        }
        return false;
    }

    void printInOrder() {
        printInOrderFrom(root);
        std::cout << std::endl;
    }

    void printTree() {
        printTreeFrom(root, 0);
    }

    bool remove(int key) {
        if (search(key) == false) {
            return false;
        }
        root = removeNode(root, key);
        count--;
        return true;
    }
};

class PriorityQueue { 

    public:

    Treap treap;

    bool enqueue(int item, int priority) {
        return treap.insert(item, priority);
    }

    int dequeue() {
        int toReturn = treap.getRootKey();
        treap.remove(toReturn);
        return toReturn;
    }

    int peekQueue() {
        return treap.getRootKey();
    }

    bool isEmpty() {
        if (treap.count <= 0) {
            return true;
        }

        return false;
    }

    int getLength() {
        return treap.count;
    }
};

int main() {
    // min heap tests
    MinHeap minHeap;
    int heapValues[] = {50, 30, 20, 15, 10, 8, 16, 40, 5, 25, 60};
    for (int value : heapValues) {
        minHeap.insert(value);
    }

    std::cout << "Min Heap:" << std::endl;
    minHeap.printHeap();

    std::cout << "peekMin: " << minHeap.peekMin() << " | expected 5" << std::endl;
    std::cout << "peekMin again: " << minHeap.peekMin() << " | expected 5" << std::endl;
    std::cout << "removeMin: " << minHeap.removeMin() << " | expected 5" << std::endl;
    std::cout << "removeMin: " << minHeap.removeMin() << " | expected 8" << std::endl;
    std::cout << "removeMin: " << minHeap.removeMin() << " | expected 10" << std::endl;
    std::cout << "peekMin: " << minHeap.peekMin() << " | expected 15" << std::endl;

    minHeap.insert(1);
    std::cout << "peekMin after inserting 1: " << minHeap.peekMin() << " | expected 1" << std::endl;
    std::cout << "removeMin: " << minHeap.removeMin() << " | expected 1" << std::endl;
    std::cout << "removeMin: " << minHeap.removeMin() << " | expected 15" << std::endl;

    MinHeap emptyMinHeap;
    try {
        emptyMinHeap.removeMin();
        std::cout << "removeMin on empty heap: no error | expected an error" << std::endl;
    } catch (std::out_of_range&) {
        std::cout << "removeMin on empty heap: error thrown | expected an error" << std::endl;
    }

    std::cout << std::endl;

    // max heap tests
    MaxHeap maxHeap;
    for (int value : heapValues) {
        maxHeap.insert(value);
    }

    std::cout << "Max Heap:" << std::endl;
    maxHeap.printHeap();

    std::cout << "peekMax: " << maxHeap.peekMax() << " | expected 60" << std::endl;
    std::cout << "peekMax again: " << maxHeap.peekMax() << " | expected 60" << std::endl;
    std::cout << "removeMax: " << maxHeap.removeMax() << " | expected 60" << std::endl;
    std::cout << "removeMax: " << maxHeap.removeMax() << " | expected 50" << std::endl;
    std::cout << "removeMax: " << maxHeap.removeMax() << " | expected 40" << std::endl;
    std::cout << "peekMax: " << maxHeap.peekMax() << " | expected 30" << std::endl;

    maxHeap.insert(99);
    std::cout << "peekMax after inserting 99: " << maxHeap.peekMax() << " | expected 99" << std::endl;
    std::cout << "removeMax: " << maxHeap.removeMax() << " | expected 99" << std::endl;
    std::cout << "removeMax: " << maxHeap.removeMax() << " | expected 30" << std::endl;

    MaxHeap emptyMaxHeap;
    try {
        emptyMaxHeap.removeMax();
        std::cout << "removeMax on empty heap: no error | expected an error" << std::endl;
    } catch (std::out_of_range&) {
        std::cout << "removeMax on empty heap: error thrown | expected an error" << std::endl;
    }

    std::cout << std::endl;

    // treap tests
    Treap treap;
    int keys[] = {50, 30, 70, 20, 40, 60, 80, 10, 35, 65};
    int priorities[] = {15, 8, 22, 3, 12, 9, 17, 25, 5, 1};
    for (int i = 0; i < 10; i++) {
        treap.insert(keys[i], priorities[i]);
    }

    std::cout << "Treap In-order (key(priority)):" << std::endl;
    treap.printInOrder();
    std::cout << "Tree sideways:" << std::endl;
    treap.printTree();

    std::cout << "count: " << treap.count << " | expected 10" << std::endl;
    std::cout << "search 40: " << (treap.search(40) ? "found" : "not found") << " | expected found" << std::endl;
    std::cout << "search 65: " << (treap.search(65) ? "found" : "not found") << " | expected found" << std::endl;
    std::cout << "search 10: " << (treap.search(10) ? "found" : "not found") << " | expected found" << std::endl;
    std::cout << "search 99: " << (treap.search(99) ? "found" : "not found") << " | expected not found" << std::endl;
    std::cout << "search 45: " << (treap.search(45) ? "found" : "not found") << " | expected not found" << std::endl;
    std::cout << "root key: " << treap.getRootKey() << " | expected 65" << std::endl;
    std::cout << "root priority: " << treap.getRootPriority() << " | expected 1" << std::endl;

    std::cout << "insert duplicate key 50: " << (treap.insert(50, 99) ? "inserted" : "rejected") << " | expected rejected" << std::endl;
    std::cout << "count after duplicate: " << treap.count << " | expected 10" << std::endl;

    std::cout << "remove 65 (root): " << (treap.remove(65) ? "removed" : "not found") << " | expected removed" << std::endl;
    std::cout << "search 65 after remove: " << (treap.search(65) ? "found" : "not found") << " | expected not found" << std::endl;
    std::cout << "count: " << treap.count << " | expected 9" << std::endl;
    std::cout << "new root key: " << treap.getRootKey() << " | expected 20" << std::endl;
    std::cout << "remove 10 (leaf): " << (treap.remove(10) ? "removed" : "not found") << " | expected removed" << std::endl;
    std::cout << "count: " << treap.count << " | expected 8" << std::endl;
    std::cout << "remove 99 (not in treap): " << (treap.remove(99) ? "removed" : "not found") << " | expected not found" << std::endl;
    std::cout << "count: " << treap.count << " | expected 8" << std::endl;

    std::cout << "In-order after removals:" << std::endl;
    treap.printInOrder();
    std::cout << "Tree sideways after removals:" << std::endl;
    treap.printTree();

    Treap rotationTreap;
    rotationTreap.insert(1, 5);
    rotationTreap.insert(2, 4);
    rotationTreap.insert(3, 3);
    rotationTreap.insert(4, 2);
    rotationTreap.insert(5, 1);
    std::cout << "rotation treap root: " << rotationTreap.getRootKey() << " | expected 5" << std::endl;
    std::cout << "rotation treap search 1: " << (rotationTreap.search(1) ? "found" : "not found") << " | expected found" << std::endl;
    std::cout << "rotation treap search 3: " << (rotationTreap.search(3) ? "found" : "not found") << " | expected found" << std::endl;

    Treap emptyTreap;
    std::cout << "empty treap count: " << emptyTreap.count << " | expected 0" << std::endl;
    std::cout << "remove on empty treap: " << (emptyTreap.remove(5) ? "removed" : "not found") << " | expected not found" << std::endl;
    try {
        emptyTreap.getRootKey();
        std::cout << "getRootKey on empty treap: no error | expected an error" << std::endl;
    } catch (std::out_of_range&) {
        std::cout << "getRootKey on empty treap: error thrown | expected an error" << std::endl;
    }

    std::cout << std::endl;

    // priority queue tests
    PriorityQueue queue;

    std::cout << "Priority queue empty: " << (queue.isEmpty() ? "yes" : "no") << " | expected yes" << std::endl;
    std::cout << "Priority queue length: " << queue.getLength() << " | expected 0" << std::endl;

    queue.enqueue(100, 3);
    queue.enqueue(200, 1);
    queue.enqueue(300, 2);
    queue.enqueue(400, 5);
    queue.enqueue(500, 4);

    std::cout << "empty after enqueues: " << (queue.isEmpty() ? "yes" : "no") << " | expected no" << std::endl;
    std::cout << "length: " << queue.getLength() << " | expected 5" << std::endl;
    std::cout << "peekQueue: " << queue.peekQueue() << " | expected 200" << std::endl;
    std::cout << "enqueue duplicate item 100: " << (queue.enqueue(100, 9) ? "added" : "rejected") << " | expected rejected" << std::endl;
    std::cout << "length: " << queue.getLength() << " | expected 5" << std::endl;

    std::cout << "dequeue: " << queue.dequeue() << " | expected 200" << std::endl;
    std::cout << "length: " << queue.getLength() << " | expected 4" << std::endl;
    std::cout << "dequeue: " << queue.dequeue() << " | expected 300" << std::endl;

    queue.enqueue(600, 0);
    std::cout << "peekQueue after enqueuing priority 0: " << queue.peekQueue() << " | expected 600" << std::endl;
    std::cout << "dequeue: " << queue.dequeue() << " | expected 600" << std::endl;
    std::cout << "dequeue: " << queue.dequeue() << " | expected 100" << std::endl;
    std::cout << "dequeue: " << queue.dequeue() << " | expected 500" << std::endl;
    std::cout << "dequeue: " << queue.dequeue() << " | expected 400" << std::endl;

    std::cout << "empty after all dequeues: " << (queue.isEmpty() ? "yes" : "no") << " | expected yes" << std::endl;
    std::cout << "length: " << queue.getLength() << " | expected 0" << std::endl;

    try {
        queue.dequeue();
        std::cout << "dequeue on empty queue: no error | expected an error" << std::endl;
    } catch (std::out_of_range&) {
        std::cout << "dequeue on empty queue: error thrown | expected an error" << std::endl;
    }

    return 0;
}

