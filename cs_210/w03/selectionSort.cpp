#include <iostream>
#include <string>
#include <vector>
using namespace std;

void selectionSort(int* numbers, int numbersSize) {
    for (int i = 0; i < numbersSize - 1; i++) {
        int smallestIndex = i;
        for (int j = i + 1; j < numbersSize; j++) {
            if (numbers[j] < numbers[smallestIndex]) {
                smallestIndex = j;
            }
        }

        int temp = numbers[i];
        numbers[i] = numbers[smallestIndex];
        numbers[smallestIndex] = temp;
    }
}

int main() {
    vector<int> arr = {4, 12, 66, 1, 5, 17};
    selectionSort(arr.data(), arr.size());

    for (auto& val : arr) {
        cout << val << endl;
    }
}