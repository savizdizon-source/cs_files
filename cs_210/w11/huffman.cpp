#include <iostream>
#include <string>
#include <map>
#include <queue>
#include <vector>
using namespace std;

struct Node
{
    char character;
    int frequency;

    Node* left;
    Node* right;

    Node(char c, int f)
    {
        character = c;
        frequency = f;
        left = nullptr;
        right = nullptr;
    }

    Node(int f, Node* leftNode, Node* rightNode)
    {
        character = '\0';
        frequency = f;
        left = leftNode;
        right = rightNode;
    }
};

struct Compare
{
    bool operator()(Node* first, Node* second)
    {
        return first->frequency > second->frequency;
    }
};


void createCodes(Node* node, string code, map<char, string>& codes)
{
    if (node == nullptr)
    {
        return;
    }

    if (node->left == nullptr && node->right == nullptr)
    {
        if (code == "")
        {
            code = "0";
        }

        codes[node->character] = code;
        return;
    }

    // Going left adds a 0.
    createCodes(node->left, code + "0", codes);

    // Going right adds a 1.
    createCodes(node->right, code + "1", codes);
}

void deleteTree(Node* node)
{
    if (node == nullptr)
    {
        return;
    }

    deleteTree(node->left);
    deleteTree(node->right);

    delete node;
}


int main()
{
    string text;

    cout << "Enter text to encode: ";
    getline(cin, text);

    if (text.empty())
    {
        cout << "No text was entered." << endl;
        return 0;
    }

    // how often does a character appear
    map<char, int> frequencies;

    for (char c : text)
    {
        frequencies[c]++;
    }

    // adding to priority queue
    priority_queue<Node*, vector<Node*>, Compare> nodes;

    for (auto item : frequencies)
    {
        char character = item.first;
        int frequency = item.second;

        Node* newNode = new Node(character, frequency);

        nodes.push(newNode);
    }

    while (nodes.size() > 1)
    {
        // Get the node with the smallest frequency
        Node* left = nodes.top();
        nodes.pop();

        // Get the node with the next smallest frequency
        Node* right = nodes.top();
        nodes.pop();

        // Combine their frequencies
        int combinedFrequency =
            left->frequency + right->frequency;
        Node* parent =
            new Node(combinedFrequency, left, right);

        // Put the new combined node back into the queue
        nodes.push(parent);
    }

    Node* root = nodes.top();

    map<char, string> codes;

    createCodes(root, "", codes);

    string encodedText = "";

    for (char c : text)
    {
        encodedText += codes[c];
    }

    cout << endl;
    cout << "Character Frequencies:" << endl;

    for (auto item : frequencies)
    {
        if (item.first == ' ')
        {
            cout << "[space]";
        }
        else
        {
            cout << item.first;
        }

        cout << ": " << item.second << endl;
    }


    cout << endl;
    cout << "Huffman Codes:" << endl;

    for (auto item : codes)
    {
        if (item.first == ' ')
        {
            cout << "[space]";
        }
        else
        {
            cout << item.first;
        }

        cout << ": " << item.second << endl;
    }


    cout << endl;
    cout << "Original text: " << text << endl;
    cout << "Encoded text:  " << encodedText << endl;

    deleteTree(root);

    return 0;
}