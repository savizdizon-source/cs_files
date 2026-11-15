#include <iostream>
#include <algorithm>   // for std::max (used in height)

class Node {
    public:
        int val;
        Node* left;
        Node* right;

        Node(int num) {
            val = num;
            left = nullptr;
            right = nullptr;
        }
};

class BinarySearchTree {
    private:
        Node* root;
        void inorderHelper(Node* node) {
            if (node == nullptr) {
                return;
            }
            inorderHelper(node->left); // smaller values first
            std::cout << node->val << " ";
            inorderHelper(node->right); // then larger values
        }

        void preorderHelper(Node* node) {
            if (node == nullptr) {
                return;
            }
            std::cout << node->val << " "; // visit node first
            preorderHelper(node->left);
            preorderHelper(node->right);
        }

        void postorderHelper(Node* node) {
            if (node == nullptr) {
                return;
            }
            postorderHelper(node->left);
            postorderHelper(node->right);
            std::cout << node->val << " "; // visit node last
        }

        int heightHelper(Node* node) {
            if (node == nullptr) {
                return -1; // empty tree has height -1
            }
            return 1 + std::max(heightHelper(node->left), heightHelper(node->right));
        }

        void destroy(Node* node) {
            if (node == nullptr) {
                return;
            }
            destroy(node->left);
            destroy(node->right);
            delete node;
        }

    public:
        BinarySearchTree() {
            root = nullptr;
        }

        ~BinarySearchTree() {
            destroy(root);
        }

        void insert(int value) {
            if (root == nullptr) {
                root = new Node(value);
                return;
            }

            Node* curr = root;
            while (true) {
                if (value < curr->val) {
                    if (curr->left == nullptr) {
                        curr->left = new Node(value);
                        return;
                    }
                    curr = curr->left;
                } else if (value > curr->val) {
                    if (curr->right == nullptr) {
                        curr->right = new Node(value);
                        return;
                    }
                    curr = curr->right;
                } else {
                    return;     // value already in the tree
                }
            }
        }

        bool search(int key) {
            Node* curr = root;
            while (curr != nullptr) {
                if (key == curr->val) {
                    return true;
                } else if (key < curr->val) {
                    curr = curr->left;
                } else {
                    curr = curr->right;
                }
            }
            return false;
        }

        bool remove(int key) {
            Node* parent = nullptr;
            Node* curr = root;
            while (curr != nullptr && curr->val != key) {
                parent = curr;
                if (key < curr->val) {
                    curr = curr->left;
                } else {
                    curr = curr->right;
                }
            }

            if (curr == nullptr) {
                return false;   
            }

            if (curr->left != nullptr && curr->right != nullptr) {
                Node* successorParent = curr;
                Node* successor = curr->right;
                while (successor->left != nullptr) {
                    successorParent = successor;
                    successor = successor->left;
                }
                curr->val = successor->val;

                parent = successorParent;
                curr = successor;
            }

            Node* child;
            if (curr->left != nullptr) {
                child = curr->left;
            } else {
                child = curr->right;    
            }

            if (parent == nullptr) {
                root = child;         
            } else if (parent->left == curr) {
                parent->left = child;
            } else {
                parent->right = child;
            }

            delete curr;
            return true;
        }

        void inorderTraversal()   { inorderHelper(root);   std::cout << std::endl; }
        void preorderTraversal()  { preorderHelper(root);  std::cout << std::endl; }
        void postorderTraversal() { postorderHelper(root); std::cout << std::endl; }

        int height() {
            return heightHelper(root);
        }
};

void showSearch(BinarySearchTree& tree, int key) {
    std::cout << "Search for " << key << ": "
              << (tree.search(key) ? "FOUND" : "NOT FOUND") << std::endl;
}

int main() {
    BinarySearchTree tree;

    // inserting integers
    int values[] = {50, 30, 70, 20, 40, 60, 80, 35, 65, 10};
    for (int v : values) {
        tree.insert(v);
    }

    std::cout << "Preorder:  ";  tree.preorderTraversal();
    std::cout << "Inorder:   ";  tree.inorderTraversal();
    std::cout << "Postorder: ";  tree.postorderTraversal();
    std::cout << "Height: " << tree.height() << std::endl;

    std::cout << "\nSearch" << std::endl;
    showSearch(tree, 60);  
    showSearch(tree, 99);   

    std::cout << "\nRemove 10 (leaf)" << std::endl;
    tree.remove(10);
    std::cout << "Inorder: ";  tree.inorderTraversal();

    std::cout << "\nRemove 40 (one child: 35)" << std::endl;
    tree.remove(40);
    std::cout << "Inorder: ";  tree.inorderTraversal();

    std::cout << "\nRemove 50 (two children, the root)" << std::endl;
    tree.remove(50);
    std::cout << "Inorder: ";  tree.inorderTraversal();

    std::cout << "\nRemove 999 (absent key)" << std::endl;
    std::cout << "remove returned: " << (tree.remove(999) ? "true" : "false") << std::endl;

    std::cout << "\nFinal height: " << tree.height() << std::endl;

    return 0;
}