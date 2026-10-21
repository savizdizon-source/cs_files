#include <iostream> 
#include <vector>

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

public:
    Treap() {
        root = nullptr;
    }

    ~Treap() {
        deleteAllFrom(root);
    }

    int getRootKey() {
        return root->key;
    }

    void insert(int key, int priority) {
        root = insertNode(root, key, priority);
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
};

int main() {
    // minheap tests
    MinHeap heap;

    int values[] = {50, 30, 20, 15, 10, 8, 16, 40, 5, 25, 60};
    for (int value : values) {
        heap.insert(value);
    }

    std::cout << "Heap after insert:" << std::endl;
    heap.printHeap();

    std::cout << "Peek Minheap: " << heap.peekMin() << std::endl;

    std::cout << "Remove Minheap: " << heap.removeMin() << std::endl;
    std::cout << "Remove Minheap: " << heap.removeMin() << std::endl;
    std::cout << "Remove Minheap: " << heap.removeMin() << std::endl;

    std::cout << "Heap after three removals:" << std::endl;
    heap.printHeap();

    heap.insert(1);
    std::cout << "Peek after inserting 1: " << heap.peekMin() << std::endl;

    std::cout << std::endl;

    // treap tests
    Treap treap;

    int keys[] = {50, 30, 70, 20, 40, 60, 80, 10, 35, 65};
    int priorities[] = {15, 8, 22, 3, 12, 9, 17, 25, 5, 1};
    for (int i = 0; i < 10; i++) {
        treap.insert(keys[i], priorities[i]);
    }

    std::cout << "In-order, shown as key(priority):" << std::endl;
    treap.printInOrder();

    std::cout << std::endl;
    std::cout << "Tree sideways (root on the left):" << std::endl;
    treap.printTree();

    std::cout << std::endl;
    std::cout << "Search 40: " << (treap.search(40) ? "found" : "not found") << std::endl;
    std::cout << "Search 65: " << (treap.search(65) ? "found" : "not found") << std::endl;
    std::cout << "Search 99: " << (treap.search(99) ? "found" : "not found") << std::endl;

    return 0;
}