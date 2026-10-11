#include <iostream>

class Node {
    public: 
        int val;
        Node* left = nullptr;
        Node* right = nullptr;

        Node(int num) {
            val = num;
            left = nullptr;
            right = nullptr;
        }
};

class BinarySearchTree {

    Node* root = nullptr; 

    public: 
        void insert(Node* newNode) {
            if (root == nullptr) {
                root = newNode;
                return;
            }

            Node* curr = root;
            while (true) {
                if (newNode->val < curr->val) {
                    if (curr->left == nullptr) {
                        curr->left = newNode;
                        return;
                    }
                    curr = curr->left;
                } else if (newNode->val > curr->val) {
                    if (curr->right == nullptr) {
                        curr->right = newNode;
                        return;
                    }
                    curr = curr->right;
                } else {
                    return;
                }
            }
        }

        int search(Node* node) {
            Node* curr = root;
            int toFind = node->val;
            while (curr != nullptr) {
                if (curr->val == toFind) {
                    return curr->val;
                } else if (toFind < curr->val) {
                    curr = curr->left;
                } else {
                    curr = curr->right;
                }
            }

            return -1;
        }

        bool remove(int key) {
            Node* parent = nullptr;
            Node* currentNode = root;
        
            while (currentNode) { 
                if (currentNode->val == key) {
                    if (currentNode->left == nullptr && currentNode->right == nullptr) {
                        if (parent == nullptr) { 
                           root = nullptr;
                        }
                        else if (parent->left == currentNode) { 
                           parent->left = nullptr;
                        }
                        else {
                           parent->right = nullptr;
                        }
                        delete currentNode;
                        return true; 
                    } else if (currentNode->left && currentNode->right == nullptr) {
                        if (parent == nullptr) { 
                           root = currentNode->left;
                        }
                        else if (parent->left == currentNode) {
                           parent->left = currentNode->left;
                        }
                        else {
                           parent->right = currentNode->left;
                        }
                        delete currentNode;
                        return true; 
                    } else if (currentNode->left == nullptr && currentNode->right) {
                        if (parent == nullptr) { 
                           root = currentNode->right;
                        }
                        else if (parent->left == currentNode) {
                           parent->left = currentNode->right;
                        }
                        else {
                           parent->right = currentNode->right;
                        }
                        delete currentNode;
                        return true; 
                    } else {
                        Node* successor = currentNode->right;
                        while (successor->left) {
                           successor = successor->left;
                        }
                        currentNode->val = successor->val;
                        parent = currentNode;
                    
                        currentNode = currentNode->right;
                        key = successor->val;
                    }
                } else if (currentNode->val < key) {
                    parent = currentNode;
                    currentNode = currentNode->right;
                } else {
                    parent = currentNode;
                    currentNode = currentNode->left;
                }
            }
            return false; 
        }

        void inorderTraversal(Node* node) { // starting from the smallest to the largest 
            if (node == nullptr) {
                return;
            }

            inorderTraversal(node->left);
            std::cout << node->val << " " << std::endl;
            inorderTraversal(node->right);
        }

        void preorderTraversal(Node* node) {
            if (node == nullptr) {
                return;
            }

            std::cout << node->val << " " << std::endl;
            preorderTraversal(node->left);
            preorderTraversal(node->right);
        }

        void postorderTraversal(Node* node) {
            if (node == nullptr) {
                return;
            }
        
            postorderTraversal(node->left);
            postorderTraversal(node->right);
            std::cout << node->val << " " << std::endl;
        }

        int height(Node* node) {
            if (node == nullptr) {
                return -1;
            }

            return 1 + std::max(height(node->left), height(node->right));
        }
};

int main() {
    Node* n1 = new Node(3);
    Node* n2 = new Node(5);
    Node* n3 = new Node(12);
    Node* n4 = new Node(7);

    BinarySearchTree* tree = new BinarySearchTree();

    tree->insert(n1);
    tree->insert(n2);
    tree->insert(n3);
    tree->insert(n4);

    std::cout << "Searching for node4 value (7): " << tree->search(n4) << std::endl;
    std::cout << std::endl;
    std::cout << "Preorder traversal: " << std::endl;
    tree->preorderTraversal(n1);

    std::cout << std::endl;
    std::cout << "Inorder traversal: " << std::endl;
    tree->inorderTraversal(n1);

    std::cout << std::endl;
    std::cout << "Postorder traversal: " << std::endl;
    tree->postorderTraversal(n1);

    std::cout << "\nHeight before remove: " << tree->height(n1) << std::endl;

    tree->remove(5);
    std::cout << "\nHeight after remove: " << tree->height(n1) << std::endl;
}