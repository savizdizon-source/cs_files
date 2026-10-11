#include <string>
#include <iostream>
#include <vector>

class hashTable {
    std::vector<std::vector<std::string>> table; // table is a vector whose items are all vectors (for collisions)
    int numBuckets;

    public: 
        int HashString(const std::string& word) {
            int hashCode = 0;
            for (int i = 0; i < word.length(); i++) {
                hashCode += word[i];
            }

            return hashCode;
        }

        hashTable() {
           int buckets = 10;
           table.resize(buckets);
           numBuckets = buckets; 
        }

        void Insert(const std::string& word) {
            table[HashString(word) % numBuckets].push_back(word);
        }

        void Print() {
            for (int i = 0; i < numBuckets; i++) {
                std::cout << i << ": ";
                for (std::string& item : table[i]) {
                    std::cout << item << " ";
                }
                std::cout << std::endl;
            }
        }
};

int main() {
    hashTable table;
    table.Insert("Hello");
    table.Insert("olleH"); // potential collision
    table.Insert("Bye");
    table.Insert("Morning");
    table.Insert("Aloe");

    table.Print();
}