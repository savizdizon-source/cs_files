#include <string>
#include <iostream>
#include <random>

// each node in the list is going to be a Property
struct Property {
    public:
        Property* next;
        Property* previous;
        std::string propertyName;
        bool isBought;
        std::string owner;
        int cost;
    
        Property(std::string val, int amount) {
            propertyName = val;
            next = nullptr;
            previous = nullptr;
            isBought = false;
            owner = "";
            cost = amount;
        }
};

struct Player {

    public: 
        Property* position;
        std::string name;
        int money;

        Player(Property* pos, std::string playerName) {
            position = pos;
            name = playerName;
            money = 1500;
        }

        void move() {
            position = position->next;
            std::cout << name << " is on " << position->propertyName << std::endl;
        }

        void purchaseProperty(Property* prop) {
            std::cout << name << " would like to purchase " << prop->propertyName << "." << std::endl; 
            if (prop->isBought == false && money >= prop->cost) {
                prop->owner = name;
                prop->isBought = true;
                std::cout << name << " bought " << prop->propertyName << std::endl;
                money = money - prop->cost;
            } else if (money < prop->cost) {
                std::cout << "Cannot purchase " << prop->propertyName << " due to insufficient funds." << std::endl;
            } else if (prop->owner != "") {
                std::cout << "Property already owned by " << prop->owner << "." << std::endl;
            }
        }

};

struct Gameboard { // the circular linked list

        Property* start;
        Property* end;

        Gameboard() {
            start = nullptr;
            end = nullptr;
        }


        void insert(Property* curr, Property* newNode) {
            if (start == nullptr) {
                start = newNode;
                end = newNode;
                newNode->next = newNode;
                newNode->previous = newNode;
            } else if (curr == end) {
                newNode->previous = end;
                newNode->next = start;
                end->next = newNode;
                start->previous = newNode;
                end = newNode;
                
            } else {
                newNode->next = curr->next;
                newNode->previous = curr; 
                curr->next->previous = newNode;
                curr->next = newNode;              
            }
        }
};

int main() {

    Gameboard* board = new Gameboard();

    // 10 properties on the board (in the circuluar linked list)
    board->insert(board->end, new Property("baltic", 60));
    board->insert(board->end, new Property("mediterranean", 60));
    board->insert(board->end, new Property("oriental", 100));
    board->insert(board->end, new Property("vermont", 100));
    board->insert(board->end, new Property("virginia", 160));
    board->insert(board->end, new Property("atlantic", 260));
    board->insert(board->end, new Property("ventnor", 260));
    board->insert(board->end, new Property("pacific", 300));
    board->insert(board->end, new Property("carolina", 300));
    board->insert(board->end, new Property("boardwalk", 400));

    // 4 players
    Player* p1 = new Player(board->start, "p1");
    Player* p2 = new Player(board->start, "p2");
    Player* p3 = new Player(board->start, "p3");
    Player* p4 = new Player(board->start, "p4");

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distrib(1, 12);

    //int dice_roll = distrib(gen);

    // Turn 1
    std::cout << "Turn 1:" << std::endl;
    for (int i = 0; i < distrib(gen); i++) {
        p1->move();    
    }
    p1->purchaseProperty(p1->position);

    // Turn 2
    std::cout << "Turn 2:" << std::endl;
    for (int i = 0; i < distrib(gen); i++) {
        p2->move();    
    }
    p2->purchaseProperty(p2->position);

    // Turn 3
    std::cout << "Turn 3:" << std::endl;
    for (int i = 0; i < distrib(gen); i++) {
        p3->move();    
    }
    p3->purchaseProperty(p3->position);

    // Turn 4
    std::cout << "Turn 4:" << std::endl;
    for (int i = 0; i < distrib(gen); i++) {
        p4->move();    
    }
    p4->purchaseProperty(p4->position);

    // Turn 5
    std::cout << "Turn 5:" << std::endl;
    for (int i = 0; i < distrib(gen); i++) {
        p1->move();    
    }
    p1->purchaseProperty(p1->position);

    // Turn 6
    std::cout << "Turn 6:" << std::endl;
    for (int i = 0; i < distrib(gen); i++) {
        p2->move();    
    }
    p2->purchaseProperty(p2->position);

    // Turn 7
    std::cout << "Turn 7:" << std::endl;
    for (int i = 0; i < distrib(gen); i++) {
        p3->move();    
    }
    p3->purchaseProperty(p3->position);

    // Turn 8
    std::cout << "Turn 8:" << std::endl;
    for (int i = 0; i < distrib(gen); i++) {
        p4->move();    
    }
    p4->purchaseProperty(p4->position);

    // Turn 9
    std::cout << "Turn 9:" << std::endl;
    for (int i = 0; i < distrib(gen); i++) {
        p1->move();    
    }
    p1->purchaseProperty(p1->position);

    // Turn 10
    std::cout << "Turn 10:" << std::endl;
    for (int i = 0; i < distrib(gen); i++) {
        p2->move();    
    }
    p2->purchaseProperty(p2->position);

}