#include <iostream>
#include <vector>
using namespace std;


int main() {
    vector<int> nums = {2, 7, 11, 15};

    // unsorted vector = most efficient way is linear search O(n)
    int target = 7;

    for (int i = 0; i < nums.size(); ++i) {
        int value = nums[i];
        
        if (value == target) {
            cout << i;
        }
    }
}

/**
 * Big O Complexity
 * 
 */