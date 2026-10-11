#include <iostream>

class Node {
    public: 
        int val;
        Node* left;
        Node* right;
};

class BinarySearchTree {

    Node* root; 

    public: 
        void insert(Node* newNode) {
            if (root == nullptr) {
                root = newNode;
            } else {
                Node* curr = root;
                while (curr != nullptr) {
                    if (newNode->val < curr->val) {
                        if (curr->left == nullptr) {
                            curr->left = newNode;
                        } else {
                            curr = curr->left;
                        }
                    } else if (newNode->val > curr->val) {
                        if (curr->right == nullptr) {
                            curr->right = newNode;
                        } else {
                            curr = curr->right;
                        }
                    }
                }
            }
        }

        Node* search(int toFind) {
            Node* curr = root;
            while (curr != nullptr) {
                if (curr->val == toFind) {
                    return curr;
                } else if (toFind < curr->val) {
                    curr = curr->left;
                } else {
                    curr = curr->right;
                }
            }

            return nullptr;
        }

        void remove(Node* node) {
            int toRemove = node->val;
            delete search(toRemove);
            std::cout << "Removed " << node->val << std::endl;
        }

        void inorderTraversal() { // starting from the smallest to the largest 
            
        }

        void preorderTraversal() {

        }

        void postorderTraversal() {

        }

        int height() {
            if (root->left == nullptr && root->right == nullptr) {
                return 0;
            }            
            
            int leftCount = 0;
            int rightCount = 0;

            Node* curr = root;
            while (curr != nullptr) {
                leftCount++;
                curr = curr->left;
            }

            while (curr != nullptr) {
                rightCount++;
                curr = curr->right;
            }

            if (leftCount > rightCount) {
                return leftCount;
            } else if (rightCount > leftCount) {
                return rightCount;
            } else {
                return rightCount;
            }
        }
};

int main() {

}