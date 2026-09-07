#include <iostream>
#include <vector>
#include <unordered_map>
using namespace std;

/**
 * & = pass by reference
 *      
 */

vector<int> twoSum(const vector<int>& nums, int target) {
    unordered_map<int, int> index; // map from int to int

    for (int i = 0; i < nums.size(); ++i) {
        int needed = target - nums[i];

        if (index.count(needed)) {
            return {index[needed], i};
        }

        index[nums[i]] = i;
    }

    return {}; // no solution found, vector literal
}

int main() {
    vector<int> nums = {2, 7, 11, 15};
    int target = 9;

    vector<int> answer = twoSum(nums, target);

    if (!answer.empty()) {
        cout << "Indices: [" << answer[0]
             << ", " << answer[1] << "]\n";
    } else {
        cout << "No solution\n";
    }
}