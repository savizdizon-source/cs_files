#include <iostream>
#include <vector>
#include <string>

class BTreeNode {
    public:
        std::vector<int> keys;
        std::vector<BTreeNode*> children;
        bool isLeaf;

        BTreeNode(bool leaf) {
            isLeaf = leaf;
        }
};

class BTree {
    private:
        BTreeNode* root;
        int t;

        // Position of the first key that is >= k (or keys.size() if none).
        // This is the index of the child we should go down into, too.
        int findPosition(BTreeNode* node, int k) {
            int i = 0;
            while (i < (int)node->keys.size() && node->keys[i] < k) {
                i++;
            }
            return i;
        }

        bool isFull(BTreeNode* node) {
            return (int)node->keys.size() == 2 * t - 1;
        }

        void splitChild(BTreeNode* parent, int i) {
            BTreeNode* child = parent->children[i];
            BTreeNode* newNode = new BTreeNode(child->isLeaf);

            int middleKey = child->keys[t - 1];

            // newNode gets the upper half of the keys (after the middle)
            newNode->keys.assign(child->keys.begin() + t, child->keys.end());

            // If child is not a leaf, newNode also gets the upper half of the children
            if (!child->isLeaf) {
                newNode->children.assign(child->children.begin() + t, child->children.end());
                child->children.resize(t);
            }

            // child keeps only the lower half of its keys
            child->keys.resize(t - 1);

            // The middle key moves up into the parent, and newNode
            // becomes the child just to the right of the old child.
            parent->keys.insert(parent->keys.begin() + i, middleKey);
            parent->children.insert(parent->children.begin() + i + 1, newNode);
        }

        void insertNonFull(BTreeNode* node, int k) {
            int i = findPosition(node, k);

            if (node->isLeaf) {
                node->keys.insert(node->keys.begin() + i, k);
                return;
            }

            // Not a leaf: choose the child to go into
            if (isFull(node->children[i])) {
                splitChild(node, i);
                // The middle key moved up into node->keys[i].
                // If k is bigger than it, go to the right child instead.
                if (k > node->keys[i]) {
                    i++;
                }
            }
            insertNonFull(node->children[i], k);
        }

        bool searchHelper(BTreeNode* node, int k) {
            int i = findPosition(node, k);

            if (i < (int)node->keys.size() && node->keys[i] == k) {
                return true;            // found it in this node
            }
            if (node->isLeaf) {
                return false;           // nowhere left to look
            }
            return searchHelper(node->children[i], k);
        }

        // Inorder: child 0, key 0, child 1, key 1, ..., last child
        void inorderHelper(BTreeNode* node, std::vector<int>& result) {
            for (int i = 0; i < (int)node->keys.size(); i++) {
                if (!node->isLeaf) {
                    inorderHelper(node->children[i], result);
                }
                result.push_back(node->keys[i]);
            }
            if (!node->isLeaf) {
                inorderHelper(node->children.back(), result);
            }
        }

        void printHelper(BTreeNode* node, int depth) {
            std::cout << std::string(depth * 4, ' ') << "[";
            for (int i = 0; i < (int)node->keys.size(); i++) {
                if (i > 0) {
                    std::cout << " ";
                }
                std::cout << node->keys[i];
            }
            std::cout << "]" << std::endl;

            if (!node->isLeaf) {
                for (int i = 0; i < (int)node->children.size(); i++) {
                    printHelper(node->children[i], depth + 1);
                }
            }
        }

        // Checks the B-tree rules for one node, then its children.
        bool checkNode(BTreeNode* node, bool isRoot, int depth, int& leafDepth) {
            int numKeys = (int)node->keys.size();

            if (numKeys > 2 * t - 1) {
                return false;                       // too many keys
            }
            if (!isRoot && numKeys < t - 1) {
                return false;                       // too few keys
            }
            for (int i = 1; i < numKeys; i++) {
                if (node->keys[i - 1] >= node->keys[i]) {
                    return false;                   // keys not sorted
                }
            }

            if (node->isLeaf) {
                if (leafDepth == -1) {
                    leafDepth = depth;
                } else if (leafDepth != depth) {
                    return false;                   // leaves at different depths
                }
                return true;
            }

            if ((int)node->children.size() != numKeys + 1) {
                return false;                       // wrong number of children
            }
            for (int i = 0; i < (int)node->children.size(); i++) {
                if (!checkNode(node->children[i], false, depth + 1, leafDepth)) {
                    return false;
                }
            }
            return true;
        }

        void destroy(BTreeNode* node) {
            if (!node->isLeaf) {
                for (int i = 0; i < (int)node->children.size(); i++) {
                    destroy(node->children[i]);
                }
            }
            delete node;
        }

    public:
        BTree(int minDegree) {
            t = minDegree;
            root = new BTreeNode(true);     // start with an empty leaf
        }

        ~BTree() {
            destroy(root);
        }

        // Returns true if k was inserted, false if it was already there
        bool insert(int k) {
            if (search(k)) {
                return false;               // keep keys distinct
            }

            // If the root is full, the tree grows taller by one level:
            // make a new empty root and split the old root under it.
            if (isFull(root)) {
                BTreeNode* newRoot = new BTreeNode(false);
                newRoot->children.push_back(root);
                splitChild(newRoot, 0);
                root = newRoot;
            }
            insertNonFull(root, k);
            return true;
        }

        bool search(int k) {
            return searchHelper(root, k);
        }

        // All keys in sorted order
        std::vector<int> inorder() {
            std::vector<int> result;
            inorderHelper(root, result);
            return result;
        }

        // Number of edges from the root down to a leaf (a lone root has height 0)
        int height() {
            int h = 0;
            BTreeNode* curr = root;
            while (!curr->isLeaf) {
                curr = curr->children[0];
                h++;
            }
            return h;
        }

        // True if every B-tree rule holds
        bool isValid() {
            int leafDepth = -1;
            return checkNode(root, true, 0, leafDepth);
        }

        void print() {
            printHelper(root, 0);
        }
};

int testsPassed = 0;
int testsFailed = 0;

void check(const std::string& name, bool condition) {
    if (condition) {
        std::cout << "  PASS: " << name << std::endl;
        testsPassed++;
    } else {
        std::cout << "  FAIL: " << name << std::endl;
        testsFailed++;
    }
}

bool isSorted(const std::vector<int>& v) {
    for (int i = 1; i < (int)v.size(); i++) {
        if (v[i - 1] >= v[i]) {
            return false;
        }
    }
    return true;
}

int main() {

    std::cout << "Test 1: empty tree (t = 2)" << std::endl;
    {
        BTree tree(2);
        check("search(5) is false", tree.search(5) == false);
        check("inorder is empty", tree.inorder().empty());
        check("height is 0", tree.height() == 0);
        check("tree is valid", tree.isValid());
    }

    std::cout << "\nTest 2: insert 10 20 5 6 12 30 7 17 (t = 2)" << std::endl;
    {
        BTree tree(2);
        int values[] = {10, 20, 5, 6, 12, 30, 7, 17};
        for (int v : values) {
            tree.insert(v);
        }
        std::cout << "  Tree shape:" << std::endl;
        tree.print();

        std::vector<int> expected = {5, 6, 7, 10, 12, 17, 20, 30};
        check("inorder is 5 6 7 10 12 17 20 30", tree.inorder() == expected);
        check("tree is valid", tree.isValid());
        check("height is 1 (root with three children)", tree.height() == 1);
    }

    std::cout << "\nTest 3: search present and absent keys" << std::endl;
    {
        BTree tree(2);
        int values[] = {10, 20, 5, 6, 12, 30, 7, 17};
        for (int v : values) {
            tree.insert(v);
        }
        check("search(6) finds a key in the tree", tree.search(6) == true);
        check("search(30) finds the largest key", tree.search(30) == true);
        check("search(5) finds the smallest key", tree.search(5) == true);
        check("search(99) is false (absent, too large)", tree.search(99) == false);
        check("search(1) is false (absent, too small)", tree.search(1) == false);
        check("search(11) is false (absent, in the middle)", tree.search(11) == false);
    }

    std::cout << "\nTest 4: duplicate insert is rejected" << std::endl;
    {
        BTree tree(2);
        tree.insert(15);
        bool secondInsert = tree.insert(15);
        check("second insert(15) returns false", secondInsert == false);
        check("inorder has only one 15", tree.inorder().size() == 1);
    }

    std::cout << "\nTest 5: insert 1..20 in ascending order (t = 2)" << std::endl;
    {
        BTree tree(2);
        for (int i = 1; i <= 20; i++) {
            tree.insert(i);
        }
        std::cout << "  Tree shape:" << std::endl;
        tree.print();

        std::vector<int> result = tree.inorder();
        check("inorder has 20 keys", result.size() == 20);
        check("inorder is sorted", isSorted(result));
        check("tree is valid", tree.isValid());
        check("height is 3 or less (a plain BST would be 19)", tree.height() <= 3);
    }

    std::cout << "\nTest 6: insert 20..1 in descending order (t = 2)" << std::endl;
    {
        BTree tree(2);
        for (int i = 20; i >= 1; i--) {
            tree.insert(i);
        }
        check("inorder has 20 keys", tree.inorder().size() == 20);
        check("inorder is sorted", isSorted(tree.inorder()));
        check("tree is valid", tree.isValid());
    }

    std::cout << "\nTest 7: insert 1..100 with t = 3" << std::endl;
    {
        BTree tree(3);
        for (int i = 1; i <= 100; i++) {
            tree.insert(i);
        }
        bool allFound = true;
        for (int i = 1; i <= 100; i++) {
            if (!tree.search(i)) {
                allFound = false;
            }
        }
        check("every key 1..100 is found", allFound);
        check("search(101) is false", tree.search(101) == false);
        check("inorder is sorted with 100 keys", isSorted(tree.inorder()) && tree.inorder().size() == 100);
        check("tree is valid", tree.isValid());
        check("height is 4 or less", tree.height() <= 4);
    }

    std::cout << "\nResults: " << testsPassed << " passed, " << testsFailed << " failed" << std::endl;
    return testsFailed;
}