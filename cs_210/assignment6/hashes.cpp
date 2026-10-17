#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <cstdlib>
#include <ctime>
using namespace std;

const string PEPPER = "pepper123";
const int SLOT_EMPTY = 0;
const int SLOT_USED = 1;
const int SLOT_DELETED = 2;

uint64_t demoHash(const std::string& s) {
    uint64_t h = 1469598103934665603ULL;
    for (unsigned char c : s) {
        h = (h ^ c) * 1099511628211ULL;
    }
    return h;
}

struct UserRecord {
    string username = "";
    string salt = "";
    uint64_t passwordHash = 0;
    int status = SLOT_EMPTY;
};

struct HashTable {
    string strategy = "chaining";
    int capacity = 7;
    int count = 0;
    int collisions = 0;
    int rehashes = 0;
    double maxLoad = 0.5;
    vector<vector<UserRecord>> buckets;
    vector<UserRecord> slots;
};

bool isPrime(int number) {
    if (number < 2) {
        return false;
    }
    for (int divisor = 2; divisor * divisor <= number; divisor++) {
        if (number % divisor == 0) {
            return false;
        }
    }
    return true;
}

int getNextPrime(int number) {
    while (isPrime(number) == false) {
        number++;
    }
    return number;
}

int getStartIndex(string username, int capacity) {
    return demoHash(username) % capacity;
}

int getStepSize(string username, int capacity) {
    return 1 + demoHash(username + "step") % (capacity - 1);
}

int getIndexForAttempt(HashTable& table, string username, int attempt) {
    int start = getStartIndex(username, table.capacity);

    if (table.strategy == "linear probing") {
        return (start + attempt) % table.capacity;
    }
    if (table.strategy == "quadratic probing") {
        return (start + attempt * attempt) % table.capacity;
    }
    int step = getStepSize(username, table.capacity);
    return (start + attempt * step) % table.capacity;
}

void buildEmptyStorage(HashTable& table) {
    table.buckets.clear();
    table.slots.clear();
    if (table.strategy == "chaining") {
        table.buckets.resize(table.capacity);
    } else {
        table.slots.resize(table.capacity);
    }
}

HashTable createTable(string strategy) {
    HashTable table;
    table.strategy = strategy;
    table.capacity = 7;
    if (strategy == "chaining") {
        table.maxLoad = 0.75;
    } else {
        table.maxLoad = 0.5;
    }
    buildEmptyStorage(table);
    return table;
}

double getLoadFactor(HashTable& table) {
    return table.count * 1.0 / table.capacity;
}

bool placeUser(HashTable& table, UserRecord user) {
    if (table.strategy == "chaining") {
        int index = getStartIndex(user.username, table.capacity);
        bool hadCollision = table.buckets[index].size() > 0;
        table.buckets[index].push_back(user);
        return hadCollision;
    }

    for (int attempt = 0; attempt < table.capacity; attempt++) {
        int index = getIndexForAttempt(table, user.username, attempt);
        if (table.slots[index].status != SLOT_USED) {
            user.status = SLOT_USED;
            table.slots[index] = user;
            return attempt > 0;
        }
    }
    return false;
}

vector<UserRecord> getAllUsers(HashTable& table) {
    vector<UserRecord> allUsers;

    if (table.strategy == "chaining") {
        for (int i = 0; i < table.capacity; i++) {
            int bucketSize = table.buckets[i].size();
            for (int j = 0; j < bucketSize; j++) {
                allUsers.push_back(table.buckets[i][j]);
            }
        }
    } else {
        for (int i = 0; i < table.capacity; i++) {
            if (table.slots[i].status == SLOT_USED) {
                allUsers.push_back(table.slots[i]);
            }
        }
    }
    return allUsers;
}

void rehash(HashTable& table) {
    vector<UserRecord> allUsers = getAllUsers(table);
    int oldCapacity = table.capacity;

    table.capacity = getNextPrime(oldCapacity * 2);
    buildEmptyStorage(table);

    for (int i = 0; i < allUsers.size(); i++) {
        placeUser(table, allUsers[i]);
    }

    table.rehashes++;
    cout << "  [rehash] capacity " << oldCapacity << " -> " << table.capacity << endl;
}

UserRecord findUser(HashTable& table, string username) {
    UserRecord notFound;

    if (table.strategy == "chaining") {
        int index = getStartIndex(username, table.capacity);
        int bucketSize = table.buckets[index].size();
        for (int i = 0; i < bucketSize; i++) {
            if (table.buckets[index][i].username == username) {
                return table.buckets[index][i];
            }
        }
        return notFound;
    }

    for (int attempt = 0; attempt < table.capacity; attempt++) {
        int index = getIndexForAttempt(table, username, attempt);
        if (table.slots[index].status == SLOT_EMPTY) {
            return notFound;
        }
        if (table.slots[index].status == SLOT_USED && table.slots[index].username == username) {
            return table.slots[index];
        }
    }
    return notFound;
}

bool insertUser(HashTable& table, UserRecord user) {
    UserRecord existing = findUser(table, user.username);
    if (existing.username != "") {
        return false;
    }

    double loadAfterInsert = (table.count + 1.0) / table.capacity;
    if (loadAfterInsert > table.maxLoad) {
        rehash(table);
    }

    bool hadCollision = placeUser(table, user);
    if (hadCollision) {
        table.collisions++;
    }
    table.count++;
    return true;
}

bool removeUser(HashTable& table, string username) {
    if (table.strategy == "chaining") {
        int index = getStartIndex(username, table.capacity);
        int bucketSize = table.buckets[index].size();
        for (int i = 0; i < bucketSize; i++) {
            if (table.buckets[index][i].username == username) {
                table.buckets[index][i] = table.buckets[index][bucketSize - 1];
                table.buckets[index].pop_back();
                table.count--;
                return true;
            }
        }
        return false;
    }

    for (int attempt = 0; attempt < table.capacity; attempt++) {
        int index = getIndexForAttempt(table, username, attempt);
        if (table.slots[index].status == SLOT_EMPTY) {
            return false;
        }
        if (table.slots[index].status == SLOT_USED && table.slots[index].username == username) {
            table.slots[index].status = SLOT_DELETED;
            table.count--;
            return true;
        }
    }
    return false;
}

string makeSalt() {
    string letters = "abcdefghijklmnopqrstuvwxyz";
    string salt = "";
    for (int i = 0; i < 6; i++) {
        salt = salt + letters[rand() % 26];
    }
    return salt;
}

uint64_t hashPassword(string salt, string password) {
    return demoHash(salt + password + PEPPER);
}

void registerUser(HashTable& table, string username, string password) {
    UserRecord user;
    user.username = username;
    user.salt = makeSalt();
    user.passwordHash = hashPassword(user.salt, password);

    if (insertUser(table, user)) {
        cout << "Registered " << username << endl;
    } else {
        cout << "Could not register " << username << " (name already taken)" << endl;
    }
}

void login(HashTable& table, string username, string password) {
    UserRecord user = findUser(table, username);

    if (user.username == "") {
        cout << "Login failed for " << username << ": unknown user" << endl;
        return;
    }

    uint64_t attemptedHash = hashPassword(user.salt, password);
    if (attemptedHash != user.passwordHash) {
        cout << "Login failed for " << username << ": wrong password" << endl;
        return;
    }

    cout << "Login succeeded for " << username << endl;
}

void runDemo(string strategy) {
    cout << "===== " << strategy << " =====" << endl;
    HashTable table = createTable(strategy);

    registerUser(table, "alice", "apple123");
    registerUser(table, "bob", "banana456");
    registerUser(table, "carol", "cherry789");
    registerUser(table, "dave", "date000");
    registerUser(table, "erin", "elder111");
    registerUser(table, "frank", "fig222");

    login(table, "alice", "apple123");
    login(table, "bob", "wrongpassword");
    login(table, "zed", "anything");

    if (removeUser(table, "carol")) {
        cout << "Removed carol" << endl;
    }
    login(table, "carol", "cherry789");

    cout << "Users: " << table.count << endl;
    cout << "Capacity: " << table.capacity << endl;
    cout << "Load factor: " << getLoadFactor(table) << endl;
    cout << "Collisions: " << table.collisions << endl;
    cout << "Rehashes: " << table.rehashes << endl;
    cout << endl;
}

int main() {
    srand(time(0));

    runDemo("chaining");
    runDemo("linear probing");
    runDemo("quadratic probing");
    runDemo("double hashing");

    return 0;
}